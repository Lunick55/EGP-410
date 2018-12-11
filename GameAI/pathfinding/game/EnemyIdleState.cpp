#include "EnemyIdleState.h"
#include "GridPathfinder.h"
#include "UnitManager.h"
#include "GridGraph.h"
#include "GameApp.h"
#include "Unit.h"
#include "EnemySteering.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "SteeringComponent.h"

void EnemyIdleState::onEntrance()
{
	cout << "\nEntering Enemy idle State id:" << mID << endl;
	mEnemyXDist = 0;
	mEnemyYDist = -32;
	mEnemyXDir = Vector2D(0, 0);
	mEnemyYDir = Vector2D(1, 0);
	//mID = 1;
	timer = 0;
	mEnemyDir = make_pair(Vector2D(0, 0), Vector2D(1, 0));
}

void EnemyIdleState::onExit()
{
	cout << "\nExiting Enemy idle State id:" << mID << endl;
}

StateTransition * EnemyIdleState::update()
{
	timer++;
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	GridPathfinder* pPathfinder = pGame->getPathfinder();
	int i = mID;

	GridGraph* pGridGraph = pGame->getGridGraph();
	Grid* pGrid = pGame->getGrid();
	//get the from and to index from the grid
	Vector2D enemyPosCenter = (pGame->getUnitManager()->getUnit(i)->getPositionComponent()->getPosition() + Vector2D(16, 16));

	int fromIndex = pGrid->getSquareIndexFromPixelXY((int)enemyPosCenter.getX(), (int)enemyPosCenter.getY());
	int toIndex = pGrid->getSquareIndexFromPixelXY((int)enemyPosCenter.getX(), (int)enemyPosCenter.getY());
	/*if we are on a INTERSECTION and it has 2+ connections, pick one, and proceed along that direction*/



	//IF PLAYER IS OUTSIDE OF RADIUS
	//when pacman is within a certain radius of ghost
	//if on spawn point
	if (timer > 50 && pGrid->getValueAtIndex(fromIndex) == SPAWNING_VALUE)
	{
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(ENEMY_WANDER_TRANSITION);
		if (iter != mTransitions.end())//found?
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}


	return NULL;//no transition
}

