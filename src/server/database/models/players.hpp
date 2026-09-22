#pragma once

#include "../database.hpp"
#include "game/game.hpp"
#include "game/parameters.hpp"
#include "utils/encoding.hpp"
#include "../utils.hpp"

namespace database::players
{
	constexpr const auto player_capacity = 1u;
	constexpr const auto max_loadout_count = 8u;
	constexpr const auto initial_loadout_count = 4u;

#pragma pack(push, 1)
	struct avatar_t
	{
		std::uint8_t accessory;
		std::uint8_t beard_length;
		std::uint8_t beard_style;
		std::uint8_t eyebrow_length;
		std::uint8_t eyebrow_style;
		std::uint8_t hair_color;
		std::uint8_t hair_style;
		std::uint8_t left_eye_brightness;
		std::uint8_t left_eye_color;
		std::uint8_t player_parts_type;
		std::uint8_t player_type;
		std::uint8_t race;
		std::uint8_t race_color;
		std::uint8_t race_type;
		std::uint8_t race_variation;
		std::uint8_t right_eye_brightness;
		std::uint8_t right_eye_color;
		std::uint8_t tattoo;
		std::uint8_t tattoo_color;
		std::uint8_t voice;
		std::uint8_t voice_pitch;
		std::uint8_t motion_frame_list[60];
		char name[64];

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct nonstackable_item_t
	{
		struct option_t
		{
			std::uint32_t option_id;
			std::uint8_t obtained;
		};

		struct perk_t
		{
			std::uint32_t perk_id;
			std::uint8_t perk_level;
		};

		std::uint8_t color;
		std::uint8_t color2;
		std::uint8_t flag;
		std::uint16_t grade;
		std::uint16_t inventory_index;
		std::uint16_t life;
		std::uint16_t life_max;
		std::uint16_t option_slot;
		std::uint16_t spec;
		std::uint32_t obtain_order;
		std::uint32_t production_id;
		option_t option_list[8];
		perk_t perk_list[5];

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct stackable_item_t
	{
		struct parseable
		{
			std::uint8_t flag;
			std::uint8_t inventory_type;
			std::uint8_t cbox_index;
			std::uint8_t inventory_index;
			std::uint8_t obtain_order;
			std::uint8_t damaged_in_count;
			std::uint32_t count;
		};

		std::uint32_t flag : 1;
		std::uint32_t inventory_type : 1;
		std::uint32_t cbox_index : 7;
		std::uint32_t inventory_index : 7;
		std::uint32_t obtain_order : 7;
		std::uint32_t damaged_in_count : 8;
		std::uint32_t production_index : 10;
		std::uint32_t count : 17;
		std::uint32_t unused : 5;

		inline std::uint32_t get_production_id() const
		{
			if (this->production_index > game::parameters_table.ssd_sbm_parameters->productions_list.size())
			{
				return 0u;
			}

			const auto& prod = game::parameters_table.ssd_sbm_parameters->productions_list[this->production_index];
			if (prod == nullptr)
			{
				return 0u;
			}

			return prod->id;
		}

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct loadout_t
	{
		struct item_t
		{
			std::uint32_t count;
			std::uint32_t idx;
		};

		struct weapon_t
		{
			std::uint16_t ammo_count;
			std::uint16_t ammo_idx;
			std::uint16_t init_ammo_count;
			std::uint16_t init_ammo_idx;
			std::uint16_t inventory_index;
		};

		struct gear_info_t
		{
			std::uint16_t arm_inventory_index;
			std::uint16_t body_inventory_index;
			std::uint16_t ext_head_production_idx;
			std::uint16_t ext_suit_production_idx;
			std::uint16_t head_inventory_index;
			std::uint16_t leg_inventory_index;
		};

		struct skill_t
		{
			std::uint16_t slot[8];
		};

		bool valid;
		std::uint16_t class_info;
		gear_info_t gear_info;
		weapon_t main_weapon_list[3];
		weapon_t sub_weapon_list[3];
		skill_t skill_list[5];
		item_t gadget_list[16];
		std::uint16_t survival_list[8];
		item_t porch_list[20];
		char name[64];

		void initialize(const std::uint32_t index = 0u);
		bool parse(json::value& data, std::uint32_t& index);
		void to_json(json::value& data, const std::uint32_t index = 0u) const;
	};

	struct mission_info_t
	{
		struct status_buffer_t
		{
			std::uint32_t buffer_type;
			std::uint32_t remaining_time;
		};

		std::uint16_t equipment_slot; // 206,
		std::uint32_t flag_mission_code; // 40070,
		std::uint16_t flag_mission_sequence_number; // 1,
		std::uint16_t hunger; // 957,
		std::uint16_t hunger_max_keep_time; // 0,
		std::uint16_t injury_whole; // 0,
		std::uint16_t life; // 1358,
		std::uint16_t location_code; // 15,
		std::uint16_t mission_code; // 30010,
		std::uint16_t oxygen; // 1000,
		std::uint16_t sequence_number; // 9,
		std::uint16_t story_sequence_number; // 9,
		std::uint16_t stamina; // 2148,
		std::uint16_t temp_body_id; // 0,
		std::uint16_t temp_crew_type_code; // 0,
		std::uint16_t temp_face_id; // 0,
		std::uint16_t temp_first_name_id; // 0,
		std::uint16_t temp_last_name_id; // 0,
		std::uint16_t temp_race_id; // 0,
		std::uint16_t temp_sex_id; // 0,
		std::uint16_t temp_unique_type_code; // 0,
		std::uint16_t temp_voice_type; // 0,
		std::uint16_t thirst; // 1808,
		std::uint16_t thirst_max_keep_time; // 0,
		std::uint16_t tiredness; // 547,
		std::uint16_t weather; // 0
		std::int32_t pos_x; // -4508,
		std::int32_t pos_y; // 2880,
		std::int32_t pos_z; // 22444,
		std::int32_t rot_y; // -89,
		std::uint64_t clock; // 60902697500,
		std::uint64_t survival_sec; // 33595,
		std::uint16_t injury_part[8]; // 0
		std::uint16_t injury_recovery_time[32]; // 0
		status_buffer_t status_buffer[32];
		std::uint8_t vars[640]; // 

		void initialize();
		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct player_inventory_t
	{
		std::uint32_t energy;
		std::uint32_t class_opened;
		std::uint32_t oxygen_convert_count;
		std::uint32_t energy_invested[5];
		std::uint16_t cbox_history[3];
		std::uint16_t cbox_location[3];
		std::uint16_t cbox_updated[3];
		std::int32_t cbox_pos[12];
		std::uint8_t event_obtained[2];
		std::uint8_t skill_status[255];
		std::uint8_t survival_new[32];
		std::uint8_t survival_obtained[32];
		std::uint8_t survival_slot_new[2];

		void initialize();
		bool parse(json::value& data, std::uint16_t& nameplate);
		bool parse_save(json::value& data, std::uint16_t& nameplate);
		void to_json(json::value& data, const std::uint16_t nameplate = 0u) const;
		void set_survival_obtained(const std::uint32_t index, bool obtained);

	};

	struct gimmick_save_data_t
	{
		struct instant_t
		{
			std::uint8_t data[2048];
		};

		struct permanent_t
		{
			std::uint8_t data[512];
		};

		struct resource_event_t
		{
			std::uint8_t data[896];
		};

		struct resource_normal_t
		{
			std::uint8_t data[1344];
		};

		struct resource_rare_t
		{
			std::uint8_t data[224];
		};

		struct resource_shared_t
		{
			std::uint8_t data[224];
		};

		instant_t instant[4];
		permanent_t permanent[4];
		resource_event_t resource_event[4];
		resource_normal_t resource_normal[4];
		resource_rare_t resource_rare[4];
		resource_shared_t resource_shared[4];

		bool parse(json::value& parse);
		void to_json(json::value& data, const std::uint32_t map_location) const;
	};

	struct loadout_list_t
	{
		loadout_t list[max_loadout_count];
	};

	struct gimmick_resource_info_t
	{
		std::uint32_t resource_event_tail;
		std::uint32_t resource_normal_tail;
		std::uint32_t resource_rare_tail;
		std::uint32_t resource_shared_tail;

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct gimmick_timer_info_t
	{
		std::uint16_t resource_timer_global_afghan;
		std::uint16_t resource_timer_global_africa;
		std::uint16_t resource_timer_stock_afghan;
		std::uint16_t resource_timer_stock_africa;

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct gimmick_info_t
	{
		gimmick_resource_info_t resource_afghan;
		gimmick_resource_info_t resource_africa;
		gimmick_timer_info_t timer;
	};

	struct base_resources_t
	{
		struct base_resource_params_t
		{
			std::uint16_t bad_status_1_risk;
			std::uint16_t bad_status_2_risk;
			std::uint16_t bad_status_3_risk;
			std::uint16_t bad_status_4_risk;
			std::uint16_t clean_water;
			std::uint16_t dirty_water;
			std::uint16_t food;
			std::uint16_t medical_supplies;
			std::uint16_t party_item_id;
			std::uint16_t party_item_updates;
			std::uint16_t total_number_of_updates;
		};

		std::uint32_t animals[57];
		std::uint32_t resource_counts[32];
		std::uint64_t next_update_time;
		std::uint64_t update_remaining_time;
		base_resource_params_t params;

		bool parse_base(json::value& base);
		bool parse_counts(json::value& count);
		bool parse_animals(json::value& animals);
		void to_json(json::value& data) const;
	};

	struct inventory_resource_t
	{
		struct parseable
		{
			std::uint8_t flag;
			std::uint8_t inventory_type;
			std::uint8_t cbox_index;
			std::uint8_t inventory_index;
			std::uint8_t obtain_order;
			std::uint8_t damaged_in_count;
			std::uint32_t count;
		};

		std::uint32_t flag : 1;
		std::uint32_t inventory_type : 1;
		std::uint32_t cbox_index : 7;
		std::uint32_t inventory_index : 7;
		std::uint32_t obtain_order : 7;
		std::uint32_t damaged_in_count : 8;
		std::uint32_t resource_index : 10;
		std::uint32_t count : 17;
		std::uint32_t unused : 5;
		
		inline std::uint32_t get_resource_id() const
		{
			if (this->resource_index > game::parameters_table.ssd_sbm_parameters->resources_list.size())
			{
				return 0u;
			}

			const auto& res = game::parameters_table.ssd_sbm_parameters->resources_list[this->resource_index];
			if (res == nullptr)
			{
				return 0u;
			}

			return res->id;
		}

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct player_play_record_t
	{
		std::uint32_t first[191];
		std::uint32_t additional[46];
	};

	struct mission_record_t
	{
		std::uint8_t clear_flag;
		//std::uint8_t clear_rank; // always zero?
		//std::uint8_t new_flag;
		std::uint16_t mission_code;
		//std::uint32_t clear_time;
		//std::uint32_t score;

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct quest_record_t
	{
		std::uint8_t flagset;
		std::uint32_t mission_code;
		std::uint16_t repop_count;

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct map_unlock_t
	{
		std::uint16_t value;
		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct story_unlock_info_t
	{
		struct tips_open_info_t
		{
			std::uint8_t value[128];
			bool parse(json::value& data);
			void to_json(json::value& data) const;
		};

		std::uint32_t demo_open_flag;
		std::uint32_t facility_new_flag;
		std::uint32_t marker_map_location;
		std::uint32_t oxygen_supply_unlock;
		std::uint32_t story_sequence_number;
		std::uint8_t fast_travel_unlock[8];
		std::uint8_t marker_afghan[522];
		std::uint8_t marker_africa[522];
		tips_open_info_t tips_open_info;

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	constexpr const auto building_grid_size = 32u;
	
	enum edge_type_t
	{
		edge_type_center = 0,
		edge_type_upper = 1,
		edge_type_left = 2,
		edge_type_count = 3,
	};

	struct building_info_t
	{
		struct cell_edge_t
		{
			std::uint8_t rotation;
			std::uint16_t extra_data;
			std::uint16_t life;
			std::uint16_t max_life;
			std::uint16_t production_index;
			std::uint32_t completion_remaining_time;
			std::uint32_t recovery_time;

			bool parse(json::value& data, std::uint32_t& row, std::uint32_t& column, const std::uint32_t type);
			void to_json(json::value& data, const std::uint32_t row, const std::uint32_t column, const std::uint32_t type) const;
		};

		struct farming_info_t
		{
			std::uint8_t animal[8];
			std::uint8_t item[10];

			bool parse(json::value& data, std::uint32_t& row, std::uint32_t& column);
			void to_json(json::value& data, const std::uint32_t row, const std::uint32_t column) const;
		};

		struct cell_t
		{
			farming_info_t farming_info;
			cell_edge_t edges[edge_type_count];
		};

		cell_t cells[building_grid_size][building_grid_size];

		bool parse_type(json::value& data, const std::uint32_t type);
		bool parse_farming(json::value& data);
		bool parse(json::value& data, const bool is_diff);

		void to_json(json::value& data) const;
		void to_json_farming(json::value& data) const;

		void load_default(const std::uint32_t map_location);
	};

	struct crew_member_t
	{
		std::uint16_t ability_accessory;
		std::uint16_t ability_animal;
		std::uint16_t ability_base_defense;
		std::uint16_t ability_defense_unit;
		std::uint16_t ability_develop;
		std::uint16_t ability_expedition;
		std::uint16_t ability_food;
		std::uint16_t ability_gadget;
		std::uint16_t ability_medical;
		std::uint16_t ability_plant;
		std::uint16_t condition;
		std::uint16_t current_group;
		std::uint16_t first_name_index;
		std::uint16_t health_flag;
		std::uint16_t initial_max_life;
		std::uint16_t item1_count;
		std::uint16_t item2_count;
		std::uint16_t item3_count;
		std::uint16_t item4_count;
		std::uint16_t item5_count;
		std::uint16_t item6_count;
		std::uint16_t last_name_index;
		std::uint16_t life;
		std::uint16_t map_location;
		std::uint16_t max_life;
		std::uint16_t previous_group;
		std::uint16_t previous_job;
		std::uint16_t race_id;
		std::uint16_t resistance_food_shortage;
		std::uint16_t resistance_sleepless;
		std::uint16_t resistance_water_shortage;
		std::uint16_t sanity;
		std::uint16_t skill;
		std::uint16_t survival_days;
		std::uint16_t voice_type;
		std::uint32_t injury_id_1;
		std::uint32_t injury_id_2;
		std::uint32_t injury_time_1;
		std::uint32_t injury_time_2;
		std::uint32_t sickness_id_1;
		std::uint32_t sickness_id_2;
		std::uint32_t sickness_time_1;
		std::uint32_t sickness_time_2;
		std::uint32_t body_id;
		std::uint32_t face_id;
		std::uint32_t generation_date;
		std::uint32_t unique_id;
		std::uint32_t sex_id;
		std::uint32_t unique_index;
		std::uint8_t motivation_history_event[10];
		std::uint8_t motivation_history_value[10];
		char nickname[32];
		
		bool parse(json::value& data);
		bool parse_update(json::value& data);
		bool parse_add_param(json::value& data);
		void apply_update(const crew_member_t& diff);
		void to_json(json::value& data) const;
	};

	struct crew_levels_t
	{
		std::uint8_t base_defense;
		std::uint8_t combat_deploy;
		std::uint8_t develop;
		std::uint8_t food;
		std::uint8_t medic;
		std::uint8_t plant;

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct defense_mission_record_t
	{
		std::uint8_t cleared;
		std::uint8_t clear_rank;
		std::uint8_t waves;
		std::uint16_t mission_code;
		std::uint32_t iris_score;

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct defense_mission_info_t
	{
		struct status_t
		{
			std::uint32_t mining_machine_life;

			bool parse(json::value& data);
			void to_json(json::value& data) const;
		};

		struct parameter_t
		{
			std::uint32_t flag;
			std::uint32_t threat;
			std::uint32_t threat_threshold;
			std::uint32_t attack_time;

			bool parse(json::value& data);
			void to_json(json::value& data) const;
		};

		status_t status;
		parameter_t parameter;
		void initialize();
	};
#pragma pack(pop)

	constexpr const auto max_item_count = 99999u;

	enum inventory_type_t : std::uint8_t
	{
		inventory_storage = 0,
		inventory_player = 1,
		inventory_type_count = 2,
	};

	constexpr const auto max_nonstackable_items = 1024;
	class nonstackable_item_list_t final : public generic_item_list<nonstackable_item_t, max_nonstackable_items>
	{
	public:
		inline bool are_elements_equal(const nonstackable_item_t& l, const nonstackable_item_t& r) const override
		{
			return l.inventory_index == r.inventory_index;
		}

		inline bool is_element_empty(const nonstackable_item_t& value) const override
		{
			return value.production_id == 0;
		}

		bool parse_life_diff(json::value& data);
		bool find_free_index(std::uint16_t& index, std::uint32_t& obtain_order);
		nonstackable_item_t* find_at_index(const std::uint16_t inventory_index);

	};

	constexpr const auto max_stackable_items = 1024;
	class stackable_item_list_t final : public generic_item_list<stackable_item_t, max_stackable_items>
	{
	public:
		inline bool are_elements_equal(const stackable_item_t& l, const stackable_item_t& r) const override
		{
			return l.inventory_index == r.inventory_index && l.inventory_type == r.inventory_type;
		}

		inline bool is_element_empty(const stackable_item_t& value) const override
		{
			return value.production_index == 0 || value.count == 0;
		}

		stackable_item_t* find_item(const std::uint32_t production_id);
		bool find_free_index(const std::uint8_t inventory_type, std::uint16_t& index, std::uint16_t& obtain_order) const;

		inline void import_element(stackable_item_t& dest, const stackable_item_t& src) const override
		{
			std::memcpy(&dest, &src, sizeof(stackable_item_t));
		}

		bool add_item(stackable_item_t& item, const std::uint8_t inventory_type = inventory_storage);

	};

	constexpr const auto max_resources = 512;
	class inventory_resource_list_t final : public generic_item_list<inventory_resource_t, max_resources>
	{
	public:
		inline bool are_elements_equal(const inventory_resource_t& l, const inventory_resource_t& r) const override
		{
			return l.inventory_index == r.inventory_index && l.inventory_type == r.inventory_type;
		}

		inline bool is_element_empty(const inventory_resource_t& value) const override
		{
			return value.resource_index == 0 || value.count == 0;
		}

		bool find_free_index(const std::uint8_t inventory_type, std::uint16_t& index, std::uint16_t& obtain_order) const;

		inline void import_element(inventory_resource_t& dest, const inventory_resource_t& src) const override
		{
			std::memcpy(&dest, &src, sizeof(inventory_resource_t));
		}

		bool add_resource(inventory_resource_t& resource, const std::uint8_t inventory_type = inventory_storage);

	};

	class mission_record_list_t final : public generic_item_list<mission_record_t, 256>
	{
	public:
		inline bool are_elements_equal(const mission_record_t& l, const mission_record_t& r) const override
		{
			return l.mission_code == r.mission_code;
		}

		inline bool is_element_empty(const mission_record_t& value) const override
		{
			return value.mission_code == 0;
		}

		bool open_mission(const std::uint16_t mission_code);

	};

	class quest_record_list_t final : public generic_item_list<quest_record_t, 256>
	{
	public:
		inline bool are_elements_equal(const quest_record_t& l, const quest_record_t& r) const override
		{
			return l.mission_code == r.mission_code;
		}

		inline bool is_element_empty(const quest_record_t& value) const override
		{
			return value.mission_code == 0;
		}

		bool open_mission(const std::uint16_t mission_code);

	};

	class defense_mission_record_list_t final : public generic_item_list<defense_mission_record_t, 50>
	{
	public:
		inline bool are_elements_equal(const defense_mission_record_t& l, const defense_mission_record_t& r) const override
		{
			return l.mission_code == r.mission_code;
		}

		inline bool is_element_empty(const defense_mission_record_t& value) const override
		{
			return value.mission_code == 0;
		}

	};

	class map_unlock_list_t final : public generic_item_list<map_unlock_t, 21904>
	{
	public:
		inline bool are_elements_equal(const map_unlock_t& l, const map_unlock_t& r) const override
		{
			return l.value == r.value;
		}

		inline bool is_element_empty(const map_unlock_t& value) const override
		{
			return value.value == 0u;
		}

		virtual void to_json(json::value& data) const override
		{
			auto idx = 1;
			data[0] = 0;

			const auto list = this->data();
			for (auto i = 0ull; i < this->size(); i++)
			{
				if (this->is_element_empty(list[i]))
				{
					continue;
				}

				list[i].to_json(data[idx++]);
			}
		}
	};

	class crew_member_list_t : public generic_item_list<crew_member_t, 30>
	{
	public:
		inline bool are_elements_equal(const crew_member_t& l, const crew_member_t& r) const override
		{
			return l.unique_id == r.unique_id;
		}

		inline bool is_element_empty(const crew_member_t& value) const override
		{
			return value.unique_id == 0u;
		}

		bool parse_update(json::value& data);

		crew_member_t* find_member(const std::uint32_t id);

	};

	template <size_t N>
	bool craft_recipe(const std::uint32_t price, const game::cost_t(&cost)[N], const std::uint32_t amount,
		inventory_resource_list_t& resource_list, stackable_item_list_t& stackable_item_list, player_inventory_t& inventory_info)
	{
		if (price * amount > inventory_info.energy)
		{
			return false;
		}

		inventory_info.energy -= price * amount;

		for (auto i = 0ull; i < N; i++)
		{
			if (cost[i].id == 0)
			{
				continue;
			}

			auto found = false;
			if (game::is_event_obtained_res(cost[i].id, inventory_info.event_obtained, &found) && found)
			{
				continue;
			}

			for (auto o = 0ull; o < resource_list.size(); o++)
			{
				if (cost[i].id == resource_list[o].get_resource_id() && cost[i].count * amount <= resource_list[o].count)
				{
					found = true;
					resource_list[o].count -= cost[i].count * amount;
					break;
				}
			}

			if (!found)
			{
				for (auto o = 0ull; o < stackable_item_list.size(); o++)
				{
					if (cost[i].id == stackable_item_list[o].get_production_id() && cost[i].count * amount <= stackable_item_list[o].count)
					{
						found = true;
						stackable_item_list[o].count -= static_cast<std::uint16_t>(cost[i].count * amount);
						break;
					}
				}
			}

			if (!found)
			{
				return false;
			}
		}

		return true;
	}

	class player
	{
	public:
		DEFINE_FIELD(player_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(f_user_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(player_index, sqlpp::integer_unsigned);
		DEFINE_FIELD(player_creation_date, sqlpp::time_point);
		DEFINE_FIELD(point, sqlpp::integer_unsigned);
		DEFINE_FIELD(nameplate, sqlpp::integer_unsigned);
		DEFINE_FIELD(playtime, sqlpp::integer_unsigned);
		DEFINE_FIELD(current_loadout, sqlpp::integer_unsigned);
		DEFINE_FIELD(loadout_count, sqlpp::integer_unsigned);
		DEFINE_FIELD(avatar, sqlpp::binary);
		DEFINE_FIELD(mission_info, sqlpp::binary);
		DEFINE_FIELD(loadout_list, sqlpp::binary);
		DEFINE_FIELD(player_inventory, sqlpp::binary);
		DEFINE_FIELD(nonstackable_item_list, sqlpp::binary);
		DEFINE_FIELD(gimmick_info, sqlpp::binary);
		DEFINE_FIELD(gimmick_data_afghan, sqlpp::binary);
		DEFINE_FIELD(gimmick_data_africa, sqlpp::binary);
		DEFINE_FIELD(player_play_record, sqlpp::binary);
		DEFINE_FIELD(base_resources, sqlpp::binary);
		DEFINE_FIELD(story_unlock_info, sqlpp::binary);
		DEFINE_FIELD(mission_record_list, sqlpp::binary);
		DEFINE_FIELD(quest_record_list, sqlpp::binary);
		DEFINE_FIELD(inventory_resource_list, sqlpp::binary);
		DEFINE_FIELD(stackable_item_list, sqlpp::binary);
		DEFINE_FIELD(map_unlock_list_afghan, sqlpp::binary);
		DEFINE_FIELD(map_unlock_list_africa, sqlpp::binary);
		DEFINE_FIELD(building_info_afghan, sqlpp::binary);
		DEFINE_FIELD(building_info_africa, sqlpp::binary);
		DEFINE_FIELD(crew_member_list, sqlpp::binary);
		DEFINE_FIELD(crew_levels, sqlpp::binary);
		DEFINE_FIELD(defense_mission_info, sqlpp::binary);
		DEFINE_FIELD(defense_mission_record_list, sqlpp::binary);
		DEFINE_TABLE(players, player_id_field_t, f_user_id_field_t, player_index_field_t,
			player_creation_date_field_t,
			point_field_t, nameplate_field_t, playtime_field_t,
			current_loadout_field_t,
			loadout_count_field_t,
			avatar_field_t, 
			mission_info_field_t, 
			loadout_list_field_t,
			player_inventory_field_t,
			nonstackable_item_list_field_t,
			gimmick_info_field_t,
			gimmick_data_afghan_field_t,
			gimmick_data_africa_field_t,
			player_play_record_field_t,
			base_resources_field_t,
			story_unlock_info_field_t,
			mission_record_list_field_t,
			quest_record_list_field_t,
			inventory_resource_list_field_t,
			stackable_item_list_field_t,
			map_unlock_list_afghan_field_t,
			map_unlock_list_africa_field_t,
			building_info_afghan_field_t,
			building_info_africa_field_t,
			crew_member_list_field_t,
			crew_levels_field_t,
			defense_mission_info_field_t,
			defense_mission_record_list_field_t
		);

		inline static table_t table;

		template <typename ...Args>
		player(const sqlpp::result_row_t<Args...>& row)
		{
			this->player_id_ = row.player_id;
			this->account_id_ = row.account_id;
			this->user_id_ = row.f_user_id;
			this->index_ = row.player_index;
			this->playtime_ = static_cast<std::uint32_t>(row.playtime);
			this->nameplate_ = static_cast<std::uint32_t>(row.nameplate);
			this->point_ = static_cast<std::uint32_t>(row.point);
			this->current_loadout_ = static_cast<std::uint32_t>(row.current_loadout);
			this->loadout_count_ = static_cast<std::uint32_t>(row.loadout_count);
			this->creation_date_ = row.player_creation_date.value().time_since_epoch();
		}

		GET_FIELD_H(std::uint64_t, player_id);
		GET_FIELD_H(std::uint64_t, user_id);
		GET_FIELD_H(std::uint64_t, account_id);
		GET_FIELD_H(std::uint64_t, index);
		GET_FIELD_H(std::uint32_t, playtime);
		GET_FIELD_H(std::uint32_t, nameplate);
		GET_FIELD_H(std::uint32_t, point);
		GET_FIELD_H(std::uint32_t, current_loadout);
		GET_FIELD_H(std::uint32_t, loadout_count);
		GET_FIELD_H(std::chrono::microseconds, creation_date);

		std::string get_name() const;
		std::uint64_t get_id() const;

		void get_avatar(avatar_t& avatar) const;
		void get_loadout_list(loadout_list_t& loadout) const;
		void get_mission_info(mission_info_t& mission_info) const;
		void get_inventory(player_inventory_t& inventory) const;
		void get_gimmick_info(gimmick_info_t& gimmick_info) const;
		void get_gimmick_save_data(gimmick_save_data_t& gimmick_data, const std::uint32_t map_location) const;
		void get_play_record(player_play_record_t& play_record) const;
		void get_base_resources(base_resources_t& base_resources) const;
		void get_story_unlock_info(story_unlock_info_t& story_unlock_info) const;
		void get_building_info(building_info_t& building, const std::uint32_t map_location) const;
		void get_crew_levels(crew_levels_t& crew_levels) const;
		void get_defense_mission_info(defense_mission_info_t& defense_mission) const;

		bool set_avatar(avatar_t& avatar) const;
		bool set_loadout_list(loadout_list_t& loadout) const;
		bool set_mission_info(mission_info_t& mission_info) const;
		bool set_inventory(player_inventory_t& inventory) const;
		bool set_gimmick_info(gimmick_info_t& gimmick_info) const;
		bool set_gimmick_save_data(gimmick_save_data_t& gimmick_data, const std::uint32_t map) const;
		bool set_play_record(player_play_record_t& play_record) const;
		bool set_base_resources(base_resources_t& base_resources) const;
		bool set_story_unlock_info(story_unlock_info_t& story_unlock_info) const;
		bool set_building_info(building_info_t& building, const std::uint32_t map_location) const;
		bool set_crew_levels(crew_levels_t& crew_levels) const;
		bool set_defense_mission_info(defense_mission_info_t& defense_mission) const;

		void get_mission_record_list(mission_record_list_t& mission_record_list, const std::size_t size_add = 0ull) const;
		void get_quest_record_list(quest_record_list_t& quest_record_list, const std::size_t size_add = 0ull) const;
		void get_nonstackable_item_list(nonstackable_item_list_t& nonstackable_list, const std::size_t size_add = 0ull) const;
		void get_inventory_resource_list(inventory_resource_list_t& inventory_resource_list, const std::size_t size_add = 0ull) const;
		void get_stackable_item_list(stackable_item_list_t& stackable_item_list, const std::size_t size_add = 0ull) const;
		void get_map_unlock_list_afghan(map_unlock_list_t& map_unlock_list, const std::size_t size_add = 0ull) const;
		void get_map_unlock_list_africa(map_unlock_list_t& map_unlock_list, const std::size_t size_add = 0ull) const;
		void get_crew_member_list(crew_member_list_t& crew_member_list, const std::size_t size_add = 0ull) const;
		void get_defense_mission_record_list(defense_mission_record_list_t& defense_mission_record_list, const std::size_t size_add = 0ull) const;

		bool set_mission_record_list(mission_record_list_t& set_mission_record_list) const;
		bool set_quest_record_list(quest_record_list_t& quest_record_list) const;
		bool set_nonstackable_item_list(nonstackable_item_list_t& nonstackable_list) const;
		bool set_inventory_resource_list(inventory_resource_list_t& inventory_resource_list) const;
		bool set_stackable_item_list(stackable_item_list_t& stackable_item_list) const;
		bool set_map_unlock_list_afghan(map_unlock_list_t& map_unlock_list) const;
		bool set_map_unlock_list_africa(map_unlock_list_t& map_unlock_list) const;
		bool set_crew_member_list(crew_member_list_t& crew_member_list) const;
		bool set_defense_mission_record_list(defense_mission_record_list_t& defense_mission_record_list) const;

		void set_nameplate(const std::uint16_t nameplate) const;
	};

	std::optional<player> find(const std::uint64_t id);
	std::optional<player> find_by_index(const std::uint64_t user_id, const std::uint64_t player_index);
	std::vector<player> get_player_list(const std::uint64_t user_id);
	std::optional<player> create(const std::uint64_t user_id);

	void delete_player_data(const std::uint64_t player_id);
}
