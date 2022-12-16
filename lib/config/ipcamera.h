#ifndef CONFIG_IP_CAMERA_H
#define CONFIG_IP_CAMERA_H

#include <string>
#include <vector>

#include "utils/file.h"

namespace dome {
namespace config {

class IpCamera : public dome::utils::File
{
public:
    explicit IpCamera(const std::string &path);

    std::string id() const;
    std::string name() const;
    std::string login() const;
    std::string pass() const;
    std::string ip() const;
    uint port() const;

private:
    bool parse();

    std::string m_id;
    std::string m_name;
    std::string m_login;
    std::string m_pass;
    std::string m_ip;
    uint m_port;
};

}
}

#endif
