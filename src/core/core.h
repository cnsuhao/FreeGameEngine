#ifndef ORANGE_CORE_H
#define ORANGE_CORE_H

#include "util/util_config.h"

namespace ora
{
	uint32 getVersion();
	
	const char * getVersionString();

    const char * getCompileString();
    
    void logEnvironment();

} //end namespace ora

#endif //ORANGE_CORE_H
