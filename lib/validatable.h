#ifndef VALIDATABLE_H
#define VALIDATABLE_H

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

#endif
