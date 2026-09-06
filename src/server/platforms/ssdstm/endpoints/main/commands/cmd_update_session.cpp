#include <std_include.hpp>

#include "cmd_update_session.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_update_session::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		if (!database::users::update_session(user.value()))
		{
			return error(ERR_DATABASE);
		}

        return result;
	}

	bool cmd_update_session::needs_user()
	{
		return true;
	}
}
