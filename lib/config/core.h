#ifndef CONFIG_CORE_H
#define CONFIG_CORE_H

#include "utils/file.h"
#include "ipcamera.h"
#include "endpoint.h"

#include <string>
#include <vector>

namespace dome {
namespace config {

class Core : public dome::utils::File
{
public:
    Core(const std::string &path);

    const std::vector<EndPoint> &endPoints() const;
    const std::vector<IpCamera> &ipCameras() const;

private:
    bool parse();

    std::vector<EndPoint> m_endPoints;
    std::vector<IpCamera> m_ipCameras;
};

}
}

#endif
