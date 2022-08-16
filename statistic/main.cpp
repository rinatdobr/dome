#include <spdlog/spdlog.h>

#include "runner.h"
#include "config.h"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::debug);

    spdlog::info("Start dome_statistic");

    Config config;
    Runner runner(std::move(config.commands()));
    runner.run();

    return EXIT_SUCCESS;
}
