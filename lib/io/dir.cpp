#include "dir.h"

#include <sstream>
#include <filesystem>
#include <spdlog/spdlog.h>

namespace dome {
namespace io {

std::map<std::string, std::shared_ptr<Dir>> Dir::Create(const std::map<std::string, dome::config::Dir::Config> &dirConfig)
{
    std::map<std::string, std::shared_ptr<Dir>> result;

    for (const auto &[name, config] : dirConfig) {
        result.insert(std::make_pair(name, std::make_shared<Dir>(config.m_name, config.m_path)));
    }

    spdlog::debug("Total dirs: {}", result.size());

    return result;
}

Dir::Dir(const std::string &name, const std::string &path)
    : m_name(name)
    , m_path(path)
    , m_isValid(false)
{
    spdlog::trace("{}:{} {} name={} path={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, name, path);

    if (!checkIfExists()) {
        spdlog::info("Dir '{}' doesn't exist", path);
        if (!create()) {
            return;
        }
    }

    m_isValid = true;
}

Dir::~Dir()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!m_isValid) {
        return;
    }
}

std::string Dir::name() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    spdlog::debug("{}:{}", __PRETTY_FUNCTION__, m_name);
    return m_name;
}

void Dir::write(const command::Result &result)
{
    spdlog::trace("{}:{} {} result={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, result.isValid());

    if (!m_isValid) {
        spdlog::error("Dir is not valid to write");
        return;
    }

    if (!result.isValid()) {
        spdlog::warn("No need to write not valid result");
        return;
    }

    writeValue(result.toString());
}

std::string Dir::readLastForSec(uint seconds)
{
    spdlog::trace("{}:{} {} seconds={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, seconds);

    if (!m_isValid) {
        spdlog::error("Dir is not valid to write");
        return {};
    }

    return readLastValuesForSec(seconds);
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
        spdlog::error("Can't create '{}' dir", m_path);
        return false;
    }

    return true;
}

void Dir::writeValue(const std::string &value)
{
    spdlog::trace("{}:{} {} value={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, value);

    std::filesystem::path srcFile(value);
    std::filesystem::path destFile(m_path);
    destFile /= srcFile.filename();
    std::error_code ec;
    spdlog::debug("{}: copy from '{}' to '{}'", __PRETTY_FUNCTION__, srcFile.string(), destFile.string());

    std::filesystem::copy(srcFile, destFile, ec);
}

std::string Dir::readLastValuesForSec(uint seconds)
{
    spdlog::trace("{}:{} {} seconds={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, seconds);

    spdlog::warn("{} not implemented", __PRETTY_FUNCTION__);

    return {};
}

}
}
