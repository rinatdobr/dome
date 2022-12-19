#include <iostream>
#include <spdlog/spdlog.h>
#include <getopt.h>

#include <mosquitto.h>
#include <config/core.h>
#include <mosquitto/mosq.h>
#include <mosquitto/reciever.h>
#include <message/dbsaver.h>
#include <message/filesaver.h>
#include <message/info.h>
#include <message/ipcamera.h>
#include <message/statistic.h>
#include <topic/provider.h>

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
    spdlog::debug("configPath=\"{}\"", configPath);

    dome::config::Core config(configPath);
    if (!config.isValid()) {
        spdlog::error("Can't setup core");
        return EXIT_FAILURE;
    }

    dome::message::DbSaver dbSaver(config.database().path);
    if (!dbSaver.isValid()) {
        spdlog::error("Can't setup core");
        return EXIT_FAILURE;
    }

    dome::message::FileSaver fileSaver;
    dome::message::Info info(config.providers());
    dome::message::Statistic statistic(config.database().path, config.providers());
    dome::message::IpCamera ipCamera(config.providers(), config.ipCameras());
    std::vector<dome::message::Processor*> messageProcessors;
    messageProcessors.push_back(&dbSaver);
    messageProcessors.push_back(&fileSaver);
    messageProcessors.push_back(&info);
    messageProcessors.push_back(&statistic);
    messageProcessors.push_back(&ipCamera);
    dome::topic::Provider topicProvider(config.providers(), messageProcessors);
    dome::mosq::Reciever reciever("core/reciever", topicProvider);
    if (!reciever.isValid()) {
        spdlog::error("Can't setup core");
        return EXIT_FAILURE;
    }
    reciever.start();

    while (1) {    
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    reciever.stop();

    return EXIT_SUCCESS;
}
