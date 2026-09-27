#pragma once

#include "game/game.hpp"

namespace utils
{
	enum language_t
	{
		LANG_ANY,
		LANG_KR,
		LANG_CH,
		LANG_JP,
		LANG_RU,
		LANG_AR,
		LANG_GR,
		LANG_IT,
		LANG_PR,
		LANG_SP,
		LANG_FR,
		LANG_EN,
		LANG_COUNT
	};

	struct lang_string_t
	{
		std::string value[LANG_COUNT];
	};

	extern std::unordered_map<std::string, std::uint32_t> language_code_map;
	const std::unordered_map<std::string, lang_string_t>& get_strings();

	template <typename ...Args>
	std::string get_lang_string(const std::string& key, const std::string& language, Args&&... args)
	{
		const auto string = get_strings().find(key);
		if (string == get_strings().end())
		{
			return key;
		}

		const auto lang = language_code_map.find(language);
		if (lang == language_code_map.end())
		{
			return key;
		}

		return std::vformat(string->second.value[lang->second], std::make_format_args(args...));
	}
}
