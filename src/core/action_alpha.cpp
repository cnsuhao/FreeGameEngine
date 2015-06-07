//
//  alpha_actioncpp.cpp
//  orange_client
//
//  Created by Kevin on 14-9-17.
//
//

#include "action_alpha.h"
#include "graphics/effect_constant.h"
#include "scene_node.h"
#include "action_container.h"

namespace ora{
    
    ActionAlpha::ActionAlpha():IAction(),
    src_(0.f),
    dst_(0.f)
    {
    
    }
    
    ActionAlpha::~ActionAlpha()
    {
    }
    
    void ActionAlpha::finishCB()
    {
        IAction::finishCB();
		owner_->setColor(Vector4(1.f, 1.f, 1.f, dst_));
    }

    void ActionAlpha::onTick(float dtime)
    {
        if ( almostEqual(src_, dst_) || 
			almostZero(interval_) || 
			elapse_ >= interval_ )
            return;

        elapse_ += dtime;
        if ( elapse_ >= interval_)
			elapse_ = interval_;

        float speed = (dst_ - src_) / interval_;
        float alpha = elapse_ * speed + src_;
        owner_->setColor(Vector4(1.f, 1.f, 1.f, alpha));

		if ( ifFinished() )
            finishCB();
    }
    
    void ActionAlpha::draw()
    {

    }
    
    void ActionAlpha::tick(float dtime)
    {   
        onTick(dtime);
    }
    
}; // end of namespace ora