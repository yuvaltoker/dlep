#include "OutLoggerEnviromentVariables.h"

using namespace LLDLEP;
using namespace LLDLEP::internal;
using std::string;

namespace LLDLEP
{
namespace internal
{

OutLoggerEnviromentVariables::OutLoggerEnviromentVariables()
{
    set_vars();
}

std::string OutLoggerEnviromentVariables::get_host()
{
    return host;
}

std::string OutLoggerEnviromentVariables::get_port()
{
    return port;
}

void OutLoggerEnviromentVariables::set_vars()
{
    char* var;
    host = "";
    port = "";

    var = getenv("UI_HOST");
    if(var != NULL)
    {
        host = var;
    }

    var = getenv("UI_PORT");
    if(var != NULL)
    {
        port = var;
    }
}

} // namespace internal
} // namespace LLDLEP