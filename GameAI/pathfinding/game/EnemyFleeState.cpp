#include "EnemyFleeState.h"
#include "GridPathfinder.h"
#include "UnitManager.h"
#include "GridGraph.h"
#include "GameApp.h"
#include "Unit.h"
#include "EnemySteering.h"
#include "SteeringComponent.h"
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
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);



	//check if player is within a certain radius, if it is, delete the enemy

	if (!pGame->getCanDestroyEnemies())
	{
		//transition back to wander
	}
	return nullptr;
}
