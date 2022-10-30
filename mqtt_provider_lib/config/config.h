#ifndef CONFIG_H
#define CONFIG_H

#include "file.h"

#include <string>
#include <vector>

namespace dome {
namespace config {

class Source
{
public:
enum class Type {
    Undefined,
    Temperature,
    Humidity
};

enum class DataType {
    Undefined,
    Float
};

    static std::string TypeToStr(Type type);

    std::string name;
    std::string description;
    std::string location;
    Type type;
    DataType dataType;
};

class Config : public File
{
public:
    Config(std::string path);

    std::string id() const;
    uint periodSec() const;
    const std::vector<Source> &sources() const;

private:
    void parse();

    std::string m_id;
    uint m_periodSec;
    std::vector<Source> m_sources;
};

}
}

#endif
