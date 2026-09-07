#include "types/endpoint_handler.hpp"

namespace emulator
{
	class var_handler final : public endpoint_handler
	{
	public:
		var_handler(const std::string& name, const std::string& value);

		void handle_command(const utils::request_params& request, utils::response_params& response) override;

	private:
		std::string name_;
		std::string value_;

	};
}