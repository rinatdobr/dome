#ifndef IO_DIR_H
#define IO_DIR_H

#include <map>
#include <memory>

#include <config/dir.h>
#include <commands/result.h>

#include "io.h"

namespace dome {
namespace io {

class Dir : public Io
{
public:
    static std::map<std::string, std::shared_ptr<Dir>> Create(const std::map<std::string, dome::config::Dir::Config> &dirConfig);

    Dir(const std::string &name, const std::string &path);
    ~Dir();

    std::string name() const;
    void write(const command::Result &result) override;
    std::string readLastForSec(uint seconds) override;

private:
    std::string m_name;
    std::string m_path;
    bool m_isValid;

    bool checkIfExists();
    bool create();
    void writeValue(const std::string &value);
    std::string readLastValuesForSec(uint seconds);
};

}
}

#endif
