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
			{RESOURCE_SERVER_PARAMETER_LIST, "resources/data/server_parameter_list.json"},

			{RESOURCE_SQL_MYSQL, "resources/sql/mysql.sql"},
			{RESOURCE_SQL_SQLITE3, "resources/sql/sqlite3.sql"},

			{RESOURCE_SERVER_PARAM_0, "resources/data/parameter/SsdBuildingParameterTable.param"},
			{RESOURCE_SERVER_PARAM_1, "resources/data/parameter/DefenseGameBaseDigging.param"},
			{RESOURCE_SERVER_PARAM_2, "resources/data/parameter/DefenseGameQuest.param"},
			{RESOURCE_SERVER_PARAM_3, "resources/data/parameter/SsdBaseDefenseSettings.param"},
			{RESOURCE_SERVER_PARAM_4, "resources/data/parameter/SsdBuffParameter.param"},
			{RESOURCE_SERVER_PARAM_5, "resources/data/parameter/SsdBuffParameter_debug.param"},
			{RESOURCE_SERVER_PARAM_6, "resources/data/parameter/SsdCollectionItems.param"},
			{RESOURCE_SERVER_PARAM_7, "resources/data/parameter/SsdCombatDeployParameterTable.param"},
			{RESOURCE_SERVER_PARAM_8, "resources/data/parameter/SsdEquipParameters.param"},
			{RESOURCE_SERVER_PARAM_9, "resources/data/parameter/SsdEquipParameters_debug.param"},
			{RESOURCE_SERVER_PARAM_10, "resources/data/parameter/SsdEquipParameters_opt.param"},
			{RESOURCE_SERVER_PARAM_11, "resources/data/parameter/SsdPlayerParameters.param"},
			{RESOURCE_SERVER_PARAM_12, "resources/data/parameter/SsdReplayMissionInfo_opt.param"},
			{RESOURCE_SERVER_PARAM_13, "resources/data/parameter/SsdWalkerGearParameters.param"},
			{RESOURCE_SERVER_PARAM_14, "resources/data/parameter/SsdWeaponParameters.param"},
			{RESOURCE_SERVER_PARAM_15, "resources/data/parameter/SsdWeaponParameters_debug.param"},
			{RESOURCE_SERVER_PARAM_16, "resources/data/parameter/SsdWeaponParameters_opt.param"},
			{RESOURCE_SERVER_PARAM_17, "resources/data/parameter/SsdCrewGeneratorTable.param"},
			{RESOURCE_SERVER_PARAM_18, "resources/data/parameter/SsdNpcLevelParameters.param"},
			{RESOURCE_SERVER_PARAM_19, "resources/data/parameter/SsdDamageParameter.param"},
			{RESOURCE_SERVER_PARAM_20, "resources/data/parameter/SsdDamageParameter_debug.param"},
			{RESOURCE_SERVER_PARAM_21, "resources/data/parameter/SsdDamageParameter_opt.param"},
			{RESOURCE_SERVER_PARAM_22, "resources/data/parameter/DefenseGameEmbeddedMission.param"},
			{RESOURCE_SERVER_PARAM_23, "resources/data/parameter/SsdSbmParameters.param"},
			{RESOURCE_SERVER_PARAM_24, "resources/data/parameter/SsdSbmParameters_debug.param"},
			{RESOURCE_SERVER_PARAM_25, "resources/data/parameter/SsdSbmParameters_opt.param"},
			{RESOURCE_SERVER_PARAM_26, "resources/data/parameter/DefenseGameEventMission.param"},
			{RESOURCE_SERVER_PARAM_27, "resources/data/parameter/SsdBuffParameter_opt.param"},
			{RESOURCE_SERVER_PARAM_28, "resources/data/parameter/SsdDlcMissionSettings.param"},
			{RESOURCE_SERVER_PARAM_29, "resources/data/parameter/SsdUiCoopMissionInfo.param"},
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

	std::optional<std::int32_t> get_resource_id(const std::string& path)
	{
		const auto iter = std::ranges::find_if(resource_map.begin(), resource_map.end(), [&](const auto& a)
		{
			return a.second == path;
		});

		if (iter == resource_map.end())
		{
			return {};
		}

		return {iter->first};
	}
}
