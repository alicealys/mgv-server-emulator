#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_inventory_save final : public command_handler
	{
	public:
		static void do_save(nlohmann::json& data, const std::optional<database::users::user>& user);
		nlohmann::json execute(nlohmann::json& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
