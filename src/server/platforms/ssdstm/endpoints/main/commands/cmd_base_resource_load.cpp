#include <std_include.hpp>

#include "cmd_base_resource_load.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_base_resource_load::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		const auto base_resources = std::make_unique<database::players::base_resources_t>();
		user->current_player->get_base_resources(*base_resources);
		base_resources->to_json(result);

        return result;
	}

	std::uint32_t cmd_base_resource_load::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
