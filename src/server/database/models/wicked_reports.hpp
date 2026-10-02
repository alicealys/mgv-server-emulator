#pragma once

#include "../database.hpp"

namespace database::wicked_reports
{
	struct report_params_t
	{
		std::uint64_t target_user_id;
		std::uint64_t source_user_id;
		std::array<std::uint32_t, 5> report_types;
	};

	struct report_type_t
	{
		std::uint32_t id;
		std::string text;
	};

	const std::vector<report_type_t>& get_report_types();

	class wicked_report
	{
	public:
		DEFINE_FIELD(report_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(target_user_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(source_user_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(report_type_01, sqlpp::integer_unsigned);
		DEFINE_FIELD(report_type_02, sqlpp::integer_unsigned);
		DEFINE_FIELD(report_type_03, sqlpp::integer_unsigned);
		DEFINE_FIELD(report_type_04, sqlpp::integer_unsigned);
		DEFINE_FIELD(report_type_05, sqlpp::integer_unsigned);
		DEFINE_FIELD(report_date, sqlpp::time_point);
		DEFINE_TABLE(wicked_reports, 
			report_id_field_t, 
			target_user_id_field_t,
			source_user_id_field_t,
			report_type_01_field_t,
			report_type_02_field_t,
			report_type_03_field_t,
			report_type_04_field_t,
			report_type_05_field_t,
			report_date_field_t
		);

		inline static table_t table;

		template <typename ...Args>
		wicked_report(const sqlpp::result_row_t<Args...>& row)
		{
			this->report_id_ = row.report_id;
			this->target_user_id_ = row.target_user_id;
			this->source_user_id_ = row.source_user_id;
			this->report_type_01_ = static_cast<std::uint32_t>(row.report_type_01);
			this->report_type_02_ = static_cast<std::uint32_t>(row.report_type_02);
			this->report_type_03_ = static_cast<std::uint32_t>(row.report_type_03);
			this->report_type_04_ = static_cast<std::uint32_t>(row.report_type_04);
			this->report_type_05_ = static_cast<std::uint32_t>(row.report_type_05);
			this->report_date_ = std::chrono::duration_cast<std::chrono::seconds>(row.report_date.value().time_since_epoch());
		}

		GET_FIELD_H(std::uint64_t, report_id);
		GET_FIELD_H(std::uint64_t, target_user_id);
		GET_FIELD_H(std::uint64_t, source_user_id);
		GET_FIELD_H(std::uint32_t, report_type_01);
		GET_FIELD_H(std::uint32_t, report_type_02);
		GET_FIELD_H(std::uint32_t, report_type_03);
		GET_FIELD_H(std::uint32_t, report_type_04);
		GET_FIELD_H(std::uint32_t, report_type_05);
		GET_FIELD_H(std::chrono::seconds, report_date);
	};

	bool send_report(const report_params_t& params);
}
