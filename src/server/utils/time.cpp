#include <std_include.hpp>

#include "time.hpp"
#include "config.hpp"

namespace utils::time
{
	void get_section(const std::chrono::seconds& duration, std::chrono::seconds& start, std::chrono::seconds& end)
	{
		const auto ref_time = config::get().vars.reference_time;
		const auto now = std::chrono::system_clock::now();
		const auto diff = std::chrono::duration_cast<std::chrono::seconds>((now - ref_time).time_since_epoch());
		const auto time = diff / duration;

		start = ref_time + time * duration;
		end = start + duration;
	}

	std::size_t get_day()
	{
		const auto ref_time = config::get().vars.reference_time;
		const auto now = std::chrono::system_clock::now();
		const auto diff = std::chrono::duration_cast<std::chrono::seconds>((now - ref_time).time_since_epoch());
		return std::chrono::floor<std::chrono::days>(diff).count();
	}

	std::chrono::seconds get_week_begin()
	{
		std::chrono::seconds start{};
		std::chrono::seconds end{};
		get_section(24h * 7, start, end);
		return start;
	}

	std::chrono::seconds get_day_begin()
	{
		std::chrono::seconds start{};
		std::chrono::seconds end{};
		get_section(24h, start, end);
		return start;
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
