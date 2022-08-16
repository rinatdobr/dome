#include "command.h"

#include <spdlog/spdlog.h>

namespace command {

Command::Command(const CommandId &commandId, const std::vector<std::string> &args)
    : m_commandId(commandId)
    , m_args(args)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

Command::~Command()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

}
