#pragma once

namespace emulator
{
	template <typename HandlerType>
	class base_handler
	{
	public:
		virtual ~base_handler()
		{
		}

		template <typename T, typename... Args>
		void register_handler(const std::string_view& name, Args&&... args)
		{
			this->print_handler_name(name);
			auto handler = std::make_unique<T>(std::forward<Args>(args)...);
			this->handlers_.insert(std::make_pair(name, std::move(handler)));
		}

		virtual void print_handler_name([[ maybe_unused ]] const std::string_view& name)
		{

		}

	protected:
		std::unordered_map<std::string_view, std::unique_ptr<HandlerType>> handlers_;
	};
}
