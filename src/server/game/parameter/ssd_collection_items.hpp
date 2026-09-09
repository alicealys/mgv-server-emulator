#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_collection_items final : public base_parameter
	{
	public:
		ssd_collection_items();
		bool parse(nlohmann::json& data) override;
	};
}
