#ifndef CONFIG_IP_CAMERA_H
#define CONFIG_IP_CAMERA_H

#include <string>
#include <vector>

#include "config.h"

namespace dome {
namespace config {

class IpCamera : public Config
{
public:
struct Raw
{
    Raw(const std::string &name,
        const std::string &login,
        const std::string &pass,
        const std::string &ip,
        const std::string &port);

    std::string m_name;
    std::string m_login;
    std::string m_pass;
    std::string m_ip;
    std::string m_port;
};

    explicit IpCamera(const std::string &configPath);

    const std::vector<Raw> ipCameraConfig() const;

private:
    void parseConfig() override;

    std::vector<Raw> m_ipCameras;
};

}
}

#endif
