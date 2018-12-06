#include "EnemyAttackState.h"
#include <iostream>

using namespace std;

void EnemyAttackState::onEntrance()
{
	cout << "\nEntering Enemy Attack State id:" << mID << endl;
}

void EnemyAttackState::onExit()
{
	cout << "\nExiting Enemy Attack State id:" << mID << endl;
}

StateTransition * EnemyAttackState::update()
{
	return nullptr;
}
