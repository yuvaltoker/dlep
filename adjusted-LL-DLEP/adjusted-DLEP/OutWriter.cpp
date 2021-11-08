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

OutWriter::OutWriter() :
    env_vars(new LLDLEP::internal::OutLoggerEnviromentVariables()),
    is_connected(false)
{
}

OutWriter::~OutWriter()
{
    delete env_vars;
}

} // namespace internal
} // namespace LL-DLEP
