#ifndef OUTDB_H
#define OUTDB_H

#include "OutLoggerEnviromentVariables.h"
#include "OutWriter.h"


namespace LLDLEP
{
namespace internal
{

class OutLogger : public OutWriter
{
public:
    OutDB();

    ~OutDB();

    bool send_out(const std::string & mesage);

private:
    
};

} // namespace internal
} // namespace LLDLEP

#endif // OUTDB_H