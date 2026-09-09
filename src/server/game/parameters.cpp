#include <std_include.hpp>

#include "game.hpp"
#include "parameters.hpp"
#include "component/console.hpp"

#include "utils/resources.hpp"

#include <utils/cryptography.hpp>
#include <utils/compression.hpp>
#include <utils/io.hpp>
#include <utils/string.hpp>

namespace game
{
	parameters_table_t parameters_table;
	std::vector<std::shared_ptr<parameters::base_parameter>> parameters_list;
	std::unordered_map<std::string, std::shared_ptr<parameters::base_parameter>> parameters_map;

	const std::string& parameters::base_parameter::get_data_path()
	{
		return this->data_path_;
	}

	const std::string& parameters::base_parameter::get_data_md5()
	{
		return this->data_md5_;
	}

	const std::string& parameters::base_parameter::name()
	{
		return this->name_;
	}

	void parameters::base_parameter::load(const std::string& name)
	{
		console::log("[Game Parameter] Loading \"%s.json\"\n", name.data());

		const auto path = std::format("resources/data/parameter/{}.json", name);
		const auto resource_id = utils::resources::get_resource_id(path);
		this->name_ = name;

		if (!resource_id.has_value())
		{
			throw std::runtime_error(std::format("no resource found for parameter {}", name));
		}

		const auto data = utils::resources::load(resource_id.value());
		const auto json = nlohmann::json::parse(data, nullptr, false);

		if (json.is_discarded() || !this->parse(json))
		{
			throw std::runtime_error(std::format("failed to parse game parameter file {}.json", name));
		}

		const auto dump = json.dump();

		utils::cryptography::blowfish blow;
		blow.set_key(game::get_static_key(game::key_type_tpp), game::get_static_key_len());

		auto encrypted_data = utils::compression::zlib::compress(dump);
		encrypted_data = blow.encrypt_internal(encrypted_data);

		this->data_path_ = std::format("tmp/parameter/{}.param", name);
		this->data_md5_ = utils::string::to_lower(utils::cryptography::md5::compute(encrypted_data, true));

		utils::io::write_file(this->data_path_, encrypted_data);
	}

	void test()
	{
		const auto files = utils::io::list_files("parameter");
		for (const auto& file : files)
		{
			if (!file.ends_with(".param"))
			{
				continue;
			}

			const auto data = utils::io::read_file(file);
			utils::cryptography::blowfish blow;
			blow.set_key(game::get_static_key(game::key_type_tpp), game::get_static_key_len());

			auto decrypted1 = blow.decrypt_internal(data);
			auto decomp1 = utils::compression::zlib::decompress(decrypted1);

			auto a = nlohmann::ordered_json::parse(decomp1);
			auto b = a.dump(4);

			const auto start_pos = file.find_last_of('/');
			const auto end_pos = file.find_last_of('.');
			const auto base_name = file.substr(start_pos, (end_pos - start_pos));
			utils::io::write_file(std::format("parameter/decrypted/{}.json", base_name), b);
		}
	}

	void initialize_parameters_table()
	{
		const auto init = []<typename T>(std::shared_ptr<T>&dest)
		{
			dest = std::make_shared<T>();
			parameters_list.emplace_back(dest);
			parameters_map.insert(std::make_pair(dest->name(), dest));
		};

		init(parameters_table.ssd_building_parameter_table);
		init(parameters_table.defense_game_base_digging);
		init(parameters_table.defense_game_quest);
		init(parameters_table.ssd_base_defense_settings);
		init(parameters_table.ssd_buff_parameter);
		init(parameters_table.ssd_buff_parameter_debug);
		init(parameters_table.ssd_collection_items);
		init(parameters_table.ssd_combat_deploy_parameter_table);
		init(parameters_table.ssd_equip_parameters);
		init(parameters_table.ssd_equip_parameters_debug);
		init(parameters_table.ssd_equip_parameters_opt);
		init(parameters_table.ssd_player_parameters);
		init(parameters_table.ssd_replay_mission_info_opt);
		init(parameters_table.ssd_walker_gear_parameters);
		init(parameters_table.ssd_weapon_parameters);
		init(parameters_table.ssd_weapon_parameters_debug);
		init(parameters_table.ssd_weapon_parameters_opt);
		init(parameters_table.ssd_crew_generator_table);
		init(parameters_table.ssd_npc_level_parameters);
		init(parameters_table.ssd_damage_parameter);
		init(parameters_table.ssd_damage_parameter_debug);
		init(parameters_table.ssd_damage_parameter_opt);
		init(parameters_table.defense_game_embedded_mission);
		init(parameters_table.ssd_sbm_parameters);
		init(parameters_table.ssd_sbm_parameters_debug);
		init(parameters_table.ssd_sbm_parameters_opt);
		init(parameters_table.defense_game_event_mission);
		init(parameters_table.ssd_buff_parameter_opt);
		init(parameters_table.ssd_dlc_mission_settings);
		init(parameters_table.ssd_ui_coop_mission_info);
	}
}
