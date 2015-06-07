//
//  action_move.cpp
//  orange_client
//
//  Created by Kevin on 14-9-17.
//
//

#include "action_motion.h"
#include "scene_node.h"

namespace ora {
    ///////////////////////////////////////////////////////
#define VALID_INTER_RANGE(dt, start, end)\
if (dt <= 0.f ) return start;\
if (dt >= 1.f ) return end;
    
    const float expX = 4.f;
    const float expY = exp(expX)-1.f;
    
    const float logX = expY;
    const float logY = log(logX+1);
    
    inline float interLiner(float dt, float start, float end)
    {
        VALID_INTER_RANGE(dt, start, end);
        return start + ( end  - start ) * dt;
    }
    
    inline float interSin(float dt, float start, float end)
    {
        VALID_INTER_RANGE(dt, start, end);
        return start + (end - start) * sin(dt * PI_HALF);
    }
    
    inline float interExp(float dt, float start, float end)
    {
        VALID_INTER_RANGE(dt, start, end);
        dt = (exp(dt*expX)-1)/expY;
        return start + (end-start)*dt;
    }
    
    inline float interLog(float dt, float start, float end)
    {
        VALID_INTER_RANGE(dt, start, end)
        dt = log(dt*logX+1)/logY;
        return start + (end - start) * dt;
    }
    
    float interByType(ACTION_INTER_TYPE type, float dt, float start, float end)
    {
        switch (type) {
            case ACTION_INTER_TYPE::inter_sin:
                return interSin(dt, start, end);
                break;
                
            case ACTION_INTER_TYPE::inter_exp:
                return interExp(dt, start, end);
                break;
                
            case ACTION_INTER_TYPE::inter_log:
                return interLog(dt, start, end);
                break;
                
            default:
                return interLiner(dt, start, end);
                break;
        }
    }
    
    
    ///////////////////////////////////////////////////////
    ActionMove::ActionMove():
    IAction(),
    src_(Vector3::zero()),
    dst_(src_),
    interType_(ACTION_INTER_TYPE::inter_liner)
    {
        
    }
    
    ActionMove::~ActionMove()
    {
    
    }
    
	void ActionMove::finishCB()
	{
		owner_->setPosition(dst_);
	}

    void ActionMove::onTick(float dtime)
    {
		if ( almostZero(interval_) || ifFinished() )
            return;
        
        elapse_ += dtime;
        
        if ( elapse_ >= interval_ )
			elapse_ = interval_;
        
        float dt = elapse_ / interval_;

        Vector3 tmp;
        tmp.x = interByType(interType_, dt, src_.x, dst_.x);
        tmp.y = interByType(interType_, dt, src_.y, dst_.y);
        tmp.z = interByType(interType_, dt, src_.z, dst_.z);
        
        owner_->setPosition(tmp);

		if ( ifFinished() )
			finishCB();
    }
    
    void ActionMove::tick(float dtime)
    {
        if ( !owner_ )
            return;
        
        onTick(dtime);
    }
    
    ///////////////////////////////////////////////////////
    ActionScale::ActionScale():ActionMove(),
    src_(Vector3::zero()),
    dst_(src_)
    {
    
    }
    
    ActionScale::~ActionScale()
    {
    
    }
    
	void ActionScale::finishCB()
	{
		owner_->setScale(dst_);
	}

    void ActionScale::onTick(float dtime)
    {
        if ( almostZero(interval_) || ifFinished() )
            return;
        
        elapse_ += dtime;
        
        if ( elapse_ >= interval_ )
			elapse_ = interval_;
        
        float dt = elapse_ / interval_;
        
        Vector3 tmp;
        tmp.x = interByType(interType_, dt, src_.x, dst_.x);
        tmp.y = interByType(interType_, dt, src_.y, dst_.y);
        tmp.z = interByType(interType_, dt, src_.z, dst_.z);
        
		owner_->setScale(tmp);

		if ( ifFinished() )
			finishCB();
    }
    
}; // end of namespace ora
