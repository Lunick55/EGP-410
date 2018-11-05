#pragma once
#include <Trackable.h>
#include "Steering.h"
#include "ArriveSteering.h"
#include "FaceSteering.h"
class ArriveAndFaceSteering : public Steering
{
public:
	ArriveAndFaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);
	float MapToRange(float rotation);
protected:
	virtual Steering* getSteering();
	ArriveSteering mArriveSteering;
	FaceSteering mFaceSteering;
};