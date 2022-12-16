#ifndef DATA_PROVIDER_H
#define DATA_PROVIDER_H

#include <mosquitto.h>
#include <functional>
#include <nlohmann/json.hpp>

#include "config/provider.h"
#include "utils/validatable.h"

namespace dome {
namespace data {

class Provider;

class Provider : public dome::utils::Validatable
{
public:
    explicit Provider();
    virtual ~Provider();

    virtual bool prepareData() = 0;
    virtual nlohmann::json getData() = 0;
    virtual bool isDataLeft() = 0;
};

}
}

#endif
