#include <std_include.hpp>

#include "cmd_check_consume_transaction.hpp"

namespace emulator::ssd
{
	json::value cmd_check_consume_transaction::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		// TODO?
		result["in_transaction"] = 0;

        return result;
	}
}
