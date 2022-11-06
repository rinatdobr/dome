#ifndef MOSQUITTO_RECIEVER_H
#define MOSQUITTO_RECIEVER_H

#include <vector>

#include "config/provider.h"
#include "data/processor.h"
#include "mosquitto/mosq.h"
#include "threader.h"

namespace dome {
namespace mosq {

enum class RecieverType
{
    Provider,
    Command
};

class Reciever : public dome::utils::Threader
{
public:
    Reciever(const std::string &mosqClientId, const std::vector<dome::config::Provider> &providers, std::vector<dome::data::Processor*> &dataProcessors);
    Reciever(const std::string &mosqClientId, const dome::config::Provider &provider, std::vector<dome::data::Processor*> &dataProcessors);
    ~Reciever();

private:
    const std::vector<dome::config::Provider> &m_providers;
    const dome::config::Provider &m_provider;
    dome::mosq::Mosquitto m_mosq;
    std::vector<dome::data::Processor*> m_dataProcessors;
    RecieverType m_recieverType;

    void setup();
    void subscribe();
    void unsubscribe();
    void backgroundWork() override;
};

}
}

#endif
