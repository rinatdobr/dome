#include <iostream>
#include <spdlog/spdlog.h>
#include <getopt.h>

#include <config/provider.h>
#include <config/openweather.h>
#include <message/get.h>
#include <mosquitto/reciever.h>
#include <topic/request.h>
#include <mosquitto/sender.h>
#include <utils/utils.h>
#include "openweather.h"

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

    dome::config::Provider config(configPath);
    if (!config.isValid()) {
        spdlog::error("Can't setup OpenWeather [1]");
        return EXIT_FAILURE;
    }

    dome::config::OpenWeather openWeatherConfig(configPath);
    if (!config.isValid()) {
        spdlog::error("Can't setup OpenWeather [2]");
        return EXIT_FAILURE;
    }

    dome::data::OpenWeather openWeather(openWeatherConfig, config);
    if (!openWeather.isValid()) {
        spdlog::error("Can't setup OpenWeather [3]");
        return EXIT_FAILURE;
    }

    dome::mosq::Sender::Trigger trigger;
    dome::mosq::Sender sender(config.id(), config, openWeather, trigger);
    if (!sender.isValid()) {
        spdlog::error("Can't setup OpenWeather [4]");
        return EXIT_FAILURE;
    }
    std::vector<dome::message::Processor*> processors;
    dome::message::Get get(trigger);
    processors.push_back(&get);
    dome::topic::Request topicRequest(config, processors);
    dome::mosq::Reciever reciever(GetRequestTopic(config.id()), topicRequest);
    if (!reciever.isValid()) {
        spdlog::error("Can't setup OpenWeather [5]");
        return EXIT_FAILURE;
    }

    reciever.start();
    sender.start();
    while (1) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    sender.stop();
    reciever.stop();

    return EXIT_SUCCESS;
}