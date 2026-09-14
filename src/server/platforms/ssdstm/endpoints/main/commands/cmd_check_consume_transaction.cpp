#include <std_include.hpp>

#include "cmd_check_consume_transaction.hpp"

namespace emulator::ssd
{
	glz::json cmd_check_consume_transaction::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;

		// TODO?
		result["in_transaction"] = 0;

        return result;
	}
}
