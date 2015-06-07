//
//  action_move.h
//  orange_client
//
//  Created by Kevin on 14-9-17.
//
//

#ifndef __orange_client__action_move__
#define __orange_client__action_move__

#include "action.h"
#include "math/Vector3.hpp"

namespace ora {
    
    enum ACTION_INTER_TYPE
    {
        inter_liner = 0,
        inter_sin,
        inter_exp,
        inter_log
    };
    
    typedef SmartPtr<class ActionMove> ActionMovePtr;
    
    class ActionMove : public IAction
    {
    public:
        ActionMove();
        virtual ~ActionMove();
        
        virtual void tick(float dtime) override;
		virtual void finishCB() override;
        
        // for script
        Vector3             src_;
        Vector3             dst_;
        ACTION_INTER_TYPE   interType_;
        
    protected:
        virtual void onTick(float dtime);
    };
    
    typedef SmartPtr<class ActionScale> ActionScalePtr;
    class ActionScale : public ActionMove
    {
    public:
        ActionScale();
        virtual ~ActionScale();

		virtual void finishCB() override;
        
        Vector3             src_;
        Vector3             dst_;
        
    protected:
        virtual void onTick(float dtime);
    };
    
}; // end of namespace ora

#endif /* defined(__orange_client__action_move__) */
