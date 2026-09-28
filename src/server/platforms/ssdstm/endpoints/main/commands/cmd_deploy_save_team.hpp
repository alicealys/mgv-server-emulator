#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_deploy_save_team final : public command_handler
	{
		struct entry_t
		{
			std::uint32_t combat;
			std::uint32_t index;
			std::string name;
			std::uint32_t status;
			std::uint32_t survive;
			std::array<std::uint64_t, 4> crew_id_list;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
