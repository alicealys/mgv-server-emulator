#include <std_include.hpp>

#include "cmd_purchase_get_product_list.hpp"

namespace emulator::ssd
{
	cmd_purchase_get_product_list::cmd_purchase_get_product_list()
	{
		this->list_ = resource(RESOURCE_PRODUCT_LIST);
	}

	nlohmann::json cmd_purchase_get_product_list::execute(nlohmann::json& data, const std::optional<database::users::user>& user)
	{
		nlohmann::json result;

		// TODO?
		result["product_list"] = this->list_;

		return result;
	}
}
