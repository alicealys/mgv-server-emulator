#include <std_include.hpp>

#include "cmd_challenge_task_complete.hpp"

#include "database/models/challenge_tasks.hpp"
#include "database/models/present_box.hpp"

namespace emulator::ssd
{
	json::value cmd_challenge_task_complete::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		std::uint32_t id{};
		if (!json::read(id, data["id"]))
		{
			return error(ERR_INVALIDARG);
		}

		const auto order = database::challenge_tasks::find_order(user->current_player->get_player_id(), id);
		if (!order.has_value())
		{
			return error(ERR_NOT_FOUND);
		}

		if (!order->complete())
		{
			return error(ERR_DATABASE);
		}

		const auto text_id = 82513511927685;
		const auto now = std::chrono::system_clock::now();
		const auto now_s = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
		const auto expire_date = now_s + 14 * 24h;

		auto& reward_info = result["reward_info"];
		reward_info["energy"] = 0;
		reward_info["expire_date"] = 0;
		reward_info["kub_boost_flag"] = 0;
		reward_info["present_box_num"] = 0;
		reward_info["battle_pack_list"] = json::array();
		reward_info["nonstackable_list"] = json::array();
		reward_info["recipe_list"] = json::array();
		reward_info["resources_list"] = json::array();
		reward_info["stackable_list"] = json::array();
		reward_info["text_id"] = text_id;
		reward_info["expire_date"] = expire_date.count();

		auto& present_list = reward_info["present_list"];
		present_list = json::array();

		database::players::battle_pack_list_t battle_pack_list{};

		user->current_player->get_battle_pack_list(battle_pack_list);

		database::users::give_item_params_t give_params{};
		give_params.battle_pack_list = &battle_pack_list;
		give_params.text_id = text_id;

		const auto rewards = database::challenge_tasks::get_task_rewards(id);
		for (auto i = 0ull; i < rewards.size(); i++)
		{
			if (!user->give_item(rewards[i], give_params, reward_info))
			{
				database::present_box::add_item(user->get_user_id(),
					database::present_box::present_flag_expire, 
					expire_date, rewards[i], text_id);
				rewards[i].to_json(present_list[present_list.size()]);
			}
		}

		user->current_player->set_battle_pack_list(battle_pack_list);

		return result;
	}

	std::uint32_t cmd_challenge_task_complete::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
