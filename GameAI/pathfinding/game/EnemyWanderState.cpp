#include "EnemyWanderState.h"
#include "GridPathfinder.h"
#include "UnitManager.h"
#include "GridGraph.h"
#include "GameApp.h"
#include "Unit.h"
#include "EnemySteering.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "SteeringComponent.h"

void EnemyWanderState::onEntrance()
{
	cout << "\nEntering Enemy Wander State id:" << mID << endl;
	mEnemyXDist = 0;
	mEnemyYDist = -32;
	mEnemyXDir = Vector2D(0, 0);
	mEnemyYDir = Vector2D(1, 0);
	timer = 0;
	mEnemyDir = make_pair(Vector2D(0, 0), Vector2D(1, 0));
}

void EnemyWanderState::onExit()
{
	cout << "\nExiting Enemy Wander State id:" << mID << endl;
}

StateTransition * EnemyWanderState::update()
{
	srand(time(NULL)+mID);
	/*set enemy distances*/
	timer++;
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	GridPathfinder* pPathfinder = pGame->getPathfinder();
	int i = mID;

	GridGraph* pGridGraph = pGame->getGridGraph();
	Grid* pGrid = pGame->getGrid();
	//get the from and to index from the grid
	Vector2D enemyPosCenter = (pGame->getUnitManager()->getUnit(i)->getPositionComponent()->getPosition() + Vector2D(16, 16));

	vector<int> adjacentIndices;
	adjacentIndices = pGrid->getAdjacentIndices(pGrid->getSquareIndexFromPixelXY((int)enemyPosCenter.getX(), (int)enemyPosCenter.getY()));

	int fromIndex = pGrid->getSquareIndexFromPixelXY((int)enemyPosCenter.getX(), (int)enemyPosCenter.getY());

	/*if we are on a INTERSECTION and it has 2+ connections, pick one, and proceed along that direction*/
	if (pGrid->getValueAtIndex(fromIndex) == INTERSECTION_VALUE)
	{
		int directionIndex = rand() % adjacentIndices.size() / 2;
		directionIndex *= 2;

		while (pGrid->getValueAtIndex(adjacentIndices[directionIndex]) == BLOCKING_VALUE || adjacentIndices[directionIndex] == oldIndex)
		{
			directionIndex = rand() % adjacentIndices.size() / 2;
			directionIndex *= 2;
		}

		Vector2D directionPos = pGrid->getULCornerOfSquare(adjacentIndices[directionIndex]) + Vector2D(16, 16);
		Vector2D fromPos = pGrid->getULCornerOfSquare(fromIndex) + Vector2D(16, 16);

		mEnemyXDist = directionPos.getX() - fromPos.getX();
		mEnemyYDist = directionPos.getY() - fromPos.getY();

		int x1, x2, y1, y2;

		//make sure enemy is going in the right direction
		x1 = mEnemyXDist / -32;
		x2 = mEnemyXDist / 32;
		y1 = mEnemyYDist / -32;
		y2 = mEnemyYDist / 32;

		if (x1 < 0)
		{
			x1 = 0;
		}
		if (x2 < 0)
		{
			x2 = 0;
		}
		if (y1 < 0)
		{
			y1 = 0;
		}
		if (y2 < 0)
		{
			y2 = 0;
		}

		mEnemyDir = make_pair(Vector2D(x1, x2), Vector2D(y1, y2));
		mEnemyXDir = mEnemyDir.first;
		mEnemyYDir = mEnemyDir.second;
	}
	else
	{
		oldIndex = fromIndex;
	}

	int toIndex = pGrid->getSquareIndexFromPixelXY((int)enemyPosCenter.getX() + mEnemyXDist, (int)enemyPosCenter.getY() + mEnemyYDist);

	//Look for any intersections or walls in a straight line
	for (int i = 1; pGrid->getValueAtIndex(toIndex) != INTERSECTION_VALUE && pGrid->getValueAtIndex(toIndex) != BLOCKING_VALUE; i++)
	{
		toIndex = pGrid->getSquareIndexFromPixelXY((int)enemyPosCenter.getX() + mEnemyXDist * i, (int)enemyPosCenter.getY() + mEnemyYDist * i);
	}

	//If we're going into a wall, stop where you are
	if (pGrid->getValueAtIndex(toIndex) == BLOCKING_VALUE)
	{
		toIndex = pGrid->getSquareIndexFromPixelXY((int)enemyPosCenter.getX(), (int)enemyPosCenter.getY());
	}

	Node* pFromNode = pGridGraph->getNode(fromIndex);
	Node* pToNode = pGridGraph->getNode(toIndex);

	EnemySteering* pEnemySteer = dynamic_cast<EnemySteering*>(pGame->getUnitManager()->getUnit(i)->getSteeringComponent()->getSteeringBehavior());
	pEnemySteer->moveDirection(mEnemyXDir, mEnemyYDir);

	Path* newPath = pPathfinder->findPath(pFromNode, pToNode);

	pEnemySteer->resetIndex();
	pEnemySteer->setPath(newPath);
	//reset the index every click

	//check within radius of player and take damage if you are
	if (abs(enemyPosCenter.getX() - pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getX()) < pGame->getDamageRadiusEnemy()
		&& abs(enemyPosCenter.getY() - pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getY()) < pGame->getDamageRadiusEnemy())
	{
		if (timer > 20)
		{
			pGame->getUnitManager()->getPlayerUnit()->subtractHealth(1);
			timer = 0;
			cout << pGame->getUnitManager()->getPlayerUnit()->getHealth() << endl;
		}

	}
	//when pacman is within a certain radius of ghost
	//needs to be at an intersection in order to change to chase
	if (pGrid->getValueAtIndex(fromIndex) == INTERSECTION_VALUE)
	{
		if (abs(enemyPosCenter.getX() - pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getX()) < pGame->getTransitionRadiusEnemy()
			&& abs(enemyPosCenter.getY() - pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getY()) < pGame->getTransitionRadiusEnemy())
		{
			map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(ENEMY_CHASE_TRANSITION);
			if (iter != mTransitions.end())//found?
			{
				StateTransition* pTransition = iter->second;
				return pTransition;
			}
		}
	}
	//means the enemy is allowed to hurt the enemy
	if (pGame->getCanDestroyEnemies())
	{
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(ENEMY_FLEE_TRANSITION);
		if (iter != mTransitions.end())//found?
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}

	//if the player is in the spawn area, switch to idle transition
	if (pGrid->getValueAtIndex(fromIndex) == SPAWNING_VALUE && timer > 60)
	{
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(ENEMY_IDLE_TRANSTION);
		if (iter != mTransitions.end())//found?
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}

		

	return NULL;//no transition
}
