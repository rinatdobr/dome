#include "tdclient.h"

#include <spdlog/spdlog.h>

int main() {
    spdlog::set_level(spdlog::level::debug);

    spdlog::info("Start dome_client");

    TdClient client(1, 5, LOGIN, PASS);
    client.run();
}
