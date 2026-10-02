#include <std_include.hpp>

#include "wicked_reports.hpp"
#include "../auth.hpp"

#include <utils/cryptography.hpp>
#include <utils/string.hpp>

namespace database::wicked_reports
{
	namespace
	{
		std::vector<report_type_t> load_report_types()
		{
			std::vector<report_type_t> result;

			auto list = utils::resources::load_json(RESOURCE_WICKED_REPORT_TYPES);
			for (auto i = 0ull; i < list.size(); i++)
			{
				report_type_t type{};
				if (!json::read(type, list[i]))
				{
					continue;
				}

				result.emplace_back(type);
			}

			return result;
		}
	}

	const std::vector<report_type_t>& get_report_types()
	{
		static const auto list = load_report_types();
		return list;
	}

	GET_FIELD_C(wicked_report, std::uint64_t, report_id);
	GET_FIELD_C(wicked_report, std::uint64_t, target_user_id);
	GET_FIELD_C(wicked_report, std::uint64_t, source_user_id);
	GET_FIELD_C(wicked_report, std::uint32_t, report_type_01);
	GET_FIELD_C(wicked_report, std::uint32_t, report_type_02);
	GET_FIELD_C(wicked_report, std::uint32_t, report_type_03);
	GET_FIELD_C(wicked_report, std::uint32_t, report_type_04);
	GET_FIELD_C(wicked_report, std::uint32_t, report_type_05);
	GET_FIELD_C(wicked_report, std::chrono::seconds, report_date);

	namespace impl
	{
		template <database_type_t Type>
		bool send_report(const report_params_t& params)
		{
			return database::access<bool>([&](database_t& db)
			{
				auto results = db.exec<Type>(
					sqlpp::insert_into(wicked_report::table)
						.set(wicked_report::table.target_user_id = params.target_user_id,
							 wicked_report::table.source_user_id = params.source_user_id,
							 wicked_report::table.report_type_01 = params.report_types[0],
							 wicked_report::table.report_type_02 = params.report_types[1],
							 wicked_report::table.report_type_03 = params.report_types[2],
							 wicked_report::table.report_type_04 = params.report_types[3],
							 wicked_report::table.report_type_05 = params.report_types[4],
							 wicked_report::table.report_date = std::chrono::system_clock::now()
						));
				return results != 0u;
			});
		}
	}

	bool send_report(const report_params_t& params)
	{
		RUN_IMPL(impl::send_report, params);
	}

	class table final : public table_interface
	{
	public:
		void create(database_t& database) override
		{
			database.run_query("mgssd.wicked_reports.create");

			get_report_types();
		}
	};
}

REGISTER_TABLE(database::wicked_reports::table, -1)
