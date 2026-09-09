#pragma once

#include "http_server.hpp"
#include <utils/cryptography.hpp>

namespace utils::encoding
{
	std::string split_into_lines(const std::string& string, const size_t chars_per_line = 76);
	std::string unescape_json(const std::string& str);
	std::string decode_url_string(const std::string& str);

	inline void byte_to_hex(const std::uint8_t value, char* out)
	{
		static const char* map = "0123456789ABCDEF";
		out[1] = map[value & 0xF];
		out[0] = map[(value >> 4) & 0xF];
	}

	std::string encode_as_hex(const char* buffer, const size_t size);
	std::string encode_as_hex(const std::string& data);

	template <typename T>
	inline void encode_as_hex_single(const T* data, char* out)
	{
		const auto buffer = reinterpret_cast<const std::uint8_t*>(data);
		for (auto i = 0ull; i < sizeof(T); i++)
		{
			byte_to_hex(buffer[i], out);
			out += 2;
		}
	}

	template <typename T>
	std::string encode_binary(const T& data)
	{
		const auto buffer = reinterpret_cast<char*>(reinterpret_cast<size_t>(&data));
		return encode_as_hex(buffer, sizeof(T));
	}

	template <typename T>
	std::string encode_base64(const T& data)
	{
		const auto buffer = reinterpret_cast<std::uint8_t*>(reinterpret_cast<size_t>(&data));
		return utils::cryptography::base64::encode(buffer, sizeof(T));
	}

	template <typename T, std::size_t SizeAdd, std::size_t MaxSize>
	class database_array : private std::vector<T>
	{
		static_assert(std::is_trivially_copyable<T>::value, "type must be trivially copyable");
	public:
		using std::vector<T>::data;
		using std::vector<T>::size;
		using std::vector<T>::resize;
		using std::vector<T>::at;
		using std::vector<T>::operator[];

		virtual ~database_array()
		{
		}

		virtual bool deserialize(const std::string& data)
		{
			if ((data.size() % sizeof(T)) != 0)
			{
				return false;
			}

			const auto size = data.size() / sizeof(T);
			this->resize(std::min(MaxSize, size + SizeAdd));
			std::memcpy(this->data(), data.data(), data.size());
			return true;
		}

		virtual inline bool skip_element(const T& element) const
		{
			return false;
		}

		virtual std::string serialize() const
		{
			std::string buffer;
			buffer.resize(this->size() * sizeof(T) * 2 + 3);

			auto serialized_data = buffer.data();
			const auto data = this->data();

			serialized_data[0] = 'x';
			serialized_data[1] = '\'';
			serialized_data += 2;

			for (auto i = 0ull; i < this->size(); i++)
			{
				if (this->skip_element(data[i]))
				{
					continue;
				}

				encode_as_hex_single(&data[i], serialized_data);
				serialized_data += sizeof(T) * 2;
			}

			*serialized_data = '\'';

			auto end_size = (serialized_data + 1) - buffer.data();
			buffer.resize(end_size);

			return buffer;
		}

		bool is_full() const
		{
			return this->size() >= MaxSize;
		}

		std::size_t max_size() const
		{
			return MaxSize;
		}

		bool push(const T& v)
		{
			if (this->is_full())
			{
				return false;
			}

			std::vector<T>::emplace_back(v);
			return true;
		}

	};
}
