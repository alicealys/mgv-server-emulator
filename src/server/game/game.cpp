#include <std_include.hpp>

#include "game.hpp"
#include "parameters.hpp"

#include "utils/resources.hpp"
#include "utils/json_utils.hpp"

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

	bool production_t::parse(nlohmann::json& data)
	{
		utils::json::get_or(data["str"], this->id_str);	
		utils::json::get_or(data["id"], this->id);
		utils::json::get_or(data["index"], this->index);
		utils::json::get_or(data["type"], this->type);
		utils::json::get_or(data["only_flag"], this->only_flag);
		utils::json::get_or(data["craftCategory"], this->craft_category);
		utils::json::get_or(data["rarity"], this->rarity);
		utils::json::get_or(data["lv_need"], this->lv_need);
		utils::json::get_or(data["countable"], this->countable);
		utils::json::get_or(data["stock"], this->stock);
		utils::json::get_or(data["life"], this->life);
		utils::json::get_or(data["weight"], this->weight);
		utils::json::get_or(data["customize"], this->customize);
		utils::json::get_or(data["conv_use"], this->conv_use);
		utils::json::get_or(data["base_c_water"], this->base_c_water);
		utils::json::get_or(data["base_food"], this->base_food);
		utils::json::get_or(data["base_medicine"], this->base_medicine);
		utils::json::get_or(data["sup_combat"], this->sup_combat);
		utils::json::get_or(data["sup_survival"], this->sup_survival);
		utils::json::get_or(data["crew_inj_food_poison"], this->crew_inj_food_poison);
		utils::json::get_or(data["crew_inj_dysentery"], this->crew_inj_dysentery);
		utils::json::get_or(data["crew_inj_physical"], this->crew_inj_physical);
		utils::json::get_or(data["crew_inj_fatigue"], this->crew_inj_fatigue);
		utils::json::get_or(data["related_id"], this->related_id);
		utils::json::get_or(data["eqp_hip"], this->eqp_hip);
		utils::json::get_or(data["eqp_back"], this->eqp_back);
		utils::json::get_or(data["eqp_sec"], this->eqp_sec);
		utils::json::get_or(data["eqp_sup"], this->eqp_sup);
		utils::json::get_or(data["pri_r_slot"], this->pri_r_slot);
		utils::json::get_or(data["lang_name"], this->lang_name);
		utils::json::get_or(data["lang_name2"], this->lang_name2);
		utils::json::get_or(data["lang_name3"], this->lang_name3);
		utils::json::get_or(data["lang_info"], this->lang_info);
		utils::json::get_or(data["show_info"], this->show_info);
		utils::json::get_or(data["icon_path"], this->icon_path);
		utils::json::get_or(data["prv_r_x"], this->prv_r_x);
		utils::json::get_or(data["prv_r_y"], this->prv_r_y);
		utils::json::get_or(data["prv_r_y_min"], this->prv_r_y_min);
		utils::json::get_or(data["prv_r_y_max"], this->prv_r_y_max);
		utils::json::get_or(data["prv_r_z_off"], this->prv_r_z_off);
		utils::json::get_or(data["prv_zm_min"], this->prv_zm_min);
		utils::json::get_or(data["prv_zm"], this->prv_zm);
		utils::json::get_or(data["prv_zm_max"], this->prv_zm_max);
		utils::json::parse_array(data["perk"], this->perk);
		return true;
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

	bool recipe_t::parse(nlohmann::json& data)
	{
		utils::json::get_or(data["opened"], this->opened);
		utils::json::get_or(data["junk"], this->junk);
		utils::json::get_or(data["index"], this->index);
		utils::json::get_or(data["group"], this->group);
		utils::json::get_or(data["dev_level"], this->dev_level);
		utils::json::get_or(data["count"], this->count);
		utils::json::get_or(data["transTime"], this->trans_time);
		utils::json::get_or(data["price"], this->price);
		utils::json::get_or(data["potentialId"], this->potential_id);
		utils::json::get_or(data["id"], this->id);
		utils::json::get_or(data["id_str"], this->id_str);
		utils::json::get_or(data["production"], this->production_id);

		utils::json::parse_array(data["cost"], this->cost, [](recipe_t::cost_t& dest, nlohmann::json& src)
		{
			utils::json::get_or(src["count"], dest.count);
			utils::json::get_or(src["type"], dest.type);
			utils::json::get_or(src["id"], dest.id);
		});

		return true;
	}

	bool gradeup_spec_t::parse(nlohmann::json& data)
	{
		utils::json::get_or(data["rarity"], this->rarity);
		utils::json::get_or(data["atk"], this->atk);
		utils::json::get_or(data["w_life"], this->w_life);
		utils::json::get_or(data["def"], this->def);
		utils::json::get_or(data["a_life"], this->a_life);
		return true;
	}

	std::int16_t calc_gradeup_life(const std::uint32_t base_life, const std::uint32_t grade)
	{
		return static_cast<std::uint16_t>(static_cast<float>(base_life) * 0.01f * static_cast<float>(grade) + static_cast<float>(base_life));
	}

	bool survival_gear_t::parse(nlohmann::json& data)
	{
		utils::json::get_or(data["sveId"], this->id);
		utils::json::get_or(data["index"], this->index);
		utils::json::get_or(data["typeId"], this->type_id);
		utils::json::get_or(data["targetId"], this->target_id);
		utils::json::get_or(data["valueu1"], this->value_u1);
		utils::json::get_or(data["mesh"], this->mesh);
		return true;
	}
}
