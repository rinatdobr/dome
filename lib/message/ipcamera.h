#ifndef MESSAGE_IP_CAMERA_H
#define MESSAGE_IP_CAMERA_H

#include <string>
#include <unordered_map>
#include <deque>

#include "config/ipcamera.h"
#include "config/provider.h"
#include "processor.h"
#include "mosquitto/mosq.h"
#include "message.h"

namespace dome {
namespace message {

class IpCameraMessage : public Message
{
public:
    std::unordered_map<std::string, bool> sources;
};

class IpCamera : public Processor
{
public:
    explicit IpCamera(const std::vector<dome::config::Provider> &providers, const std::vector<dome::config::IpCamera> &ipCameras);
    ~IpCamera();

    void process(dome::mosq::Mosquitto &mosq, const dome::config::Provider &provider, nlohmann::json &jMessage) override;

private:
    const std::vector<dome::config::Provider> &m_providers;
    const std::vector<dome::config::IpCamera> &m_ipCameras;
    std::deque<std::shared_ptr<Message>> m_messages;
};

}
}

#endif
