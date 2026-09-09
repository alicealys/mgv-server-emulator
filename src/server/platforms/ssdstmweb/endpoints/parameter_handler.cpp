#include <std_include.hpp>

#include "parameter_handler.hpp"
#include "game/parameters.hpp"

#include <utils/io.hpp>
#include <utils/cryptography.hpp>
#include <utils/compression.hpp>

namespace emulator
{
	void parameter_handler::handle_command(const utils::request_params& request, utils::response_params& response)
	{
		static const auto prefix = "/ssdstmweb/parameter/";
		const auto pos = request.uri.find(prefix);
		if (pos == std::string::npos)
		{
			return;
		}

		static const auto start = std::strlen(prefix);
		const auto name = request.uri.substr(start);
		const auto end_pos = name.find_last_of('.');
		const auto base_name = name.substr(0, end_pos);

		console::debug("[Endpoint] Sending parameter %s\n", name.data());

		const auto iter = game::parameters_map.find(base_name);
		if (iter == game::parameters_map.end())
		{
			return;
		}

		response.body = iter->second->get_data_path();
		response.type = utils::response_file;
	}
}
