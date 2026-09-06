#include <std_include.hpp>

#include "cmd_get_urllist.hpp"

#include "utils/config.hpp"

namespace emulator::ssd
{
	namespace
	{
		struct url_t
		{
			std::string type;
			std::string url;
			std::uint32_t version;
			bool replace_hostname = false;
		};

		std::vector<url_t> url_list =
		{
            {
                .type = "GATE",
                .url = "ssdstm/gate",
                .version = 3,
                .replace_hostname = true
            },
            {
                .type = "WEB",
                .url = "ssdstm/main",
                .version = 19,
                .replace_hostname = true
            },
            {
                .type = "EULA",
                .url = "ssdstmweb/eula/eula.var",
                .version = 3,
                .replace_hostname = true
            },
            {
                .type = "EULA_COIN",
                .url = "ssdstmweb/coin/coin.var",
                .version = 2,
                .replace_hostname = true
            },
            {
                .type = "POLICY_GDPR",
                .url = "ssdstmweb/gdpr/privacy.var",
                .version = 2,
                .replace_hostname = true
            },
            {
                .type = "POLICY_JP",
                .url = "ssdstmweb/privacy_jp/privacy.var",
                .version = 4,
                .replace_hostname = true
            },
            {
                .type = "POLICY_ELSE",
                .url = "ssdstmweb/privacy/privacy.var",
                .version = 3,
                .replace_hostname = true
            },
            {
                .type = "LEGAL",
                .url = "https://legal.konami.com/games/survive/",
                .version = 1
            },
            {
                .type = "PERMISSION",
                .url = "https://www.konami.com/",
                .version = 0
            },
            {
                .type = "POLICY_CCPA",
                .url = "ssdstmweb/privacy_ccpa/privacy.var",
                .version = 1,
                .replace_hostname = true
            },
            {
                .type = "EULA_TEXT",
                .url = "https://legal.konami.com/games/survive/terms/",
                .version = 1
            },
            {
                .type = "EULA_COIN_TEXT",
                .url = "https://legal.konami.com/games/survive/terms/currency/",
                .version = 1
            },
            {
                .type = "POLICY_GDPR_TEXT",
                .url = "https://legal.konami.com/games/survive/",
                .version = 1
            },
            {
                .type = "POLICY_JP_TEXT",
                .url = "https://legal.konami.com/games/privacy/view/",
                .version = 2
            },
            {
                .type = "POLICY_ELSE_TEXT",
                .url = "https://legal.konami.com/games/privacy/view/",
                .version = 1
            },
            {
                .type = "POLICY_CCPA_TEXT",
                .url = "https://legal.konami.com/games/survive/ppa4ca/",
                .version = 1
            }
		};
	}

	cmd_get_urllist::cmd_get_urllist()
	{
		const auto base_url = config::get<std::string>("base_url");
		for (auto& url : url_list)
		{
			if (url.replace_hostname)
			{
				url.url = std::format("{}/{}", base_url, url.url);
			}
		}
	}

	nlohmann::json cmd_get_urllist::execute(nlohmann::json& data, const std::optional<database::users::user>&)
	{
		nlohmann::json result;

		for (auto i = 0ull; i < url_list.size(); i++)
		{
			result["url_list"][i]["type"] = url_list[i].type;
			result["url_list"][i]["url"] = url_list[i].url;
			result["url_list"][i]["version"] = url_list[i].version;

			if (url_list[i].type == "GATE" || url_list[i].type == "WEB")
			{
				result["url_list"][i]["version"] = database::vars.server_version;
			}
		}

        result["url_num"] = url_list.size();
        result["server_name"] = "DAY900(DNS:(*'v'*))"; // wtf is this

		return result;
	}
}
