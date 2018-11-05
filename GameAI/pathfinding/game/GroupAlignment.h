#pragma once
#include <Trackable.h>
#include "Steering.h"
#include "FaceSteering.h"
class GroupAlignment : public Steering
{
public:
	GroupAlignment(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, bool shouldFlee);
	Vector2D getAlignment();

private:
	const float mRadius = 300.0f;
	FaceSteering mFace;
	Vector2D mFinalDirection;
};
