#pragma once
#include <Trackable.h>
#include "Steering.h"
class Cohesion : public Steering
{
public:
	Cohesion(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, bool shouldFlee);
	Vector2D getCohesion();

private:
	float mRadius = 200.0f;
	Vector2D mFinalDirection;
};