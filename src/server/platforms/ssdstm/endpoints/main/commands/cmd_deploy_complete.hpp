#pragma once
#include "types/command_handler.hpp"

#include "database/models/deployments.hpp"

namespace emulator::ssd
{
	class cmd_deploy_complete final : public command_handler
	{
	public:
		static bool complete_mission(const database::users::user& user, const database::deployments::deployment_team& team, const bool is_forced, json::value& result);
		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
