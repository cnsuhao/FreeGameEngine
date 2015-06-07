//
//  action.h
//  orange_client
//
//  Created by Kevin on 14-9-17.
//
//

#ifndef __orange_client__action__
#define __orange_client__action__

#include "util/smartptr.h"

namespace ora{
    
    class SceneNode;
    typedef SmartPtr<class IAction> ActionPtr;
    
    class IAction : public IReferenceCount
    {
	public:
		IAction();
        virtual ~IAction();
        
        virtual void draw();
        virtual void tick(float dtime);
        virtual void finishCB();
        virtual void pause();
		virtual bool ifFinished();
        
		SceneNode* owner(){ return owner_; }
        
        float getElapse() { return elapse_; }
        
        float interval_;
        
    protected:
        float elapse_;
        SceneNode* owner_;
		virtual void owner(SceneNode* owner){ owner_ = owner; }

    private:
        bool enable_;

        friend class SceneNode;
        friend class ActionContainerBase;
        friend class ActionContainerOnce;
    };
    
}; // end of namespace ora

#endif /* defined(__orange_client__action__) */
