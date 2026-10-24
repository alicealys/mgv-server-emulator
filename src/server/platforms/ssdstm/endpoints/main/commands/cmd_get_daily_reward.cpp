#include <std_include.hpp>

#include "cmd_get_daily_reward.hpp"

#include "database/models/present_box.hpp"

#include "utils/random.hpp"
#include "utils/time.hpp"

namespace emulator::ssd
{
	game::item_t cmd_get_daily_reward::daily_reward_settings_t::pool_t::roll() const
	{
		const auto reward_idx = utils::random::index(this->rewards);
		const auto& reward = this->rewards[reward_idx];
		const auto code_idx = utils::random::index(reward.codes);
		const auto num_idx = utils::random::index(reward.nums);

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

		database::users::reward_info_t reward_info{};

		constexpr const auto text_id = game::lang_id_login_bonus;

		std::vector<game::item_t> presents;

		if (is_reward)
		{
			reward_info.text_id = text_id;
			reward_info.expire_date = utils::time::from_now(14d);
			
			std::unordered_set<std::uint32_t> added_codes;

			const auto reward_count = utils::random::get_integer(this->settings_.reward_min, this->settings_.reward_max);
			for (auto i = 0ull; i < reward_count; i++)
			{
				const auto& pool = this->rewards_[i];
				auto item = pool->roll();
				
				auto attempts = 8;
				while (item.code != 0 && added_codes.contains(item.code) && --attempts > 0)
				{
					item = pool->roll();
				}

				added_codes.insert(item.code);
				reward_info.present_list.emplace_back(item);
			}

			database::present_box::add_items(user->get_user_id(),
				database::present_box::present_flag_expire,
				reward_info.expire_date, reward_info.present_list, text_id);

			user->set_daily_reward();
		}

		reward_info.present_box_num = database::present_box::get_present_count(user->get_user_id());
		reward_info.to_json(result["reward_info"]);

		return result;
	}

	std::uint32_t cmd_get_daily_reward::flags()
	{
		return CMD_NEEDS_USER | CMD_NEEDS_PLAYER;
	}
}
