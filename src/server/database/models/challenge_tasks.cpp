#include <std_include.hpp>

#include "challenge_tasks.hpp"

#include "utils/encoding.hpp"
#include "utils/time.hpp"

#include <utils/cryptography.hpp>
#include <utils/string.hpp>

namespace database::challenge_tasks
{
	namespace
	{
		challenge_task_settings_t load_settings()
		{
			auto data = utils::resources::load_json(RESOURCE_CHALLENGE_TASK_SETTINGS);
			challenge_task_settings_t settings{};
			if (!json::read(settings, data))
			{
				throw std::runtime_error("[challenge tasks] failed to parse settings");
			}

			return settings;
		}
	}

	void load_current_tasks(std::vector<challenge_task_t>& tasks)
	{
		tasks.clear();

		std::unordered_set<std::uint32_t> used_tasks;

		const auto& settings = load_settings();
		const auto week_number = utils::time::get_week();
		const auto weekday_number = utils::time::get_weekday();

		const auto section_size = settings.daily_task_count * 7 + settings.weekly_task_count;
		const auto section_count = (settings.task_list.size() / section_size);
		const auto section_idx = week_number % section_count;

		const auto task_begin = section_idx * section_size;
		const auto task_daily_begin = task_begin + weekday_number * settings.daily_task_count;

		const auto task_weekly_begin = task_begin + 7 * settings.daily_task_count;

		const auto next_daily = utils::time::get_day_begin() + 24h;
		const auto next_weekly = utils::time::get_week_begin() + 24h * 7;

		for (auto i = task_daily_begin; i < task_daily_begin + settings.daily_task_count; i++)
		{
			const auto idx = i % settings.task_list.size();
			const auto& task = settings.task_list[idx];

			if (used_tasks.contains(task.id))
			{
				continue;
			}

			used_tasks.insert(task.id);

			auto& inserted = tasks.emplace_back(task);
			inserted.term = 0;
			inserted.expire_date = next_daily;
		}

		for (auto i = task_weekly_begin; i < task_weekly_begin + settings.weekly_task_count; i++)
		{
			const auto idx = i % settings.task_list.size();
			const auto& task = settings.task_list[idx];

			if (used_tasks.contains(task.id))
			{
				continue;
			}

			used_tasks.insert(task.id);

			auto& inserted = tasks.emplace_back(task);
			inserted.term = 1;
			inserted.expire_date = next_weekly;
		}
	}

	const challenge_task_settings_t& get_settings()
	{
		static const auto settings = load_settings();
		return settings;
	}

	std::chrono::seconds get_next_date()
	{
		return utils::time::get_day_begin() + 24h;;
	}

	bool is_current_task(const std::uint32_t id)
	{
		return access_tasks<bool>([&](std::vector<challenge_task_t>& tasks)
			-> bool
		{
			const auto iter = std::ranges::find_if(tasks.begin(), tasks.end(), [&](const challenge_task_t& task)
			{
				return task.id == id;
			});

			return iter != tasks.end();
		});
	}

	std::optional<std::chrono::seconds> get_task_expire_date(const std::uint32_t id)
	{
		return access_tasks<std::optional<std::chrono::seconds>>([&](std::vector<challenge_task_t>& tasks)
			-> std::optional<std::chrono::seconds>
		{
			const auto iter = std::ranges::find_if(tasks.begin(), tasks.end(), [&](const challenge_task_t& task)
			{
				return task.id == id;
			});

			if (iter == tasks.end())
			{
				return {};
			}

			return iter->expire_date;
		});
	}

	std::vector<game::item_t> get_task_rewards(const std::uint32_t id)
	{
		return access_tasks<std::vector<game::item_t>>([&](std::vector<challenge_task_t>& tasks)
			-> std::vector<game::item_t>
		{
			const auto iter = std::ranges::find_if(tasks.begin(), tasks.end(), [&](const challenge_task_t& task)
			{
				return task.id == id;
			});

			if (iter == tasks.end())
			{
				return {};
			}

			return iter->reward_list;
		});
	}

	void challenge_task_t::to_json(json::value& data) const
	{
		data["category"] = this->category;
		data["require"] = this->require;
		data["id"] = this->id;
		data["info_id"] = this->info_id;
		data["name_id"] = this->name_id;
		data["target"] = this->target;
		data["term"] = this->term;
		data["expire_date"] = this->expire_date;

		data["is_progress"] = 0;
		data["is_new"] = 0;
		data["is_complete"] = 0;
		data["progress"] = 0;

		data["condition_list"] = json::array();
		data["extra_list"] = json::array();
		data["reward_list"] = json::array();

		for (auto i = 0ull; i < this->condition_list.size(); i++)
		{
			data["condition_list"][i]["target_code"] = this->extra_list[i].category;
			data["condition_list"][i]["required"] = this->extra_list[i].target;
			data["condition_list"][i]["target_lang"] = this->extra_list[i].target;
			data["condition_list"][i]["progress"] = 0;
		}

		for (auto i = 0ull; i < this->extra_list.size(); i++)
		{
			data["extra_list"][i]["category"] = this->extra_list[i].category;
			data["extra_list"][i]["target"] = this->extra_list[i].target;
		}

		for (auto i = 0ull; i < this->reward_list.size(); i++)
		{
			this->reward_list[i].to_json(data["reward_list"][i]);
		}
	}

	GET_FIELD_C(challenge_task_order, std::uint64_t, task_order_id);
	GET_FIELD_C(challenge_task_order, std::uint64_t, player_id);
	GET_FIELD_C(challenge_task_order, std::uint32_t, task_id);
	GET_FIELD_C(challenge_task_order, bool, is_complete);
	GET_FIELD_C(challenge_task_order, std::uint32_t, progress);
	GET_FIELD_C(challenge_task_order, std::chrono::seconds, expire_date);

	namespace impl
	{
		template <database_type_t Type>
		std::uint64_t create_order(const std::uint64_t player_id, const std::uint32_t task_id, const std::chrono::system_clock::time_point expire_date)
		{
			return database::access<std::uint64_t>([&](database::database_t& db)
			{
				return db.exec<Type>(
					sqlpp::insert_into(challenge_task_order::table)
						.set(challenge_task_order::table.f_player_id = player_id, 
							 challenge_task_order::table.task_id = task_id,
							 challenge_task_order::table.is_complete = false,
							 challenge_task_order::table.expire_date = expire_date)
				);
			});
		}

		template <database_type_t Type>
		std::optional<challenge_task_order> find_order(const std::uint64_t player_id, const std::uint32_t task_id)
		{
			return database::access<std::optional<challenge_task_order>>([&](database::database_t& db)
				-> std::optional<challenge_task_order>
			{
				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::all_of(challenge_task_order::table)).from(challenge_task_order::table)
						.where(challenge_task_order::table.f_player_id == player_id && 
							   challenge_task_order::table.task_id == task_id &&
							   challenge_task_order::table.expire_date > std::chrono::system_clock::now())
				);

				std::optional<challenge_task_order> result;

				if (results.empty())
				{
					return result;
				}

				const auto& row = results.front();
				result.emplace(row);

				return result;
			});
		}
		
		template <database_type_t Type>
		std::vector<challenge_task_order> get_all_orders(const std::uint64_t player_id)
		{
			return database::access<std::vector<challenge_task_order>>([&](database::database_t& db)
				-> std::vector<challenge_task_order>
			{
				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::all_of(challenge_task_order::table)).from(challenge_task_order::table)
						.where(challenge_task_order::table.f_player_id == player_id && 
							   challenge_task_order::table.expire_date > std::chrono::system_clock::now())
				);

				std::vector<challenge_task_order> result;

				for (const auto& row : results)
				{
					result.emplace_back(row);
				}

				return result;
			});
		}

		template <database_type_t Type>
		bool cancel_order(const std::uint64_t task_order_id)
		{
			return database::access<bool>([&](database_t& db)
			{
				const auto result = db.exec<Type>(
					sqlpp::remove_from(challenge_task_order::table)
						.where(challenge_task_order::table.task_order_id == task_order_id));
				return result != 0ull;
			});
		}
		
		template <database_type_t Type>
		bool complete_order(const std::uint64_t task_order_id)
		{
			return database::access<bool>([&](database_t& db)
			{
				const auto result = db.exec<Type>(
					sqlpp::update(challenge_task_order::table)
						.set(challenge_task_order::table.is_complete = true)
							.where(challenge_task_order::table.task_order_id == task_order_id));
				return result != 0ull;
			});
		}
				
		template <database_type_t Type>
		bool set_order_progress(const std::uint64_t task_order_id, const std::uint32_t progress)
		{
			return database::access<bool>([&](database_t& db)
			{
				const auto result = db.exec<Type>(
					sqlpp::update(challenge_task_order::table)
						.set(challenge_task_order::table.progress = progress)
							.where(challenge_task_order::table.task_order_id == task_order_id));
				return result != 0ull;
			});
		}

		template <database_type_t Type>
		void delete_player_data(const std::uint64_t player_id)
		{
			database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::remove_from(challenge_task_order::table)
						.where(challenge_task_order::table.f_player_id == player_id));
			});
		}
	}

	bool challenge_task_order::cancel() const
	{
		RUN_IMPL(impl::cancel_order, this->get_task_order_id());
	}

	bool challenge_task_order::complete() const
	{
		RUN_IMPL(impl::complete_order, this->get_task_order_id());
	}

	bool challenge_task_order::set_progress(const std::uint32_t progress) const
	{
		RUN_IMPL(impl::set_order_progress, this->get_task_order_id(), progress);
	}

	std::optional<challenge_task_order> find_order(const std::uint64_t player_id, const std::uint32_t task_id)
	{
		RUN_IMPL(impl::find_order, player_id, task_id);
	}

	std::vector<challenge_task_order> get_all_orders(const std::uint64_t player_id)
	{
		RUN_IMPL(impl::get_all_orders, player_id);
	}

	std::uint64_t create_order(const std::uint64_t player_id, const std::uint32_t task_id, const std::chrono::system_clock::time_point expire_date)
	{
		RUN_IMPL(impl::create_order, player_id, task_id, expire_date);
	}

	void dump_order_list(const std::uint64_t player_id, json::value& order_expired_list, json::value& order_list)
	{
		order_expired_list = json::array{0, 0, 0, 0, 0};
		order_list = json::array();

		const auto current_orders = database::challenge_tasks::get_all_orders(player_id);
		database::challenge_tasks::access_tasks([&](std::vector<database::challenge_tasks::challenge_task_t>& tasks)
		{
			for (auto i = 0ull; i < tasks.size(); i++)
			{
				auto& entry = order_list[i];
				tasks[i].to_json(entry);

				const auto order = std::ranges::find_if(current_orders.begin(), current_orders.end(),
					[&](const database::challenge_tasks::challenge_task_order& order)
					{
						return order.get_task_id() == tasks[i].id;
					}
				);

				if (order != current_orders.end())
				{
					entry["is_complete"] = order->get_is_complete();
					entry["is_progress"] = !order->get_is_complete();
					entry["is_new"] = 0;
					entry["progress"] = order->get_progress();
				}
			}
		});
	}

	void send_progress_list(const std::uint64_t player_id, json::value& progress_list)
	{
		if (!progress_list.is_array())
		{
			return;
		}

		for (auto i = 0ull; i < progress_list.size(); i++)
		{
			order_progress_entry_t entry{};
			if (!json::read(entry, progress_list[i]))
			{
				continue;
			}

			const auto order = find_order(player_id, entry.id);
			if (order.has_value())
			{
				order->set_progress(entry.progress);
			}
		}
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
			database.run_query("mgssd.challenge_task_orders.create");

			access_tasks([&](std::vector<challenge_task_t>& tasks)
			{
				console::print("[challenge tasks] prepared %lli tasks\n", tasks.size());
			});
		}
	};
}

REGISTER_TABLE(database::challenge_tasks::table, 2)
