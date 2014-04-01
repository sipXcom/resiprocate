#include "rutil/Version.hxx"

namespace resip
{
#define TO_STRING(NUM) #NUM
#define STRING(STR) STR
#define VERSION_STRING(MAJOR,MINOR) STRING("Resiprocate-Version ")TO_STRING(MAJOR)STRING(".")TO_STRING(MINOR)

  const char* version=VERSION_STRING(RESIP_INT_VERSION_MAJ, RESIP_INT_VERSION_MIN);

#undef VERSION_STRING
#undef STRING
#undef TO_STRING
};
