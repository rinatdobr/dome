#include "parser.h"

#include <iostream>

#include "cputemperature.h"
#include "statistic.h"

namespace command {

std::unique_ptr<Command> Parser::Parse(const std::string &line)
{
    std::size_t delimetr = line.find(' ');
    std::string command(line, 0, delimetr);

    std::vector<std::string> args;
    std::size_t start = delimetr + 1;
    int i = 0;
    while (start != std::string::npos) {
        std::size_t end = line.find(' ', start);
        if (end != std::string::npos) {
            args.push_back(std::string(line, start, end - start));
            // std::cout << std::string(line) << std::endl;
            // std::cout << start << std::endl;
            // std::cout << end << std::endl;
            // std::cout << std::string(line, start, end - start) << std::endl;
            std::cout << "arg[" << i++ << "] is " << args.back() << std::endl;
        }
        else {
            args.push_back(std::string(line, start));
            std::cout << std::string(line, start) << std::endl;
            std::cout << "arg[" << i++ << "] is " << args.back() << std::endl;
            break;
        }
        start = end + 1;
    }

    if (command == command::CpuTemperatureName) {
        return std::make_unique<command::CpuTemperature>(args);
    }
    else if (command == command::StatisticName) {
        return std::make_unique<command::Statistic>(args);
    }

    return nullptr;
}

}
