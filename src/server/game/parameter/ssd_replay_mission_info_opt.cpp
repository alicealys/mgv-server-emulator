#include <std_include.hpp>

#include "ssd_replay_mission_info_opt.hpp"

namespace game::parameters
{
	ssd_replay_mission_info_opt::ssd_replay_mission_info_opt()
	{
		this->load("SsdReplayMissionInfo_opt");
	}

	bool ssd_replay_mission_info_opt::parse(nlohmann::json& data)
	{
		return true;
	}
}
