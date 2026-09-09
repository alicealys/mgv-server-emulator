#include <std_include.hpp>

#include "cmd_craft_item.hpp"

#include "game/parameters.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_craft_item::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		auto& craft_item_param_j = data["craft_item_param"];
		if (!craft_item_param_j.is_object())
		{
			return error(ERR_INVALIDARG);
		}

		auto& ammo_count_j = craft_item_param_j["ammo_count"];
		auto& ammo_id_j = craft_item_param_j["ammo_id"];
		auto& craft_num_j = craft_item_param_j["craft_num"];
		auto& group_level_j = craft_item_param_j["group_level"];
		auto& group_skills_j = craft_item_param_j["group_skills"];
		auto& inventory_index_junk_j = craft_item_param_j["inventory_index_junk"];
		auto& recipe_id_j = craft_item_param_j["recipe_id"];

		if (!ammo_count_j.is_number_unsigned() || !ammo_id_j.is_number_unsigned() ||
			!craft_num_j.is_number_unsigned() || !group_level_j.is_number_unsigned() ||
			!group_skills_j.is_array() || !inventory_index_junk_j.is_number_unsigned() ||
			!recipe_id_j.is_number_unsigned())
		{
			return error(ERR_INVALIDARG);
		}

		const auto craft_num = std::min(std::uint16_t(8u), craft_num_j.get<std::uint16_t>());
		const auto recipe_id = recipe_id_j.get<std::uint32_t>();

		const auto iter = game::parameters_table.ssd_sbm_parameters->recipes.find(recipe_id);
		if (iter == game::parameters_table.ssd_sbm_parameters->recipes.end())
		{
			return error(ERR_INVALIDARG);
		}

		// todo: check if player can craft

		database::players::nonstackable_item_t nonstackable_item{};
		if (!iter->second->production->countable)
		{
			nonstackable_item.life = static_cast<std::uint16_t>(iter->second->production->life);
			nonstackable_item.life_max = static_cast<std::uint16_t>(iter->second->production->life);
			nonstackable_item.production_id = iter->second->production->id;
			nonstackable_item.spec = 1000; // ?
			nonstackable_item.flag = 1; // ?
		}
		else
		{
			database::players::stackable_item_t stackable_item{};
			stackable_item.production_id = iter->second->production->id;
			stackable_item.flag = 1;
			stackable_item.count = craft_num;
			stackable_item.inventory_index = 0; // ?
			stackable_item.to_json(result["crafted_stackable_items"][0]);
		}

		nonstackable_item.to_json(result["crafted_nonstackable_item"]);

		result["inventory_index_junk"] = 0xFFFF;
		result["left_resources"] = nlohmann::json::array();

        return result;
	}

	std::uint32_t cmd_craft_item::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
