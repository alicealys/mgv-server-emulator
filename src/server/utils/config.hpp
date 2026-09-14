#pragma once

#include "component/console.hpp"

#include <utils/string.hpp>

namespace config
{
	struct config_t
	{
		struct vars_t
		{
			std::uint32_t session_heartbeat;
			std::uint32_t session_timeout;
			std::uint32_t server_version;
		};

		std::string base_url = "http://localhost:80";
		std::uint16_t https_port = 443;
		std::uint16_t http_port = 80;
		std::string database_type = "sqlite3";
		std::string database_user = "root";
		std::string database_password = "root";
		std::string database_host = "127.0.0.1";
		std::uint16_t database_port = 3306;
		std::string database_name = "mgssd";
		std::string auth_mode = "offline";
		std::string cert_file = "";
		std::string key_file = "";
		bool use_tmp_folder = true;
		std::string http_client_ip_header = "";
		bool enable_web_api = true;
		bool allow_save_restore = true;
		vars_t vars;
	};

	const config_t& get();
}
