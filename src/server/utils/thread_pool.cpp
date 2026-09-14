#include <std_include.hpp>

#include "thread_pool.hpp"

namespace utils
{
	thread_pool::worker::worker()
	{
	}

	void thread_pool::worker::start(thread_pool& pool)
	{
		this->thread_ = std::thread([&]
		{
			this->loop(pool);
		});
	}

	void thread_pool::worker::stop()
	{
		if (this->thread_.joinable())
		{
			this->thread_.join();
		}
	}

	void thread_pool::worker::loop(thread_pool& pool)
	{
		while (!pool.stopped_)
		{
			pool.run_job();
		}
	}

	thread_pool::thread_pool(const std::size_t num_workers)
	{
		for (auto i = 0u; i < num_workers; i++)
		{
			this->workers_.emplace_back(std::make_unique<thread_pool::worker>());
		}
	}

	void thread_pool::start()
	{
		for (auto& worker : this->workers_)
		{
			worker->start(*this);
		}
	}

	void thread_pool::update()
	{

	}

	void thread_pool::stop()
	{
		this->stopped_ = true;
		this->event_.notify_all();
		for (auto& worker : this->workers_)
		{
			worker->stop();
		}
	}
}
