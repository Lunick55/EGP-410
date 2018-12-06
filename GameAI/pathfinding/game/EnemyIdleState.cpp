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
	return nullptr;
}
