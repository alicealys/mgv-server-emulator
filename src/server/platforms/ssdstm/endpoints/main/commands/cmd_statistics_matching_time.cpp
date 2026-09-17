#include <std_include.hpp>

#include "cmd_statistics_matching_time.hpp"

struct param_t
{
	std::uint32_t cancel_flag;
	std::uint32_t flag;
	std::uint32_t matching_time;
	std::uint32_t member_num;
	std::uint32_t region_level;
	std::uint32_t room_id;
	std::uint32_t rtt_reject;
	std::uint32_t search_num;
};

namespace emulator::ssd
{
	json::value cmd_statistics_matching_time::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		return result;
	}

	std::uint32_t cmd_statistics_matching_time::flags()
	{
		return CMD_NEEDS_USER;
	}
}
