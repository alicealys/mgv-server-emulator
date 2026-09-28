#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_deploy_deploy final : public command_handler
	{
		struct param_t
		{
			struct team_info_t
			{
				std::uint32_t combat;
				std::uint32_t survive;
				std::uint32_t status;
				std::uint64_t index;
				std::string name;
				std::array<std::uint64_t, 4> crew_id_list;
			};

			std::uint32_t injure_rate;
			std::uint32_t success_rate;
			std::uint32_t use_fast_travel;
			std::uint32_t mission_id;
			std::uint32_t mission_info;
			std::uint32_t mission_type;
			std::uint32_t required_combat;
			std::uint32_t required_survive;
			std::uint32_t required_time;
			team_info_t team_info;
		};

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
