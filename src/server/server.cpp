#include <std_include.hpp>

#include "loader/component_loader.hpp"

#include "types/server.hpp"
#include "database/database.hpp"
#include "database/auth.hpp"
#include "game/parameters.hpp"

#include "component/console.hpp"
#include "component/command.hpp"

struct crew_member_type_t
{
	std::uint16_t face;
	std::uint16_t race;
	std::uint16_t sex;
	std::uint16_t body;
	std::uint16_t develop;
	std::uint16_t food;
	std::uint16_t medic;
	std::uint16_t farm;
	std::uint16_t base_defense;
	std::uint16_t combat_deploy;
	std::uint16_t thirst_resist;
	std::uint16_t hunger_resist;
	std::uint16_t injury_resist;
	std::uint16_t sick_resist;
	std::uint16_t sleeplack_resist;
	std::uint16_t life;
	std::uint16_t job_table;
	std::uint16_t skill_table;
	std::uint32_t id;
	std::string id_str;
};

namespace emulator
{
	namespace
	{
		std::atomic_bool killed;
		std::chrono::system_clock::time_point startup;
	}

	std::chrono::system_clock::time_point get_server_startup()
	{
		return startup;
	}

	void stop_server()
	{
		killed = true;
	}

	void start_server()
	{
		const auto _0 = gsl::finally(&component_loader::pre_destroy);
		component_loader::pre_start();

		std::vector<std::thread> threads;

		try
		{
			database::initialize();
		}
		catch (const std::exception& e)
		{
			console::error("Failed to initialize database: %s\n", e.what());
			return;
		}

		auth::initialize_lists();
		game::initialize_parameters_table();

		server s;
		if (!s.start())
		{
			console::error("Failed to start server (%i)\n", errno);
			return;
		}

		startup = std::chrono::system_clock::now();

		threads.emplace_back([&]
		{
			while (!killed)
			{
				s.run_frame();
			}
		});

		threads.emplace_back([]
		{
			try
			{
				while (!killed)
				{
					database::run_tasks();
					std::this_thread::sleep_for(100ms);
				}
			}
			catch (const std::exception& e)
			{
				console::error("database thread: %s\n", e.what());
				killed = true;
			}
		});

		database::post_start();
		component_loader::post_start();

		while (!killed)
		{
			command::run_frame();
			std::this_thread::sleep_for(1ms);
		}

		database::stop();

		for (auto& thread : threads)
		{
			if (thread.joinable())
			{
				thread.join();
			}
		}
	}
}
