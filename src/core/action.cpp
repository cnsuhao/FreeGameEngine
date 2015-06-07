//
//  action.cpp
//  orange_client
//
//  Created by Kevin on 14-9-17.
//
//

#include "action.h"
#include "scene_node.h"
#include "util/watcher.h"

namespace ora{
    static int g_numAction = 0;

    IAction::IAction(): interval_(0.f)
    , elapse_(0.f)
    , enable_(false)
    , owner_(nullptr)
    {
        if(g_numAction++ == 0)
        {
            ADD_WATCHER("counter/Action", g_numAction);
        }
    }
    
    IAction::~IAction()
    {
        --g_numAction;
    }
    
    void IAction::draw()
    {
    
    }
    
    void IAction::tick(float dtime)
    {
    
    }
    
    void IAction::finishCB()
    {
        
    }
    
    void IAction::pause()
    {
    
    }
    
	bool IAction::ifFinished()
	{
		return elapse_ >= interval_;
	}
}; // end of namespace ora
