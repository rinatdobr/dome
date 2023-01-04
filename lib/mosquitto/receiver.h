#ifndef MOSQUITTO_RECEIVER_H
#define MOSQUITTO_RECEIVER_H

#include <map>
#include <vector>

#include "mosq.h"
#include "message/processor.h"
#include "topic.h"
#include "utils/threader.h"
#include "utils/validatable.h"

namespace dome {
namespace mosq {

class Receiver : public dome::utils::Threader, public dome::utils::Validatable
{
public:
    Receiver(const std::string &mosqClientId, std::vector<dome::mosq::Topic> topics);
    ~Receiver();

private:
    dome::mosq::Mosquitto m_mosq;
    std::vector<dome::mosq::Topic> m_topics;

    void setup();
    void backgroundWork() override;
};

}
}

#endif
