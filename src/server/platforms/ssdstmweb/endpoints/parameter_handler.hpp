#include "types/endpoint_handler.hpp"

namespace emulator
{
	class parameter_handler final : public endpoint_handler
	{
	public:
		parameter_handler();

		void handle_command(const utils::request_params& request, utils::response_params& response) override;

	private:
		std::unordered_map<std::string, std::string> parameters_;

	};
}