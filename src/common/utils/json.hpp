#pragma once

#include <glaze/glaze.hpp>
#include <glaze/glaze_exceptions.hpp>

namespace json
{
	using value = glz::generic_sorted_u64;
	using array = value::array_t;
	using object = value::object_t;
	using null = value::null_t;

	class result
	{
	public:
		inline result(const glz::error_ctx& error)
			: error_(error)
		{
		}

		inline auto error_message() const noexcept
		{
			return this->error_.custom_error_message;
		}

		inline auto error_code() const noexcept
		{
			return this->error_.ec;
		}

		inline operator bool() const noexcept
		{
			return !this->error_;
		}

	private:
		glz::error_ctx error_;

	};

	constexpr auto options = glz::opts
	{
		.error_on_unknown_keys = false,
		.skip_null_members = false
	};

	template <typename T, typename Buffer>
	inline result read(T&& t, Buffer&& buffer)
	{
		return glz::read<options>(t, buffer);
	}

	template <typename T, typename Buffer>
	inline result write(T&& t, Buffer&& buffer)
	{
		return glz::write<options>(t, buffer);
	}

	inline std::string dump(const value& v)
	{
		std::string result;
		glz::ex::write<options>(v, result);
		return result;
	}

	inline value parse(const std::string& data)
	{
		value result;
		glz::ex::read<options>(result, data);
		return result;
	}
}
