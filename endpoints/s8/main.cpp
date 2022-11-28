#include <iostream>
#include <spdlog/spdlog.h>
#include <getopt.h>

#include <config/provider.h>
#include <data/getter.h>
#include <mosquitto/reciever.h>
#include <mosquitto/sender.h>
#include <utils.h>
#include "s8.h"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::info);
    spdlog::info("Start dome_s8");

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

    dome::config::Provider config(configPath);
    dome::data::S8 s8(config);
    dome::mosq::Sender::Trigger trigger;
    dome::mosq::Sender sender(config.id(), config, s8, trigger);
    std::vector<dome::data::Processor*> processors;
    dome::data::Getter getter(trigger);
    processors.push_back(&getter);
    dome::mosq::Reciever reciever(GetRequestTopic(config.id()), config, processors, dome::mosq::Reciever::Type::Request);

    reciever.start();
    sender.start();
    while (1) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    sender.stop();
    reciever.stop();

    return 0;
}
