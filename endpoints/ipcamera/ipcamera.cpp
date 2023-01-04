#include "ipcamera.h"

#include <message/message.h>
#include <spdlog/spdlog.h>
#include <utils/utils.h>

namespace dome {
namespace data {

IpCamera::IpCamera(const dome::config::EndPoint &endPointConfig, const dome::config::IpCamera &ipCameraConfig)
    : m_endPointConfig(endPointConfig)
    , m_ipCameraConfig(ipCameraConfig)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    I_am_valid();
}

IpCamera::~IpCamera()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

bool IpCamera::prepareData()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    auto outFile = GetTmpName("ipcamera");

    std::ostringstream cmd;
    cmd << "ffmpeg" << " "
    << "-rtsp_transport"    << " " << "tcp" << " "
    << "-i"                 << " " << "rtsp://" 
                                            << m_ipCameraConfig.login() <<
                                            ":"
                                            << m_ipCameraConfig.pass() <<
                                            "@"
                                            << m_ipCameraConfig.ip() <<
                                            ":"
                                            << m_ipCameraConfig.port() <<
                                            "/ISAPI/Streaming/Channels/101" << " "
    << "-f"                 << " " << "image2" << " "
    << "-vframes"           << " " << "1" << " "
    << "-y"                 << " "
    << "-loglevel"          << " " << "error" << " "
    << outFile;

    auto execRes = Exec(cmd.str().c_str());
    if (execRes.first == 0) {
        m_photoPath = outFile;
        return true;
    }
    else {
        spdlog::error("Command \"{}\" was exited with code {}", cmd.str(), execRes.first);
        return false;
    }
}

nlohmann::json IpCamera::getData()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    nlohmann::json jData;
    jData["type"] = dome::message::type::Data;
    jData[m_endPointConfig.sources()[0].id()] = m_photoPath;

    return jData;
}

bool IpCamera::isDataLeft()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return false;
}

}
}
