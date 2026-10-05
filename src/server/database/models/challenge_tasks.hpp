#pragma once

#include "../database.hpp"
#include "../utils.hpp"

#include "players.hpp"

#include "utils/time.hpp"

#include "game/game.hpp"

#include <utils/concurrency.hpp>

namespace database::challenge_tasks
{
	struct challenge_task_t
	{
		struct condition_t
		{
			std::uint32_t target_code;
			std::uint32_t required;
			std::uint64_t target_lang;
		};

		struct extra_t
		{
			std::uint8_t category;
			std::uint32_t target;
		};

		std::uint32_t category;
		std::uint32_t require;
		std::uint32_t id;
		std::uint32_t info_id;
		std::uint32_t name_id;
		std::uint32_t target;
		std::uint32_t term;
		std::chrono::seconds expire_date;
		std::vector<condition_t> condition_list;
		std::vector<extra_t> extra_list;
		std::vector<game::item_t> reward_list;

		void to_json(json::value& data) const;
	};

	struct challenge_task_settings_t
	{
		std::size_t daily_task_count;
		std::size_t weekly_task_count;
		std::vector<challenge_task_t> task_list;
	};

	const challenge_task_settings_t& get_settings();
	void load_current_tasks(std::vector<challenge_task_t>& tasks);

	std::chrono::seconds get_next_date();

	template <typename T = void, typename F>
	T access_tasks(F&& accessor)
	{
		static utils::concurrency::container<std::vector<challenge_task_t>> tasks_container;
		return tasks_container.access<T>([&](std::vector<challenge_task_t>& tasks)
		{
			static std::size_t prev_day{};
			static std::size_t prev_week{};

			const auto now_day = utils::time::get_weekday();
			const auto now_week = utils::time::get_week();

			if (prev_day != now_day || prev_week != now_week)
			{
				load_current_tasks(tasks);
				prev_day = now_day;
				prev_week = now_week;
			}

			return accessor(tasks);
		});
	}

	bool is_current_task(const std::uint32_t id);
	std::optional<std::chrono::seconds> get_task_expire_date(const std::uint32_t id);
	std::vector<game::item_t> get_task_rewards(const std::uint32_t id);

	class challenge_task_order
	{
	public:
		DEFINE_FIELD(task_order_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(f_player_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(task_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(is_complete, sqlpp::boolean);
		DEFINE_FIELD(progress, sqlpp::integer_unsigned);
		DEFINE_FIELD(expire_date, sqlpp::time_point);
		DEFINE_TABLE(challenge_task_orders,
			task_order_id_field_t,
			f_player_id_field_t,
			task_id_field_t,
			is_complete_field_t,
			progress_field_t,
			expire_date_field_t
		);

		inline static table_t table;

		template <typename ...Args>
		challenge_task_order(const sqlpp::result_row_t<Args...>& row)
		{
			this->task_order_id_ = row.task_order_id;
			this->player_id_ = row.f_player_id;
			this->task_id_ = static_cast<std::uint32_t>(row.task_id);
			this->is_complete_ = row.is_complete;
			this->progress_ = static_cast<std::uint32_t>(row.progress);
		}

		GET_FIELD_H(std::uint64_t, task_order_id);
		GET_FIELD_H(std::uint64_t, player_id);
		GET_FIELD_H(std::uint32_t, task_id);
		GET_FIELD_H(bool, is_complete);
		GET_FIELD_H(std::uint32_t, progress);
		GET_FIELD_H(std::chrono::seconds, expire_date);

		bool cancel() const;
		bool complete() const;
		bool set_progress(const std::uint32_t progress) const;

	};

	struct order_progress_entry_t
	{
		std::uint32_t id;
		std::uint32_t progress;
	};

	std::optional<challenge_task_order> find_order(const std::uint64_t player_id, const std::uint32_t task_id);
	std::vector<challenge_task_order> get_all_orders(const std::uint64_t player_id);
	std::uint64_t create_order(const std::uint64_t player_id, const std::uint32_t task_id, const std::chrono::system_clock::time_point expire_date);

	void dump_order_list(const std::uint64_t player_id, json::value& order_expired_list, json::value& order_list);
	void send_progress_list(const std::uint64_t player_id, json::value& progress_list);

	void delete_player_data(const std::uint64_t player_id);
}
