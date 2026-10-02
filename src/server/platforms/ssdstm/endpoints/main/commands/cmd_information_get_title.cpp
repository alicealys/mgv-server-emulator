#include <std_include.hpp>

#include "cmd_information_get_title.hpp"

#include "utils/lang_strings.hpp"
#include "server.hpp"

#include <version.h>

namespace emulator::ssd
{
	std::vector<cmd_information_get_title::message_t> cmd_information_get_title::messages;
	std::unordered_map<std::string, std::function<std::string()>> cmd_information_get_title::message_vars;


	std::string cmd_information_get_title::format_message(const std::string& text, const std::uint8_t lang)
	{
		// maybe use fmt for this

		const auto lang_text = utils::get_lang_string(text, lang);

		std::string formatted;
		std::string current_key;

		auto in_key = false;
		auto in_ignore = false;
		for (auto i = 0ull; i < lang_text.size(); i++)
		{
			if (lang_text[i] == '{')
			{
				if (!in_key)
				{
					in_key = true;
					continue;
				}
				else if (!in_ignore)
				{
					in_ignore = true;
				}
			}
			else if (lang_text[i] == '}')
			{
				if (in_key)
				{
					if (!current_key.empty())
					{
						const auto iter = cmd_information_get_title::message_vars.find(current_key);
						if (iter != cmd_information_get_title::message_vars.end())
						{
							formatted.append(iter->second());
						}
						current_key.clear();
					}

					in_key = false;
					continue;
				}
				else if (in_ignore)
				{
					in_ignore = false;
				}
			}

			if (in_key && !in_ignore)
			{
				current_key.push_back(lang_text[i]);
			}
			else
			{
				formatted.push_back(lang_text[i]);
			}
		}

		if (!current_key.empty())
		{
			formatted.append(current_key);
			current_key.clear();
		}
		
		return formatted;
	}
	
	std::uint64_t cmd_information_get_title::message_t::get_release_date() const
	{
		if (this->release_date == 0)
		{
			return std::time(nullptr);
		}
		else
		{
			return this->release_date;
		}
	}

	std::uint64_t cmd_information_get_title::message_t::get_update_date() const
	{
		if (this->update_date == 0)
		{
			return std::time(nullptr);
		}
		else
		{
			return this->update_date;
		}
	}

	std::string cmd_information_get_title::message_t::format_title(const std::uint8_t lang) const
	{
		return cmd_information_get_title::format_message(this->title, lang);
	}

	std::string cmd_information_get_title::message_t::format_text(const std::uint8_t lang) const
	{
		return cmd_information_get_title::format_message(this->text, lang);
	}

	void cmd_information_get_title::message_t::to_json(json::value& data, const std::uint8_t lang) const
	{
		data["title"] = this->format_title(lang);
		data["important"] = std::uint8_t(this->important);
		data["personal"] = 0;
		data["type"] = this->type;
		data["info_id"] = this->info_id;
		data["update_date"] = this->get_update_date();
		data["release_date"] = this->get_release_date();
	}

	cmd_information_get_title::cmd_information_get_title()
	{
		this->register_message_var("online_players", static_cast<std::uint64_t(*)()>(database::users::get_online_user_count));
		this->register_message_var("total_players", database::users::get_user_count);

		this->register_message_var("version", []()
		{
			return VERSION;
		});

		auto list = utils::resources::load_json(RESOURCE_INFORMATION_LIST);
		for (auto i = 0ull; i < list.size(); i++)
		{
			message_t message{};
			if (!json::read(message, list[i]))
			{
				continue;
			}

			this->messages.emplace_back(message);
		}
	}

	json::value cmd_information_get_title::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;
		
		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		auto count = 0u;
		const auto end = std::min(cmd_information_get_title::messages.size(), param.start + param.num);
		for (auto i = param.start; i < end; i++)
		{
			const auto& message = cmd_information_get_title::messages[i];
			message.to_json(result["info_list"][count++], param.lang);
		}

		result["info_num"] = count;

		return result;
	}
}
