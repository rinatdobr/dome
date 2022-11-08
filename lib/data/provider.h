#ifndef DATA_PROVIDER_H
#define DATA_PROVIDER_H

#include <mosquitto.h>
#include <functional>
#include <nlohmann/json.hpp>

#include "config/provider.h"

namespace dome {
namespace data {

class Provider;

class Provider
{
public:
    explicit Provider();
    virtual ~Provider();

    virtual bool prepareData() = 0;
    virtual nlohmann::json getData() = 0;
};

}
}

#endif
