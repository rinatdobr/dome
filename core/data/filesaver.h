#ifndef DATA_FILE_SAVER_H
#define DATA_FILE_SAVER_H

#include <map>
#include <chrono>

#include <data/processor.h>

namespace dome {
namespace data {

class FileSaver : public Processor
{
public:
    explicit FileSaver();
    ~FileSaver();

    void process(dome::mosq::Mosquitto &, const dome::config::Provider &provider, nlohmann::json &jMessage) override;
private:
    std::map<std::string, std::chrono::seconds> m_timestamps;
};

}
}

#endif
