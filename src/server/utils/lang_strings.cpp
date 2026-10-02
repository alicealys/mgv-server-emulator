#include <std_include.hpp>

#include "lang_strings.hpp"
#include "resources.hpp"

namespace utils
{
	std::unordered_map<std::string, std::uint8_t> language_code_map =
	{
		{"ANY", LANG_ANY},
		{"KR", LANG_KR},
		{"CH", LANG_CH},
		{"JP", LANG_JP},
		{"RU", LANG_RU},
		{"AR", LANG_AR},
		{"GR", LANG_GR},
		{"IT", LANG_IT},
		{"PR", LANG_PR},
		{"SP", LANG_SP},
		{"FR", LANG_FR},
		{"EN", LANG_EN},
	};

	std::unordered_map<std::string, lang_string_t> load_strings()
	{
		std::unordered_map<std::string, lang_string_t> map;

		auto list = resources::load_json(RESOURCE_LANG_STRINGS);
		for (auto& entry : list.get_object())
		{
			lang_string_t string;
			for (auto& lang : entry.second.get_object())
			{
				const auto lang_code = language_code_map.find(lang.first);
				if (lang_code == language_code_map.end())
				{
					continue;
				}

				string.value[lang_code->second] = lang.second.get_string();
			}

			map.insert(std::make_pair(entry.first, string));
		}

		return map;
	}

	std::string get_lang_string(const std::string& key, const std::uint8_t language)
	{
		const auto string = get_strings().find(key);
		if (string == get_strings().end())
		{
			return key;
		}

		auto lang_idx = language;
		if (lang_idx >= LANG_COUNT)
		{
			lang_idx = LANG_ANY;
		}

		const auto& value = string->second.value[lang_idx];
		const auto& text = value.empty()
			? string->second.value[LANG_ANY]
			: value;

		return text;
	}

	std::string get_lang_string(const std::string& key, const std::string& language)
	{
		std::uint8_t lang_code = LANG_ANY;
		const auto lang = language_code_map.find(language);
		if (lang != language_code_map.end())
		{
			lang_code = lang->second;
		}

		return get_lang_string(key, lang->second);
	}

	const std::unordered_map<std::string, lang_string_t>& get_strings()
	{
		static const auto map = load_strings();
		return map;
	}
}
