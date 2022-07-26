#ifndef MESSAGE_DB_SAVER_H
#define MESSAGE_DB_SAVER_H

#include <map>
#include <chrono>

#include "processor.h"
#include "db/writer.h"
#include "utils/validatable.h"

namespace dome {
namespace message {

class DbSaver : public Processor, public dome::utils::Validatable
{
public:
    explicit DbSaver(const std::string &path);
    ~DbSaver();

    void process(dome::mosq::Mosquitto &, const dome::config::Provider &provider, nlohmann::json &jMessage) override;
private:
    dome::db::Writer m_dbWriter;
    std::map<std::string, std::chrono::seconds> m_timestamps;
};

}
}

#endif
