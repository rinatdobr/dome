#ifndef MOSQUITTO_RECIEVER_H
#define MOSQUITTO_RECIEVER_H

#include <vector>

#include "config/provider.h"
#include "data/processor.h"
#include "mosquitto/mosq.h"
#include "threader.h"

namespace dome {
namespace mosq {

class Reciever : public dome::utils::Threader
{
public:
enum class Type
{
    Provider,
    Request,
    Reply
};

    Reciever(const std::string &mosqClientId, const std::vector<dome::config::Provider> &providers, std::vector<dome::data::Processor*> &dataProcessors);
    Reciever(const std::string &mosqClientId, const dome::config::Provider &provider, std::vector<dome::data::Processor*> &dataProcessors, Type type);
    ~Reciever();

private:
    const std::vector<dome::config::Provider> &m_providers;
    const dome::config::Provider &m_provider;
    dome::mosq::Mosquitto m_mosq;
    std::vector<dome::data::Processor*> m_dataProcessors;
    Type m_type;

    void setup();
    void subscribe();
    void unsubscribe();
    void backgroundWork() override;
};

}
}

#endif
