//
//  alpha_actioncpp.h
//  orange_client
//
//  Created by Kevin on 14-9-17.
//
//

#ifndef __orange_client__alpha_actioncpp__
#define __orange_client__alpha_actioncpp__

#include "action.h"

namespace ora{
    
    typedef SmartPtr<class AlphaAction> AlphaActionPtr;
    
    class ActionAlpha : public IAction
    {
    public:
        ActionAlpha();
        virtual ~ActionAlpha();
        
        virtual void draw() override;
        virtual void tick(float dtime) override;
        virtual void finishCB() override;
        
        // for script
        float src_;
        float dst_;
        
    protected:
        void onTick(float dtime);
    };
    
}; // end of namespace ora

#endif /* defined(__orange_client__alpha_actioncpp__) */
