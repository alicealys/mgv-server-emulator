#pragma once

#include "../database.hpp"
#include "users.hpp"
#include "game/game.hpp"

namespace database::boosts
{
	constexpr const auto max_boost_duration = 90 * 24h;

	struct boost_params_t
	{
		std::uint8_t boost_type;
		std::uint32_t duration;
		std::uint32_t mag1;
		std::uint32_t mag2;
		std::chrono::seconds expire_date;

		std::chrono::seconds get_time_left() const;
		void to_json(json::value& data) const;
	};

	enum boost_type_t : std::uint8_t
	{
		boost_type_resource = 0,
		boost_type_kub = 1,
		boost_type_bp = 2,
		boost_type_pass = 3,
		boost_type_count = 4,
	};

	class boost
	{
	public:
		DEFINE_FIELD(boost_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(f_user_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(boost_type, sqlpp::integer_unsigned);
		DEFINE_FIELD(duration, sqlpp::integer_unsigned);
		DEFINE_FIELD(mag1, sqlpp::integer_unsigned);
		DEFINE_FIELD(mag2, sqlpp::integer_unsigned);
		DEFINE_FIELD(expire_date, sqlpp::time_point);
		DEFINE_TABLE(boosts, 
			boost_id_field_t,
			f_user_id_field_t,
			boost_type_field_t,
			duration_field_t,
			mag1_field_t,
			mag2_field_t,
			expire_date_field_t
		);

		inline static table_t table;

		template <typename ...Args>
		boost(const sqlpp::result_row_t<Args...>& row)
		{
			this->boost_id_ = row.boost_id;
			this->user_id_ = row.f_user_id;
			this->boost_type_ = static_cast<std::uint8_t>(row.boost_type);
			this->duration_ = static_cast<std::uint32_t>(row.duration);
			this->mag1_ = static_cast<std::uint32_t>(row.mag1);
			this->mag2_ = static_cast<std::uint32_t>(row.mag2);
			this->expire_date_ = std::chrono::duration_cast<std::chrono::seconds>(row.expire_date.value().time_since_epoch());
		}

		GET_FIELD_H(std::uint64_t, boost_id);
		GET_FIELD_H(std::uint64_t, user_id);
		GET_FIELD_H(std::uint8_t, boost_type);
		GET_FIELD_H(std::uint32_t, duration);
		GET_FIELD_H(std::uint32_t, mag1);
		GET_FIELD_H(std::uint32_t, mag2);
		GET_FIELD_H(std::chrono::seconds, expire_date);

		void to_json(json::value& data) const;

	private:
		game::item_t item_{};

	};

	void add_boost(const std::uint64_t user_id, const std::uint8_t type, const std::uint32_t duration,
		const std::uint32_t mag1, const std::uint32_t mag2, const std::chrono::system_clock::time_point expire_date);
	std::array<boost_params_t, boost_type_count> get_boost_list(const std::uint64_t user_id);
	std::optional<boost_params_t> get_boost_of_type(const std::uint64_t user_id, const std::uint8_t type);

	void delete_user_data(const std::uint64_t user_id);
}
