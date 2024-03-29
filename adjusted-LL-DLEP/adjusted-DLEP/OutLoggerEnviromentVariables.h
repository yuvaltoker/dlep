#ifndef OUTLOGGERENVIROMENTVARIABLES_H
#define OUTLOGGERENVIROMENTVARIABLES_H

#include <iostream>
#include <cstdlib>

namespace LLDLEP
{
namespace internal
{

class OutLoggerEnviromentVariables
{
public:
    OutLoggerEnviromentVariables();

    ~OutLoggerEnviromentVariables();

    std::string get_host() const;
    std::string get_port() const;


private:
    void set_vars();
    
    std::string host;
    std::string port;   
};

} // namespace internal
} // namespace LLDLEP

#endif // OUTLOGGERENVIROMENTVARIABLES_H