#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_information_get_title final : public command_handler
	{
	public:
		cmd_information_get_title();
		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;

		struct message_t
		{
			std::string title;
			std::string text;
			std::string url;
			bool important;
			std::uint8_t type;
			std::uint64_t update_date;
			std::uint64_t release_date;
			std::uint32_t info_id;

			std::uint64_t get_release_date() const;
			std::uint64_t get_update_date() const;
			std::string format_text(const std::uint8_t lang) const;
			std::string format_title(const std::uint8_t lang) const;
			void to_json(json::value& data, const std::uint8_t lang) const;
		};

		static std::vector<message_t> messages;
		static std::unordered_map<std::string, std::function<std::string()>> message_vars;

	private:
		struct param_t
		{
			std::uint8_t lang;
			std::uint8_t region;
			std::uint8_t is_note;
			std::size_t start;
			std::size_t num;
		};
		
		template <typename F>
		static void register_message_var(const std::string& name, F&& cb)
		{
			message_vars.insert(std::make_pair(name, [=]()
			{
				return std::format("{}", cb());
			}));
		}

		static std::string format_message(const std::string& text, const std::uint8_t lang);

	};
}
