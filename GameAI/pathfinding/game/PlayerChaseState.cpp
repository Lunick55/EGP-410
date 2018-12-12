#include "PlayerChaseState.h"
#include "GridPathfinder.h"
#include "UnitManager.h"
#include "GridGraph.h"
#include "GameApp.h"
#include "Unit.h"
#include "PacSteering.h"
#include "SteeringComponent.h"

void PlayerChaseState::onEntrance()
{
	cout << "\nEntering Player Chase State id:" << mID << endl;
	mPlayerXDist = 0;
	mPlayerYDist = 0;
	mPlayerXDir = Vector2D(0, 0);
	mPlayerYDir = Vector2D(0, 0);
	mPathIndex = 1;
	timer = 0;
	mPlayerDir = make_pair(Vector2D(0, 0), Vector2D(0, 0));
}

void PlayerChaseState::onExit()
{
	cout << "\nExiting Player Chase State id:" << mID << endl;
}

StateTransition * PlayerChaseState::update()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	GridPathfinder* pPathfinder = pGame->getPathfinder();
	int i = mID;
	timer++;

	GridGraph* pGridGraph = pGame->getGridGraph();
	Grid* pGrid = pGame->getGrid();
	//get the from and to index from the grid
	Vector2D playerPosCenter = (pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition() + Vector2D(16, 16));

	int fromIndex = pGrid->getSquareIndexFromPixelXY((int)playerPosCenter.getX(), (int)playerPosCenter.getY());

	//Find nearest enemy
	int numEnemies = pGame->getUnitManager()->getUnitCount();

	int closestEnemyIndex = 0;

	if (numEnemies > 0)
	{ 
		closestEnemyIndex = pGame->getUnitManager()->findClosestUnit(playerPosCenter);
	}

	int toIndex = pGrid->getSquareIndexFromPixelXY(pGame->getUnitManager()->getUnit(closestEnemyIndex)->getPositionComponent()->getPosition().getX(), pGame->getUnitManager()->getUnit(closestEnemyIndex)->getPositionComponent()->getPosition().getY());
	/*if we are on a INTERSECTION and it has 2+ connections, pick one, and proceed along that direction*/

	Node* pFromNode = pGridGraph->getNode(fromIndex);
	Node* pToNode = pGridGraph->getNode(toIndex);
	mpPath = pPathfinder->findPath(pFromNode, pToNode);

	PacSteering* pPacSteer = dynamic_cast<PacSteering*>(pGame->getUnitManager()->getUnit(i)->getSteeringComponent()->getSteeringBehavior());

	/*find direction to next node. use that to go in*/
	if (mpPath->peekNode(mPathIndex) != NULL)
	{
		mPlayerXDist = pGrid->getULCornerOfSquare(mpPath->peekNode(1)->getId()).getX() - pGrid->getULCornerOfSquare(fromIndex).getX();
		mPlayerYDist = pGrid->getULCornerOfSquare(mpPath->peekNode(1)->getId()).getY() - pGrid->getULCornerOfSquare(fromIndex).getY();
	}


	int x1, x2, y1, y2 = 0;
	//make sure we are going the right direction
	x1 = mPlayerXDist / -32;
	x2 = mPlayerXDist / 32;
	y1 = mPlayerYDist / -32;
	y2 = mPlayerYDist / 32;

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

	mPlayerDir = make_pair(Vector2D(x1, x2), Vector2D(y1, y2));
	mPlayerXDir = mPlayerDir.first;
	mPlayerYDir = mPlayerDir.second;

	pPacSteer->moveDirection(mPlayerXDir, mPlayerYDir);

	pPacSteer->resetIndex();
	pPacSteer->setPath(mpPath);
	//reset the index every click

	/*go to wander if pp runs out*/
	if (pGame->getCanDestroyEnemies() == false)
	{ 
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(PLAYER_WANDER_TRANSITION);
		if (iter != mTransitions.end())//found?
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}
	return NULL;//no transition
}