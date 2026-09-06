#include <std_include.hpp>

#include "cmd_reqauth_https.hpp"

#include "database/auth.hpp"
#include "database/models/users.hpp"

namespace emulator::ssd
{
	nlohmann::json cmd_reqauth_https::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		const auto& hash_val = data["hash"];
		const auto& account_id_val = data["user_name"];
		if (!hash_val.is_string() || !account_id_val.is_string())
		{
			return error(ERR_INVALIDARG);
		}

		const auto hash = hash_val.get<std::string>();

		result["timeout_sec"] = database::vars.session_timeout.count();
		result["heartbeat_sec"] = database::vars.session_heartbeat.count();

		result["inquiry_id"] = 0;
		result["is_not_host"] = 0;
		result["is_release_note"] = 1;
		result["login_count"] = 0;

		result["aes_key"] = nullptr;
		result["cbc_iv"] = nullptr;
		result["hmac_key"] = nullptr;

		if (!user.has_value())
		{
			const auto auth_result_opt = auth::authenticate_user(account_id_val, hash);
			if (!auth_result_opt.has_value())
			{
				return error(ERR_INVALID_ACCOUNT);
			}

			const auto& auth_result = auth_result_opt.value();
			result["crypto_key"] = auth_result.crypto_key;
			result["session"] = auth_result.session_id;
			result["user_id"] = auth_result.user_id;
		}
		else
		{
			auto expired = false;
			const auto session_key = user->get_session_id();
			const auto user_opt = database::users::find_by_session_id(session_key, false, &expired);

			if (!user_opt.has_value())
			{
				return error(ERR_INVALID_SESSION);
			}

			if (expired)
			{
				database::users::update_session(user_opt.value());
			}

			result["crypto_key"] = user_opt->get_crypto_key();
			result["session"] = session_key;
			result["user_id"] = user_opt->get_id();
		}

        return result;
	}
}
