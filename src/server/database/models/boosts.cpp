#include <std_include.hpp>

#include "boosts.hpp"
#include "users.hpp"

#include <utils/string.hpp>

namespace database::boosts
{
	namespace alias
	{
		SQLPP_ALIAS_PROVIDER(duration);
		SQLPP_ALIAS_PROVIDER(expire_date);
		SQLPP_ALIAS_PROVIDER(mag1);
		SQLPP_ALIAS_PROVIDER(mag2);
	}

	GET_FIELD_C(boost, std::uint64_t, boost_id);
	GET_FIELD_C(boost, std::uint64_t, user_id);
	GET_FIELD_C(boost, std::uint8_t, boost_type);
	GET_FIELD_C(boost, std::uint32_t, duration);
	GET_FIELD_C(boost, std::uint32_t, mag1);
	GET_FIELD_C(boost, std::uint32_t, mag2);
	GET_FIELD_C(boost, std::chrono::seconds, expire_date);

	void boost::to_json(json::value& data) const
	{
		data["expire_date"] = this->expire_date_.count();
		data["mag1"] = this->mag1_;
		data["mag2"] = this->mag2_;
		data["time"] = this->duration_;
		data["type"] = this->boost_type_;
	}

	void boost_params_t::to_json(json::value& data) const
	{
		data["expire_date"] = this->expire_date.count();
		data["mag1"] = this->mag1;
		data["mag2"] = this->mag2;
		data["time"] = this->duration;
		data["type"] = this->boost_type;
	}

	std::uint32_t boost_params_t::get_multiplier() const
	{
		return this->mag1 / 100;
	}

	std::chrono::seconds boost_params_t::get_time_left() const
	{
		const auto now = std::chrono::system_clock::now();
		if (this->expire_date < now.time_since_epoch())
		{
			return 0s;
		}
		else
		{
			const auto diff = this->expire_date - now.time_since_epoch();
			return std::chrono::duration_cast<std::chrono::seconds>(diff);
		}
	}

	namespace impl
	{
		template <database_type_t Type>
		void delete_user_data(const std::uint64_t user_id)
		{
			return database::access([&](database_t& db)
			{
				db.get_database<Type>()->operator()(
					sqlpp::remove_from(boost::table)
						.where(boost::table.f_user_id == user_id));
			});
		}

		template <database_type_t Type>
		std::array<boost_params_t, boost_type_count> get_boost_list(const std::uint64_t user_id)
		{
			const auto now = std::chrono::system_clock::now();

			return database::access<std::array<boost_params_t, boost_type_count>>([&](database_t& db)
				-> std::array<boost_params_t, boost_type_count>
			{
				auto results = db.exec<Type>(
					sqlpp::select(boost::table.boost_type,
								  sqlpp::max(boost::table.mag1).as(alias::mag1),
								  sqlpp::max(boost::table.mag2).as(alias::mag2),
								  sqlpp::max(boost::table.duration).as(alias::duration),
								  sqlpp::max(boost::table.expire_date).as(alias::expire_date))
						.from(boost::table)
							.where(boost::table.f_user_id == user_id && boost::table.expire_date > now)
								.group_by(boost::table.boost_type));

				std::array<boost_params_t, boost_type_count> list{};

				for (auto& row : results)
				{
					const auto type = static_cast<std::uint8_t>(row.boost_type);
					if (type >= boost_type_count)
					{
						continue;
					}

					auto& entry = list[type];
					entry.boost_type = static_cast<std::uint8_t>(row.boost_type);
					entry.duration = static_cast<std::uint32_t>(row.duration);
					entry.mag1 = static_cast<std::uint32_t>(row.mag1);
					entry.mag2 = static_cast<std::uint32_t>(row.mag2);
					entry.expire_date = std::chrono::duration_cast<std::chrono::seconds>(row.expire_date.value().time_since_epoch());
				}

				return list;
			});
		}

		template <database_type_t Type>
		std::optional<boost_params_t> get_boost_of_type(const std::uint64_t user_id, const std::uint8_t boost_type)
		{
			const auto now = std::chrono::system_clock::now();

			return database::access<std::optional<boost_params_t>>([&](database_t& db)
				-> std::optional<boost_params_t>
			{
				auto results = db.exec<Type>(
					sqlpp::select(boost::table.boost_type,
								  sqlpp::max(boost::table.mag1).as(alias::mag1),
								  sqlpp::max(boost::table.mag2).as(alias::mag2),
								  sqlpp::max(boost::table.duration).as(alias::duration),
								  sqlpp::max(boost::table.expire_date).as(alias::expire_date))
						.from(boost::table)
							.where(boost::table.f_user_id == user_id && boost::table.expire_date > now &&
								   boost::table.boost_type == boost_type)
								.group_by(boost::table.boost_type));

				std::optional<boost_params_t> list;

				if (!results.empty())
				{
					auto& row = results.front();
					boost_params_t params{};
					params.boost_type = static_cast<std::uint8_t>(row.boost_type);
					params.duration = static_cast<std::uint32_t>(row.duration);
					params.mag1 = static_cast<std::uint32_t>(row.mag1);
					params.mag2 = static_cast<std::uint32_t>(row.mag2);
					params.expire_date = std::chrono::duration_cast<std::chrono::seconds>(row.expire_date.value().time_since_epoch());

					list.emplace(params);
				}

				return list;
			});
		}

		template <database_type_t Type>
		void add_boost(const std::uint64_t user_id, const std::uint8_t type, const std::uint32_t duration,
			const std::uint32_t mag1, const std::uint32_t mag2, const std::chrono::system_clock::time_point expire_date)
		{
			return database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::insert_into(boost::table)
						.set(boost::table.f_user_id = user_id,
							 boost::table.boost_type = type,
							 boost::table.duration = duration,
							 boost::table.mag1 = mag1,
							 boost::table.mag2 = mag2,
							 boost::table.expire_date = expire_date));
			});
		}
	}

	void delete_user_data(const std::uint64_t user_id)
	{
		RUN_IMPL(impl::delete_user_data, user_id);
	}

	void add_boost(const std::uint64_t user_id, const std::uint8_t type, const std::uint32_t duration,
		const std::uint32_t mag1, const std::uint32_t mag2, const std::chrono::system_clock::time_point expire_date)
	{
		RUN_IMPL(impl::add_boost, user_id, type, duration, mag1, mag2, expire_date);
	}

	std::array<boost_params_t, boost_type_count> get_boost_list(const std::uint64_t user_id)
	{
		RUN_IMPL(impl::get_boost_list, user_id);
	}

	std::optional<boost_params_t> get_boost_of_type(const std::uint64_t user_id, const std::uint8_t type)
	{
		RUN_IMPL(impl::get_boost_of_type, user_id, type);
	}

	class table final : public table_interface
	{
	public:
		void create(database_t& database) override
		{
			database.run_query("mgssd.boosts.create");

			users::register_privilege(0x6FDA2A0E, [](const users::user& user, game::item_t& item)
			{
				auto boost_list = get_boost_list(user.get_user_id());
				const auto now = std::chrono::system_clock::now();
				std::array<std::chrono::system_clock::time_point, boost_type_count> dates;

				for (auto i = 0u; i < boost_type_count; i++)
				{
					dates[i] = now + boost_list[i].get_time_left() + item.param1 * 1h;
				}

				add_boost(user.get_user_id(), boost_type_resource,
					item.param1, item.param2, item.param3, dates[boost_type_resource]);

				add_boost(user.get_user_id(), boost_type_kub,
					item.param1, item.param4, 0, dates[boost_type_kub]);

				add_boost(user.get_user_id(), boost_type_bp,
					item.param1, item.param5, 0, dates[boost_type_bp]);

				add_boost(user.get_user_id(), boost_type_pass,
					item.param1, 0, 0, dates[boost_type_pass]);

				item.param1 = 0x6FDA2A0E;

				for (auto i = 0u; i < boost_type_count; i++)
				{
					const auto result_date = std::chrono::duration_cast<std::chrono::seconds>(dates[i].time_since_epoch()).count();
					(&item.param2)[i] = static_cast<std::uint32_t>(result_date);
				}
			});
		}
	};
}

REGISTER_TABLE(database::boosts::table, 2)
