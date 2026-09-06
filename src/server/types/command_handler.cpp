#include <std_include.hpp>

#include "command_handler.hpp"

namespace emulator
{
	nlohmann::json error(const std::string& id)
	{
		nlohmann::json result;
		result["result"] = id;
		return result;
	}

	nlohmann::json error(const std::uint32_t id)
	{
		nlohmann::json result;
		result["result"] = game::get_error(id);
		return result;
	}

	nlohmann::json resource(const std::uint32_t id)
	{
		auto resource = utils::resources::load_json(id);
		return resource;
	}

	nlohmann::json player_info(const std::uint64_t player_id, const std::uint64_t account_id)
	{
		nlohmann::json info;

		info["npid"]["handler"]["data"] = "";
		info["npid"]["handler"]["dummy"] = {0, 0, 0};
		info["npid"]["handler"]["term"] = 0;
		info["npid"]["opt"] = {0, 0, 0, 0, 0, 0, 0, 0};
		info["npid"]["reserved"] = {0, 0, 0, 0, 0, 0, 0, 0};
		info["player_id"] = player_id;
		info["player_name"] = account_id == 0 ? "NotImplement" : std::format("{}_player01", account_id);
		info["ugc"] = player_id != 0 ? 1 : 0;
		info["xuid"] = account_id;

		return info;
	}

	nlohmann::json player_info(const database::players::player& player)
	{
		return player_info(player.get_id(), player.get_account_id());
	}

	nlohmann::json player_info(const std::optional<database::players::player>& player)
	{
		return player_info(*player);
	}

	void merge_json(nlohmann::json& data, const nlohmann::json& extra_data)
	{
		if (!extra_data.is_object())
		{
			return;
		}

		for (const auto& [k, v] : extra_data.items())
		{
			if (v.is_object())
			{
				if (data[k].is_object())
				{
					merge_json(data[k], v);
				}
				else
				{
					data[k] = v;
				}
			}
			else
			{
				data[k] = v;
			}
		}
	}
}
