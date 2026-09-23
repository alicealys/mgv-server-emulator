#include <std_include.hpp>

#include "cmd_present_box_delete_new_flag.hpp"

#include "database/models/present_box.hpp"

namespace emulator::ssd
{
	json::value cmd_present_box_delete_new_flag::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		auto& id_list_j = data["id_list"];
		if (!id_list_j.is_array())
		{
			return result;
		}

		for (auto i = 0ull; i < id_list_j.size(); i++)
		{
			std::uint64_t present_id{};
			if (!json::read(present_id, id_list_j[i]))
			{
				continue;
			}

			database::present_box::remove_new_flag(present_id);
		}

        return result;
	}

	std::uint32_t cmd_present_box_delete_new_flag::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
