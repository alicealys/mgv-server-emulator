#include <std_include.hpp>

#include "ssd_equip_parameters.hpp"

namespace game::parameters
{
	ssd_equip_parameters::ssd_equip_parameters()
	{
		this->load("SsdEquipParameters");
	}

	bool ssd_equip_parameters::parse(json::value& data)
	{
		for (auto i = 0ull; i < data["equipId"].size(); i++)
		{
			auto equip = std::make_shared<equip_t>();
			if (!equip->parse(data["equipId"][i]))
			{
				continue;
			}

			this->equips[equip->id] = equip;
		}

		return true;
	}
}
