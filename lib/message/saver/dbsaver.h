#ifndef MESSAGE_DB_SAVER_H
#define MESSAGE_DB_SAVER_H

#include <map>
#include <chrono>

#include "config/database.h"
#include "message/processor.h"
#include "db/writer.h"
#include "utils/validatable.h"

namespace dome {
namespace message {

class DbSaver : public Processor, public dome::utils::Validatable
{
public:
    explicit DbSaver(const dome::config::Database &config);
    ~DbSaver();

    void process(dome::mosq::Mosquitto &, const dome::config::EndPoint &endPointConfig, nlohmann::json &jMessage) override;
private:
    const dome::config::Database &m_config;
    dome::db::Writer m_dbWriter;
    std::map<std::string, std::chrono::seconds> m_timestamps;
};

}
}

#endif
