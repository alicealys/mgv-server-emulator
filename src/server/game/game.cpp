#include <std_include.hpp>

#include "game.hpp"
#include "parameters.hpp"
#include "lottery.hpp"

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
struct glz::meta<game::building_t>
{
	using T = game::building_t;
	static constexpr auto value = glz::object(
		"id_str", &T::id_str,
		"id", &T::id,
		"type", &T::type,
		"spec", &T::spec,
		"mapIcon", &T::map_icon,
		"craftCategories", &T::craft_categories,
		"fill", &T::fill,
		"edge", &T::edge,
		"gimmickId", &T::gimmick_id,
		"origin", &T::origin,
		"load", &T::load,
		"basis", &T::basis,
		"ridableRoof", &T::ridable_roof,
		"spikyBody", &T::spiky_body,
		"spikyHead", &T::spiky_head,
		"spikyFloor", &T::spiky_floor,
		"auto", &T::auto_,
		"noStopper", &T::no_stopper,
		"muteki", &T::muteki,
		"switch", &T::switch_,
		"turn", &T::turn,
		"fake", &T::fake,
		"friendlyFire", &T::friendly_fire,
		"chain", &T::chain,
		"tempEnclosure", &T::temp_enclosure,
		"aroundAccess", &T::around_access,
		"flagType", &T::flag_type,
		"aimHead", &T::aim_head,
		"rain", &T::rain,
		"soundArea", &T::sound_area,
		"attackId", &T::attack_id,
		"baseAttackId", &T::base_attack_id,
		"blastAttackId", &T::blast_attack_id,
		"buffs", &T::buffs,
		"maxCount", &T::max_count,
		"block", &T::block,
		"cost", &T::cost,
		"panelLimitAngle", &T::panel_limit_angle,
		"panelOffsetY", &T::panel_offset_y,
		"panelOffsetZ", &T::panel_offset_z,
		"bulletCount", &T::bullet_count,
		"aimAngle", &T::aim_angle,
		"shootRange", &T::shoot_range,
		"attackRadius", &T::attack_radius,
		"shootInterval", &T::shoot_interval,
		"shootMaxBeatCount", &T::shoot_max_beat_count,
		"shootCoolDownTime", &T::shoot_cooldown_time,
		"defenseValue", &T::defense_value,
		"defenseLevel", &T::defense_level,
		"damageRate", &T::damage_rate,
		"stamina", &T::stamina,
		"staminaDefense", &T::stamina_defense,
		"attackDamage", &T::attack_damage,
		"maxUnitCount", &T::max_unit_oount,
		"lifeTime", &T::life_time,
		"autoResetTime", &T::auto_reset_time,
		"damageSoundId", &T::damage_sound_id,
		"breakSoundId", &T::break_sound_id,
		"breakSoundIdSub", &T::break_sound_id_sub,
		"breakSoundIdSub2", &T::break_sound_id_sub2,
		"idleSoundId", &T::idle_sound_id,
		"idleStopSoundId", &T::idle_stop_sound_id,
		"sparkEffect", &T::spark_effect,
		"breakEffect", &T::break_effect,
		"assetId", &T::asset_id,
		"resources", &T::resources
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
struct glz::meta<game::crew_type_t>
{
	using T = game::crew_type_t;
	static constexpr auto value = glz::object(
		"id_str", &T::id_str,
		"id", &T::id,
		"face", &T::face,
		"race", &T::race,
		"sex", &T::sex,
		"body", &T::body,
		"DEVELOP.low", &T::develop_low,
		"DEVELOP.high", &T::develop_high,
		"FOOD.low", &T::food_low,
		"FOOD.high", &T::food_high,
		"MEDIC.low", &T::medic_low,
		"MEDIC.high", &T::medic_high,
		"FARM.low", &T::farm_low,
		"FARM.high", &T::farm_high,
		"BASE_DEFENSE.low", &T::base_defense_low,
		"BASE_DEFENSE.high", &T::base_defense_high,
		"COMBAT_DEPLOY.low", &T::combat_deploy_low,
		"COMBAT_DEPLOY.high", &T::combat_deploy_high,
		"THIRST_RESIST.low", &T::thirst_resist_low,
		"THIRST_RESIST.high", &T::thirst_resist_high,
		"HUNGER_RESIST.low", &T::hunger_resist_low,
		"HUNGER_RESIST.high", &T::hunger_resist_high,
		"INJURY_RESIST.low", &T::injury_resist_low,
		"INJURY_RESIST.high", &T::injury_resist_high,
		"SICK_RESIST.low", &T::sick_resist_low,
		"SICK_RESIST.high", &T::sick_resist_high,
		"SLEEPLACK_RESIST.low", &T::sleeplack_resist_low,
		"SLEEPLACK_RESIST.high", &T::sleeplack_resist_high,
		"LIFE.low", &T::life_low,
		"LIFE.high", &T::life_high,
		"job_table", &T::job_table,
		"skill_table", &T::skill_table
	);
};

template <>
struct glz::meta<game::crew_unique_type_t>
{
	using T = game::crew_unique_type_t;
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


template <>
struct glz::meta<game::coop_mission_info_t>
{
	using T = game::coop_mission_info_t;
	static constexpr auto value = glz::object(
		"waveCountMax", &T::max_wave_count,
		"enemyLevel", &T::enemy_level,
		"missionId", &T::mission_id,
		"rankThreashold", &T::rank_threshold,
		"uiRewardInfo", &T::ui_reward_info
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

	bool building_t::parse(json::value& data)
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

	bool crew_sickness_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	bool crew_set_table_t::parse(json::value& data, const std::string& key)
	{
		this->id = data["id"].as<std::uint32_t>();

		for (auto i = 0ull; i < 10; i++)
		{
			const auto ratio_k = std::format("ratio{}", i + 1);
			const auto value_k = std::format("{}{}", key, i + 1);

			crew_set_table_t::entry_t entry{};

			entry.ratio = data[ratio_k].as<std::uint32_t>();
			entry.value = data[value_k].as<std::uint32_t>();

			if (entry.ratio != 0 || entry.value != 0)
			{
				this->entries.emplace_back(entry);
				this->values.emplace_back(entry.value);
				this->ratios.emplace_back(entry.ratio);
			}
		}

		return true;
	}

	bool crew_skill_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	bool crew_type_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	bool crew_unique_type_t::parse(json::value& data)
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

	bool coop_mission_info_t::parse(json::value& data)
	{
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

	std::uint32_t rank_hash_to_value(const std::uint32_t hash)
	{
		switch (hash)
		{
		case 0x3EFB41DC: // "S"
			return 1;
		case 0x8D6B1987: // "A"
			return 2;
		case 0x0B6CEF41: // "B"
			return 3;
		case 0x4AF41426: // "C"
			return 4;
		case 0x5CC6B17E: // "D"
			return 5;
		case 0xB9FC4076: // "E"
			return 6;
		default:
			return 7;
		}
	}

	std::uint32_t rank_value_to_hash(const std::uint32_t hash)
	{
		switch (hash)
		{
		case 1: // "S"
			return 0x3EFB41DC;
		case 2: // "A"
			return 0x8D6B1987;
		case 3: // "B"
			return 0x0B6CEF41;
		case 4: // "C"
			return 0x4AF41426;
		case 5: // "D"
			return 0x5CC6B17E;
		case 6: // "E"
			return 0xB9FC4076;
		default:
			return 0xB9FC4076;
		}
	}
}
