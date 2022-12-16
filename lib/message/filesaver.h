#ifndef MESSAGE_FILE_SAVER_H
#define MESSAGE_FILE_SAVER_H

#include <map>
#include <chrono>

#include "processor.h"

namespace dome {
namespace message {

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
