#include <std_include.hpp>

#include "config.hpp"
#include "database/auth.hpp"

#include <utils/io.hpp>
#include <utils/flags.hpp>

namespace config
{
	namespace
	{
		std::string get_config_file_path()
		{
			const auto config_flag = utils::flags::get_flag("config");
			return config_flag.value_or("config.json");
		}

		config_t load()
		{
			std::string data;
			const auto path = get_config_file_path();

			if (!utils::io::read_file(path, &data))
			{
				throw std::runtime_error("failed to read config");
			}

			config_t config{};
			if (!json::read(config, data))
			{
				throw std::runtime_error("failed to parse config");
			}

			if (!auth::validate_auth_mode(config.auth_mode))
			{
				throw std::runtime_error("invalid auth mode");
			}

			return config;
		}
	}

	const config_t& get()
	{
		static const auto config = load();
		return config;
	}
}
