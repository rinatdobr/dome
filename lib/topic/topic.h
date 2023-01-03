#ifndef TOPIC_TOPIC_H
#define TOPIC_TOPIC_H

#include "mosquitto/mosq.h"
#include "config/provider.h"
#include "message/processor.h"

#include <vector>
#include <string>

namespace dome {
namespace topic {

class Topic
{
public:
    Topic(const std::string name, const dome::config::Provider &config, std::vector<dome::message::Processor*> &processors);
    ~Topic();

    const std::string name() const;
    const dome::config::Provider &config() const;
    std::vector<dome::message::Processor*> processors() const;

private:
    const std::string m_name;
    const dome::config::Provider &m_config;
    std::vector<dome::message::Processor*> m_processors;
};

}
}

#endif
