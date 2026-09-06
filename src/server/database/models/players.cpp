#include <std_include.hpp>

#include "players.hpp"
#include "users.hpp"
#include "../auth.hpp"

#include "utils/encoding.hpp"

#include <utils/cryptography.hpp>
#include <utils/string.hpp>

namespace database::players
{
	namespace
	{
		namespace exp
		{
			const auto select = sqlpp::select(
				sqlpp::all_of(player::table), users::user::table.account_id)
					.from(player::table.join(users::user::table).on(users::user::table.user_id == player::table.f_user_id));
		}
	}

	void avatar_t::to_json(nlohmann::json& data)
	{
		data["accessory"] = this->accessory;
		data["beard_length"] = this->beard_length;
		data["beard_style"] = this->beard_style;
		data["eyebrow_length"] = this->eyebrow_length;
		data["eyebrow_style"] = this->eyebrow_style;
		data["hair_color"] = this->hair_color;
		data["hair_style"] = this->hair_style;
		data["left_eye_brightness"] = this->left_eye_brightness;
		data["left_eye_color"] = this->left_eye_color;
		data["player_parts_type"] = this->player_parts_type;
		data["player_type"] = this->player_type;
		data["race"] = this->race;
		data["race_color"] = this->race_color;
		data["race_type"] = this->race_type;
		data["race_variation"] = this->race_variation;
		data["right_eye_brightness"] = this->right_eye_brightness;
		data["right_eye_color"] = this->right_eye_color;
		data["tattoo"] = this->tattoo;
		data["tattoo_color"] = this->tattoo_color;
		data["voice"] = this->voice;
		data["voice_pitch"] = this->voice_pitch;
		data["name"] = utils::encoding::encode_base64(this->name);
		data["parameter"] = utils::encoding::encode_base64(this->motion_frame_list);
	}

	void loadout_t::to_json(nlohmann::json& data)
	{

	}

	void nonstackable_list_t::to_json(nlohmann::json& data)
	{
		for (auto i = 0; i < 4; i++)
		{
			auto& entry = data[i];
			entry["color"] = this->list[i].color;
			entry["color2"] = this->list[i].color2;
			entry["flag"] = this->list[i].flag;
			entry["grade"] = this->list[i].grade;
			entry["inventory_index"] = this->list[i].inventory_index;
			entry["life"] = this->list[i].life;
			entry["life_max"] = this->list[i].life_max;
			entry["obtain_order"] = this->list[i].obtain_order;
			entry["spec"] = this->list[i].spec;
			entry["option_slot"] = this->list[i].option_slot;
			entry["production_id"] = this->list[i].production_id;

			for (auto o = 0; o < 8; o++)
			{
				entry["option_list"][o]["perk_id"] = this->list[i].option_list[o].obtained;
				entry["option_list"][o]["perk_level"] = this->list[i].option_list[o].option_id;
			}

			for (auto o = 0; o < 5; o++)
			{
				entry["perk_list"][o]["perk_id"] = this->list[i].perk_list[o].perk_id;
				entry["perk_list"][o]["perk_level"] = this->list[i].perk_list[o].perk_level;
			}
		}
	}

	GET_FIELD_C(player, std::uint64_t, player_id);
	GET_FIELD_C(player, std::uint64_t, user_id);
	GET_FIELD_C(player, std::uint64_t, account_id);
	GET_FIELD_C(player, std::uint64_t, index);
	GET_FIELD_C(player, std::uint32_t, playtime);
	GET_FIELD_C(player, std::uint32_t, nameplate);
	GET_FIELD_C(player, std::uint32_t, point);
	GET_FIELD_C(player, std::chrono::microseconds, creation_date);

	std::string player::get_name() const
	{
		return std::format("{}_player{:02}", this->get_account_id(), this->get_index() + 1);
	}

	std::uint64_t player::get_id() const
	{
		return this->player_id_;
	}

	namespace impl
	{
		template <database_type_t Type>
		std::optional<player> find(const std::uint64_t player_id)
		{
			return database::access<std::optional<player>>([&](database::database_t& db)
				-> std::optional<player>
			{
				auto results = db.exec<Type>(exp::select.where(player::table.player_id == player_id));

				if (results.empty())
				{
					return {};
				}

				const auto& row = results.front();
				return {player(row)};
			});
		}

		template <database_type_t Type>
		std::optional<player> find_by_index(const std::uint64_t user_id, const std::uint64_t player_index)
		{
			return database::access<std::optional<player>>([&](database::database_t& db)
				-> std::optional<player>
			{
				auto results = db.exec<Type>(exp::select
					.where(player::table.f_user_id == user_id && player::table.player_index == player_index));

				if (results.empty())
				{
					return {};
				}

				const auto& row = results.front();
				return {player(row)};
			});
		}

		template <database_type_t Type>
		player create(const std::uint64_t user_id)
		{
			const auto id = database::access<std::uint64_t>([&](database::database_t& db)
			{
				return db.exec<Type>(
					sqlpp::insert_into(player::table)
						.set(player::table.f_user_id = user_id,
							 player::table.player_creation_date = std::chrono::system_clock::now()));
			});

			const auto found = find<Type>(id);
			if (!found.has_value())
			{
				throw std::runtime_error("[database::players::create] Insertion failed");
			}

			return found.value();
		}

		template <database_type_t Type>
		std::vector<player> get_player_list(const std::uint64_t user_id)
		{
			return database::access<std::vector<player>>([&](database::database_t& db)
				-> std::vector<player>
			{
				auto results = db.exec<Type>(exp::select.where(player::table.f_user_id == user_id));

				std::vector<player> list;

				for (auto& row : results)
				{
					list.emplace_back(row);
				}

				return list;
			});
		}

		template <database_type_t Type>
		void delete_player_data(const std::uint64_t player_id)
		{
			return database::access([&](database_t& db)
			{
				db.exec<Type>(
					sqlpp::remove_from(player::table)
						.where(player::table.player_id == player_id));
			});
		}

#define DEF_BINARY_GET(__type__, __name__) \
		template <database_type_t Type> \
		void get_##__name__(const std::uint64_t player_id, __type__& __name__) \
		{ \
			std::memset(&__name__, 0, sizeof(__type__)); \
			database::access([&](database::database_t& db) \
			{ \
				auto results = db.get_database<Type>()->operator()( \
					sqlpp::select(player::table.__name__) \
							.from(player::table) \
								.where(player::table.player_id == player_id)); \
				if (results.empty()) \
				{ \
					return; \
				} \
				const auto data = results.front().__name__.value(); \
				load_binary_field(&__name__, data); \
			}); \
		} \

#define DEF_BINARY_SET(__type__, __name__) \
		template <database_type_t Type> \
		bool set_##__name__(const std::uint64_t player_id, __type__& __name__) \
		{ \
			return database::access<bool>([&](database::database_t& db) \
			{ \
				auto result = db.get_database<Type>()->operator()( \
					sqlpp::update(player::table) \
							.set(player::table.__name__ = sqlpp::verbatim<sqlpp::binary>(utils::encoding::encode_binary(__name__))) \
								.where(player::table.player_id == player_id)); \
				return result != 0ull; \
			}); \
		} \
			
		DEF_BINARY_GET(avatar_t, avatar);
		DEF_BINARY_GET(loadout_t, loadout);
		DEF_BINARY_GET(mission_info_t, mission_info);
		DEF_BINARY_GET(inventory_t, inventory);
		DEF_BINARY_GET(nonstackable_list_t, nonstackable_list);

		DEF_BINARY_SET(avatar_t, avatar);
		DEF_BINARY_SET(loadout_t, loadout);
		DEF_BINARY_SET(mission_info_t, mission_info);
		DEF_BINARY_SET(inventory_t, inventory);
		DEF_BINARY_SET(nonstackable_list_t, nonstackable_list);
	}

	void player::get_avatar(avatar_t& avatar) const
	{
		RUN_IMPL(impl::get_avatar, this->get_player_id(), avatar);
	}

	void player::get_loadout(loadout_t& loadout) const
	{
		RUN_IMPL(impl::get_loadout, this->get_player_id(), loadout);
	}

	void player::get_mission_info(mission_info_t& mission_info) const
	{
		RUN_IMPL(impl::get_mission_info, this->get_player_id(), mission_info);
	}

	void player::get_inventory(inventory_t& inventory) const
	{
		RUN_IMPL(impl::get_inventory, this->get_player_id(), inventory);
	}

	void player::get_nonstackable_list(nonstackable_list_t& nonstackable_list) const
	{
		RUN_IMPL(impl::get_nonstackable_list, this->get_player_id(), nonstackable_list);
	}

	bool player::set_avatar(avatar_t& avatar) const
	{
		RUN_IMPL(impl::set_avatar, this->get_player_id(), avatar);
	}

	bool player::set_loadout(loadout_t& loadout) const
	{
		RUN_IMPL(impl::set_loadout, this->get_player_id(), loadout);
	}

	bool player::set_mission_info(mission_info_t& mission_info) const
	{
		RUN_IMPL(impl::set_mission_info, this->get_player_id(), mission_info);
	}

	bool player::set_inventory(inventory_t& inventory) const
	{
		RUN_IMPL(impl::set_inventory, this->get_player_id(), inventory);
	}

	bool player::set_nonstackable_list(nonstackable_list_t& nonstackable_list) const
	{
		RUN_IMPL(impl::set_nonstackable_list, this->get_player_id(), nonstackable_list);
	}

	std::optional<player> find(const std::uint64_t id)
	{
		RUN_IMPL(impl::find, id);
	}

	std::optional<player> find_by_index(const std::uint64_t user_id, const std::uint64_t player_index)
	{
		RUN_IMPL(impl::find_by_index, user_id, player_index);
	}

	std::vector<player> get_player_list(const std::uint64_t user_id)
	{
		RUN_IMPL(impl::get_player_list, user_id);
	}

	std::optional<player> create(const std::uint64_t user_id)
	{
		RUN_IMPL(impl::create, user_id);
	}

	void get_avatar(const std::uint64_t player_id, avatar_t& avatar)
	{
		RUN_IMPL(impl::get_avatar, player_id, avatar);
	}

	void get_loadout(const std::uint64_t player_id, loadout_t& loadout)
	{
		RUN_IMPL(impl::get_loadout, player_id, loadout);
	}

	void get_mission_info(const std::uint64_t player_id, mission_info_t& mission_info)
	{
		RUN_IMPL(impl::get_mission_info, player_id, mission_info);
	}

	void get_inventory(const std::uint64_t player_id, inventory_t& inventory)
	{
		RUN_IMPL(impl::get_inventory, player_id, inventory);
	}

	void get_nonstackable_list(const std::uint64_t player_id, nonstackable_list_t& nonstackable_list)
	{
		RUN_IMPL(impl::get_nonstackable_list, player_id, nonstackable_list);
	}

	bool set_avatar(const std::uint64_t player_id, avatar_t& avatar)
	{
		RUN_IMPL(impl::set_avatar, player_id, avatar);
	}

	bool set_loadout(const std::uint64_t player_id, loadout_t& loadout)
	{
		RUN_IMPL(impl::set_loadout, player_id, loadout);
	}

	bool set_mission_info(const std::uint64_t player_id, mission_info_t& mission_info)
	{
		RUN_IMPL(impl::set_mission_info, player_id, mission_info);
	}

	bool set_inventory(const std::uint64_t player_id, inventory_t& inventory)
	{
		RUN_IMPL(impl::set_inventory, player_id, inventory);
	}

	bool set_nonstackable_list(const std::uint64_t player_id, nonstackable_list_t& nonstackable_list)
	{
		RUN_IMPL(impl::set_nonstackable_list, player_id, nonstackable_list);
	}

	void delete_player_data(const std::uint64_t player_id)
	{
		RUN_IMPL(impl::delete_player_data, player_id);
	}

	class table final : public table_interface
	{
	public:
		void create(database_t& database) override
		{
			database.run_query("mgssd.players.create");
			database.run_query("mgssd.players.remove_insert_trigger");
			database.run_query("mgssd.players.add_insert_trigger");

			try
			{
				database.run_query("mgssd.users.add_player_foreign_key");
			}
			catch (...)
			{
			}

			database.run_query("mgssd.users.remove_update_trigger");
			database.run_query("mgssd.users.add_update_trigger");
		}
	};
}

REGISTER_TABLE(database::players::table, 1)
