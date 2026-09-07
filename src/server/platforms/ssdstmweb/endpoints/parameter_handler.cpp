#include <std_include.hpp>

#include "parameter_handler.hpp"

#include <utils/io.hpp>

namespace emulator
{
	parameter_handler::parameter_handler()
	{
		const auto base_url = config::get<std::string>("base_url");
		auto list = resource(RESOURCE_SERVER_PARAMETER_LIST);

		for (auto i = 0ull; i < list["file_list"].size(); i++)
		{
			const auto name = list["file_list"][i].get<std::string>();
			const auto path = std::format("resources/data/parameter/{}", name);
			const auto resource_id = utils::resources::get_resource_id(path);
			if (!resource_id.has_value())
			{
				continue;
			}

			const auto temp_path = std::format("tmp/{}", path);
			const auto data = utils::resources::load(resource_id.value());
			utils::io::write_file(temp_path, data);
			this->parameters_.insert(std::make_pair(name, temp_path));
		}
	}

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

		console::debug("[Endpoint] Sending parameter %s\n", name.data());

		const auto iter = this->parameters_.find(name);
		if (iter == this->parameters_.end())
		{
			return;
		}

		response.body = iter->second;
		response.type = utils::response_file;
	}
}
