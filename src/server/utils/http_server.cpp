#include <std_include.hpp>

#include "http_server.hpp"

#include "component/console.hpp"
#include "utils/config.hpp"

#include <utils/io.hpp>

namespace utils
{
	namespace
	{
		//std::string_view mg_str_to_view(const std::string& new_string, mg_str source, mg_str str)
		//{
		//	const auto offset = str.buf - source.buf;
		//	return {new_string.data() + offset, str.len};
		//}

		std::string mg_str_copy(mg_str str)
		{
			return {str.buf, str.len};
		}

		mg_str str_to_mg_str(std::string& str)
		{
			return mg_str{str.data(), str.size()};
		}

		void request_to_http_message(request_params& request, mg_http_message& message)
		{
			message.body = str_to_mg_str(request.body);
			message.uri = str_to_mg_str(request.uri);

			for (auto i = 0ull; i < request.headers.size(); i++)
			{
				message.headers[i].name = str_to_mg_str(request.headers[i].key);
				message.headers[i].value = str_to_mg_str(request.headers[i].value);
			}
		}

		bool parse_client_ip(mg_connection* c, mg_http_message* http_message, std::uint8_t* ip)
		{
			static const auto& client_ip_header = config::get().http_client_ip_header;

			if (!client_ip_header.empty())
			{
				const auto header_value = mg_http_get_header(http_message, client_ip_header.data());
				if (header_value == nullptr)
				{
					return false;
				}

				const auto address = std::string{header_value->buf, header_value->len};
				return inet_pton(AF_INET, address.data(), ip);
			}
			else
			{
				ip[0] = c->rem.addr.ip[0];
				ip[1] = c->rem.addr.ip[1];
				ip[2] = c->rem.addr.ip[2];
				ip[3] = c->rem.addr.ip[3];
			}

			return true;
		}

		void parse_query_params(mg_http_message* msg, std::unordered_map<std::string, std::string>& query)
		{
			mg_str entry{};
			mg_str k{};
			mg_str v{};
			while (mg_span(msg->query, &entry, &msg->query, '&'))
			{
				if (mg_span(entry, &k, &v, '='))
				{
					const auto key = std::string{k.buf, k.len};
					std::string value(0x100, 0);
					const auto res = mg_url_decode(v.buf, v.len, value.data(), value.size(), 1);
					if (res != -1)
					{
						value.resize(res);
					}
					query.insert(std::make_pair(key, value));
				}
			}
		}

		void parse_request_headers(mg_http_message* msg, std::vector<request_params::header_t>& headers)
		{
			for (auto i = 0ull; i < ARRAYSIZE(msg->headers); i++)
			{
				if (msg->headers[i].value.len == 0)
				{
					continue;
				}

				headers.emplace_back(
					mg_str_copy(msg->headers[i].name),
					mg_str_copy(msg->headers[i].value));
			}
		}

		static void mg_wakeup_handler(struct mg_connection* c, int ev, void* ev_data) 
		{
			if (ev == MG_EV_READ) 
			{
				auto id = reinterpret_cast<unsigned long*>(c->recv.buf);

				if (c->recv.len >= sizeof(*id)) 
				{
					mg_connection* t{};

					auto data = mg_str_n(
						reinterpret_cast<char*>(c->recv.buf) + sizeof(*id),
						c->recv.len - sizeof(*id)
					);

					for (t = c->mgr->conns; t != NULL; t = t->next) 
					{
						if (t->id == *id) 
						{
							mg_call(t, MG_EV_WAKEUP, &data);
						}
					}

					if (data.len == sizeof(task_data_t*))
					{
						auto task_data = *reinterpret_cast<task_data_t**>(data.buf);
						delete task_data;
					}

					console::debug("[HTTP Server] Delete task data\n");
				}

				c->recv.len = 0;
			}
			else if (ev == MG_EV_CLOSE) 
			{
#ifdef WIN32
				closesocket(c->mgr->pipe.fd);
#else
				close(c->mgr->pipe.fd);
#endif
				c->mgr->pipe.fd = MG_INVALID_SOCKET;
			}
		}
	}

	std::optional<std::string> request_query::get(const std::string& key) const
	{
		const auto iter = this->find(key);
		if (iter == this->end())
		{
			return {};
		}

		return {iter->second};
	}

	std::optional<std::int32_t> request_query::get_int(const std::string& key) const
	{
		const auto iter = this->find(key);
		if (iter == this->end())
		{
			return {};
		}

		const auto value = std::atoi(iter->second.data());
		return {value};
	}

	std::optional<std::uint32_t> request_query::get_uint(const std::string& key) const
	{
		const auto iter = this->find(key);
		if (iter == this->end())
		{
			return {};
		}

		const auto value = std::strtoul(iter->second.data(), nullptr, 0);
		return {static_cast<std::uint32_t>(value)};
	}

	std::optional<std::uint64_t> request_query::get_uint64(const std::string& key) const
	{
		const auto iter = this->find(key);
		if (iter == this->end())
		{
			return {};
		}

		const auto value = std::strtoull(iter->second.data(), nullptr, 0);
		return {value};
	}

	http_server::http_server()
		: thread_pool_(100)
	{
		this->set_ports(80, 443);
	}

	http_server::~http_server()
	{
		this->shutdown();
	}

	void http_server::set_request_handler(const event_handler_t& handler)
	{
		this->request_handler.emplace(handler);
	}

	void http_server::event_handler(mg_connection* c, int ev, void* ev_data)
	{
		const auto inst = reinterpret_cast<http_server*>(c->fn_data);

		switch (ev)
		{
		case MG_EV_ACCEPT:
		{
			if (inst->using_tls_)
			{
				mg_tls_init(c, &inst->tls_options_);
			}
			break;
		}
		case MG_EV_HTTP_MSG:
		{
			const auto http_message = static_cast<mg_http_message*>(ev_data);
			if (!inst->request_handler.has_value())
			{
				mg_http_reply(c, 500, "", "");
				break;
			}

			auto task_data = new task_data_t();
			task_data->request.body = mg_str_copy(http_message->body);
			task_data->request.uri = mg_str_copy(http_message->uri);
			task_data->request.address.is_valid = parse_client_ip(c, http_message, task_data->request.address.ip);
			parse_query_params(http_message, task_data->request.query);
			parse_request_headers(http_message, task_data->request.headers);

#ifdef DEBUG
			static std::atomic_int64_t thread_index;
			const auto index = thread_index++;
			const auto start = std::chrono::high_resolution_clock::now();
			console::debug("[HTTP Server] [Request %lli] Started\n", index);
#endif
			const auto id = c->id;

#ifdef DEBUG
			console::debug("[HTTP Server] Create task data\n");
			inst->thread_pool_.push([task_data, inst, id, start, index]()
#else
			inst->thread_pool_.push([task_data, inst, id]()
#endif
			{
				inst->request_handler->operator()(task_data->request, task_data->response);
				mg_wakeup(&inst->manager_, id, &task_data, sizeof(task_data));
#ifdef DEBUG
				const auto now = std::chrono::high_resolution_clock::now();
				console::debug("[HTTP Server] [Request %lli] Finished in %lli msec\n", index,
					std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count());
#endif
			});

			break;
		}
		case MG_EV_WAKEUP:
		{
			auto data = reinterpret_cast<mg_str*>(ev_data);
			auto task_data = *reinterpret_cast<task_data_t**>(data->buf);
			switch (task_data->response.type)
			{
			case response_file:
			{
				mg_http_serve_opts options{};
				mg_http_message http_message{};
				request_to_http_message(task_data->request, http_message);
				mg_http_serve_file(c, &http_message, task_data->response.body.data(), &options);
				break;
			}
			default:
			case response_text:
				mg_http_reply(c, task_data->response.code, task_data->response.headers.data(), "%s", task_data->response.body.data());
				break;
			}
			break;
		}
		case MG_EV_CLOSE:
		{
			console::debug("[HTTP Server] Close connection %i\n", c->id);
			break;
		}
		}
	}

	void http_server::set_ports(const std::uint16_t http_port, const std::uint16_t https_port)
	{
		this->http_port_ = http_port;
		this->https_port_ = https_port;

		this->http_url_ = std::format("http://0.0.0.0:{}", http_port);
		this->https_url_ = std::format("https://0.0.0.0:{}", https_port);
	}

	void http_server::set_tls(const std::string& cert, const std::string& key)
	{
		this->tls_cert_ = utils::io::read_file(cert);
		this->tls_key_ = utils::io::read_file(key);

		this->tls_options_.cert = mg_str_n(this->tls_cert_.data(), this->tls_cert_.size());
		this->tls_options_.ca = mg_str_n(this->tls_key_.data(), this->tls_key_.size());

		this->using_tls_ = true;
	}

	bool http_server::start()
	{
		mg_mgr_init(&this->manager_);

#ifdef DEBUG
		mg_log_set(MG_LL_DEBUG);
#else
		mg_log_set(MG_LL_ERROR);
#endif

		if (this->using_tls_)
		{
			mg_http_listen(&this->manager_, this->https_url_.data(), http_server::event_handler, this);
		}

		const auto conn = mg_http_listen(&this->manager_, this->http_url_.data(), http_server::event_handler, this);

		mg_wakeup_init(&this->manager_);
		this->manager_.conns->fn = mg_wakeup_handler;

		if (conn != nullptr)
		{
			this->thread_pool_.start();
			return true;
		}

		return false;
	}

	void http_server::run_frame()
	{
		mg_mgr_poll(&this->manager_, 100);
		this->thread_pool_.update();
	}

	void http_server::shutdown()
	{
		mg_mgr_free(&this->manager_);
		this->thread_pool_.stop();
	}
}
