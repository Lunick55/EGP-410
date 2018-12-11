#include "AllMightyCandy.h"
#include "Sprite.h"
#include "GridPathfinder.h"
#include "UnitManager.h"
#include "GridGraph.h"
#include "GameApp.h"
#include "Unit.h"
#include "EnemySteering.h"
#include "SteeringComponent.h"
#include "Score.h"

AllMightyCandy::AllMightyCandy(const Sprite & sprite)
	:mSprite(sprite)
{
	//mCoins = 0;
	timer = 0;
}

AllMightyCandy::~AllMightyCandy()
{

}

void AllMightyCandy::draw()
{
	Unit* pUnit = pGame->getUnitManager()->createCandyObject(mSprite);
	
}

void AllMightyCandy::update()
{
	GridPathfinder* pPathfinder = pGame->getPathfinder();
	GridGraph* pGridGraph = pGame->getGridGraph();
	Grid* pGrid = pGame->getGrid();
	int i = mID;
	Vector2D enemyPosCenter = (pGame->getUnitManager()->getCandyUnit(i)->getPositionComponent()->getPosition() + Vector2D(16, 16));
	int fromIndex = pGrid->getSquareIndexFromPixelXY((int)enemyPosCenter.getX(), (int)enemyPosCenter.getY());
	int toIndex = pGrid->getSquareIndexFromPixelXY((int)enemyPosCenter.getX(), (int)enemyPosCenter.getY());


	
	if (abs(enemyPosCenter.getX() - pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getX()) < 20
		&& abs(enemyPosCenter.getY() - pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getY()) < 20)
	{
		pGame->getUnitManager()->addToCandyDelete(i);
		pGame->setCanDestroyEnemies(true);
		//pGame->getScore()->addToScore(1000);
	}


}
