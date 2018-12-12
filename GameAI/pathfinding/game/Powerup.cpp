#include "Powerup.h"
#include "Sprite.h"
#include "GridPathfinder.h"
#include "UnitManager.h"
#include "GridGraph.h"
#include "GameApp.h"
#include "Unit.h"
#include "EnemySteering.h"
#include "SteeringComponent.h"
#include "Score.h"

Powerup::Powerup(const Sprite & sprite)
	:mSprite(sprite)
{
	//mCoins = 0;
}

Powerup::~Powerup()
{

}

void Powerup::draw()
{
	Unit* pUnit = pGame->getUnitManager()->createPowerUpObject(mSprite);
}

void Powerup::update(float elapsedTime)
{
	GridPathfinder* pPathfinder = pGame->getPathfinder();
	GridGraph* pGridGraph = pGame->getGridGraph();
	Grid* pGrid = pGame->getGrid();
	int i = mID;
	int j = enemyID;

	Vector2D enemyPosCenter = (pGame->getUnitManager()->getPowerUpUnit(i)->getPositionComponent()->getPosition() + Vector2D(16, 16));
	int fromIndex = pGrid->getSquareIndexFromPixelXY((int)enemyPosCenter.getX(), (int)enemyPosCenter.getY());
	int toIndex = pGrid->getSquareIndexFromPixelXY((int)enemyPosCenter.getX(), (int)enemyPosCenter.getY());


		if (abs(enemyPosCenter.getX() - pGame->getUnitManager()->getUnit(j)->getPositionComponent()->getPosition().getX()) < 20
			&& abs(enemyPosCenter.getY() - pGame->getUnitManager()->getUnit(j)->getPositionComponent()->getPosition().getY()) < 20)
		{
			/*if (pGame->getUnitManager()->getUnit(j) != NULL)
			{*/
				pGame->getUnitManager()->addToPowerUpDelete(i);

				//this sets it to every unit in the game, not just one
				pGame->setEnemySpeed(1.6);
				increaseTime = true;
			/*}*/
			//pGame->getScore()->addToScore(1000);
		}

	

	if(increaseTime)
	{
		timer+=elapsedTime;
	}

	if (timer > 50)
	{
		increaseTime = false;
		pGame->setEnemySpeed(1.0);
		timer = 0;
	}

}

void Powerup::reset()
{
	Unit* pUnit = pGame->getUnitManager()->createCandyObject(mSprite);
}
