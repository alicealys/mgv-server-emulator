#include <std_include.hpp>

#include "deployments.hpp"

#include "utils/encoding.hpp"
#include "utils/json_utils.hpp"

#include <utils/cryptography.hpp>
#include <utils/string.hpp>

namespace database::deployments
{
	namespace
	{
		std::vector<deployment_mission_info_t> load_deployments_mission_list()
		{
			std::vector<deployment_mission_info_t> result;

			auto list = utils::resources::load_json(RESOURCE_DEPLOYMENTS_MISSION_LIST);
			for (auto i = 0ull; i < list.size(); i++)
			{
				deployment_mission_info_t info{};
				info.parse(list[i]);
				result.emplace_back(info);
			}

			return result;
		}

		std::unordered_map<std::uint32_t, deployment_mission_info_t> load_deployments_mission_map()
		{
			std::unordered_map<std::uint32_t, deployment_mission_info_t> map;
			auto list = load_deployments_mission_list();

			for (auto& entry : list)
			{
				if (map.contains(entry.id))
				{
					console::warning("[deployments] duplicate mission id %i\n", entry.id);
					continue;
				}

				map.insert(std::make_pair(entry.id, entry));
			}

			return map;
		}
	}

	const std::vector<deployment_mission_info_t>& get_deployments_mission_list()
	{
		static const auto list = load_deployments_mission_list();
		return list;
	}

	const std::unordered_map<std::uint32_t, deployment_mission_info_t>& get_deployments_mission_map()
	{
		static const auto map = load_deployments_mission_map();
		return map;
	}

	bool deployment_mission_info_t::parse(json::value& data)
	{
		return json::read(*this, data);
	}

	void deployment_mission_info_t::to_json(json::value& data) const
	{
		data["combat"] = this->combat;
		data["combat_penalty"] = this->combat_penalty;
		data["combat_revise"] = this->combat_revise;
		data["destination_id"] = this->destination_id;
		data["difficulty"] = this->difficulty;
		data["id"] = this->id;
		data["inactive"] = this->inactive;
		data["info"] = this->info;
		data["injure_base"] = this->injure_base;
		data["injure_max"] = this->injure_max;
		data["injure_min"] = this->injure_min;
		data["is_new"] = this->is_new;
		data["limit_index"] = this->limit_index;
		data["limit_max"] = this->limit_max;
		data["location"] = this->location;
		data["name_id_01"] = this->name_id_01;
		data["name_id_02"] = this->name_id_02;
		data["pos_x"] = this->pos_x;
		data["pos_z"] = this->pos_z;
		data["required_time"] = this->required_time;
		data["reward_id"] = this->reward_id;

		data["reward_list"] = json::array();
		for (auto i = 0ull; i < this->reward_list.size(); i++)
		{
			data["reward_list"][i]["combat_rate"] = this->reward_list[i].combat_rate;
			data["reward_list"][i]["rate"] = this->reward_list[i].rate;
			data["reward_list"][i]["survival_rate"] = this->reward_list[i].survival_rate;
			this->reward_list[i].item_info.to_json(data["reward_list"][i]["item_info"]);
		}

		data["survival"] = this->survival;
		data["survival_penalty"] = this->survival_penalty;
		data["survival_revise"] = this->survival_revise;
		data["time_revise"] = this->time_revise;
		data["type"] = this->type;
	}

	void team_params_t::parse(const std::string& data)
	{
		const auto raw_data = utils::cryptography::base64::decode(data);
		if (raw_data.size() != sizeof(team_params_t))
		{
			return;
		}

		std::memcpy(this, raw_data.data(), raw_data.size());
	}

	std::string team_params_t::serialize() const
	{
		return utils::encoding::encode_base64(*this);
	}

	GET_FIELD_C(deployment_team, std::uint64_t, team_id);
	GET_FIELD_C(deployment_team, std::uint64_t, player_id);
	GET_FIELD_C(deployment_team, std::uint64_t, index);
	GET_FIELD_C(deployment_team, std::string, info_name);
	GET_FIELD_C(deployment_team, std::uint32_t, info_status);
	GET_FIELD_C(deployment_team, std::uint32_t, info_combat);
	GET_FIELD_C(deployment_team, std::uint32_t, info_survive);
	GET_FIELD_C(deployment_team, std::uint32_t, mission_id);
	GET_FIELD_C(deployment_team, std::uint32_t, mission_type);
	GET_FIELD_C(deployment_team, std::uint32_t, mission_info);
	GET_FIELD_C(deployment_team, std::uint64_t, crew_id_01);
	GET_FIELD_C(deployment_team, std::uint64_t, crew_id_02);
	GET_FIELD_C(deployment_team, std::uint64_t, crew_id_03);
	GET_FIELD_C(deployment_team, std::uint64_t, crew_id_04);
	GET_FIELD_C(deployment_team, team_item_t, item_01);
	GET_FIELD_C(deployment_team, team_item_t, item_02);
	GET_FIELD_C(deployment_team, team_item_t, item_03);
	GET_FIELD_C(deployment_team, team_item_t, item_04);
	GET_FIELD_C(deployment_team, team_item_t, item_05);
	GET_FIELD_C(deployment_team, std::chrono::seconds, complete_date);
	GET_FIELD_C(deployment_team, std::chrono::seconds, creation_date);

	const team_params_t& deployment_team::get_params() const
	{
		return this->params_;
	}

	team_item_t deployment_team::get_item(const std::uint32_t index) const
	{
		switch (index)
		{
		case 0:
			return this->item_01_;
		case 1:
			return this->item_02_;
		case 2:
			return this->item_03_;
		case 3:
			return this->item_04_;
		case 4:
			return this->item_05_;
		}

		return {};
	}

	void team_item_t::to_json(json::value& data) const
	{
		switch (this->f.type)
		{
		case item_type_resource:
			data["id"] = game::parameters_table.ssd_sbm_parameters->get_resource_id(this->f.id_index);
			break;
		case item_type_stackable:
		case item_type_nonstackable:
			data["id"] = game::parameters_table.ssd_sbm_parameters->get_production_id(this->f.id_index);
			break;
		}

		data["param1"] = this->f.param1;
		data["param2"] = this->f.param2;
		data["type"] = this->f.type;
	}

	namespace impl
	{
		template <database_type_t Type>
		std::uint64_t create_team(const std::uint64_t player_id)
		{
			return database::access<std::uint64_t>([&](database::database_t& db)
			{
				team_params_t params{};

				return db.exec<Type>(
					sqlpp::insert_into(deployment_team::table)
						.set(deployment_team::table.f_player_id = player_id,
							 deployment_team::table.complete_date = std::chrono::system_clock::now(),
							 deployment_team::table.creation_date = std::chrono::system_clock::now(),
							 deployment_team::table.info_status = static_cast<std::uint32_t>(team_status_deploy_none),
							 deployment_team::table.params = params.serialize()
				));
			});
		}

		template <database_type_t Type>
		std::optional<deployment_team> find_team(const std::uint64_t team_id)
		{
			return database::access<std::optional<deployment_team>>([&](database::database_t& db)
				-> std::optional<deployment_team>
			{
				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::all_of(deployment_team::table)).from(deployment_team::table)
						.where(deployment_team::table.team_id == team_id)
				);

				std::optional<deployment_team> list;

				if (!results.empty())
				{
					list.emplace(results.front());
				}

				return list;
			});
		}

		template <database_type_t Type>
		std::optional<deployment_team> find_team_by_index(const std::uint64_t player_id, const std::uint64_t index)
		{
			return database::access<std::optional<deployment_team>>([&](database::database_t& db)
				-> std::optional<deployment_team>
			{
				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::all_of(deployment_team::table)).from(deployment_team::table)
						.where(deployment_team::table.f_player_id == player_id && deployment_team::table.team_index == index)
				);

				std::optional<deployment_team> list;

				if (!results.empty())
				{
					list.emplace(results.front());
				}

				return list;
			});
		}

		template <database_type_t Type>
		std::vector<deployment_team> get_all_teams(const std::uint64_t player_id)
		{
			return database::access<std::vector<deployment_team>>([&](database::database_t& db)
				-> std::vector<deployment_team>
			{
				auto results = db.exec<Type>(
					sqlpp::select(sqlpp::all_of(deployment_team::table)).from(deployment_team::table)
						.where(deployment_team::table.f_player_id == player_id)
							.order_by(deployment_team::table.team_index.asc()));

				std::vector<deployment_team> list;

				for (auto& row : results)
				{
					list.emplace_back(deployment_team(row));
				}

				return list;
			});
		}
		
		template <database_type_t Type>
		std::size_t get_team_count(const std::uint64_t player_id)
		{
			return database::access<std::size_t>([&](database::database_t& db)
			{
				auto results = db.get_database<Type>()->operator()(
					sqlpp::select(
						sqlpp::count(1))
							.from(deployment_team::table)
								.where(deployment_team::table.f_player_id == player_id));

				return results.front().count.value();
			});
		}
		
		template <database_type_t Type>
		void update_team_info(const std::uint64_t team_id, const std::uint32_t combat, const std::string& name, const std::uint32_t survive)
		{
			return database::access([&](database::database_t& db)
			{
				db.exec<Type>(sqlpp::update(deployment_team::table)
					.set(deployment_team::table.info_combat = combat,
						 deployment_team::table.info_name = name,
						 deployment_team::table.info_survive = survive)
							.where(deployment_team::table.team_id == team_id));
			});
		}
					
		template <database_type_t Type>
		void update_team_status(const std::uint64_t team_id, const std::uint32_t status)
		{
			return database::access([&](database::database_t& db)
			{
				db.exec<Type>(sqlpp::update(deployment_team::table)
					.set(deployment_team::table.info_status = status)
						.where(deployment_team::table.team_id == team_id));
			});
		}
					
		template <database_type_t Type>
		void update_team_info_by_index(const std::uint64_t player_id,  
			const std::uint64_t team_index, const std::uint32_t combat, const std::string& name, 
			const std::uint32_t status, const std::uint32_t survive)
		{
			return database::access([&](database::database_t& db)
			{
				db.exec<Type>(sqlpp::update(deployment_team::table)
					.set(deployment_team::table.info_combat = combat,
						 deployment_team::table.info_name = name,
						 deployment_team::table.info_status = status,
						 deployment_team::table.info_survive = survive)
							.where(deployment_team::table.f_player_id == player_id && deployment_team::table.team_index == team_index));
			});
		}

		template <database_type_t Type>
		void update_team_params(const std::uint64_t team_id, const team_params_t& params)
		{
			return database::access([&](database::database_t& db)
			{
				db.exec<Type>(sqlpp::update(deployment_team::table)
					.set(deployment_team::table.params = params.serialize())
						.where(deployment_team::table.team_id == team_id));
			});
		}
						
		template <database_type_t Type>
		void update_team_params_by_index(const std::uint64_t player_id, const std::uint64_t team_index, const team_params_t& params)
		{
			return database::access([&](database::database_t& db)
			{
				db.exec<Type>(sqlpp::update(deployment_team::table)
					.set(deployment_team::table.params = params.serialize())
						.where(deployment_team::table.f_player_id == player_id && deployment_team::table.team_index == team_index));
			});
		}
		
		template <database_type_t Type>
		void update_team_item(const std::uint64_t team_id, const std::uint32_t index, const team_item_t& item)
		{
			return database::access([&](database::database_t& db)
			{
				const auto do_case = [&]<typename T>(T&& t)
				{
					db.exec<Type>(sqlpp::update(deployment_team::table).set(t).where(deployment_team::table.team_id == team_id));
				};

				switch (index)
				{
				case 0:
					do_case(deployment_team::table.item_01 = item.packed);
					break;
				case 1:
					do_case(deployment_team::table.item_02 = item.packed);
					break;
				case 2:
					do_case(deployment_team::table.item_03 = item.packed);
					break;
				case 3:
					do_case(deployment_team::table.item_04 = item.packed);
					break;
				case 4:
					do_case(deployment_team::table.item_05 = item.packed);
					break;
				}
			});
		}
				
		template <database_type_t Type>
		void update_team_crew_ids(const std::uint64_t team_id, const std::uint64_t crew_id_01, const std::uint64_t crew_id_02, const std::uint64_t crew_id_03, const std::uint64_t crew_id_04)
		{
			return database::access([&](database::database_t& db)
			{
				db.exec<Type>(sqlpp::update(deployment_team::table)
					.set(deployment_team::table.crew_id_01 = crew_id_01,
						 deployment_team::table.crew_id_02 = crew_id_02,
						 deployment_team::table.crew_id_03 = crew_id_03,
						 deployment_team::table.crew_id_04 = crew_id_04
						).where(deployment_team::table.team_id == team_id));
			});
		}
						
		template <database_type_t Type>
		void update_team_mission(const std::uint64_t team_id, const std::uint32_t mission_id, const std::uint32_t mission_info,
			const std::uint32_t mission_type, const std::chrono::system_clock::time_point complete_date)
		{
			return database::access([&](database::database_t& db)
			{
				db.exec<Type>(sqlpp::update(deployment_team::table)
					.set(deployment_team::table.mission_id = mission_id,
						 deployment_team::table.mission_info = mission_info,
						 deployment_team::table.mission_type = mission_type,
						 deployment_team::table.complete_date = complete_date)
							.where(deployment_team::table.team_id == team_id));
			});
		}
								
		template <database_type_t Type>
		bool deploy(const std::uint64_t team_id, const deploy_params_t& params)
		{
			return database::access<bool>([&](database::database_t& db)
			{
				const auto result = db.exec<Type>(
					sqlpp::update(deployment_team::table)
						.set(deployment_team::table.info_combat = params.combat,
							 deployment_team::table.info_survive = params.survive,
							 deployment_team::table.info_status = params.status,
							 deployment_team::table.info_name = params.name,
							 deployment_team::table.params = params.team_params.serialize(),
							 deployment_team::table.crew_id_01 = params.crew_ids[0],
							 deployment_team::table.crew_id_02 = params.crew_ids[1],
							 deployment_team::table.crew_id_03 = params.crew_ids[2],
							 deployment_team::table.crew_id_04 = params.crew_ids[3],
							 deployment_team::table.mission_id = params.mission_id,
							 deployment_team::table.mission_info = params.mission_info,
							 deployment_team::table.mission_type = params.mission_type,
							 deployment_team::table.complete_date = params.complete_date)
								.where(deployment_team::table.team_id == team_id));
				return result != 0ull;
			});
		}

		template <database_type_t Type>
		void delete_player_data(const std::uint64_t player_id)
		{
			database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::remove_from(deployment_team::table)
						.where(deployment_team::table.f_player_id == player_id));
			});
		}
	}

	void deployment_team::update_info(const std::uint32_t combat, const std::string& name, const std::uint32_t survive) const
	{
		RUN_IMPL(impl::update_team_info, this->get_team_id(), combat, name, survive);
	}

	void deployment_team::update_status(const std::uint32_t status) const
	{
		RUN_IMPL(impl::update_team_status, this->get_team_id(), status);
	}

	void deployment_team::update_params(const team_params_t& params) const
	{
		RUN_IMPL(impl::update_team_params, this->get_team_id(), params);
	}

	void deployment_team::update_item(const std::uint32_t index, const team_item_t& item) const
	{
		RUN_IMPL(impl::update_team_item, this->get_team_id(), index, item);
	}

	void deployment_team::update_crew_ids(const std::uint64_t crew_id_01, const std::uint64_t crew_id_02, const std::uint64_t crew_id_03, const std::uint64_t crew_id_04) const
	{
		RUN_IMPL(impl::update_team_crew_ids, this->get_team_id(), crew_id_01, crew_id_02, crew_id_03, crew_id_04);
	}

	bool deployment_team::deploy(const deploy_params_t& params) const
	{
		RUN_IMPL(impl::deploy, this->get_team_id(), params);
	}

	void deployment_team::update_mission(const std::uint32_t mission_id, const std::uint32_t mission_info, 
		const std::uint32_t mission_type, const std::uint32_t required_time) const
	{
		const auto complete_date = std::chrono::system_clock::now() + required_time * 1s;
		RUN_IMPL(impl::update_team_mission, this->get_team_id(), mission_id, mission_info, mission_type, complete_date);
	}

	void deployment_team::to_json(json::value& data) const
	{
		data["complete_date"] = this->complete_date_.count();
		data["injure_rate"] = this->params_.injure_rate;
		data["success_rate"] = this->params_.success_rate;
		data["use_fast_travel"] = this->params_.use_fast_travel;

		data["mission_id"] = this->mission_id_;
		data["mission_type"] = this->mission_type_;
		data["mission_info"] = this->mission_info_;

		data["required_combat"] = this->params_.required_combat;
		data["required_survive"] = this->params_.required_survive;
		data["required_time"] = this->params_.required_time;

		this->item_01_.to_json(data["item_list"][0]);
		this->item_02_.to_json(data["item_list"][1]);
		this->item_03_.to_json(data["item_list"][2]);
		this->item_04_.to_json(data["item_list"][3]);
		this->item_05_.to_json(data["item_list"][4]);

		data["mission_name_id_01"] = 0;
		data["mission_name_id_02"] = 0;
		data["limit_index"] = 0;
		data["limit_max"] = 0;
		data["difficulty"] = 0;

		if (this->mission_id_ != 0u)
		{
			const auto& mission_map = get_deployments_mission_map();
			const auto iter = mission_map.find(this->mission_id_);

			if (iter != mission_map.end())
			{
				data["mission_name_id_01"] = iter->second.name_id_01;
				data["mission_name_id_02"] = iter->second.name_id_02;
				data["limit_index"] = iter->second.limit_index;
				data["limit_max"] = iter->second.limit_max;
				data["difficulty"] = iter->second.difficulty;
			}
		}

		data["team_info"]["combat"] = this->info_combat_;
		data["team_info"]["index"] = this->index_;
		data["team_info"]["name"] = utils::cryptography::base64::encode(this->info_name_);
		data["team_info"]["status"] = this->info_status_;
		data["team_info"]["survive"] = this->info_survive_;

		data["team_info"]["crew_id_list"][0] = this->crew_id_01_;
		data["team_info"]["crew_id_list"][1] = this->crew_id_02_;
		data["team_info"]["crew_id_list"][2] = this->crew_id_03_;
		data["team_info"]["crew_id_list"][3] = this->crew_id_04_;
	}

	std::uint64_t create_team(const std::uint64_t player_id)
	{
		RUN_IMPL(impl::create_team, player_id);
	}

	std::optional<deployment_team> find_team(const std::uint64_t team_id)
	{
		RUN_IMPL(impl::find_team, team_id);
	}

	std::optional<deployment_team> find_team_by_index(const std::uint64_t player_id, const std::uint64_t index)
	{
		RUN_IMPL(impl::find_team_by_index, player_id, index);
	}

	std::vector<deployment_team> get_all_teams(const std::uint64_t player_id)
	{
		RUN_IMPL(impl::get_all_teams, player_id);
	}

	void update_team_info_by_index(const std::uint64_t player_id, const std::uint64_t team_index, const std::uint32_t combat, 
		const std::string& name, const std::uint32_t status, const std::uint32_t survive)
	{
		RUN_IMPL(impl::update_team_info_by_index, player_id, team_index, combat, name, status, survive);
	}

	void update_team_params_by_index(const std::uint64_t player_id, const std::uint64_t team_index, const team_params_t& params)
	{
		RUN_IMPL(impl::update_team_params_by_index, player_id, team_index, params);
	}

	std::size_t get_team_count(const std::uint64_t player_id)
	{
		RUN_IMPL(impl::get_team_count, player_id);
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
			database.run_query("mgssd.deployment_teams.create");
			database.run_query("mgssd.deployment_teams.remove_insert_trigger");
			database.run_query("mgssd.deployment_teams.add_insert_trigger");

			get_deployments_mission_list();
			get_deployments_mission_map();

			if (get_team_count(3) < 1)
			{
				create_team(3);
			}
		}
	};
}

REGISTER_TABLE(database::deployments::table, 2)
