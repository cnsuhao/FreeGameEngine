//
//  action_self_destruction.h
//  orange_client
//
//  Created by Kevin on 14-10-17.
//
//

#ifndef __orange_client_alpha_self_destruction__
#define __orange_client_alpha_self_destruction__

#include "action.h"

namespace ora{

	class ActionSelfDestruction : public IAction
	{
	public:
		ActionSelfDestruction();
		virtual ~ActionSelfDestruction();
		virtual void finishCB() override;
		virtual bool ifFinished() override;

		virtual void tick(float time) override;

	protected:
		virtual void onTick(float);
	private:
		bool finished_;
	};

}; // end of namespace ora

#endif // __orange_client_alpha_self_destruction__