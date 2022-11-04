#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include <string>

namespace dome {
namespace config {

class File
{
public:
    explicit File(const std::string &path);

protected:
    std::string read();
    virtual void parse() = 0;

private:
    std::string m_path;
};

}
}

#endif
