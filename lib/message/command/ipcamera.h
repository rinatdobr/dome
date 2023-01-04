#ifndef MESSAGE_IP_CAMERA_H
#define MESSAGE_IP_CAMERA_H

#include <string>
#include <unordered_map>
#include <deque>

#include "config/ipcamera.h"
#include "config/endpoint.h"
#include "message/processor.h"
#include "mosquitto/mosq.h"
#include "command.h"
#include "reply.h"

namespace dome {
namespace message {

class IpCameraReply : public Reply
{
public:
    std::unordered_map<std::string, bool> sources;
};

class IpCamera : public Processor, public Command
{
public:
    explicit IpCamera(const std::vector<dome::config::EndPoint> &endPointConfigs, const std::vector<dome::config::IpCamera> &ipCameras);
    ~IpCamera();

    void process(dome::mosq::Mosquitto &mosq, const dome::config::EndPoint &endPointConfig, nlohmann::json &jMessage) override;

private:
    const std::vector<dome::config::EndPoint> &m_endPointConfigs;
    const std::vector<dome::config::IpCamera> &m_ipCameras;
    std::deque<std::shared_ptr<Reply>> m_replies;
};

}
}

#endif
