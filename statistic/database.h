#ifndef DATABSE_H
#define DATABSE_H

#include <string>

class Database
{
public:
    Database(const std::string &name);

private:
    const std::string m_name;
};

#endif
