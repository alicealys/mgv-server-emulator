#pragma once

#include "database/models/players.hpp"
#include "database/models/users.hpp"
#include "game/game.hpp"

namespace emulator
{
	enum command_flags_t
	{
		CMD_NEEDS_USER = (1 << 0),
		CMD_NEEDS_PLAYER = (1 << 1),
	};

	class command_handler
	{
	public:
		virtual ~command_handler()
		{
		}

		virtual json::value execute(json::value& data, const std::optional<database::users::user>& user)
		{
			throw std::runtime_error("unimplemented command");
		};

		virtual std::uint32_t flags()
		{
			return 0u;
		}
	};

	json::value error(const std::string& id);
	json::value error(const std::uint32_t id);
	json::value resource(const std::uint32_t id);
	json::value player_info(const std::uint64_t player_id, const std::uint64_t account_id);
	json::value player_info(const database::players::player& player);
	json::value player_info(const std::optional<database::players::player>& player);
	void merge_json(json::value& data, const json::value& extra_data);
}
