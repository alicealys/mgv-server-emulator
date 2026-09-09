#pragma once

#include <string>
#include <unordered_map>
#include <zlib.h>

#define CHUNK 16384u

namespace utils::compression
{
	namespace zlib
	{
		std::string compress(const std::string& data, const std::uint32_t extra_bound = 0u, const std::int32_t method = Z_BEST_COMPRESSION);
		std::string decompress(const std::string& data);
	}
};
