#pragma once
#include <Trackable.h>
#include "Steering.h"
#include "SeekSteering.h"
#include "FaceSteering.h"
#include "WanderSteering.h"
class WanderAndChaseSteering : public Steering
{
public:
	WanderAndChaseSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);
	virtual Steering* getSteering();

protected:
	WanderSteering mWander;
	FaceSteering mFace;
	SeekSteering mSeek;

private:
	float mTargetOrientation;
	float mWanderOrientation;

	const float mWanderRadius = 200.0f;
};