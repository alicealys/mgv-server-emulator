#pragma once
#include <sqlpp11/sqlpp11.h>

namespace utils::time
{
	// Tuesday - 8 AM (konami maintenance time)
	constexpr const auto reference_day = date::Tuesday;
	constexpr const auto reference_day_hour = 8h;

	std::chrono::seconds get_week_begin();
	std::chrono::seconds get_day_begin();

	std::size_t get_day();
	std::size_t get_weekday();
	std::size_t get_week();
}
