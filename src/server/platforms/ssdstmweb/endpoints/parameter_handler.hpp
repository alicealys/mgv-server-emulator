#include "types/endpoint_handler.hpp"

namespace emulator
{
	class parameter_handler final : public endpoint_handler
	{
	public:
		parameter_handler();

		std::optional<std::string> handle_command(const utils::request_params& params) override;

	private:
		std::unordered_map<std::string, std::string> parameters_;

	};
}