#ifndef DATA_PROVIDER_H
#define DATA_PROVIDER_H

#include <mosquitto.h>
#include <functional>
#include <nlohmann/json.hpp>

#include "config/endpoint.h"
#include "utils/validatable.h"

namespace dome {
namespace data {

class Provider : public dome::utils::Validatable
{
public:
    virtual bool prepareData() = 0;
    virtual nlohmann::json getData() = 0;
    virtual bool isDataLeft() = 0;
};

}
}

#endif
