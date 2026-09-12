#pragma once

#include "utils/encoding.hpp"

namespace database
{
	template <typename T, std::size_t MaxSize>
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

		virtual bool deserialize(const std::string& data, const std::size_t add_size = 0ull)
		{
			if ((data.size() % sizeof(T)) != 0)
			{
				return false;
			}

			const auto size = data.size() / sizeof(T);
			this->resize(std::min(MaxSize, size + add_size));
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

				utils::encoding::encode_as_hex_single(&data[i], serialized_data);
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

	template <typename S, typename T>
	std::size_t get_offset(T S::* field)
	{
		S s{};
		return reinterpret_cast<std::size_t>(&(s.*field)) - reinterpret_cast<std::size_t>(&s);
	}

	template <typename S, typename T>
	bool set_blob_value_sqlite(database_t& db, const std::string& table, const std::uint64_t id, const std::string& column, T S::* field, const T& data)
	{
		const auto offset = get_offset(field);
		const auto& db_name = database::get_database_name();
		sqlite3_blob* blob{};
		sqlite3_blob_open(db.get_sqlite3()->native_handle(), db_name.data(), table.data(), column.data(), id, 1, &blob);
		return sqlite3_blob_write(blob, reinterpret_cast<void*>(&data), static_cast<int>(sizeof(T)), static_cast<int>(offset)) == SQLITE_OK;
	}

#ifdef MYSQL_SUPPORTED
	template <typename S, typename T>
	bool set_blob_value_mysql(database_t& db, const std::string& table, const std::uint64_t id, const std::string& column, T S::* field, const T& data)
	{
		const auto offset = get_offset(field);
		char encoded_data[sizeof(T) * 2 + 1]{};
		utils::encoding::encode_as_hex_single(&data, encoded_data);
		const auto query = std::format("update {} set {} = insert({}, {}, {}, x'{}') where _rowid = {}",
			table, column, column, offset + 1, sizeof(T), encoded_data, id);
		return mysql_query(db.get_mysql()->native_handle(), query.data()) == 0;
	}
#endif

	template <typename S, typename T>
	bool set_blob_value(database_t& db, const std::string& table, const std::uint64_t id, const std::string& column, T S::* field, const T& data)
	{
#ifdef MYSQL_SUPPORTED
		if (get_database_type() == database_mysql)
		{
			return set_blob_value_mysql(db, table, id, column, field, data);
		}
#endif

		if (get_database_type() == database_sqlite3)
		{
			return set_blob_value_mysql(db, table, id, column, field, data);
		}

		throw std::runtime_error("utils::set_binary_field invalid database type");
	}
}
