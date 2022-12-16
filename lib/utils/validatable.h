#ifndef UTILS_VALIDATABLE_H
#define UTILS_VALIDATABLE_H

namespace dome {
namespace utils {

class Validatable
{
public:
    Validatable();

    bool isValid();
    void I_am_valid();
    void I_am_not_valid();

protected:
    bool m_isValid;
};

}
}

#endif
