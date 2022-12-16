#ifndef UTILS_FILE_H
#define UTILS_FILE_H

#include <string>

#include "validatable.h"

namespace dome {
namespace utils {

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
