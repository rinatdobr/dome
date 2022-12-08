#include <iostream>
#include <spdlog/spdlog.h>
#include <getopt.h>

#include <mosquitto.h>
#include <config/core.h>
#include <mosquitto/mosq.h>
#include <mosquitto/reciever.h>

#include "data/dbsaver.h"
#include "data/filesaver.h"
#include "data/inforequester.h"
#include "data/ipcamerarequester.h"
#include "data/statisticrequester.h"

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
    if (!config.isValid()) {
        spdlog::error("Can't setup core");
        return EXIT_FAILURE;
    }

    dome::data::DbSaver dbSaver(config.database().path);
    dome::data::FileSaver fileSaver;
    dome::data::InfoRequester infoRequester(config.providers());
    dome::data::StatisticRequester statisticRequester(config.database().path, config.providers());
    dome::data::IpCameraRequester ipCameraRequester(config.providers(), config.ipCameras());
    std::vector<dome::data::Processor*> dataProcessors;
    dataProcessors.push_back(&dbSaver);
    dataProcessors.push_back(&fileSaver);
    dataProcessors.push_back(&infoRequester);
    dataProcessors.push_back(&statisticRequester);
    dataProcessors.push_back(&ipCameraRequester);
    dome::mosq::Reciever reciever("core/reciever", config.providers(), dataProcessors);
    reciever.start();

    while (1) {    
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    reciever.stop();

    return EXIT_SUCCESS;
}
