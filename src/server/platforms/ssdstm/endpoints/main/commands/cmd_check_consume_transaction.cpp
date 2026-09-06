#include <std_include.hpp>

#include "cmd_check_consume_transaction.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_check_consume_transaction::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		// TODO?
		result["in_transaction"] = 0;

        return result;
	}
}
