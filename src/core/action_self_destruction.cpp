//
//  action_self_destruction.cpp
//  orange_client
//
//  Created by Kevin on 14-10-17.
//
//

#include "action_self_destruction.h"
#include "scene_node.h"

namespace ora{

	ActionSelfDestruction::ActionSelfDestruction() : IAction()
		,finished_(false)
	{
	
	}

	ActionSelfDestruction::~ActionSelfDestruction()
	{
	
	}

	bool ActionSelfDestruction::ifFinished()
	{
		return finished_;
	}


	void ActionSelfDestruction::finishCB()
	{
        IAction::finishCB();

		owner_->removeFromParent(); 

		finished_ = true;
	}

	void ActionSelfDestruction::tick(float dtime)
	{
		if ( !owner_ )
			return;

		onTick(dtime);
	}

	void ActionSelfDestruction::onTick(float dtime)
	{
		if ( ifFinished() )
			return;

		elapse_ += dtime;
		if ( elapse_ >= interval_ )
			finishCB();
	}
}; // end of namespace ora