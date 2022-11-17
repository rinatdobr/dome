#include "statisticrequester.h"

#include <spdlog/spdlog.h>

#include <utils.h>
#include <sstream>

namespace dome {
namespace data {

const std::string StatisticRequestStr("/statistic");

StatisticRequester::StatisticRequester(const std::string &dbPath, const std::vector<dome::config::Provider> &providers)
    : m_dbPath(dbPath)
    , m_providers(providers)
{
    spdlog::trace("{}:{} {} dbPath={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, dbPath);
}

StatisticRequester::~StatisticRequester()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void StatisticRequester::process(dome::mosq::Mosquitto &mosq, const dome::config::Provider &provider, nlohmann::json &jMessage)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (jMessage["type"] == "request") {
        spdlog::debug("request processing...");
        nlohmann::json jSource = nlohmann::json::parse(jMessage[provider.sources()[0].id].get<std::string>());
        std::string text = jSource["request"].get<std::string>();
        std::size_t delimetr = text.find(' ');
        std::string name(text, 0, delimetr);
        if (StatisticRequestStr != name) {
            spdlog::trace("ignore...");
            return;
        }

        auto args = ParseArgs(std::string(text, delimetr + 1));

        auto statistic = std::make_shared<Statistic>();
        statistic->type = Request::Type::Statistic;
        statistic->idFrom = provider.id();
        statistic->args = args;
        statistic->reply["type"] = StatisticRequestStr;
        statistic->reply["message_id"] = jSource["message_id"];
        statistic->reply["chat_id"] = jSource["chat_id"];

        uint periodSec = 3600;
        std::string location;
        std::string tables;
        std::string lables;
        // 0 - period
        if (args.size() == 1) {
            periodSec = PeriodToSeconds(args[0]);
        }
        // 0 - location
        // 1 - period
        else if (args.size() == 2) {
            location = args[0];
            periodSec = PeriodToSeconds(args[1]);
        }

        for (const auto &provider : m_providers) {
            if (location.empty() || provider.location() == location) {
                for (const auto &source : provider.sources()) {
                    auto type = dome::config::Source::TypeToStr(source.type);
                    if (source.type == dome::config::Source::Type::Temperature ||
                        source.type == dome::config::Source::Type::Humidity) {
                        tables += source.id + " ";
                        lables += type + " ";
                    }
                }
            }
        }

        auto outFile = GetTmpName("chart");

        std::ostringstream cmd;
        cmd << "python" << " "
        << "dome_plotdrawer.py" << " "
        << "-d" << " " << m_dbPath << " "
        << "-p" << " " << periodSec << " "
        << "-l" << " " << "Date" << " " << "Value" << " "
        << "-n" << " " << "\"\"" << " "
        << "-t" << " " << tables << " "
        << "-y" << " " << lables << " "
        << "-f" << " " << outFile;

        auto execRes = Exec(cmd.str().c_str());
        if (execRes.first == 0) {
            statistic->reply["path"] = execRes.second;
            std::string data = statistic->reply.dump();
            spdlog::debug("replying...");
            int res = mosquitto_publish(mosq.mosq(), nullptr, GetReplyTopic(statistic->idFrom).c_str(), data.size(), data.c_str(), 0, false);
            if (res != MOSQ_ERR_SUCCESS) {
                spdlog::error("mosquitto_publish to \"{}\" error[{}]: {}", GetReplyTopic(provider.id()), res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
            }
        }
        else {
            spdlog::error("command {} was exited with code {}", cmd.str(), execRes.first);
        }
    }
}

}
}