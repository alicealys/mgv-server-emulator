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

		virtual glz::json execute(glz::json& data, const std::optional<database::users::user>& user)
		{
			throw std::runtime_error("unimplemented command");
		};

		virtual std::uint32_t flags()
		{
			return 0u;
		}
	};

	glz::json error(const std::string& id);
	glz::json error(const std::uint32_t id);
	glz::json resource(const std::uint32_t id);
	glz::json player_info(const std::uint64_t player_id, const std::uint64_t account_id);
	glz::json player_info(const database::players::player& player);
	glz::json player_info(const std::optional<database::players::player>& player);
	void merge_json(glz::json& data, const glz::json& extra_data);
}
