#include "tdclient.h"

#include <spdlog/spdlog.h>

#include "config.h"

int main() {
    spdlog::set_level(spdlog::level::debug);

    spdlog::info("Start dome_client");

    Config config;
    TdClient client(config.telegramConfig());
    client.run();
}
