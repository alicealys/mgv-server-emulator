#include <std_include.hpp>

#include "cmd_wicked_get_text.hpp"

#include "database/models/wicked_reports.hpp"

#include "utils/lang_strings.hpp"

namespace emulator::ssd
{
	json::value cmd_wicked_get_text::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint8_t lang{};
		if (!json::read(lang, data["lang"]))
		{
			return error(ERR_INVALIDARG);
		}

		const auto& types = database::wicked_reports::get_report_types();
		for (auto i = 0ull; i < types.size(); i++)
		{
			auto& entry = result["report_list"][i];
			entry["report_id"] = types[i].id;
			entry["text"] = utils::get_lang_string(types[i].text, lang);
		}

		return result;
	}
}
