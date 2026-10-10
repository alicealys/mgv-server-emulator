#pragma once

#include "../database.hpp"
#include "players.hpp"
#include "../utils.hpp"
#include "game/game.hpp"

namespace database::coop_missions
{
	struct coop_mission_record_t
	{
		std::uint8_t clear_rank;
		std::uint32_t iris_score;
		std::uint32_t mission_code;
		std::uint32_t personal_score;
		std::uint32_t rescue;
		std::uint32_t waves;

		void to_json(json::value& data) const;
	};

	enum mission_type_t : std::uint8_t
	{
		mission_type_none = 0,
		mission_type_embedded = 1,
		mission_type_event = 2,
	};

	class coop_mission_result
	{
	public:
		DEFINE_FIELD(coop_mission_result_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(f_player_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(mission_code, sqlpp::integer_unsigned);
		DEFINE_FIELD(mission_type, sqlpp::integer_unsigned);
		DEFINE_FIELD(clear_rank, sqlpp::integer_unsigned);
		DEFINE_FIELD(score, sqlpp::integer_unsigned);
		DEFINE_FIELD(personal_score, sqlpp::integer_unsigned);
		DEFINE_FIELD(waves, sqlpp::integer_unsigned);
		DEFINE_FIELD(rescue, sqlpp::integer_unsigned);
		DEFINE_FIELD(create_date, sqlpp::time_point);
		DEFINE_TABLE(coop_mission_results,
			coop_mission_result_id_field_t,
			f_player_id_field_t,
			mission_code_field_t,
			mission_type_field_t,
			clear_rank_field_t,
			score_field_t,
			personal_score_field_t,
			waves_field_t,
			rescue_field_t,
			create_date_field_t
		);

		inline static table_t table;

		template <typename ...Args>
		coop_mission_result(const sqlpp::result_row_t<Args...>& row)
		{
			this->coop_mission_result_id_ = row.coop_mission_result_id;
			this->player_id_ = row.player_id;
			this->mission_code_ = static_cast<std::uint32_t>(row.mission_code);
			this->mission_type_ = static_cast<std::uint8_t>(row.mission_type);
			this->score_ = static_cast<std::uint32_t>(row.score);
			this->personal_score_ = static_cast<std::uint32_t>(row.personal_score);
			this->rescue_ = static_cast<std::uint8_t>(row.rescue);
			this->waves_ = static_cast<std::uint8_t>(row.waves);
			this->clear_rank_ = static_cast<std::uint8_t>(row.clear_rank);
			this->create_date_ = std::chrono::duration_cast<std::chrono::seconds>(row.create_date.value().time_since_epoch());
		}

		GET_FIELD_H(std::uint64_t, coop_mission_result_id);
		GET_FIELD_H(std::uint64_t, player_id);
		GET_FIELD_H(std::uint32_t, mission_code);
		GET_FIELD_H(std::uint32_t, score);
		GET_FIELD_H(std::uint32_t, personal_score);
		GET_FIELD_H(std::uint8_t, mission_type);
		GET_FIELD_H(std::uint8_t, clear_rank);
		GET_FIELD_H(std::uint8_t, waves);
		GET_FIELD_H(std::uint8_t, rescue);
		GET_FIELD_H(std::chrono::seconds, create_date);

	};

	void add_result(const std::uint64_t player_id, const std::uint8_t mission_type, const coop_mission_record_t& param);
	std::vector<coop_mission_record_t> get_record_list(const std::uint64_t player_id, const std::uint8_t mission_type);

	class coop_mission
	{
	public:
		DEFINE_FIELD(coop_mission_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(owner_player_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(mission_code, sqlpp::integer_unsigned);
		DEFINE_FIELD(flag, sqlpp::integer_unsigned);
		DEFINE_FIELD(create_date, sqlpp::time_point);
		DEFINE_TABLE(coop_missions,
			coop_mission_id_field_t,
			owner_player_id_field_t,
			mission_code_field_t,
			flag_field_t,
			create_date_field_t
		);

		inline static table_t table;

		template <typename ...Args>
		coop_mission(const sqlpp::result_row_t<Args...>& row)
		{
			this->coop_mission_id_ = row.coop_mission_id;
			this->owner_player_id_ = row.owner_player_id;
			this->mission_code_ = row.mission_code;
			this->flag_ = row.flag;
			this->create_date_ = std::chrono::duration_cast<std::chrono::seconds>(row.create_date.value().time_since_epoch());
		}

		GET_FIELD_H(std::uint64_t, coop_mission_id);
		GET_FIELD_H(std::uint64_t, owner_player_id);
		GET_FIELD_H(std::uint32_t, mission_code);
		GET_FIELD_H(std::uint8_t, flag);
		GET_FIELD_H(std::chrono::seconds, create_date);

	};

	std::uint64_t start_mission(const std::uint64_t lobby_owner_id, const std::uint32_t mission_code, const std::uint8_t flag);

	void delete_player_data(const std::uint64_t player_id);
}
