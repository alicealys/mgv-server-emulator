#include <std_include.hpp>

#include "cmd_steam_shop_get_item_list.hpp"

#include "utils/lang_strings.hpp"

namespace emulator::ssd
{
	namespace
	{
		bool item_type_coin(const steam_shop_item_info_t& item, const database::users::user& user)
		{
			return user.add_sv_coins(item.num);
		}

		std::unordered_map<std::string, item_callback_t> item_callbacks =
		{
			{"coin", item_type_coin}
		};
	}

	std::vector<steam_shop_item_t> cmd_steam_shop_get_item_list::items;

	cmd_steam_shop_get_item_list::cmd_steam_shop_get_item_list()
	{
		auto list = utils::resources::load_json(RESOURCE_STEAM_SHOP_ITEM_LIST);
		for (auto i = 0ull; i < list.size(); i++)
		{
			steam_shop_item_t item{};
			if (!json::read(item.info, list[i]))
			{
				continue;
			}
			
			const auto iter = item_callbacks.find(item.info.type);
			if (iter == item_callbacks.end())
			{
				console::warning("[cmd_steam_shop_get_item_list] invalid item type \"%s\"\n", item.info.type.data());
				continue;
			}

			item.callback = iter->second;
			cmd_steam_shop_get_item_list::items.emplace_back(item);
		}
	}

	json::value cmd_steam_shop_get_item_list::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		param_t param{};
		if (!json::read(param, data))
		{
			return error(ERR_INVALIDARG);
		}

		for (auto i = 0ull; i < cmd_steam_shop_get_item_list::items.size(); i++)
		{
			auto& item = cmd_steam_shop_get_item_list::items[i];
			const auto text = utils::get_lang_string(item.info.text, param.lang, item.info.num);
			result["list"][i]["product_name"] = text;
			result["list"][i]["steam_item_id"] = i;
		}

		return result;
	}
}
