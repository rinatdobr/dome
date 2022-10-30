#include <iostream>
#include <spdlog/spdlog.h>
#include <getopt.h>

#include <config/config.h>
#include <core/sender.h>
#include "dht22.h"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::trace);
    spdlog::info("Start dht22_provider");

    option longOptions[] = {
        {"config", required_argument, 0, 'c'}, 
        { 0 }
    };
    
    std::string configPath;

    int opt;
    while ((opt = getopt_long(argc, argv, "c:", longOptions, 0)) != -1) {
        switch (opt) {
            case 'c':
                configPath = std::string(optarg);
                break;
            case '?':
                spdlog::info("Unsupported option was passed");
        }
    }
    spdlog::debug("configPath={}", configPath);

    dome::config::Config config(configPath);
    dome::data::Dht22 dht22;
    dome::core::Sender sender(config, dht22);
    sender.start(1);

    return 0;
}
