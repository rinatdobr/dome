#ifndef CONFIG_SOURCE_H
#define CONFIG_SOURCE_H

#include <string>

namespace dome {
namespace config {

class Source
{
public:

enum class Type {
    Undefined,
    Temperature,
    Humidity,
    Command,
    IpCamera,
    Co2
};

enum class DataType {
    Undefined,
    Float,
    Path,
    Int,
};

    Source(const std::string &id, const std::string &type, const std::string &dataType);
    ~Source();

    std::string id() const;
    Type type() const;
    DataType dataType() const;

    static std::string TypeToStr(Type type);
    static Type StrToType(const std::string &str);

private:
    std::string m_id;
    Type m_type;
    DataType m_dataType;

};

}
}

#endif
