#include <std_include.hpp>

#include "players.hpp"
#include "users.hpp"
#include "../auth.hpp"
#include "component/command.hpp"

#include "game/parameters.hpp"

#include "utils/encoding.hpp"
#include "utils/json_utils.hpp"

#include <utils/cryptography.hpp>
#include <utils/string.hpp>

namespace database::players
{
	namespace
	{
		namespace exp
		{
			const auto select = sqlpp::select(
				player::table.player_id, 
				player::table.player_index, 
				player::table.f_user_id, 
				player::table.loadout_count, 
				player::table.current_loadout,
				player::table.playtime,
				player::table.nameplate,
				player::table.player_creation_date,
				player::table.point,
				users::user::table.account_id
			).from(player::table.join(users::user::table).on(users::user::table.user_id == player::table.f_user_id));
		}

		const json::value& get_default_data()
		{
			static const auto data = utils::resources::load_json(RESOURCE_DEFAULT_DATA);
			return data;
		}

		const json::value& get_default_data(const std::string_view& key)
		{
			const auto& data = get_default_data();
			return data[key];
		}
	}

	void player_inventory_t::initialize()
	{
		static const auto default_inventory = []()
		{
			static player_inventory_t data{};

			const auto& default_data = get_default_data("inventory_player_info");
			const auto load = [&]<typename T>(const std::string_view& key, T& buffer)
			{
				auto& data = default_data[key].get<std::string>();
				const auto decoded = utils::cryptography::base64::decode(data);

				if (decoded.size() != sizeof(T))
				{
					throw std::runtime_error("invalid player inventory data");
				}

				std::memcpy(buffer, decoded.data(), sizeof(T));
			};

			load("event_obtained", data.event_obtained);
			load("skill_status", data.skill_status);
			load("survival_new", data.survival_new);
			load("survival_obtained", data.survival_obtained);
			load("survival_slot_new", data.survival_slot_new);
			data.oxygen_convert_count = default_data["oxygen_convert_count"].as<std::uint32_t>();

			return &data;
		}();

		std::memcpy(this, default_inventory, sizeof(player_inventory_t));
	}

	bool player_inventory_t::parse(json::value& data, std::uint16_t& nameplate)
	{
		utils::json_utils::get_or(data["energy"], this->energy);
		utils::json_utils::get_or(data["name_plate"], nameplate);
		utils::json_utils::get_or(data["class_opened"], this->class_opened);
		utils::json_utils::get_or(data["oxygen_convert_count"], this->oxygen_convert_count);

		utils::json_utils::parse_array(data["cbox_history"], this->cbox_history);
		utils::json_utils::parse_array(data["cbox_location"], this->cbox_location);
		utils::json_utils::parse_array(data["cbox_pos"], this->cbox_pos);
		utils::json_utils::parse_array(data["cbox_updated"], this->cbox_updated);
		utils::json_utils::parse_array(data["energy_invested"], this->energy_invested);

		if (!utils::json_utils::parse_base64(data["event_obtained"], this->event_obtained, true) ||
			!utils::json_utils::parse_base64(data["survival_slot_new"], this->survival_slot_new, true) ||
			!utils::json_utils::parse_base64(data["survival_obtained"], this->survival_obtained, true) ||
			!utils::json_utils::parse_base64(data["survival_new"], this->survival_new, true) ||
			!utils::json_utils::parse_base64(data["skill_status"], this->skill_status, true))
		{
			return false;
		}

		return true;
	}

	bool player_inventory_t::parse_save(json::value& data, std::uint16_t& nameplate)
	{
		utils::json_utils::get_or(data["energy"], this->energy);
		utils::json_utils::get_or(data["name_plate"], nameplate);
		utils::json_utils::get_or(data["class_opened"], this->class_opened);
		utils::json_utils::get_or(data["oxygen_convert_count"], this->oxygen_convert_count);

		utils::json_utils::parse_array(data["cbox_history"], this->cbox_history);
		utils::json_utils::parse_array(data["cbox_location"], this->cbox_location);
		utils::json_utils::parse_array(data["cbox_pos"], this->cbox_pos);
		utils::json_utils::parse_array(data["cbox_updated"], this->cbox_updated);
		//utils::json_utils::parse_array(data["energy_invested"], this->energy_invested);

		const auto try_parse_part = [&]<typename T>(const std::string_view & name, T & dest, const bool or_ = false)
		{
			auto& value_j = data[name];
			if (!value_j.is_array() || value_j.size() <= 0)
			{
				return;
			}

			auto& obj_j = value_j[0];
			if (!obj_j.is_object())
			{
				return;
			}

			auto& data_j = obj_j[name];
			if (!data_j.is_string())
			{
				return;
			}

			if (or_)
			{
				T new_data{};
				utils::json_utils::parse_base64(data_j, new_data, true);

				auto src = reinterpret_cast<std::uint8_t*>(&new_data);
				auto dst = reinterpret_cast<std::uint8_t*>(&dest);

				for (auto i = 0ull; i < sizeof(T); i++)
				{
					dst[i] |= src[i];
				}
			}
			else
			{
				utils::json_utils::parse_base64(data_j, dest, true);
			}
		};

		try_parse_part("event_obtained", this->event_obtained, true);
		try_parse_part("survival_slot_new", this->survival_slot_new);
		try_parse_part("survival_obtained", this->survival_obtained, true);
		try_parse_part("survival_new", this->survival_new);
		try_parse_part("skill_status", this->skill_status, true);

		return true;
	}

	void player_inventory_t::to_json(json::value& data, const std::uint16_t nameplate) const
	{
		data["cbox_history"] = this->cbox_history;
		data["cbox_location"] = this->cbox_location;
		data["cbox_pos"] = this->cbox_pos;
		data["cbox_updated"] = this->cbox_updated;
		data["class_opened"] = this->class_opened;
		data["energy"] = this->energy;
		data["energy_invested"] = this->energy_invested;
		data["name_plate"] = nameplate;
		data["oxygen_convert_count"] = this->oxygen_convert_count;
		data["event_obtained"] = utils::encoding::encode_base64(this->event_obtained);
		data["survival_obtained"] = utils::encoding::encode_base64(this->survival_obtained);
		data["skill_status"] = utils::encoding::encode_base64(this->skill_status);
		data["survival_slot_new"] = utils::encoding::encode_base64(this->survival_slot_new);
		data["survival_new"] = utils::encoding::encode_base64(this->survival_new);
	}

	void player_inventory_t::set_survival_obtained(const std::uint32_t index, bool obtained)
	{
		const auto byte_index = (index >> 3);
		if (obtained)
		{
			this->survival_obtained[byte_index] |= (1 << (index & 7));
		}
		else
		{
			this->survival_obtained[byte_index] &= ~(1 << (index & 7));
		}
	}

	void avatar_t::to_json(json::value& data) const
	{
		data["accessory"] = this->accessory;
		data["beard_length"] = this->beard_length;
		data["beard_style"] = this->beard_style;
		data["eyebrow_length"] = this->eyebrow_length;
		data["eyebrow_style"] = this->eyebrow_style;
		data["hair_color"] = this->hair_color;
		data["hair_style"] = this->hair_style;
		data["left_eye_brightness"] = this->left_eye_brightness;
		data["left_eye_color"] = this->left_eye_color;
		data["player_parts_type"] = this->player_parts_type;
		data["player_type"] = this->player_type;
		data["race"] = this->race;
		data["race_color"] = this->race_color;
		data["race_type"] = this->race_type;
		data["race_variation"] = this->race_variation;
		data["right_eye_brightness"] = this->right_eye_brightness;
		data["right_eye_color"] = this->right_eye_color;
		data["tattoo"] = this->tattoo;
		data["tattoo_color"] = this->tattoo_color;
		data["voice"] = this->voice;
		data["voice_pitch"] = this->voice_pitch;
		data["avatar_name"] = utils::encoding::encode_base64(this->name);
		data["parameter"] = utils::encoding::encode_base64(this->motion_frame_list);
	}

	bool avatar_t::parse(json::value& data)
	{
		std::memset(this, 0, sizeof(avatar_t));

		auto& name_j = data["avatar_name"];
		auto& parameter_j = data["parameter"];

		if (!name_j.is_string() || !parameter_j.is_string())
		{
			return false;
		}

		const auto decode_data = [&]<typename T>(json::value& src, T& dest)
		{
			auto data = src.get<std::string>();
			data = utils::encoding::decode_url_string(data);
			data = utils::cryptography::base64::decode(data);

			if (data.size() > sizeof(T))
			{
				return false;
			}

			std::memcpy(dest, data.data(), data.size());
			return true;
		};

		if (!decode_data(name_j, this->name) || !decode_data(parameter_j, this->motion_frame_list))
		{
			return false;
		}

		utils::json_utils::get_or(data["accessory"], this->accessory);
		utils::json_utils::get_or(data["beard_length"], this->beard_length);
		utils::json_utils::get_or(data["beard_style"], this->beard_style);
		utils::json_utils::get_or(data["eyebrow_length"], this->eyebrow_length);
		utils::json_utils::get_or(data["eyebrow_style"], this->eyebrow_style);
		utils::json_utils::get_or(data["hair_color"], this->hair_color);
		utils::json_utils::get_or(data["hair_style"], this->hair_style);
		utils::json_utils::get_or(data["left_eye_brightness"], this->left_eye_brightness);
		utils::json_utils::get_or(data["left_eye_color"], this->left_eye_color);
		utils::json_utils::get_or(data["player_parts_type"], this->player_parts_type);
		utils::json_utils::get_or(data["player_type"], this->player_type);
		utils::json_utils::get_or(data["race"], this->race);
		utils::json_utils::get_or(data["race_color"], this->race_color);
		utils::json_utils::get_or(data["race_type"], this->race_type);
		utils::json_utils::get_or(data["race_variation"], this->race_variation);
		utils::json_utils::get_or(data["right_eye_brightness"], this->right_eye_brightness);
		utils::json_utils::get_or(data["right_eye_color"], this->right_eye_color);
		utils::json_utils::get_or(data["tattoo"], this->tattoo);
		utils::json_utils::get_or(data["tattoo_color"], this->tattoo_color);
		utils::json_utils::get_or(data["voice"], this->voice);
		utils::json_utils::get_or(data["voice_pitch"], this->voice_pitch);

		return true;
	}

	void loadout_t::initialize(const std::uint32_t index)
	{
		std::memset(this, 0, sizeof(loadout_t));

		snprintf(this->name, sizeof(this->name), "LOAD OUT %i", index);

		this->gear_info.arm_inventory_index = 0xFFFF;
		this->gear_info.body_inventory_index = 0xFFFF;
		this->gear_info.head_inventory_index = 0xFFFF;
		this->gear_info.leg_inventory_index = 0xFFFF;

		for (auto i = 0ull; i < ARRAYSIZE(this->main_weapon_list); i++)
		{
			this->main_weapon_list[i].inventory_index = 0xFFFF;
			this->sub_weapon_list[i].inventory_index = 0xFFFF;
		}
	}

	bool loadout_t::parse(json::value& data, std::uint32_t& index)
	{
		std::memset(this, 0, sizeof(loadout_t));
		index = 0u;

		auto& gear_info_j = data["gear_info"];
		auto& gadget_list_j = data["gadget_list"];
		auto& main_weapon_list_j = data["main_weapon_list"];
		auto& sub_weapon_list_j = data["sub_weapon_list"];
		auto& porch_list_j = data["porch_list"];
		auto& skill_list_j = data["skill_list"];
		auto& name_j = data["name"];
		auto& index_j = data["index"];

		if (!gadget_list_j.is_array() || !main_weapon_list_j.is_array() || 
			!sub_weapon_list_j.is_array() || !index_j.is_uint64() ||
			!porch_list_j.is_array() || !name_j.is_string() ||
			!gear_info_j.is_object() || !skill_list_j.is_array())
		{
			return false;
		}

		index = index_j.as<std::uint32_t>();
		
		utils::json_utils::parse_string(name_j, this->name);

		utils::json_utils::get_or(gear_info_j["arm_inventory_index"], this->gear_info.arm_inventory_index);
		utils::json_utils::get_or(gear_info_j["body_inventory_index"], this->gear_info.body_inventory_index);
		utils::json_utils::get_or(gear_info_j["ext_head_production_idx"], this->gear_info.ext_head_production_idx);
		utils::json_utils::get_or(gear_info_j["ext_suit_production_idx"], this->gear_info.ext_suit_production_idx);
		utils::json_utils::get_or(gear_info_j["head_inventory_index"], this->gear_info.head_inventory_index);
		utils::json_utils::get_or(gear_info_j["leg_inventory_index"], this->gear_info.leg_inventory_index);

		if (!utils::json_utils::parse_array(gadget_list_j, this->gadget_list, [](item_t& dest, json::value& src)
			{
				utils::json_utils::get_or(src["count"], dest.count);
				utils::json_utils::get_or(src["production_idx"], dest.idx);
			}))
		{
			return false;
		}

		const auto weapon_iter = [](weapon_t& dest, json::value& src)
		{
			utils::json_utils::get_or(src["ammo_count"], dest.ammo_count);
			utils::json_utils::get_or(src["ammo_idx"], dest.ammo_idx);
			utils::json_utils::get_or(src["init_ammo_count"], dest.init_ammo_count);
			utils::json_utils::get_or(src["init_ammo_idx"], dest.init_ammo_idx);
			utils::json_utils::get_or(src["inventory_index"], dest.inventory_index);
		};

		if (!utils::json_utils::parse_array(main_weapon_list_j, this->main_weapon_list, weapon_iter) ||
			!utils::json_utils::parse_array(sub_weapon_list_j, this->sub_weapon_list, weapon_iter))
		{
			return false;
		}

		if (!utils::json_utils::parse_array(porch_list_j, this->porch_list, [](item_t& dest, json::value& src)
			{
				utils::json_utils::get_or(src, dest.idx);
				utils::json_utils::get_or(src, dest.count);
			}, false))
		{
			return false;
		}

		if (!utils::json_utils::parse_array(skill_list_j, this->skill_list, [](skill_t& dest, json::value& src)
			{
				utils::json_utils::parse_array(src, dest.slot);
			}))
		{
			return false;
		}

		utils::json_utils::get_or(data["class_info"], this->class_info);
		
		if (!utils::json_utils::parse_array(data["survival_list"], this->survival_list, false))
		{
			return false;
		}

		return true;
	}

	void loadout_t::to_json(json::value& data, const std::uint32_t index) const
	{
		data["class_info"] = this->class_info;
		data["index"] = index;

		auto& gadget_list_j = data["gadget_list"];
		for (auto i = 0ull; i < ARRAYSIZE(this->gadget_list); i++)
		{
			auto& entry = gadget_list_j[i];
			entry["count"] = this->gadget_list[i].count;
			entry["production_idx"] = this->gadget_list[i].idx;
		}

		auto& gear_info_j = data["gear_info"];
		gear_info_j["arm_inventory_index"] = this->gear_info.arm_inventory_index;
		gear_info_j["body_inventory_index"] = this->gear_info.body_inventory_index;
		gear_info_j["ext_head_production_idx"] = this->gear_info.ext_head_production_idx;
		gear_info_j["ext_suit_production_idx"] = this->gear_info.ext_suit_production_idx;
		gear_info_j["head_inventory_index"] = this->gear_info.head_inventory_index;
		gear_info_j["leg_inventory_index"] = this->gear_info.leg_inventory_index;

		auto& main_weapon_list_j = data["main_weapon_list"];
		auto& sub_weapon_list_j = data["sub_weapon_list"];

		for (auto i = 0ull; i < ARRAYSIZE(this->main_weapon_list); i++)
		{
			auto& main_entry = main_weapon_list_j[i];
			auto& sub_entry = sub_weapon_list_j[i];

			main_entry["ammo_count"] = this->main_weapon_list[i].ammo_count;
			main_entry["ammo_idx"] = this->main_weapon_list[i].ammo_idx;
			main_entry["init_ammo_count"] = this->main_weapon_list[i].init_ammo_count;
			main_entry["init_ammo_idx"] = this->main_weapon_list[i].init_ammo_idx;
			main_entry["inventory_index"] = this->main_weapon_list[i].inventory_index;

			sub_entry["ammo_count"] = this->sub_weapon_list[i].ammo_count;
			sub_entry["ammo_idx"] = this->sub_weapon_list[i].ammo_idx;
			sub_entry["init_ammo_count"] = this->sub_weapon_list[i].init_ammo_count;
			sub_entry["init_ammo_idx"] = this->sub_weapon_list[i].init_ammo_idx;
			sub_entry["inventory_index"] = this->sub_weapon_list[i].inventory_index;
		}

		data["name"] = this->name;

		auto& porch_list_j = data["porch_list"];
		for (auto i = 0ull; i < ARRAYSIZE(this->porch_list); i++)
		{
			auto& entry = porch_list_j[i];
			entry["count"] = this->porch_list[i].count;
			entry["production_idx"] = this->porch_list[i].idx;
		}

		auto& skill_list_j = data["skill_list"];
		for (auto i = 0ull; i < ARRAYSIZE(this->skill_list); i++)
		{
			auto& entry = skill_list_j[i];
			entry["slot"] = this->skill_list[i].slot;
		}

		auto& survival_list_j = data["survival_list"];
		for (auto i = 0ull; i < ARRAYSIZE(this->survival_list); i++)
		{
			survival_list_j[i] = this->survival_list[i];
		}
	}

	bool nonstackable_item_t::parse(json::value& data)
	{
		std::memset(this, 0, sizeof(nonstackable_item_t));

		utils::json_utils::get_or(data["color"], this->color);
		utils::json_utils::get_or(data["color2"], this->color2);
		utils::json_utils::get_or(data["flag"], this->flag);
		utils::json_utils::get_or(data["grade"], this->grade);
		utils::json_utils::get_or(data["inventory_index"], this->inventory_index);
		utils::json_utils::get_or(data["life"], this->life);
		utils::json_utils::get_or(data["life_max"], this->life_max);
		utils::json_utils::get_or(data["obtain_order"], this->obtain_order);
		utils::json_utils::get_or(data["option_slot"], this->option_slot);
		utils::json_utils::get_or(data["spec"], this->spec);
		utils::json_utils::get_or(data["production_id"], this->production_id);

		utils::json_utils::parse_array(data["option_list"], this->option_list, [](nonstackable_item_t::option_t& dest, json::value& src)
		{
			utils::json_utils::get_or(src["obtained"], dest.obtained);
			utils::json_utils::get_or(src["option_id"], dest.option_id);
		}, false);

		utils::json_utils::parse_array(data["perk_list"], this->perk_list, [](perk_t& dest, json::value& src)
		{
			utils::json_utils::get_or(src["perk_id"], dest.perk_id);
			utils::json_utils::get_or(src["perk_level"], dest.perk_level);
		}, false);

		return true;
	}

	void nonstackable_item_t::to_json(json::value& data) const
	{
		data["color"] = this->color;
		data["color2"] = this->color2;
		data["flag"] = this->flag;
		data["grade"] = this->grade;
		data["inventory_index"] = this->inventory_index;
		data["life"] = this->life;
		data["life_max"] = this->life_max;
		data["obtain_order"] = this->obtain_order;
		data["spec"] = this->spec;
		data["option_slot"] = this->option_slot;
		data["production_id"] = this->production_id;

		for (auto i = 0ull; i < ARRAYSIZE(this->option_list); i++)
		{
			data["option_list"][i]["obtained"] = this->option_list[i].obtained;
			data["option_list"][i]["option_id"] = this->option_list[i].option_id;
		}

		for (auto i = 0ull; i < ARRAYSIZE(this->perk_list); i++)
		{
			data["perk_list"][i]["perk_id"] = this->perk_list[i].perk_id;
			data["perk_list"][i]["perk_level"] = this->perk_list[i].perk_level;
		}
	}

	bool stackable_item_t::parse(json::value& data)
	{
		stackable_item_t::parseable parsed_data{};

		std::memset(this, 0, sizeof(stackable_item_t));
		if (!json::read(parsed_data, data))
		{
			return false;
		}

		this->flag = parsed_data.flag;
		this->inventory_type = parsed_data.inventory_type;
		this->cbox_index = parsed_data.cbox_index;
		this->inventory_index = parsed_data.inventory_index;
		this->obtain_order = parsed_data.obtain_order;
		this->damaged_in_count = parsed_data.damaged_in_count;
		this->count = parsed_data.count;

		std::uint32_t production_id{};
		if (!json::read(production_id, data["production_id"]))
		{
			return false;
		}

		const auto iter = game::parameters_table.ssd_sbm_parameters->productions.find(production_id);
		if (iter == game::parameters_table.ssd_sbm_parameters->productions.end())
		{
			return false;
		}

		this->production_index = iter->second->index;
		return true;
	}

	void stackable_item_t::to_json(json::value& data) const
	{
		data["cbox_index"] = this->cbox_index;
		data["count"] = this->count;
		data["damaged_in_count"] = this->damaged_in_count;
		data["flag"] = this->flag;
		data["inventory_index"] = this->inventory_index;
		data["inventory_type"] = this->inventory_type;
		data["obtain_order"] = this->obtain_order;
		data["production_id"] = this->get_production_id();
	}

	bool inventory_resource_t::parse(json::value& data)
	{
		std::memset(this, 0, sizeof(inventory_resource_t));

		inventory_resource_t::parseable parsed_data{};
		if (!json::read(parsed_data, data))
		{
			return false;
		}

		this->flag = parsed_data.flag;
		this->inventory_type = parsed_data.inventory_type;
		this->cbox_index = parsed_data.cbox_index;
		this->inventory_index = parsed_data.inventory_index;
		this->obtain_order = parsed_data.obtain_order;
		this->damaged_in_count = parsed_data.damaged_in_count;
		this->count = parsed_data.count;

		std::uint32_t resource_id{};
		if (!json::read(resource_id, data["resource_id"]))
		{
			return false;
		}

		const auto iter = game::parameters_table.ssd_sbm_parameters->resources.find(resource_id);
		if (iter == game::parameters_table.ssd_sbm_parameters->resources.end())
		{
			return false;
		}

		this->resource_index = iter->second->index;
		return true;
	}

	void inventory_resource_t::to_json(json::value& data) const
	{
		data["inventory_index"] = this->inventory_index;
		data["inventory_type"] = this->inventory_type;
		data["cbox_index"] = this->cbox_index;
		data["count"] = this->count;
		data["damaged_in_count"] = this->damaged_in_count;
		data["flag"] = this->flag;
		data["obtain_order"] = this->obtain_order;
		data["resource_id"] = this->get_resource_id();
	}

	void mission_info_t::initialize()
	{
		std::memset(this, 0, sizeof(mission_info_t));
	}

	bool mission_info_t::parse(json::value& data)
	{
		utils::json_utils::get_or(data["equipment_slot"], this->equipment_slot);
		utils::json_utils::get_or(data["flag_mission_code"], this->flag_mission_code);
		utils::json_utils::get_or(data["flag_mission_sequence_number"], this->flag_mission_sequence_number);
		utils::json_utils::get_or(data["hunger"], this->hunger);
		utils::json_utils::get_or(data["hunger_max_keep_time"], this->hunger_max_keep_time);
		utils::json_utils::get_or(data["injury_whole"], this->injury_whole);
		utils::json_utils::get_or(data["life"], this->life);
		utils::json_utils::get_or(data["location_code"], this->location_code);
		utils::json_utils::get_or(data["mission_code"], this->mission_code);
		utils::json_utils::get_or(data["oxygen"], this->oxygen);
		utils::json_utils::get_or(data["sequence_number"], this->sequence_number);
		utils::json_utils::get_or(data["stamina"], this->stamina);
		utils::json_utils::get_or(data["temp_body_id"], this->temp_body_id);
		utils::json_utils::get_or(data["temp_crew_type_code"], this->temp_crew_type_code);
		utils::json_utils::get_or(data["temp_face_id"], this->temp_face_id);
		utils::json_utils::get_or(data["temp_first_name_id"], this->temp_first_name_id);
		utils::json_utils::get_or(data["temp_last_name_id"], this->temp_last_name_id);
		utils::json_utils::get_or(data["temp_race_id"], this->temp_race_id);
		utils::json_utils::get_or(data["temp_sex_id"], this->temp_sex_id);
		utils::json_utils::get_or(data["temp_unique_type_code"], this->temp_unique_type_code);
		utils::json_utils::get_or(data["temp_voice_type"], this->temp_voice_type);
		utils::json_utils::get_or(data["thirst"], this->thirst);
		utils::json_utils::get_or(data["thirst_max_keep_time"], this->thirst_max_keep_time);
		utils::json_utils::get_or(data["tiredness"], this->tiredness);
		utils::json_utils::get_or(data["weather"], this->weather);
		utils::json_utils::get_or(data["pos_x"], this->pos_x);
		utils::json_utils::get_or(data["pos_y"], this->pos_y);
		utils::json_utils::get_or(data["pos_z"], this->pos_z);
		utils::json_utils::get_or(data["rot_y"], this->rot_y);
		utils::json_utils::get_or(data["clock"], this->clock);
		utils::json_utils::get_or(data["survival_sec"], this->survival_sec);

		if (!utils::json_utils::parse_array(data["injury_part"], this->injury_part) ||
			!utils::json_utils::parse_array(data["injury_recovery_time"], this->injury_recovery_time))
		{
			return false;
		}

		const auto status_buffer_iter = [](mission_info_t::status_buffer_t& dest, const json::value& src)
		{
			utils::json_utils::get_or(src, dest.buffer_type);
			utils::json_utils::get_or(src, dest.remaining_time);
		};

		utils::json_utils::parse_array(data["status_buffer"], this->status_buffer, status_buffer_iter);

		if (!utils::json_utils::parse_base64(data["vars"], this->vars))
		{
			return false;
		}
		
		return true;
	}

	void mission_info_t::to_json(json::value& data) const
	{
		data["equipment_slot"] = this->equipment_slot;
		data["flag_mission_code"] = this->flag_mission_code;
		data["flag_mission_sequence_number"] = this->flag_mission_sequence_number;
		data["hunger"] = this->hunger;
		data["hunger_max_keep_time"] = this->hunger_max_keep_time;
		data["injury_whole"] = this->injury_whole;
		data["life"] = this->life;
		data["location_code"] = this->location_code;
		data["mission_code"] = this->mission_code;
		data["oxygen"] = this->oxygen;
		data["sequence_number"] = this->sequence_number;
		data["stamina"] = this->stamina;
		data["temp_body_id"] = this->temp_body_id;
		data["temp_crew_type_code"] = this->temp_crew_type_code;
		data["temp_face_id"] = this->temp_face_id;
		data["temp_first_name_id"] = this->temp_first_name_id;
		data["temp_last_name_id"] = this->temp_last_name_id;
		data["temp_race_id"] = this->temp_race_id;
		data["temp_sex_id"] = this->temp_sex_id;
		data["temp_unique_type_code"] = this->temp_unique_type_code;
		data["temp_voice_type"] = this->temp_voice_type;
		data["thirst"] = this->thirst;
		data["thirst_max_keep_time"] = this->thirst_max_keep_time;
		data["tiredness"] = this->tiredness;
		data["weather"] = this->weather;
		data["pos_x"] = this->pos_x;
		data["pos_y"] = this->pos_y;
		data["pos_z"] = this->pos_z;
		data["rot_y"] = this->rot_y;
		data["clock"] = this->clock;
		data["survival_sec"] = this->survival_sec;
		data["injury_part"] = this->injury_part;
		data["injury_recovery_time"] = this->injury_recovery_time;

		auto& status_buffer_j = data["status_buffer"];
		for (auto i = 0ull; i < ARRAYSIZE(this->status_buffer); i++)
		{
			auto& entry = status_buffer_j[i];
			entry["buffer_type"] = this->status_buffer[i].buffer_type;
			entry["remaining_time"] = this->status_buffer[i].remaining_time;
		}

		data["vars"] = utils::encoding::encode_base64(this->vars);
	}

	bool gimmick_save_data_t::parse(json::value& data)
	{
		if (!data.is_object())
		{
			return false;
		}

		for (auto i = 0; i < 4; i++)
		{
			utils::json_utils::parse_base64(data["instant"][i]["instant"], this->instant[i].data);
			utils::json_utils::parse_base64(data["permanent"][i]["permanent"], this->permanent[i].data);
			utils::json_utils::parse_base64(data["resource_event"][i]["resource_event"], this->resource_event[i].data);
			utils::json_utils::parse_base64(data["resource_normal"][i]["resource_normal"], this->resource_normal[i].data);
			utils::json_utils::parse_base64(data["resource_rare"][i]["resource_rare"], this->resource_rare[i].data);
			utils::json_utils::parse_base64(data["resource_shared"][i]["resource_shared"], this->resource_shared[i].data);
		}

		return true;
	}

	void gimmick_save_data_t::to_json(json::value& data, const std::uint32_t map_location) const
	{
		data["map_location"] = map_location;

		for (auto i = 0; i < 4; i++)
		{
			data["instant"][i]["instant"] = utils::encoding::encode_base64(this->instant[i].data);
			data["permanent"][i]["permanent"] = utils::encoding::encode_base64(this->permanent[i].data);
			data["resource_event"][i]["resource_event"] = utils::encoding::encode_base64(this->resource_event[i].data);
			data["resource_normal"][i]["resource_normal"] = utils::encoding::encode_base64(this->resource_normal[i].data);
			data["resource_rare"][i]["resource_rare"] = utils::encoding::encode_base64(this->resource_rare[i].data);
			data["resource_shared"][i]["resource_shared"] = utils::encoding::encode_base64(this->resource_shared[i].data);
		}
	}

	bool gimmick_resource_info_t::parse(json::value& data)
	{
		utils::json_utils::get_or(data["resource_event_tail"], this->resource_event_tail);
		utils::json_utils::get_or(data["resource_normal_tail"], this->resource_normal_tail);
		utils::json_utils::get_or(data["resource_rare_tail"], this->resource_rare_tail);
		utils::json_utils::get_or(data["resource_shared_tail"], this->resource_shared_tail);
		return true;
	}

	void gimmick_resource_info_t::to_json(json::value& data) const
	{
		data["resource_event_tail"] = this->resource_event_tail;
		data["resource_normal_tail"] = this->resource_normal_tail;
		data["resource_rare_tail"] = this->resource_rare_tail;
		data["resource_shared_tail"] = this->resource_shared_tail;
	}

	bool gimmick_timer_info_t::parse(json::value& data)
	{
		utils::json_utils::get_or(data["resource_timer_global_afghan"], this->resource_timer_global_afghan);
		utils::json_utils::get_or(data["resource_timer_global_africa"], this->resource_timer_global_africa);
		utils::json_utils::get_or(data["resource_timer_stock_afghan"], this->resource_timer_stock_afghan);
		utils::json_utils::get_or(data["resource_timer_stock_africa"], this->resource_timer_stock_africa);
		return true;
	}

	void gimmick_timer_info_t::to_json(json::value& data) const
	{
		data["resource_timer_global_afghan"] = this->resource_timer_global_afghan;
		data["resource_timer_global_africa"] = this->resource_timer_global_africa;
		data["resource_timer_stock_afghan"] = this->resource_timer_stock_afghan;
		data["resource_timer_stock_africa"] = this->resource_timer_stock_africa;
	}

	bool base_resources_t::parse_base(json::value& base)
	{
		utils::json_utils::get_or(base["bad_status_1_risk"], this->params.bad_status_1_risk);
		utils::json_utils::get_or(base["bad_status_2_risk"], this->params.bad_status_2_risk);
		utils::json_utils::get_or(base["bad_status_3_risk"], this->params.bad_status_3_risk);
		utils::json_utils::get_or(base["bad_status_4_risk"], this->params.bad_status_4_risk);
		utils::json_utils::get_or(base["clean_water"], this->params.clean_water);
		utils::json_utils::get_or(base["dirty_water"], this->params.dirty_water);
		utils::json_utils::get_or(base["food"], this->params.food);
		utils::json_utils::get_or(base["medical_supplies"], this->params.medical_supplies);
		utils::json_utils::get_or(base["party_item_id"], this->params.party_item_id);
		utils::json_utils::get_or(base["party_item_updates"], this->params.party_item_updates);
		utils::json_utils::get_or(base["total_number_of_updates"], this->params.total_number_of_updates);
		return true;
	}

	bool base_resources_t::parse_counts(json::value& count)
	{
		for (auto i = 0ull; i < ARRAYSIZE(this->resource_counts); i++)
		{
			const auto key = std::format("c{:02}", i);
			utils::json_utils::get_or(count[key], this->resource_counts[i]);
		}

		utils::json_utils::get_or(count["next_update_time"], this->next_update_time);
		utils::json_utils::get_or(count["update_remaining_time"], this->update_remaining_time);
		return true;
	}

	bool base_resources_t::parse_animals(json::value& animal_list)
	{
		if (!animal_list.is_array())
		{
			return false;
		}

		const auto size = std::min(ARRAYSIZE(this->animals), animal_list.size());
		for (auto i = 0ull; i < size; i++)
		{
			auto& entry = animal_list[i];
			auto& animal_kind_j = entry["animal_kind"];
			auto& num_j = entry["num"];

			if (!animal_kind_j.is_uint64() || !num_j.is_uint64())
			{
				continue;
			}

			const auto animal_kind = animal_kind_j.as<std::uint32_t>();
			const auto num = num_j.as<std::uint32_t>();

			if (animal_kind >= ARRAYSIZE(this->animals))
			{
				continue;
			}

			this->animals[animal_kind] = num;
		}

		return true;
	}

	void base_resources_t::to_json(json::value& data) const
	{
		auto& base_resource_j = data["base_resource"];
		auto& resource_count_j = data["base_resource_count"];
		auto& animals_j = data["animal_list"];

		base_resource_j["bad_status_1_risk"] = this->params.bad_status_1_risk;
		base_resource_j["bad_status_2_risk"] = this->params.bad_status_2_risk;
		base_resource_j["bad_status_3_risk"] = this->params.bad_status_3_risk;
		base_resource_j["bad_status_4_risk"] = this->params.bad_status_4_risk;
		base_resource_j["clean_water"] = this->params.clean_water;
		base_resource_j["dirty_water"] = this->params.dirty_water;
		base_resource_j["food"] = this->params.food;
		base_resource_j["medical_supplies"] = this->params.medical_supplies;
		base_resource_j["party_item_id"] = this->params.party_item_id;
		base_resource_j["party_item_updates"] = this->params.party_item_updates;
		base_resource_j["total_number_of_updates"] = this->params.total_number_of_updates;

		for (auto i = 0ull; i < ARRAYSIZE(this->animals); i++)
		{
			animals_j[i]["animal_kind"] = i;
			animals_j[i]["num"] = this->animals[i];
		}

		for (auto i = 0ull; i < ARRAYSIZE(this->resource_counts); i++)
		{
			const auto key = std::format("c{:02}", i);
			resource_count_j[key] = this->resource_counts[i];
		}

		resource_count_j["next_update_time"] = this->next_update_time;
		resource_count_j["update_remaining_time"] = this->update_remaining_time;
	}

	bool mission_record_list_t::open_mission(const std::uint16_t mission_code)
	{
		if (!game::mission_list.contains(mission_code))
		{
			return false;
		}

		mission_record_t record{};
		record.mission_code = mission_code;
		return this->try_add_item(record, false);
	}

	bool mission_record_t::parse(json::value& data)
	{
		if (!json::read(*this, data))
		{
			return false;
		}

		return game::mission_list.contains(this->mission_code);
	}

	void mission_record_t::to_json(json::value& data) const
	{
		data["clear_flag"] = this->clear_flag;
		data["clear_rank"] = 0;
		data["clear_time"] = 0;
		data["mission_code"] = this->mission_code;
		data["new_flag"] = 0;
		data["score"] = 0;
	}

	bool quest_record_t::parse(json::value& data)
	{
		if (!json::read(*this, data))
		{
			return false;
		}

		return game::quest_list.contains(this->mission_code);
	}

	void quest_record_t::to_json(json::value& data) const
	{
		data["flagset"] = this->flagset;
		data["mission_code"] = this->mission_code;
		data["repop_count"] = this->repop_count;
	}

	bool map_unlock_t::parse(json::value& data)
	{
		utils::json_utils::get_or(data, this->value); 
		return true;
	}

	void map_unlock_t::to_json(json::value& data) const
	{
		data = this->value;
	}

	bool story_unlock_info_t::parse(json::value& data)
	{
		utils::json_utils::get_or(data["demo_open_flag"], this->demo_open_flag);
		utils::json_utils::get_or(data["facility_new_flag"], this->facility_new_flag);
		utils::json_utils::get_or(data["marker_map_location"], this->marker_map_location);
		utils::json_utils::get_or(data["oxygen_supply_unlock"], this->oxygen_supply_unlock);
		utils::json_utils::get_or(data["story_sequence_number"], this->story_sequence_number);

		utils::json_utils::parse_base64(data["marker_africa"], this->marker_africa);
		utils::json_utils::parse_base64(data["marker_afghan"], this->marker_afghan);
		utils::json_utils::parse_base64(data["fast_travel_unlock"], this->fast_travel_unlock, true, true);
		return true;
	}

	void story_unlock_info_t::to_json(json::value& data) const
	{
		data["demo_open_flag"] = this->demo_open_flag;
		data["facility_new_flag"] = this->facility_new_flag;
		data["marker_map_location"] = this->marker_map_location;
		data["oxygen_supply_unlock"] = this->oxygen_supply_unlock;
		data["story_sequence_number"] = this->story_sequence_number;
		data["marker_afghan"] = utils::encoding::encode_base64(this->marker_afghan);
		data["marker_africa"] = utils::encoding::encode_base64(this->marker_africa);
		data["fast_travel_unlock"] = utils::encoding::encode_base64(this->fast_travel_unlock);
	}

	bool story_unlock_info_t::tips_open_info_t::parse(json::value& data)
	{
		return utils::json_utils::parse_base64(data["data"], this->value);
	}

	void story_unlock_info_t::tips_open_info_t::to_json(json::value& data) const
	{
		if (database::vars.disable_tutorial_tips)
		{
			std::uint8_t tips_data[128]{};
			std::memset(tips_data, 0xFF, sizeof(tips_data));
			data["data"] = utils::encoding::encode_base64(tips_data);
		}
		else
		{
			data["data"] = utils::encoding::encode_base64(this->value);
		}
	}

	bool building_info_t::cell_edge_t::parse(json::value& data, std::uint32_t& row, std::uint32_t& column, const std::uint32_t type)
	{
		if (type == edge_type_center)
		{
			utils::json_utils::get_or(data["extra_data"], this->extra_data, this->extra_data);
			utils::json_utils::get_or(data["rotation"], this->rotation, this->rotation);
		}

		utils::json_utils::get_or(data["life"], this->life, this->life);
		utils::json_utils::get_or(data["max_life"], this->max_life, this->max_life);
		utils::json_utils::get_or(data["production_id"], this->production_index, this->production_index);
		utils::json_utils::get_or(data["completion_remaining_time"], this->completion_remaining_time, this->completion_remaining_time);
		utils::json_utils::get_or(data["recovery_time"], this->recovery_time, this->recovery_time);
		utils::json_utils::get_or(data["row"], row);
		utils::json_utils::get_or(data["column"], column);

		if (row >= building_grid_size || column >= building_grid_size)
		{
			return false;
		}

		return true;
	}

	void building_info_t::cell_edge_t::to_json(json::value& data, const std::uint32_t row, const std::uint32_t column, const std::uint32_t type) const
	{
		if (type == edge_type_center)
		{
			data["rotation"] = this->rotation;
			data["extra_data"] = this->extra_data;
		}

		data["life"] = this->life;
		data["max_life"] = this->max_life;
		data["production_id"] = this->production_index;
		data["completion_remaining_time"] = this->completion_remaining_time;
		data["recovery_time"] = this->recovery_time;
		data["row"] = row;
		data["column"] = column;
	}

	bool building_info_t::farming_info_t::parse(json::value& data, std::uint32_t& row, std::uint32_t& column)
	{
		utils::json_utils::get_or(data["animal00"], this->animal[0]);
		utils::json_utils::get_or(data["animal01"], this->animal[1]);
		utils::json_utils::get_or(data["animal02"], this->animal[2]);
		utils::json_utils::get_or(data["animal03"], this->animal[3]);
		utils::json_utils::get_or(data["animal04"], this->animal[4]);
		utils::json_utils::get_or(data["animal05"], this->animal[5]);
		utils::json_utils::get_or(data["animal06"], this->animal[6]);
		utils::json_utils::get_or(data["animal07"], this->animal[7]);
		utils::json_utils::get_or(data["item00"], this->item[0]);
		utils::json_utils::get_or(data["item01"], this->item[1]);
		utils::json_utils::get_or(data["item02"], this->item[2]);
		utils::json_utils::get_or(data["item03"], this->item[3]);
		utils::json_utils::get_or(data["item04"], this->item[4]);
		utils::json_utils::get_or(data["item05"], this->item[5]);
		utils::json_utils::get_or(data["item06"], this->item[6]);
		utils::json_utils::get_or(data["item07"], this->item[7]);
		utils::json_utils::get_or(data["item08"], this->item[8]);
		utils::json_utils::get_or(data["item09"], this->item[9]);
		utils::json_utils::get_or(data["column"], column);
		utils::json_utils::get_or(data["row"], row);

		if (row >= building_grid_size || column >= building_grid_size)
		{
			return false;
		}

		return true;
	}

	void building_info_t::farming_info_t::to_json(json::value& data, const std::uint32_t row, const std::uint32_t column) const
	{
		data["animal00"] = this->animal[0];
		data["animal01"] = this->animal[1];
		data["animal02"] = this->animal[2];
		data["animal03"] = this->animal[3];
		data["animal04"] = this->animal[4];
		data["animal05"] = this->animal[5];
		data["animal06"] = this->animal[6];
		data["animal07"] = this->animal[7];
		data["item00"] = this->item[0];
		data["item01"] = this->item[1];
		data["item02"] = this->item[2];
		data["item03"] = this->item[3];
		data["item04"] = this->item[4];
		data["item05"] = this->item[5];
		data["item06"] = this->item[6];
		data["item07"] = this->item[7];
		data["item08"] = this->item[8];
		data["item09"] = this->item[9];
		data["column"] = column;
		data["row"] = row;
	}

	bool building_info_t::parse_type(json::value& data, const std::uint32_t type)
	{
		if (!data.is_array())
		{
			return false;
		}

		for (auto i = 0ull; i < data.size(); i++)
		{
			cell_edge_t cell_edge{};
			std::uint32_t row{};
			std::uint32_t column{};
			if (!cell_edge.parse(data[i], row, column, type))
			{
				continue;
			}

			std::memcpy(&this->cells[row][column].edges[type], &cell_edge, sizeof(cell_edge_t));
		}

		return true;
	}

	bool building_info_t::parse_farming(json::value& data)
	{
		if (!data.is_array())
		{
			return false;
		}

		for (auto i = 0ull; i < data.size(); i++)
		{
			farming_info_t farming{};
			std::uint32_t row{};
			std::uint32_t column{};
			if (!farming.parse(data[i], row, column))
			{
				continue;
			}

			std::memcpy(&this->cells[row][column].farming_info, &farming, sizeof(farming_info_t));
		}

		return true;
	}

	bool building_info_t::parse(json::value& data, const bool is_diff)
	{
		if (!is_diff)
		{
			std::memset(this, 0, sizeof(building_info_t));
		}

		this->parse_type(data["center_info"], edge_type_center);
		this->parse_type(data["upper_edge_info"], edge_type_upper);
		this->parse_type(data["left_edge_info"], edge_type_left);

		return true;
	}

	void building_info_t::to_json(json::value& data) const
	{
		for (auto row = 0u; row < building_grid_size; row++)
		{
			for (auto col = 0u; col < building_grid_size; col++)
			{
				auto& cell = this->cells[row][col];
				const auto do_edge = [&](json::value& edge, const std::uint32_t type)
				{
					if (cell.edges[type].life == 0)
					{
						return;
					}

					cell.edges[type].to_json(edge[edge.size()], row, col, type);
				};

				do_edge(data["center_info"], edge_type_center);
				do_edge(data["upper_edge_info"], edge_type_upper);
				do_edge(data["left_edge_info"], edge_type_left);
			}
		}
	}

	void building_info_t::to_json_farming(json::value& data) const
	{
		for (auto row = 0u; row < building_grid_size; row++)
		{
			for (auto col = 0u; col < building_grid_size; col++)
			{
				auto& cell = this->cells[row][col];

				farming_info_t dummy_info{};
				if (std::memcmp(&cell.farming_info, &dummy_info, sizeof(dummy_info)) != 0)
				{
					cell.farming_info.to_json(data[data.size()], row, col);
				}
			}
		}
	}

	void building_info_t::load_default(const std::uint32_t map_location)
	{
		static const auto default_building = [&]()
		{
			static building_info_t data{};

			json::value default_data = get_default_data("building_info");
			data.parse_type(default_data[map_location]["center_info"], edge_type_center);
			data.parse_type(default_data[map_location]["upper_edge_info"], edge_type_upper);
			data.parse_type(default_data[map_location]["left_edge_info"], edge_type_left);

			return &data;
		}();

		std::memcpy(this, default_building, sizeof(building_info_t));
	}

	bool nonstackable_item_list_t::parse_life_diff(json::value& data)
	{
		if (!data.is_array())
		{
			return false;
		}

		const auto count = std::min(data.size(), this->max_size());
		for (auto i = 0ull; i < count; i++)
		{
			nonstackable_item_t target_item{};
			if (!target_item.parse(data[i]))
			{
				continue;
			}

			for (auto o = 0ull; o < this->size(); o++)
			{
				auto& item = this->operator[](o);
				if (item.inventory_index == target_item.inventory_index)
				{
					item.life = target_item.life;
				}
			}
		}

		return true;
	}

	bool nonstackable_item_list_t::find_free_index(std::uint16_t& index, std::uint32_t& obtain_order)
	{
		index = 0u;
		obtain_order = 0u;

		for (auto i = 0u; i < this->size(); )
		{
			auto& entry = this->operator[](i);
			obtain_order = std::max(obtain_order, entry.obtain_order);

			if (entry.inventory_index == index)
			{
				++index;
				i = 0u;
				continue;
			}
			else
			{
				++i;
			}
		}

		++obtain_order;
		return index < max_nonstackable_items;
	}

	nonstackable_item_t* nonstackable_item_list_t::find_at_index(const std::uint16_t inventory_index)
	{
		const auto iter = std::ranges::find_if(this->begin(), this->end(), [&](const auto& item)
		{
			return item.inventory_index == inventory_index;
		});

		if (iter == this->end())
		{
			return nullptr;
		}

		return &(*iter);
	}

	bool inventory_resource_list_t::find_free_index(const std::uint8_t inventory_type, std::uint16_t& index, std::uint16_t& obtain_order) const
	{
		index = 0u;
		obtain_order = 0u;

		for (auto i = 0u; i < this->size(); )
		{
			auto& entry = this->operator[](i);
			obtain_order = std::max(obtain_order, static_cast<std::uint16_t>(entry.obtain_order));

			if (entry.inventory_index == index && entry.inventory_type == inventory_type && entry.resource_index != 0)
			{
				++index;
				i = 0u;
				continue;
			}
			else
			{
				++i;
			}
		}

		++obtain_order;
		return index < max_resources;
	}

	bool inventory_resource_list_t::add_resource(inventory_resource_t& resource, const std::uint8_t inventory_type)
	{
		std::int64_t free_index = -1;
		for (auto o = 0ull; o < this->size(); o++)
		{
			auto& entry = this->operator[](o);
			if (entry.resource_index == resource.resource_index && entry.inventory_type == inventory_type &&
				entry.count < max_item_count)
			{
				const auto add_count = std::min(resource.count, max_item_count - entry.count);
				entry.count += add_count;
				if (add_count == resource.count)
				{
					std::memcpy(&resource, &entry, sizeof(inventory_resource_t));
					return true;
				}
				else
				{
					resource.count -= add_count;
				}
			}
			else if (this->is_element_empty(this->operator[](o)) && free_index == -1)
			{
				free_index = static_cast<std::int64_t>(o);
			}
		}

		std::uint16_t inventory_index{};
		std::uint16_t obtain_order{};
		resource.inventory_type = inventory_type;
		if (!this->find_free_index(resource.inventory_type, inventory_index, obtain_order))
		{
			return false;
		}

		resource.inventory_index = inventory_index;
		resource.obtain_order = obtain_order;

		if (free_index != -1)
		{
			auto& entry = this->operator[](free_index);
			std::memcpy(&entry, &resource, sizeof(inventory_resource_t));
			return true;
		}

		return this->push(resource);
	}

	inventory_resource_t* inventory_resource_list_t::get_entry(const std::uint16_t inventory_index, const std::uint8_t inventory_type)
	{
		for (auto i = 0u; i < this->size(); i++)
		{
			auto& entry = this->operator[](i);
			if (entry.inventory_index == inventory_index && entry.inventory_type == inventory_type)
			{
				return &entry;
			}
		}

		return nullptr;
	}

	stackable_item_t* stackable_item_list_t::find_item(const std::uint32_t production_id)
	{
		for (auto i = 0u; i < this->size(); i++)
		{
			if (this->operator[](i).get_production_id() == production_id)
			{
				return &this->operator[](i);
			}
		}

		return nullptr;
	}

	stackable_item_t* stackable_item_list_t::get_entry(const std::uint16_t inventory_index, const std::uint8_t inventory_type)
	{
		for (auto i = 0u; i < this->size(); i++)
		{
			auto& entry = this->operator[](i);
			if (entry.inventory_index == inventory_index && entry.inventory_type == inventory_type)
			{
				return &entry;
			}
		}

		return nullptr;
	}

	bool stackable_item_list_t::find_free_index(const std::uint8_t inventory_type, std::uint16_t& index, std::uint16_t& obtain_order) const
	{
		index = 0u;
		obtain_order = 0u;

		for (auto i = 0u; i < this->size(); )
		{
			auto& entry = this->operator[](i);
			obtain_order = std::max(obtain_order, static_cast<std::uint16_t>(entry.obtain_order));

			if (entry.inventory_index == index && entry.inventory_type == inventory_type && entry.production_index != 0)
			{
				++index;
				i = 0u;
				continue;
			}
			else
			{
				++i;
			}
		}

		++obtain_order;
		return index < max_stackable_items;
	}

	bool stackable_item_list_t::add_item(stackable_item_t& item, const std::uint8_t inventory_type)
	{
		std::int64_t free_index = -1;
		for (auto o = 0ull; o < this->size(); o++)
		{
			auto& entry = this->operator[](o);
			if (entry.production_index == item.production_index && entry.inventory_type == inventory_type &&
				entry.count < max_item_count)
			{
				const auto add_count = std::min(item.count, max_item_count - entry.count);
				entry.count += add_count;
				if (add_count == item.count)
				{
					std::memcpy(&item, &entry, sizeof(stackable_item_t));
					return true;
				}
				else
				{
					item.count -= add_count;
				}
			}
			else if (this->is_element_empty(this->operator[](o)) && free_index == -1)
			{
				free_index = static_cast<std::int64_t>(o);
			}
		}

		item.inventory_type = inventory_type;
		std::uint16_t inventory_index{};
		std::uint16_t obtain_order{};
		if (!this->find_free_index(item.inventory_type, inventory_index, obtain_order))
		{
			return false;
		}

		item.inventory_index = inventory_index;
		item.obtain_order = inventory_index;

		if (free_index != -1)
		{
			auto& entry = this->operator[](free_index);
			std::memcpy(&entry, &item, sizeof(stackable_item_t));
			return true;
		}

		return this->push(item);
	}

	bool crew_member_t::parse(json::value& data)
	{
		utils::json_utils::get_or(data["ability_accessory"], this->ability_accessory);
		utils::json_utils::get_or(data["ability_animal"], this->ability_animal);
		utils::json_utils::get_or(data["ability_base_defense"], this->ability_base_defense);
		utils::json_utils::get_or(data["ability_defense_unit"], this->ability_defense_unit);
		utils::json_utils::get_or(data["ability_develop"], this->ability_develop);
		utils::json_utils::get_or(data["ability_expedition"], this->ability_expedition);
		utils::json_utils::get_or(data["ability_food"], this->ability_food);
		utils::json_utils::get_or(data["ability_gadget"], this->ability_gadget);
		utils::json_utils::get_or(data["ability_medical"], this->ability_medical);
		utils::json_utils::get_or(data["ability_plant"], this->ability_plant);
		utils::json_utils::get_or(data["condition"], this->condition);
		utils::json_utils::get_or(data["current_group"], this->current_group);
		utils::json_utils::get_or(data["first_name_index"], this->first_name_index);
		utils::json_utils::get_or(data["health_flag"], this->health_flag);
		utils::json_utils::get_or(data["initial_max_life"], this->initial_max_life);
		utils::json_utils::get_or(data["injury_id_1"], this->injury_id_1);
		utils::json_utils::get_or(data["injury_id_2"], this->injury_id_2);
		utils::json_utils::get_or(data["injury_time_1"], this->injury_time_1);
		utils::json_utils::get_or(data["injury_time_2"], this->injury_time_2);
		utils::json_utils::get_or(data["item1_count"], this->item1_count);
		utils::json_utils::get_or(data["item2_count"], this->item2_count);
		utils::json_utils::get_or(data["item3_count"], this->item3_count);
		utils::json_utils::get_or(data["item4_count"], this->item4_count);
		utils::json_utils::get_or(data["item5_count"], this->item5_count);
		utils::json_utils::get_or(data["item6_count"], this->item6_count);
		utils::json_utils::get_or(data["last_name_index"], this->last_name_index);
		utils::json_utils::get_or(data["life"], this->life);
		utils::json_utils::get_or(data["map_location"], this->map_location);
		utils::json_utils::get_or(data["max_life"], this->max_life);
		utils::json_utils::get_or(data["previous_group"], this->previous_group);
		utils::json_utils::get_or(data["previous_job"], this->previous_job);
		utils::json_utils::get_or(data["race_id"], this->race_id);
		utils::json_utils::get_or(data["resistance_food_shortage"], this->resistance_food_shortage);
		utils::json_utils::get_or(data["resistance_sleepless"], this->resistance_sleepless);
		utils::json_utils::get_or(data["resistance_water_shortage"], this->resistance_water_shortage);
		utils::json_utils::get_or(data["sanity"], this->sanity);
		utils::json_utils::get_or(data["sickness_id_1"], this->sickness_id_1);
		utils::json_utils::get_or(data["sickness_id_2"], this->sickness_id_2);
		utils::json_utils::get_or(data["sickness_time_1"], this->sickness_time_1);
		utils::json_utils::get_or(data["sickness_time_2"], this->sickness_time_2);
		utils::json_utils::get_or(data["skill"], this->skill);
		utils::json_utils::get_or(data["survival_days"], this->survival_days);
		utils::json_utils::get_or(data["voice_type"], this->voice_type);
		utils::json_utils::get_or(data["body_id"], this->body_id);
		utils::json_utils::get_or(data["face_id"], this->face_id);
		utils::json_utils::get_or(data["generation_date"], this->generation_date);
		utils::json_utils::get_or(data["unique_id"], this->unique_id);
		utils::json_utils::get_or(data["sex_id"], this->sex_id);
		utils::json_utils::get_or(data["unique_index"], this->unique_index);
		utils::json_utils::parse_array(data["motivation_history_event"], this->motivation_history_event);
		utils::json_utils::parse_array(data["motivation_history_value"], this->motivation_history_value);
		utils::json_utils::parse_string(data["nickname"], this->nickname);
		return true;
	}
	
	void crew_member_t::apply_update(const crew_member_t& diff)
	{
		this->condition = diff.condition;
		this->health_flag = diff.health_flag;
		this->injury_id_1 = diff.injury_id_1;
		this->injury_id_2 = diff.injury_id_2;
		this->injury_time_1 = diff.injury_time_1;
		this->injury_time_2 = diff.injury_time_2;
		this->life = diff.life;
		this->max_life = diff.max_life;
		this->previous_group = diff.previous_group;
		this->sanity = diff.sanity;
		this->sickness_id_1 = diff.sickness_id_1;
		this->sickness_id_2 = diff.sickness_id_2;
		this->sickness_time_1 = diff.sickness_time_1;
		this->sickness_time_2 = diff.sickness_time_2;
		this->survival_days = diff.survival_days;
		std::memcpy(this->motivation_history_event, diff.motivation_history_event, sizeof(this->motivation_history_event));
		std::memcpy(this->motivation_history_value, diff.motivation_history_value, sizeof(this->motivation_history_value));
	}

	bool crew_member_t::parse_update(json::value& data)
	{
		utils::json_utils::get_or(data["condition"], this->condition);
		utils::json_utils::get_or(data["health_flag"], this->health_flag);
		utils::json_utils::get_or(data["injury_id_1"], this->injury_id_1);
		utils::json_utils::get_or(data["injury_id_2"], this->injury_id_2);
		utils::json_utils::get_or(data["injury_time_1"], this->injury_time_1);
		utils::json_utils::get_or(data["injury_time_2"], this->injury_time_2);
		utils::json_utils::get_or(data["life"], this->life, this->life);
		utils::json_utils::get_or(data["max_life"], this->max_life, this->life);
		utils::json_utils::get_or(data["previous_group"], this->previous_group);
		utils::json_utils::get_or(data["sanity"], this->sanity);
		utils::json_utils::get_or(data["sickness_id_1"], this->sickness_id_1);
		utils::json_utils::get_or(data["sickness_id_2"], this->sickness_id_2);
		utils::json_utils::get_or(data["sickness_time_1"], this->sickness_time_1);
		utils::json_utils::get_or(data["sickness_time_2"], this->sickness_time_2);
		utils::json_utils::get_or(data["survival_days"], this->survival_days);
		utils::json_utils::parse_array(data["motivation_history_event"], this->motivation_history_event);
		utils::json_utils::parse_array(data["motivation_history_value"], this->motivation_history_value);
		return true;
	}

	bool crew_member_t::parse_add_param(json::value& data)
	{
		//utils::json_utils::get_or(data["banish_unique_id"], this->banish_unique_id);
        utils::json_utils::get_or(data["body_id"], this->body_id);
        //utils::json_utils::get_or(data["crew_type_code"], this->crew_type_code);
        utils::json_utils::get_or(data["face_id"], this->face_id);
        //utils::json_utils::get_or(data["first_name_id"], this->first_name_id);
        utils::json_utils::get_or(data["generation_date"], this->generation_date);
        utils::json_utils::get_or(data["item1_count"], this->item1_count);
        utils::json_utils::get_or(data["item2_count"], this->item2_count);
        utils::json_utils::get_or(data["item3_count"], this->item3_count);
        utils::json_utils::get_or(data["item4_count"], this->item4_count);
        utils::json_utils::get_or(data["item5_count"], this->item5_count);
        utils::json_utils::get_or(data["item6_count"], this->item6_count);
        //utils::json_utils::get_or(data["last_name_id"], this->last_name_id);
        utils::json_utils::get_or(data["map_location"], this->map_location);
        utils::json_utils::get_or(data["race_id"], this->race_id);
        utils::json_utils::get_or(data["sex_id"], this->sex_id);

		if (this->map_location > 1)
		{
			return false;
		}

		std::uint32_t crew_type_code{};
		std::uint32_t unique_type_code{};

		utils::json_utils::get_or(data["crew_type_code"], crew_type_code);
		utils::json_utils::get_or(data["unique_type_code"], unique_type_code);

		if (crew_type_code != 0u)
		{
			this->unique_index = crew_type_code;
		}
		else if (unique_type_code != 0u)
		{
			this->unique_index = unique_type_code;
		}
		else
		{
			return false;
		}

		utils::json_utils::get_or(data["voice_type"], this->voice_type);
		return true;
	}

	void crew_member_t::to_json(json::value& data) const
	{
		data["ability_accessory"] = this->ability_accessory;
		data["ability_animal"] = this->ability_animal;
		data["ability_base_defense"] = this->ability_base_defense;
		data["ability_defense_unit"] = this->ability_defense_unit;
		data["ability_develop"] = this->ability_develop;
		data["ability_expedition"] = this->ability_expedition;
		data["ability_food"] = this->ability_food;
		data["ability_gadget"] = this->ability_gadget;
		data["ability_medical"] = this->ability_medical;
		data["ability_plant"] = this->ability_plant;
		data["condition"] = this->condition;
		data["current_group"] = this->current_group;
		data["first_name_index"] = this->first_name_index;
		data["health_flag"] = this->health_flag;
		data["initial_max_life"] = this->initial_max_life;
		data["injury_id_1"] = this->injury_id_1;
		data["injury_id_2"] = this->injury_id_2;
		data["injury_time_1"] = this->injury_time_1;
		data["injury_time_2"] = this->injury_time_2;
		data["item1_count"] = this->item1_count;
		data["item2_count"] = this->item2_count;
		data["item3_count"] = this->item3_count;
		data["item4_count"] = this->item4_count;
		data["item5_count"] = this->item5_count;
		data["item6_count"] = this->item6_count;
		data["last_name_index"] = this->last_name_index;
		data["life"] = this->life;
		data["map_location"] = this->map_location;
		data["max_life"] = this->max_life;
		data["previous_group"] = this->previous_group;
		data["previous_job"] = this->previous_job;
		data["race_id"] = this->race_id;
		data["resistance_food_shortage"] = this->resistance_food_shortage;
		data["resistance_sleepless"] = this->resistance_sleepless;
		data["resistance_water_shortage"] = this->resistance_water_shortage;
		data["sanity"] = this->sanity;
		data["sickness_id_1"] = this->sickness_id_1;
		data["sickness_id_2"] = this->sickness_id_2;
		data["sickness_time_1"] = this->sickness_time_1;
		data["sickness_time_2"] = this->sickness_time_2;
		data["skill"] = this->skill;
		data["survival_days"] = this->survival_days;
		data["voice_type"] = this->voice_type;
		data["body_id"] = this->body_id;
		data["face_id"] = this->face_id;
		data["generation_date"] = this->generation_date;
		data["unique_id"] = this->unique_id;
		data["sex_id"] = this->sex_id;
		data["unique_index"] = this->unique_index;

		for (auto i = 0ull; i < ARRAYSIZE(this->motivation_history_event); i++)
		{
			data["motivation_history_event"][i] = this->motivation_history_event[i];
			data["motivation_history_value"][i] = this->motivation_history_value[i];
		}

		data["nickname"] = this->nickname;
	}

	bool crew_member_list_t::parse_update(json::value& data)
	{
		if (!data.is_array())
		{
			return false;
		}

		const auto count = std::min(data.size(), this->max_size());
		for (auto i = 0ull; i < count; i++)
		{
			crew_member_t new_item{};
			if (!new_item.parse_update(data[i]) || new_item.unique_id == 0)
			{
				continue;
			}

			for (auto o = 0ull; o < this->size(); o++)
			{
				auto& entry = this->operator[](o);
				if (entry.unique_id == new_item.unique_id)
				{
					entry.apply_update(new_item);
				}
			}
		}

		return true;
	}

	crew_member_t* crew_member_list_t::find_member(const std::uint32_t id)
	{
		const auto iter = std::ranges::find_if(this->begin(), this->end(), [&](const crew_member_t& member)
		{
			return member.unique_id == id;
		});

		if (iter == this->end())
		{
			return nullptr;
		}

		return &(*iter);
	}

	bool crew_levels_t::parse(json::value& data)
	{
		utils::json_utils::get_or(data["base_defense"], this->base_defense);
		utils::json_utils::get_or(data["combat_deploy"], this->combat_deploy);
		utils::json_utils::get_or(data["develop"], this->develop);
		utils::json_utils::get_or(data["food"], this->food);
		utils::json_utils::get_or(data["medic"], this->medic);
		utils::json_utils::get_or(data["plant"], this->plant);
		return true;
	}

	void crew_levels_t::to_json(json::value& data) const
	{
		data["base_defense"] = this->base_defense;
		data["combat_deploy"] = this->combat_deploy;
		data["develop"] = this->develop;
		data["food"] = this->food;
		data["medic"] = this->medic;
		data["plant"] = this->plant;
	}

	bool defense_mission_record_t::parse(json::value& data)
	{
		if (!json::read(*this, data))
		{
			return false;
		}

		return game::parameters_table.ssd_base_defense_settings->mission_settings.contains(this->mission_code);
	}

	void defense_mission_record_t::to_json(json::value& data) const
	{
		data["mission_code"] = this->mission_code;
		data["cleared"] = this->cleared;
		data["clear_rank"] = this->clear_rank;
		data["clear_rank"] = this->clear_rank;
		data["iris_score"] = this->iris_score;
		data["waves"] = this->waves;
	}

	bool defense_mission_info_t::status_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	void defense_mission_info_t::status_t::to_json(json::value& data) const
	{
		data["mining_machine_life"] = this->mining_machine_life;
	}

	bool defense_mission_info_t::parameter_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	void defense_mission_info_t::parameter_t::to_json(json::value& data) const
	{
		data["flag"] = this->flag;
		data["threat"] = this->threat;
		data["threat_threshold"] = this->threat_threshold;
		data["attack_time"] = this->attack_time;
	}

	void defense_mission_info_t::initialize()
	{
		std::memset(this, 0, sizeof(defense_mission_info_t));
		this->status.mining_machine_life = 9360;
	}

	bool communication_gesture_info_t::parse(json::value& data)
	{
		if (!data.is_object())
		{
			return false;
		}

		if (!utils::json_utils::parse_array(data["communication_slot"], this->communication_slot) ||
			!utils::json_utils::parse_array(data["communication_type"], this->communication_type) ||
			!utils::json_utils::parse_array(data["gesture_slot"], this->gesture_slot))
		{
			return false;
		}

		return true;
	}

	void communication_gesture_info_t::to_json(json::value& data) const
	{
		for (auto i = 0; i < ARRAYSIZE(this->communication_slot); i++)
		{
			data["communication_slot"][i] = this->communication_slot[i];
			data["communication_type"][i] = this->communication_type[i];
			data["gesture_slot"][i] = this->gesture_slot[i];
		}
	}

	void communication_gesture_info_t::initialize()
	{
		static const auto default_info = []()
		{
			static communication_gesture_info_t data{};
			json::value default_data = get_default_data("communication_gesture_slot");
			data.parse(default_data);
			return &data;
		}();

		std::memcpy(this, default_info, sizeof(communication_gesture_info_t));
	}

	GET_FIELD_C(player, std::uint64_t, player_id);
	GET_FIELD_C(player, std::uint64_t, user_id);
	GET_FIELD_C(player, std::uint64_t, account_id);
	GET_FIELD_C(player, std::uint64_t, index);
	GET_FIELD_C(player, std::uint32_t, playtime);
	GET_FIELD_C(player, std::uint32_t, nameplate);
	GET_FIELD_C(player, std::uint32_t, point);
	GET_FIELD_C(player, std::chrono::microseconds, creation_date);

	std::uint32_t player::get_current_loadout() const
	{
		if (this->current_loadout_ >= max_loadout_count)
		{
			return 0u;
		}

		return this->current_loadout_;
	}

	std::uint32_t player::get_loadout_count() const
	{
		if (this->loadout_count_ >= max_loadout_count)
		{
			return max_loadout_count;
		}

		return this->loadout_count_;
	}

	std::string player::get_name() const
	{
		return std::format("{}_player{:02}", this->get_account_id(), this->get_index() + 1);
	}

	std::uint64_t player::get_id() const
	{
		return this->player_id_;
	}

	namespace impl
	{
		template <database_type_t Type>
		std::optional<player> find(const std::uint64_t player_id)
		{
			return database::access<std::optional<player>>([&](database::database_t& db)
				-> std::optional<player>
			{
				auto results = db.exec<Type>(exp::select.where(player::table.player_id == player_id));

				if (results.empty())
				{
					return {};
				}

				const auto& row = results.front();
				return {player(row)};
			});
		}

		template <database_type_t Type>
		std::optional<player> find_by_index(const std::uint64_t user_id, const std::uint64_t player_index)
		{
			return database::access<std::optional<player>>([&](database::database_t& db)
				-> std::optional<player>
			{
				auto results = db.exec<Type>(exp::select
					.where(player::table.f_user_id == user_id && player::table.player_index == player_index));

				if (results.empty())
				{
					return {};
				}

				const auto& row = results.front();
				return {player(row)};
			});
		}

		template <database_type_t Type>
		player create(const std::uint64_t user_id)
		{
			static const auto default_inventory = []()
			{
				static player_inventory_t inv{};
				inv.initialize();
				return sqlpp::verbatim<sqlpp::binary>(utils::encoding::encode_binary(inv));
			}();

			static const auto loadout_list = []()
			{
				static loadout_list_t list{};
				for (auto i = 0u; i < max_loadout_count; i++)
				{
					list.list[i].initialize(static_cast<std::uint16_t>(i));
				}

				return sqlpp::verbatim<sqlpp::binary>(utils::encoding::encode_binary(list));
			}();

			static const auto building_info_afghan = []()
			{
				static building_info_t building{};
				building.load_default(1);
				return sqlpp::verbatim<sqlpp::binary>(utils::encoding::encode_binary(building));
			}();

			static const auto defense_mission_info = []()
			{
				static defense_mission_info_t info{};
				info.initialize();
				return sqlpp::verbatim<sqlpp::binary>(utils::encoding::encode_binary(info));
			}();

			static const auto communication_gesture_info = []()
			{
				static communication_gesture_info_t info{};
				info.initialize();
				return sqlpp::verbatim<sqlpp::binary>(utils::encoding::encode_binary(info));
			}();

			const auto id = database::access<std::uint64_t>([&](database::database_t& db)
			{
				return db.exec<Type>(
					sqlpp::insert_into(player::table)
						.set(player::table.f_user_id = user_id,
							 player::table.player_inventory = default_inventory,
							 player::table.loadout_list = loadout_list,
							 player::table.building_info_afghan = building_info_afghan,
							 player::table.defense_mission_info = defense_mission_info,
							 player::table.communication_gesture_info = communication_gesture_info,
							 player::table.current_loadout = 0,
							 player::table.loadout_count = initial_loadout_count,
							 player::table.player_creation_date = std::chrono::system_clock::now()));
			});

			const auto found = find<Type>(id);
			if (!found.has_value())
			{
				throw std::runtime_error("[database::players::create] Insertion failed");
			}

			return found.value();
		}

		template <database_type_t Type>
		std::vector<player> get_player_list(const std::uint64_t user_id)
		{
			return database::access<std::vector<player>>([&](database::database_t& db)
				-> std::vector<player>
			{
				auto results = db.exec<Type>(exp::select.where(player::table.f_user_id == user_id));

				std::vector<player> list;

				for (auto& row : results)
				{
					list.emplace_back(row);
				}

				return list;
			});
		}

		template <database_type_t Type>
		void delete_player_data(const std::uint64_t player_id)
		{
			return database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::remove_from(player::table)
						.where(player::table.player_id == player_id));
			});
		}

		template <database_type_t Type>
		void set_nameplate(const std::uint64_t player_id, const std::uint16_t nameplate)
		{
			return database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(player::table)
						.set(player::table.nameplate = nameplate)
							.where(player::table.player_id == player_id));
			});
		}

		DEF_BINARY_GET(player, avatar_t, avatar);
		DEF_BINARY_GET(player, loadout_list_t, loadout_list);
		DEF_BINARY_GET(player, mission_info_t, mission_info);
		DEF_BINARY_GET(player, player_inventory_t, player_inventory);
		DEF_BINARY_GET(player, gimmick_info_t, gimmick_info);
		DEF_BINARY_GET(player, gimmick_save_data_t, gimmick_data_afghan);
		DEF_BINARY_GET(player, gimmick_save_data_t, gimmick_data_africa);
		DEF_BINARY_GET(player, player_play_record_t, player_play_record);
		DEF_BINARY_GET(player, base_resources_t, base_resources);
		DEF_BINARY_GET(player, story_unlock_info_t, story_unlock_info);
		DEF_BINARY_GET(player, building_info_t, building_info_afghan);
		DEF_BINARY_GET(player, crew_levels_t, crew_levels);
		DEF_BINARY_GET(player, defense_mission_info_t, defense_mission_info);
		DEF_BINARY_GET(player, communication_gesture_info_t, communication_gesture_info);

		DEF_BINARY_SET(player, avatar_t, avatar);
		DEF_BINARY_SET(player, loadout_list_t, loadout_list);
		DEF_BINARY_SET(player, mission_info_t, mission_info);
		DEF_BINARY_SET(player, player_inventory_t, player_inventory);
		DEF_BINARY_SET(player, gimmick_info_t, gimmick_info);
		DEF_BINARY_SET(player, gimmick_save_data_t, gimmick_data_afghan);
		DEF_BINARY_SET(player, gimmick_save_data_t, gimmick_data_africa);
		DEF_BINARY_SET(player, player_play_record_t, player_play_record);
		DEF_BINARY_SET(player, base_resources_t, base_resources);
		DEF_BINARY_SET(player, story_unlock_info_t, story_unlock_info);
		DEF_BINARY_SET(player, building_info_t, building_info_afghan);
		DEF_BINARY_SET(player, crew_levels_t, crew_levels);
		DEF_BINARY_SET(player, defense_mission_info_t, defense_mission_info);
		DEF_BINARY_SET(player, communication_gesture_info_t, communication_gesture_info);

		DEF_ARRAY_GET(player, nonstackable_item_list_t, nonstackable_item_list);
		DEF_ARRAY_GET(player, stackable_item_list_t, stackable_item_list);
		DEF_ARRAY_GET(player, inventory_resource_list_t, inventory_resource_list);
		DEF_ARRAY_GET(player, mission_record_list_t, mission_record_list);
		DEF_ARRAY_GET(player, quest_record_list_t, quest_record_list);
		DEF_ARRAY_GET(player, map_unlock_list_t, map_unlock_list_afghan);
		DEF_ARRAY_GET(player, map_unlock_list_t, map_unlock_list_africa);
		DEF_ARRAY_GET(player, crew_member_list_t, crew_member_list);
		DEF_ARRAY_GET(player, defense_mission_record_list_t, defense_mission_record_list);

		DEF_ARRAY_SET(player, nonstackable_item_list_t, nonstackable_item_list);
		DEF_ARRAY_SET(player, stackable_item_list_t, stackable_item_list);
		DEF_ARRAY_SET(player, inventory_resource_list_t, inventory_resource_list);
		DEF_ARRAY_SET(player, mission_record_list_t, mission_record_list);
		DEF_ARRAY_SET(player, quest_record_list_t, quest_record_list);
		DEF_ARRAY_SET(player, map_unlock_list_t, map_unlock_list_afghan);
		DEF_ARRAY_SET(player, map_unlock_list_t, map_unlock_list_africa);
		DEF_ARRAY_SET(player, crew_member_list_t, crew_member_list);
		DEF_ARRAY_SET(player, defense_mission_record_list_t, defense_mission_record_list);
	}

	void player::get_avatar(avatar_t& avatar) const
	{
		RUN_IMPL(impl::get_avatar, this->get_player_id(), avatar);
	}

	void player::get_loadout_list(loadout_list_t& loadout_list) const
	{
		RUN_IMPL(impl::get_loadout_list, this->get_player_id(), loadout_list);
	}

	void player::get_mission_info(mission_info_t& mission_info) const
	{
		RUN_IMPL(impl::get_mission_info, this->get_player_id(), mission_info);
	}

	void player::get_inventory(player_inventory_t& inventory) const
	{
		RUN_IMPL(impl::get_player_inventory, this->get_player_id(), inventory);
	}

	void player::get_nonstackable_item_list(nonstackable_item_list_t& nonstackable_item_list, const std::size_t size_add) const
	{
		RUN_IMPL(impl::get_nonstackable_item_list, this->get_player_id(), nonstackable_item_list, size_add);
	}

	void player::get_gimmick_info(gimmick_info_t& gimmick_info) const
	{
		RUN_IMPL(impl::get_gimmick_info, this->get_player_id(), gimmick_info);
	}

	void player::get_gimmick_save_data(gimmick_save_data_t& gimmick_data, const std::uint32_t map_location) const
	{
		switch (map_location)
		{
		case 0:
		{
			RUN_IMPL(impl::get_gimmick_data_afghan, this->get_player_id(), gimmick_data);
		}
		case 1:
		{
			RUN_IMPL(impl::get_gimmick_data_africa, this->get_player_id(), gimmick_data);
		}
		}
	}

	void player::get_base_resources(base_resources_t& base_resources) const
	{
		RUN_IMPL(impl::get_base_resources, this->get_player_id(), base_resources);
	}

	void player::get_play_record(player_play_record_t& play_record) const
	{
		RUN_IMPL(impl::get_player_play_record, this->get_user_id(), play_record);
	}

	void player::get_story_unlock_info(story_unlock_info_t& story_unlock_info) const
	{
		RUN_IMPL(impl::get_story_unlock_info, this->get_user_id(), story_unlock_info);
	}

	void player::get_building_info(building_info_t& building) const
	{
		RUN_IMPL(impl::get_building_info_afghan, this->get_player_id(), building);
	}

	void player::get_crew_levels(crew_levels_t& crew_levels) const
	{
		RUN_IMPL(impl::get_crew_levels, this->get_user_id(), crew_levels);
	}

	void player::get_defense_mission_info(defense_mission_info_t& defense_mission_info) const
	{
		RUN_IMPL(impl::get_defense_mission_info, this->get_user_id(), defense_mission_info);
	}

	void player::get_communication_gesture_info(communication_gesture_info_t& communication_gesture_info) const
	{
		RUN_IMPL(impl::get_communication_gesture_info, this->get_user_id(), communication_gesture_info);
	}

	void player::get_mission_record_list(mission_record_list_t& mission_record_list, const std::size_t size_add) const
	{
		RUN_IMPL(impl::get_mission_record_list, this->get_user_id(), mission_record_list, size_add);
	}

	void player::get_quest_record_list(quest_record_list_t& quest_record_list, const std::size_t size_add) const
	{
		RUN_IMPL(impl::get_quest_record_list, this->get_user_id(), quest_record_list, size_add);
	}

	void player::get_inventory_resource_list(inventory_resource_list_t& inventory_resource_list, const std::size_t size_add) const
	{
		RUN_IMPL(impl::get_inventory_resource_list, this->get_user_id(), inventory_resource_list, size_add);
	}

	void player::get_stackable_item_list(stackable_item_list_t& stackable_item_list, const std::size_t size_add) const
	{
		RUN_IMPL(impl::get_stackable_item_list, this->get_user_id(), stackable_item_list, size_add);
	}

	void player::get_map_unlock_list_afghan(map_unlock_list_t& map_unlock_list, const std::size_t size_add) const
	{
		RUN_IMPL(impl::get_map_unlock_list_afghan, this->get_user_id(), map_unlock_list, size_add);
	}

	void player::get_map_unlock_list_africa(map_unlock_list_t& map_unlock_list, const std::size_t size_add) const
	{
		RUN_IMPL(impl::get_map_unlock_list_africa, this->get_user_id(), map_unlock_list, size_add);
	}

	void player::get_crew_member_list(crew_member_list_t& crew_member_list, const std::size_t size_add) const
	{
		RUN_IMPL(impl::get_crew_member_list, this->get_user_id(), crew_member_list, size_add);
	}

	void player::get_defense_mission_record_list(defense_mission_record_list_t& defense_mission_record_list, const std::size_t size_add) const
	{
		RUN_IMPL(impl::get_defense_mission_record_list, this->get_user_id(), defense_mission_record_list, size_add);
	}

	bool player::set_avatar(avatar_t& avatar) const
	{
		RUN_IMPL(impl::set_avatar, this->get_player_id(), avatar);
	}

	bool player::set_loadout_list(loadout_list_t& loadout_list) const
	{
		RUN_IMPL(impl::set_loadout_list, this->get_player_id(), loadout_list);
	}

	bool player::set_mission_info(mission_info_t& mission_info) const
	{
		RUN_IMPL(impl::set_mission_info, this->get_player_id(), mission_info);
	}

	bool player::set_inventory(player_inventory_t& inventory) const
	{
		RUN_IMPL(impl::set_player_inventory, this->get_player_id(), inventory);
	}

	bool player::set_nonstackable_item_list(nonstackable_item_list_t& nonstackable_item_list) const
	{
		RUN_IMPL(impl::set_nonstackable_item_list, this->get_player_id(), nonstackable_item_list);
	}

	bool player::set_gimmick_info(gimmick_info_t& gimmick_info) const
	{
		RUN_IMPL(impl::set_gimmick_info, this->get_player_id(), gimmick_info);
	}

	bool player::set_gimmick_save_data(gimmick_save_data_t& gimmick_data, const std::uint32_t map_location) const
	{
		switch (map_location)
		{
		case 0:
		{
			RUN_IMPL(impl::set_gimmick_data_afghan, this->get_player_id(), gimmick_data);
		}
		case 1:
		{
			RUN_IMPL(impl::set_gimmick_data_africa, this->get_player_id(), gimmick_data);
		}
		}

		return false;
	}

	bool player::set_play_record(player_play_record_t& play_record) const
	{
		RUN_IMPL(impl::set_player_play_record, this->get_user_id(), play_record);
	}

	bool player::set_base_resources(base_resources_t& base_resources) const
	{
		RUN_IMPL(impl::set_base_resources, this->get_user_id(), base_resources);
	}

	bool player::set_story_unlock_info(story_unlock_info_t& story_unlock_info) const
	{
		RUN_IMPL(impl::set_story_unlock_info, this->get_user_id(), story_unlock_info);
	}

	bool player::set_building_info(building_info_t& building) const
	{
		RUN_IMPL(impl::set_building_info_afghan, this->get_player_id(), building);
	}

	bool player::set_crew_levels(crew_levels_t& crew_levels) const
	{
		RUN_IMPL(impl::set_crew_levels, this->get_user_id(), crew_levels);
	}

	bool player::set_defense_mission_info(defense_mission_info_t& defense_mission_info) const
	{
		RUN_IMPL(impl::set_defense_mission_info, this->get_user_id(), defense_mission_info);
	}

	bool player::set_communication_gesture_info(communication_gesture_info_t& communication_gesture_info) const
	{
		RUN_IMPL(impl::set_communication_gesture_info, this->get_user_id(), communication_gesture_info);
	}

	bool player::set_mission_record_list(mission_record_list_t& set_mission_record_list) const
	{
		RUN_IMPL(impl::set_mission_record_list, this->get_user_id(), set_mission_record_list);
	}

	bool player::set_quest_record_list(quest_record_list_t& quest_record_list) const
	{
		RUN_IMPL(impl::set_quest_record_list, this->get_user_id(), quest_record_list);
	}

	bool player::set_inventory_resource_list(inventory_resource_list_t& inventory_resource_list) const
	{
		RUN_IMPL(impl::set_inventory_resource_list, this->get_user_id(), inventory_resource_list);
	}

	bool player::set_stackable_item_list(stackable_item_list_t& stackable_item_list) const
	{
		RUN_IMPL(impl::set_stackable_item_list, this->get_user_id(), stackable_item_list);
	}

	bool player::set_map_unlock_list_afghan(map_unlock_list_t& map_unlock_list) const
	{
		RUN_IMPL(impl::set_map_unlock_list_afghan, this->get_user_id(), map_unlock_list);
	}

	bool player::set_map_unlock_list_africa(map_unlock_list_t& map_unlock_list) const
	{
		RUN_IMPL(impl::set_map_unlock_list_africa, this->get_user_id(), map_unlock_list);
	}

	bool player::set_crew_member_list(crew_member_list_t& crew_member_list) const
	{
		RUN_IMPL(impl::set_crew_member_list, this->get_user_id(), crew_member_list);
	}

	bool player::set_defense_mission_record_list(defense_mission_record_list_t& defense_mission_record_list) const
	{
		RUN_IMPL(impl::set_defense_mission_record_list, this->get_user_id(), defense_mission_record_list);
	}

	void player::set_nameplate(const std::uint16_t nameplate) const
	{
		RUN_IMPL(impl::set_nameplate, this->get_user_id(), nameplate);
	}

	std::optional<player> find(const std::uint64_t id)
	{
		RUN_IMPL(impl::find, id);
	}

	std::optional<player> find_by_index(const std::uint64_t user_id, const std::uint64_t player_index)
	{
		RUN_IMPL(impl::find_by_index, user_id, player_index);
	}

	std::vector<player> get_player_list(const std::uint64_t user_id)
	{
		RUN_IMPL(impl::get_player_list, user_id);
	}

	std::optional<player> create(const std::uint64_t user_id)
	{
		RUN_IMPL(impl::create, user_id);
	}

	void delete_player_data(const std::uint64_t player_id)
	{
		RUN_IMPL(impl::delete_player_data, player_id);
	}

	class table final : public table_interface
	{
	public:
		void create(database_t& database) override
		{
			database.run_query("mgssd.players.create");
			database.run_query("mgssd.players.remove_insert_trigger");
			database.run_query("mgssd.players.add_insert_trigger");

			try
			{
				database.run_query("mgssd.users.add_player_foreign_key");
			}
			catch (...)
			{
			}

			database.run_query("mgssd.users.remove_update_trigger");
			database.run_query("mgssd.users.add_update_trigger");

			command::add("give_resource", [](const command::params& params)
			{
				const auto player_id = params.get_uint64(1);
				const auto resource_id = params.get_uint64(2);
				const auto amount = params.get_int(3);

				const auto player = find(player_id);
				if (!player.has_value())
				{
					console::log("player not found\n");
					return;
				}

				const auto iter = game::parameters_table.ssd_sbm_parameters->resources.find(static_cast<std::uint32_t>(resource_id));
				if (iter == game::parameters_table.ssd_sbm_parameters->resources.end())
				{
					console::log("resource not found\n");
					return;
				}

				auto resource_list = std::make_unique<inventory_resource_list_t>();
				player->get_inventory_resource_list(*resource_list, 1);
				inventory_resource_t resource{};
				resource.resource_index = iter->second->index;
				resource.count = static_cast<std::uint32_t>(amount);
				resource.flag = 1;
				resource_list->add_resource(resource);
				player->set_inventory_resource_list(*resource_list);
			});
		}
	};
}

REGISTER_TABLE(database::players::table, 1)
