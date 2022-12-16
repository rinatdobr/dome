#include <getopt.h>
#include <spdlog/spdlog.h>

#include <config/provider.h>
#include <config/telegram.h>
#include <mosquitto/reciever.h>
#include <topic/reply.h>
#include <mosquitto/sender.h>
#include <utils/utils.h>
#include "reply.h"
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
    if (!telegramConfig.isValid()) {
        spdlog::error("Can't setup Telegram");
        return EXIT_FAILURE;
    }

    dome::config::Provider providerConfig(configPath);
    if (!providerConfig.isValid()) {
        spdlog::error("Can't setup Telegram");
        return EXIT_FAILURE;
    }

    dome::mosq::Sender::Trigger trigger;
    dome::data::TdClient tdClient(telegramConfig, providerConfig, trigger);
    dome::mosq::Sender sender(providerConfig.id(), providerConfig, tdClient, trigger);
    if (!sender.isValid()) {
        spdlog::error("Can't setup Telegram");
        return EXIT_FAILURE;
    }

    std::vector<dome::message::Processor*> messageProcessors;
    dome::message::Reply reply(tdClient);
    messageProcessors.push_back(&reply);
    dome::topic::Reply topicReply(providerConfig, messageProcessors);
    dome::mosq::Reciever reciever(GetReplyTopic(providerConfig.id()), topicReply);
    if (!reciever.isValid()) {
        spdlog::error("Can't setup Telegram");
        return EXIT_FAILURE;
    }

    sender.start();
    reciever.start();
    tdClient.run();

    // while (1) {
    //     std::this_thread::sleep_for(std::chrono::seconds(10));
    //     trigger.cv.notify_one();
    // }
}
