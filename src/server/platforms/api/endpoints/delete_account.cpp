#include <std_include.hpp>

#include "steam_openid.hpp"
#include "delete_account.hpp"
#include "database/auth.hpp"
#include "database/models/users.hpp"

#include <utils/http.hpp>

namespace emulator
{
	glz::json delete_account::handle_request(const utils::request_params& params)
	{
		glz::json result;
		result["result"] = false;

		std::uint64_t steam_id{};
		const auto is_valid = steam_openid::verify_user(params, steam_id);
		if (is_valid)
		{
			result["result"] = database::users::delete_all_user_data(steam_id);
		}

		return result;
	}
}
