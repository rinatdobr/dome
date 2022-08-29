#include "statistic.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <commands/parser.h>
#include <utils.h>

namespace dome {
namespace config {

Statistic::Config::Config(const std::string &name,
                          const std::string &period,
                          const std::vector<std::string> &args,
                          const std::string &outputType,
                          const std::string &outputName)
    : m_name(name)
    , m_period(period)
    , m_args(args)
    , m_outputType(outputType)
    , m_outputName(outputName)
{
    spdlog::trace("{}:{} {} name={} period={} args.size()={} outputType={} outputName={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, name, period, args.size(), outputType, outputName);
}

Statistic::Statistic(const std::string &configPath)
    : File(configPath)
{
    spdlog::trace("{}:{} {} configPath={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, configPath);

    parse();
}

const std::vector<Statistic::Config> Statistic::statisticConfig() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_statistic;
}

void Statistic::parse()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    auto configData = read();
    nlohmann::json jConfig = nlohmann::json::parse(configData);

    auto jCommands = jConfig["commands"];
    for(const auto &jCommand : jCommands) {
        if (jCommand.size() != 5) {
            spdlog::error("Invalid command format: \" {} \"", jCommand.dump());
            continue;
        }

        std::string name = jCommand["name"].get<std::string>();
        std::string period = jCommand["period"].get<std::string>();
        std::string args = jCommand["args"].get<std::string>();
        std::string outputType = jCommand["output_type"].get<std::string>();
        std::string outputName = jCommand["output_name"].get<std::string>();
        spdlog::info("Statistic command config: name={} period={} args={} outputType={} outputName={}", name, period, args, outputType, outputName);

        m_statistic.push_back(
            Config(name, period, {}, outputType, outputName)
        );
    }
}

}
}
