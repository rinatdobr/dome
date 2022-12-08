#include "validatable.h"

Validatable::Validatable()
    : m_isValid(false)
{

}

bool Validatable::isValid()
{
    return m_isValid;
}

void Validatable::I_am_valid()
{
    m_isValid = true;
}

void Validatable::I_am_not_valid()
{
    m_isValid = false;
}