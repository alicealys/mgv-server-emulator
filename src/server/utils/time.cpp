#include <std_include.hpp>

#include "time.hpp"

namespace utils::time
{
	std::chrono::seconds get_week_begin()
	{
		const auto day = date::floor<date::days>(std::chrono::system_clock::now());
		auto start = (day - (date::weekday{day} - reference_day)) + reference_day_hour;
		
		if (std::chrono::system_clock::now() < start)
		{
			start -= 24h * 7;
		}
		
		return std::chrono::duration_cast<std::chrono::seconds>(start.time_since_epoch());
	}

	std::chrono::seconds get_day_begin()
	{
		const auto week_begin = get_week_begin();
		const auto now = std::chrono::system_clock::now();
		const auto diff = std::chrono::floor<std::chrono::days>(now - week_begin);
		return week_begin + std::chrono::duration_cast<std::chrono::seconds>(diff.time_since_epoch());
	}

	std::size_t get_day()
	{
		return std::chrono::duration_cast<std::chrono::days>(get_day_begin()).count();
	}

	std::size_t get_weekday()
	{
		return get_day() % 7;
	}

	std::size_t get_week()
	{
		return get_day() / 7;
	}
}
