#include <std_include.hpp>

#include "cmd_information_get_detail.hpp"
#include "cmd_information_get_title.hpp"

namespace emulator::ssd
{
	json::value cmd_information_get_detail::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		const auto iter = std::ranges::find_if(cmd_information_get_title::messages.begin(), cmd_information_get_title::messages.end(),
			[&](const cmd_information_get_title::message_t& msg)
			{
				return msg.info_id == param.info_id;
			}
		);

		if (iter != cmd_information_get_title::messages.end())
		{
			iter->to_json(result["title"], param.lang);
			result["text"] = iter->format_text(param.lang);
			result["url"] = iter->url;
		}
		else
		{
			result["title"]["title"] = "";
			result["title"]["release_date"] = 0;
			result["title"]["update_date"] = 0;
			result["title"]["important"] = 0;
			result["title"]["personal"] = 0;
			result["title"]["info_id"] = 0;
			result["text"] = "";
			result["url"] = "";
		}

		return result;
	}
}
