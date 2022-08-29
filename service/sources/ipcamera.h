#ifndef IP_CAMERA_H
#define IP_CAMERA_H

#include <string>
#include <memory>
#include <map>
#include <config/ipcamera.h>

class IpCamera
{
public:
    IpCamera(const std::string &name,
             const std::string &login,
             const std::string &pass,
             const std::string &ip,
             const uint port
    );

    static std::map<std::string, std::unique_ptr<IpCamera>> Create(const std::vector<dome::config::IpCamera::Config> &ipCameraConfig);
    std::string photo();

private:
    std::string m_name;
    std::string m_login;
    std::string m_pass;
    std::string m_ip;
    uint m_port;
};

#endif
