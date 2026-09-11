#include <std_include.hpp>

#include "players.hpp"
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
						players::player::table.loadout_count,
						players::player::table.current_loadout,
						players::player::table.player_creation_date)
					.from(user::table.left_outer_join(players::player::table).on(user::table.user_id == players::player::table.f_user_id));
		}

		const nlohmann::json& get_default_data()
		{
			static const auto data = utils::resources::load_json(RESOURCE_DEFAULT_DATA);
			return data;
		}

		const nlohmann::json& get_default_data(const std::string_view& key)
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

			data.bgm_my_list_setting = default_data["bgm_my_list_setting"].get<std::uint8_t>();

			return &data;
		}();

		std::memcpy(this, default_inventory, sizeof(user_inventory_t));
	}

	void user_inventory_t::to_json(nlohmann::json& data)
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
	GET_FIELD_C(user, std::chrono::microseconds, last_update);
	GET_FIELD_C(user, std::chrono::microseconds, creation_date);

	bool user_inventory_t::parse_save(nlohmann::json& data)
	{
		const auto try_parse_part = [&]<typename T>(const std::string_view& name, T& dest)
		{
			auto& value_j = data[name];
			if (!value_j.is_array() || value_j.size() <= 0)
			{
				return;
			}

			value_j = value_j[0];
			if (!value_j.is_object())
			{
				return;
			}

			auto& data_j = value_j[name];
			if (!data_j.is_string())
			{
				return;
			}

			utils::json::parse_base64(data_j, dest, true);
		};

		try_parse_part("archive_new", this->archive_new);
		try_parse_part("archive_obtained", this->archive_obtained);
		try_parse_part("battle_pack_opened", this->battle_pack_opened);
		try_parse_part("cassette_new", this->cassette_new);
		try_parse_part("cassette_obtained", this->cassette_obtained);
		try_parse_part("command_marker_new", this->command_marker_new);
		try_parse_part("command_marker_obtained", this->command_marker_obtained);
		try_parse_part("face_paint_new", this->face_paint_new);
		try_parse_part("face_paint_obtained", this->face_paint_obtained);
		try_parse_part("food_used", this->food_used);
		try_parse_part("gesture_new", this->gesture_new);
		try_parse_part("gesture_obtained", this->gesture_obtained);
		try_parse_part("name_plate_new", this->name_plate_new);
		try_parse_part("name_plate_obtained", this->name_plate_obtained);
		try_parse_part("preset_radio_new", this->preset_radio_new);
		try_parse_part("preset_radio_obtained", this->preset_radio_obtained);
		try_parse_part("production_opened", this->production_opened);
		try_parse_part("recipe_new", this->recipe_new);
		try_parse_part("recipe_new_for_db", this->recipe_new_for_db);
		try_parse_part("recipe_opened", this->recipe_opened);
		try_parse_part("recipe_used", this->recipe_used);
		try_parse_part("resource_opened", this->resource_opened);

		utils::json::get_or(data["bgm_my_list_setting"], this->bgm_my_list_setting, this->bgm_my_list_setting);
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
		std::optional<user> find_by_session_id(const std::string session_id, bool use_timeout, bool* is_expired)
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
							 user::table.currency = "EUR",
							 user::table.user_inventory = default_inventory,
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
		bool update_session(const user& user)
		{
			size_t result = 0;
			if (!is_session_expired(user))
			{
				database::access([&](database::database_t& db)
				{
					result = db.exec<Type>(
						sqlpp::update(user::table)
							.set(user::table.last_update = std::chrono::system_clock::now())
								.where(user::table.user_id == user.get_user_id()));
				});
			}
			else
			{
				database::access([&](database::database_t& db)
				{
					result = db.exec<Type>(
						sqlpp::update(user::table)
							.set(user::table.last_update = std::chrono::system_clock::now())
									.where(user::table.user_id == user.get_user_id()));
				});
			}

			return result != 0;
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

	std::optional<user> find_by_session_id(const std::string session_id, bool use_timeout, bool* is_expired)
	{
		RUN_IMPL(impl::find_by_session_id, session_id, use_timeout, is_expired);
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

	bool update_session(const user& user)
	{
		RUN_IMPL(impl::update_session, user);
	}

	bool set_current_player(const std::uint64_t user_id, const std::uint64_t player_id)
	{
		RUN_IMPL(impl::set_current_player, user_id, player_id);
	}

	bool reset_current_player(const std::uint64_t user_id)
	{
		RUN_IMPL(impl::reset_current_player, user_id);
	}

	void delete_user_data(const std::uint64_t user_id)
	{
		RUN_IMPL(impl::delete_user_data, user_id);
	}

	bool delete_all_user_data(const std::uint64_t account_id)
	{
		const auto user = find(account_id);
		if (!user.has_value())
		{
			return false;
		}

		reset_current_player(user->get_id());

		const auto players = players::get_player_list(user->get_user_id());
		for (const auto& player : players)
		{
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

REGISTER_TABLE(database::users::table, 2)
