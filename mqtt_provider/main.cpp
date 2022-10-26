#include <iostream>
#include <spdlog/spdlog.h>
#include <getopt.h>

#include "config/config.h"
#include "sender.h"
#include "providers/dht22.h"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::trace);
    spdlog::info("Start mqtt_provider");

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
    dome::mqtt::Sender sender(config, dht22);
    sender.start(1);

    // mosquitto_lib_init();

    // struct mosquitto *mosq = mosquitto_new("test_cient", false, nullptr);

    // int res = mosquitto_connect(mosq, "localhost", 1883, 60);

    // res = mosquitto_publish(mosq, nullptr, "test", 7, "hello1", 0, false);

    // mosquitto_destroy(mosq);

    return 0;
}