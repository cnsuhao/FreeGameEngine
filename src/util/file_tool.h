#ifndef UTIL_FILE_TOOL_H
#define UTIL_FILE_TOOL_H

#include "string_tool.h"

namespace ora
{
    bool readFile(std::string & content, const std::string & filename, bool isBinary);
    bool writeFile(const std::string & content, const std::string & filename, bool isBinary);

}// end namespace ora

#endif //UTIL_FILE_TOOL_H