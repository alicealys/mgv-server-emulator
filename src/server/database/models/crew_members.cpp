#include <std_include.hpp>

#include "crew_members.hpp"

#include "utils/encoding.hpp"
#include "utils/json_utils.hpp"
#include "utils/random.hpp"

#include <utils/cryptography.hpp>
#include <utils/string.hpp>

namespace database::crew_members
{
	GET_FIELD_C(crew_member, std::uint64_t, member_id);
	GET_FIELD_C(crew_member, std::uint64_t, player_id);
	GET_FIELD_C(crew_member, std::uint32_t, member_type);
	GET_FIELD_C(crew_member, std::uint32_t, face_id);
	GET_FIELD_C(crew_member, std::uint32_t, body_id);
	GET_FIELD_C(crew_member, std::uint32_t, race_id);
	GET_FIELD_C(crew_member, std::uint32_t, sex_id);
	GET_FIELD_C(crew_member, std::uint32_t, voice_type);
	GET_FIELD_C(crew_member, std::uint32_t, previous_group);
	GET_FIELD_C(crew_member, std::uint32_t, current_group);
	GET_FIELD_C(crew_member, std::uint32_t, previous_job);
	GET_FIELD_C(crew_member, std::uint32_t, item1_count);
	GET_FIELD_C(crew_member, std::uint32_t, item2_count);
	GET_FIELD_C(crew_member, std::uint32_t, item3_count);
	GET_FIELD_C(crew_member, std::uint32_t, item4_count);
	GET_FIELD_C(crew_member, std::uint32_t, item5_count);
	GET_FIELD_C(crew_member, std::uint32_t, item6_count);
	GET_FIELD_C(crew_member, std::uint32_t, life);
	GET_FIELD_C(crew_member, std::uint32_t, life_max);
	GET_FIELD_C(crew_member, std::uint32_t, condition);
	GET_FIELD_C(crew_member, std::uint32_t, health_flag);
	GET_FIELD_C(crew_member, std::uint32_t, map_location);
	GET_FIELD_C(crew_member, std::uint32_t, sanity);
	GET_FIELD_C(crew_member, std::uint32_t, skill);
	GET_FIELD_C(crew_member, std::uint32_t, survival_days);
	GET_FIELD_C(crew_member, std::uint32_t, injury_id_1);
	GET_FIELD_C(crew_member, std::uint32_t, injury_id_2);
	GET_FIELD_C(crew_member, std::uint32_t, injury_time_1);
	GET_FIELD_C(crew_member, std::uint32_t, injury_time_2);
	GET_FIELD_C(crew_member, std::uint32_t, sickness_id_1);
	GET_FIELD_C(crew_member, std::uint32_t, sickness_id_2);
	GET_FIELD_C(crew_member, std::uint32_t, sickness_time_1);
	GET_FIELD_C(crew_member, std::uint32_t, sickness_time_2);
	GET_FIELD_C(crew_member, std::uint32_t, ability_accessory);
	GET_FIELD_C(crew_member, std::uint32_t, ability_animal);
	GET_FIELD_C(crew_member, std::uint32_t, ability_gadget);
	GET_FIELD_C(crew_member, std::uint32_t, ability_defense_unit);
	GET_FIELD_C(crew_member, std::uint32_t, ability_base_defense);
	GET_FIELD_C(crew_member, std::uint32_t, ability_develop);
	GET_FIELD_C(crew_member, std::uint32_t, ability_expedition);
	GET_FIELD_C(crew_member, std::uint32_t, ability_food);
	GET_FIELD_C(crew_member, std::uint32_t, ability_medical);
	GET_FIELD_C(crew_member, std::uint32_t, ability_plant);
	GET_FIELD_C(crew_member, std::uint32_t, resistance_food_shortage);
	GET_FIELD_C(crew_member, std::uint32_t, resistance_sleepless);
	GET_FIELD_C(crew_member, std::uint32_t, resistance_water_shortage);
	GET_FIELD_C(crew_member, std::uint32_t, first_name_index);
	GET_FIELD_C(crew_member, std::uint32_t, last_name_index);
	GET_FIELD_C(crew_member, std::string, nickname);
	GET_FIELD_C(crew_member, std::chrono::seconds, creation_date);

	bool member_params_t::parse(json::value& data)
	{
		utils::json_utils::get_or(data["condition"], this->condition);
		utils::json_utils::get_or(data["current_group"], this->current_group);
		utils::json_utils::get_or(data["first_name_index"], this->first_name_index);
		utils::json_utils::get_or(data["health_flag"], this->health_flag);
		utils::json_utils::get_or(data["initial_max_life"], this->initial_max_life);
		utils::json_utils::get_or(data["injury_id_1"], this->injury_id_1);
		utils::json_utils::get_or(data["injury_id_2"], this->injury_id_2);
		utils::json_utils::get_or(data["injury_time_1"], this->injury_time_1);
		utils::json_utils::get_or(data["injury_time_2"], this->injury_time_2);
		utils::json_utils::get_or(data["item1_count"], this->item1_count);
		utils::json_utils::get_or(data["item2_count"], this->item2_count);
		utils::json_utils::get_or(data["item3_count"], this->item3_count);
		utils::json_utils::get_or(data["item4_count"], this->item4_count);
		utils::json_utils::get_or(data["item5_count"], this->item5_count);
		utils::json_utils::get_or(data["item6_count"], this->item6_count);
		utils::json_utils::get_or(data["last_name_index"], this->last_name_index);
		utils::json_utils::get_or(data["life"], this->life);
		utils::json_utils::get_or(data["map_location"], this->map_location);
		utils::json_utils::get_or(data["max_life"], this->max_life);
		utils::json_utils::get_or(data["previous_group"], this->previous_group);
		utils::json_utils::get_or(data["previous_job"], this->previous_job);
		utils::json_utils::get_or(data["race_id"], this->race_id);
		utils::json_utils::get_or(data["resistance_food_shortage"], this->resistance_food_shortage);
		utils::json_utils::get_or(data["resistance_sleepless"], this->resistance_sleepless);
		utils::json_utils::get_or(data["resistance_water_shortage"], this->resistance_water_shortage);
		utils::json_utils::get_or(data["sanity"], this->sanity);
		utils::json_utils::get_or(data["sickness_id_1"], this->sickness_id_1);
		utils::json_utils::get_or(data["sickness_id_2"], this->sickness_id_2);
		utils::json_utils::get_or(data["sickness_time_1"], this->sickness_time_1);
		utils::json_utils::get_or(data["sickness_time_2"], this->sickness_time_2);
		utils::json_utils::get_or(data["skill"], this->skill);
		utils::json_utils::get_or(data["survival_days"], this->survival_days);
		utils::json_utils::get_or(data["voice_type"], this->voice_type);
		utils::json_utils::get_or(data["body_id"], this->body_id);
		utils::json_utils::get_or(data["face_id"], this->face_id);
		utils::json_utils::get_or(data["generation_date"], this->generation_date);
		//utils::json_utils::get_or(data["unique_id"], this->unique_id);
		utils::json_utils::get_or(data["sex_id"], this->sex_id);
		utils::json_utils::get_or(data["unique_index"], this->unique_index);
		utils::json_utils::parse_array(data["motivation_history_event"], this->motivation_history.event);
		utils::json_utils::parse_array(data["motivation_history_value"], this->motivation_history.value);
		utils::json_utils::parse_string(data["nickname"], this->nickname);
		return true;
	}

	void member_params_t::apply_update(const member_params_t& diff)
	{
		this->condition = diff.condition;
		this->health_flag = diff.health_flag;
		this->injury_id_1 = diff.injury_id_1;
		this->injury_id_2 = diff.injury_id_2;
		this->injury_time_1 = diff.injury_time_1;
		this->injury_time_2 = diff.injury_time_2;
		this->life = diff.life;
		this->max_life = diff.max_life;
		this->previous_group = diff.previous_group;
		this->sanity = diff.sanity;
		this->sickness_id_1 = diff.sickness_id_1;
		this->sickness_id_2 = diff.sickness_id_2;
		this->sickness_time_1 = diff.sickness_time_1;
		this->sickness_time_2 = diff.sickness_time_2;
		this->survival_days = diff.survival_days;
		std::memcpy(this->motivation_history.event, diff.motivation_history.event, sizeof(this->motivation_history.event));
		std::memcpy(this->motivation_history.value, diff.motivation_history.value, sizeof(this->motivation_history.value));
	}

	bool member_params_t::parse_update(json::value& data)
	{
		utils::json_utils::get_or(data["condition"], this->condition);
		utils::json_utils::get_or(data["health_flag"], this->health_flag);
		utils::json_utils::get_or(data["injury_id_1"], this->injury_id_1);
		utils::json_utils::get_or(data["injury_id_2"], this->injury_id_2);
		utils::json_utils::get_or(data["injury_time_1"], this->injury_time_1);
		utils::json_utils::get_or(data["injury_time_2"], this->injury_time_2);
		utils::json_utils::get_or(data["life"], this->life, this->life);
		utils::json_utils::get_or(data["max_life"], this->max_life, this->life);
		utils::json_utils::get_or(data["previous_group"], this->previous_group);
		utils::json_utils::get_or(data["sanity"], this->sanity);
		utils::json_utils::get_or(data["sickness_id_1"], this->sickness_id_1);
		utils::json_utils::get_or(data["sickness_id_2"], this->sickness_id_2);
		utils::json_utils::get_or(data["sickness_time_1"], this->sickness_time_1);
		utils::json_utils::get_or(data["sickness_time_2"], this->sickness_time_2);
		utils::json_utils::get_or(data["survival_days"], this->survival_days);
		utils::json_utils::parse_array(data["motivation_history_event"], this->motivation_history.event);
		utils::json_utils::parse_array(data["motivation_history_value"], this->motivation_history.value);
		return true;
	}

	bool member_params_t::parse_add_param(json::value& data)
	{
		utils::json_utils::get_or(data["generation_date"], this->generation_date);
		utils::json_utils::get_or(data["item1_count"], this->item1_count);
		utils::json_utils::get_or(data["item2_count"], this->item2_count);
		utils::json_utils::get_or(data["item3_count"], this->item3_count);
		utils::json_utils::get_or(data["item4_count"], this->item4_count);
		utils::json_utils::get_or(data["item5_count"], this->item5_count);
		utils::json_utils::get_or(data["item6_count"], this->item6_count);
		utils::json_utils::get_or(data["map_location"], this->map_location);
		utils::json_utils::get_or(data["voice_type"], this->voice_type);

		utils::json_utils::get_or(data["face_id"], this->face_id);
		utils::json_utils::get_or(data["body_id"], this->body_id);
		utils::json_utils::get_or(data["race_id"], this->race_id);
		utils::json_utils::get_or(data["sex_id"], this->sex_id);

		if (this->map_location > 1)
		{
			return false;
		}

		std::uint32_t crew_type_code{};
		std::uint32_t unique_type_code{};
		std::uint32_t unique_index1{};

		utils::json_utils::get_or(data["crew_type_code"], crew_type_code);
		utils::json_utils::get_or(data["unique_type_code"], unique_type_code);
		utils::json_utils::get_or(data["unique_index"], unique_index1);

		if (crew_type_code != 0u)
		{
			const auto iter = game::parameters_table.ssd_crew_generator_table->type_table.find(crew_type_code);
			if (iter == game::parameters_table.ssd_crew_generator_table->type_table.end())
			{
				return false;
			}

			this->first_name_index = static_cast<std::uint16_t>(utils::random::get_integer(0, 20u));
			this->last_name_index = static_cast<std::uint16_t>(utils::random::get_integer(0, 20u));

			const auto roll_skill_value = [&](const std::uint32_t rank_low_h, const std::uint32_t rank_high_h, const std::uint32_t type)
				-> std::uint16_t
			{
				const auto rank_low = game::rank_hash_to_value(rank_low_h);
				const auto rank_high = game::rank_hash_to_value(rank_high_h);
				const auto rank = utils::random::get_integer(rank_high, rank_low);
				const auto rank_h = game::rank_value_to_hash(rank);

				const auto skill = game::parameters_table.ssd_crew_generator_table->find_skill(type, rank_h);
				if (skill == nullptr)
				{
					return 0u;
				}

				if (skill->low >= skill->high)
				{
					return static_cast<std::uint16_t>(skill->low);
				}

				return static_cast<std::uint16_t>(utils::random::get_integer(skill->low, skill->high));
			};

			auto& crew_type = *iter->second;

			this->ability_accessory = 0;
			this->ability_animal = 0;
			this->ability_defense_unit = 0;
			this->ability_gadget = 0;

			this->ability_base_defense = roll_skill_value(crew_type.base_defense_low, crew_type.base_defense_high, game::BASE_DEFENSE);
			this->ability_develop = roll_skill_value(crew_type.develop_low, crew_type.develop_high, game::DEVELOP);
			this->ability_expedition = roll_skill_value(crew_type.combat_deploy_low, crew_type.combat_deploy_high, game::COMBAT_DEPLOY);
			this->ability_food = roll_skill_value(crew_type.food_low, crew_type.food_high, game::FOOD);
			this->ability_medical = roll_skill_value(crew_type.medic_low, crew_type.medic_high, game::MEDIC);
			this->ability_plant = roll_skill_value(crew_type.farm_low, crew_type.farm_high, game::FARM);

			this->resistance_food_shortage = roll_skill_value(crew_type.hunger_resist_low, crew_type.hunger_resist_high, game::HUNGER_RESIST);
			this->resistance_sleepless = roll_skill_value(crew_type.sleeplack_resist_low, crew_type.sleeplack_resist_high, game::SLEEPLACK_RESIST);
			this->resistance_water_shortage = roll_skill_value(crew_type.thirst_resist_low, crew_type.thirst_resist_high, game::THIRST_RESIST);

			const auto init_life = roll_skill_value(crew_type.life_low, crew_type.life_high, game::LIFE);
			this->life = init_life;
			this->max_life = init_life;

			this->skill = static_cast<std::uint16_t>(game::parameters_table.ssd_crew_generator_table->roll_skill(crew_type.skill_table));
			const auto job_type_idx = game::parameters_table.ssd_crew_generator_table->roll_job(crew_type.job_table);

			auto job_type = 0u;
			switch (job_type_idx)
			{
			case 0:
				job_type = game::GROUP_DEVELOP;
				break;
			case 1:
				job_type = game::GROUP_FOOD;
				break;
			case 2:
				job_type = game::GROUP_MEDIC;
				break;
			case 3:
				job_type = game::GROUP_FARM;
				break;
			case 4:
				job_type = game::GROUP_BASE_DEFENSE;
				break;
			case 5:
				job_type = game::GROUP_COMBAT_DEPLOY;
				break;
			}

			if (job_type != 0u)
			{
				const auto base_rank = game::rank_value_to_hash(6);
				this->previous_job = roll_skill_value(base_rank, base_rank, job_type);
			}
		}
		else if (unique_type_code != 0u || unique_index1 != 0u)
		{
			this->unique_index = unique_type_code != 0u 
				? unique_type_code 
				: unique_index1;

			const auto iter = game::parameters_table.ssd_crew_generator_table->unique_table.find(this->unique_index);
			if (iter == game::parameters_table.ssd_crew_generator_table->unique_table.end())
			{
				return false;
			}

			this->life = iter->second->life;
			this->max_life = iter->second->life;
			this->face_id = iter->second->face;
			this->body_id = iter->second->body;
			this->sex_id = iter->second->sex;
			this->race_id = iter->second->race;
		}
		else
		{
			return false;
		}

		this->sanity = 1000;
		this->previous_group = 1;
		this->current_group = 1;

		return true;
	}

	void motivation_history_internal_t::to_json(json::value& data) const
	{
		for (auto i = 0ull; i < ARRAYSIZE(this->event); i++)
		{
			data["motivation_history_event"][i] = this->event[i];
			data["motivation_history_value"][i] = this->value[i];
		}
	}

	namespace impl
	{
		template <database_type_t Type>
		std::uint64_t create(const std::uint64_t player_id, const member_params_t& params)
		{
			return database::access<std::uint64_t>([&](database::database_t& db)
			{
				return db.exec<Type>(
					sqlpp::insert_into(crew_member::table)
						.set(crew_member::table.f_player_id = player_id,
							 crew_member::table.member_type = params.unique_index,
							 crew_member::table.face_id = params.face_id,
							 crew_member::table.body_id = params.body_id,
							 crew_member::table.race_id = params.race_id,
							 crew_member::table.sex_id = params.sex_id,
							 crew_member::table.voice_type = params.voice_type,
							 crew_member::table.previous_group = params.previous_group,
							 crew_member::table.current_group = params.current_group,
							 crew_member::table.previous_job = params.previous_job,
							 crew_member::table.item1_count = params.item1_count,
							 crew_member::table.item2_count = params.item2_count,
							 crew_member::table.item3_count = params.item3_count,
							 crew_member::table.item4_count = params.item4_count,
							 crew_member::table.item5_count = params.item5_count,
							 crew_member::table.item6_count = params.item6_count,
							 crew_member::table.life = params.life,
							 crew_member::table.life_max = params.max_life,
							 crew_member::table.health_condition = params.condition,
							 crew_member::table.health_flag = params.health_flag,
							 crew_member::table.map_location = params.map_location,
							 crew_member::table.skill = params.skill,
							 crew_member::table.sanity = params.sanity,
							 crew_member::table.survival_days = params.survival_days,
							 crew_member::table.injury_id_1 = params.injury_id_1,
							 crew_member::table.injury_id_2 = params.injury_id_2,
							 crew_member::table.injury_time_1 = params.injury_time_1,
							 crew_member::table.injury_time_2 = params.injury_time_2,
							 crew_member::table.sickness_id_1 = params.sickness_id_1,
							 crew_member::table.sickness_id_2 = params.sickness_id_2,
							 crew_member::table.sickness_time_1 = params.sickness_time_1,
							 crew_member::table.sickness_time_2 = params.sickness_time_2,
							 crew_member::table.ability_accessory = params.ability_accessory,
							 crew_member::table.ability_animal = params.ability_animal,
							 crew_member::table.ability_gadget = params.ability_gadget,
							 crew_member::table.ability_defense_unit = params.ability_defense_unit,
							 crew_member::table.ability_base_defense = params.ability_base_defense,
							 crew_member::table.ability_develop = params.ability_develop,
							 crew_member::table.ability_expedition = params.ability_expedition,
							 crew_member::table.ability_food = params.ability_food,
							 crew_member::table.ability_medical = params.ability_medical,
							 crew_member::table.ability_plant = params.ability_plant,
							 crew_member::table.resistance_food_shortage = params.resistance_food_shortage,
							 crew_member::table.resistance_sleepless = params.resistance_sleepless,
							 crew_member::table.resistance_water_shortage = params.resistance_water_shortage,
							 crew_member::table.first_name_index = params.first_name_index,
							 crew_member::table.last_name_index = params.last_name_index,
							 crew_member::table.nickname = params.nickname,
							 crew_member::table.motivation_history = params.motivation_history.serialize(),
							 crew_member::table.creation_date = std::chrono::system_clock::now()
						)
				);
			});
		}

		template <database_type_t Type>
		std::optional<crew_member> find(const std::uint64_t player_id, const std::uint64_t member_id)
		{
			return database::access<std::optional<crew_member>>([&](database::database_t& db)
				-> std::optional<crew_member>
			{
				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::all_of(crew_member::table)).from(crew_member::table)
						.where(crew_member::table.member_id == member_id && crew_member::table.f_player_id == player_id)
				);

				std::optional<crew_member> list;

				if (!results.empty())
				{
					list.emplace(results.front());
				}

				return list;
			});
		}

		template <database_type_t Type>
		std::vector<crew_member> get_all(const std::uint64_t player_id)
		{
			return database::access<std::vector<crew_member>>([&](database::database_t& db)
				-> std::vector<crew_member>
			{
				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::all_of(crew_member::table)).from(crew_member::table)
						.where(crew_member::table.f_player_id == player_id)
							.order_by(crew_member::table.creation_date.asc()));

				std::vector<crew_member> list;

				for (auto& row : results)
				{
					list.emplace_back(crew_member(row));
				}

				return list;
			});
		}
		
		template <database_type_t Type>
		std::size_t get_member_count(const std::uint64_t player_id)
		{
			return database::access<std::size_t>([&](database::database_t& db) -> std::size_t
			{
				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::count(1))
						.from(crew_member::table)
							.where(crew_member::table.f_player_id == player_id));

				if (results.empty())
				{
					return 0u;
				}

				return results.front().count;
			});
		}
		
		template <database_type_t Type>
		void update1(const std::uint64_t player_id, const std::uint64_t member_id, const member_params_t& params)
		{
			database::access([&](database::database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(crew_member::table)
						.set(crew_member::table.health_condition = params.condition,
							 crew_member::table.health_flag = params.health_flag,
							 crew_member::table.injury_id_1 = params.injury_id_1,
							 crew_member::table.injury_id_2 = params.injury_id_2,
							 crew_member::table.injury_time_1 = params.injury_time_1,
							 crew_member::table.injury_time_2 = params.injury_time_2,
							 crew_member::table.life = params.life,
							 crew_member::table.life_max = params.max_life,
							 crew_member::table.previous_group = params.previous_group,
							 crew_member::table.sanity = params.sanity,
							 crew_member::table.sickness_id_1 = params.sickness_id_1,
							 crew_member::table.sickness_id_2 = params.sickness_id_2,
							 crew_member::table.sickness_time_1 = params.sickness_time_1,
							 crew_member::table.sickness_time_2 = params.sickness_time_2,
							 crew_member::table.survival_days = params.survival_days,
							 crew_member::table.motivation_history = params.motivation_history.serialize())
								.where(crew_member::table.member_id == member_id && crew_member::table.f_player_id == player_id));
			});
		}

		template <database_type_t Type>
		void update_group(const std::uint64_t player_id, const std::uint64_t member_id, const std::uint32_t previous_group, const std::uint32_t group_id)
		{
			database::access([&](database::database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(crew_member::table)
						.set(crew_member::table.previous_group = previous_group,
							 crew_member::table.current_group = group_id)
								.where(crew_member::table.member_id == member_id && crew_member::table.f_player_id == player_id));
			});
		}
		
		template <database_type_t Type>
		void update_health_flag(const std::uint64_t player_id, const std::uint64_t member_id, const std::uint32_t health_flag)
		{
			database::access([&](database::database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(crew_member::table)
						.set(crew_member::table.health_flag = health_flag)
							.where(crew_member::table.member_id == member_id && crew_member::table.f_player_id == player_id));
			});
		}

		template <database_type_t Type>
		void update_nickname(const std::uint64_t player_id, const std::uint64_t member_id, const std::string& nickname)
		{
			database::access([&](database::database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(crew_member::table)
						.set(crew_member::table.nickname = nickname)
								.where(crew_member::table.member_id == member_id && crew_member::table.f_player_id == player_id));
			});
		}
		
		template <database_type_t Type>
		void update_sickness(const std::uint64_t player_id, const std::uint64_t member_id, 
			const std::uint32_t life,
			const std::uint32_t sickness_id_1, const std::uint32_t sickness_id_2,
			const std::uint32_t sickness_time_1, const std::uint32_t sickness_time_2)
		{
			database::access([&](database::database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(crew_member::table)
						.set(crew_member::table.life = life,
							 crew_member::table.sickness_id_1 = sickness_id_1,
							 crew_member::table.sickness_id_2 = sickness_id_2,
							 crew_member::table.sickness_time_1 = sickness_time_1,
							 crew_member::table.sickness_time_2 = sickness_time_2)
								.where(crew_member::table.member_id == member_id && crew_member::table.f_player_id == player_id));
			});
		}
			
		template <database_type_t Type>
		void update_injury(const std::uint64_t player_id, const std::uint64_t member_id, 
			const std::uint32_t life,
			const std::uint32_t injury_id_1, const std::uint32_t injury_id_2,
			const std::uint32_t injury_time_1, const std::uint32_t injury_time_2)
		{
			database::access([&](database::database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(crew_member::table)
						.set(crew_member::table.life = life,
							 crew_member::table.injury_id_1 = injury_id_1,
							 crew_member::table.injury_id_2 = injury_id_2,
							 crew_member::table.injury_time_1 = injury_time_1,
							 crew_member::table.injury_time_2 = injury_time_2)
								.where(crew_member::table.member_id == member_id && crew_member::table.f_player_id == player_id));
			});
		}
		template <database_type_t Type>
		void add_items(const std::uint64_t player_id, const std::uint64_t member_id, const std::array<std::uint32_t, 6>& add_counts)
		{
			database::access([&](database::database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(crew_member::table)
						.set(crew_member::table.item1_count = crew_member::table.item1_count + add_counts[0],
							 crew_member::table.item2_count = crew_member::table.item2_count + add_counts[1],
							 crew_member::table.item3_count = crew_member::table.item3_count + add_counts[2],
							 crew_member::table.item4_count = crew_member::table.item4_count + add_counts[3],
							 crew_member::table.item5_count = crew_member::table.item5_count + add_counts[4],
							 crew_member::table.item6_count = crew_member::table.item6_count + add_counts[5])
								.where(crew_member::table.member_id == member_id && crew_member::table.f_player_id == player_id));
			});
		}

		template <database_type_t Type>
		bool remove(const std::uint64_t player_id, const std::uint64_t member_id)
		{
			return database::access<bool>([&](database::database_t& db)
			{
				const auto result = db.exec<Type>(
					sqlpp::remove_from(crew_member::table)
						.where(crew_member::table.member_id == member_id && crew_member::table.f_player_id == player_id));
				return result != 0ull;
			});
		}
						
		template <database_type_t Type>
		void remove_all(const std::uint64_t player_id)
		{
			database::access([&](database::database_t& db)
			{
				db.exec<Type>(
					sqlpp::remove_from(crew_member::table)
						.where(crew_member::table.f_player_id == player_id));
			});
		}

		template <database_type_t Type>
		void delete_player_data(const std::uint64_t player_id)
		{
			database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::remove_from(crew_member::table)
						.where(crew_member::table.f_player_id == player_id));
			});
		}
	}

	void crew_member::update(const member_params_t& params) const
	{
		RUN_IMPL(impl::update1, this->get_player_id(), this->get_member_id(), params);
	}

	void crew_member::update_group(const std::uint32_t group_id) const
	{
		RUN_IMPL(impl::update_group, this->get_player_id(), this->get_member_id(), this->get_current_group(), group_id);
	}

	void crew_member::update_health_flag(const std::uint32_t health_flag) const
	{
		RUN_IMPL(impl::update_health_flag, this->get_player_id(), this->get_member_id(), health_flag);
	}

	void crew_member::update_nickname(const std::string& nickname) const
	{
		RUN_IMPL(impl::update_nickname, this->get_player_id(), this->get_member_id(), nickname);
	}

	void crew_member::update_injury(const std::uint32_t life, const std::uint32_t injury_id_1, const std::uint32_t injury_id_2,
		const std::uint32_t injury_time_1, const std::uint32_t injury_time_2) const
	{
		RUN_IMPL(impl::update_injury, this->get_player_id(), this->get_member_id(), life, injury_id_1, injury_id_2, injury_time_1, injury_time_2);
	}

	void crew_member::update_sickness(const std::uint32_t life, const std::uint32_t injury_id_1, const std::uint32_t injury_id_2,
		const std::uint32_t injury_time_1, const std::uint32_t injury_time_2) const
	{
		RUN_IMPL(impl::update_sickness, this->get_player_id(), this->get_member_id(), life, injury_id_1, injury_id_2, injury_time_1, injury_time_2);
	}

	void crew_member::add_items(const std::array<std::uint32_t, 6>& add_counts) const
	{
		RUN_IMPL(impl::add_items, this->get_player_id(), this->get_member_id(), add_counts);
	}

	void crew_member::to_json(json::value& data) const
	{
		data["condition"] = this->condition_;
		data["current_group"] = this->current_group_;
		data["health_flag"] = this->health_flag_;
		data["injury_id_1"] = this->injury_id_1_;
		data["injury_id_2"] = this->injury_id_2_;
		data["injury_time_1"] = this->injury_time_1_;
		data["injury_time_2"] = this->injury_time_2_;
		data["item1_count"] = this->item1_count_;
		data["item2_count"] = this->item2_count_;
		data["item3_count"] = this->item3_count_;
		data["item4_count"] = this->item4_count_;
		data["item5_count"] = this->item5_count_;
		data["item6_count"] = this->item6_count_;
		data["life"] = this->life_;
		data["map_location"] = this->map_location_;
		data["max_life"] = this->life_max_;
		data["previous_group"] = this->previous_group_;
		data["previous_job"] = this->previous_job_;
		data["race_id"] = this->race_id_;
		data["sanity"] = this->sanity_;
		data["sickness_id_1"] = this->sickness_id_1_;
		data["sickness_id_2"] = this->sickness_id_2_;
		data["sickness_time_1"] = this->sickness_time_1_;
		data["sickness_time_2"] = this->sickness_time_2_;
		data["skill"] = this->skill_;
		data["survival_days"] = this->survival_days_;
		data["body_id"] = this->body_id_;
		data["face_id"] = this->face_id_;
		data["sex_id"] = this->sex_id_;
		data["voice_type"] = this->voice_type_;

		const auto iter = game::parameters_table.ssd_crew_generator_table->unique_table.find(this->member_type_);
		if (iter != game::parameters_table.ssd_crew_generator_table->unique_table.end())
		{
			data["ability_accessory"] = 0;
			data["ability_animal"] = 0;
			data["ability_gadget"] = 0;
			data["ability_defense_unit"] = 0;
			data["ability_base_defense"] = iter->second->base_defense;
			data["ability_develop"] = iter->second->develop;
			data["ability_expedition"] = iter->second->combat_deploy;
			data["ability_food"] = iter->second->food;
			data["ability_medical"] = iter->second->medic;
			data["ability_plant"] = iter->second->farm;
			data["resistance_food_shortage"] = iter->second->hunger_resist;
			data["resistance_sleepless"] = iter->second->sleeplack_resist;
			data["resistance_water_shortage"] = iter->second->thirst_resist;
			data["initial_max_life"] = iter->second->life;
			data["first_name_index"] = 0;
			data["last_name_index"] = 0;
		}
		else
		{
			data["ability_accessory"] = this->ability_accessory_;
			data["ability_animal"] = this->ability_animal_;
			data["ability_gadget"] = this->ability_gadget_;
			data["ability_defense_unit"] = this->ability_defense_unit_;
			data["ability_base_defense"] = this->ability_base_defense_;
			data["ability_develop"] = this->ability_develop_;
			data["ability_expedition"] = this->ability_expedition_;
			data["ability_food"] = this->ability_food_;
			data["ability_medical"] = this->ability_medical_;
			data["ability_plant"] = this->ability_plant_;
			data["resistance_food_shortage"] = this->resistance_food_shortage_;
			data["resistance_sleepless"] = this->resistance_sleepless_;
			data["resistance_water_shortage"] = this->resistance_water_shortage_;
			data["initial_max_life"] = this->life_max_;
			data["first_name_index"] = this->first_name_index_;
			data["last_name_index"] = this->last_name_index_;
		}

		data["nickname"] = this->nickname_;

		data["previous_group"] = this->previous_group_;
		data["current_group"] = this->current_group_;
		data["item1_count"] = this->item1_count_;
		data["item2_count"] = this->item2_count_;
		data["item3_count"] = this->item3_count_;
		data["item4_count"] = this->item4_count_;
		data["item5_count"] = this->item5_count_;
		data["item6_count"] = this->item6_count_;
		data["life"] = this->life_;
		data["life_max"] = this->life_max_;
		data["unique_id"] = this->member_id_;
		data["unique_index"] = this->member_type_;
		data["generation_date"] = this->creation_date_.count();

		this->motivation_history_.to_json(data);
	}

	std::uint64_t create(const std::uint64_t player_id, const member_params_t& params)
	{
		RUN_IMPL(impl::create, player_id, params);
	}

	std::optional<crew_member> find(const std::uint64_t player_id, const std::uint64_t member_id)
	{
		RUN_IMPL(impl::find, player_id, member_id);
	}

	std::vector<crew_member> get_all(const std::uint64_t player_id)
	{
		RUN_IMPL(impl::get_all, player_id);
	}

	std::size_t get_member_count(const std::uint64_t player_id)
	{
		RUN_IMPL(impl::get_member_count, player_id);
	}

	bool remove(const std::uint64_t player_id, const std::uint64_t member_id)
	{
		RUN_IMPL(impl::remove, player_id, member_id);
	}

	void remove_all(const std::uint64_t player_id)
	{
		RUN_IMPL(impl::remove_all, player_id);
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
			database.run_query("mgssd.crew_members.create");
		}
	};
}

REGISTER_TABLE(database::crew_members::table, 2)
