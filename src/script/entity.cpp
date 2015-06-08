
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
	}

	void IEntity::tick(float dtime)
	{
		SceneNode::tick(dtime);

	}

	IEntity::~IEntity()
	{
        --g_numEntity;
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
