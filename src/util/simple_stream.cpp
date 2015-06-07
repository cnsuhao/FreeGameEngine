//
//  simple_stream.cpp
//  liborange
//
//  Created by Kevin on 14-4-14.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#include "simple_stream.h"
#include "file_tool.h"

namespace ora
{

    bool ifKeyValid(const std::string & data, std::string)
    {
        for (auto iter : data)
        {
            if ( !(
                        (iter >= 'a' && iter <= 'z')
                        || (iter >= 'A' && iter <= 'Z')
                        || (iter >= '0' && iter <= '9')
                        || (iter == '_')
                    ) )
                return false;
        }
        return true;
    }

    bool ifKeyValid(const std::string & data, uint32)
    {
        for ( auto iter : data)
        {
            if ( !(iter >= '0' && iter <= '9') )
                return false;
        }
        return true;
    }

    bool convertKey(const std::string & key, uint32 & out)
    {
        out = atoi(key.c_str());
        return true;
    }

    bool convertKey(const std::string & key, std::string & out)
    {
        out = key;
        return true;
    }

    ISimpleStream::ISimpleStream()
    {
    }

    ISimpleStream::~ISimpleStream()
    {
    }

    bool ISimpleStream::loadFromFile(const std::string & filename)
    {
        if ( filename.empty() )
            return false;

        std::string data;
        if (!readFile(data, filename, false))
            return false;

        return loadFromMemory(data);
    }

    bool ISimpleStream::loadFromMemory(const std::string & data)
    {
        size_t pos = 0;
        while ( pos < data.size() )
        {
            size_t a = data.find_first_of('=', pos);
            if ( a == std::string::npos )
                break;

            size_t b = data.find_first_of('\n', pos);
            if ( b == std::string::npos )
                b = data.size();

            std::string key = data.substr(pos, a - pos);
            std::string value = data.substr(a + 1, b - a);

            trimString(key);
            trimString(value);

            value = valueStringUnformat(value);

            insert(key, value);

            pos = b + 1;
        }

        //print();

        return true;
    }

}; // end of namespace ora