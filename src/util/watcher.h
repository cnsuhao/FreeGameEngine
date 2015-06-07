//
//  watcher.h
//  liborange
//
//  Created by zhoubao on 14-5-8.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#ifndef __liborange__watcher__
#define __liborange__watcher__

#ifndef _RELEASE

#include "smartptr.h"
#include "singleton.h"

#include <sstream>
#include <map>

namespace ora
{

    typedef SmartPtr<class Watcher> WatcherPtr;
    
    template<typename T>
    void string2value(T & v, const std::string & str)
    {
        std::istringstream ss(str);
        ss >> v;
    }
    
    inline void string2value(std::string & v, const std::string & str)
    {
        v = str;
    }
    
    template<typename T>
    void value2string(std::string & v, const T & t)
    {
        std::ostringstream ss;
        ss << t;
        v = ss.str();
    }
    
    inline void value2string(std::string & v, const std::string & str)
    {
        v = str;
    }
    
    inline void value2string(std::string & v, const char* str)
    {
        v = str;
    }
    
    class Watcher : public IReferenceCount
    {
    public:
        typedef std::map<std::string, WatcherPtr> Children;
        
        Watcher();
        virtual ~Watcher();
        
        virtual bool isReadOnly() const { return true; }
        
        virtual std::string getValue() const;
        virtual void setValue(const std::string & v);
        
        void addChild(const std::string & name, WatcherPtr watch);
        void delChild(const std::string & name);
        WatcherPtr getChild(const std::string & name, bool createIfMiss=false);
        void clearChildren();
        const Children & getChildren() const { return children_; }
        
        void print(std::ostream & ss, int depth=0) const;
        const std::string & getName() const { return name_; }
        
        static Watcher * root();
        
    protected:
        void addChild_(const std::string & name, WatcherPtr watch);
        void delChild_(const std::string & name);
        WatcherPtr getChild_(const std::string & name, bool createIfMiss);
    
        std::string name_;
        Children    children_;
    };

    //this a watcher root
    class WatcherMgr :
        public Watcher,
        public Singleton<WatcherMgr>
    {
    public:
        WatcherMgr();
        ~WatcherMgr();
    };
    
    template<typename T>
    class WatchValue : public Watcher
    {
    public:
        explicit WatchValue(T & v, bool readOnly=false)
            : value_(v)
            , readOnly_(readOnly)
        {}
        
        ~WatchValue()
        {}
        
        bool isReadOnly() const override { return readOnly_; }
        
        std::string getValue() const override
        {
            std::string str;
            value2string(str, value_);
            return str;
        }
        
        void setValue(const std::string & v) override
        {
            if(readOnly_) return;
            
            string2value(value_, v);
        }
        
    private:
        T & value_;
        bool readOnly_;
    };
    
    template<typename T, typename U>
    class WatchMethod: public Watcher
    {
    public:
        typedef T (U::*GET_METHOD)() const;
        typedef void (U::*SET_METHOD)(const T & v);
    
        WatchMethod(U * target, GET_METHOD get, SET_METHOD set=nullptr)
            : target_(target)
            , get_(get)
            , set_(set)
        {}
        
        ~WatchMethod()
        {}
        
        bool isReadOnly() const override { return nullptr == set_; }
        
        std::string getValue() const override
        {
            std::string str;
            value2string(str, (target_->*get_)());
            return str;
        }
        
        void setValue(const std::string & v) override
        {
            if(nullptr == set_) return;
            
            T value;
            string2value(value, v);
            (target_->*set_)(value);
        }
        
    private:
        U           *target_;
        GET_METHOD  get_;
        SET_METHOD  set_;
    };
    
    template<typename T>
    class WatchFunction: public Watcher
    {
    public:
        typedef T (*GET_METHOD)();
        typedef void (*SET_METHOD)(T v);
        
        WatchFunction(GET_METHOD get, SET_METHOD set=nullptr)
            : get_(get)
            , set_(set)
        {}
        
        ~WatchFunction()
        {}
        
        bool isReadOnly() const override { return nullptr == set_; }
        
        std::string getValue() const override
        {
            std::string str;
            value2string(str, get_());
            return str;
        }
        
        void setValue(const std::string & v) override
        {
            if(nullptr == set_) return;
            
            T value;
            string2value(value, v);
            set_(value);
        }
        
    private:
        GET_METHOD  get_;
        SET_METHOD  set_;
    };
    
    template<typename T>
    void addWatcher(const char * name, T & v, bool readOnly=true)
    {
        WatcherPtr watcher( new WatchValue<T>(v, readOnly) );
        Watcher::root()->addChild(name, watcher);
    }
    
    
    template<typename T, typename U>
    void addWatcher(const char * name,
                    U * target,
                    T (U::*getmethod)() const,
                    void (U::*setmethod)(const T &)=nullptr)
    {
        WatcherPtr watcher = new WatchMethod<T, U>(target, getmethod, setmethod);
        Watcher::root()->addChild(name, watcher);
    }
    
    template<typename T>
    void addWatcher(const char * name,
                    T (*getmethod)(),
                    void (*setmethod)(T)=nullptr)
    {
        WatcherPtr watcher = new WatchFunction<T>(getmethod, setmethod);
        Watcher::root()->addChild(name, watcher);
    }
    
    
}// end namespace ora

#define ADD_WATCHER         ::ora::addWatcher
#define DEL_WATCHER(name)   ::ora::Watcher::root()->delChild(name)

#else // _RELEASE
#   define ADD_WATCHER(...)
#   define DEL_WATCHER(...)
#endif // _RELEASE
    
#endif /* defined(__liborange__watcher__) */
