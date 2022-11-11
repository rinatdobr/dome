#ifndef DATA_DB_SAVER_H
#define DATA_DB_SAVER_H

#include <map>
#include <chrono>

#include <data/processor.h>
#include "db/writer.h"

namespace dome {
namespace data {

class DbSaver : public Processor
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
