#include <std_include.hpp>

#include "cmd_craft_deliver.hpp"

namespace emulator::ssd
{
	json::value cmd_craft_deliver::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

        param_t param{};
        if (!json::read(param, data))
        {
            return error(ERR_INVALIDARG);
        }

		const auto sub_item = []<typename T>(T & list, const std::uint16_t inventory_index, const std::uint8_t inventory_type, std::uint16_t& num)
		{
			if (inventory_index == 0xFFFF)
			{
				return;
			}

			const auto entry = list.get_entry(inventory_index, inventory_type);
			if (entry == nullptr)
			{
				return;
			}

			const auto amount = std::min(static_cast<std::uint16_t>(entry->count), num);
			entry->count -= amount;
			num -= amount;
		};

		switch (param.list_type)
		{
		case 0:
		{
			const auto resource_list = std::make_unique<database::players::inventory_resource_list_t>();
			user->current_player->get_inventory_resource_list(*resource_list);

			sub_item(*resource_list, param.inventory_index, param.inventory_type, param.num);
			sub_item(*resource_list, param.inventory_index2, param.inventory_type2, param.num);

			if (param.num > 0)
			{
				return error(ERR_RESOURCE_SHORTAGE);
			}

			user->current_player->set_inventory_resource_list(*resource_list);
			break;
		}
		case 1:
		{
			const auto stackable_list = std::make_unique<database::players::stackable_item_list_t>();
			user->current_player->get_stackable_item_list(*stackable_list);

			sub_item(*stackable_list, param.inventory_index, param.inventory_type, param.num);
			sub_item(*stackable_list, param.inventory_index2, param.inventory_type2, param.num);

			if (param.num > 0)
			{
				return error(ERR_RESOURCE_SHORTAGE);
			}

			user->current_player->set_stackable_item_list(*stackable_list);
			break;
		}
		default:
			return error(ERR_INVALIDARG);
		}

		const auto base_resources = std::make_unique<database::players::base_resources_t>();
		user->current_player->get_base_resources(*base_resources);

		const auto add_value = [](std::uint16_t& dst, std::int16_t src)
		{
			if (src < 0)
			{
				dst -= std::min(dst, static_cast<std::uint16_t>(src * -1));
			}
			else
			{
				dst += std::min(
					static_cast<std::uint16_t>(std::numeric_limits<std::uint16_t>::max() - dst),
					static_cast<std::uint16_t>(src));
			}
		};


		add_value(base_resources->params.bad_status_1_risk, param.bad_status_1_risk);
		add_value(base_resources->params.bad_status_2_risk, param.bad_status_2_risk);
		add_value(base_resources->params.bad_status_3_risk, param.bad_status_3_risk);
		add_value(base_resources->params.bad_status_4_risk, param.bad_status_4_risk);
		add_value(base_resources->params.food, param.food);
		add_value(base_resources->params.clean_water, param.clean_water);
		add_value(base_resources->params.medical_supplies, param.medical_supplies);

		user->current_player->set_base_resources(*base_resources);

        result["gain_resources"] = json::array();
        result["gain_stackables"] = json::array();

        return result;
	}

	std::uint32_t cmd_craft_deliver::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
