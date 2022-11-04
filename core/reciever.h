#ifndef SENDER_H
#define SENDER_H

#include <mosquitto.h>

#include <thread>
#include <config/core.h>

#include "db/writer.h"

namespace dome {
namespace core {

class Reciever
{
public:
    explicit Reciever(const dome::config::Core &config, dome::db::Writer &dbWriter);
    ~Reciever();

    void start();
    void stop();
private:
    const dome::config::Core &m_config;
    struct mosquitto *m_mosq;
    std::thread m_worker;
    dome::db::Writer &m_dbWriter;
    bool m_isWorking;

    void backgroundWork();
};

}
}

#endif
