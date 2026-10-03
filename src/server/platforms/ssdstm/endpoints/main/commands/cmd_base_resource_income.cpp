#include <std_include.hpp>

#include "cmd_base_resource_income.hpp"

namespace emulator::ssd
{
	json::value cmd_base_resource_income::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto base_resources = std::make_unique<database::players::base_resources_t>();
		user->current_player->get_base_resources(*base_resources);

		database::players::base_resources_t::base_resource_params_t diff{};
		if (!json::read(diff, data["base_resource"]))
		{
			return error(ERR_INVALIDARG);
		}

		const auto income = [&](std::uint32_t& dest, const std::uint32_t amount)
		{
			const auto capped_amount = std::min(std::numeric_limits<std::uint32_t>::max() - dest, amount);
			dest += capped_amount;
		};

		income(base_resources->params.bad_status_1_risk, diff.bad_status_1_risk);
		income(base_resources->params.bad_status_2_risk, diff.bad_status_2_risk);
		income(base_resources->params.bad_status_3_risk, diff.bad_status_3_risk);
		income(base_resources->params.bad_status_4_risk, diff.bad_status_4_risk);
		income(base_resources->params.clean_water, diff.clean_water);
		income(base_resources->params.dirty_water, diff.dirty_water);
		income(base_resources->params.food, diff.food);
		income(base_resources->params.medical_supplies, diff.medical_supplies);

		user->current_player->set_base_resources(*base_resources);

		return result;
	}

	std::uint32_t cmd_base_resource_income::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
