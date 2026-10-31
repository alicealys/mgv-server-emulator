#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "player_names.hpp"
#include "command.hpp"
#include "../server.hpp"

#include "database/models/users.hpp"

#include <utils/io.hpp>
#include <utils/string.hpp>
#include <utils/http.hpp>

struct steam_id_response_t
{
	struct response_t
	{
		struct player_t
		{
			std::string steamid;
			std::string personaname;
		};

		std::vector<player_t> players;
	};

	response_t response;
};

namespace player_names
{
	namespace
	{
		struct player_name_t
		{
			std::uint64_t account_id;
			std::string name;
			std::chrono::system_clock::time_point last_update;
		};
		
		struct state_t
		{
			std::unordered_map<std::uint64_t, player_name_t> player_list;
		};

		struct
		{
			utils::concurrency::container<state_t> state;
			std::thread thread;
			std::atomic_bool killed;
			std::size_t error_count;
		} runner{};

		void run_update()
		{
			const auto current_list = runner.state.access<std::unordered_map<std::uint64_t, player_name_t>>([&](state_t& state)
			{
				return state.player_list;
			});

			const auto account_id_list = database::users::get_account_id_list();

			std::vector<std::uint64_t> to_update_list;
			const auto now = std::chrono::system_clock::now();
			for (auto& entry : account_id_list)
			{
				if (entry == 0)
				{
					continue;
				}

				const auto iter = current_list.find(entry);
				if (iter == current_list.end() || (iter->second.name.empty() || (now - iter->second.last_update) > 1h))
				{
					to_update_list.emplace_back(entry);
				}

				if (to_update_list.size() > 100)
				{
					break;
				}
			}

			if (to_update_list.empty())
			{
				return;
			}

			std::string steam_id_list;
			for (const auto& steam_id : to_update_list)
			{
				steam_id_list.append(std::format("{},", steam_id));
			}

			const auto& cfg = config::get();
			const auto url = "https://api.steampowered.com/ISteamUser/GetPlayerSummaries/v0002/?key={}&steamids={}";
			const auto formatted = std::format(url, cfg.steam_api_key, steam_id_list);
			const auto data = utils::http::get_data(formatted);

			if (!data.has_value())
			{
				runner.error_count++;
				return;
			}

			steam_id_response_t response{};
			if (!json::read(response, data.value()))
			{
				runner.error_count++;
				return;
			}

			runner.error_count = 0;

			const auto now2 = std::chrono::system_clock::now();
			runner.state.access([&](state_t& state)
			{
				for (auto& entry : response.response.players)
				{
					const auto steam_id = std::strtoull(entry.steamid.data(), nullptr, 0);
					auto& player = state.player_list[steam_id];
					player.last_update = now2;
					player.name = entry.personaname;
				}
			});
		}
	}

	std::string get(const std::uint64_t account_id)
	{
		return runner.state.access<std::string>([&](state_t& state)
			-> std::string
		{
			const auto iter = state.player_list.find(account_id);
			if (iter == state.player_list.end())
			{
				return "[unknown]";
			}

			return iter->second.name;
		});
	}

	class component final : public component_interface
	{
	public:
		void post_start() override
		{
			const auto& cfg = config::get();
			if (cfg.steam_api_key.empty())
			{
				return;
			}

			runner.thread = std::thread([]
			{
				while (!runner.killed)
				{
					run_update();
					std::this_thread::sleep_for(500ms * std::min(std::size_t(10u), runner.error_count));
				}
			});
		}

		void pre_destroy() override
		{
			runner.killed = true;
			if (runner.thread.joinable())
			{
				runner.thread.join();
			}
		}
	};
}

REGISTER_COMPONENT(player_names::component)
