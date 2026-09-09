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
	namespace
	{
		void generate_structs(const std::string& name, nlohmann::json& data)
		{
			std::string buffer;
			
			const auto get_primitive_type_name = [](const std::string& name, nlohmann::json& value)
			{
				switch (value.type())
				{
				case nlohmann::json::value_t::number_integer:
				{
					if (name == "id")
					{
						return "std::uint64_t";
					}

					const auto value_int32 = value.get<std::int32_t>();
					const auto value_int64 = value.get<std::int64_t>();
					if (value_int32 == value_int64)
					{
						return "std::int32_t";
					}
					else
					{
						return "std::int64_t";
					}
				}
				case nlohmann::json::value_t::boolean:
					return "bool";
				case nlohmann::json::value_t::number_float:
					return "float";
				case nlohmann::json::value_t::string:
					return "std::string";
				}

				return "unknown";
			};

			const auto line = [&](const std::string& text)
			{
				buffer.append(std::format("{}\r\n", text));

			};

			const auto struct_begin = [&](const std::string& name)
			{
				line(std::format("struct {}_t", name));
				line("{");
			};

			const auto struct_end = [&](const std::string& name)
			{
				line("};");
			};

			const auto add_field = [&](const std::string& name, nlohmann::json& value)
			{
				switch (value.type())
				{
				case nlohmann::json::value_t::number_integer:
					line(std::format("\t{} {};", get_primitive_type_name(name, value), name));
					break;
				case nlohmann::json::value_t::boolean:
					line(std::format("\t{} {};", get_primitive_type_name(name, value), name));
					break;
				case nlohmann::json::value_t::number_float:
					line(std::format("\t{} {};", get_primitive_type_name(name, value), name));
					break;
				case nlohmann::json::value_t::string:
					line(std::format("\t{} {};", get_primitive_type_name(name, value), name));
					break;
				case nlohmann::json::value_t::array:
				{
					const auto& is_primitive = value[0].is_primitive();
					if (!is_primitive)
					{
						console::warning("field %s has array of non primitives", name.data());
						line(std::format("\tchar {}[1]; // FIXME\r\n", name));
					}
					else
					{
						line(std::format("\t{} {}[{}];", get_primitive_type_name("", value[0]), name, value.size()));
					}
				}
					break;
				}
			};

			for (auto& [key, value] : data.items())
			{
				if (!value.is_array())
				{
					continue;
				}

				struct_begin(key);

				for (auto& [field, field_value] : value[0].items())
				{
					add_field(field, field_value);
				}

				struct_end(key);
			}

			utils::io::write_file(std::format("structs/{}/structs.hpp", name), buffer);
		}
	}

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
		auto json = nlohmann::json::parse(data, nullptr, false);

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
