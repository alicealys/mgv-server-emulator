#include <std_include.hpp>

#include "resources.hpp"
#include "component/console.hpp"

#include <utils/nt.hpp>
#include <utils/io.hpp>

namespace utils::resources
{
	namespace
	{
		std::unordered_map<std::int32_t, std::string> resource_map =
		{
			{RESOURCE_DATA, "resources/data/data.json"},
			{RESOURCE_GDPR_CHECK, "resources/data/gdpr_check.json"},
			{RESOURCE_PRODUCT_LIST, "resources/data/product_list.json"},

			{RESOURCE_SQL_MYSQL, "resources/sql/mysql.sql"},
			{RESOURCE_SQL_SQLITE3, "resources/sql/sqlite3.sql"},
		};
	}

	std::string load(const std::int32_t resource_id)
	{
		const auto resource_iter = resource_map.find(resource_id);
		if (resource_iter == resource_map.end())
		{
			throw std::runtime_error(std::format("invalid resource id {}", resource_id));
		}

		{
			std::string data;
			if (utils::io::read_file(resource_iter->second, &data))
			{
				return data;
			}
		}

#ifdef _WIN32
		return utils::nt::load_resource(resource_id);
#else
		console::error("resource \"%s\" (%i) not found\n", resource_iter->second.data(), resource_id);
		return {};
#endif
	}

	nlohmann::json load_json(const std::int32_t resource_id)
	{
		return nlohmann::json::parse(load(resource_id));
	}
}
