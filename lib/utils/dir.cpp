#include "dir.h"

#include <filesystem>
#include <spdlog/spdlog.h>

namespace dome {
namespace utils {

Dir::Dir(const std::string &path)
    : m_path(path)
    , m_isValid(false)
{
    spdlog::trace("{}:{} {} path={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, path);

    if (!checkIfExists()) {
        spdlog::info("directory '{}' doesn't exist", path);
        if (!create()) {
            return;
        }
    }

    m_isValid = true;
}

Dir::~Dir()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void Dir::copyFile(const std::string &filePath)
{
    spdlog::trace("{}:{} {} filePath={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, filePath);

    if (!m_isValid) {
        spdlog::error("Invalid directory to write");
        return;
    }

    std::filesystem::path srcFile(filePath);
    std::filesystem::path destFile(m_path);
    destFile /= srcFile.filename();
    std::error_code ec;
    spdlog::debug("copying '{}' to '{}'", srcFile.string(), destFile.string());

    std::filesystem::copy(srcFile, destFile, ec);
}

bool Dir::checkIfExists()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return std::filesystem::exists(m_path);
}

bool Dir::create()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!std::filesystem::create_directory(m_path)) {
        spdlog::error("can't create '{}' directory", m_path);
        return false;
    }

    return true;
}

}
}
