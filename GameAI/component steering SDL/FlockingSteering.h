#pragma once
#include <Trackable.h>
#include "Steering.h"
#include "CohesionSteering.h"
#include "SeparationSteering.h"
#include "GroupAlignSteering.h"
#include "WanderSteering.h"

class FlockingSteering : public Steering
{
public:
	FlockingSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);

	virtual Steering* getSteering();

protected:
	CohesionSteering mCohesion;
	SeparationSteering mSeparation;
	GroupAlignSteering mGroupAlign;
	WanderSteering mWander;

	float mWeightCohesion;
	float mWeightSeparation;
	float mWeightGroupAlign;
};