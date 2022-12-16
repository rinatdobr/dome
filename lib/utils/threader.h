#ifndef UTILS_THREADER_H
#define UTILS_THREADER_H

#include <thread>

namespace dome {
namespace utils {

class Threader
{
public:
    Threader();
    ~Threader();

    virtual void start();
    virtual void stop();

protected:
    bool m_isWorking;

    virtual void backgroundWork() = 0;

private:
    std::thread m_worker;

};

}
}

#endif
