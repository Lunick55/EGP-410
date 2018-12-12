#include "PlayerWanderState.h"
#include "GridPathfinder.h"
#include "UnitManager.h"
#include "GridGraph.h"
#include "GameApp.h"
#include "Unit.h"
#include "PacSteering.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "SteeringComponent.h"

void PlayerWanderState::onEntrance()
{
	cout << "\nEntering Player Wander State id:" << mID << endl;
	mPlayerXDist = 0;
	mPlayerYDist = -32;
	mPlayerXDir = Vector2D(0, 0);
	mPlayerYDir = Vector2D(1, 0);
	//mID = 1;
	timer = 0;
	mPlayerDir = make_pair(Vector2D(0, 0), Vector2D(1, 0));
}

void PlayerWanderState::onExit()
{
	cout << "\nExiting Player Wander State id:" << mID << endl;
}

StateTransition * PlayerWanderState::update()
{
	srand(time(NULL));
	/*set enemy distances*/
	timer++;
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	GridPathfinder* pPathfinder = pGame->getPathfinder();
	int i = mID;

	GridGraph* pGridGraph = pGame->getGridGraph();
	Grid* pGrid = pGame->getGrid();
	//get the from and to index from the grid
	Vector2D playerPosCenter = (pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition() + Vector2D(16, 16));
	//float enemyX = pGame->getUnitManager()->getUnit(i)->getPositionComponent()->getPosition().getX() + 16;
	//float enemyY = pGame->getUnitManager()->getUnit(i)->getPositionComponent()->getPosition().getY() + 16;

	vector<int> adjacentIndices;
	adjacentIndices = pGrid->getAdjacentIndices(pGrid->getSquareIndexFromPixelXY((int)playerPosCenter.getX(), (int)playerPosCenter.getY()));

	int fromIndex = pGrid->getSquareIndexFromPixelXY((int)playerPosCenter.getX(), (int)playerPosCenter.getY());

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

		mPlayerXDist = directionPos.getX() - fromPos.getX();
		mPlayerYDist = directionPos.getY() - fromPos.getY();

		int x1, x2, y1, y2;

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
	}
	else
	{
		oldIndex = fromIndex;
	}

	int toIndex = pGrid->getSquareIndexFromPixelXY((int)playerPosCenter.getX() + mPlayerXDist, (int)playerPosCenter.getY() + mPlayerYDist);

	//Look for any intersections or walls in a straight line
	for (int i = 1; pGrid->getValueAtIndex(toIndex) != INTERSECTION_VALUE && pGrid->getValueAtIndex(toIndex) != BLOCKING_VALUE; i++)
	{
		toIndex = pGrid->getSquareIndexFromPixelXY((int)playerPosCenter.getX() + mPlayerXDist * i, (int)playerPosCenter.getY() + mPlayerYDist * i);
	}

	//If we're going into a wall, stop where you are
	if (pGrid->getValueAtIndex(toIndex) == BLOCKING_VALUE)
	{
		toIndex = pGrid->getSquareIndexFromPixelXY((int)playerPosCenter.getX(), (int)playerPosCenter.getY());
	}

	Node* pFromNode = pGridGraph->getNode(fromIndex);
	Node* pToNode = pGridGraph->getNode(toIndex);

	PacSteering* pPacSteer = dynamic_cast<PacSteering*>(pGame->getUnitManager()->getPlayerUnit()->getSteeringComponent()->getSteeringBehavior());
	pPacSteer->moveDirection(mPlayerXDir, mPlayerYDir);

	Path* newPath = pPathfinder->findPath(pFromNode, pToNode);

	pPacSteer->resetIndex();
	pPacSteer->setPath(newPath);
	//reset the index every click

	//check within radius of player and take damage if you are

	//when pacman is within a certain radius of ghost
	//needs to be at an intersection in order to change to chase

	//means the enemy is allowed to hurt the enemy




	return NULL;//no transition
}