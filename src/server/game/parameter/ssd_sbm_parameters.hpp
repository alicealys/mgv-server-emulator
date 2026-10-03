#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_sbm_parameters final : public base_parameter
	{
	public:
		ssd_sbm_parameters();
		bool parse(json::value& data) override;

		std::uint32_t get_resource_id(const std::uint32_t resource_index) const;
		std::uint32_t get_production_id(const std::uint32_t production_index) const;
		std::uint32_t get_battle_pack_id(const std::uint32_t production_index) const;

		std::unordered_map<std::uint32_t, std::shared_ptr<resource_t>> resources;
		std::unordered_map<std::uint32_t, std::shared_ptr<production_t>> productions;
		std::unordered_map<std::uint32_t, std::shared_ptr<recipe_t>> recipes;
		std::unordered_map<std::uint32_t, std::shared_ptr<accessory_t>> accessories;
		std::unordered_map<std::uint32_t, std::shared_ptr<battle_pack_t>> battle_packs;
		std::unordered_map<std::uint32_t, std::shared_ptr<survival_gear_t>> survival_gears;
		std::unordered_map<std::uint32_t, std::shared_ptr<customize_option_group_t>> customize_option_group;
		std::unordered_map<std::uint32_t, std::shared_ptr<customize_option_t>> customize_option;
		std::unordered_map<std::uint32_t, std::shared_ptr<customize_t>> customize;
		std::unordered_map<std::uint32_t, std::shared_ptr<cassette_t>> cassettes;
		std::unordered_map<std::uint32_t, std::shared_ptr<potential_t>> potentials;
		std::array<gradeup_spec_t, 255> gradeup_spec;
		std::vector<std::shared_ptr<production_t>> productions_list;
		std::vector<std::shared_ptr<recipe_t>> recipes_list;
		std::vector<std::shared_ptr<resource_t>> resources_list;
		std::vector<std::shared_ptr<battle_pack_t>> battle_pack_list;
		std::vector<std::shared_ptr<cassette_t>> cassettes_list;

	};
}
