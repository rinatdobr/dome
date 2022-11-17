#ifndef UTILS_DIR_H
#define UTILS_DIR_H

#include <string>

namespace dome {
namespace utils {

class Dir
{
public:
    Dir(const std::string &path);
    ~Dir();

    void copyFile(const std::string &filePath);

private:
    std::string m_path;
    bool m_isValid;

    bool checkIfExists();
    bool create();
};

}
}

#endif
