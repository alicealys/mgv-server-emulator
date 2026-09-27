#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	struct steam_shop_item_info_t
	{
		std::string text;
		std::string type;
		std::uint32_t num;
		std::uint32_t param1;
		std::uint32_t param2;
		std::uint32_t param3;
		std::uint32_t param4;
		std::uint32_t param5;
		std::uint32_t param6;
	};

	using item_callback_t = bool(*)(const steam_shop_item_info_t&, const database::users::user&);

	struct steam_shop_item_t
	{
		steam_shop_item_info_t info;
		item_callback_t callback;
	};

	class cmd_steam_shop_get_item_list final : public command_handler
	{
	public:
		struct param_t
		{
			std::string lang;
			std::uint32_t region;
			std::uint32_t version;
		};

		cmd_steam_shop_get_item_list();
		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;

		static std::vector<steam_shop_item_t> items;
	};
}
