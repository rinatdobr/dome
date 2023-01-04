#include <iostream>
#include <spdlog/spdlog.h>
#include <getopt.h>

#include <mosquitto.h>
#include <config/core.h>
#include <config/database.h>
#include <mosquitto/mosq.h>
#include <mosquitto/receiver.h>
#include <message/command/info.h>
#include <message/command/ipcamera.h>
#include <message/command/statistic.h>
#include <message/saver/dbsaver.h>
#include <message/saver/filesaver.h>
#include <mosquitto/topic.h>

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

    dome::config::Core coreConfig(configPath);
    if (!coreConfig.isValid()) {
        spdlog::error("Can't setup core [1]");
        return EXIT_FAILURE;
    }

    dome::config::Database dbConfig(configPath);
    if (!dbConfig.isValid()) {
        spdlog::error("Can't setup core [2]");
        return EXIT_FAILURE;
    }

    dome::message::DbSaver dbSaver(dbConfig);
    if (!dbSaver.isValid()) {
        spdlog::error("Can't setup core [3]");
        return EXIT_FAILURE;
    }

    dome::message::FileSaver fileSaver;
    dome::message::Info info(coreConfig.endPoints());
    dome::message::Statistic statistic(dbConfig.path(), coreConfig.endPoints());
    dome::message::IpCamera ipCamera(coreConfig.endPoints(), coreConfig.ipCameras());

    std::vector<dome::message::Processor*> messageProcessors;
    messageProcessors.push_back(&dbSaver);
    messageProcessors.push_back(&fileSaver);
    messageProcessors.push_back(&info);
    messageProcessors.push_back(&statistic);
    messageProcessors.push_back(&ipCamera);

    std::vector<dome::mosq::Topic> topics;
    for (const auto &endPoint : coreConfig.endPoints()) {
        topics.push_back(dome::mosq::Topic(endPoint.id(), endPoint, messageProcessors));
    }

    dome::mosq::Receiver receiver("core", topics);
    if (!receiver.isValid()) {
        spdlog::error("Can't setup core [4]");
        return EXIT_FAILURE;
    }
    receiver.start();

    while (1) {    
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    receiver.stop();

    return EXIT_SUCCESS;
}
