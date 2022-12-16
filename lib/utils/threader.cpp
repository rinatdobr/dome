#include "utils/threader.h"

#include <spdlog/spdlog.h>

namespace dome {
namespace utils {

Threader::Threader()
    : m_isWorking(false)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

Threader::~Threader()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void Threader::start()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    m_isWorking = true;

    m_worker = std::thread(&Threader::backgroundWork, this);
}
void Threader::stop()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    m_isWorking = false;

    m_worker.join();
}

}
}
