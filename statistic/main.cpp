#include "config.h"
#include "runner.h"

#include <iostream>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        std::cerr << "No config file to read" << std::endl;
    }

    Config config(argv[1]);
    Runner runner(config.commands());
    runner.run();

    return 0;
}
