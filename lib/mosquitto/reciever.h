#ifndef MOSQUITTO_RECIEVER_H
#define MOSQUITTO_RECIEVER_H

#include <map>
#include <vector>

#include "mosq.h"
#include "message/processor.h"
#include "topic/topic.h"
#include "utils/threader.h"
#include "utils/validatable.h"

namespace dome {
namespace mosq {

class Reciever : public dome::utils::Threader, public dome::utils::Validatable
{
public:
    Reciever(const std::string &mosqClientId, std::vector<std::string> topicNames, std::vector<dome::topic::Topic> topics);
    ~Reciever();

private:
    dome::mosq::Mosquitto m_mosq;
    std::vector<std::string> m_topicNames;
    std::vector<dome::topic::Topic> m_topics;

    void setup();
    void backgroundWork() override;
};

}
}

#endif
