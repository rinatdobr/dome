#include <iostream>

#include "runner.h"
#include "config.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "No config file to read" << std::endl;
    }

    Config config(argv[1], argv[2]);
    Runner runner(std::move(config.commands()));
    runner.run();

    return 0;
}
