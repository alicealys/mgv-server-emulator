#pragma once

#include "../database.hpp"
#include "players.hpp"
#include "../utils.hpp"
#include "game/game.hpp"

namespace database::crew_members
{
	constexpr const auto max_crew_members = 30ull;
	
	struct motivation_history_internal_t
	{
		std::uint8_t event[10];
		std::uint8_t value[10];

		void to_json(json::value& data) const;
	};

	using motivation_history_t = database_struct<motivation_history_internal_t>;

	struct member_params_t
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
		std::uint64_t unique_id;
		std::uint32_t sex_id;
		std::uint32_t unique_index;
		motivation_history_t motivation_history;
		char nickname[32];

		bool parse(json::value& data);
		bool parse_update(json::value& data);
		bool parse_add_param(json::value& data);
		void apply_update(const member_params_t& diff);
	};

	class crew_member
	{
	public:
		DEFINE_FIELD(member_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(f_player_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(member_type, sqlpp::integer_unsigned);
		DEFINE_FIELD(face_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(body_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(race_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(sex_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(voice_type, sqlpp::integer_unsigned);
		DEFINE_FIELD(previous_group, sqlpp::integer_unsigned);
		DEFINE_FIELD(current_group, sqlpp::integer_unsigned);
		DEFINE_FIELD(previous_job, sqlpp::integer_unsigned);
		DEFINE_FIELD(item1_count, sqlpp::integer_unsigned);
		DEFINE_FIELD(item2_count, sqlpp::integer_unsigned);
		DEFINE_FIELD(item3_count, sqlpp::integer_unsigned);
		DEFINE_FIELD(item4_count, sqlpp::integer_unsigned);
		DEFINE_FIELD(item5_count, sqlpp::integer_unsigned);
		DEFINE_FIELD(item6_count, sqlpp::integer_unsigned);
		DEFINE_FIELD(life, sqlpp::integer_unsigned);
		DEFINE_FIELD(life_max, sqlpp::integer_unsigned);
		DEFINE_FIELD(health_condition, sqlpp::integer_unsigned);
		DEFINE_FIELD(health_flag, sqlpp::integer_unsigned);
		DEFINE_FIELD(map_location, sqlpp::integer_unsigned);
		DEFINE_FIELD(skill, sqlpp::integer_unsigned);
		DEFINE_FIELD(sanity, sqlpp::integer_unsigned);
		DEFINE_FIELD(survival_days, sqlpp::integer_unsigned);
		DEFINE_FIELD(injury_id_1, sqlpp::integer_unsigned);
		DEFINE_FIELD(injury_id_2, sqlpp::integer_unsigned);
		DEFINE_FIELD(injury_time_1, sqlpp::integer_unsigned);
		DEFINE_FIELD(injury_time_2, sqlpp::integer_unsigned);
		DEFINE_FIELD(sickness_id_1, sqlpp::integer_unsigned);
		DEFINE_FIELD(sickness_id_2, sqlpp::integer_unsigned);
		DEFINE_FIELD(sickness_time_1, sqlpp::integer_unsigned);
		DEFINE_FIELD(sickness_time_2, sqlpp::integer_unsigned);
		DEFINE_FIELD(nickname, sqlpp::text);
		DEFINE_FIELD(motivation_history, sqlpp::text);
		DEFINE_FIELD(creation_date, sqlpp::time_point);
		DEFINE_TABLE(crew_members,
			member_id_field_t,
			f_player_id_field_t,
			member_type_field_t,
			face_id_field_t,
			body_id_field_t,
			race_id_field_t,
			sex_id_field_t,
			voice_type_field_t,
			previous_group_field_t,
			current_group_field_t,
			previous_job_field_t,
			item1_count_field_t,
			item2_count_field_t,
			item3_count_field_t,
			item4_count_field_t,
			item5_count_field_t,
			item6_count_field_t,
			life_field_t,
			life_max_field_t,
			health_condition_field_t,
			health_flag_field_t,
			map_location_field_t,
			skill_field_t,
			sanity_field_t,
			survival_days_field_t,
			injury_id_1_field_t,
			injury_id_2_field_t,
			injury_time_1_field_t,
			injury_time_2_field_t,
			sickness_id_1_field_t,
			sickness_id_2_field_t,
			sickness_time_1_field_t,
			sickness_time_2_field_t,
			nickname_field_t,
			motivation_history_field_t,
			creation_date_field_t
		);

		inline static table_t table;

		template <typename ...Args>
		crew_member(const sqlpp::result_row_t<Args...>& row)
		{
			this->member_id_ = row.member_id;
			this->player_id_ = row.f_player_id;

			this->member_type_ = static_cast<std::uint32_t>(row.member_type);
			this->face_id_ = static_cast<std::uint32_t>(row.face_id);
			this->body_id_ = static_cast<std::uint32_t>(row.body_id);
			this->race_id_ = static_cast<std::uint32_t>(row.race_id);
			this->sex_id_ = static_cast<std::uint32_t>(row.sex_id);
			this->voice_type_ = static_cast<std::uint32_t>(row.voice_type);
			this->previous_group_ = static_cast<std::uint32_t>(row.previous_group);
			this->current_group_ = static_cast<std::uint32_t>(row.current_group);
			this->previous_job_ = static_cast<std::uint32_t>(row.previous_job);
			this->item1_count_ = static_cast<std::uint32_t>(row.item1_count);
			this->item2_count_ = static_cast<std::uint32_t>(row.item2_count);
			this->item3_count_ = static_cast<std::uint32_t>(row.item3_count);
			this->item4_count_ = static_cast<std::uint32_t>(row.item4_count);
			this->item5_count_ = static_cast<std::uint32_t>(row.item5_count);
			this->item6_count_ = static_cast<std::uint32_t>(row.item6_count);
			this->life_ = static_cast<std::uint32_t>(row.life);
			this->life_max_ = static_cast<std::uint32_t>(row.life_max);
			this->condition_ = static_cast<std::uint32_t>(row.health_condition);
			this->health_flag_ = static_cast<std::uint32_t>(row.health_flag);
			this->map_location_ = static_cast<std::uint32_t>(row.map_location);
			this->sanity_ = static_cast<std::uint32_t>(row.sanity);
			this->skill_ = static_cast<std::uint32_t>(row.skill);
			this->survival_days_ = static_cast<std::uint32_t>(row.survival_days);
			this->injury_id_1_ = static_cast<std::uint32_t>(row.injury_id_1);
			this->injury_id_2_ = static_cast<std::uint32_t>(row.injury_id_2);
			this->injury_time_1_ = static_cast<std::uint32_t>(row.injury_time_1);
			this->injury_time_2_ = static_cast<std::uint32_t>(row.injury_time_2);
			this->sickness_id_1_ = static_cast<std::uint32_t>(row.sickness_id_1);
			this->sickness_id_2_ = static_cast<std::uint32_t>(row.sickness_id_2);
			this->sickness_time_1_ = static_cast<std::uint32_t>(row.sickness_time_1);
			this->sickness_time_2_ = static_cast<std::uint32_t>(row.sickness_time_2);

			this->nickname_ = row.nickname;
			this->motivation_history_.deserialize(row.motivation_history.value());

			this->creation_date_ = std::chrono::duration_cast<std::chrono::seconds>(row.creation_date.value().time_since_epoch());
		}

		GET_FIELD_H(std::uint64_t, member_id);
		GET_FIELD_H(std::uint64_t, player_id);
		GET_FIELD_H(std::uint32_t, member_type);
		GET_FIELD_H(std::uint32_t, face_id);
		GET_FIELD_H(std::uint32_t, body_id);
		GET_FIELD_H(std::uint32_t, race_id);
		GET_FIELD_H(std::uint32_t, sex_id);
		GET_FIELD_H(std::uint32_t, voice_type);
		GET_FIELD_H(std::uint32_t, previous_group);
		GET_FIELD_H(std::uint32_t, current_group);
		GET_FIELD_H(std::uint32_t, previous_job);
		GET_FIELD_H(std::uint32_t, item1_count);
		GET_FIELD_H(std::uint32_t, item2_count);
		GET_FIELD_H(std::uint32_t, item3_count);
		GET_FIELD_H(std::uint32_t, item4_count);
		GET_FIELD_H(std::uint32_t, item5_count);
		GET_FIELD_H(std::uint32_t, item6_count);
		GET_FIELD_H(std::uint32_t, life);
		GET_FIELD_H(std::uint32_t, life_max);
		GET_FIELD_H(std::uint32_t, condition);
		GET_FIELD_H(std::uint32_t, health_flag);
		GET_FIELD_H(std::uint32_t, map_location);
		GET_FIELD_H(std::uint32_t, sanity);
		GET_FIELD_H(std::uint32_t, skill);
		GET_FIELD_H(std::uint32_t, survival_days);
		GET_FIELD_H(std::uint32_t, injury_id_1);
		GET_FIELD_H(std::uint32_t, injury_id_2);
		GET_FIELD_H(std::uint32_t, injury_time_1);
		GET_FIELD_H(std::uint32_t, injury_time_2);
		GET_FIELD_H(std::uint32_t, sickness_id_1);
		GET_FIELD_H(std::uint32_t, sickness_id_2);
		GET_FIELD_H(std::uint32_t, sickness_time_1);
		GET_FIELD_H(std::uint32_t, sickness_time_2);
		GET_FIELD_H(std::string, nickname);
		GET_FIELD_H(std::chrono::seconds, creation_date);

		void to_json(json::value& data) const;

		void update(const member_params_t& params) const;
		void update_group(const std::uint32_t group_id) const;
		void update_nickname(const std::string& nickname) const;

		void update_injury(const std::uint32_t injury_id_1, const std::uint32_t injury_id_2,
			const std::uint32_t injury_time_1, const std::uint32_t injury_time_2) const;
		void update_sickness(const std::uint32_t injury_id_1, const std::uint32_t injury_id_2,
			const std::uint32_t injury_time_1, const std::uint32_t injury_time_2) const;

		void add_items(const std::array<std::uint32_t, 6>& add_counts) const;

	private:
		motivation_history_t motivation_history_{};

	};

	std::uint64_t create(const std::uint64_t player_id, const member_params_t& params);

	std::optional<crew_member> find(const std::uint64_t player_id, const std::uint64_t member_id);
	std::vector<crew_member> get_all(const std::uint64_t player_id);
	std::size_t get_member_count(const std::uint64_t player_id);

	bool remove(const std::uint64_t player_id, const std::uint64_t member_id);
	void remove_all(const std::uint64_t player_id);

	void delete_player_data(const std::uint64_t player_id);
}
