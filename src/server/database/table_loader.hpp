#pragma once

#include "database.hpp"

namespace database
{
	class table_interface
	{
	public:
		virtual ~table_interface()
		{
		}

		virtual void create(database_t& db)
		{
		}

		virtual void post_start(database_t& db)
		{
		}

		virtual void run_tasks(database_t& db)
		{
		}
	};

	template <typename T>
	class installer final
	{
		static_assert(std::is_base_of<table_interface, T>::value, "table has invalid base class");

	public:
		installer(int priority)
		{
			register_table(std::make_unique<T>(), priority);
		}
	};

	void register_table(std::unique_ptr<table_interface>&& component, int priority);

	struct table_def
	{
		int priority;
		std::unique_ptr<table_interface> inst;
	};

	using tables = std::vector<table_def>;
	tables& get_tables();

	std::string get_sql_query(const database::database_type_t type, const std::string& name);

	template <typename T>
	bool load_binary_field(T* dest, const std::string& data)
	{
		if (data.size() != sizeof(T))
		{
			return false;
		}

		std::memcpy(dest, data.data(), sizeof(T));
		return true;
	}
}

#define REGISTER_TABLE(name, ...)												\
namespace																		\
{																				\
	static database::installer<name> table__(__VA_ARGS__);						\
}																				\

#define DEFINE_FIELD(field_name, type)											\
struct field_name##_field_t														\
	{																			\
	struct _alias_t																\
	{																			\
		static constexpr const char _literal[] = #field_name;					\
		using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;	\
		template <typename T>													\
		struct _member_t														\
		{																		\
			T field_name;														\
			T& operator()()														\
			{																	\
				return field_name;												\
			}																	\
			const T& operator()() const											\
			{																	\
				return field_name;												\
			}																	\
		};																		\
	};																			\
																				\
	using _traits = sqlpp::make_traits<type>;									\
};																				\

#define DEFINE_TABLE(table_name, ...)											\
struct table_t : sqlpp::table_t<table_t, ##__VA_ARGS__>\
{																				\
	struct _alias_t																\
	{																			\
		static constexpr const char _literal[] = #table_name;					\
		using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;	\
		template <typename T>													\
		struct _member_t														\
		{																		\
			T table_name;														\
			T& operator()()														\
			{																	\
				return table_name;												\
			}																	\
			const T& operator()() const											\
			{																	\
				return table_name;												\
			}																	\
		};																		\
	};																			\
};																				\

#define GET_FIELD_H(__type__, __name__) \
public: \
	__type__ get_##__name__() const; \
private: \
	__type__ __name__##_; \
public: \

#define GET_FIELD_C(__class__, __type__, __name__) \
	__type__ __class__::get_##__name__() const \
	{ \
		return this->__name__##_; \
	} \

#define DEF_BINARY_GET(__table__, __type__, __name__) \
		template <database_type_t Type> \
		void get_##__name__(const std::uint64_t __table__##_id, __type__& __name__) \
		{ \
			std::memset(&__name__, 0, sizeof(__type__)); \
			database::access([&](database::database_t& db) \
			{ \
				auto results = db.get_database<Type>()->operator()( \
					sqlpp::select(__table__::table.__name__) \
							.from(__table__::table) \
								.where(__table__::table.__table__##_id == __table__##_id)); \
				if (results.empty()) \
				{ \
					return; \
				} \
				const auto data = results.front().__name__.value(); \
				load_binary_field(&__name__, data); \
			}); \
		} \

#define DEF_BINARY_SET(__table__, __type__, __name__) \
		template <database_type_t Type> \
		bool set_##__name__(const std::uint64_t __table__##id, __type__& __name__) \
		{ \
			return database::access<bool>([&](database::database_t& db) \
			{ \
				auto result = db.get_database<Type>()->operator()( \
					sqlpp::update(__table__::table) \
							.set(__table__::table.__name__ = sqlpp::verbatim<sqlpp::binary>(utils::encoding::encode_binary(__name__))) \
								.where(__table__::table.__table__##_id == __table__##id)); \
				return result != 0ull; \
			}); \
		} \

#define DEF_ARRAY_GET(__table__, __type__, __name__) \
		template <database_type_t Type> \
		void get_##__name__(const std::uint64_t __table__##_id, __type__& __name__) \
		{ \
			database::access([&](database::database_t& db) \
			{ \
				auto results = db.get_database<Type>()->operator()( \
					sqlpp::select(__table__::table.__name__) \
							.from(__table__::table) \
								.where(__table__::table.__table__##_id == __table__##_id)); \
				if (results.empty()) \
				{ \
					return; \
				} \
				const auto data = results.front().__name__.value(); \
				__name__.deserialize(data); \
			}); \
		} \

#define DEF_ARRAY_SET(__table__, __type__, __name__) \
		template <database_type_t Type> \
		bool set_##__name__(const std::uint64_t __table__##id, const __type__& __name__) \
		{ \
			return database::access<bool>([&](database::database_t& db) \
			{ \
				auto result = db.get_database<Type>()->operator()( \
					sqlpp::update(__table__::table) \
							.set(__table__::table.__name__ = sqlpp::verbatim<sqlpp::binary>(__name__.serialize())) \
								.where(__table__::table.__table__##_id == __table__##id)); \
				return result != 0ull; \
			}); \
		} \
