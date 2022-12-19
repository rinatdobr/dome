#ifndef UTILS_DIR_H
#define UTILS_DIR_H

#include <string>

#include "validatable.h"

namespace dome {
namespace utils {

class Dir : public Validatable
{
public:
    Dir(const std::string &path);
    ~Dir();

    void copyFile(const std::string &filePath);

private:
    std::string m_path;

    bool checkIfExists();
    bool create();
};

}
}

#endif
