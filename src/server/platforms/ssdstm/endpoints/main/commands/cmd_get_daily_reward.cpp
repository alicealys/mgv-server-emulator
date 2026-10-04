#include <std_include.hpp>

#include "cmd_get_daily_reward.hpp"

#include "database/models/present_box.hpp"

namespace emulator::ssd
{
	game::item_t cmd_get_daily_reward::daily_reward_settings_t::pool_t::roll() const
	{
		const auto reward_idx = utils::cryptography::random::get_integer64() % this->rewards.size();
		const auto& reward = this->rewards[reward_idx];
		const auto code_idx = utils::cryptography::random::get_integer64() % reward.codes.size();
		const auto num_idx = utils::cryptography::random::get_integer64() % reward.nums.size();

		game::item_t item{};
		item.category = reward.category;
		item.code = reward.codes[code_idx];
		item.num = reward.nums[num_idx];

		return item;
	}

	cmd_get_daily_reward::cmd_get_daily_reward()
	{
		auto settings = utils::resources::load_json(RESOURCE_DAILY_REWARD_SETTINGS);

		if (!json::read(this->settings_, settings))
		{
			throw std::runtime_error("[daily reward] failed to parse settings json");
		}

		this->settings_.reward_min = std::min(this->settings_.rewards.size(), this->settings_.reward_min);
		this->settings_.reward_max = std::min(this->settings_.rewards.size(), this->settings_.reward_max);

		if (this->settings_.reward_min > this->settings_.reward_max)
		{
			throw std::runtime_error("[daily reward] invalid reward_min");
		}

		for (const auto& reward_type : this->settings_.rewards)
		{
			const auto iter = std::ranges::find_if(this->settings_.pools.begin(), this->settings_.pools.end(), 
				[&](const std::shared_ptr<daily_reward_settings_t::pool_t>& pool)
				{
					return pool->name == reward_type;
				}
			);

			if (iter == this->settings_.pools.end())
			{
				throw std::runtime_error(std::format("[daily reward] invalid reward type {}", reward_type));
			}

			this->rewards_.emplace_back(*iter);
		}
	}

	json::value cmd_get_daily_reward::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		const auto now = std::chrono::system_clock::now();
		const auto now_s = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());

		auto is_reward = false;
		std::chrono::seconds next_date = user->get_last_daily_reward() + this->settings_.reward_interval * 1s;

		if (user->get_last_daily_reward() < now_s)
		{
			const auto diff = now_s - user->get_last_daily_reward();
			const auto diff_s = std::chrono::duration_cast<std::chrono::seconds>(diff);
			is_reward = diff_s >= this->settings_.reward_interval * 1s;
			next_date = now_s + this->settings_.reward_interval * 1s;
		}

		result["next_date"] = next_date.count();
		result["is_reward"] = std::uint8_t(is_reward);

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
		reward_info["text_id"] = 0;

		auto& present_list = reward_info["present_list"];
		present_list = json::array();

		const auto text_id = 181130097427538;

		if (is_reward)
		{
			const auto expire_date = now_s + 14 * 24h;
			reward_info["text_id"] = text_id;
			reward_info["expire_date"] = expire_date.count();
			
			std::unordered_set<std::uint32_t> added_codes;

			const auto reward_count = utils::cryptography::random::get_integer64(this->settings_.reward_min, this->settings_.reward_max);
			for (auto i = 0ull; i < reward_count; i++)
			{
				const auto& pool = this->rewards_[i];
				auto item = pool->roll();

				while (item.code != 0 && added_codes.contains(item.code))
				{
					item = pool->roll();
				}

				added_codes.insert(item.code);

				database::present_box::add_item(user->current_player->get_player_id(), 
					database::present_box::present_flag_new | database::present_box::present_flag_expire,
					expire_date, item, text_id);
				item.to_json(present_list[present_list.size()]);
			}

			user->set_daily_reward();
		}

		reward_info["present_box_num"] = database::present_box::get_present_count(user->current_player->get_player_id());

		return result;
	}

	std::uint32_t cmd_get_daily_reward::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
