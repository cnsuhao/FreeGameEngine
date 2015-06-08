#ifndef _SHADER_PARSER_H
#define _SHADER_PARSER_H

#include "util/smartptr.h"
#include "util/singleton.h"

#include <map>
#include <thread>

namespace ora
{
    
    typedef SmartPtr<class ShaderCode> ShaderCodePtr;

    class ShaderCode : public IReferenceCount
    {
    public:
        typedef std::map<std::string, int> TSemantics;

        ShaderCode(const std::string & resource);

        bool dump(const std::string & filename);

        int findSemantics(const std::string & name) const;

        const std::string & getCode() const { return code_; }
        const TSemantics & getSemantics() const { return semantics_; }

    private:

        void processSemantics();
        bool merge(ShaderCodePtr shader);

        std::string resource_;
        std::string code_;
        TSemantics semantics_;

        friend class ShaderParser;
    };

    class ShaderParser :
        public Singleton<ShaderParser>
    {
    public:
        ShaderParser();
        ~ShaderParser();

        ShaderCodePtr loadShader(const std::string & filename);

    private:

        bool parseFile(const std::string & filename, ShaderCodePtr shader);
        bool parseInclude(const std::string & filename, ShaderCodePtr shader);
        bool parseCode(const std::string & filename, const std::string & code, ShaderCodePtr shader);

    private:
        //file  -> code
        typedef std::map<std::string, ShaderCodePtr> CodeCache;
        CodeCache       codeCache_;
        std::mutex      mutex_;
    };

} // end namespace ora

#endif //_SHADER_PARSER_H