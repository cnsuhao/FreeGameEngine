//
//  action_container.h
//  orange_client
//
//  Created by Kevin on 14-9-17.
//
//

#ifndef __orange_client__action_container__
#define __orange_client__action_container__

#include "action.h"

#include <list>

namespace ora{
    
    typedef SmartPtr<IAction> ActionPtr;
    typedef std::list<ActionPtr> ActionContainer;
    
    class ActionContainerBase : public IAction
    {
    public:
        ActionContainerBase();
        virtual ~ActionContainerBase();
        
        typedef ActionContainer::iterator iterator;
        typedef ActionPtr value_type;
        
        ActionContainer::iterator begin();
        ActionContainer::iterator end();
        void clear();
        
        void push(ActionPtr action);
        void pop(ActionPtr action);
        
        virtual void draw() override;
        virtual void tick(float dtime) override;
		virtual bool ifFinished() override;

    protected:
        virtual void owner(SceneNode* owner);
        
    protected:
        ActionContainer container_;
    };
    
    typedef SmartPtr<class ActionContainerOder> ActionContainerOderPtr;
    class ActionContainerOder : public ActionContainerBase
    {
    public:
        ActionContainerOder();
        virtual ~ActionContainerOder();
        
        virtual void draw() override;
        virtual void tick(float dtime) override;
        
        virtual void finishCB() override;
        friend class SceneNode;
    };
    
    typedef SmartPtr<class ActionContainerOnce> ActionContainerOncePtr;
    class ActionContainerOnce : public ActionContainerBase
    {
    public:
        ActionContainerOnce();
        virtual ~ActionContainerOnce();
        
        virtual void draw() override;
        virtual void tick(float dtime) override;
        
        virtual void finishCB() override;
        
    };
    
}; // end of namespace ora

#endif /* defined(__orange_client__action_container__) */
