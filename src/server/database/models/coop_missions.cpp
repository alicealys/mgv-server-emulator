#include <std_include.hpp>

#include "coop_missions.hpp"

#include "utils/encoding.hpp"
#include "utils/json_utils.hpp"

#include <utils/cryptography.hpp>
#include <utils/string.hpp>

namespace database::coop_missions
{
	namespace
	{
		reward_settings_t load_reward_settings()
		{
			reward_settings_t settings{};

			auto data = utils::resources::load_json(RESOURCE_COOP_MISSION_REWARD_SETTINGS);
			if (!json::read(settings, data))
			{
				throw std::runtime_error("[coop missions] failed to parse reward settings");
			}

			return settings;
		}
	}

	const reward_settings_t& get_reward_settings()
	{
		static const auto settings = load_reward_settings();
		return settings;
	}

	std::vector<reward_info_t> generate_rewards(const std::uint32_t enemy_level, const std::uint8_t rank, std::uint32_t& event_points)
	{
		std::vector<reward_info_t> list;

		const auto& settings = get_reward_settings();

		const reward_settings_t::level_t* reward_level = nullptr;
		for (auto& level : settings.levels)
		{
			if (enemy_level >= level.enemy_level)
			{
				reward_level = &level;
				break;
			}
		}

		if (reward_level == nullptr)
		{
			return list;
		}

		for (std::size_t i = rank; i < reward_level->rank_rewards.size(); i++)
		{
			auto& reward = reward_level->rank_rewards[i];
			if (reward.energy != 0)
			{
				reward_info_t info{};
				info.rank = static_cast<std::uint8_t>(i);
				info.item.category = game::ITEM_CATEGORY_ENERGY;
				info.item.num = reward.energy;
				list.emplace_back(info);
			}

			if (i == rank)
			{
				event_points = reward.event_points;
			}

			const auto reward_count = utils::cryptography::random::get_integer(reward.min_rewards, reward.max_rewards);
			if (i > settings.rank_reward_pools.size())
			{
				break;
			}

			const auto& pool = settings.rank_reward_pools[i];
			for (auto o = 0u; o < reward_count; o++)
			{
				const auto idx = utils::cryptography::random::get_integer() % pool.size();
				const auto& item = pool[idx];
				const auto num = item.max <= 1
					? 1
					: utils::cryptography::random::get_integer(1u, item.max);

				reward_info_t info{};
				info.rank = static_cast<std::uint8_t>(i);
				info.item.category = item.category;
				info.item.code = item.code;
				info.item.num = num;
				list.emplace_back(info);
			}
		}

		return list;
	}

	void coop_mission_record_t::to_json(json::value& data) const
	{
		data["clear_rank"] = this->clear_rank;
		data["iris_score"] = this->iris_score;
		data["mission_code"] = this->mission_code;
		data["personal_score"] = this->personal_score;
		data["rescue"] = this->rescue;
		data["waves"] = this->waves;
	}

	GET_FIELD_C(coop_mission_result, std::uint64_t, coop_mission_result_id);
	GET_FIELD_C(coop_mission_result, std::uint64_t, player_id);
	GET_FIELD_C(coop_mission_result, std::uint32_t, mission_code);
	GET_FIELD_C(coop_mission_result, std::uint8_t, clear_rank);
	GET_FIELD_C(coop_mission_result, std::uint32_t, score);
	GET_FIELD_C(coop_mission_result, std::uint32_t, personal_score);
	GET_FIELD_C(coop_mission_result, std::uint8_t, waves);
	GET_FIELD_C(coop_mission_result, std::uint8_t, rescue);
	GET_FIELD_C(coop_mission_result, std::chrono::seconds, create_date);

	GET_FIELD_C(coop_mission, std::uint64_t, coop_mission_id);
	GET_FIELD_C(coop_mission, std::uint64_t, owner_player_id);
	GET_FIELD_C(coop_mission, std::uint32_t, mission_code);
	GET_FIELD_C(coop_mission, std::uint8_t, flag);
	GET_FIELD_C(coop_mission, std::chrono::seconds, create_date);

	namespace impl
	{
		template <database_type_t Type>
		void add_result(const std::uint64_t player_id, const std::uint8_t mission_type, const coop_mission_record_t& params)
		{
			return database::access([&](database::database_t& db)
			{
				db.exec<Type>(
					sqlpp::insert_into(coop_mission_result::table)
						.set(coop_mission_result::table.f_player_id = player_id,
							 coop_mission_result::table.mission_type = mission_type,
							 coop_mission_result::table.mission_code = params.mission_code,
							 coop_mission_result::table.clear_rank = params.clear_rank,
							 coop_mission_result::table.waves = params.waves,
							 coop_mission_result::table.rescue = params.rescue,
							 coop_mission_result::table.score = params.iris_score,
							 coop_mission_result::table.personal_score = params.personal_score,
							 coop_mission_result::table.create_date = std::chrono::system_clock::now())
				);
			});
		}

		template <database_type_t Type>
		std::vector<coop_mission_record_t> get_record_list(const std::uint64_t player_id, const std::uint8_t mission_type)
		{
			return database::access<std::vector<coop_mission_record_t>>([&](database::database_t& db)
				-> std::vector<coop_mission_record_t>
			{
				auto results = db.exec<Type>(
					sqlpp::select(coop_mission_result::table.mission_code,
								  sqlpp::max(coop_mission_result::table.score).as(sqlpp::alias::a),
								  sqlpp::max(coop_mission_result::table.personal_score).as(sqlpp::alias::b),
								  sqlpp::min(coop_mission_result::table.clear_rank).as(sqlpp::alias::c),
								  sqlpp::max(coop_mission_result::table.rescue).as(sqlpp::alias::d),
								  sqlpp::max(coop_mission_result::table.waves).as(sqlpp::alias::e))
						.from(coop_mission_result::table)
							.where(coop_mission_result::table.f_player_id == player_id && coop_mission_result::table.mission_type == mission_type)
								.group_by(coop_mission_result::table.mission_code));

				std::vector<coop_mission_record_t> list;

				for (const auto& row : results)
				{
					coop_mission_record_t record{};
					record.mission_code = static_cast<std::uint32_t>(row.mission_code);
					record.iris_score = static_cast<std::uint32_t>(row.a);
					record.personal_score = static_cast<std::uint32_t>(row.b);
					record.clear_rank = static_cast<std::uint8_t>(row.c);
					record.waves = static_cast<std::uint8_t>(row.d);
					record.rescue = static_cast<std::uint8_t>(row.e);
					list.emplace_back(record);
				}

				return list;
			});
		}

		template <database_type_t Type>
		std::uint64_t start_mission(const std::uint64_t player_id, const std::uint32_t mission_code, const std::uint8_t flag)
		{
			return database::access<std::uint64_t>([&](database::database_t& db)
			{
				return db.exec<Type>(
					sqlpp::insert_into(coop_mission::table)
						.set(coop_mission::table.owner_player_id = player_id,
							 coop_mission::table.mission_code = mission_code,
							 coop_mission::table.flag = flag,
							 coop_mission::table.create_date = std::chrono::system_clock::now())
				);
			});
		}

		template <database_type_t Type>
		void delete_player_data(const std::uint64_t player_id)
		{
			database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::remove_from(coop_mission_result::table)
						.where(coop_mission_result::table.f_player_id == player_id));

				db.exec<Type>(
					sqlpp::remove_from(coop_mission::table)
						.where(coop_mission::table.owner_player_id == player_id));
			});
		}
	}

	void add_result(const std::uint64_t player_id, const std::uint8_t mission_type, const coop_mission_record_t& params)
	{
		RUN_IMPL(impl::add_result, player_id, mission_type, params);
	}

	std::vector<coop_mission_record_t> get_record_list(const std::uint64_t player_id, const std::uint8_t mission_type)
	{
		RUN_IMPL(impl::get_record_list, player_id, mission_type);
	}

	std::uint64_t start_mission(const std::uint64_t lobby_owner_id, const std::uint32_t mission_code, const std::uint8_t flag)
	{
		RUN_IMPL(impl::start_mission, lobby_owner_id, mission_code, flag);
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
			database.run_query("mgssd.coop_missions.create");
			database.run_query("mgssd.coop_mission_results.create");

			get_reward_settings();
		}
	};
}

REGISTER_TABLE(database::coop_missions::table, 3)
