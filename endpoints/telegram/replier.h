#ifndef REPLIER_H
#define REPLIER_H

#include <data/processor.h>
#include "tdclient.h"

namespace dome {
namespace data {

class Replier : public Processor
{
public:
    explicit Replier(dome::data::TdClient &tdClient);
    ~Replier();

    void process(const dome::config::Provider &provider, nlohmann::json &jMessage) override;

private:
    dome::data::TdClient &m_tdClient;
};

}
}

#endif
