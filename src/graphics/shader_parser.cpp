

#include "util/string_tool.h"
#include "util/file_tool.h"
#include "util/log_tool.h"

#include "shader_parser.h"
#include "vertex_declaration.h"

namespace ora
{
    static const char * ShaderMagic = "\xff\x6f\x73\x68";
    static const uint8 ShaderPWD[32] = { 
        148, 237, 80, 109, 185, 134, 86, 145,
        95, 211, 193, 16, 53, 91, 31, 166,
        246, 114, 140, 188, 47, 228, 202, 130,
        234, 131, 138, 73, 187, 215, 220, 99
    };
    ////////////////////////////////////////////////////////////////////
    /// ShaderCode
    ////////////////////////////////////////////////////////////////////
    ShaderCode::ShaderCode(const std::string & resource)
        : resource_(resource)
    {
    }

    bool ShaderCode::dump(const std::string & filename)
    {
        return writeFile(code_, filename, true);
    }

    int ShaderCode::findSemantics(const std::string & name) const
    {
        TSemantics::const_iterator it = semantics_.find(name);
        if(it != semantics_.end())
            return it->second;

        return int(VertexUsage::NONE);
    }

    void ShaderCode::processSemantics()
    {
        size_t pos = 0;
        while(pos < code_.size())
        {
            pos = code_.find("/*:", pos);
            if(pos == std::string::npos)
                break;

            //find the semantics.
            size_t beg = pos + 3;
            size_t end = code_.find("*/", beg);
            if(end == std::string::npos)
                break;

            std::string semantics = code_.substr(beg, end - beg);
            trimString(semantics);
            
            beg = pos - 1; //prepare for search name.
            pos = end + 2; //prepare for the next search.

            //find the variable name.
            end = code_.find_last_not_of(" \t\r\n", beg);
            if(end == std::string::npos)
                continue;

            beg = code_.find_last_of(" \t\r\n", end);
            if(beg == std::string::npos)
                continue;

            std::string name = code_.substr(beg+1, end - beg);

            VertexUsage usage = vertexAttr2Usage(semantics);
            if(usage == VertexUsage::NONE)
            {
                ORA_LOG_ERROR("Invalid semantics '%s' in '%s'",
                    semantics.c_str(), resource_.c_str());
            }

            semantics_[name] = int(usage);
        }
    }

    bool ShaderCode::merge(ShaderCodePtr shader)
    {
        if(!shader) return false;

        code_ += shader->code_;
        semantics_.insert(shader->semantics_.begin(), shader->semantics_.end());
        return true;
    }

    ////////////////////////////////////////////////////////////////////
    /// ShaderParser
    ////////////////////////////////////////////////////////////////////
    IMPLEMENT_SINGLETON(ShaderParser);

    ShaderParser::ShaderParser()
    {

    }

    ShaderParser::~ShaderParser()
    {

    }

    ShaderCodePtr ShaderParser::loadShader(const std::string & filename)
    {
        std::lock_guard<std::mutex> holder(mutex_);

        CodeCache::iterator it = codeCache_.find(filename);
        if (it != codeCache_.end())
        {
            return it->second;
        }

        ShaderCodePtr shader = new ShaderCode(filename);
        if(!parseFile(filename, shader))
        {
            shader = nullptr;
        }

        codeCache_[filename] = shader;
        return shader;
    }

    bool ShaderParser::parseFile(const std::string & filename, ShaderCodePtr shader)
    {
        ASSERT_1(codeCache_.find(filename) == codeCache_.end());

        //prevent loop include.
        codeCache_[filename] = nullptr;

        std::string code;
        if (!readFile(code, filename, true))
        {
            return false;
        }

        if (code.size() >= 4)
        {
            if (memcmp(code.c_str(), ShaderMagic, 4) == 0)
            {
                size_t realSize = code.size() - 4;
                for (size_t i = 0; i < realSize; ++i)
                {
                    code[i] = code[i + 4] ^ ShaderPWD[i % 32];
                }
                code.resize(realSize);
            }
        }

        ShaderCodePtr thisShader = new ShaderCode(filename);
        if (!parseCode(filename, code, thisShader))
        {
            return false;
        }
        thisShader->processSemantics();

        codeCache_[filename] = thisShader;
        return shader->merge(thisShader);
    }

    bool ShaderParser::parseInclude(const std::string & filename, ShaderCodePtr shader)
    {
        CodeCache::iterator it = codeCache_.find(filename);
        if (it != codeCache_.end())
        {
            return shader->merge(it->second.get());
        }

        return parseFile(filename, shader);
    }

    bool ShaderParser::parseCode(const std::string & filename, const std::string & code, ShaderCodePtr shader)
    {
        ASSERT_1(shader);

        size_t last = 0;
        while (true)
        {
            //parse include file.
            size_t pos = code.find("#include", last);
            if (pos == code.npos)
                break;

            size_t iStart = code.find("\"", pos + 8);
            if (iStart == code.npos)
            {
                ORA_LOG_ERROR("Failed to parse shader - invlid symbol '#include'! file '%s'", filename.c_str());
                return false;
            }
            iStart += 1;

            size_t iEnd = code.find("\"", iStart);
            if (iEnd == code.npos)
            {
                ORA_LOG_ERROR("Failed to parse shader - invlid symbol '#include'! file '%s'", filename.c_str());
                return false;
            }

            shader->code_ += code.substr(last, pos - last);

            std::string includeFile = code.substr(iStart, iEnd - iStart);
            std::string filePath = getFilePath(filename);

            if(FileSystemMgr::instance()->isFileExist(filePath + includeFile))
                includeFile = filePath + includeFile;

            if (!parseInclude(includeFile, shader))
                return false;

            last = iEnd + 1;
        }

        shader->code_ += code.substr(last);
        return true;
    }

} // end namespace ora