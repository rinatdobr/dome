#include <iostream>
#include <spdlog/spdlog.h>
#include <getopt.h>

#include <config/endpoint.h>
#include <config/ipcamera.h>
#include <endpoint.h>
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

    dome::endpoint::EndPoint endPoint(endPointConfig, ipCamera);
    endPoint.start();

    return EXIT_SUCCESS;
}
