#include "types/endpoint_handler.hpp"

namespace emulator
{
	class parameter_handler final : public endpoint_handler
	{
	public:
		void handle_command(const utils::request_params& request, utils::response_params& response) override;

	};
}