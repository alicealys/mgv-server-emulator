#pragma once

#include "../database.hpp"
#include "players.hpp"
#include "../utils.hpp"
#include "game/game.hpp"

namespace database::deployments
{
	constexpr const auto min_team_count = 1u;
	constexpr const auto max_team_count = 5u;

	enum team_item_type_t
	{
		item_type_resource = 0,
		item_type_stackable = 1,
		item_type_nonstackable = 2,
		item_type_count = 3
	};

	enum team_status_t
	{
		team_status_none = 0,
		team_status_deploy_none = 1,
		team_status_unk2 = 2,
		team_status_deploy_progress = 3,
		team_status_deploy_success = 4,
		team_status_deploy_fail = 5,
	};

	union team_item_t
	{
		struct
		{
			std::uint16_t id_index;
			std::uint16_t param1;
			std::uint16_t param2;
			std::uint8_t type;
			std::uint8_t unused;
		} f;
		std::uint64_t packed;

		void to_json(json::value& data) const;
	};

	static_assert(sizeof(team_item_t) == 8);

	struct deployment_mission_info_t
	{
		struct reward_t
		{
			std::uint32_t combat_rate;
			std::uint32_t rate;
			std::uint32_t survival_rate;
			game::item_t item_info;
		};

		std::uint32_t combat;
        std::uint32_t combat_penalty;
        std::uint32_t combat_revise;
        std::uint32_t destination_id;
        std::uint32_t difficulty;
        std::uint32_t id;
        std::uint32_t inactive;
        std::uint32_t info;
        std::uint32_t injure_base;
        std::uint32_t injure_max;
        std::uint32_t injure_min;
        std::uint32_t is_new;
        std::uint32_t limit_index;
        std::uint32_t limit_max;
        std::uint32_t location;
        std::uint64_t name_id_01;
        std::uint64_t name_id_02;
        std::int32_t pos_x;
        std::int32_t pos_z;
        std::uint32_t required_time;
		std::uint32_t reward_id;
		std::vector<reward_t> reward_list;
		std::uint32_t survival;
		std::uint32_t survival_penalty;
		std::uint32_t survival_revise;
		std::uint32_t time_revise;
		std::uint32_t type;

		bool parse(json::value& data);
		void to_json(json::value& data) const;
	};

	struct team_params_internal_t
	{
		std::uint32_t use_fast_travel;
        std::uint32_t required_combat;
        std::uint32_t required_survive;
        std::uint32_t required_time;
		std::uint32_t success_rate;
		std::uint32_t injure_rate;
	};

	using team_params_t = database_struct<team_params_internal_t>;

	struct deploy_params_t
	{
		std::uint32_t combat;
		std::uint32_t survive;
		std::uint32_t status;
		std::string name;
		std::uint32_t mission_id;
		std::uint32_t mission_type;
		std::uint32_t mission_info;
		std::chrono::system_clock::time_point complete_date;
		team_params_t team_params;
		std::array<std::uint64_t, 4> crew_ids;
	};

	const std::vector<deployment_mission_info_t>& get_deployments_mission_list();
	const std::unordered_map<std::uint32_t, deployment_mission_info_t>& get_deployments_mission_map();

	class deployment_team
	{
	public:
		DEFINE_FIELD(team_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(f_player_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(team_index, sqlpp::integer_unsigned);
		DEFINE_FIELD(params, sqlpp::text);
		DEFINE_FIELD(info_name, sqlpp::text);
		DEFINE_FIELD(info_status, sqlpp::integer_unsigned);
		DEFINE_FIELD(info_combat, sqlpp::integer_unsigned);
		DEFINE_FIELD(info_survive, sqlpp::integer_unsigned);
		DEFINE_FIELD(mission_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(mission_type, sqlpp::integer_unsigned);
		DEFINE_FIELD(mission_info, sqlpp::integer_unsigned);
		DEFINE_FIELD(crew_id_01, sqlpp::integer_unsigned);
		DEFINE_FIELD(crew_id_02, sqlpp::integer_unsigned);
		DEFINE_FIELD(crew_id_03, sqlpp::integer_unsigned);
		DEFINE_FIELD(crew_id_04, sqlpp::integer_unsigned);
		DEFINE_FIELD(item_01, sqlpp::integer_unsigned);
		DEFINE_FIELD(item_02, sqlpp::integer_unsigned);
		DEFINE_FIELD(item_03, sqlpp::integer_unsigned);
		DEFINE_FIELD(item_04, sqlpp::integer_unsigned);
		DEFINE_FIELD(item_05, sqlpp::integer_unsigned);
		DEFINE_FIELD(complete_date, sqlpp::time_point);
		DEFINE_FIELD(creation_date, sqlpp::time_point);
		DEFINE_TABLE(deployment_teams,
			team_id_field_t,
			f_player_id_field_t,
			team_index_field_t,
			params_field_t,
			info_name_field_t,
			info_status_field_t,
			info_combat_field_t,
			info_survive_field_t,
			mission_id_field_t,
			mission_type_field_t,
			mission_info_field_t,
			crew_id_01_field_t,
			crew_id_02_field_t,
			crew_id_03_field_t,
			crew_id_04_field_t,
			item_01_field_t,
			item_02_field_t,
			item_03_field_t,
			item_04_field_t,
			item_05_field_t,
			complete_date_field_t,
			creation_date_field_t
		);

		inline static table_t table;

		template <typename ...Args>
		deployment_team(const sqlpp::result_row_t<Args...>& row)
		{
			this->team_id_ = row.team_id;
			this->player_id_ = row.f_player_id;
			this->index_ = row.team_index;
			this->info_name_ = row.info_name;
			this->info_status_ = static_cast<std::uint32_t>(row.info_status);
			this->info_combat_ = static_cast<std::uint32_t>(row.info_combat);
			this->info_survive_ = static_cast<std::uint32_t>(row.info_survive);
			this->mission_id_ = static_cast<std::uint32_t>(row.mission_id);
			this->mission_type_ = static_cast<std::uint32_t>(row.mission_type);
			this->mission_info_ = static_cast<std::uint32_t>(row.mission_info);
			this->crew_id_01_ = row.crew_id_01;
			this->crew_id_02_ = row.crew_id_02;
			this->crew_id_03_ = row.crew_id_03;
			this->crew_id_04_ = row.crew_id_04;
			this->item_01_.packed = row.item_01;
			this->item_02_.packed = row.item_02;
			this->item_03_.packed = row.item_03;
			this->item_04_.packed = row.item_04;
			this->item_05_.packed = row.item_05;
			this->complete_date_ = std::chrono::duration_cast<std::chrono::seconds>(row.complete_date.value().time_since_epoch());
			this->creation_date_ = std::chrono::duration_cast<std::chrono::seconds>(row.creation_date.value().time_since_epoch());
			this->params_.deserialize(row.params.value());
		}

		GET_FIELD_H(std::uint64_t, team_id);
		GET_FIELD_H(std::uint64_t, player_id);
		GET_FIELD_H(std::uint64_t, index);
		GET_FIELD_H(std::string, info_name);
		GET_FIELD_H(std::uint32_t, info_status);
		GET_FIELD_H(std::uint32_t, info_combat);
		GET_FIELD_H(std::uint32_t, info_survive);
		GET_FIELD_H(std::uint32_t, mission_id);
		GET_FIELD_H(std::uint32_t, mission_type);
		GET_FIELD_H(std::uint32_t, mission_info);
		GET_FIELD_H(std::uint32_t, mission_name_id_01);
		GET_FIELD_H(std::uint32_t, mission_name_id_02);
		GET_FIELD_H(std::uint64_t, crew_id_01);
		GET_FIELD_H(std::uint64_t, crew_id_02);
		GET_FIELD_H(std::uint64_t, crew_id_03);
		GET_FIELD_H(std::uint64_t, crew_id_04);
		GET_FIELD_H(team_item_t, item_01);
		GET_FIELD_H(team_item_t, item_02);
		GET_FIELD_H(team_item_t, item_03);
		GET_FIELD_H(team_item_t, item_04);
		GET_FIELD_H(team_item_t, item_05);
		GET_FIELD_H(std::chrono::seconds, complete_date);
		GET_FIELD_H(std::chrono::seconds, creation_date);

		const team_params_t& get_params() const;
		team_item_t get_item(const std::uint32_t index) const;

		void update_info(const std::uint32_t combat, const std::string& name, const std::uint32_t survive) const;
		void update_status(const std::uint32_t status) const;
		void update_mission(const std::uint32_t mission_id, const std::uint32_t mission_info, const std::uint32_t mission_type, const std::uint32_t required_time) const;
		void update_params(const team_params_t& params) const;
		void update_item(const std::uint32_t index, const team_item_t& item) const;
		void update_crew_ids(const std::uint64_t crew_id_01, const std::uint64_t crew_id_02, const std::uint64_t crew_id_03, const std::uint64_t crew_id_04) const;

		bool deploy(const deploy_params_t& params) const;

		void to_json(json::value& data) const;

	private:
		team_params_t params_{};

	};

	std::uint64_t create_team(const std::uint64_t player_id);

	std::optional<deployment_team> find_team(const std::uint64_t team_id);
	std::optional<deployment_team> find_team_by_index(const std::uint64_t player_id, const std::uint64_t index);

	std::vector<deployment_team> get_all_teams(const std::uint64_t player_id);
	std::size_t get_team_count(const std::uint64_t player_id);

	void update_team_info_by_index(const std::uint64_t player_id, const std::uint64_t team_index, const std::uint32_t combat,
		const std::string& name, const std::uint32_t status, const std::uint32_t survive);
	void update_team_params_by_index(const std::uint64_t player_id, const std::uint64_t team_index, const team_params_t& params);

	void delete_player_data(const std::uint64_t player_id);
}
