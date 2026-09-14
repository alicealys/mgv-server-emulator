#include <std_include.hpp>

#include "cmd_update_session.hpp"

namespace emulator::ssd
{
	glz::json cmd_update_session::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;

		if (!database::users::update_session(user->get_user_id()))
		{
			return error(ERR_DATABASE);
		}

        return result;
	}

	std::uint32_t cmd_update_session::flags()
	{
		return CMD_NEEDS_USER;
	}
}
