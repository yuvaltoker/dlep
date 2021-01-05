#include "Parameter.h"

using namespace LLDLEP;
using namespace LLDLEP::internal;
using namespace LLDLEP::internal::ConfigStrings;
using std::string;
using std::cout;
using std::endl;

namespace LLDLEP
{
namespace internal
{
    
Parameter::Parameter(const std::string & config_field,
                     const std::string & value) :
            config_field(config_field),
            value(value)
{
}

Parameter::~Parameter()
{
}

std::string Parameter::getConfigField() const
{
    return config_field;
}

std::string Parameter::getValue() const
{
    return value;
}


} // internal
} // LLDLEP