#include <std_include.hpp>

#include "gate_handler.hpp"

#include "commands/cmd_get_urllist.hpp"
#include "commands/cmd_get_svrlist.hpp"
#include "commands/cmd_get_svrtime.hpp"
#include "commands/cmd_maintenance_get_schedule.hpp"

namespace emulator::ssd
{
	gate_handler::gate_handler()
	{
		this->set_platform("ssdstm/gate");
		this->register_handler<cmd_get_urllist>("CMD_GET_URLLIST");
		this->register_handler<cmd_get_svrlist>("CMD_GET_SVRLIST");
		this->register_handler<cmd_get_svrtime>("CMD_GET_SVRTIME");
		this->register_handler<cmd_maintenance_get_schedule>("CMD_MAINTENANCE_GET_SCHEDULE");
	}
}
