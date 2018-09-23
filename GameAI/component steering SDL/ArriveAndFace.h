#pragma once

#include <Trackable.h>
#include "Steering.h"
#include "ArriveSteering.h"
#include "FaceSteering.h"

class ArriveAndFace : public Steering
{
public:
	ArriveAndFace(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);

protected:
	virtual Steering* getSteering();
	ArriveSteering mArriveSteering;
	FaceSteering mFaceSteering;
};