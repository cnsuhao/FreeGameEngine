#if defined(ORA_SERVER)
#else
#include "duplo/pch.hpp"
#endif
#include "entity.h"
#include "util/watcher.h"

namespace ora{

    static int g_numEntity = 0;

	/*static*/ EntityPtr IEntity::create()
	{
		EntityPtr entity = new IEntity();
		return entity;
	}

	IEntity::IEntity()
	{
        if(g_numEntity == 0)
        {
            ADD_WATCHER("counter/Entity", g_numEntity);
        }
        ++g_numEntity;

#ifndef ORA_SERVER
		am_ = new ActionMatcher(this);
#endif
	}

	void IEntity::tick(float dtime)
	{
#ifndef ORA_SERVER
		SceneNode::tick(dtime);
		am_->rev(dtime);
#endif
	}

	IEntity::~IEntity()
	{
        --g_numEntity;
	}

    void IEntity::setModel(PyModelPtr model) 
	{
		pyModel_ = model; 
        if(pyModel_)
            pyModel_->pSuperModel()->getMatchableActions(am_->matchedActions_);
	}

    float IEntity::yaw() const 
    {
        return getWorldMatrix().yaw();
    }

	float IEntity::pitch() const 
    { 
        return getWorldMatrix().pitch();
    }

	float IEntity::roll() const
    {
        return getWorldMatrix().roll();
    }

	void IEntity::setYaw(float y)
    {
        this->setRotationY(y + PI_HALF);
    }


}; // end of namespace ora
