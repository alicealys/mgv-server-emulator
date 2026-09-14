#include <std_include.hpp>

#include "cmd_base_resource_load.hpp"

namespace emulator::ssd
{
	glz::json cmd_base_resource_load::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;

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
