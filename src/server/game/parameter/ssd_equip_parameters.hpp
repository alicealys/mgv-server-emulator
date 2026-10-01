#pragma once

#include "base_parameter.hpp"

namespace game::parameters
{
	class ssd_equip_parameters final : public base_parameter
	{
	public:
		ssd_equip_parameters();
		bool parse(json::value& data) override;

		std::unordered_map<std::uint32_t, std::shared_ptr<equip_t>> equips;

	};
}
