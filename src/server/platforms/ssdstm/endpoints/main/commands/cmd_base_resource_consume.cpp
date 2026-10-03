#include <std_include.hpp>

#include "cmd_base_resource_consume.hpp"

namespace emulator::ssd
{
	json::value cmd_base_resource_consume::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto base_resources = std::make_unique<database::players::base_resources_t>();
		user->current_player->get_base_resources(*base_resources);

		database::players::base_resources_t::base_resource_params_t diff{};
		if (!json::read(diff, data["base_resource"]))
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

		if (!consume(base_resources->params.clean_water, diff.clean_water) ||
			!consume(base_resources->params.dirty_water, diff.dirty_water) ||
			!consume(base_resources->params.food, diff.food) ||
			!consume(base_resources->params.medical_supplies, diff.medical_supplies))
		{
			return error(ERR_RESOURCE_SHORTAGE);
		}

		user->current_player->set_base_resources(*base_resources);

		return result;
	}

	std::uint32_t cmd_base_resource_consume::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
