#include "Coins.h"
#include "Game.h"
#include "GameApp.h"
#include "Sprite.h"
#include "GridPathfinder.h"
#include "UnitManager.h"
#include "GridGraph.h"
#include "GameApp.h"
#include "Unit.h"
#include "EnemySteering.h"
#include "SteeringComponent.h"
#include "Score.h"

Coins::Coins(const Sprite & sprite)
	:mSprite(sprite)
{
	mCoins = 0;
}

Coins::~Coins()
{
	
}

void Coins::addCoins(int amount)
{
	mCoins += amount;
}

void Coins::draw()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	for (int i = 0; i < mCoins; i++)
	{
		Unit* pUnit = pGame->getUnitManager()->createCoinObject(mSprite);
	}
}

void Coins::update()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	GridPathfinder* pPathfinder = pGame->getPathfinder();
	GridGraph* pGridGraph = pGame->getGridGraph();
	Grid* pGrid = pGame->getGrid();
	int i = mID;
	Vector2D enemyPosCenter = (pGame->getUnitManager()->getCoinUnit(i)->getPositionComponent()->getPosition() + Vector2D(16, 16));
	int fromIndex = pGrid->getSquareIndexFromPixelXY((int)enemyPosCenter.getX(), (int)enemyPosCenter.getY());
	int toIndex = pGrid->getSquareIndexFromPixelXY((int)enemyPosCenter.getX(), (int)enemyPosCenter.getY());

	if (abs(enemyPosCenter.getX() - pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getX()) < 20
		&& abs(enemyPosCenter.getY() - pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getY()) < 20)
	{
		
 		pGame->getUnitManager()->addToDelete(i);
		pGame->getScore()->addToScore(100);
	}


}
