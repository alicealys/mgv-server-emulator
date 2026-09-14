#include <std_include.hpp>

#include "cmd_commit_consume_transaction.hpp"

// not implemented
namespace emulator::ssd
{
	glz::json cmd_commit_consume_transaction::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;
        result["result"] = "ERR_NOTIMPLEMENTED";
        return result;
	}
}
