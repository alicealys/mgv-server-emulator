#include <std_include.hpp>

#include "game.hpp"
#include "parameters.hpp"

#include "utils/resources.hpp"
#include "utils/json_utils.hpp"

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
	static constexpr auto modify = glz::object(
		"craftCategory", &T::craft_category
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

	bool production_t::parse(json::value& data)
	{
		return !glz::read<glz::opts{.error_on_unknown_keys = false}>(*this, data);
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
		return !json::read(*this, data);
	}

	bool gradeup_spec_t::parse(json::value& data)
	{
		return !json::read(*this, data);
	}

	std::int16_t calc_gradeup_life(const std::uint32_t base_life, const std::uint32_t grade)
	{
		return static_cast<std::uint16_t>(static_cast<float>(base_life) * 0.01f * static_cast<float>(grade) + static_cast<float>(base_life));
	}

	bool survival_gear_t::parse(json::value& data)
	{
		return !json::read(*this, data);
	}
 
	bool crew_member_type_t::parse(json::value& data)
	{
		return !json::read(*this, data);
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
