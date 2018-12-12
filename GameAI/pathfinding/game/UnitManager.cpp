#include <map>

#include "UnitManager.h"
#include "Unit.h"
#include "Game.h"
#include "ComponentManager.h"
#include "GraphicsSystem.h"
#include "AllMightyCandy.h"
#include "GameApp.h"
#include "Grid.h"
#include "Coins.h"

UnitID UnitManager::msNextUnitID = PLAYER_UNIT_ID + 1;

using namespace std;

UnitManager::UnitManager(Uint32 maxSize)
	:mPool(maxSize, sizeof(Unit))
{
	timer = 0;
}

Unit* UnitManager::createUnit(const Sprite& sprite, bool shouldWrap, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/, const UnitID& id)
{
	Unit* pUnit = NULL;

	Byte* ptr = mPool.allocateObject();
	if (ptr != NULL)
	{
		//create unit
		pUnit = new (ptr)Unit(sprite);//placement new

		UnitID theID = id;
		if (theID == INVALID_UNIT_ID)
		{
			theID = msNextUnitID;
			msNextUnitID++;
		}
		 
		//place in map
		mUnitMap[theID] = pUnit;

		//assign id and increment nextID counter
		pUnit->mID = theID;


		pUnit->mHealth = 10;

		//create some components
		GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
		ComponentManager* pComponentManager = pGame->getComponentManager();
		ComponentID id = pComponentManager->allocatePositionComponent(posData, shouldWrap);
		pUnit->mPositionComponentID = id;
		pUnit->mpPositionComponent = pComponentManager->getPositionComponent(id);
		pUnit->mPhysicsComponentID = pComponentManager->allocatePhysicsComponent(pUnit->mPositionComponentID, physicsData);
		pUnit->mSteeringComponentID = pComponentManager->allocateSteeringComponent(pUnit->mPhysicsComponentID);

		//set max's
		pUnit->mMaxSpeed = MAX_SPEED;
		pUnit->mMaxAcc = MAX_ACC;
		pUnit->mMaxRotAcc = MAX_ROT_ACC;
		pUnit->mMaxRotVel = MAX_ROT_VEL;

	}

	return pUnit;
}


Unit* UnitManager::createPlayerUnit(const Sprite& sprite, bool shouldWrap /*= true*/, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/)
{
	return createUnit(sprite, shouldWrap, posData, physicsData, PLAYER_UNIT_ID);
}

Unit * UnitManager::createCoinUnit(const Sprite & sprite, bool shouldWrap, const PositionData & posData, const PhysicsData & physicsData, const UnitID & id)
{
	Unit* pUnit = NULL;

	Byte* ptr = mPool.allocateObject();
	if (ptr != NULL)
	{
		//create unit
		pUnit = new (ptr)Unit(sprite);//placement new

		UnitID theID = id;
		if (theID == INVALID_UNIT_ID)
		{
			theID = msNextUnitID;
			msNextUnitID++;
		}

		//place in map
		mCoinUnitMap[theID] = pUnit;

		//assign id and increment nextID counter
		pUnit->mID = theID;

		//create some components
		GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
		ComponentManager* pComponentManager = pGame->getComponentManager();
		ComponentID id = pComponentManager->allocatePositionComponent(posData, shouldWrap);
		pUnit->mPositionComponentID = id;
		pUnit->mpPositionComponent = pComponentManager->getPositionComponent(id);

	}

	return pUnit;
}


Unit* UnitManager::createRandomUnit(const Sprite& sprite)
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Grid* pGrid = pGame->getGrid();
	int posX = rand() % gpGame->getGraphicsSystem()->getWidth();
	int posY = rand() % gpGame->getGraphicsSystem()->getHeight();
	int velX = rand() % 50 - 25;
	int velY = rand() % 40 - 20;

	while (pGrid->getValueAtIndex(pGrid->getSquareIndexFromPixelXY(posX, posY)) == BLOCKING_VALUE)
	{
		posX = rand() % gpGame->getGraphicsSystem()->getWidth();
		posY = rand() % gpGame->getGraphicsSystem()->getHeight();
	}

	Unit* pUnit = createUnit(sprite, true, PositionData(Vector2D(posX, posY), 0)/*, PhysicsData(Vector2D(velX, velY), Vector2D(0.1f, 0.1f), 0.1f, 0.05f)*/);
	if (pUnit != NULL)
	{
		pUnit->setSteering(Steering::FOLLOW_PATH, Vector2D(pUnit->getPositionComponent()->getPosition().getX(), pUnit->getPositionComponent()->getPosition().getY()));//gpGame->getGraphicsSystem()->getWidth() / 2, gpGame->getGraphicsSystem()->getHeight() / 2));
		int temp = rand() % 360;
		temp = temp * (PI / 180);
		pUnit->getPositionComponent()->setFacing(temp);
	}
	return pUnit;
}

Unit * UnitManager::createRandomObject(const Sprite & sprite)
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Grid* pGrid = pGame->getGrid();
	int posX = rand() % pGrid->getGridWidth() * PIXEL_SIZE;
	int posY = rand() % pGrid->getGridHeight() * PIXEL_SIZE;
	int velX = rand() % 50 - 25;
	int velY = rand() % 40 - 20;

	while (pGrid->getValueAtPixelXY(posX, posY) != CLEAR_VALUE)
	{
		posX = rand() % pGrid->getGridWidth() * PIXEL_SIZE;
		posY = rand() % pGrid->getGridHeight() * PIXEL_SIZE;
	}

	Unit* pUnit = createUnit(sprite, true, PositionData(Vector2D(posX, posY), 0)/*, PhysicsData(Vector2D(velX, velY), Vector2D(0.1f, 0.1f), 0.1f, 0.05f)*/);
	return pUnit;
}

void UnitManager::checkIfPlayerDead()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	if (getPlayerUnit()->getHealth() < 0)
	{
		cout << "I am dead boi" << endl;
		pGame->markForExiting();
	}

}

Unit * UnitManager::createCoinObject(const Sprite & sprite)
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Grid* pGrid = pGame->getGrid();
	int posX = rand() % pGrid->getGridWidth() * PIXEL_SIZE;
	int posY = rand() % pGrid->getGridHeight() * PIXEL_SIZE;
	int velX = rand() % 50 - 25;
	int velY = rand() % 40 - 20;

	while (pGrid->getValueAtPixelXY(posX, posY) != CLEAR_VALUE)
	{
		posX = rand() % pGrid->getGridWidth() * PIXEL_SIZE;
		posY = rand() % pGrid->getGridHeight() * PIXEL_SIZE;
	}

	Unit* pUnit = createCoinUnit(sprite, true, PositionData(Vector2D(posX, posY), 0)/*, PhysicsData(Vector2D(velX, velY), Vector2D(0.1f, 0.1f), 0.1f, 0.05f)*/);

	return pUnit;
}
 
bool UnitManager::checkWall(int posX, int posY)
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Grid* pGrid = pGame->getGrid();
	if (pGrid->getValueAtPixelXY(posX, posY) == BLOCKING_VALUE)
	{
		return true;
	}

	return false;

}
void UnitManager::updatePacman(const Sprite & sprite, int posX, int posY)
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Grid* pGrid = pGame->getGrid();
	Unit* pUnit = createUnit(sprite, true, PositionData(Vector2D(posX, posY), 0), ZERO_PHYSICS_DATA, PLAYER_UNIT_ID);
}

void UnitManager::respawnEnemy()
{
	
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	if (needsRespawn)
	{
		
		needsRespawn = false;
		Unit* pUnit = pGame->getUnitManager()->createUnit(*pGame->getEnemySprite().at(spriteID) , true, PositionData(Vector2D(224, 130), 0));
		pUnit->setSteering(Steering::ENEMY_STEER, Vector2D(224, 130));
		spriteID++;
		if (spriteID > 4)
		{
			spriteID = 0;
		}
	}
}

Unit* UnitManager::getUnit(const UnitID& id) const
{
	auto it = mUnitMap.find(id);
	if (it != mUnitMap.end())//found?
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

Unit * UnitManager::getCoinUnit(const UnitID & id) const
{
	auto it = mCoinUnitMap.find(id);
	if (it != mCoinUnitMap.end())//found?
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void UnitManager::deleteUnit(const UnitID& id)
{
	auto it = mUnitMap.find(id);
	if (it != mUnitMap.end())//found?
	{
		Unit* pUnit = it->second;//hold for later

								 //remove from map
		mUnitMap.erase(it);

		//remove components
		GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
		ComponentManager* pComponentManager = pGame->getComponentManager();
		pComponentManager->deallocatePhysicsComponent(pUnit->mPhysicsComponentID);
		pComponentManager->deallocatePositionComponent(pUnit->mPositionComponentID);
		pComponentManager->deallocateSteeringComponent(pUnit->mSteeringComponentID);

		//call destructor
		pUnit->~Unit();

		//free the object in the pool
		mPool.freeObject((Byte*)pUnit);
	}
}

void UnitManager::deleteCoinUnit(const UnitID & id)
{
	auto it = mCoinUnitMap.find(id);
	if (it != mCoinUnitMap.end())//found?
	{
		Unit* pUnit = it->second;//hold for later

								 //remove from map
		mCoinUnitMap.erase(it);

		//remove components
		GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
		ComponentManager* pComponentManager = pGame->getComponentManager();
		pComponentManager->deallocatePositionComponent(pUnit->mPositionComponentID);

		//call destructor
		pUnit->~Unit();

		//free the object in the pool
		mPool.freeObject((Byte*)pUnit);
	}
}

Unit * UnitManager::createCandyUnit(const Sprite & sprite, bool shouldWrap, const PositionData & posData, const PhysicsData & physicsData, const UnitID & id)
{
	Unit* pUnit = NULL;

	Byte* ptr = mPool.allocateObject();
	if (ptr != NULL)
	{
		//create unit
		pUnit = new (ptr)Unit(sprite);//placement new

		UnitID theID = id;
		if (theID == INVALID_UNIT_ID)
		{
			theID = msNextUnitID;
			msNextUnitID++;
		}

		//place in map
		mCandyUnitMap[theID] = pUnit;

		//assign id and increment nextID counter
		pUnit->mID = theID;

		//create some components
		GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
		ComponentManager* pComponentManager = pGame->getComponentManager();
		ComponentID id = pComponentManager->allocatePositionComponent(posData, shouldWrap);
		pUnit->mPositionComponentID = id;
		pUnit->mpPositionComponent = pComponentManager->getPositionComponent(id);

	}

	return pUnit;
}

Unit * UnitManager::createCandyObject(const Sprite & sprite)
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Grid* pGrid = pGame->getGrid();
	int posX = rand() % pGrid->getGridWidth() * PIXEL_SIZE;
	int posY = rand() % pGrid->getGridHeight() * PIXEL_SIZE;
	int velX = rand() % 50 - 25;
	int velY = rand() % 40 - 20;

	while (pGrid->getValueAtPixelXY(posX, posY) != CLEAR_VALUE)
	{
		posX = rand() % pGrid->getGridWidth() * PIXEL_SIZE;
		posY = rand() % pGrid->getGridHeight() * PIXEL_SIZE;
	}

	Unit* pUnit = createCandyUnit(sprite, true, PositionData(Vector2D(posX, posY), 0)/*, PhysicsData(Vector2D(velX, velY), Vector2D(0.1f, 0.1f), 0.1f, 0.05f)*/);

	return pUnit;
}

Unit * UnitManager::getCandyUnit(const UnitID & id) const
{
	auto it = mCandyUnitMap.find(id);
	if (it != mCandyUnitMap.end())//found?
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void UnitManager::deleteCandyUnit(const UnitID & id)
{
	auto it = mCandyUnitMap.find(id);
	if (it != mCandyUnitMap.end())//found?
	{
		Unit* pUnit = it->second;//hold for later

								 //remove from map
		mCandyUnitMap.erase(it);

		//remove components
		GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
		ComponentManager* pComponentManager = pGame->getComponentManager();
		pComponentManager->deallocatePositionComponent(pUnit->mPositionComponentID);

		//call destructor
		pUnit->~Unit();

		//free the object in the pool
		mPool.freeObject((Byte*)pUnit);
	}
}

void UnitManager::resetCandyUnit(float elapsedTime)
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	if (pGame->getCanDestroyEnemies())
	{
		canAdd = true;
	}
	if (canAdd)
	{
		timer += elapsedTime;
	}
	if (timer > respawnTime)
	{
		pGame->getCandy()->reset();
		//pGame->setCanDestroyEnemies(false);
		timer = 0;
		canAdd = false;
	}
}


void UnitManager::deleteRandomUnit()
{
	if (mUnitMap.size() >= 1)
	{
		Uint32 target = rand() % mUnitMap.size();
		if (target == 0)//don't allow the 0th element to be deleted as it is the player unit
		{
			target = 1;
		}
		Uint32 cnt = 0;
		for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it, cnt++)
		{
			if (cnt == target)
			{
				deleteUnit(it->first);
				break;
			}
		}
	}
}

void UnitManager::drawAll() const
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->draw();
	}
	for (auto it = mCoinUnitMap.begin(); it != mCoinUnitMap.end(); ++it)
	{
		it->second->drawCoins();
	}
	for (auto it = mCandyUnitMap.begin(); it != mCandyUnitMap.end(); ++it)
	{
		it->second->drawCandy();
	}
	for (auto it = mPowerUpUnitMap.begin(); it != mPowerUpUnitMap.end(); ++it)
	{
		it->second->drawPowerUp();
	}
}

void UnitManager::updateAll(float elapsedTime)
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->update(elapsedTime);
	}
	for (auto it = mCoinUnitMap.begin(); it != mCoinUnitMap.end(); ++it)
	{
		it->second->updateCoins();
	}
	for (auto it = mCandyUnitMap.begin(); it != mCandyUnitMap.end(); ++it)
	{
		it->second->updateCandy();
	}
	for (auto it = mPowerUpUnitMap.begin(); it != mPowerUpUnitMap.end(); ++it)
	{
		it->second->updatePowerUp(elapsedTime);
	}
	for (auto& it : toBeDeleted)
	{
		deleteCoinUnit(it);
		pGame->getCoins()->addCoin();
	}
	toBeDeleted.clear();

	for (auto& it : candyToBeDeleted)
	{
		deleteCandyUnit(it);
		//pGame->getCoins()->addCoin();
	}
	candyToBeDeleted.clear();

	for (auto& it : powerUpToBeDeleted)
	{
		deletePowerUpUnit(it);
		//pGame->getCoins()->addCoin();
	}
	powerUpToBeDeleted.clear();

	for (auto& it : enemyToBeDeleted)
	{
		deleteUnit(it);
		needsRespawn = true;
		//pGame->getCoins()->addCoin();
	}
	enemyToBeDeleted.clear();
	respawnEnemy();
	resetCandyUnit(elapsedTime);
	checkIfPlayerDead();
}
void UnitManager::addToDelete(UnitID myID)
{
	toBeDeleted.push_back(myID);
}

void UnitManager::addToCandyDelete(UnitID myID)
{
	candyToBeDeleted.push_back(myID);
}

void UnitManager::addToPowerUpDelete(UnitID myID)
{
	powerUpToBeDeleted.push_back(myID);
}

void UnitManager::addToEnemyDelete(UnitID myID)
{
	enemyToBeDeleted.push_back(myID);
}

Unit * UnitManager::createPowerUpUnit(const Sprite & sprite, bool shouldWrap, const PositionData & posData, const PhysicsData & physicsData, const UnitID & id)
{
	Unit* pUnit = NULL;

	Byte* ptr = mPool.allocateObject();
	if (ptr != NULL)
	{
		//create unit
		pUnit = new (ptr)Unit(sprite);//placement new

		UnitID theID = id;
		if (theID == INVALID_UNIT_ID)
		{
			theID = msNextUnitID;
			msNextUnitID++;
		}

		//place in map
		mPowerUpUnitMap[theID] = pUnit;

		//assign id and increment nextID counter
		pUnit->mID = theID;

		//create some components
		GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
		ComponentManager* pComponentManager = pGame->getComponentManager();
		ComponentID id = pComponentManager->allocatePositionComponent(posData, shouldWrap);
		pUnit->mPositionComponentID = id;
		pUnit->mpPositionComponent = pComponentManager->getPositionComponent(id);

	}

	return pUnit;
}

Unit * UnitManager::createPowerUpObject(const Sprite & sprite)
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Grid* pGrid = pGame->getGrid();
	int posX = rand() % pGrid->getGridWidth() * PIXEL_SIZE;
	int posY = rand() % pGrid->getGridHeight() * PIXEL_SIZE;
	int velX = rand() % 50 - 25;
	int velY = rand() % 40 - 20;

	while (pGrid->getValueAtPixelXY(posX, posY) != CLEAR_VALUE)
	{
		posX = rand() % pGrid->getGridWidth() * PIXEL_SIZE;
		posY = rand() % pGrid->getGridHeight() * PIXEL_SIZE;
	}

	Unit* pUnit = createPowerUpUnit(sprite, true, PositionData(Vector2D(posX, posY), 0)/*, PhysicsData(Vector2D(velX, velY), Vector2D(0.1f, 0.1f), 0.1f, 0.05f)*/);

	return pUnit;
}

Unit * UnitManager::getPowerUpUnit(const UnitID & id) const
{
	auto it = mPowerUpUnitMap.find(id);
	if (it != mPowerUpUnitMap.end())//found?
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void UnitManager::deletePowerUpUnit(const UnitID & id)
{
	auto it = mPowerUpUnitMap.find(id);
	if (it != mPowerUpUnitMap.end())//found?
	{
		Unit* pUnit = it->second;//hold for later

								 //remove from map
		mPowerUpUnitMap.erase(it);

		//remove components
		GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
		ComponentManager* pComponentManager = pGame->getComponentManager();
		pComponentManager->deallocatePositionComponent(pUnit->mPositionComponentID);

		//call destructor
		pUnit->~Unit();

		//free the object in the pool
		mPool.freeObject((Byte*)pUnit);
	}
}
