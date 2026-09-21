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

namespace database::defense_missions
{
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

	std::uint8_t calc_rank(const std::array<std::uint32_t, 5>& thresholds, const std::uint32_t score)
	{
		const auto score_s = static_cast<std::uint32_t>(static_cast<float>(score) / 100.f);
		for (auto i = 0ull; i < thresholds.size(); i++)
		{
			if (score_s >= thresholds[i])
			{
				return static_cast<std::uint8_t>(i);
			}
		}

		return 5u;
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

	class table final : public table_interface
	{
	public:
		void create(database_t& database) override
		{
			database.run_query("mgssd.defense_missions.create");
			database.run_query("mgssd.defense_mission_waves.create");
		}
	};
}

REGISTER_TABLE(database::defense_missions::table, 2)
