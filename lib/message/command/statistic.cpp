#include "statistic.h"

#include <spdlog/spdlog.h>

#include "message/message.h"
#include "utils/utils.h"
#include <sstream>
#include <set>

namespace dome {
namespace message {

Statistic::Statistic(const std::string &dbPath, const std::vector<dome::config::EndPoint> &endPointConfigs)
    : m_dbPath(dbPath)
    , m_endPointConfigs(endPointConfigs)
{
    spdlog::trace("{}:{} {} dbPath=\"{}\"", __FILE__, __LINE__, __PRETTY_FUNCTION__, dbPath);
}

Statistic::~Statistic()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void Statistic::process(dome::mosq::Mosquitto &mosq, const dome::config::EndPoint &endPointConfig, nlohmann::json &jMessage)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!CheckJsonMessageForKeys(jMessage, { "type" })) return;

    if (jMessage["type"] == dome::message::type::Command) {
        spdlog::debug("command processing...");

        if (!CheckJsonMessageForKeys(jMessage, { endPointConfig.sources()[0].id() })) return;
        nlohmann::json jSource = nlohmann::json::parse(jMessage[endPointConfig.sources()[0].id()].get<std::string>());

        if (!CheckJsonMessageForKeys(jSource, { "body" })) return;
        std::string body = jSource["body"].get<std::string>();
        std::size_t delimetr = body.find(' ');
        std::string name(body, 0, delimetr);
        if (dome::message::command::Statistic != name) {
            spdlog::trace("ignore...");
            return;
        }
        if (!CheckJsonMessageForKeys(jSource, { "message_id", "chat_id" })) return;

        auto args = parseArgs(std::string(body, delimetr + 1));

        auto reply = std::make_shared<StatisticReply>();
        reply->idFrom = endPointConfig.id();
        reply->body["type"] = dome::message::type::Reply;
        reply->body["reply"] = dome::message::command::Statistic;
        reply->body["message_id"] = jSource["message_id"];
        reply->body["chat_id"] = jSource["chat_id"];

        uint periodSec = 3600;
        std::string location;
        std::string tables;
        std::string lables;
        std::set<dome::config::Source::Type> sourceTypes;
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
        // 0 - location
        // 1 - period
        // 2 - sources type
        else if (args.size() == 3) {
            location = args[0];
            periodSec = PeriodToSeconds(args[1]);
            std::istringstream sourceTypesStream(args[2]);
            std::string sourceType;
            while (std::getline(sourceTypesStream, sourceType, '&')) {
                sourceTypes.insert(dome::config::Source::StrToType(sourceType));
            }
        }

        for (const auto &endPointConfig : m_endPointConfigs) {
            if (location.empty() || endPointConfig.location() == location) {
                for (const auto &source : endPointConfig.sources()) {
                    auto type = dome::config::Source::TypeToStr(source.type());
                    if ((sourceTypes.empty() && (source.type() == dome::config::Source::Type::Temperature ||
                                                source.type() == dome::config::Source::Type::Humidity ||
                                                source.type() == dome::config::Source::Type::Co2))
                            ||
                        sourceTypes.count(source.type())
                        ) {
                        tables += source.id() + " ";
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
            reply->body["path"] = execRes.second;
            std::string data = reply->body.dump();
            spdlog::debug("replying to \"{}\" from \"{}\"...", GetReplyTopic(reply->idFrom), mosq.clientId());
            mosq.publish(GetReplyTopic(reply->idFrom), data);
        }
        else {
            spdlog::error("Command \"{}\" was exited with code {}", cmd.str(), execRes.first);
        }
    }
}

}
}
