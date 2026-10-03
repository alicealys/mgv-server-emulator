#include <std_include.hpp>

#include "game.hpp"
#include "parameters.hpp"

#include "utils/resources.hpp"
#include "utils/json_utils.hpp"

template <>
struct glz::meta<game::customize_option_group_t::option_t>
{
	using T = game::customize_option_group_t::option_t;
	static constexpr auto value = glz::object(
		"optid", &T::optid,
		"obtained", &T::obtained,
		"level", &T::level
	);
};

template <>
struct glz::meta<game::customize_option_t>
{
	using T = game::customize_option_t;
	static constexpr auto value = glz::object(
		"id_str", &T::id_str,
		"id", &T::id,
		"type_id", &T::type_id,
		"relatedId1", &T::related_id1,
		"relatedId2", &T::related_id2,
		"valueu1", &T::value_u1,
		"valuef1", &T::value_f1,
		"price", &T::price,
		"cost", &T::cost,
		"lang_name", &T::lang_name,
		"lang_name2", &T::lang_name2,
		"lang_name3", &T::lang_name3,
		"lang_info", &T::lang_info,
		"icon_path", &T::icon_path
	);
};

template <>
struct glz::meta<game::customize_t>
{
	using T = game::customize_t;
	static constexpr auto modify = glz::object(
		"optionSlotsMin", &T::option_slots_min,
		"optionSlots", &T::option_slot_ids
	);
};

template <>
struct glz::meta<game::recipe_t>
{
	using T = game::recipe_t;
	static constexpr auto value = glz::object(
		"id", &T::id,
		"id_str", &T::id_str,
		"index", &T::index,
		"group", &T::group,
		"dev_level", &T::dev_level,
		"opened", &T::opened,
		"production", &T::production_id,
		"count", &T::count,
		"leftover", &T::leftover,
		"transTime", &T::trans_time,
		"junk", &T::junk,
		"potentialId", &T::potential_id,
		"multiCraft", &T::multi_craft,
		"price", &T::price,
		"cost", &T::cost
	);
};

template <>
struct glz::meta<game::production_t>
{
	using T = game::production_t;
	static constexpr auto value = glz::object(
		"id", &T::id,
		"index", &T::index,
		"type", &T::type,
		"craftCategory", &T::craft_category,
		"rarity", &T::rarity,
		"lv_need", &T::lv_need,
		"stock", &T::stock,
		"life", &T::life,
		"customize", &T::customize_id,
		"conv_use", &T::conv_use,
		"base_c_water", &T::base_c_water,
		"base_food", &T::base_food,
		"base_medicine", &T::base_medicine,
		"sup_combat", &T::sup_combat,
		"sup_survival", &T::sup_survival,
		"related_id", &T::related_id,
		"attr_icon", &T::attr_icon,
		"material", &T::material,
		"only_flag", &T::only_flag,
		"countable", &T::countable,
		"eqp_hip", &T::eqp_hip,
		"eqp_back", &T::eqp_back,
		"eqp_sec", &T::eqp_sec,
		"eqp_sup", &T::eqp_sup,
		"pri_r_slot", &T::pri_r_slot,
		"show_info", &T::show_info,
		"weight", &T::weight,
		"crew_inj_food_poison", &T::crew_inj_food_poison,
		"crew_inj_dysentery", &T::crew_inj_dysentery,
		"crew_inj_physical", &T::crew_inj_physical,
		"crew_inj_fatigue", &T::crew_inj_fatigue,
		"prv_r_x", &T::prv_r_x,
		"prv_r_y", &T::prv_r_y,
		"prv_r_y_min", &T::prv_r_y_min,
		"prv_r_y_max", &T::prv_r_y_max,
		"prv_r_z_off", &T::prv_r_z_off,
		"prv_zm_min", &T::prv_zm_min,
		"prv_zm", &T::prv_zm,
		"prv_zm_max", &T::prv_zm_max,
		"lang_name", &T::lang_name,
		"lang_name2", &T::lang_name2,
		"lang_name3", &T::lang_name3,
		"lang_info", &T::lang_info,
		"icon_path", &T::icon_path,
		"perk", &T::perk,
		"id_str", &T::id_str
	);
};

template <>
struct glz::meta<game::crew_member_type_t>
{
	using T = game::crew_member_type_t;
	static constexpr auto modify = glz::object(
		"DEVELOP", &T::develop,
		"FOOD", &T::food,
		"MEDIC", &T::medic,
		"FARM", &T::farm,
		"BASE_DEFENSE", &T::base_defense,
		"COMBAT_DEPLOY", &T::combat_deploy,
		"THIRST_RESIST", &T::thirst_resist,
		"HUNGER_RESIST", &T::hunger_resist,
		"INJURY_RESIST", &T::injury_resist,
		"SICK_RESIST", &T::sick_resist,
		"SLEEPLACK_RESIST", &T::sleeplack_resist,
		"LIFE", &T::life
	);
};

template <>
struct glz::meta<game::survival_gear_t>
{
	using T = game::survival_gear_t;
	static constexpr auto modify = glz::object(
		"sveId", &T::id,
		"typeId", &T::type_id,
		"targetId", &T::target_id,
		"valueu1", &T::value_u1
	);
};

template <>
struct glz::meta<game::defense_mission_settings_t::rank_reward_info_t>
{
	using T = game::defense_mission_settings_t::rank_reward_info_t;
	static constexpr auto value = glz::object(
		"line_info", &T::line_info,
		"rank", &T::rank
	);
};

template <>
struct glz::meta<game::defense_mission_settings_t>
{
	using T = game::defense_mission_settings_t;
	static constexpr auto value = glz::object(
		"waveCountMax", &T::max_wave_count,
		"waveTime", &T::wave_time,
		"interval", &T::interval,
		"missionId", &T::mission_id,
		"missionType", &T::mission_type,
		"rankRewardThreashold", &T::rank_threshold
	);
};

namespace game
{
	// server

	namespace
	{
		// mgv.exe fox::ncl::NclHttpCodec::EndEncode
		std::uint8_t static_key[16] = {0x17, 0xF0, 0xF7, 0xA6, 0x2E, 0x8F, 0xEE, 0x67, 0x78, 0x66, 0x79, 0xD0, 0x8C, 0x8A, 0x0A, 0x1C};
		std::uint8_t static_key_tpp[16] = {0xD8, 0x89, 0x0A, 0xF0, 0x66, 0xC9, 0x6B, 0x40, 0xD7, 0x01, 0xAE, 0xFC, 0x43, 0x6F, 0xF9, 0xFE};

		mission_list_t load_mission_list()
		{
			auto list = utils::resources::load_json(RESOURCE_MISSION_LIST);
			
			const auto parse_part = [](json::value& data, std::vector<std::uint32_t>& list, std::unordered_map<std::uint32_t, std::int32_t>& map)
			{
				if (!data.is_array())
				{
					return;
				}

				for (auto i = 0ull; i < data.size(); i++)
				{
					mission_list_t::entry_t entry{};
					if (!json::read(entry, data[i]))
					{
						continue;
					}

					if (list.size() <= static_cast<std::size_t>(entry.index))
					{
						list.resize(entry.index + 1ull);
					}

					list[entry.index] = entry.id;
					map.insert(std::make_pair(entry.id, entry.index));
				}
			};

			mission_list_t mission_list;

			parse_part(list["mission_id_list"], mission_list.mission_id_list, mission_list.mission_id_map);
			parse_part(list["quest_id_list"], mission_list.quest_id_list, mission_list.quest_id_map);
			parse_part(list["defense_mission_id_list"], mission_list.defense_mission_id_list, mission_list.defense_mission_id_map);
			parse_part(list["replay_mission_id_list"], mission_list.replay_mission_id_list, mission_list.replay_mission_id_map);

			return mission_list;
		}
	}

	std::array<std::string, ERR_COUNT> error_map =
	{
		"NOERR",
		"ERR_DEFENSE_MISSION_ALREADY_STARTED",
		"ERR_DEFENSE_MISSION_NOT_IN_MISSION",
		"ERR_PURCHASE_LIMIT",
		"ERR_NOT_DEPLOY",
		"ERR_ALREADY_DEPLOY",
		"ERR_INACTIVE",
		"ERR_SHORTAGE",
		"ERR_ALREADY_ENABLED",
		"ERR_NOT_FOUND",
		"ERR_EXPIRED",
		"ERR_RESTRICT_USER",
		"ERR_COOP_ITEM_USED",
		"ERR_COOP_ITEM_NOT_IN_MISSION",
		"ERR_CREW_NOT_FOUND",
		"ERR_BASE_RESOURCE_SHORTAGE",
		"ERR_BUILDING_OVERLAPPED",
		"ERR_BUILDING_MATERIAL_SHORTAGE",
		"ERR_BUILDING_DOES_NOT_MEET_CONDITIONS",
		"ERR_CRAFT_MATERIAL_SHORTAGE",
		"ERR_CRAFT_DOES_NOT_MEET_CONDITIONS",
		"ERR_PERMISSION_DENIED",
		"ERR_ALREADY_LOCKED",
		"ERR_NOT_IN_ROOM",
		"ERR_ROOM_NOT_FOUND",
		"ERR_PASSWORD_DO_NOT_MATCH",
		"ERR_ROOMTOOMANY",
		"ERR_ALREADYINROOM",
		"ERR_NOT_SOLD",
		"ERR_MAINTENANCE",
		"ERR_BANNED",
		"ERR_GMP_FULL",
		"ERR_GMP_SHORTAGE",
		"ERR_RESOURCE_FULL",
		"ERR_RESOURCE_SHORTAGE",
		"ERR_ALREADY_EXCHANGED",
		"ERR_DETAIL_NOTFOUND",
		"ERR_NOT_COMPLETE",
		"ERR_ALREADY_COMPLETED",
		"ERR_MBCOIN_SHORTAGE",
		"ERR_PAYMENT_INCONSISTENCY",
		"ERR_OVER_CAPACITY",
		"ERR_TARGETPLAYER_NOTFOUND",
		"ERR_NOTEMPTY",
		"ERR_PLAYER_COUNT_FULL",
		"ERR_PLAYER_NOTFOUND",
		"ERR_AUTH_LIMIT",
		"ERR_AUTHKONAMIID_SSLETC",
		"ERR_AUTHKONAMIID_AUTH",
		"ERR_LOGIN_FAILED",
		"ERR_INVALID_SESSION",
		"ERR_INVALID_TICKET",
		"ERR_PREPARE_CIPHERINFO",
		"ERR_READ_CIPHERINFO",
		"ERR_READ_PASSPHRASE",
		"ERR_GET_TICKETINFO",
		"ERR_DEFCLIENTVER",
		"ERR_ALREADYLOGGEDIN",
		"ERR_ALREADY_EXISTS",
		"ERR_INVALID_ACCOUNT",
		"ERR_INVALIDARG",
		"ERR_DATABASE",
		"ERR_FLOWID_OUTOFRANGE",
		"ERR_UNSELECTED_USE_FLOW",
		"ERR_RESULT_ILLEGAL_FLOW",
		"ERR_OPTION_ILLEGAL_FLOW",
		"ERR_RESULT_OUTOFRANGE",
		"ERR_OPTION_OUTOFRANGE",
		"ERR_UNKNOWN",
		"ERR_TIMEOUT",
	};

	const mission_list_t& get_mission_list()
	{
		static const auto mission_list = load_mission_list();
		return mission_list;
	}

	std::int32_t get_mission_index(const std::uint32_t mission_id)
	{
		const auto& list = get_mission_list();
		const auto iter = list.mission_id_map.find(mission_id);
		if (iter == list.mission_id_map.end())
		{
			return -1;
		}

		return static_cast<std::int32_t>(iter->second);
	}

	std::int32_t get_quest_index(const std::uint32_t quest_id)
	{
		const auto& list = get_mission_list();
		const auto iter = list.quest_id_map.find(quest_id);
		if (iter == list.quest_id_map.end())
		{
			return -1;
		}

		return static_cast<std::int32_t>(iter->second);
	}

	std::int32_t get_defense_mission_index(const std::uint32_t mission_id)
	{
		const auto& list = get_mission_list();
		const auto iter = list.defense_mission_id_map.find(mission_id);
		if (iter == list.defense_mission_id_map.end())
		{
			return -1;
		}

		return static_cast<std::int32_t>(iter->second);
	}

	std::int32_t get_replay_mission_index(const std::uint32_t mission_id)
	{
		const auto& list = get_mission_list();
		const auto iter = list.replay_mission_id_map.find(mission_id);
		if (iter == list.replay_mission_id_map.end())
		{
			return -1;
		}

		return static_cast<std::int32_t>(iter->second);
	}

	std::uint32_t get_mission_id(const std::uint32_t index)
	{
		const auto& list = get_mission_list();
		if (list.mission_id_list.size() <= index)
		{
			return 0u;
		}

		return list.mission_id_list[index];
	}

	std::uint32_t get_quest_id(const std::uint32_t index)
	{
		const auto& list = get_mission_list();
		if (list.quest_id_list.size() <= index)
		{
			return 0u;
		}

		return list.quest_id_list[index];
	}

	std::uint32_t get_defense_mission_id(const std::uint32_t index)
	{
		const auto& list = get_mission_list();
		if (list.defense_mission_id_list.size() <= index)
		{
			return 0u;
		}

		return list.defense_mission_id_list[index];
	}

	std::uint32_t get_replay_mission_id(const std::uint32_t index)
	{
		const auto& list = get_mission_list();
		if (list.replay_mission_id_list.size() <= index)
		{
			return 0u;
		}

		return list.replay_mission_id_list[index];
	}

	std::uint32_t get_mission_count()
	{
		const auto& list = get_mission_list();
		return static_cast<std::uint32_t>(list.mission_id_list.size());
	}

	std::uint32_t get_quest_count()
	{
		const auto& list = get_mission_list();
		return static_cast<std::uint32_t>(list.quest_id_list.size());
	}

	std::uint32_t get_defense_mission_count()
	{
		const auto& list = get_mission_list();
		return static_cast<std::uint32_t>(list.defense_mission_id_list.size());
	}

	std::uint32_t get_replay_mission_count()
	{
		const auto& list = get_mission_list();
		return static_cast<std::uint32_t>(list.replay_mission_id_list.size());
	}

	std::uint8_t* get_static_key(const std::uint32_t type)
	{
		switch (type)
		{
		case key_type_tpp:
			return static_key_tpp;
		default:
		case key_type_ssd:
			return static_key;
		}
	}

	std::size_t get_static_key_len()
	{
		return sizeof(static_key);
	}


	bool item_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	void item_t::to_json(json::value& data) const
	{
		data["category"] = this->category;
		data["code"] = this->code;
		data["param1"] = this->param1;
		data["param2"] = this->param2;
		data["param3"] = this->param3;
		data["param4"] = this->param4;
		data["param5"] = this->param5;
		data["num"] = this->num;
	}

	bool customize_option_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	bool customize_option_group_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	bool customize_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	bool resource_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	bool production_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	bool cassette_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	bool production_t::is_stackable() const
	{
		return this->countable && !this->only_flag;
	}

	std::shared_ptr<survival_gear_t> production_t::get_survival_gear()
	{
		const auto iter = parameters_table.ssd_sbm_parameters->survival_gears.find(this->related_id);
		if (iter == parameters_table.ssd_sbm_parameters->survival_gears.end())
		{
			return nullptr;
		}

		return iter->second;
	}

	std::shared_ptr<equip_t> production_t::get_equip()
	{
		const auto iter = parameters_table.ssd_equip_parameters->equips.find(this->related_id);
		if (iter == parameters_table.ssd_equip_parameters->equips.end())
		{
			return nullptr;
		}

		return iter->second;
	}

	std::shared_ptr<accessory_t> production_t::get_accessory()
	{
		const auto iter = parameters_table.ssd_sbm_parameters->accessories.find(this->related_id);
		if (iter == parameters_table.ssd_sbm_parameters->accessories.end())
		{
			return nullptr;
		}

		return iter->second;
	}

	bool recipe_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	bool battle_pack_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	bool equip_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	bool potential_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	bool accessory_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	bool gradeup_spec_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	std::int16_t calc_gradeup_life(const std::uint32_t base_life, const std::uint32_t grade)
	{
		return static_cast<std::uint16_t>(static_cast<float>(base_life) * 0.01f * static_cast<float>(grade) + static_cast<float>(base_life));
	}

	bool survival_gear_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}
 
	bool crew_member_type_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	bool defense_mission_settings_t::parse(json::value& data)
	{
		const auto count = std::min(this->rank_reward_info.size(), data["uiRewardInfo"]["rank_reward_info"].size());
		for (auto i = 0ull; i < count; i++)
		{
			if (!json::read(this->rank_reward_info[i], data["uiRewardInfo"]["rank_reward_info"][i]))
			{
				return false;
			}
		}

		return json::read(*this, data);
	}

	bool is_event_obtained_res(const std::uint32_t resource_id, std::uint8_t* obtained, bool* result)
	{
		// PRD_TYPE_Event

		switch (resource_id)
		{
		case RES_BrokenGasCylinder:
			*result = obtained[1];
			return true;
		case RES_WheelChairParts:
			*result = obtained[0];
			return true;
		}

		return false;
	}

	std::uint32_t calc_time_reduction_cost(const std::uint32_t time_left)
	{
		return static_cast<std::uint32_t>(std::roundf(static_cast<float>(time_left) * 0.00133f + 5.f)); // todo
	}
}
