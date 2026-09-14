#include <std_include.hpp>

#include "cmd_purchase_get_product_list.hpp"

namespace emulator::ssd
{
	cmd_purchase_get_product_list::cmd_purchase_get_product_list()
	{
		this->list_ = resource(RESOURCE_PRODUCT_LIST);
	}

	json::value cmd_purchase_get_product_list::execute(json::value& data, const std::optional<database::users::user>& user)
	{
		json::value result;

		// TODO?
		result["product_list"] = this->list_;

		return result;
	}
}
