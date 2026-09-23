#pragma once

#include "../database.hpp"
#include "players.hpp"
#include "../utils.hpp"
#include "game/game.hpp"

namespace database::present_box
{
	constexpr const auto max_entries = 250u;

	enum present_flag_t
	{
		present_flag_expire = (1 << 0),
		present_flag_new = (1 << 1),
		present_flag_purchase = (1 << 2),
	};

	class present_box_entry
	{
	public:
		DEFINE_FIELD(present_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(f_player_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(flags, sqlpp::integer_unsigned);
		DEFINE_FIELD(item_category, sqlpp::integer_unsigned);
		DEFINE_FIELD(item_code, sqlpp::integer_unsigned);
		DEFINE_FIELD(item_num, sqlpp::integer_unsigned);
		DEFINE_FIELD(item_param1, sqlpp::integer_unsigned);
		DEFINE_FIELD(item_param2, sqlpp::integer_unsigned);
		DEFINE_FIELD(item_param3, sqlpp::integer_unsigned);
		DEFINE_FIELD(item_param4, sqlpp::integer_unsigned);
		DEFINE_FIELD(item_param5, sqlpp::integer_unsigned);
		DEFINE_FIELD(expire_date, sqlpp::time_point);
		DEFINE_TABLE(present_box_entries,
			present_id_field_t,
			f_player_id_field_t,
			flags_field_t,
			item_category_field_t,
			item_code_field_t,
			item_num_field_t,
			item_param1_field_t,
			item_param2_field_t,
			item_param3_field_t,
			item_param4_field_t,
			item_param5_field_t,
			expire_date_field_t
		);

		inline static table_t table;

		template <typename ...Args>
		present_box_entry(const sqlpp::result_row_t<Args...>& row)
		{
			this->present_id_ = row.present_id;
			this->player_id_ = row.f_player_id;
			this->flags_ = static_cast<std::uint32_t>(row.flags);
			this->reward_.category = static_cast<std::uint8_t>(row.item_category);
			this->reward_.code = static_cast<std::uint32_t>(row.item_code);
			this->reward_.num = static_cast<std::uint16_t>(row.item_num);
			this->reward_.param1 = static_cast<std::uint32_t>(row.item_param1);
			this->reward_.param2 = static_cast<std::uint32_t>(row.item_param2);
			this->reward_.param3 = static_cast<std::uint32_t>(row.item_param3);
			this->reward_.param4 = static_cast<std::uint32_t>(row.item_param4);
			this->reward_.param5 = static_cast<std::uint32_t>(row.item_param5);
			this->expire_date_ = std::chrono::duration_cast<std::chrono::seconds>(row.expire_date.value().time_since_epoch());
		}

		GET_FIELD_H(std::uint64_t, present_id);
		GET_FIELD_H(std::uint64_t, player_id);
		GET_FIELD_H(std::uint32_t, flags);
		GET_FIELD_H(std::chrono::seconds, expire_date);

		void to_json(json::value& data) const;
		void to_json_item(json::value& data) const;

		const game::reward_t& get_reward() const;

	private:
		game::reward_t reward_{};

	};

	std::optional<present_box_entry> find_item(const std::uint64_t present_id);
	bool delete_item(const std::uint64_t present_id);
	void remove_new_flag(const std::uint64_t present_id);
	std::vector<present_box_entry> get_all_items(const std::uint64_t player_id);
	std::size_t get_present_count(const std::uint64_t player_id);
	void delete_all_items(const std::uint64_t player_id);
	void add_item(const std::uint64_t player_id, const std::uint32_t flags, const std::chrono::seconds expire_date, const game::reward_t& item);
	bool has_new_item(const std::uint64_t player_id);
	void delete_player_data(const std::uint64_t player_id);
}
