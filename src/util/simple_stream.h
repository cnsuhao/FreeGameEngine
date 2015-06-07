//
//  simple_stream.h
//  liborange
//
//  Created by Kevin on 14-4-14.
//  Copyright (c) 2014. All rights reserved.
//

#ifndef __liborange__simple_stream__
#define __liborange__simple_stream__

#include "smartptr.h"
#include "data_stream.h"
#include "assert_tool.h"
#include "string_tool.h"

#include <map>

namespace ora
{

    bool ifKeyValid(const std::string & data, std::string);
    bool ifKeyValid(const std::string & data, uint32);

    bool convertKey(const std::string & key, uint32 & out);
    bool convertKey(const std::string & key, std::string & out);

    class ISimpleStream : public IReferenceCount
    {
    public:
        ISimpleStream();
        virtual ~ISimpleStream();

        bool loadFromFile(const std::string & filename);
        bool loadFromMemory(const std::string & data);
        virtual void print() = 0;

    protected:
        virtual bool insert(const std::string & key, const std::string & value) = 0;
    };

    /// class SimpleStream
    template<typename T>
    class SimpleStream : public ISimpleStream
    {
    public:

        typedef std::map<T, std::string> StringTablePool;

        SimpleStream()
        {};

        ~SimpleStream()
        {};

        const std::string & get(const T & key) const
        {
            auto iter = pool_.find(key);
            IF_NOT_ASSERT(iter != pool_.end())
                return EmptyString;

            return iter->second;
        }

        virtual void print()
        {
            std::ostringstream ss;
            for (auto iter : pool_)
                ss << iter.first << "," << iter.second << std::endl;

            //CCLOG("the simple stream is %s", ss.str().c_str());
        }

        virtual bool insert(const std::string & key, const std::string & value)
        {
            if ( !ifKeyValid(key, T()) )
                return false;

            T t;
            if ( !convertKey(key, t) )
                return false;

            pool_[t] = value;

            return true;
        }

    private:
        StringTablePool pool_;
    };

    typedef SimpleStream<std::string> StrStream;
    typedef SimpleStream<uint32> UintStream;

    typedef SmartPtr<StrStream> StrStreamPtr;
    typedef SmartPtr<UintStream> UintStreamPtr;

}; // end of namespace ora

#endif /* defined(__liborange__simple_stream__) */
