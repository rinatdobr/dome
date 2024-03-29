#include "utils/file.h"

#include <sstream>
#include <fstream>
#include <filesystem>
#include <spdlog/spdlog.h>

namespace dome {
namespace utils {

File::File(const std::string &path)
    : m_path(path)
{
    spdlog::trace("{}:{} {} path=\"{}\"", __FILE__, __LINE__, __PRETTY_FUNCTION__,
                            path);

    if (!std::filesystem::exists(m_path)) {
        spdlog::error("File does not exists: \"{}\"", m_path);
        return;
    }

    I_am_valid();
}

std::string File::read()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!isValid()) {
        spdlog::error("Invalid file: \"{}\"", m_path);
        return {};
    }

    std::ifstream file(m_path);
    if (!file.is_open()) {
        spdlog::error("Can't open file: \"{}\"", m_path);
        return {};
    }

    std::ostringstream fileStream;
    fileStream << file.rdbuf();

    return fileStream.str();
}

}
}
