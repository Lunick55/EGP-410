#pragma once
#include <Trackable.h>
#include "Steering.h"

class Seperation : public Steering
{
public:
	Seperation(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, bool shouldFlee);
	Vector2D getSeperation();

private:
	const float mRadius = 75.0f;
	Vector2D mFinalDirection;
};
