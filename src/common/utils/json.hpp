#pragma once

#include <glaze/glaze.hpp>
#include <glaze/glaze_exceptions.hpp>

namespace json
{
	using value = glz::generic_sorted_u64;
	using array = value::array_t;
	using object = value::object_t;
	using null = value::null_t;

	constexpr auto options = glz::opts
	{
		.error_on_unknown_keys = false,
		.skip_null_members = false
	};

	template <typename T, typename Buffer>
	inline auto read(T&& t, Buffer&& buffer)
	{
		return glz::read<options>(t, buffer);
	}

	template <typename T, typename Buffer>
	inline auto write(T&& t, Buffer&& buffer)
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
