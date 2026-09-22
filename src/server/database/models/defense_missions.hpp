#pragma once

#include "../database.hpp"
#include "players.hpp"
#include "../utils.hpp"

namespace database::defense_missions
{
#pragma pack(push, 1)
	struct injury_crew_t
	{
		char crew_name[32];
		std::uint16_t injury_flag;
		std::uint16_t face_id;
		std::uint16_t server_unique_id;
		std::uint16_t group_id;

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct reward_t
	{
		std::uint8_t category;
		std::uint32_t code;
		std::uint32_t param1;
		std::uint32_t param2;
		std::uint32_t param3;
		std::uint32_t param4;
		std::uint32_t param5;
		std::uint32_t num;

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct broken_facility_t
	{
		std::uint16_t product_index;
		std::uint32_t num;

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct wave_params_t
	{
		std::uint32_t attack_power_1;
		std::uint32_t attack_power_2;
		std::uint32_t attack_power_3;
		std::uint32_t attack_power_4;
		std::uint32_t attack_power_1_after;
		std::uint32_t attack_power_2_after;
		std::uint32_t attack_power_3_after;
		std::uint32_t attack_power_4_after;
		std::uint32_t defense_flag_1;
		std::uint32_t defense_flag_2;
		std::uint32_t defense_flag_3;
		std::uint32_t defense_flag_4;
		std::uint32_t defense_power_1_before;
		std::uint32_t defense_power_2_before;
		std::uint32_t defense_power_3_before;
		std::uint32_t defense_power_4_before;
		std::uint32_t defense_power_1_after;
		std::uint32_t defense_power_2_after;
		std::uint32_t defense_power_3_after;
		std::uint32_t defense_power_4_after;
		std::uint32_t water_before;
		std::uint32_t water_after;
		std::uint32_t food_before;
		std::uint32_t food_after;
		std::uint32_t medical_before;
		std::uint32_t medical_after;
		std::uint32_t mining_machine_life_before;
		std::uint32_t mining_machine_life_after;
		std::uint32_t mining_machine_life_max;

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct wave_result_t
	{
		std::uint32_t start_date;
		std::uint32_t end_date;
		std::uint8_t result;
		std::uint8_t wave;
		std::uint32_t total_score;
		std::uint32_t next_wave_start_date;
	};
#pragma pack(pop)

	struct reward_info_t
	{
		std::uint8_t rank;
		std::uint32_t id_index;
		reward_t param;
	};

	enum reward_type_t
	{
		reward_type_invalid = 0,
		reward_type_resource = 1,
		reward_type_item = 2,
	};

	struct reward_pool_t
	{
		struct reward_t
		{
			std::uint32_t id;
			std::uint32_t max;
			std::string type;
			std::uint32_t reward_type;
			std::shared_ptr<game::resource_t> resource;
			std::shared_ptr<game::production_t> item;
		};

		struct rank_t
		{
			std::uint32_t count;
			std::uint32_t energy;
			std::vector<std::uint32_t> recipe_list;
			std::vector<reward_t> rewards;
		};

		std::uint32_t mission_code;
		std::array<rank_t, 6> reward_pool;
	};

	std::uint8_t calc_rank(const game::defense_mission_settings_t& mission_settings, const std::uint32_t score);
	std::vector<reward_info_t> generate_rewards(const game::defense_mission_settings_t& mission_settings, const std::uint8_t rank);

	class broken_facility_list_t final : public generic_item_list<broken_facility_t, 256>
	{
	public:
		inline bool are_elements_equal(const broken_facility_t& l, const broken_facility_t& r) const override
		{
			return false;
		}

		inline bool is_element_empty(const broken_facility_t& value) const override
		{
			return false;
		}
	};

	class injury_crew_list_t final : public generic_item_list<injury_crew_t, 30>
	{
	public:
		inline bool are_elements_equal(const injury_crew_t& l, const injury_crew_t& r) const override
		{
			return l.server_unique_id == r.server_unique_id;
		}

		inline bool is_element_empty(const injury_crew_t& value) const override
		{
			return value.server_unique_id == 0;
		}
	};

	class reward_list_t final : public generic_item_list<reward_t, 32>
	{
	public:
		inline bool are_elements_equal(const reward_t& l, const reward_t& r) const override
		{
			return false;
		}

		inline bool is_element_empty(const reward_t& value) const override
		{
			return value.code == 0;
		}
	};

	class defense_mission_wave
	{
	public:
		DEFINE_FIELD(defense_mission_wave_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(f_defense_mission_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(player_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(wave, sqlpp::integer_unsigned);
		DEFINE_FIELD(start_date, sqlpp::time_point);
		DEFINE_FIELD(end_date, sqlpp::time_point);
		DEFINE_FIELD(total_score, sqlpp::integer_unsigned);
		DEFINE_FIELD(result, sqlpp::integer_unsigned);
		DEFINE_FIELD(params, sqlpp::binary);
		DEFINE_FIELD(injury_crew_list, sqlpp::binary);
		DEFINE_FIELD(broken_facility_list, sqlpp::binary);
		DEFINE_FIELD(reward_list, sqlpp::binary);
		DEFINE_TABLE(defense_mission_waves,
			defense_mission_wave_id_field_t,
			f_defense_mission_id_field_t,
			wave_field_t,
			start_date_field_t,
			end_date_field_t,
			total_score_field_t,
			result_field_t,
			params_field_t,
			injury_crew_list_field_t,
			broken_facility_list_field_t,
			reward_list_field_t
		);

		inline static table_t table;

		template <typename ...Args>
		defense_mission_wave(const sqlpp::result_row_t<Args...>& row)
		{
			this->defense_mission_wave_id_ = row.defense_mission_wave_id;
			this->defense_mission_id_ = row.f_defense_mission_id;
			this->wave_ = static_cast<std::uint8_t>(row.wave);
			this->total_score_ = static_cast<std::uint32_t>(row.total_score);
			this->result_ = static_cast<std::uint8_t>(row.result);
			this->start_date_ = std::chrono::duration_cast<std::chrono::seconds>(row.start_date.value().time_since_epoch());
			this->end_date_ = std::chrono::duration_cast<std::chrono::seconds>(row.end_date.value().time_since_epoch());
			
			load_binary_field(&this->params_, row.params.value());
			this->broken_facility_list_.deserialize(row.broken_facility_list.value());
			this->injury_crew_list_.deserialize(row.injury_crew_list.value());
			this->reward_list_.deserialize(row.reward_list.value());
		}

		GET_FIELD_H(std::uint64_t, defense_mission_wave_id);
		GET_FIELD_H(std::uint64_t, defense_mission_id);
		GET_FIELD_H(std::uint8_t, wave);
		GET_FIELD_H(std::uint32_t, total_score);
		GET_FIELD_H(std::uint8_t, result);
		GET_FIELD_H(std::chrono::seconds, start_date);
		GET_FIELD_H(std::chrono::seconds, end_date);

		void to_json(json::value& data) const;

	private:
		wave_params_t params_;
		broken_facility_list_t broken_facility_list_;
		injury_crew_list_t injury_crew_list_;
		reward_list_t reward_list_;

	};

	class defense_mission
	{
	public:
		DEFINE_FIELD(defense_mission_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(f_player_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(mission_code, sqlpp::integer_unsigned);
		DEFINE_FIELD(wave, sqlpp::integer_unsigned);
		DEFINE_FIELD(current_wave, sqlpp::integer_unsigned);
		DEFINE_FIELD(clear_rank, sqlpp::integer_unsigned);
		DEFINE_FIELD(total_score, sqlpp::integer_unsigned);
		DEFINE_FIELD(result, sqlpp::integer_unsigned);
		DEFINE_FIELD(start_date, sqlpp::time_point);
		DEFINE_FIELD(next_wave_date, sqlpp::time_point);
		DEFINE_FIELD(end_date, sqlpp::time_point);
		DEFINE_TABLE(defense_missions,
			defense_mission_id_field_t,
			f_player_id_field_t,
			mission_code_field_t,
			current_wave_field_t,
			clear_rank_field_t,
			total_score_field_t,
			result_field_t,
			start_date_field_t,
			next_wave_date_field_t,
			end_date_field_t
		);

		inline static table_t table;

		template <typename ...Args>
		defense_mission(const sqlpp::result_row_t<Args...>& row)
		{
			this->defense_mission_id_ = row.defense_mission_id;
			this->player_id_ = row.f_player_id;
			this->mission_code_ = static_cast<std::uint16_t>(row.mission_code);
			this->current_wave_ = static_cast<std::uint8_t>(row.current_wave);
			this->clear_rank_ = static_cast<std::uint8_t>(row.clear_rank);
			this->result_ = static_cast<std::uint8_t>(row.result);
			this->total_score_ = static_cast<std::uint32_t>(row.total_score);
			this->start_date_ = std::chrono::duration_cast<std::chrono::seconds>(row.start_date.value().time_since_epoch());
			this->next_wave_date_ = std::chrono::duration_cast<std::chrono::seconds>(row.next_wave_date.value().time_since_epoch());
			this->end_date_ = std::chrono::duration_cast<std::chrono::seconds>(row.end_date.value().time_since_epoch());
		}

		GET_FIELD_H(std::uint64_t, defense_mission_id);
		GET_FIELD_H(std::uint64_t, player_id);
		GET_FIELD_H(std::uint16_t, mission_code);
		GET_FIELD_H(std::uint8_t, current_wave);
		GET_FIELD_H(std::uint8_t, clear_rank);
		GET_FIELD_H(std::uint8_t, result);
		GET_FIELD_H(std::uint32_t, total_score);
		GET_FIELD_H(std::chrono::seconds, start_date);
		GET_FIELD_H(std::chrono::seconds, next_wave_date);
		GET_FIELD_H(std::chrono::seconds, end_date);

		void to_json(json::value& data) const;
		std::vector<defense_mission_wave> get_waves() const;
		void update(const std::uint8_t result, const std::uint8_t current_wave, 
			const std::uint8_t clear_rank, const std::uint32_t total_score, 
			const std::chrono::seconds end_date, const std::chrono::seconds next_wave_date) const;

	};

	std::optional<defense_mission> find(const std::uint64_t defense_mission_id);
	std::vector<defense_mission> get_history(const std::uint64_t player_id, const std::uint32_t limit = 30u);
	std::optional<defense_mission> get_current_mission(const std::uint64_t player_id);
	std::vector<defense_mission_wave> get_waves(const std::uint64_t defense_mission_id);
	defense_mission find_or_create(const std::uint64_t player_id, const std::uint16_t mission_code, const std::uint32_t max_wave);
	std::uint64_t create_mission(const std::uint64_t player_id, const std::uint16_t mission_code);

	std::uint64_t add_wave(const std::uint64_t defense_mission_id, 
		wave_result_t& result,
		wave_params_t& params,
		broken_facility_list_t& broken_facility_list,
		injury_crew_list_t& injury_crew_list,
		reward_list_t& reward_list);
}
