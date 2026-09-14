#include <std_include.hpp>

#include "cmd_gdpr_check.hpp"

namespace emulator::ssd
{
	cmd_gdpr_check::cmd_gdpr_check()
	{
		this->list_ = resource(RESOURCE_GDPR_CHECK);
	}

	json::value cmd_gdpr_check::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		return this->list_;
	}
}
