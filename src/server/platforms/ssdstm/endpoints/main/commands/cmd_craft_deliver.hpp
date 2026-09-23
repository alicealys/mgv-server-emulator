#pragma once
#include "types/command_handler.hpp"

namespace emulator::ssd
{
	class cmd_craft_deliver final : public command_handler
	{
        struct param_t
        {
            std::int16_t bad_status_1_risk;
            std::int16_t bad_status_2_risk;
            std::int16_t bad_status_3_risk;
            std::int16_t bad_status_4_risk;
            std::int16_t clean_water;
            std::int16_t food;
            std::uint16_t inventory_index;
            std::uint16_t inventory_index2;
            std::uint8_t inventory_type;
            std::uint8_t inventory_type2;
            std::uint8_t list_type;
            std::int16_t medical_supplies;
            std::uint16_t num;
        };

		json::value execute(json::value& data, const std::optional<database::users::user>& user) override;
		std::uint32_t flags() override;
	};
}
