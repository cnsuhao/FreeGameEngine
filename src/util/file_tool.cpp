#include "file_tool.h"
#include "log_tool.h"

#include "resmgr/bwresource.hpp"
#include "resmgr/bin_section.hpp"
#include "cstdmf/bw_util.hpp"

namespace ora
{

    bool readFile(std::string & content, const std::string & filename, bool isBinary)
    {
        DataSectionPtr section = BWResource::openSection(filename, false, BinSection::creator());
        if(!section)
            return false;
            
        BinaryPtr bin = section->asBinary();
        if (!bin)
            return false;

        content.assign(bin->cdata(), bin->len());
        return true;
    }

    bool writeFile(const std::string & content, const std::string & filename, bool isBinary)
    {
        if(filename.empty())
            return false;
        
        if(filename[0] == '/' || (filename.size() >= 2 && filename[1] == ':')) //an absolute path
        {
            FILE* pFile = bw_fopen(filename.c_str(), "wb");
            if(!pFile)
                return false;
                
            fwrite(content.c_str(), content.size(), 1, pFile);
            fclose(pFile);
            return true;
        }
        else
        {
            DataSectionPtr section = BWResource::openSection(filename, true, BinSection::creator());
            
            if(!section)
                return false;
            
            BinaryPtr bin = new BinaryBlock(content.c_str(), content.length(), "writeFile");
            section->setBinary(bin);
            return section->save();
        }
    }

}// end namespace ora
