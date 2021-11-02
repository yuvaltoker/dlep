#include "OutLoggerEnviromentVariables.h"
#include "OutWriter.h"


using namespace LLDLEP;
using namespace LLDLEP::internal;
using std::string;
using std::cout;
using std::endl;

namespace LLDLEP
{
namespace internal
{

OutLogger::OutLogger() :
    env_vars(new LLDLEP::internal::OutLoggerEnviromentVariables()),
    is_connected(false)
{
}

OutLogger::~OutLogger()
{
    delete env_vars;
}

} // namespace internal
} // namespace LL-DLEP
