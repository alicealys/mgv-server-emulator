#include <std_include.hpp>

#include "cmd_purchase_get_product_list.hpp"

namespace emulator::ssd
{
	cmd_purchase_get_product_list::cmd_purchase_get_product_list()
	{
		this->list_ = resource(RESOURCE_PRODUCT_LIST);
	}

	glz::json cmd_purchase_get_product_list::execute(glz::json& data, const std::optional<database::users::user>& user)
	{
		glz::json result;

		// TODO?
		result["product_list"] = this->list_;

		return result;
	}
}
