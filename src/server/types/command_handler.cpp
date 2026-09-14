#include <std_include.hpp>

#include "command_handler.hpp"

namespace emulator
{
	glz::json error(const std::string& id)
	{
		glz::json result;
		result["result"] = id;
		return result;
	}

	glz::json error(const std::uint32_t id)
	{
		glz::json result;
		result["result"] = game::get_error(id);
		return result;
	}

	glz::json resource(const std::uint32_t id)
	{
		auto resource = utils::resources::load_json(id);
		return resource;
	}

	glz::json player_info(const std::uint64_t player_id, const std::uint64_t account_id)
	{
		glz::json info;

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

	glz::json player_info(const database::players::player& player)
	{
		return player_info(player.get_player_id(), player.get_account_id());
	}

	glz::json player_info(const std::optional<database::players::player>& player)
	{
		return player_info(*player);
	}

	void merge_json(glz::json& data, const glz::json& extra_data)
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
