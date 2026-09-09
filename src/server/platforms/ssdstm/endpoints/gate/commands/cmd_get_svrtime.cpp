#include <std_include.hpp>

#include "cmd_get_svrtime.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_get_svrtime::execute(nlohmann::json& data, const std::optional<database::users::user>&)
	{
		nlohmann::json result;

		result["result"] = "NOERR";
		result["date"] = std::time(nullptr);

		std::chrono::system_clock::time_point start = date::floor<date::days>(std::chrono::system_clock::now());

		for (auto i = 0ull; i < 7ull; i++)
		{
			auto& entry = result["day_of_week_timestamp_list"][i];

			const auto day = date::floor<date::days>(start);
			const auto day_s = std::chrono::duration_cast<std::chrono::seconds>(start.time_since_epoch()).count();
			const auto day_start_s = day_s + 86400ull * i;
			date::weekday wd{day};

			entry["day_of_week"] = wd.c_encoding() + 1;
			entry["start_datetime"] = day_start_s;
			entry["end_datetime"] = day_start_s + 86399ull;

			start += 86400s;
		}

		return result;
	}
}
