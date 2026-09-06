#include <std_include.hpp>

#include "endpoints/var_handler.hpp"

#include "ssdstmweb_handler.hpp"

namespace emulator
{
	ssdstmweb_handler::ssdstmweb_handler()
	{
		this->register_handler<var_handler>("coin", "coin/coin.var", "null");
		this->register_handler<var_handler>("eula", "eula/eula.var", "null");
		this->register_handler<var_handler>("gdpr", "gdpr/privacy.var", "null");
		this->register_handler<var_handler>("privacy", "privacy/privacy.var", "null");
		this->register_handler<var_handler>("privacy_jp", "privacy_jp/privacy.var", "null");
	}
}
