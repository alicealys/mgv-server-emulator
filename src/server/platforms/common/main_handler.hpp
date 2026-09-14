#pragma once

#include "types/endpoint_handler.hpp"

#include <utils/cryptography.hpp>

namespace emulator
{
	class main_handler : public endpoint_handler
	{
	public:
		main_handler();

		std::optional<json::value> decrypt_request(const std::string& data,
			std::optional<database::users::user>& player) override;
		bool verify_request(json::value& request) override;
		bool encrypt_response(json::value& request, json::value& data,
			const std::optional<database::users::user>& player, std::string& result) override;

	private:
		utils::cryptography::blowfish blow_;

	};
}
