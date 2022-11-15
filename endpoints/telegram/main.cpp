#include <getopt.h>
#include <spdlog/spdlog.h>

#include <config/provider.h>
#include <config/telegram.h>
#include <mosquitto/reciever.h>
#include <mosquitto/sender.h>
#include <utils.h>
#include "replier.h"
#include "tdclient.h"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::info);
    spdlog::info("Start dome_telegram");

    option longOptions[] = {
        {"config", required_argument, 0, 'c'}, 
        {"loglevel", required_argument, 0, 'l'}, 
        { 0 }
    };
    
    std::string configPath;

    int opt;
    while ((opt = getopt_long(argc, argv, "c:l:", longOptions, 0)) != -1) {
        switch (opt) {
            case 'c':
                configPath = std::string(optarg);
                break;
            case 'l':
                switch (optarg[0]) {
                    case 't': spdlog::set_level(spdlog::level::trace); break;
                    case 'd': spdlog::set_level(spdlog::level::debug); break;
                    case 'i': spdlog::set_level(spdlog::level::info); break;
                    case 'w': spdlog::set_level(spdlog::level::warn); break;
                    case 'e': spdlog::set_level(spdlog::level::err); break;
                    case 'c': spdlog::set_level(spdlog::level::critical); break;
                }
                break;
            case '?':
                spdlog::info("Unsupported option was passed");
        }
    }
    spdlog::debug("configPath={}", configPath);

    dome::config::Telegram telegramConfig(configPath);
    dome::config::Provider providerConfig(configPath);

    dome::mosq::Sender::Trigger trigger;
    dome::data::TdClient tdClient(telegramConfig, providerConfig, trigger);
    dome::mosq::Sender sender(providerConfig.id(), providerConfig, tdClient, trigger);

    std::vector<dome::data::Processor*> dataProcessors;
    dome::data::Replier replier(tdClient);
    dataProcessors.push_back(&replier);
    dome::mosq::Reciever reciever(GetReplyTopic(providerConfig.id()), providerConfig, dataProcessors, dome::mosq::Reciever::Type::Reply);

    sender.start();
    reciever.start();
    tdClient.run();

    // while (1) {
    //     std::this_thread::sleep_for(std::chrono::seconds(10));
    //     trigger.cv.notify_one();
    // }
}