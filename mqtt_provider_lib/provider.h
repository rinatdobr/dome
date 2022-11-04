#ifndef DATA_PROVIDER_H
#define DATA_PROVIDER_H

#include <mosquitto.h>
#include <functional>

#include <config/provider.h>

namespace dome {
namespace mqtt_provider {

class Provider;

template <class T>
class Reader
{
public:
    Reader(Provider *provider) : m_provider(provider) {}

    virtual T operator()() = 0;

protected:
    Provider *m_provider;
    T value;
};

class Provider
{
public:
    explicit Provider();
    virtual ~Provider();

    virtual bool prepareData() = 0;
    virtual Reader<double> *getReaderForFloat(const std::string &name) = 0;
};

}
}

#endif
