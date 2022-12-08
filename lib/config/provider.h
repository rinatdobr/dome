#ifndef CONFIG_PROVIDER_H
#define CONFIG_PROVIDER_H

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
    Humidity,
    Request,
    IpCamera,
    Co2
};

enum class DataType {
    Undefined,
    Float,
    Path,
    Int
};

    static std::string TypeToStr(Type type);
    static Type StrToType(const std::string &str);

    std::string id;
    Type type;
    DataType dataType;
};

class Request
{
public:
    std::string name;
};

class Provider : public File
{
public:
    Provider();
    Provider(std::string path);

    std::string id() const;
    std::string location() const;
    uint periodSec() const;
    uint savePeriodSec() const;
    uint maxFrequenceSec() const;
    const std::vector<Source> &sources() const;
    const std::vector<Request> &requests() const;

private:
    bool parse();

    std::string m_id;
    uint m_periodSec;
    uint m_savePeriodSec;
    uint m_maxFrequenceSec;
    std::string m_location;
    std::vector<Source> m_sources;
    std::vector<Request> m_requests;
};

}
}

#endif
