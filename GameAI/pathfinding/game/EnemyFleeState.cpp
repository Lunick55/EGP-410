#include "EnemyFleeState.h"
#include <iostream>

using namespace std;
void EnemyFleeState::onEntrance()
{
	cout << "\nEntering Enemy Flee State id:" << mID << endl;
}

void EnemyFleeState::onExit()
{
	cout << "\nExiting Enemy Flee State id:" << mID << endl;
}

StateTransition * EnemyFleeState::update()
{
	return nullptr;
}
