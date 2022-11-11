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
    Request
};

enum class DataType {
    Undefined,
    Float,
    String
};

    static std::string TypeToStr(Type type);

    std::string id;
    std::string location;
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
    uint periodSec() const;
    uint savePeriodSec() const;
    const std::vector<Source> &sources() const;
    const std::vector<Request> &requests() const;

private:
    void parse();

    std::string m_id;
    uint m_periodSec;
    uint m_savePeriodSec;
    std::vector<Source> m_sources;
    std::vector<Request> m_requests;
};

}
}

#endif
