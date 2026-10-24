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

		database::users::reward_info_t reward_info{};
		reward_info.text_id = game::lang_id_order_completion_reward;
		reward_info.expire_date = utils::time::from_now(14d);

		database::players::battle_pack_list_t battle_pack_list{};

		user->current_player->get_battle_pack_list(battle_pack_list);

		database::users::give_item_params_t give_params{};
		give_params.battle_pack_list = &battle_pack_list;
		give_params.text_id = reward_info.text_id;

		const auto rewards = database::challenge_tasks::get_task_rewards(id);
		for (auto i = 0ull; i < rewards.size(); i++)
		{
			if (!user->give_item(rewards[i], give_params, &reward_info))
			{
				reward_info.present_list.emplace_back(rewards[i]);
			}
		}

		database::present_box::add_items(user->get_user_id(),
			database::present_box::present_flag_expire,
			reward_info.expire_date, reward_info.present_list, reward_info.text_id);

		reward_info.to_json(result["reward_info"]);

		user->current_player->set_battle_pack_list(battle_pack_list);

		return result;
	}

	std::uint32_t cmd_challenge_task_complete::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
