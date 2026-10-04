#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_get_daily_reward final : public command_handler
	{
	public:
		struct daily_reward_settings_t
		{
			struct pool_t
			{
				struct reward_t
				{
					std::uint8_t category;
					std::vector<std::uint32_t> codes;
					std::vector<std::uint32_t> nums;
				};

				std::string name;
				std::vector<reward_t> rewards;

				game::item_t roll() const;
			};

			std::size_t reward_min;
			std::size_t reward_max;
			std::uint64_t reward_interval;
			std::vector<std::shared_ptr<pool_t>> pools;
			std::vector<std::string> rewards;
		};

		cmd_get_daily_reward();
		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;

	private:
		daily_reward_settings_t settings_{};
		std::vector<std::shared_ptr<daily_reward_settings_t::pool_t>> rewards_{};

	};
}
