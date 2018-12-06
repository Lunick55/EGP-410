#pragma once

#include "StateMachine.h"

class EnemyAttackState : public StateMachineState
{
public:
	EnemyAttackState(const SM_idType& id) :StateMachineState(id) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	inline void stopRun() { mContinueRun = false; };

private:
	bool mContinueRun;

};