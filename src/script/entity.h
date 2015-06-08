//
//  IEntity.h
//  libserver
//
//  Created by Kevin on 14-8-12.
//  Copyright (c) 2014. All rights reserved.
//

#ifndef __libserver_enitty__
#define __libserver_enitty__

#include "util/smartptr.h"
#include "math/vector3.hpp"
#include "core/scene_node.h"

namespace ora{
    typedef SmartPtr<class IEntity> EntityPtr;

	class IEntity : public SceneNode
	{
	public:

		static EntityPtr create();

		IEntity();
		virtual ~IEntity();
		

		inline void velocity(Vector3 v){ velocity_ = v; }
		inline Vector3 velocity() const { return velocity_; }

        float yaw() const;
		float pitch() const;
		float roll() const;

		void setYaw(float y);

		virtual void tick(float dtime);

	protected:
		Vector3             velocity_;
	};

}; // end of namespace ora

#endif // __libserver_enitty__

