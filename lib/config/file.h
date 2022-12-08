#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include <string>

#include "validatable.h"

namespace dome {
namespace config {

class File : public Validatable
{
public:
    explicit File(const std::string &path);

protected:
    std::string read();

private:
    std::string m_path;
};

}
}

#endif
