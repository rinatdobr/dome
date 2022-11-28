#ifndef ENDPOINTS_S8_H
#define ENDPOINTS_S8_H

#include <config/provider.h>
#include <data/provider.h>

namespace dome {
namespace data {

class S8 : public dome::data::Provider
{
public:

    explicit S8(const dome::config::Provider &config);
    ~S8();

protected:

    virtual bool prepareData() override;
    virtual nlohmann::json getData() override;
    virtual bool isDataLeft() override;

private:

    const dome::config::Provider &m_config;
    int m_co2;
};

}
}

#endif
