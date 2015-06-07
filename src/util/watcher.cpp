//
//  watcher.cpp
//  liborange
//
//  Created by zhoubao on 14-5-8.
//  Copyright (c) 2014 jjgame. All rights reserved.
//

#ifndef _RELEASE

#include "watcher.h"
#include "string_tool.h"

namespace ora
{
    
    /*static*/ Watcher * Watcher::root()
    {
        return WatcherMgr::instance();
    }


    IMPLEMENT_SINGLETON(WatcherMgr);
    WatcherMgr::WatcherMgr()
    {
        name_ = "root";
    }

    WatcherMgr::~WatcherMgr()
    {}
    

    Watcher::Watcher()
    {
    }
    
    Watcher::~Watcher()
    {
    }
    
    std::string Watcher::getValue() const
    {
        return "group";
    }
    
    void Watcher::setValue(const std::string & v)
    {
    }
    
    void Watcher::addChild_(const std::string & name, WatcherPtr watch)
    {
        watch->name_ = name;
        children_[name] = watch;
    }
    
    void Watcher::addChild(const std::string & name, WatcherPtr watch)
    {
        size_t pos = name.find_last_of('/');
        if(pos == name.npos)
            addChild_(name, watch);
        else
            getChild(name.substr(0, pos), true)->addChild_(name.substr(pos+1), watch);
    }
    
    void Watcher::delChild_(const std::string & name)
    {
        auto it = children_.find(name);
        if(it != children_.end())
            children_.erase(it);
    }
    
    void Watcher::delChild(const std::string & name)
    {
        size_t pos = name.find('/');
        if(pos == name.npos) return delChild_(name);
        
        WatcherPtr child = getChild_(name.substr(0, pos), false);
        if(child) child->delChild(name.substr(pos+1));
    }
    
    WatcherPtr Watcher::getChild_(const std::string & name, bool createIfMiss)
    {
        auto it = children_.find(name);
        if(it != children_.end())
        {
            return it->second;
        }
        else if(createIfMiss)
        {
            WatcherPtr child = new Watcher();
            addChild_(name, child);
            return child;
        }
        
        return nullptr;
    }
    
    WatcherPtr Watcher::getChild(const std::string & name, bool createIfMiss/*=false*/)
    {
        size_t pos = name.find('/');
        if(pos == name.npos) return getChild_(name, createIfMiss);
        
        WatcherPtr child = getChild_(name.substr(0, pos), createIfMiss);
        if(child) return child->getChild(name.substr(pos+1), createIfMiss);
        
        return nullptr;
    }
    
    void Watcher::clearChildren()
    {
        children_.clear();
    }
    
    void Watcher::print(std::ostream & ss, int depth) const
    {
        ss << TabProxy(depth) << name_ << " : " << getValue() << std::endl;
        
        for(auto it : children_)
        {
            it.second->print(ss, depth + 1);
        }
    }
    
}// end namespace ora

#endif //#ifndef _RELEASE
