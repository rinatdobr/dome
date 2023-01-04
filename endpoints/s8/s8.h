#ifndef ENDPOINTS_S8_H
#define ENDPOINTS_S8_H

#include <config/endpoint.h>
#include <data/provider.h>

namespace dome {
namespace data {

class S8 : public dome::data::Provider
{
public:

    explicit S8(const dome::config::EndPoint &endPointConfig);
    ~S8();

protected:

    virtual bool prepareData() override;
    virtual nlohmann::json getData() override;
    virtual bool isDataLeft() override;

private:

    const dome::config::EndPoint &m_endPointConfig;
    int m_co2;
};

}
}

#endif
