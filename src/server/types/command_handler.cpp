#include <std_include.hpp>

#include "command_handler.hpp"

namespace emulator
{
	json::value error(const std::string& id)
	{
		json::value result;
		result["result"] = id;
		return result;
	}

	json::value error(const std::uint32_t id)
	{
		json::value result;
		result["result"] = game::get_error(id);
		return result;
	}

	json::value resource(const std::uint32_t id)
	{
		auto resource = utils::resources::load_json(id);
		return resource;
	}

	json::value player_info(const std::uint64_t player_id, const std::uint64_t account_id)
	{
		json::value info;

		info["npid"]["handler"]["data"] = "";
		info["npid"]["handler"]["dummy"] = json::value::array_t{0, 0, 0};
		info["npid"]["handler"]["term"] = 0;
		info["npid"]["opt"] = json::value::array_t{0, 0, 0, 0, 0, 0, 0, 0};
		info["npid"]["reserved"] = json::value::array_t{0, 0, 0, 0, 0, 0, 0, 0};
		info["player_id"] = player_id;
		info["player_name"] = account_id == 0 ? "NotImplement" : std::format("{}_player01", account_id);
		info["ugc"] = player_id != 0 ? 1 : 0;
		info["xuid"] = account_id;

		return info;
	}

	json::value player_info(const database::players::player& player)
	{
		return player_info(player.get_player_id(), player.get_account_id());
	}

	json::value player_info(const std::optional<database::players::player>& player)
	{
		return player_info(*player);
	}

	void merge_json(json::value& data, const json::value& extra_data)
	{
		if (!extra_data.is_object())
		{
			return;
		}

		for (const auto& [k, v] : extra_data.get_object())
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
