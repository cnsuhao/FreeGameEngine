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

#if defined (ORA_SERVER)
#else
#include "math/vector3.hpp"
#include "duplo/pymodel.hpp"
#include "duplo/action_matcher.h"
#endif

namespace ora{
    typedef SmartPtr<class IEntity> EntityPtr;

#if defined (ORA_SERVER)
	class IEntity : public IReferenceCount
#else
	class IEntity : public SceneNode
#endif
	{
	public:

		static EntityPtr create();

		IEntity();
		virtual ~IEntity();
		
#if defined (ORA_SERVER)
#else

		inline void velocity(Vector3 v){ velocity_ = v; }
		inline Vector3 velocity() const { return velocity_; }

		inline PyModel* getModel() const { return pyModel_.get(); }

        float yaw() const;
		float pitch() const;
		float roll() const;

		void setYaw(float y);

		void setModel(PyModelPtr model);

		ActionMatcher* getAM() const { return am_.get(); }

		virtual void tick(float dtime);

	protected:


	private:
		PyModelPtr          pyModel_;
		Vector3             velocity_;
		ActionMatcherPtr	am_;
#endif
	};

}; // end of namespace ora

#endif // __libserver_enitty__

