#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_collection_items : public base_parameter
	{
	public:
		ssd_collection_items();
		bool parse(const nlohmann::json& data) override;
	};
}
