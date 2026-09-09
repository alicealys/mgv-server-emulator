#include <std_include.hpp>

#include "ssd_collection_items.hpp"

namespace game::parameters
{
	ssd_collection_items::ssd_collection_items()
	{
		this->load("SsdCollectionItems");
	}

	bool ssd_collection_items::parse(nlohmann::json& data)
	{
		return true;
	}
}
