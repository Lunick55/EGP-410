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
	mEnemyDir = make_pair(Vector2D(0, 0), Vector2D(1, 0));
}

void EnemyIdleState::onExit()
{
	cout << "\nExiting Enemy idle State id:" << mID << endl;
}

StateTransition * EnemyIdleState::update()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	GridPathfinder* pPathfinder = pGame->getPathfinder();
	int i = mID;

	GridGraph* pGridGraph = pGame->getGridGraph();
	Grid* pGrid = pGame->getGrid();
	//get the from and to index from the grid
	Vector2D enemyPosCenter = (pGame->getUnitManager()->getUnit(i)->getPositionComponent()->getPosition() + Vector2D(16, 16));

	int fromIndex = pGrid->getSquareIndexFromPixelXY((int)enemyPosCenter.getX(), (int)enemyPosCenter.getY());
	int toIndex = pGrid->getSquareIndexFromPixelXY(enemyPosCenter.getX(), enemyPosCenter.getY());
	/*if we are on a INTERSECTION and it has 2+ connections, pick one, and proceed along that direction*/

	Node* pFromNode = pGridGraph->getNode(fromIndex);
	Node* pToNode = pGridGraph->getNode(toIndex);
	//mpPath = pPathfinder->findPath(pFromNode, pToNode);

	if (pGrid->getValueAtIndex(fromIndex) == INTERSECTION_VALUE || mpPath == nullptr)
	{
		/*THIS NEEDS TO pathfind peemans location ONLY at intersections*/
	}

	EnemySteering* pEnemySteer = dynamic_cast<EnemySteering*>(pGame->getUnitManager()->getUnit(i)->getSteeringComponent()->getSteeringBehavior());

	/*find direction to next node. use that to go in*/
	/*if (mpPath->peekNode(mPathIndex) != NULL)
	{
		mEnemyXDist = pGrid->getULCornerOfSquare(mpPath->peekNode(1)->getId()).getX() - pGrid->getULCornerOfSquare(fromIndex).getX();
		mEnemyYDist = pGrid->getULCornerOfSquare(mpPath->peekNode(1)->getId()).getY() - pGrid->getULCornerOfSquare(fromIndex).getY();
	}*/


	int x1, x2, y1, y2 = 0;

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


	pEnemySteer->moveDirection(mEnemyXDir, mEnemyYDir);


	pEnemySteer->resetIndex();
	pEnemySteer->setPath(mpPath);
	//reset the index every click


	//IF PLAYER IS OUTSIDE OF RADIUS
	//when pacman is within a certain radius of ghost
	//if (abs(enemyPosCenter.getX() - pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getX()) >= 60
	//	&& abs(enemyPosCenter.getY() - pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getY()) >= 60)
	//{
	//	map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(ENEMY_WANDER_TRANSITION);
	//	if (iter != mTransitions.end())//found?
	//	{
	//		StateTransition* pTransition = iter->second;
	//		return pTransition;
	//	}
	//}

	//when pacman is within a certain radius of ghost
	//if (abs(enemyPosCenter.getX() - pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getX()) < 60
	//	&& abs(enemyPosCenter.getY() - pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getY()) < 60)
	//{
	//	map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(ENEMY_CHASE_TRANSITION);
	//	if (iter != mTransitions.end())//found?
	//	{
	//		StateTransition* pTransition = iter->second;
	//		return pTransition;
	//	}
	//}

	return NULL;//no transition
}

