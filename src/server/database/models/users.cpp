#include <std_include.hpp>

#include "players.hpp"
#include "defense_missions.hpp"
#include "present_box.hpp"
#include "deployments.hpp"
#include "crew_members.hpp"
#include "shop_purchases.hpp"
#include "users.hpp"
#include "variables.hpp"
#include "../auth.hpp"

#include "utils/encoding.hpp"
#include "utils/json_utils.hpp"

#include <utils/cryptography.hpp>
#include <utils/string.hpp>

namespace database::users
{
	namespace
	{
		bool is_session_expired(const user& user)
		{
			const auto now = std::chrono::duration_cast<std::chrono::microseconds>(
				std::chrono::system_clock::now().time_since_epoch());
			return (now - user.get_last_update()) > database::vars.session_timeout;
		}

#define IS_ACTIVE_EXPR user::table.last_update >= std::chrono::system_clock::now() - database::vars.session_timeout

		namespace
		{
			std::vector<std::string> nat_types =
			{
				"SYMMETRIC_NAT",
				"RESTRICTED_PORT_CONE_NAT",
				"RESTRICTED_CONE_NAT",
				"OPEN_CHOICE_PORT",
				"FULL_CONE_NAT",
				"SYMMETRIC_OPEN",
				"SYMMETRIC_UDP_FIREWALL",
				"OPEN_INTERNET"
			};
		}

		std::uint32_t get_nat_type_id(const std::string& nat_type)
		{
			for (auto i = 0u; i < nat_types.size(); i++)
			{
				if (nat_type == nat_types[i])
				{
					return i;
				}
			}

			return 0;
		}

		std::string get_nat_type(const std::uint32_t nat_type_id)
		{
			if (nat_type_id < nat_types.size())
			{
				return nat_types[nat_type_id];
			}

			return nat_types[0];
		}

		namespace exp
		{
			const auto select =
				sqlpp::select(sqlpp::all_of(user::table), 
						players::player::table.player_id, 
						players::player::table.f_user_id, 
						players::player::table.player_index, 
						players::player::table.playtime, 
						players::player::table.point, 
						players::player::table.nameplate,
						players::player::table.current_loadout,
						players::player::table.player_creation_date)
					.from(user::table.left_outer_join(players::player::table).on(user::table.current_player_id == players::player::table.player_id));
		}

		const json::value& get_default_data()
		{
			static const auto data = utils::resources::load_json(RESOURCE_DEFAULT_DATA);
			return data;
		}

		const json::value& get_default_data(const std::string_view& key)
		{
			const auto& data = get_default_data();
			return data[key];
		}
	}

	void user_inventory_t::initialize()
	{
		static const auto default_inventory = []()
		{
			static user_inventory_t data{};

			const auto& default_data = get_default_data("inventory_user_info");
			const auto load = [&]<typename T>(const std::string_view& key, T& buffer)
			{
				auto data = default_data[key].get<std::string>();
				data = utils::cryptography::base64::decode(data);

				if (data.size() != sizeof(T))
				{
					throw std::runtime_error("invalid user inventory data");
				}

				std::memcpy(buffer, data.data(), sizeof(T));
			};

			load("archive_new", data.archive_new);
			load("archive_obtained", data.archive_obtained);
			load("battle_pack_opened", data.battle_pack_opened);
			load("cassette_new", data.cassette_new);
			load("cassette_obtained", data.cassette_obtained);
			load("command_marker_obtained", data.command_marker_obtained);
			load("face_paint_new", data.face_paint_new);
			load("face_paint_obtained", data.face_paint_obtained);
			load("food_used", data.food_used);
			load("gesture_new", data.gesture_new);
			load("gesture_obtained", data.gesture_obtained);
			load("name_plate_new", data.name_plate_new);
			load("name_plate_obtained", data.name_plate_obtained);
			load("preset_radio_obtained", data.preset_radio_obtained);
			load("production_opened", data.production_opened);
			load("recipe_new", data.recipe_new);
			load("recipe_new_for_db", data.recipe_new_for_db);
			load("recipe_opened", data.recipe_opened);
			load("recipe_used", data.recipe_used);
			load("resource_opened", data.resource_opened);

			data.bgm_my_list_setting = default_data["bgm_my_list_setting"].as<std::uint8_t>();

			return &data;
		}();

		std::memcpy(this, default_inventory, sizeof(user_inventory_t));
	}

	void user_inventory_t::to_json(json::value& data)
	{
		data["archive_new"] = utils::encoding::encode_base64(this->archive_new);
		data["archive_obtained"] = utils::encoding::encode_base64(this->archive_obtained);
		data["battle_pack_opened"] = utils::encoding::encode_base64(this->battle_pack_opened);
		data["cassette_new"] = utils::encoding::encode_base64(this->cassette_new);
		data["cassette_obtained"] = utils::encoding::encode_base64(this->cassette_obtained);
		data["command_marker_new"] = utils::encoding::encode_base64(this->command_marker_new);
		data["command_marker_obtained"] = utils::encoding::encode_base64(this->command_marker_obtained);
		data["face_paint_new"] = utils::encoding::encode_base64(this->face_paint_new);
		data["face_paint_obtained"] = utils::encoding::encode_base64(this->face_paint_obtained);
		data["food_used"] = utils::encoding::encode_base64(this->food_used);
		data["gesture_new"] = utils::encoding::encode_base64(this->gesture_new);
		data["gesture_obtained"] = utils::encoding::encode_base64(this->gesture_obtained);
		data["name_plate_new"] = utils::encoding::encode_base64(this->name_plate_new);
		data["name_plate_obtained"] = utils::encoding::encode_base64(this->name_plate_obtained);
		data["preset_radio_new"] = utils::encoding::encode_base64(this->preset_radio_new);
		data["preset_radio_obtained"] = utils::encoding::encode_base64(this->preset_radio_obtained);
		data["production_opened"] = utils::encoding::encode_base64(this->production_opened);
		data["recipe_new"] = utils::encoding::encode_base64(this->recipe_new);
		data["recipe_new_for_db"] = utils::encoding::encode_base64(this->recipe_new_for_db);
		data["recipe_opened"] = utils::encoding::encode_base64(this->recipe_opened);
		data["recipe_used"] = utils::encoding::encode_base64(this->recipe_used);
		data["resource_opened"] = utils::encoding::encode_base64(this->resource_opened);
		data["bgm_my_list_setting"] = this->bgm_my_list_setting;
	}

	void user_inventory_t::open_production(const std::uint32_t index)
	{
		this->set_obtained_generic(this->production_opened, index);
	}

	void user_inventory_t::set_obtained_gesture(const std::uint32_t index)
	{
		this->set_obtained_generic(this->gesture_obtained, index);
		this->set_obtained_generic(this->gesture_new, index);
	}

	void user_inventory_t::set_obtained_radio(const std::uint32_t index)
	{
		this->set_obtained_generic(this->preset_radio_obtained, index);
		this->set_obtained_generic(this->preset_radio_new, index);
	}

	void user_inventory_t::set_obtained_marker(const std::uint32_t index)
	{
		this->set_obtained_generic(this->command_marker_obtained, index);
		this->set_obtained_generic(this->command_marker_new, index);
	}

	void user_inventory_t::set_obtained_nameplate(const std::uint32_t index)
	{
		this->set_obtained_generic(this->name_plate_obtained, index);
		this->set_obtained_generic(this->name_plate_new, index);
	}

	void user_inventory_t::set_obtained_recipe(const std::uint32_t index)
	{
		this->set_obtained_generic(this->recipe_new, index);
		this->set_obtained_generic(this->recipe_opened, index);
	}

	void user_inventory_t::set_obtained_battle_pack(const std::uint32_t index)
	{
		this->set_obtained_generic(this->battle_pack_opened, index);
	}

	void user_inventory_t::set_obtained_face_paint(const std::uint32_t index)
	{
		this->set_obtained_generic(this->face_paint_obtained, index);
		this->set_obtained_generic(this->face_paint_new, index);
	}

	void user_inventory_t::set_obtained_cassette(const std::uint32_t index)
	{
		this->set_obtained_generic(this->cassette_obtained, index);
		this->set_obtained_generic(this->cassette_new, index);
	}

	GET_FIELD_C(user, std::uint64_t, user_id);
	GET_FIELD_C(user, std::uint64_t, account_id);
	GET_FIELD_C(user, std::uint64_t, current_player_id);
	GET_FIELD_C(user, std::string, session_id);
	GET_FIELD_C(user, std::string, password_hash);
	GET_FIELD_C(user, std::string, crypto_key);
	GET_FIELD_C(user, std::string, currency);
	GET_FIELD_C(user, std::string, ex_ip);
	GET_FIELD_C(user, std::string, in_ip);
	GET_FIELD_C(user, std::uint16_t, ex_port);
	GET_FIELD_C(user, std::uint16_t, in_port);
	GET_FIELD_C(user, std::uint32_t, user_flag);
	GET_FIELD_C(user, std::uint32_t, dlc_flag);
	GET_FIELD_C(user, std::uint32_t, sv_coin);
	GET_FIELD_C(user, std::uint64_t, player_capacity);
	GET_FIELD_C(user, std::chrono::microseconds, last_update);
	GET_FIELD_C(user, std::chrono::microseconds, creation_date);


	std::uint32_t user::get_loadout_count() const
	{
		if (this->loadout_count_ >= players::max_loadout_count)
		{
			return players::max_loadout_count;
		}

		return this->loadout_count_;
	}

	bool user_inventory_t::parse_save(json::value& data)
	{
		const auto try_parse_part = [&]<typename T>(const std::string_view& name, T& dest, const bool or_ = false)
		{
			auto& value_j = data[name];
			if (!value_j.is_array() || value_j.size() <= 0)
			{
				return;
			}

			auto& obj_j = value_j[0];
			if (!obj_j.is_object())
			{
				return;
			}

			auto& data_j = obj_j[name];
			if (!data_j.is_string())
			{
				return;
			}

			if (or_)
			{
				T new_data{};
				utils::json_utils::parse_base64(data_j, new_data, true);

				auto src = reinterpret_cast<std::uint8_t*>(&new_data);
				auto dst = reinterpret_cast<std::uint8_t*>(&dest);

				for (auto i = 0ull; i < sizeof(T); i++)
				{
					dst[i] |= src[i];
				}
			}
			else
			{
				utils::json_utils::parse_base64(data_j, dest, true);
			}
		};

		try_parse_part("archive_new", this->archive_new);
		try_parse_part("archive_obtained", this->archive_obtained, true);
		try_parse_part("battle_pack_opened", this->battle_pack_opened, true);
		try_parse_part("cassette_new", this->cassette_new);
		try_parse_part("cassette_obtained", this->cassette_obtained, true);
		try_parse_part("command_marker_new", this->command_marker_new);
		try_parse_part("command_marker_obtained", this->command_marker_obtained, true);
		try_parse_part("face_paint_new", this->face_paint_new);
		try_parse_part("face_paint_obtained", this->face_paint_obtained, true);
		try_parse_part("food_used", this->food_used);
		try_parse_part("gesture_new", this->gesture_new);
		try_parse_part("gesture_obtained", this->gesture_obtained, true);
		try_parse_part("name_plate_new", this->name_plate_new);
		try_parse_part("name_plate_obtained", this->name_plate_obtained, true);
		try_parse_part("preset_radio_new", this->preset_radio_new);
		try_parse_part("preset_radio_obtained", this->preset_radio_obtained, true);
		try_parse_part("production_opened", this->production_opened, true);
		try_parse_part("recipe_new", this->recipe_new);
		try_parse_part("recipe_new_for_db", this->recipe_new_for_db);
		try_parse_part("recipe_opened", this->recipe_opened, true);
		try_parse_part("recipe_used", this->recipe_used);
		try_parse_part("resource_opened", this->resource_opened, true);

		utils::json_utils::get_or(data["bgm_my_list_setting"], this->bgm_my_list_setting, this->bgm_my_list_setting);
		return true;
	}

	bool user_inventory_t::parse(json::value& data)
	{
		utils::json_utils::parse_base64(data["archive_new"], this->archive_new);
		utils::json_utils::parse_base64(data["archive_obtained"], this->archive_obtained);
		utils::json_utils::parse_base64(data["battle_pack_opened"], this->battle_pack_opened);
		utils::json_utils::parse_base64(data["cassette_new"], this->cassette_new);
		utils::json_utils::parse_base64(data["cassette_obtained"], this->cassette_obtained);
		utils::json_utils::parse_base64(data["command_marker_new"], this->command_marker_new);
		utils::json_utils::parse_base64(data["command_marker_obtained"], this->command_marker_obtained);
		utils::json_utils::parse_base64(data["face_paint_new"], this->face_paint_new);
		utils::json_utils::parse_base64(data["face_paint_obtained"], this->face_paint_obtained);
		utils::json_utils::parse_base64(data["food_used"], this->food_used);
		utils::json_utils::parse_base64(data["gesture_new"], this->gesture_new);
		utils::json_utils::parse_base64(data["gesture_obtained"], this->gesture_obtained);
		utils::json_utils::parse_base64(data["name_plate_new"], this->name_plate_new);
		utils::json_utils::parse_base64(data["name_plate_obtained"], this->name_plate_obtained);
		utils::json_utils::parse_base64(data["preset_radio_new"], this->preset_radio_new);
		utils::json_utils::parse_base64(data["preset_radio_obtained"], this->preset_radio_obtained);
		utils::json_utils::parse_base64(data["production_opened"], this->production_opened);
		utils::json_utils::parse_base64(data["recipe_new"], this->recipe_new);
		utils::json_utils::parse_base64(data["recipe_new_for_db"], this->recipe_new_for_db);
		utils::json_utils::parse_base64(data["recipe_opened"], this->recipe_opened);
		utils::json_utils::parse_base64(data["recipe_used"], this->recipe_used);
		utils::json_utils::parse_base64(data["resource_opened"], this->resource_opened);

		utils::json_utils::get_or(data["bgm_my_list_setting"], this->bgm_my_list_setting, this->bgm_my_list_setting);
		return true;
	}

	std::string user::get_nat() const
	{
		return get_nat_type(this->nat_);
	}

	std::uint64_t user::get_id() const
	{
		return this->user_id_;
	}

	namespace impl
	{
		template <database_type_t Type>
		std::optional<user> find(const std::uint64_t user_id)
		{
			return database::access<std::optional<user>>([&](database_t& db)
				-> std::optional<user>
			{
				auto results = db.exec<Type>(exp::select.where(user::table.user_id == user_id));

				if (results.empty())
				{
					return {};
				}

				const auto& row = results.front();
				return {user(row)};
			});
		}

		template <database_type_t Type>
		bool exists(const std::uint64_t user_id)
		{
			return database::access<bool>([&](database_t& db)
			{
				auto results = db.exec<Type>(
					sqlpp::select(
						sqlpp::count(1))
							.from(user::table)
								.where(user::table.user_id == user_id));

				return results.front().count.value() > 0;
			});
		}

		template <database_type_t Type>
		std::optional<user> find_from_account(const std::uint64_t account_id)
		{
			return database::access<std::optional<user>>([&](database::database_t& db)
				-> std::optional<user>
			{
				auto results = db.exec<Type>(exp::select.where(user::table.account_id == account_id));

				if (results.empty())
				{
					return {};
				}

				const auto& row = results.front();
				return {user(row)};
			});
		}

		template <database_type_t Type>
		std::optional<user> find_from_session_id(const std::string session_id, bool use_timeout, bool* is_expired)
		{
			return database::access<std::optional<user>>([&](database::database_t& db)
				-> std::optional<user>
			{
				auto results = db.exec<Type>(exp::select.where(user::table.session_id == session_id));

				if (results.empty())
				{
					return {};
				}

				const auto& row = results.front();
				user player(row);

				const auto expired = is_session_expired(player);
				if (is_expired != nullptr)
				{
					*is_expired = expired;
				}

				if (use_timeout && expired)
				{
					return {};
				}

				return {player};
			});
		}

		template <database_type_t Type>
		user find_or_insert(const std::uint64_t account_id)
		{
			{
				const auto found = find_from_account<Type>(account_id);
				if (found.has_value())
				{
					return found.value();
				}
			}

			static const auto default_inventory = []()
			{
				static user_inventory_t inventory{};
				inventory.initialize();
				return sqlpp::verbatim<sqlpp::binary>(utils::encoding::encode_binary(inventory));
			}();

			database::access([&](database::database_t& db)
			{
				db.exec<Type>(
					sqlpp::insert_into(user::table)
						.set(user::table.account_id = account_id,
							 user::table.currency = "",
							 user::table.user_inventory = default_inventory,
							 user::table.loadout_count = players::initial_loadout_count,
							 user::table.player_capacity = 1,
							 user::table.last_update = std::chrono::system_clock::now(),
							 user::table.user_creation_date = std::chrono::system_clock::now()));
			});

			const auto found = find_from_account<Type>(account_id);
			if (!found.has_value())
			{
				throw std::runtime_error("[database::users::insert] Insertion failed");
			}

			return found.value();
		}

		template <database_type_t Type>
		void delete_user_data(const std::uint64_t user_id)
		{
			return database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::remove_from(user::table)
						.where(user::table.user_id == user_id));
			});
		}

		template <database_type_t Type>
		bool generate_password(const std::uint64_t user_id, std::string& password)
		{
			password = auth::generate_data(16, false);
			const auto hash = utils::cryptography::md5::compute(password);
			const auto hash_b64 = utils::cryptography::base64::encode(hash);

			return database::access<bool>([&](database::database_t& db)
			{
				const auto result = db.exec<Type>(
					sqlpp::update(user::table)
						.set(user::table.password_hash = hash_b64)
							.where(user::table.user_id == user_id));
				return result != 0ull;
			});
		}

		template <database_type_t Type>
		bool generate_session(const std::uint64_t user_id, std::string& session_id, std::string& crypto_key)
		{
			session_id = auth::generate_data(16, false);
			crypto_key = auth::generate_data(16, true);

			return database::access<bool>([&](database::database_t& db)
			{
				const auto result = db.exec<Type>(
					sqlpp::update(user::table)
						.set(user::table.session_id = session_id,
							 user::table.crypto_key = crypto_key,
							 user::table.last_update = std::chrono::system_clock::now())
								.where(user::table.user_id == user_id));
				return result != 0ull;
			});
		}

		template <database_type_t Type>
		bool update_session(const std::uint64_t user_id)
		{
			return database::access<bool>([&](database::database_t& db)
			{
				const auto result = db.exec<Type>(
					sqlpp::update(user::table)
						.set(user::table.last_update = std::chrono::system_clock::now())
							.where(user::table.user_id == user_id));
				return result != 0;
			});
		}

		template <database_type_t Type>
		void set_ip_and_port(const std::uint64_t user_id, const std::string& ex_ip, const std::uint16_t ex_port,
			const std::string& in_ip, const std::uint16_t in_port, const std::string& nat_type)
		{
			const auto nat_type_id = get_nat_type_id(nat_type);

			database::access([&](database::database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(user::table)
						.set(user::table.ex_ip = ex_ip,
							 user::table.in_ip = in_ip,
							 user::table.ex_port = ex_port,
							 user::table.in_port = in_port,
							 user::table.nat = nat_type_id)
								.where(user::table.user_id == user_id));
			});
		}

		template <database_type_t Type>
		bool set_current_player(const std::uint64_t user_id, const std::uint64_t player_id)
		{
			return database::access<bool>([&](database::database_t& db)
			{
				const auto result = db.exec<Type>(
					sqlpp::update(user::table)
						.set(user::table.current_player_id = player_id)
							.where(user::table.user_id == user_id));
				return result != 0ull;
			});
		}

		template <database_type_t Type>
		bool reset_current_player(const std::uint64_t user_id)
		{
			return database::access<bool>([&](database::database_t& db)
			{
				const auto result = db.exec<Type>(
					sqlpp::update(user::table)
						.set(user::table.current_player_id = sqlpp::value_or_null<sqlpp::integer_unsigned>(sqlpp::null))
							.where(user::table.user_id == user_id));
				return result != 0ull;
			});
		}
		
		template <database_type_t Type>
		void set_user_flag(const std::uint64_t user_id, const std::uint32_t flag)
		{
			database::access([&](database::database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(user::table)
						.set(user::table.user_flag = flag)
							.where(user::table.user_id == user_id));
			});
		}
				
		template <database_type_t Type>
		void set_dlc_flag(const std::uint64_t user_id, const std::uint32_t flag)
		{
			database::access([&](database::database_t& db)
			{
				db.exec<Type>(
					sqlpp::update(user::table)
						.set(user::table.dlc_flag = flag)
							.where(user::table.user_id == user_id));
			});
		}

		template <database_type_t Type>
		std::uint64_t get_user_count()
		{
			return database::access<std::uint64_t>([&](database::database_t& db)
			{
				auto results = db.get_database<Type>()->operator()(
					sqlpp::select(
						sqlpp::count(1))
							.from(user::table).unconditionally());

				return results.front().count.value();
			});
		}

		template <database_type_t Type>
		std::uint64_t get_online_user_count(const std::chrono::milliseconds within)
		{
			return database::access<std::uint64_t>([&](database::database_t& db)
			{
				auto results = db.get_database<Type>()->operator()(
					sqlpp::select(
						sqlpp::count(1))
							.from(user::table)
								.where(user::table.last_update >= std::chrono::system_clock::now() - within));

				return results.front().count.value();
			});
		}

		template <database_type_t Type>
		std::uint32_t get_sv_coins(const std::uint64_t user_id)
		{
			return database::access<std::uint32_t>([&](database::database_t& db)
			{
				const auto result = db.get_database<Type>()->operator()(
					sqlpp::select(user::table.sv_coin)
							.from(user::table)
								.where(user::table.user_id == user_id)
				);

				if (result.empty())
				{
					return 0u;
				}

				return static_cast<std::uint32_t>(result.front().sv_coin.value());
			});
		}

		template <database_type_t Type>
		bool spend_sv_coins(const std::uint64_t user_id, const std::uint32_t value)
		{
			if (value == 0)
			{
				return true;
			}

			return database::access<bool>([&](database::database_t& db)
			{
				const auto result = db.get_database<Type>()->operator()(
					sqlpp::update(user::table)
						.set(user::table.sv_coin = user::table.sv_coin - value)
							.where(user::table.user_id == user_id &&
								   user::table.sv_coin >= value)
					);

				return result != 0;
			});
		}

		template <database_type_t Type>
		bool add_sv_coins(const std::uint64_t user_id, const std::uint32_t value)
		{
			if (value == 0)
			{
				return true;
			}

			return database::access<bool>([&](database::database_t& db)
			{
				const auto result = db.get_database<Type>()->operator()(
					sqlpp::update(user::table)
						.set(user::table.sv_coin = user::table.sv_coin + value)
							.where(user::table.user_id == user_id)
					);

				return result != 0;
			});
		}

		template <database_type_t Type>
		bool set_loadout_count(const std::uint64_t user_id, const std::uint32_t loadout_count)
		{
			return database::access<bool>([&](database_t& db)
			{
				const auto result = db.exec<Type>(
					sqlpp::update(user::table)
						.set(user::table.loadout_count = loadout_count)
							.where(user::table.user_id == user_id));
				return result != 0ull;
			});
		}
		
		template <database_type_t Type>
		bool inc_player_capacity(const std::uint64_t user_id)
		{
			return database::access<bool>([&](database_t& db)
			{
				const auto result = db.exec<Type>(
					sqlpp::update(user::table)
						.set(user::table.player_capacity = user::table.player_capacity + 1)
							.where(user::table.user_id == user_id && user::table.player_capacity < database::players::max_player_count));
				return result != 0ull;
			});
		}

		DEF_BINARY_GET(user, user_inventory_t, user_inventory);
		DEF_BINARY_GET(user, user_play_record_t, user_play_record);

		DEF_BINARY_SET(user, user_inventory_t, user_inventory);
		DEF_BINARY_SET(user, user_play_record_t, user_play_record);
	}

	void user::get_inventory(user_inventory_t& user_inventory) const
	{
		RUN_IMPL(impl::get_user_inventory, this->get_user_id(), user_inventory);
	}

	void user::get_play_record(user_play_record_t& play_record) const
	{
		RUN_IMPL(impl::get_user_play_record, this->get_user_id(), play_record);
	}

	bool user::set_inventory(user_inventory_t& user_inventory) const
	{
		RUN_IMPL(impl::set_user_inventory, this->get_user_id(), user_inventory);
	}

	bool user::set_play_record(user_play_record_t& play_record) const
	{
		RUN_IMPL(impl::set_user_play_record, this->get_user_id(), play_record);
	}

	void user::set_user_flag(const std::uint32_t flag) const
	{
		RUN_IMPL(impl::set_user_flag, this->get_user_id(), flag);
	}

	void user::set_dlc_flag(const std::uint32_t flag) const
	{
		RUN_IMPL(impl::set_dlc_flag, this->get_user_id(), flag);
	}

	bool user::spend_sv_coins(const std::uint32_t value) const
	{
		RUN_IMPL(impl::spend_sv_coins, this->get_user_id(), value);
	}

	bool user::add_sv_coins(const std::uint32_t value) const
	{
		RUN_IMPL(impl::add_sv_coins, this->get_user_id(), value);
	}

	bool user::set_loadout_count(const std::uint32_t loadout_count) const
	{
		RUN_IMPL(impl::set_loadout_count, this->get_user_id(), loadout_count);
	}

	bool user::inc_player_capacity() const
	{
		RUN_IMPL(impl::inc_player_capacity, this->get_user_id());
	}

	bool user::give_item(const game::item_t& reward, give_item_params_t& params, json::value& reward_info) const
	{
		switch (reward.category)
		{
		case game::ITEM_CATEGORY_RESOURCE:
		{
			if (params.resource_list == nullptr)
			{
				return false;
			}

			const auto iter = game::parameters_table.ssd_sbm_parameters->resources.find(reward.code);
			if (iter == game::parameters_table.ssd_sbm_parameters->resources.end())
			{
				return false;
			}

			database::players::inventory_resource_t resource{};
			resource.resource_index = iter->second->index;
			resource.count = static_cast<std::uint16_t>(reward.num);
			if (!params.resource_list->add_resource(resource))
			{
				return false;
			}

			if (reward_info.is_object())
			{
				auto& resources_list_j = reward_info["resources_list"];
				resource.count = static_cast<std::uint16_t>(reward.num);
				resource.to_json(resources_list_j[resources_list_j.size()]);
			}

			return true;
		}
		case game::ITEM_CATEGORY_PRODUCTION:
		{
			const auto iter = game::parameters_table.ssd_sbm_parameters->productions.find(reward.code);
			if (iter == game::parameters_table.ssd_sbm_parameters->productions.end())
			{
				return false;
			}

			if (params.user_inventory != nullptr)
			{
				params.user_inventory->set_obtained_generic(params.user_inventory->production_opened, iter->second->index);
			}

			if (iter->second->only_flag)
			{
				return params.user_inventory != nullptr;
			}

			if (iter->second->is_stackable())
			{
				if (params.stackable_list == nullptr)
				{
					return false;
				}

				database::players::stackable_item_t stackable_item{};
				stackable_item.production_index = iter->second->index;
				stackable_item.count = static_cast<std::uint16_t>(reward.num);

				if (!params.stackable_list->add_item(stackable_item))
				{
					return false;
				}

				if (reward_info.is_object())
				{
					auto& stackable_list_j = reward_info["stackable_list"];
					stackable_item.count = static_cast<std::uint16_t>(reward.num);
					stackable_item.to_json(stackable_list_j[stackable_list_j.size()]);
				}

				return true;
			}
			else
			{
				if (params.nonstackable_list == nullptr)
				{
					return false;
				}

				database::players::nonstackable_item_t nonstackable_item{};
				nonstackable_item.initialize(*iter->second);

				if (!params.nonstackable_list->add_item(nonstackable_item))
				{
					return false;
				}

				if (reward_info.is_object())
				{
					auto& stackable_list_j = reward_info["nonstackable_list"];
					nonstackable_item.to_json(stackable_list_j[stackable_list_j.size()]);
				}

				return true;
			}
		}
		case game::ITEM_CATEGORY_RECIPE:
		{
			if (params.user_inventory == nullptr)
			{
				return false;
			}

			params.user_inventory->set_obtained_recipe(reward.code);
			return true;
		}
		case game::ITEM_CATEGORY_PRESET_RADIO:
		{
			if (params.user_inventory == nullptr)
			{
				return false;
			}

			params.user_inventory->set_obtained_radio(reward.code);
			return true;
		}
		case game::ITEM_CATEGORY_GESTURE:
		{
			if (params.user_inventory == nullptr)
			{
				return false;
			}

			params.user_inventory->set_obtained_gesture(reward.code);
			return true;
		}
		case game::ITEM_CATEGORY_COMMUNICATION_MARKER:
		{
			if (params.user_inventory == nullptr)
			{
				return false;
			}

			params.user_inventory->set_obtained_marker(reward.code);
			return true;
		}
		case game::ITEM_CATEGORY_NAMEPLATE:
		{
			if (params.user_inventory == nullptr)
			{
				return false;
			}

			params.user_inventory->set_obtained_nameplate(reward.code);
			return true;
		}
		case game::ITEM_CATEGORY_PRIVILEGE:
		{
			return false;
		}
		case game::ITEM_CATEGORY_COIN:
		{
			return this->add_sv_coins(reward.num);
		}
		case game::ITEM_CATEGORY_ENERGY:
		{
			if (params.player_inventory == nullptr)
			{
				return false;
			}

			params.player_inventory->energy += reward.num;

			if (reward_info.is_object())
			{
				auto& energy_j = reward_info["energy"];
				if (energy_j.is_uint64())
				{
					energy_j = energy_j.as<std::uint32_t>() + reward.num;
				}
			}

			return true;
		}
		case game::ITEM_CATEGORY_BATTLE_PACK:
		{
			if (params.user_inventory == nullptr)
			{
				return false;
			}

			params.user_inventory->set_obtained_battle_pack(reward.code);
			return true;
		}
		case game::ITEM_CATEGORY_FACE_PAINT:
		{
			if (params.user_inventory == nullptr)
			{
				return false;
			}

			params.user_inventory->set_obtained_face_paint(reward.code);
			return true;
		}
		case game::ITEM_CATEGORY_CASSETTE:
		{
			if (params.user_inventory == nullptr)
			{
				return false;
			}

			params.user_inventory->set_obtained_cassette(reward.code);
			return true;
		}
		case game::ITEM_CATEGORY_CHARACTER_SLOT:
		{
			return this->inc_player_capacity();
		}
		}

		return false;
	}

	std::optional<players::player> user::create_additional_player() const
	{
		if (!this->current_player.has_value())
		{
			return {};
		}

		auto player = database::players::create(this->get_user_id());
		if (!player.has_value())
		{
			return player;
		}

		const auto team_count = database::deployments::get_team_count(this->current_player->get_player_id());
		for (auto i = 1ull; i < team_count; i++)
		{
			database::deployments::create_team(player->get_player_id());
		}

		return player;
	}

	bool add_sv_coins(const std::uint64_t user_id, const std::uint32_t value)
	{
		RUN_IMPL(impl::add_sv_coins, user_id, value);
	}

	std::uint32_t get_sv_coins(const std::uint64_t user_id)
	{
		RUN_IMPL(impl::get_sv_coins, user_id);
	}

	std::optional<user> find(const std::uint64_t user_id)
	{
		RUN_IMPL(impl::find, user_id);
	}

	bool exists(const std::uint64_t user_id)
	{
		RUN_IMPL(impl::exists, user_id);
	}

	std::optional<user> find_from_account(const std::uint64_t account_id)
	{
		RUN_IMPL(impl::find_from_account, account_id);
	}

	std::optional<user> find_from_session_id(const std::string session_id, bool use_timeout, bool* is_expired)
	{
		RUN_IMPL(impl::find_from_session_id, session_id, use_timeout, is_expired);
	}

	user find_or_insert(const std::uint64_t account_id)
	{
		RUN_IMPL(impl::find_or_insert, account_id);
	}

	bool generate_password(const std::uint64_t user_id, std::string& password)
	{
		RUN_IMPL(impl::generate_password, user_id, password);
	}

	bool generate_session(const std::uint64_t user_id, std::string& session_id, std::string& crypto_key)
	{
		RUN_IMPL(impl::generate_session, user_id, session_id, crypto_key);
	}

	void set_ip_and_port(const std::uint64_t user_id, const std::string& ex_ip, const std::uint16_t ex_port,
		const std::string& in_ip, const std::uint16_t in_port, const std::string& nat_type)
	{
		RUN_IMPL(impl::set_ip_and_port, user_id, ex_ip, ex_port, in_ip, in_port, nat_type);
	}

	bool update_session(const std::uint64_t user_id)
	{
		RUN_IMPL(impl::update_session, user_id);
	}

	bool set_current_player(const std::uint64_t user_id, const std::uint64_t player_id)
	{
		RUN_IMPL(impl::set_current_player, user_id, player_id);
	}

	bool reset_current_player(const std::uint64_t user_id)
	{
		RUN_IMPL(impl::reset_current_player, user_id);
	}

	std::uint64_t get_user_count()
	{
		RUN_IMPL(impl::get_user_count);
	}

	std::uint64_t get_online_user_count(const std::chrono::milliseconds within)
	{
		RUN_IMPL(impl::get_online_user_count, within);
	}

	std::uint64_t get_online_user_count()
	{
		return get_online_user_count(database::vars.session_timeout);
	}

	void delete_user_data(const std::uint64_t user_id)
	{
		RUN_IMPL(impl::delete_user_data, user_id);
	}

	bool delete_all_user_data(const std::uint64_t account_id)
	{
		const auto user = find_from_account(account_id);
		if (!user.has_value())
		{
			return false;
		}

		reset_current_player(user->get_id());

		const auto players = players::get_player_list(user->get_user_id());
		for (const auto& player : players)
		{
			defense_missions::delete_player_data(player.get_player_id());
			shop_purchases::delete_player_data(player.get_player_id());
			present_box::delete_player_data(player.get_player_id());
			deployments::delete_player_data(player.get_player_id());
			crew_members::delete_player_data(player.get_player_id());
			players::delete_player_data(player.get_player_id());
		}

		delete_user_data(user->get_user_id());

		return true;
	}

	class table final : public table_interface
	{
	public:
		void create(database_t& database) override
		{
			database.run_query("mgssd.users.create");
		}
	};
}

REGISTER_TABLE(database::users::table, -1)
