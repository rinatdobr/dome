#include "utils.h"

#include <sstream>
#include <spdlog/spdlog.h>
#include <stdio.h>
#include <array>
#include <map>

uint PeriodToSeconds(const std::string &period)
{
    spdlog::trace("{}:{} {} period={}", __FILE__, __LINE__, __PRETTY_FUNCTION__,
                            period);

    std::istringstream parser(period);
    unsigned int result;
    parser >> result;

    if (period.rfind("s") != std::string::npos) {
        // nothing to do
    }
    else if (period.rfind("m") != std::string::npos) {
        result *= 60;
    }
    else if (period.rfind("h") != std::string::npos) {
        result *= 3600;
    }
    else if (period.rfind("d") != std::string::npos) {
        result *= 86400;
    }

    return result;
}

std::string GetRequestTopic(const std::string &providerId)
{
    spdlog::trace("{}:{} {} providerId={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, providerId);

    return std::string(providerId + "/request");
}

std::string GetReplyTopic(const std::string &providerId)
{
    spdlog::trace("{}:{} {} providerId={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, providerId);

    return std::string(providerId + "/reply");
}

std::pair<int, std::string> Exec(const std::string cmdLine)
{
    spdlog::trace("{}:{} {} cmdLine=\"{}\"", __FILE__, __LINE__, __PRETTY_FUNCTION__, cmdLine);

    auto f = popen(cmdLine.c_str(), "r");
    if (!f) {
        return std::make_pair(-1, "");
    }

    std::array<char, 1024> buffer;
    std::string output;
    while (fgets(buffer.data(), buffer.size(), f) != nullptr) {
        output += buffer.data();
    }

    int rc = pclose(f);

    return std::make_pair(rc, output);
}

std::string GetTmpName(const std::string id)
{
    spdlog::trace("{}:{} {} id={}", __FILE__, __LINE__, __PRETTY_FUNCTION__,
                            id);

    std::string tmpDir("/tmp/dome." + id + ".XXXXXX");
    char *tmpDirRaw = mkdtemp(&tmpDir[0]);
    chmod(tmpDirRaw, S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);
    spdlog::debug("tmpDir=\"{}\"", tmpDir);

    auto const time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::array<char, 50> buffer;
    std::strftime(buffer.data(), buffer.size(), "%Y%m%d_%H%M%S", std::localtime(&time));

    std::string tmpName = tmpDir + "/" + buffer.data() + ".png";
    spdlog::debug("tmpName=\"{}\"", tmpName);

    return tmpName;
}

std::string PingMessage()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    nlohmann::json jMessage;
    jMessage["type"] = "ping";
    return jMessage.dump();
}

bool CheckJsonMessageForKeys(const nlohmann::json &jMessage, const std::vector<std::string> &keys)
{
    spdlog::trace("{}:{} {}");

    for (const auto &key : keys) {
        if (!jMessage.contains(key)) {
            spdlog::warn("JSON message [{}] doesn't contain \"{}\" key", jMessage.dump(), key);
            return false;
        }
    }

    spdlog::trace("JSON message [{}] contains all keys", jMessage.dump());
    return true;
}

std::string ErrorMessage(ErrorNum errorNum)
{
    const static std::map<ErrorNum, std::string> Descriptions {
        {
            ErrorNum::JsonKeyNoType,
            "В ответном сообщении отсутствует поле 'type'"
        }
    };

    return Descriptions.at(errorNum);
}
