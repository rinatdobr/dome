#include <iostream>
#include <spdlog/spdlog.h>
#include <getopt.h>

#include <mosquitto.h>
#include <config/core.h>
#include <mosquitto/mosq.h>
#include <mosquitto/reciever.h>

#include "data/dbsaver.h"
#include "data/requester.h"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::info);
    spdlog::info("Start dome_core");

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

    dome::config::Core config(configPath);
    dome::data::DbSaver dbSaver(config.database().path);
    dome::data::Requester requester(config.providers());
    std::vector<dome::data::Processor*> dataProcessors;
    dataProcessors.push_back(&dbSaver);
    dataProcessors.push_back(&requester);
    dome::mosq::Reciever reciever("core/reciever", config.providers(), dataProcessors);
    reciever.start();
    
    std::this_thread::sleep_for(std::chrono::seconds(60 * 60));
    reciever.stop();

    return 0;
}
