#pragma once

#include "game/game.hpp"

namespace utils
{
	enum language_t : std::uint8_t
	{
		LANG_EN = 0,
		LANG_FR = 1,
		LANG_SP = 2,
		LANG_PR = 3,
		LANG_IT = 4,
		LANG_GR = 5,
		LANG_AR = 6,
		LANG_RU = 7,
		LANG_JP = 8,
		LANG_CH = 9,
		LANG_KR = 10,
		LANG_ANY = 11,
		LANG_COUNT = 12,
	};

	struct lang_string_t
	{
		std::string value[LANG_COUNT];
	};

	extern std::unordered_map<std::string, std::uint8_t> language_code_map;
	const std::unordered_map<std::string, lang_string_t>& get_strings();

	std::string get_lang_string(const std::string& key, const std::string& language);
	std::string get_lang_string(const std::string& key, const std::uint8_t language);

	template <typename ...Args>
	std::string get_lang_string(const std::string& key, const std::string& language, Args&&... args)
	{
		const auto text = get_lang_string(key, language);
		return std::vformat(text, std::make_format_args(args...));
	}

	template <typename ...Args>
	std::string get_lang_string(const std::string& key, const std::uint8_t language, Args&&... args)
	{
		const auto text = get_lang_string(key, language);
		return std::vformat(text, std::make_format_args(args...));
	}
}
