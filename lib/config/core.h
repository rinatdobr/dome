#ifndef CONFIG_CORE_H
#define CONFIG_CORE_H

#include "file.h"
#include "ipcamera.h"
#include "provider.h"

#include <string>
#include <vector>

namespace dome {
namespace config {

class Database
{
public:
    std::string path;
};

class Core : public File
{
public:
    Core(const std::string &path);

    Database database() const;
    const std::vector<Provider> &providers() const;
    const std::vector<IpCamera> &ipCameras() const;

private:
    bool parse();

    Database m_database;
    std::vector<Provider> m_providers;
    std::vector<IpCamera> m_ipCameras;
};

}
}

#endif
