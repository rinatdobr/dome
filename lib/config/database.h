#ifndef CONFIG_DATABASE_H
#define CONFIG_DATABASE_H

#include "utils/file.h"
#include <string>

namespace dome {
namespace config {

class Database : public dome::utils::File
{
public:
    explicit Database(const std::string &path);

    std::string path() const;

private:
    bool parse();

    std::string m_path;
};

}
}

#endif
