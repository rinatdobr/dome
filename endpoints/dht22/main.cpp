#include <iostream>
#include <spdlog/spdlog.h>
#include <getopt.h>

#include <config/endpoint.h>
#include <endpoint.h>
#include "dht22.h"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::info);
    spdlog::info("Start dome_dht22");

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
        spdlog::error("Can't setup DHT22 [1]");
        return EXIT_FAILURE;
    }

    dome::data::Dht22 dht22(endPointConfig);
    if (!dht22.isValid()) {
        spdlog::error("Can't setup DHT22 [2]");
        return EXIT_FAILURE;
    }

    dome::endpoint::EndPoint endPoint(endPointConfig, dht22);
    endPoint.start();

    return EXIT_SUCCESS;
}
