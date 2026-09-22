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

		std::unordered_map<std::uint32_t, std::string> error_map =
		{
			{NOERR, "NOERR"},
			{ERR_DEFENSE_MISSION_ALREADY_STARTED, "ERR_DEFENSE_MISSION_ALREADY_STARTED"},
			{ERR_DEFENSE_MISSION_NOT_IN_MISSION, "ERR_DEFENSE_MISSION_NOT_IN_MISSION"},
			{ERR_PURCHASE_LIMIT, "ERR_PURCHASE_LIMIT"},
			{ERR_NOT_DEPLOY, "ERR_NOT_DEPLOY"},
			{ERR_ALREADY_DEPLOY, "ERR_ALREADY_DEPLOY"},
			{ERR_INACTIVE, "ERR_INACTIVE"},
			{ERR_SHORTAGE, "ERR_SHORTAGE"},
			{ERR_ALREADY_ENABLED, "ERR_ALREADY_ENABLED"},
			{ERR_NOT_FOUND, "ERR_NOT_FOUND"},
			{ERR_EXPIRED, "ERR_EXPIRED"},
			{ERR_RESTRICT_USER, "ERR_RESTRICT_USER"},
			{ERR_COOP_ITEM_USED, "ERR_COOP_ITEM_USED"},
			{ERR_COOP_ITEM_NOT_IN_MISSION, "ERR_COOP_ITEM_NOT_IN_MISSION"},
			{ERR_CREW_NOT_FOUND, "ERR_CREW_NOT_FOUND"},
			{ERR_BASE_RESOURCE_SHORTAGE, "ERR_BASE_RESOURCE_SHORTAGE"},
			{ERR_BUILDING_OVERLAPPED, "ERR_BUILDING_OVERLAPPED"},
			{ERR_BUILDING_MATERIAL_SHORTAGE, "ERR_BUILDING_MATERIAL_SHORTAGE"},
			{ERR_BUILDING_DOES_NOT_MEET_CONDITIONS, "ERR_BUILDING_DOES_NOT_MEET_CONDITIONS"},
			{ERR_CRAFT_MATERIAL_SHORTAGE, "ERR_CRAFT_MATERIAL_SHORTAGE"},
			{ERR_CRAFT_DOES_NOT_MEET_CONDITIONS, "ERR_CRAFT_DOES_NOT_MEET_CONDITIONS"},
			{ERR_PERMISSION_DENIED, "ERR_PERMISSION_DENIED"},
			{ERR_ALREADY_LOCKED, "ERR_ALREADY_LOCKED"},
			{ERR_NOT_IN_ROOM, "ERR_NOT_IN_ROOM"},
			{ERR_ROOM_NOT_FOUND, "ERR_ROOM_NOT_FOUND"},
			{ERR_PASSWORD_DO_NOT_MATCH, "ERR_PASSWORD_DO_NOT_MATCH"},
			{ERR_ROOMTOOMANY, "ERR_ROOMTOOMANY"},
			{ERR_ALREADYINROOM, "ERR_ALREADYINROOM"},
			{ERR_NOT_SOLD, "ERR_NOT_SOLD"},
			{ERR_MAINTENANCE, "ERR_MAINTENANCE"},
			{ERR_BANNED, "ERR_BANNED"},
			{ERR_GMP_FULL, "ERR_GMP_FULL"},
			{ERR_GMP_SHORTAGE, "ERR_GMP_SHORTAGE"},
			{ERR_RESOURCE_FULL, "ERR_RESOURCE_FULL"},
			{ERR_RESOURCE_SHORTAGE, "ERR_RESOURCE_SHORTAGE"},
			{ERR_ALREADY_EXCHANGED, "ERR_ALREADY_EXCHANGED"},
			{ERR_DETAIL_NOTFOUND, "ERR_DETAIL_NOTFOUND"},
			{ERR_NOT_COMPLETE, "ERR_NOT_COMPLETE"},
			{ERR_ALREADY_COMPLETED, "ERR_ALREADY_COMPLETED"},
			{ERR_MBCOIN_SHORTAGE, "ERR_MBCOIN_SHORTAGE"},
			{ERR_PAYMENT_INCONSISTENCY, "ERR_PAYMENT_INCONSISTENCY"},
			{ERR_OVER_CAPACITY, "ERR_OVER_CAPACITY"},
			{ERR_TARGETPLAYER_NOTFOUND, "ERR_TARGETPLAYER_NOTFOUND"},
			{ERR_NOTEMPTY, "ERR_NOTEMPTY"},
			{ERR_PLAYER_COUNT_FULL, "ERR_PLAYER_COUNT_FULL"},
			{ERR_PLAYER_NOTFOUND, "ERR_PLAYER_NOTFOUND"},
			{ERR_AUTH_LIMIT, "ERR_AUTH_LIMIT"},
			{ERR_AUTHKONAMIID_SSLETC, "ERR_AUTHKONAMIID_SSLETC"},
			{ERR_AUTHKONAMIID_AUTH, "ERR_AUTHKONAMIID_AUTH"},
			{ERR_LOGIN_FAILED, "ERR_LOGIN_FAILED"},
			{ERR_INVALID_SESSION, "ERR_INVALID_SESSION"},
			{ERR_INVALID_TICKET, "ERR_INVALID_TICKET"},
			{ERR_PREPARE_CIPHERINFO, "ERR_PREPARE_CIPHERINFO"},
			{ERR_READ_CIPHERINFO, "ERR_READ_CIPHERINFO"},
			{ERR_READ_PASSPHRASE, "ERR_READ_PASSPHRASE"},
			{ERR_GET_TICKETINFO, "ERR_GET_TICKETINFO"},
			{ERR_DEFCLIENTVER, "ERR_DEFCLIENTVER"},
			{ERR_ALREADYLOGGEDIN, "ERR_ALREADYLOGGEDIN"},
			{ERR_ALREADY_EXISTS, "ERR_ALREADY_EXISTS"},
			{ERR_INVALID_ACCOUNT, "ERR_INVALID_ACCOUNT"},
			{ERR_INVALIDARG, "ERR_INVALIDARG"},
			{ERR_DATABASE, "ERR_DATABASE"},
			{ERR_FLOWID_OUTOFRANGE, "ERR_FLOWID_OUTOFRANGE"},
			{ERR_UNSELECTED_USE_FLOW, "ERR_UNSELECTED_USE_FLOW"},
			{ERR_RESULT_ILLEGAL_FLOW, "ERR_RESULT_ILLEGAL_FLOW"},
			{ERR_OPTION_ILLEGAL_FLOW, "ERR_OPTION_ILLEGAL_FLOW"},
			{ERR_RESULT_OUTOFRANGE, "ERR_RESULT_OUTOFRANGE"},
			{ERR_OPTION_OUTOFRANGE, "ERR_OPTION_OUTOFRANGE"},
			{ERR_UNKNOWN, "ERR_UNKNOWN"},
			{ERR_TIMEOUT, "ERR_TIMEOUT"},
		};
	}

	std::unordered_set<std::uint32_t> mission_list =
	{
		30010,
		10010,
		40040,
		40050,
		40060,
		40070,
		40010,
		40025,
		40015,
		40020,
		40030,
		40035,
		10020,
		40075,
		40077,
		20010,
		20020,
		20030,
		40080,
		40090,
		40130,
		10030,
		40145,
		20110,
		20120,
		20130,
		40140,
		40150,
		40155,
		10035,
		30020,
		40160,
		40180,
		40170,
		10040,
		20210,
		20220,
		20230,
		40220,
		40230,
		10050,
		40250,
		40260,
		40270,
		10060,
		40310,
		40320,
		20610,
		20620,
		20630,
		20710,
		20720,
		20730,
		20105,
		20115,
		20125,
		20725,
		31010,
		31020,
		00001,
		00005,
		21000,
		21005,
		21010,
		21020,
		21019,
		21025,
		01010,
		01020,
		01030,
		01040,
		02010,
		12010,
		12020,
		12030,
		32010,
		60010,
		60011,
		60012,
		60013,
		60014,
		65010,
		65020,
		65030,
		65040,
		65050,
		65060,
		62010,
	};

	std::unordered_set<std::uint32_t> quest_list =
	{
		22010,
		22020,
		22030,
		22040,
		22050,
		22060,
		22140,
		22150,
		22090,
		22100,
		22120,
		22130,
		22160,
		11010,
		11020,
		11030,
		11040,
		11050,
		11060,
		11070,
		11080,
		11100,
		11110,
		11090,
		11120,
		11130,
		11140,
		11150,
		11160,
		11170,
		11180,
		11190,
		11200,
		11210,
		11220,
		11230,
		11240,
		11250,
		11260,
		11270,
		11280,
		11290,
		11700,
		11710,
		11720,
		11730,
		11740,
		11750,
		11760,
		44050,
		44010,
		44020,
		44030,
		44040,
		44060,
		44070,
		44080,
		44090,
		44100,
		44110,
		44120,
		44130,
		44140,
		44150,
		44160,
		44170,
		44180,
		44190,
		44200,
		33080,
		33090,
		33100,
		33110,
		11300,
		11310,
		11320,
		11330,
		11340,
		33040,
		33050,
		33060,
		33070,
		33071,
		33010,
		33020,
		33030,
		44220,
		44210,
		44230,
		44240,
		44250,
		44260,
		44270,
		44280,
		44290,
		44300,
		33260,
		33270,
		33220,
		33230,
		33240,
		33250,
		33280,
		33290,
		33300,
		11350,
		54010,
		54020,
		54030,
		54040,
		54050,
		54060,
		54070,
		54080,
		54090,
		54100,
		11901,
		11902,
		11903,
		11911,
		11912,
		11913,
		11921,
		11922,
		11923,
		11924,
		11925,
		11926,
		11928,
		11929,
		11930,
		11931,
		61010,
		61020,
		61030,
		61040,
		54110,
		54120,
		54130,
		54140,
		54150,
		54160,
		54170,
		54180,
		54190,
		54200,
		54210,
		54220,
		54230,
		54240,
		54250,
		54260,
		54270,
		54280,
		54290,
		54300,
		54310,
		74010,
		74020,
		74030,
		74040,
	};

	std::string get_error(const std::uint32_t error)
	{
		if (error > ERR_COUNT)
		{
			throw std::runtime_error("invalid error");
		}

		return error_map.at(error);
	}

	const std::unordered_map<std::uint32_t, std::string>& get_error_map()
	{
		return error_map;
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

	std::shared_ptr<survival_gear_t> production_t::get_survival_gear()
	{
		const auto iter = parameters_table.ssd_sbm_parameters->survival_gears.find(this->related_id);
		if (iter == parameters_table.ssd_sbm_parameters->survival_gears.end())
		{
			return nullptr;
		}

		return iter->second;
	}

	bool recipe_t::parse(json::value& data)
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
		switch (resource_id)
		{
		case 1576527750:
			*result = obtained[1];
			return true;
		//case 0: // ???
		//	*result = obtained[0];
		//	return true;
		}

		return false;
	}
}
