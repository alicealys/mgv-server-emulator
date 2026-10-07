#include <std_include.hpp>

#include "events.hpp"
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
				settings.event_type_map.insert(std::make_pair(event->name, event));
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

		for (auto i = 0ull; i < this->reward_border_list.size(); i++)
		{
			auto& entry = data["reward_ranking_list"][i];
			entry["border_point"] = this->reward_border_list[i].border_point;
			this->reward_border_list[i].item_info.to_json(entry["item_info"]);
		}

		for (auto i = 0ull; i < this->reward_catalog_list.size(); i++)
		{
			auto& entry = data["reward_catalog_list"][i];
			entry["catalog_id"] = this->reward_catalog_list[i].catalog_id;
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
			this->reward_ranking_list[i].item_info.to_json(entry["item_info"]);
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

	GET_FIELD_C(running_event, std::uint64_t, event_id);
	GET_FIELD_C(running_event, std::string, event_type);
	GET_FIELD_C(running_event, std::chrono::seconds, start_date);
	GET_FIELD_C(running_event, std::chrono::seconds, end_date);

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
		std::optional<running_event> get_current_event1(database_t& db)
		{
			auto results = db.exec<Type>(
				sqlpp::select(sqlpp::all_of(running_event::table))
					.from(running_event::table)
						.where(running_event::table.start_date <= std::chrono::system_clock::now() &&
							   running_event::table.end_date > std::chrono::system_clock::now()).limit(1u));


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
							.where(running_event::table.start_date <= std::chrono::system_clock::now() &&
								   running_event::table.end_date > std::chrono::system_clock::now()).limit(1u));


				std::optional<running_event> event;

				if (results.empty())
				{
					return event;
				}

				event.emplace(results.front());
				return event;
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

		users::reset_event_points();
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
	}

	class table final : public table_interface
	{
	public:
		void create(database_t& database) override
		{
			database.run_query("mgssd.running_events.create");

			get_event_settings();
		}

		void run_tasks(database_t& database) override
		{
			update_event(database);
		}
	};
}

REGISTER_TABLE(database::events::table, 3)
