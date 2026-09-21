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
		using std::vector<T>::begin;
		using std::vector<T>::end;

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


	template <typename T, std::size_t MaxSize = 1024>
	class generic_item_list : public database_array<T, MaxSize>
	{
	public:
		bool try_add_item(const T& item, const bool overwrite_existing = true)
		{
			std::int64_t free_index = -1;
			for (auto o = 0ull; o < this->size(); o++)
			{
				auto& entry = this->operator[](o);
				if (this->are_elements_equal(item, entry))
				{
					if (overwrite_existing)
					{
						this->import_element(entry, item);
					}
					return true;
				}
				else if (this->is_element_empty(entry) && free_index == -1)
				{
					free_index = static_cast<std::int64_t>(o);
				}
			}

			if (free_index != -1)
			{
				this->import_element(this->operator[](free_index), item);
				return true;
			}

			return this->push(item);
		}

		virtual bool parse_diff(json::value& data)
		{
			if (!data.is_array())
			{
				return false;
			}

			const auto count = std::min(data.size(), this->max_size());
			for (auto i = 0ull; i < count; i++)
			{
				T new_item{};
				if (!new_item.parse(data[i]))
				{
					continue;
				}

				if (!try_add_item(new_item))
				{
					break;
				}
			}

			return true;
		}

		virtual bool parse(json::value& data)
		{
			std::memset(this->data(), 0, this->size() * sizeof(T));

			if (!data.is_array())
			{
				return false;
			}

			const auto count = std::min(this->max_size(), data.size());
			this->resize(count);

			for (auto i = 0ull; i < count; i++)
			{
				this->operator[](i).parse(data[i]);
			}

			return true;
		}

		virtual void to_json(json::value& data) const
		{
			auto idx = 0;
			data = json::array();

			const auto list = this->data();
			for (auto i = 0ull; i < this->size(); i++)
			{
				if (this->is_element_empty(list[i]))
				{
					continue;
				}

				list[i].to_json(data[idx++]);
			}
		}

		virtual inline bool is_element_empty(const T& value) const
		{
			return false;
		}

		virtual inline bool are_elements_equal(const T& l, const T& r) const
		{
			return false;
		}
		
		virtual inline void import_element(T& dest, const T& src) const
		{
			std::memcpy(&dest, &src, sizeof(T));
		}

		inline bool skip_element(const T& value) const override
		{
			return this->is_element_empty(value);
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
