#ifndef CONFIG_ENDPOINT_H
#define CONFIG_ENDPOINT_H

#include "source.h"
#include "utils/file.h"

#include <string>
#include <vector>

namespace dome {
namespace config {

class EndPoint : public dome::utils::File
{
public:
    EndPoint();
    EndPoint(std::string path);

    std::string id() const;
    std::string location() const;
    bool requestable() const;
    bool gettable() const;
    bool savable() const;
    bool providerable() const;
    bool replyable() const;
    uint periodSec() const;
    uint savePeriodSec() const;
    uint maxFrequenceSec() const;
    const std::vector<Source> &sources() const;

private:
    bool parse();

    std::string m_id;
    std::string m_location;
    bool m_isRequestable;
    bool m_isGettable;
    bool m_isSavable;
    bool m_isProviderable;
    bool m_isReplyable;
    uint m_periodSec;
    uint m_savePeriodSec;
    uint m_maxFrequenceSec;
    std::vector<Source> m_sources;
};

}
}

#endif
