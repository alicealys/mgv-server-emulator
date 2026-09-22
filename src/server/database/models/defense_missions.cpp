#include <std_include.hpp>

#include "defense_missions.hpp"

#include "utils/encoding.hpp"
#include "utils/json_utils.hpp"

#include <utils/cryptography.hpp>
#include <utils/string.hpp>

template <>
struct glz::meta<database::defense_missions::injury_crew_t>
{
	using T = database::defense_missions::injury_crew_t;
	static constexpr auto value = glz::object(
		"injury_flag", &T::injury_flag,
		"face_id", &T::face_id,
		"server_unique_id", &T::server_unique_id,
		"group_id", &T::group_id
	);
};

template <>
struct glz::meta<database::defense_missions::reward_pool_t::reward_t>
{
	using T = database::defense_missions::reward_pool_t::reward_t;
	static constexpr auto value = glz::object(
		"id", &T::id,
		"max", &T::max,
		"type", &T::type
	);
};

template <>
struct glz::meta<database::defense_missions::reward_pool_t::rank_t>
{
	using T = database::defense_missions::reward_pool_t::rank_t;
	static constexpr auto value = glz::object(
		"count", &T::count,
		"energy", &T::energy,
		"recipe_list", &T::recipe_list,
		"rewards", &T::rewards
	);
};

namespace database::defense_missions
{
	namespace
	{
		std::unordered_map<std::uint32_t, reward_pool_t> load_reward_pools()
		{
			const auto data = utils::resources::load_json(RESOURCE_BASE_DEFENSE_REWARD_POOLS);

			std::unordered_map<std::uint32_t, reward_pool_t> map;
			for (auto i = 0ull; i < data.size(); i++)
			{
				reward_pool_t pool{};

				if (!json::read(pool.mission_code, data[i]["mission_code"]))
				{
					throw std::runtime_error("failed to parse reward pool list (mission_code)");
				}

				for (auto o = 0ull; o < pool.reward_pool.size(); o++)
				{
					if (!json::read(pool.reward_pool[o], data[i]["reward_pool"][o]))
					{
						throw std::runtime_error("failed to parse reward pool list (reward_pool)");
					}

					for (auto& reward : pool.reward_pool[o].rewards)
					{
						if (reward.type == "resource")
						{
							const auto iter = game::parameters_table.ssd_sbm_parameters->resources.find(reward.id);
							if (iter == game::parameters_table.ssd_sbm_parameters->resources.end())
							{
								console::warning("[defense missions] reward has invalid resource id %lli\n", reward.id);
								reward.reward_type = reward_type_invalid;
							}
							else
							{
								reward.reward_type = reward_type_resource;
								reward.resource = iter->second;
							}
						}
						else if (reward.type == "item")
						{
							const auto iter = game::parameters_table.ssd_sbm_parameters->productions.find(reward.id);
							if (iter == game::parameters_table.ssd_sbm_parameters->productions.end())
							{
								console::warning("[defense missions] reward has invalid item id %lli\n", reward.id);
								reward.reward_type = reward_type_invalid;
							}
							else
							{
								reward.reward_type = reward_type_item;
								reward.item = iter->second;
							}
						}
					}
				}

				map.insert(std::make_pair(pool.mission_code, pool));
			}

			return map;
		}

		const std::unordered_map<std::uint32_t, reward_pool_t>& get_reward_pools()
		{
			static const auto pools = load_reward_pools();
			return pools;
		}
	}

	bool injury_crew_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	void injury_crew_t::to_json(json::value& data) const
	{
		data["crew_name"] = this->crew_name;
		data["injury_flag"] = this->injury_flag;
		data["face_id"] = this->face_id;
		data["server_unique_id"] = this->server_unique_id;
		data["group_id"] = this->group_id;
	}

	bool reward_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	void reward_t::to_json(json::value& data) const
	{
		data["category"] = this->category;
		data["code"] = this->code;
		data["param1"] = this->param1;
		data["param2"] = this->param2;
		data["param3"] = this->param3;
		data["param4"] = this->param4;
		data["param5"] = this->param5;
		data["num"] = this->num;
	}

	bool broken_facility_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	void broken_facility_t::to_json(json::value& data) const
	{
		data["product_index"] = this->product_index;
		data["num"] = this->num;
	}

	bool wave_params_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	void wave_params_t::to_json(json::value& data) const
	{
		data["attack_power_1"] = this->attack_power_1;
		data["attack_power_2"] = this->attack_power_2;
		data["attack_power_3"] = this->attack_power_3;
		data["attack_power_4"] = this->attack_power_4;
		data["attack_power_1_after"] = this->attack_power_1_after;
		data["attack_power_2_after"] = this->attack_power_2_after;
		data["attack_power_3_after"] = this->attack_power_3_after;
		data["attack_power_4_after"] = this->attack_power_4_after;
		data["defense_flag_1"] = this->defense_flag_1;
		data["defense_flag_2"] = this->defense_flag_2;
		data["defense_flag_3"] = this->defense_flag_3;
		data["defense_flag_4"] = this->defense_flag_4;
		data["defense_power_1_before"] = this->defense_power_1_before;
		data["defense_power_2_before"] = this->defense_power_2_before;
		data["defense_power_3_before"] = this->defense_power_3_before;
		data["defense_power_4_before"] = this->defense_power_4_before;
		data["defense_power_1_after"] = this->defense_power_1_after;
		data["defense_power_2_after"] = this->defense_power_2_after;
		data["defense_power_3_after"] = this->defense_power_3_after;
		data["defense_power_4_after"] = this->defense_power_4_after;
		data["water_before"] = this->water_before;
		data["water_after"] = this->water_after;
		data["food_before"] = this->food_before;
		data["food_after"] = this->food_after;
		data["medical_before"] = this->medical_before;
		data["medical_after"] = this->medical_after;
		data["mining_machine_life_before"] = this->mining_machine_life_before;
		data["mining_machine_life_after"] = this->mining_machine_life_after;
		data["mining_machine_life_max"] = this->mining_machine_life_max;
	}

	std::uint8_t calc_rank(const game::defense_mission_settings_t& mission_settings, const std::uint32_t score)
	{
		const auto score_s = static_cast<std::uint32_t>(static_cast<float>(score) / 1000.f);
		for (auto i = 0ull; i < mission_settings.rank_threshold.size(); i++)
		{
			if (score_s >= mission_settings.rank_threshold[i])
			{
				return static_cast<std::uint8_t>(i);
			}
		}

		return 5u;
	}

	std::uint32_t find_random_resource(const std::uint8_t rarity, std::uint8_t& category)
	{
		const auto find = []<typename T, typename F>(const T& list, F&& f)
		{
			const auto offset = utils::cryptography::random::get_integer() % list.size();
			for (auto i = 0ull; i < list.size(); i++)
			{
				auto index = (i + offset) % list.size();
				auto& entry = list[index];
				if (entry != nullptr && f(*entry))
				{
					return entry->id;
				}
			}

			return 0u;
		};

		const auto is_resource = utils::cryptography::random::get_integer() % 1 == 1;
		if (is_resource)
		{
			category = 0;
			return find(game::parameters_table.ssd_sbm_parameters->resources_list, [&](const game::resource_t& resource)
			{
				return resource.rarity == rarity;
			});
		}
		else
		{
			category = 1;
			return find(game::parameters_table.ssd_sbm_parameters->productions_list, [&](const game::production_t& production)
			{
				return production.rarity == rarity && production.countable;
			});
		}
	}

	std::vector<reward_info_t> generate_rewards(const game::defense_mission_settings_t& mission_settings, const std::uint8_t rank)
	{
		std::vector<reward_info_t> rewards;

		const auto& reward_pools = get_reward_pools();
		const auto iter = reward_pools.find(mission_settings.mission_id);
		if (iter == reward_pools.end())
		{
			return rewards;
		}

		// category
		// 0: resource
		// 1: stackable item
		// 9: energy

		const auto& reward_pool = iter->second;
		for (auto i = rank; i < reward_pool.reward_pool.size(); i++)
		{
			if (reward_pool.reward_pool[i].energy > 0)
			{
				reward_info_t reward_info{};
				reward_info.rank = i;
				reward_info.param.category = 9;
				reward_info.param.num = reward_pool.reward_pool[i].energy;
				rewards.emplace_back(reward_info);
			}

			for (auto o = 0ull; o < reward_pool.reward_pool[i].count; o++)
			{
				const auto& reward_list = reward_pool.reward_pool[i].rewards;
				const auto rand = utils::cryptography::random::get_integer() % reward_list.size();
				const auto& reward = reward_list[rand];

				reward_info_t reward_info{};
				reward_info.rank = i;

				auto num = reward.max;

				if (reward.max < 50 && reward.max > 5)
				{
					constexpr const auto min_percent = 80; // should it be 100?
					const auto percent = static_cast<float>(utils::cryptography::random::get_integer() %
						(100 - min_percent) + min_percent) / 100.f;
					num = static_cast<std::uint32_t>(static_cast<float>(reward.max) * percent);
				}

				switch (reward.reward_type)
				{
				case reward_type_resource:
				{
					reward_info.param.category = 0;
					reward_info.param.code = reward.id;
					reward_info.param.num = num;
					reward_info.id_index = reward.resource->index;
					break;
				}
				case reward_type_item:
				{
					reward_info.param.category = 1;
					reward_info.param.code = reward.id;
					reward_info.param.num = num;
					reward_info.id_index = reward.item->index;
					break;
				}
				default:
				case reward_type_invalid:
					continue;
				}

				rewards.emplace_back(reward_info);
			}
		}

		std::shuffle(rewards.begin(), rewards.end(), utils::cryptography::random::get_engine());

		return rewards;
	}

	GET_FIELD_C(defense_mission_wave, std::uint64_t, defense_mission_id);
	GET_FIELD_C(defense_mission_wave, std::uint64_t, defense_mission_wave_id);
	GET_FIELD_C(defense_mission_wave, std::uint8_t, wave);
	GET_FIELD_C(defense_mission_wave, std::uint32_t, total_score);
	GET_FIELD_C(defense_mission_wave, std::uint8_t, result);
	GET_FIELD_C(defense_mission_wave, std::chrono::seconds, start_date);
	GET_FIELD_C(defense_mission_wave, std::chrono::seconds, end_date);

	GET_FIELD_C(defense_mission, std::uint64_t, defense_mission_id);
	GET_FIELD_C(defense_mission, std::uint64_t, player_id);
	GET_FIELD_C(defense_mission, std::uint16_t, mission_code);
	GET_FIELD_C(defense_mission, std::uint8_t, current_wave);
	GET_FIELD_C(defense_mission, std::uint8_t, clear_rank);
	GET_FIELD_C(defense_mission, std::uint32_t, total_score);
	GET_FIELD_C(defense_mission, std::uint8_t, result);
	GET_FIELD_C(defense_mission, std::chrono::seconds, start_date);
	GET_FIELD_C(defense_mission, std::chrono::seconds, next_wave_date);
	GET_FIELD_C(defense_mission, std::chrono::seconds, end_date);

	namespace impl
	{
		template <database_type_t Type>
		std::optional<defense_mission> find(const std::uint64_t defense_mission_id)
		{
			return database::access<std::optional<defense_mission>>([&](database::database_t& db)
				-> std::optional<defense_mission>
			{
				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::all_of(defense_mission::table)).from(defense_mission::table)
						.where(defense_mission::table.defense_mission_id == defense_mission_id));

				if (results.empty())
				{
					return {};
				}

				const auto& row = results.front();
				return {defense_mission(row)};
			});
		}

		template <database_type_t Type>
		defense_mission find_or_create(const std::uint64_t player_id, const std::uint16_t mission_code, const std::uint32_t max_wave)
		{
			return database::access<defense_mission>([&](database::database_t& db)
				-> defense_mission
			{
				{
					auto results = db.exec<Type>(
						sqlpp::select(sqlpp::all_of(defense_mission::table)).from(defense_mission::table)
							.where(defense_mission::table.f_player_id == player_id &&
								   defense_mission::table.mission_code == static_cast<std::uint32_t>(mission_code) &&
								   defense_mission::table.current_wave < max_wave)
					);

					if (!results.empty())
					{
						return defense_mission(results.front());
					}
				}

				const auto id = db.exec<Type>(
					sqlpp::insert_into(defense_mission::table)
						.set(defense_mission::table.f_player_id = player_id, 
							 defense_mission::table.mission_code = mission_code, 
							 defense_mission::table.start_date = std::chrono::system_clock::now(),
							 defense_mission::table.end_date = std::chrono::system_clock::now())
				);

				const auto found = find<Type>(id);
				if (!found.has_value())
				{
					throw std::runtime_error("database::defense_missions::find_or_create: insert failed");
				}

				return found.value();
			});
		}

		template <database_type_t Type>
		std::uint64_t create_mission(const std::uint64_t player_id, const std::uint16_t mission_code)
		{
			return database::access<std::uint64_t>([&](database::database_t& db)
			{
				return db.exec<Type>(
					sqlpp::insert_into(defense_mission::table)
						.set(defense_mission::table.f_player_id = player_id, 
							 defense_mission::table.mission_code = mission_code, 
							 defense_mission::table.start_date = std::chrono::system_clock::now(),
							 defense_mission::table.end_date = std::chrono::system_clock::now(),
							 defense_mission::table.next_wave_date = std::chrono::system_clock::now(),
							 defense_mission::table.result = 0)
				);
			});
		}

		template <database_type_t Type>
		std::vector<defense_mission> get_history(const std::uint64_t player_id, const std::uint32_t limit)
		{
			return database::access<std::vector<defense_mission>>([&](database::database_t& db)
				-> std::vector<defense_mission>
			{
				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::all_of(defense_mission::table)).from(defense_mission::table)
						.where(defense_mission::table.f_player_id == player_id)
							.order_by(defense_mission::table.start_date.desc()).limit(limit));

				std::vector<defense_mission> list;

				for (auto& row : results)
				{
					list.emplace_back(defense_mission(row));
				}

				return list;
			});
		}
		
		template <database_type_t Type>
		std::optional<defense_mission> get_current_mission(const std::uint64_t player_id)
		{
			return database::access<std::optional<defense_mission>>([&](database::database_t& db)
				-> std::optional<defense_mission>
			{
				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::all_of(defense_mission::table))
						.from(defense_mission::table)
							.where(defense_mission::table.f_player_id == player_id && defense_mission::table.result == 0)
							.order_by(defense_mission::table.defense_mission_id.desc()));

				if (results.empty())
				{
					return {};
				}

				return {defense_mission(results.front())};
			});
		}

		template <database_type_t Type>
		std::vector<defense_mission_wave> get_waves(const std::uint64_t defense_mission_id)
		{
			return database::access<std::vector<defense_mission_wave>>([&](database::database_t& db)
				-> std::vector<defense_mission_wave>
			{
				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::all_of(defense_mission_wave::table)).from(defense_mission_wave::table)
						.where(defense_mission_wave::table.f_defense_mission_id == defense_mission_id)
							.order_by(defense_mission_wave::table.start_date.desc()));

				std::vector<defense_mission_wave> list;

				for (const auto& row : results)
				{
					list.emplace_back(defense_mission_wave(row));
				}

				return list;
			});
		}

		template <database_type_t Type>
		std::uint64_t add_wave(const std::uint64_t defense_mission_id,
			wave_result_t& result,
			wave_params_t& params,
			broken_facility_list_t& broken_facility_list,
			injury_crew_list_t& injury_crew_list,
			reward_list_t& reward_list)
		{
			return database::access<std::uint64_t>([&](database::database_t& db)
			{
				return db.exec<Type>(sqlpp::insert_into(defense_mission_wave::table)
					.set(defense_mission_wave::table.f_defense_mission_id = defense_mission_id,
						 defense_mission_wave::table.wave = result.wave,
						 defense_mission_wave::table.result = result.result,
						 defense_mission_wave::table.total_score = result.total_score,
						 defense_mission_wave::table.start_date = std::chrono::system_clock::time_point(std::chrono::seconds(result.start_date)),
						 defense_mission_wave::table.end_date = std::chrono::system_clock::time_point(std::chrono::seconds(result.end_date)),
						 defense_mission_wave::table.params = sqlpp::verbatim<sqlpp::binary>(utils::encoding::encode_binary(params)),
						 defense_mission_wave::table.broken_facility_list = sqlpp::verbatim<sqlpp::binary>(broken_facility_list.serialize()),
						 defense_mission_wave::table.injury_crew_list = sqlpp::verbatim<sqlpp::binary>(injury_crew_list.serialize()),
						 defense_mission_wave::table.reward_list = sqlpp::verbatim<sqlpp::binary>(reward_list.serialize())
					)
				);
			});
		}

		template <database_type_t Type>
		void update_mission(const std::uint64_t defense_mission_id, const std::uint8_t result, 
			const std::uint8_t current_wave, const std::uint8_t clear_rank, const std::uint32_t total_score, const std::chrono::seconds end_date, const std::chrono::seconds next_wave_date)
		{
			return database::access([&](database::database_t& db)
			{
				db.exec<Type>(sqlpp::update(defense_mission::table)
						.set(defense_mission::table.result = static_cast<std::uint32_t>(result),
							 defense_mission::table.current_wave = current_wave,
							 defense_mission::table.clear_rank = static_cast<std::uint32_t>(clear_rank),
							 defense_mission::table.total_score = total_score,
							 defense_mission::table.end_date = std::chrono::system_clock::time_point(end_date),
							 defense_mission::table.next_wave_date = std::chrono::system_clock::time_point(next_wave_date))
								.where(defense_mission::table.defense_mission_id == defense_mission_id && defense_mission::table.current_wave < current_wave));
			});
		}

		template <database_type_t Type>
		void delete_player_data(const std::uint64_t player_id)
		{
			database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::remove_from(defense_mission_wave::table)
						.where(defense_mission_wave::table.f_defense_mission_id.in(
							sqlpp::select(defense_mission::table.defense_mission_id).from(defense_mission::table).where(defense_mission::table.f_player_id == player_id))));

				db.exec<Type>(
					sqlpp::remove_from(defense_mission::table)
						.where(defense_mission::table.f_player_id == player_id));
			});
		}
	}

	void defense_mission_wave::to_json(json::value& data) const
	{
		data["wave"] = this->wave_ + 1;
		data["total_score"] = this->total_score_;
		data["result"] = this->result_;
		data["start_date"] = this->start_date_.count();
		data["end_date"] = this->end_date_.count();
		this->params_.to_json(data);
		this->reward_list_.to_json(data["reward_list"]);
		this->injury_crew_list_.to_json(data["injury_crew_list"]);
		this->broken_facility_list_.to_json(data["broken_facility_list"]);
	}

	void defense_mission::to_json(json::value& data) const
	{
		data["unique_id"] = this->defense_mission_id_;
		data["mission_id"] = this->mission_code_;
		data["start_date"] = this->start_date_.count();
		data["end_date"] = this->end_date_.count();
		data["total_score"] = this->total_score_;
		data["result"] = this->result_;

		// ?
		if (this->current_wave_ == 0)
		{
			data["current_wave"] = 1;
		}
		else
		{
			data["current_wave"] = this->current_wave_;
		}

		data["clear_rank"] = this->clear_rank_;
	}

	std::vector<defense_mission_wave> defense_mission::get_waves() const
	{
		RUN_IMPL(impl::get_waves, this->get_defense_mission_id());
	}

	void defense_mission::update(const std::uint8_t result, const std::uint8_t current_wave, 
		const std::uint8_t clear_rank, const std::uint32_t total_score, const std::chrono::seconds end_date, const std::chrono::seconds next_wave_date) const
	{
		RUN_IMPL(impl::update_mission, this->get_defense_mission_id(), result, current_wave, clear_rank, total_score, end_date, next_wave_date);
	}

	std::optional<defense_mission> find(const std::uint64_t defense_mission_id)
	{
		RUN_IMPL(impl::find, defense_mission_id);
	}

	std::vector<defense_mission> get_history(const std::uint64_t player_id, const std::uint32_t limit)
	{
		RUN_IMPL(impl::get_history, player_id, limit);
	}

	std::optional<defense_mission> get_current_mission(const std::uint64_t player_id)
	{
		RUN_IMPL(impl::get_current_mission, player_id);
	}

	std::vector<defense_mission_wave> get_waves(const std::uint64_t defense_mission_id)
	{
		RUN_IMPL(impl::get_waves, defense_mission_id);
	}

	defense_mission find_or_create(const std::uint64_t player_id, const std::uint16_t mission_code, const std::uint32_t max_wave)
	{
		RUN_IMPL(impl::find_or_create, player_id, mission_code, max_wave);
	}

	std::uint64_t create_mission(const std::uint64_t player_id, const std::uint16_t mission_code)
	{
		RUN_IMPL(impl::create_mission, player_id, mission_code);
	}

	std::uint64_t add_wave(const std::uint64_t defense_mission_id, wave_result_t& result, wave_params_t& params,
		broken_facility_list_t& broken_facility_list, injury_crew_list_t& injury_crew_list, reward_list_t& reward_list)
	{
		RUN_IMPL(impl::add_wave, defense_mission_id, result, params, broken_facility_list, injury_crew_list, reward_list);
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
			load_reward_pools();
			database.run_query("mgssd.defense_missions.create");
			database.run_query("mgssd.defense_mission_waves.create");
		}
	};
}

REGISTER_TABLE(database::defense_missions::table, 2)
