#include <std_include.hpp>

#include "cmd_base_resource_draw_water.hpp"

namespace emulator::ssd
{
	json::value cmd_base_resource_draw_water::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto base_resources = std::make_unique<database::players::base_resources_t>();
		user->current_player->get_base_resources(*base_resources);

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		const auto consume = [&](std::uint32_t& dest, const std::uint32_t amount)
		{
			if (amount > dest)
			{
				return false;
			}

			dest -= amount;
			return true;
		};

		if (!consume(base_resources->params.clean_water, param.clean_water) ||
			!consume(base_resources->params.dirty_water, param.dirty_water))
		{
			return error(ERR_RESOURCE_SHORTAGE);
		}

		user->current_player->set_base_resources(*base_resources);

		return result;
	}

	std::uint32_t cmd_base_resource_draw_water::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
