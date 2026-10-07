#include <std_include.hpp>

#include "events.hpp"
#include "rankings.hpp"
#include "../auth.hpp"

#include "utils/time.hpp"
#include "platforms/ssdstm/endpoints/main/commands/cmd_information_get_title.hpp"

#include <utils/cryptography.hpp>
#include <utils/string.hpp>

namespace database::events
{
	utils::concurrency::container<std::optional<running_event>> cached_event;

	namespace
	{
		bool info_message_visible_cb(const emulator::ssd::cmd_information_get_title::message_t& message)
		{
			return cached_event.access<bool>([&](std::optional<running_event>& event)
			{
				if (!event.has_value())
				{
					return false;
				}

				return event->params->info_id == message.info_id;
			});
		}

		event_settings_t load_event_settings()
		{
			auto data = utils::resources::load_json(RESOURCE_EVENT_SETTINGS);
			event_settings_t settings{};

			if (!json::read(settings, data))
			{
				throw std::runtime_error("[events] failed to parse settings");
			}

			auto info_id_begin = 700;

			for (auto& event : settings.event_list)
			{
				emulator::ssd::cmd_information_get_title::message_t message{};
				message.title = event->info_title;
				message.text = event->info_text;
				message.info_id = info_id_begin++;
				message.visible = info_message_visible_cb;
				message.type = 2;
				event->info_id = message.info_id;
				emulator::ssd::cmd_information_get_title::messages.emplace_back(message);

				if (settings.event_type_map.contains(event->name))
				{
					throw std::runtime_error(std::format("[events] duplicate event type name \"{}\"", event->name));
				}

				settings.event_type_map.insert(std::make_pair(event->name, event));

				const auto initialize_reward_ids = [&]<typename T>(std::vector<T>& reward_list)
				{
					for (auto i = 0ull; i < reward_list.size(); i++)
					{
						if (reward_list[i].reward_id == 0u)
						{
							reward_list[i].reward_id = i + 1;
						}

						const auto iter = std::ranges::find_if(reward_list.begin(), reward_list.end(), [&](T& reward)
						{
							return reward.reward_id == reward_list[i].reward_id && &reward != &reward_list[i];
						});

						if (iter != reward_list.end())
						{
							throw std::runtime_error(std::format("[events] duplicate reward id {}", reward_list[i].reward_id));
						}
					}
				};

				initialize_reward_ids(event->reward_border_list);
				initialize_reward_ids(event->reward_catalog_list);
				initialize_reward_ids(event->reward_ranking_list);
			}

			for (const auto& name : settings.event_rotation)
			{
				const auto iter = std::ranges::find_if(settings.event_list.begin(), settings.event_list.end(),
					[&](const std::shared_ptr<event_params_t>& params)
					{
						return params->name == name;
					}
				);

				if (iter == settings.event_list.end())
				{
					throw std::runtime_error(std::format("[events] unknown event \"{}\" in rotation", name));
				}

				settings.event_rotation_linked.emplace_back(*iter);
			}

			emulator::ssd::cmd_information_get_title::register_message_var("event_start", [](const std::uint8_t lang)
				-> std::string
			{
				const auto current_event = get_current_event();
				if (!current_event.has_value())
				{
					return {};
				}

				const auto start = cached_event.access<std::chrono::seconds>([&](std::optional<running_event>& event)
				{
					if (!event.has_value())
					{
						return std::chrono::seconds(0u);
					}

					return event->get_start_date();
				});

				return date::format("%Y/%m/%d %H:%M", std::chrono::system_clock::time_point(start));
			});

			emulator::ssd::cmd_information_get_title::register_message_var("event_end", [](const std::uint8_t lang)
				-> std::string
			{
				const auto current_event = get_current_event();
				if (!current_event.has_value())
				{
					return {};
				}

				const auto end = cached_event.access<std::chrono::seconds>([&](std::optional<running_event>& event)
				{
					if (!event.has_value())
					{
						return std::chrono::seconds(0u);
					}

					return event->get_end_date();
				});

				return date::format("%Y/%m/%d %H:%M", std::chrono::system_clock::time_point(end));
			});

			return settings;
		}
	}

	void event_params_t::to_json(json::value& data) const
	{
		data["current_event_point"] = 0;
		data["total_event_point"] = 0;
		data["disp_rank"] = 0;
		data["rank"] = 0;

		data["reward_end_date"] = 0;
		data["reward_start_date"] = 0;
		data["start_date"] = 0;
		data["end_date"] = 0;
		data["event_id"] = 0;

		data["description_id"] = 326972843;
		data["enable_border"] = this->enable_border;
		data["enable_catalog"] = this->enable_catalog;
		data["enable_ranking"] = this->enable_ranking;
		data["event_name"] = "";
		data["event_name_id"] = 326972843;
		data["info_id"] = this->info_id;

		data["reward_border_list"] = json::array();
		data["reward_catalog_list"] = json::array();
		data["reward_ranking_list"] = json::array();

		for (auto i = 0ull; i < this->reward_border_list.size(); i++)
		{
			auto& entry = data["reward_border_list"][i];
			entry["border_point"] = this->reward_border_list[i].border_point;
			this->reward_border_list[i].item_info.to_json(entry["item_info"]);
		}

		for (auto i = 0ull; i < this->reward_catalog_list.size(); i++)
		{
			auto& entry = data["reward_catalog_list"][i];
			entry["catalog_id"] = this->reward_border_list[i].reward_id;
			entry["price_point"] = this->reward_catalog_list[i].price_point;
			entry["purchase_limit"] = this->reward_catalog_list[i].purchase_limit;
			entry["purchased_num"] = 0;
			this->reward_catalog_list[i].item_info.to_json(entry["item_info"]);
		}

		for (auto i = 0ull; i < this->reward_ranking_list.size(); i++)
		{
			auto& entry = data["reward_ranking_list"][i];
			entry["start_rank"] = this->reward_ranking_list[i].start_rank;
			entry["end_rank"] = this->reward_ranking_list[i].end_rank;

			entry["item_info"] = json::array();
			for (auto o = 0ull; o < this->reward_ranking_list[i].item_info.size(); o++)
			{
				this->reward_ranking_list[i].item_info[o].to_json(entry["item_info"][o]);
			}
		}
	}

	const event_settings_t& get_event_settings()
	{
		static const auto settings = load_event_settings();
		return settings;
	}

	void get_event_range(std::chrono::seconds& start, std::chrono::seconds& end)
	{
		const auto& settings = get_event_settings();
		utils::time::get_section(settings.event_duration * 24h, start, end);
	}

	std::optional<running_event_info_t> get_target_event()
	{
		const auto& settings = get_event_settings();
		if (settings.event_rotation_linked.empty())
		{
			return {};
		}

		running_event_info_t event{};

		const auto event_number = utils::time::get_day() / settings.event_duration;
		const auto event_index = event_number % settings.event_rotation_linked.size();
		event.params = settings.event_rotation_linked[event_index];
		get_event_range(event.start, event.end);

		return {event};
	}

	std::shared_ptr<event_params_t> get_event_params(const std::string& event_type)
	{
		const auto& settings = get_event_settings();
		const auto iter = settings.event_type_map.find(event_type);
		if (iter == settings.event_type_map.end())
		{
			return nullptr;
		}

		return iter->second;
	}

	GET_FIELD_C(running_event, std::uint64_t, event_id);
	GET_FIELD_C(running_event, std::string, event_type);
	GET_FIELD_C(running_event, std::uint32_t, event_state);
	GET_FIELD_C(running_event, std::chrono::seconds, start_date);
	GET_FIELD_C(running_event, std::chrono::seconds, end_date);

	GET_FIELD_C(event_reward, std::uint64_t, event_reward_id);
	GET_FIELD_C(event_reward, std::uint64_t, event_id);
	GET_FIELD_C(event_reward, std::string, event_type);
	GET_FIELD_C(event_reward, std::uint64_t, user_id);
	GET_FIELD_C(event_reward, std::uint8_t, reward_type);
	GET_FIELD_C(event_reward, std::uint64_t, reward_id);
	GET_FIELD_C(event_reward, bool, acquired);
	GET_FIELD_C(event_reward, std::chrono::seconds, create_date);

	namespace impl
	{
		template <database_type_t Type>
		std::uint64_t create_event(database_t& db, const std::string& type,
			const std::chrono::system_clock::time_point& start_date, const std::chrono::system_clock::time_point& end_date)
		{
			auto result = db.exec<Type>(
				sqlpp::insert_into(running_event::table)
					.set(running_event::table.start_date = start_date,
						 running_event::table.end_date = end_date,
						 running_event::table.event_type = type));
			return result;
		}

		template <database_type_t Type>
		void set_event_state(database_t& db, const std::uint64_t event_id, const std::uint32_t state)
		{
			db.exec<Type>(
				sqlpp::update(running_event::table)
					.set(running_event::table.event_state = state)
						.where(running_event::table.event_id == event_id));
		}

		template <database_type_t Type>
		std::optional<running_event> get_current_event1(database_t& db)
		{
			auto results = db.exec<Type>(
				sqlpp::select(sqlpp::all_of(running_event::table))
					.from(running_event::table)
						.where(running_event::table.event_state != static_cast<std::uint32_t>(event_state_dead)).limit(1u));


			std::optional<running_event> event;

			if (results.empty())
			{
				return event;
			}

			event.emplace(results.front());
			return event;
		}

		template <database_type_t Type>
		std::optional<running_event> get_current_event2()
		{
			return database::access<std::optional<running_event>>([&](database_t& db)
				-> std::optional<running_event>
			{
				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::all_of(running_event::table))
						.from(running_event::table)
							.where(running_event::table.event_state != static_cast<std::uint32_t>(event_state_dead)).limit(1u));

				std::optional<running_event> event;

				if (results.empty())
				{
					return event;
				}

				event.emplace(results.front());
				return event;
			});
		}

		template <database_type_t Type>
		bool add_reward(const std::uint64_t event_id, const std::uint64_t user_id, const std::uint8_t reward_type, const std::uint64_t reward_id, const bool acquired)
		{
			return database::access<bool>([&](database_t& db)
				-> bool
			{
				const auto result = db.exec<Type>(
					sqlpp::insert_into(event_reward::table)
						.set(event_reward::table.f_event_id = event_id,
							 event_reward::table.f_user_id = user_id,
							 event_reward::table.reward_type = reward_type,
							 event_reward::table.reward_id = reward_id,
							 event_reward::table.acquired = acquired,
							 event_reward::table.create_date = std::chrono::system_clock::now()
						));
				return result != 0ull;
			});
		}
		
		template <database_type_t Type>
		std::vector<event_reward> get_acquired_rewards(const std::uint64_t event_id, const std::uint64_t user_id)
		{
			return database::access<std::vector<event_reward>>([&](database_t& db)
				-> std::vector<event_reward>
			{
				const auto joined_tables = event_reward::table.join(running_event::table)
					.on(event_reward::table.f_event_id == running_event::table.event_id);

				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::all_of(event_reward::table), running_event::table.event_type)
						.from(joined_tables)
							.where(event_reward::table.f_event_id == event_id && 
								   event_reward::table.acquired &&
								   event_reward::table.f_user_id == user_id)
				);

				std::vector<event_reward> list;

				for (auto& row : results)
				{
					list.emplace_back(row);
				}

				return list;
			});
		}

		template <database_type_t Type>
		std::vector<event_reward> get_entitlements(const std::uint64_t current_event_id, const std::uint64_t user_id)
		{
			return database::access<std::vector<event_reward>>([&](database_t& db)
				-> std::vector<event_reward>
			{
				const auto joined_tables = event_reward::table.join(running_event::table)
					.on(event_reward::table.f_event_id == running_event::table.event_id);

				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::all_of(event_reward::table), running_event::table.event_type)
						.from(joined_tables)
							.where(!event_reward::table.acquired && 
								   event_reward::table.f_event_id != current_event_id &&
								   event_reward::table.f_user_id == user_id)
				);


				std::vector<event_reward> list;

				for (auto& row : results)
				{
					list.emplace_back(row);
				}

				return list;
			});
		}

		template <database_type_t Type>
		void clear_entitlements(const std::uint64_t current_event_id, const std::uint64_t user_id)
		{
			return database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(event_reward::table)
						.set(event_reward::table.acquired = true)
							.where(event_reward::table.f_user_id == user_id && event_reward::table.f_event_id != current_event_id));
			});
		}
	}

	bool running_event::initialize_params()
	{
		const auto& settings = get_event_settings();
		const auto iter = settings.event_type_map.find(this->event_type_);
		if (iter == settings.event_type_map.end())
		{
			return false;
		}

		this->params = iter->second;
		return true;
	}

	std::optional<running_event> get_current_event(database_t& db)
	{
		RUN_IMPL(impl::get_current_event1, db);
	}

	void set_event_state(database_t& db, const std::uint64_t event_id, const std::uint32_t state)
	{
		RUN_IMPL(impl::set_event_state, db, event_id, state);
	}

	std::optional<running_event> get_current_event()
	{
		auto event = []()
		{
			RUN_IMPL(impl::get_current_event2);
		}();

		if (event.has_value() && !event->initialize_params())
		{
			return {};
		}

		return event;
	}

	std::uint64_t create_event(database_t& db, const std::string& type, const std::chrono::system_clock::time_point& start_date, const std::chrono::system_clock::time_point& end_date)
	{
		RUN_IMPL(impl::create_event, db, type, start_date, end_date);
	}

	void create_target_event(database_t& db)
	{
		const auto target_event = get_target_event();
		if (!target_event.has_value())
		{
			return;
		}

		create_event(db, target_event->params->name,
			std::chrono::system_clock::time_point(target_event->start),
			std::chrono::system_clock::time_point(target_event->end));

		console::print("[events] creating event instance for event type \"%s\"\n", target_event->params->name.data());
	}

	bool update_event_entitlements(database_t& db, const running_event& event)
	{
		if (!event.params->enable_ranking)
		{
			return false;
		}

		const auto entries = rankings::get_entries_with_state(database::rankings::ranking_type_event, ranking_state_none, 128u);
		for (const auto& entry : entries)
		{
			for (const auto& reward : event.params->reward_ranking_list)
			{
				if (entry.get_rank() > reward.end_rank || entry.get_rank() < reward.start_rank)
				{
					continue;
				}

				add_reward(event.get_event_id(), entry.get_user_id(), reward_type_ranking, reward.reward_id, false);
			}

			rankings::set_ranking_state(entry.get_ranking_id(), ranking_state_ack);
		}

		return !entries.empty();
	}

	void update_event(database_t& db)
	{
		auto current_event = get_current_event(db);
		if (!current_event.has_value() || !current_event->initialize_params())
		{
			cached_event.access([&](std::optional<running_event>& info)
			{
				info.reset();
			});

			create_target_event(db);
			return;
		}

		cached_event.access([&](std::optional<running_event>& info)
		{
			if (!info.has_value() || info->get_event_id() != current_event->get_event_id())
			{
				info = current_event.value();
				const auto type = current_event->get_event_type();
				console::print("[events] set current event: \"%s\"\n", type.data());
			}
		});

		static std::chrono::system_clock::time_point last_ranking_update;
		const auto now = std::chrono::system_clock::now();
		const auto now_s = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());

		switch (current_event->get_event_state())
		{
		case event_state_none:
		{
			users::reset_event_points();
			rankings::reset_points(rankings::ranking_type_event);
			set_event_state(db, current_event->get_event_id(), event_state_running);
			console::print("[events] event state changed: RUNNING\n");
			break;
		}
		case event_state_running:
		{
			if (now_s > current_event->get_end_date())
			{
				set_event_state(db, current_event->get_event_id(), event_state_finished);
				console::print("[events] event state changed: RUNNING\n");
				break;
			}

			const auto time_left = current_event->get_end_date() - now_s;
			if (time_left < 8h)
			{
				set_event_state(db, current_event->get_event_id(), event_state_finished);
				console::print("[events] event state changed: FINISHED\n");
				break;
			}
			else
			{
				if (current_event->params->enable_ranking && now - last_ranking_update > 10min)
				{
					last_ranking_update = now;
					rankings::update_rankings(rankings::ranking_type_event);
				}
			}
		}
		case event_state_finished:
		{
			if (!update_event_entitlements(db, current_event.value()) && now_s >= current_event->get_end_date())
			{
				set_event_state(db, current_event->get_event_id(), event_state_dead);
				console::print("[events] event state changed: DEAD\n");
			}
		}
		case event_state_dead:
		{
			break;
		}
		}
	}

	bool add_reward(const std::uint64_t event_id, const std::uint64_t user_id, const std::uint8_t reward_type, const std::uint64_t reward_id, const bool acquired)
	{
		RUN_IMPL(impl::add_reward, event_id, user_id, reward_type, reward_id, acquired);
	}

	std::vector<event_reward> get_acquired_rewards(const std::uint64_t event_id, const std::uint64_t user_id)
	{
		RUN_IMPL(impl::get_acquired_rewards, event_id, user_id);
	}

	std::vector<event_reward> get_entitlements(const std::uint64_t current_event_id, const std::uint64_t user_id)
	{
		RUN_IMPL(impl::get_entitlements, current_event_id, user_id);
	}

	void clear_entitlements(const std::uint64_t current_event_id, const std::uint64_t user_id)
	{
		RUN_IMPL(impl::clear_entitlements, current_event_id, user_id);
	}

	class table final : public table_interface
	{
	public:
		void create(database_t& database) override
		{
			database.run_query("mgssd.running_events.create");
			database.run_query("mgssd.event_rewards.create");

			get_event_settings();
		}

		void run_tasks(database_t& database) override
		{
			update_event(database);
		}
	};
}

REGISTER_TABLE(database::events::table, 3)
