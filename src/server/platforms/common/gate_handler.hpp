#pragma once

#include "types/endpoint_handler.hpp"

#include <utils/cryptography.hpp>

namespace emulator
{
	class gate_handler : public endpoint_handler
	{
	public:
		gate_handler();

		std::optional<json::value> decrypt_request(const std::string& data, std::optional<database::users::user>& player) override;
		bool verify_request(json::value& request) override;
		std::optional<std::string> encrypt_response(json::value& request, json::value& data,
			const std::optional<database::users::user>& player) override;

	private:
		utils::cryptography::blowfish blow_;

	};
}