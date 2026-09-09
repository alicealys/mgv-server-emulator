#pragma once

#include "../game.hpp"

namespace game::parameters
{
	class base_parameter : std::enable_shared_from_this<base_parameter>
	{
	public:
		virtual ~base_parameter()
		{
		}

		const std::string& get_data_path();
		const std::string& get_data_md5();
		const std::string& name();

	protected:
		void load(const std::string& name);
		virtual bool parse(nlohmann::json& data) = 0;

	private:
		std::string data_path_;
		std::string data_md5_;
		std::string name_;

	};
}
