#pragma once
#include <Trackable.h>
#include "Steering.h"
#include "WanderSteering.h"
#include "SeekSteering.h"
#include "FaceSteering.h"

class WanderChaseSteering : public Steering
{
public:
	WanderChaseSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);

protected:
	virtual Steering* getSteering();

	float mSeekRadius = 200;

	WanderSteering mWanderSteering;
	SeekSteering mSeekSteering;
	FaceSteering mFaceSteering;
	
};