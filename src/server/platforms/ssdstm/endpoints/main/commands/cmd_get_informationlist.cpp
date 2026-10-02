#include <std_include.hpp>

#include "cmd_get_informationlist.hpp"
#include "cmd_information_get_title.hpp"

#include "utils/lang_strings.hpp"

namespace emulator::ssd
{
	json::value cmd_get_informationlist::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		std::uint8_t lang = utils::LANG_ANY;
		const auto iter = utils::language_code_map.find(param.lang);
		if (iter != utils::language_code_map.end())
		{
			lang = iter->second;
		}

		auto count = 0u;
		for (auto i = 0ull; i < cmd_information_get_title::messages.size(); i++)
		{
			const auto& msg = cmd_information_get_title::messages[i];
			auto& entry = result["info_list"][count++];
			entry["date"] = msg.get_update_date();
			entry["info_id"] = msg.info_id;
			entry["important"] = msg.important != 0u ? "TRUE" : "FALSE";
			entry["mes_subject"] = msg.format_title(lang);
			entry["mes_body"] = msg.format_text(lang);
		}

		result["info_num"] = count;

		return result;
	}
}
