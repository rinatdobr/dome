#ifndef MOSQUITTO_TOPIC_H
#define MOSQUITTO_TOPIC_H

#include "config/endpoint.h"
#include "message/processor.h"

#include <vector>
#include <string>

namespace dome {
namespace mosq {

class Topic
{
public:
    Topic(const std::string name, const dome::config::EndPoint &endPointConfig, std::vector<dome::message::Processor*> &processors);
    ~Topic();

    const std::string name() const;
    const dome::config::EndPoint &endPointConfig() const;
    std::vector<dome::message::Processor*> processors() const;

    static std::vector<std::string> GetTopicNames(const std::vector<Topic> &topics);

private:
    const std::string m_name;
    const dome::config::EndPoint &m_endPointConfig;
    std::vector<dome::message::Processor*> m_processors;
};

}
}

#endif
