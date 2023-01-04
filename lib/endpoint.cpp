#include "endpoint.h"

#include <spdlog/spdlog.h>
#include <vector>

#include "mosquitto/topic.h"
#include "utils/utils.h"

namespace dome {
namespace endpoint {

EndPoint::EndPoint(dome::config::EndPoint &endPointConfig, dome::data::Provider &dataProvider)
    : m_endPointConfig(endPointConfig)
    , m_dataProvider(dataProvider)
    , m_sender(nullptr)
    , m_receiver(nullptr)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (endPointConfig.providerable()) {
        m_sender = new dome::mosq::Sender(endPointConfig.id(), endPointConfig, dataProvider, m_trigger);
        if (!m_sender->isValid()) {
            spdlog::error("Can't setup endpoint [1]");
            return;
        }

        if (endPointConfig.gettable()) {
            dome::message::Get *get = new dome::message::Get(m_trigger);
            m_processors.push_back(get);
        }

        if (endPointConfig.gettable()) {
            m_receiver = new dome::mosq::Receiver(
                endPointConfig.id(),
                { dome::mosq::Topic(GetRequestTopic(endPointConfig.id()), endPointConfig, m_processors) }
            );
            if (!m_receiver->isValid()) {
                spdlog::error("Can't setup endpoint [2]");
                return;
            }
        }
    }

    I_am_valid();
}

EndPoint::~EndPoint()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (m_sender) {
        m_sender->stop();
    }
    if (m_receiver) {
        m_receiver->stop();
    }

    delete m_sender;
    delete m_receiver;
}

void EndPoint::start()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (m_sender) {
        m_sender->start();
    }
    if (m_receiver) {
        m_receiver->start();
    }

    while (1) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

}
}
