#include <iostream>
#include <spdlog/spdlog.h>
#include <getopt.h>

#include <mosquitto.h>
#include <config/core.h>

#include "reciever.h"
#include "db/writer.h"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::trace);
    spdlog::info("Start dome_core");

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

    dome::config::Core config(configPath);
    dome::db::Writer writer(config.database().path);
    dome::core::Reciever reciever(config, writer);
    reciever.start();
    
    std::this_thread::sleep_for(std::chrono::seconds(10));
    reciever.stop();

    return 0;
}
