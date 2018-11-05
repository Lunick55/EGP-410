#include "Cohesion.h"
#include "Unit.h"
#include "UnitManager.h"
#include "Game.h"

Cohesion::Cohesion(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, bool shouldFlee)
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::COHESION;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Vector2D Cohesion::getCohesion()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	Vector2D tmp;
	map<UnitID, Unit*> mMap = gpGame->getUnitManager()->getMap();
	//flock count
	int threshold = 0;

	float x, y;
	map<UnitID, Unit*>::iterator unit;
	for (unit = mMap.begin(); unit != mMap.end(); unit++)
	{
		//check to make sure the unit isnt the current unit
		if (unit->second != pOwner)
		{
			Vector2D myCurrentDirection = unit->second->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();
			float distance = myCurrentDirection.getLength();
			//check if target is close and then try to get in radius of it
			if (distance < mRadius)
			{
				tmp += unit->second->getPositionComponent()->getPosition();
				threshold++;
			}

		}
	}

	if (threshold == 0)
	{
		return mFinalDirection;
	}

	//average out locations
	tmp /= threshold;
	tmp = tmp - pOwner->getPositionComponent()->getPosition();

	mFinalDirection = tmp;

	return mFinalDirection;
}
