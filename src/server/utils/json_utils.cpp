#include <std_include.hpp>

#include "json_utils.hpp"

namespace utils::json_utils
{
	template <>
	bool is<std::int8_t>(const glz::json& value)
	{
		return value.is_int64();
	}

	template <>
	bool is<std::int16_t>(const glz::json& value)
	{
		return value.is_int64();
	}

	template <>
	bool is<std::int32_t>(const glz::json& value)
	{
		return value.is_int64();
	}

	template <>
	bool is<std::int64_t>(const glz::json& value)
	{
		return value.is_int64();
	}

	template <>
	bool is<std::uint8_t>(const glz::json& value)
	{
		return value.is_uint64();
	}

	template <>
	bool is<std::uint16_t>(const glz::json& value)
	{
		return value.is_uint64();
	}

	template <>
	bool is<std::uint32_t>(const glz::json& value)
	{
		return value.is_uint64();
	}

	template <>
	bool is<std::uint64_t>(const glz::json& value)
	{
		return value.is_uint64();
	}

	template <>
	bool is<std::string>(const glz::json& value)
	{
		return value.is_string();
	}

	template <>
	bool is<bool>(const glz::json& value)
	{
		return value.is_boolean();
	}

	template <>
	bool is<float>(const glz::json& value)
	{
		return value.is_number();
	}

	// get

	template <>
	std::int8_t get(const glz::json& value)
	{
		return value.as<std::int8_t>();
	}

	template <>
	std::int16_t get(const glz::json& value)
	{
		return value.as<std::int16_t>();
	}

	template <>
	std::int32_t get(const glz::json& value)
	{
		return value.as<std::int32_t>();
	}

	template <>
	std::int64_t get(const glz::json& value)
	{
		return value.get<std::int64_t>();
	}

	template <>
	std::uint8_t get(const glz::json& value)
	{
		return value.as<std::uint8_t>();
	}

	template <>
	std::uint16_t get(const glz::json& value)
	{
		return value.as<std::uint16_t>();
	}

	template <>
	std::uint32_t get(const glz::json& value)
	{
		return value.as<std::uint32_t>();
	}

	template <>
	std::uint64_t get(const glz::json& value)
	{
		return value.get<std::uint64_t>();
	}

	template <>
	bool get(const glz::json& value)
	{
		return value.get<bool>();
	}

	template <>
	float get(const glz::json& value)
	{
		return value.as<float>();
	}

	template <>
	std::string get(const glz::json& value)
	{
		return value.get<std::string>();
	}
}
