#include <std_include.hpp>

#include "cmd_wicked_report.hpp"

#include "database/models/wicked_reports.hpp"

namespace emulator::ssd
{
	json::value cmd_wicked_report::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data) || param.target.type != 3)
		{
			return error(ERR_INVALIDARG);
		}

		const auto target_user = database::users::find_from_account(param.target.id);
		if (!target_user.has_value())
		{
			return error(ERR_NOT_FOUND);
		}

		database::wicked_reports::report_params_t report_params{};
		report_params.source_user_id = user->get_user_id();
		report_params.target_user_id = target_user->get_user_id();

		if (report_params.source_user_id == report_params.target_user_id)
		{
			return error(ERR_DATABASE);
		}

		const auto& report_types_def = database::wicked_reports::get_report_types();

		auto count = 0u;
		const auto report_count = std::min(param.report_list.size(), report_params.report_types.size());
		for (auto i = 0ull; i < report_count; i++)
		{
			const auto iter = std::ranges::find_if(report_types_def.begin(), report_types_def.end(), 
				[&](const database::wicked_reports::report_type_t& type)
				{
					return type.id == param.report_list[i];
				}
			);

			if (iter == report_types_def.end())
			{
				return error(ERR_INVALIDARG);
			}

			report_params.report_types[count++] = iter->id;
		}

		database::wicked_reports::send_report(report_params);

		return result;
	}

	std::uint32_t cmd_wicked_report::flags()
	{
		return CMD_NEEDS_USER;
	}
}
