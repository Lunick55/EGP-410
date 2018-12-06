#pragma once

#include "StateMachine.h"

class EnemyFleeState : public StateMachineState
{
public:
	EnemyFleeState(const SM_idType& id) :StateMachineState(id) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	inline void stopRun() { mContinueRun = false; };

private:
	bool mContinueRun;

};