#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_purchase_get_product_list final : public command_handler
	{
	public:
		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;

	};
}
