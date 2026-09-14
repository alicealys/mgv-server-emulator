#pragma once

#include <utils/cryptography.hpp>
#include "encoding.hpp"

namespace utils::json_utils
{
	template <typename T>
	bool is(const glz::json& value);

	template <typename T>
	T get(const glz::json& value);

	template <typename T>
	void get_or(const glz::json& value, T& dest, const T& default_value = {})
	{
		if (!is<T>(value))
		{
			dest = default_value;
		}
		else
		{
			dest = get<T>(value);
		}
	}

	template <typename T>
	bool parse_array(glz::json& src, T& dest, const bool strict = true)
	{
		if (!src.is_array() || (strict && src.size() != ARRAYSIZE(dest)))
		{
			return false;
		}

		for (auto i = 0ull; i < src.size(); i++)
		{
			get_or(src[i], dest[i]);
		}

		return true;
	}

	template <typename T, typename F>
	bool parse_array(glz::json& src, T& dest, F&& fn, const bool strict = true)
	{
		if (!src.is_array() || (strict && src.size() != ARRAYSIZE(dest)))
		{
			return false;
		}

		const auto count = std::min(src.size(), ARRAYSIZE(dest));
		for (auto i = 0ull; i < count; i++)
		{
			fn(dest[i], src[i]);
		}

		return true;
	}

	template <std::size_t N>
	bool parse_base64(glz::json& src, std::uint8_t(&dest)[N], const bool strict = false)
	{
		if (!src.is_string())
		{
			return false;
		}

		auto data = utils::encoding::decode_url_string(src.get<std::string>());
		data = utils::cryptography::base64::decode(data);

		if ((strict && data.size() != N) || (!strict && data.size() > N))
		{
			return false;
		}

		std::memcpy(dest, data.data(), data.size());
		return true;
	}

	template <std::size_t N>
	bool parse_string(glz::json& src, char(&dest)[N])
	{
		if (!src.is_string())
		{
			return false;
		}

		const auto& string = src.get<std::string>();
		if (string.size() > N - 1)
		{
			return false;
		}

		std::memcpy(dest, string.data(), string.size());
		return true;
	}
}
