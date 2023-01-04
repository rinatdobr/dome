#include <iostream>
#include <spdlog/spdlog.h>
#include <getopt.h>

#include <config/endpoint.h>
#include <config/ipcamera.h>
#include <message/request/get.h>
#include <mosquitto/receiver.h>
#include <mosquitto/topic.h>
#include <mosquitto/sender.h>
#include <utils/utils.h>
#include "ipcamera.h"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::info);
    spdlog::info("Start dome_ipcamera");

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

    dome::config::EndPoint endPointConfig(configPath);
    if (!endPointConfig.isValid()) {
        spdlog::error("Can't setup IP camera [1]");
        return EXIT_FAILURE;
    }

    dome::config::IpCamera ipCameraConfig(configPath);
    if (!ipCameraConfig.isValid()) {
        spdlog::error("Can't setup IP camera [2]");
        return EXIT_FAILURE;
    }

    dome::data::IpCamera ipCamera(endPointConfig, ipCameraConfig);
    if (!ipCamera.isValid()) {
        spdlog::error("Can't setup IP camera [3]");
        return EXIT_FAILURE;
    }

    dome::mosq::Sender::Trigger trigger;
    dome::mosq::Sender sender(endPointConfig.id(), endPointConfig, ipCamera, trigger);
    if (!sender.isValid()) {
        spdlog::error("Can't setup IP camera [4]");
        return EXIT_FAILURE;
    }

    std::vector<dome::message::Processor*> processors;
    dome::message::Get get(trigger);
    processors.push_back(&get);

    dome::mosq::Receiver receiver(
        endPointConfig.id(),
        { dome::mosq::Topic(GetRequestTopic(endPointConfig.id()), endPointConfig, processors) }
    );
    if (!receiver.isValid()) {
        spdlog::error("Can't setup IP camera [5]");
        return EXIT_FAILURE;
    }

    receiver.start();
    sender.start();
    while (1) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    sender.stop();
    receiver.stop();

    return EXIT_SUCCESS;
}
