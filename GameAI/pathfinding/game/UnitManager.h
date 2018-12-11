#pragma once

#include <Trackable.h>
#include <DeanLibDefines.h>
#include <MemoryPool.h>
#include <map>
#include "PositionComponent.h"
#include "PhysicsComponent.h"
#include "Unit.h"
#include <vector>

class Unit;
class Sprite;
class StateMachine;
struct PositionData;
struct PhysicsData;

const UnitID PLAYER_UNIT_ID = 0;
enum Direction
{
	up,
	down,
	left,
	right
};

class UnitManager : public Trackable
{
public:
	UnitManager(Uint32 maxSize);
	~UnitManager() 
	{
		for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
		{
			it->second->~Unit();
		}
		mUnitMap.clear();
		for (auto it = mCoinUnitMap.begin(); it != mCoinUnitMap.end(); ++it)
		{
			it->second->~Unit();
		}
		mCoinUnitMap.clear();
		for (auto it = mCandyUnitMap.begin(); it != mCandyUnitMap.end(); ++it)
		{
			it->second->~Unit();
		}
		mCandyUnitMap.clear();
	};

	Unit* createUnit(const Sprite& sprite, bool shouldWrap = true, const PositionData& posData = ZERO_POSITION_DATA, const PhysicsData& physicsData = ZERO_PHYSICS_DATA, const UnitID& id = INVALID_UNIT_ID);
	Unit* createPlayerUnit(const Sprite& sprite, bool shouldWrap = true, const PositionData& posData = ZERO_POSITION_DATA, const PhysicsData& physicsData = ZERO_PHYSICS_DATA);
	Unit* createRandomUnit(const Sprite& sprite);
	Unit* createRandomObject(const Sprite& sprite);
	Unit* createPacman(const Sprite& sprite);

	bool checkWall(int posX, int posY);

	int getUnitCount() const { return mUnitMap.size(); }
	Unit* getUnit(const UnitID& id) const;
	void deleteUnit(const UnitID& id);
	void deleteRandomUnit();

	void drawAll() const;
	void updateAll(float elapsedTime);
	void updatePacman(const Sprite& sprite, int posX, int posY);
	
	Unit* createCoinUnit(const Sprite& sprite, bool shouldWrap = true, const PositionData& posData = ZERO_POSITION_DATA, const PhysicsData& physicsData = ZERO_PHYSICS_DATA, const UnitID& id = INVALID_UNIT_ID);
	Unit* createCoinObject(const Sprite& sprite);
	Unit* getCoinUnit(const UnitID& id) const;
	void deleteCoinUnit(const UnitID& id);

	Unit* createCandyUnit(const Sprite& sprite, bool shouldWrap = true, const PositionData& posData = ZERO_POSITION_DATA, const PhysicsData& physicsData = ZERO_PHYSICS_DATA, const UnitID& id = INVALID_UNIT_ID);
	Unit* createCandyObject(const Sprite& sprite);
	Unit* getCandyUnit(const UnitID& id) const;
	void deleteCandyUnit(const UnitID& id);

	Unit* getPlayerUnit() const { return getUnit(PLAYER_UNIT_ID); };
	std::map<UnitID, Unit*> getMap() { return mUnitMap; };
	std::map<UnitID, Unit*> getCoinMap() { return mCoinUnitMap; };
	void addToDelete(UnitID myID);
	void addToCandyDelete(UnitID myID);

	void updateFlockWeights();

private:
	static UnitID msNextUnitID;
	MemoryPool mPool;
	bool mShouldWander;
	const int PIXEL_SIZE = 32;
	const int HALF = 2;
	const int OFFSET = 64;
	std::map<UnitID, Unit*> mUnitMap;
	std::map<UnitID, Unit*> mCoinUnitMap;
	std::map<UnitID, Unit*> mCandyUnitMap;
	std::vector<UnitID> toBeDeleted;
	std::vector<UnitID> candyToBeDeleted;
};

