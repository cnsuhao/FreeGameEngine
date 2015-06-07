//
//  action_container.cpp
//  orange_client
//
//  Created by Kevin on 14-9-17.
//
//

#include "action_container.h"
#include "scene_node.h"

namespace ora{

    ActionContainerBase::ActionContainerBase():
		IAction()
    {
    }
    
    ActionContainerBase::~ActionContainerBase()
    {
    
    }
    
    ActionContainer::iterator ActionContainerBase::begin()
    {
        return container_.begin();
    }
    
    ActionContainer::iterator ActionContainerBase::end()
    {
        return container_.end();
    }
    
    void ActionContainerBase::push(ActionPtr action)
    {
        action->owner(owner_);
        container_.push_back(action);
    }
    
    void ActionContainerBase::clear()
    {
        for (value_type action : container_)
        {
            action->owner(nullptr);
        }
        container_.clear();
    }
    
    void ActionContainerBase::pop(ActionPtr action)
    {
        iterator iter = std::find(begin(), end(), action);
        if ( iter != end() )
        {
            action->owner(nullptr);
            container_.erase(iter);
        }
    }

    void ActionContainerBase::draw()
    {

    }
    
    void ActionContainerBase::tick(float dtime)
    {

    }

    void ActionContainerBase::owner(SceneNode* owner)
    {
        owner_ = owner; 

        for (value_type action : container_)
        {
            action->owner(owner_);
        }
    }
    
	bool ActionContainerBase::ifFinished()
	{
		return container_.empty();
	}

    //////////////////////////////////////////////////
    ActionContainerOder::ActionContainerOder():
    ActionContainerBase()
    {
    
    }
    
    ActionContainerOder::~ActionContainerOder()
    {
    
    }
    
    void ActionContainerOder::finishCB()
    {
        
    }
    
    void ActionContainerOder::draw()
    {
		if ( ifFinished() )
			return;

        value_type action = container_.front();
        
		action->draw();
    }
    
    void ActionContainerOder::tick(float dtime)
    {
		if ( ifFinished() )
		{
            finishCB();
			return;
		}
		
        value_type action = container_.front();
		action->tick(dtime);

		if ( action->ifFinished() )
        {
            pop(action);
        }
    }
    
    //////////////////////////////////////////////////
    ActionContainerOnce::ActionContainerOnce():
    ActionContainerBase()
    {
        
    }
    
    ActionContainerOnce::~ActionContainerOnce()
    {
    
    }
    
    void ActionContainerOnce::finishCB()
    {
    }
    
    void ActionContainerOnce::draw()
    {   
        for ( value_type action : *this )
        {
            action->draw();
        }
    }
    
    void ActionContainerOnce::tick(float dtime)
    {
		if ( ifFinished() )
		{
            finishCB();
			return;
		}

		iterator iter = begin();
		while ( iter != end() )
		{
			(*iter)->tick(dtime);
			if ( (*iter)->ifFinished() )
			{
                (*iter)->owner(nullptr);
				iter = container_.erase(iter);
			}
			else
			{
                ++iter;
            }
		}
    }
    
    
}; // end of namespace ora