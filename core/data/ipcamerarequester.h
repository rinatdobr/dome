#ifndef DATA_IP_CAMERA_REQUESTER_H
#define DATA_IP_CAMERA_REQUESTER_H

#include <string>
#include <unordered_map>
#include <deque>

#include <config/ipcamera.h>
#include <config/provider.h>
#include <data/processor.h>
#include <mosquitto/mosq.h>
#include "requester.h"

namespace dome {
namespace data {

class IpCamera : public Request
{
public:
    std::unordered_map<std::string, bool> sources;
};

class IpCameraRequester : public Processor
{
public:
    explicit IpCameraRequester(const std::vector<dome::config::Provider> &providers, const std::vector<dome::config::IpCamera> &ipCameras);
    ~IpCameraRequester();

    void process(dome::mosq::Mosquitto &mosq, const dome::config::Provider &provider, nlohmann::json &jMessage) override;

private:
    const std::vector<dome::config::Provider> &m_providers;
    const std::vector<dome::config::IpCamera> &m_ipCameras;
    std::deque<std::shared_ptr<Request>> m_requests;
};

}
}

#endif
