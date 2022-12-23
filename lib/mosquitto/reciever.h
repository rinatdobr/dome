#ifndef MOSQUITTO_RECIEVER_H
#define MOSQUITTO_RECIEVER_H

#include <vector>

#include "mosq.h"
#include "topic/processor.h"
#include "utils/threader.h"
#include "utils/validatable.h"

namespace dome {
namespace mosq {

class Reciever : public dome::utils::Threader, public dome::utils::Validatable
{
public:
    Reciever(const std::string &mosqClientId, dome::topic::Processor &topicProcessor);
    ~Reciever();

private:
    dome::topic::Processor &m_topicProcessor;
    dome::mosq::Mosquitto m_mosq;

    void setup();
    void backgroundWork() override;
};

}
}

#endif
