#pragma once

#include "../database.hpp"
#include "users.hpp"
#include "game/game.hpp"

namespace database::shop_purchases
{
	constexpr const auto purchase_duration = 24h * 180;

	enum purchase_type_t : std::uint32_t
	{
		product_10010 = 10010, // 0
		product_10020 = 10020, // 0
		product_10030 = 10030, // 0
		product_10090 = 10090, // 176702308069091
		product_10091 = 10091, // 176702308069091
		product_10092 = 10092, // 176702308069091
		product_10093 = 10093, // 176702308069091
		product_10094 = 10094, // 0
		product_additional_slot_avatar = 20010, // 0
		product_additional_slot_avatar_1 = 20011, // 118282266416844
		product_additional_slot_avatar_2 = 20012, // 0
		product_additional_slot_avatar_3 = 20013, // 0
		product_increase_storage_limit_weapons = 20020, // 0
		product_increase_storage_limit_gear = 20030, // 0
		product_additional_slot_loadout = 20050, // 0
		product_additional_slot_exploration = 20070, // 0
		product_20090 = 20090, // 0
		product_40030 = 40030, // 0
		product_40031 = 40031, // 0
		product_40032 = 40032, // 127547405028823
		product_gesture_begin = 50000, // 0
		product_50001 = 50001, // 0
		product_50003 = 50003, // 0
		product_50005 = 50005, // 0
		product_50006 = 50006, // 0
		product_50007 = 50007, // 0
		product_50008 = 50008, // 0
		product_50018 = 50018, // 0
		product_50019 = 50019, // 0
		product_50020 = 50020, // 0
		product_50021 = 50021, // 0
		product_50022 = 50022, // 0
		product_50024 = 50024, // 0
		product_50026 = 50026, // 0
		product_50027 = 50027, // 0
		product_50028 = 50028, // 0
		product_50030 = 50030, // 0
		product_50032 = 50032, // 0
		product_50033 = 50033, // 0
		product_50039 = 50039, // 0
		product_50040 = 50040, // 0
		product_50042 = 50042, // 0
		product_50043 = 50043, // 0
		product_60001 = 60001, // 201166507808394
		product_60002 = 60002, // 191329924624191
		product_60003 = 60003, // 251313580807137
		product_60101 = 60101, // 29498001435880
		product_60102 = 60102, // 0
		product_60103 = 60103, // 0
		product_60104 = 60104, // 0
		product_60105 = 60105, // 0
		product_60106 = 60106, // 19329350830104
		product_60107 = 60107, // 105419985908861
		product_60108 = 60108, // 51273834480689
		product_60109 = 60109, // 111276158422441
		product_60110 = 60110, // 61029669411831
		product_60111 = 60111, // 82223035396603
		product_60112 = 60112, // 0
		product_60113 = 60113, // 87155778956332
		product_60114 = 60114, // 0
		product_60626 = 60626, // 189297463612136
		product_deploy_reduction = 90010, // 0
		product_defense_mission_reduction = 90020, // 0

		product_radio_begin = 100000,  // made up
		product_marker_begin = 110000, // made up
	};

	enum event_type_t : std::uint32_t
	{
		event_none = 0,
		event_loss = 1,
		event_gain = 2,
	};

	struct shop_product_t
	{
		std::uint32_t product_id;
		std::uint32_t price;
		std::uint32_t limit_count;
		std::uint64_t lang_id;
		game::item_t item;

		void to_json(json::value& data) const;
	};

	const std::vector<shop_product_t>& get_shop_product_list();
	const std::unordered_map<std::uint32_t, shop_product_t>& get_shop_product_map();
	std::optional<shop_product_t> find_product(const std::uint32_t product_type);

	const std::vector<shop_product_t>& get_shop_item_list();
	const std::unordered_map<std::uint32_t, shop_product_t>& get_shop_item_map();
	std::optional<shop_product_t> find_item(const std::uint32_t product_type);

	struct entry_params_t
	{
		std::uint32_t event_type;
		std::uint32_t purchase_quantity;
		std::uint32_t product_type;
		std::uint32_t coin_quantity;
		std::uint64_t lang_id;
		game::item_t item;
	};

	class shop_purchase
	{
	public:
		DEFINE_FIELD(purchase_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(f_user_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(lang_id, sqlpp::integer_unsigned);
		DEFINE_FIELD(event_type, sqlpp::integer_unsigned);
		DEFINE_FIELD(product_type, sqlpp::integer_unsigned);
		DEFINE_FIELD(purchase_quantity, sqlpp::integer_unsigned);
		DEFINE_FIELD(coin_quantity, sqlpp::integer_unsigned);
		DEFINE_FIELD(remaining_coin, sqlpp::integer_unsigned);
		DEFINE_FIELD(item_category, sqlpp::integer_unsigned);
		DEFINE_FIELD(item_code, sqlpp::integer_unsigned);
		DEFINE_FIELD(item_param1, sqlpp::integer_unsigned);
		DEFINE_FIELD(item_param2, sqlpp::integer_unsigned);
		DEFINE_FIELD(item_param3, sqlpp::integer_unsigned);
		DEFINE_FIELD(item_param4, sqlpp::integer_unsigned);
		DEFINE_FIELD(item_param5, sqlpp::integer_unsigned);
		DEFINE_FIELD(item_num, sqlpp::integer_unsigned);
		DEFINE_FIELD(purchase_date, sqlpp::time_point);
		DEFINE_FIELD(expire_date, sqlpp::time_point);
		DEFINE_TABLE(shop_purchases, 
			purchase_id_field_t,
			f_user_id_field_t,
			lang_id_field_t,
			event_type_field_t,
			product_type_field_t,
			purchase_quantity_field_t,
			coin_quantity_field_t,
			remaining_coin_field_t,
			item_category_field_t,
			item_code_field_t,
			item_param1_field_t,
			item_param2_field_t,
			item_param3_field_t,
			item_param4_field_t,
			item_param5_field_t,
			item_num_field_t,
			purchase_date_field_t,
			expire_date_field_t
		);

		inline static table_t table;

		template <typename ...Args>
		shop_purchase(const sqlpp::result_row_t<Args...>& row)
		{
			this->purchase_id_ = row.purchase_id;
			this->user_id_ = row.f_user_id;
			this->lang_id_ = row.lang_id;
			this->event_type_ = static_cast<std::uint32_t>(row.event_type);
			this->product_type_ = static_cast<std::uint32_t>(row.product_type);
			this->purchase_quantity_ = static_cast<std::uint32_t>(row.purchase_quantity);
			this->coin_quantity_ = static_cast<std::uint32_t>(row.coin_quantity);
			this->remaining_coin_ = static_cast<std::uint32_t>(row.remaining_coin);
			this->item_.category = static_cast<std::uint8_t>(row.item_category);
			this->item_.code = static_cast<std::uint32_t>(row.item_code);
			this->item_.param1 = static_cast<std::uint32_t>(row.item_param1);
			this->item_.param2 = static_cast<std::uint32_t>(row.item_param2);
			this->item_.param3 = static_cast<std::uint32_t>(row.item_param3);
			this->item_.param4 = static_cast<std::uint32_t>(row.item_param4);
			this->item_.param5 = static_cast<std::uint32_t>(row.item_param5);
			this->item_.num = static_cast<std::uint32_t>(row.item_num);
			this->purchase_date_ = std::chrono::duration_cast<std::chrono::seconds>(row.purchase_date.value().time_since_epoch());
			this->expire_date_ = std::chrono::duration_cast<std::chrono::seconds>(row.expire_date.value().time_since_epoch());
		}

		GET_FIELD_H(std::uint64_t, purchase_id);
		GET_FIELD_H(std::uint64_t, user_id);
		GET_FIELD_H(std::uint64_t, lang_id);
		GET_FIELD_H(std::uint32_t, event_type);
		GET_FIELD_H(std::uint32_t, product_type);
		GET_FIELD_H(std::uint32_t, purchase_quantity);
		GET_FIELD_H(std::uint32_t, coin_quantity);
		GET_FIELD_H(std::uint32_t, remaining_coin);
		GET_FIELD_H(std::chrono::seconds, purchase_date);
		GET_FIELD_H(std::chrono::seconds, expire_date);

		const game::item_t& get_item() const;

		void to_json(json::value& data) const;

	private:
		game::item_t item_{};

	};

	bool add_entry(const std::uint64_t user_id, const entry_params_t& params);
	bool add_product(const std::uint64_t user_id, const shop_product_t& product, const std::uint32_t purchase_quantity = 1u);
	bool add_spent(const std::uint64_t user_id, const std::uint64_t lang_id, const std::uint32_t coin_quantity);
	bool add_spent(const std::uint64_t user_id, const std::uint64_t lang_id, const std::uint32_t coin_quantity, const game::item_t& item);
	bool add_received_coin(const std::uint64_t user_id, const std::uint64_t lang_id, const std::uint32_t coin_quantity);

	std::size_t get_history_size(const std::uint64_t user_id);
	std::vector<shop_purchase> get_history(const std::uint64_t user_id, const std::uint32_t offset, const std::uint32_t limit);
	std::optional<shop_purchase> get_last_item_purchase(const std::uint64_t user_id, const std::uint32_t item_type);
	std::optional<shop_purchase> get_last_item_purchase_range(const std::uint64_t user_id, const std::uint32_t item_type_beg, const std::uint32_t item_type_end);
	std::unordered_map<std::uint32_t, std::size_t> get_purchase_counts(const std::uint64_t user_id);
	std::size_t get_purchase_count(const std::uint64_t user_id, const std::uint32_t product_type);

	void delete_user_data(const std::uint64_t user_id);

	json::value purchase_product(const users::user& user, const std::uint32_t product_type, const std::function<bool(json::value& result)>& callback);
	json::value purchase_product(const users::user& user, const shop_product_t& product, const std::function<bool(json::value& result)>& callback);
}
