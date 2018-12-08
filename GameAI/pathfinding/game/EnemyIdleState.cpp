#include "EnemyIdleState.h"
#include <iostream>

using namespace std;
void EnemyIdleState::onEntrance()
{
	cout << "\nEntering Enemy idle State id:" << mID << endl;
}

void EnemyIdleState::onExit()
{
	cout << "\nExiting Enemy Flee State id:" << mID << endl;
}

StateTransition * EnemyIdleState::update()
{
	map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(ENEMY_WANDER_TRANSITION);
	if (iter != mTransitions.end())//found?
	{
		StateTransition* pTransition = iter->second;
		return pTransition;
	}

	return NULL;//no transition
}
