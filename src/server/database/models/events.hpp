#pragma once

#include "../database.hpp"
#include "game/game.hpp"

#include <utils/concurrency.hpp>

namespace database::events
{
	struct event_ranking_reward_t
	{
		std::uint64_t reward_id;
		std::uint32_t start_rank;
		std::uint32_t end_rank;
		std::vector<game::item_t> item_info;
	};

	struct event_border_reward_t
	{
		std::uint64_t reward_id;
		std::uint32_t border_point;
		game::item_t item_info;
	};

	struct event_catalog_reward_t
	{
		std::uint64_t reward_id;
		std::uint32_t price_point;
		std::uint32_t purchase_limit;
		game::item_t item_info;
	};

	enum reward_type_t : std::uint8_t
	{
		reward_type_none = 0,
		reward_type_border = 1,
		reward_type_catalog = 2,
		reward_type_ranking = 3,
		reward_type_count = 4
	};

	struct event_params_t
	{
		std::string name;
		std::uint8_t enable_border;
		std::uint8_t enable_catalog;
		std::uint8_t enable_ranking;
		std::string event_name;
		std::uint32_t info_id;
		std::string info_title;
		std::string info_text;
		std::vector<event_border_reward_t> reward_border_list;
		std::vector<event_catalog_reward_t> reward_catalog_list;
		std::vector<event_ranking_reward_t> reward_ranking_list;

		void to_json(json::value& data) const;
	};

	struct event_settings_t
	{
		std::uint32_t event_duration;
		std::vector<std::shared_ptr<event_params_t>> event_list;
		std::unordered_map<std::string, std::shared_ptr<event_params_t>> event_type_map;
		std::vector<std::string> event_rotation;
		std::vector<std::shared_ptr<event_params_t>> event_rotation_linked;
	};

	struct running_event_info_t
	{
		std::chrono::seconds start;
		std::chrono::seconds end;
		std::shared_ptr<event_params_t> params;
	};

	const event_settings_t& get_event_settings();
	std::optional<running_event_info_t> get_target_event();
	void get_event_range(std::chrono::seconds& start, std::chrono::seconds& end);
	std::shared_ptr<event_params_t> get_event_params(const std::string& event_type);

	enum event_state_t
	{
		event_state_none = 0u,
		event_state_running = 1u,
		event_state_finished = 2u,
		event_state_dead = 3u,
	};
	
	enum event_ranking_state_t
	{
		ranking_state_none = 0u,
		ranking_state_ack = 1u
	};

	class running_event
	{
	public:
		DEFINE_FIELD(event_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(event_type, sqlpp::text);
		DEFINE_FIELD(event_state, sqlpp::integer_unsigned);
		DEFINE_FIELD(start_date, sqlpp::time_point);
		DEFINE_FIELD(end_date, sqlpp::time_point);
		DEFINE_TABLE(running_events,
			event_id_field_t,
			event_type_field_t,
			event_state_field_t,
			start_date_field_t,
			end_date_field_t
		);

		inline static table_t table;

		template <typename ...Args>
		running_event(const sqlpp::result_row_t<Args...>& row)
		{
			this->event_id_ = row.event_id;
			this->event_type_ = row.event_type;
			this->event_state_ = static_cast<std::uint32_t>(row.event_state);
			this->start_date_ = std::chrono::duration_cast<std::chrono::seconds>(row.start_date.value().time_since_epoch());
			this->end_date_ = std::chrono::duration_cast<std::chrono::seconds>(row.end_date.value().time_since_epoch());
		}

		bool initialize_params();

		GET_FIELD_H(std::uint64_t, event_id);
		GET_FIELD_H(std::string, event_type);
		GET_FIELD_H(std::uint32_t, event_state);
		GET_FIELD_H(std::chrono::seconds, start_date);
		GET_FIELD_H(std::chrono::seconds, end_date);

		std::shared_ptr<event_params_t> params;

	};

	class event_reward
	{
	public:
		DEFINE_FIELD(event_reward_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(f_event_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(f_user_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(reward_type, sqlpp::integer_unsigned);
		DEFINE_FIELD(reward_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(acquired, sqlpp::boolean);
		DEFINE_FIELD(create_date, sqlpp::time_point);
		DEFINE_TABLE(event_rewards,
			event_reward_id_field_t,
			f_event_id_field_t,
			f_user_id_field_t,
			reward_type_field_t,
			reward_id_field_t,
			acquired_field_t,
			create_date_field_t
		);

		inline static table_t table;

		template <typename ...Args>
		event_reward(const sqlpp::result_row_t<Args...>& row)
		{
			this->event_reward_id_ = row.event_reward_id;
			this->event_id_ = row.f_event_id;
			this->event_type_ = row.event_type;
			this->user_id_ = row.f_user_id;
			this->reward_type_ = static_cast<std::uint8_t>(row.reward_type);
			this->reward_id_ = row.reward_id;
			this->create_date_ = std::chrono::duration_cast<std::chrono::seconds>(row.create_date.value().time_since_epoch());
		}

		GET_FIELD_H(std::uint64_t, event_reward_id);
		GET_FIELD_H(std::uint64_t, event_id);
		GET_FIELD_H(std::string, event_type);
		GET_FIELD_H(std::uint64_t, user_id);
		GET_FIELD_H(std::uint8_t, reward_type);
		GET_FIELD_H(std::uint64_t, reward_id);
		GET_FIELD_H(bool, acquired);
		GET_FIELD_H(std::chrono::seconds, create_date);
	};

	bool add_reward(const std::uint64_t event_id, const std::uint64_t user_id, const std::uint8_t reward_type, const std::uint64_t reward_id, const bool acquired);
	std::vector<event_reward> get_acquired_rewards(const std::uint64_t event_id, const std::uint64_t user_id);

	std::vector<event_reward> get_entitlements(const std::uint64_t current_event_id, const std::uint64_t user_id);
	void clear_entitlements(const std::uint64_t current_event_id, const std::uint64_t user_id);

	extern utils::concurrency::container<std::optional<running_event>> cached_event;

	std::optional<running_event> get_current_event();
}
