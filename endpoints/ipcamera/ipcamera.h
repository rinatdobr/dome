#ifndef ENDPOINTS_IP_CAMERA_H
#define ENDPOINTS_IP_CAMERA_H

#include <config/provider.h>
#include <config/ipcamera.h>
#include <data/provider.h>

namespace dome {
namespace data {

class IpCamera : public dome::data::Provider
{
public:

    explicit IpCamera(const dome::config::Provider &providerConfig, const dome::config::IpCamera &ipCameraConfig);
    ~IpCamera();

protected:

    virtual bool prepareData() override;
    virtual nlohmann::json getData() override;
    virtual bool isDataLeft() override;

private:
    std::string m_photoPath;
    const dome::config::Provider &m_providerConfig;
    const dome::config::IpCamera &m_ipCameraConfig;
};

}
}

#endif
