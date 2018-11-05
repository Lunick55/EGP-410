#pragma once
#include <Trackable.h>
#include "Steering.h"

class ArriveSteering : public Steering
{
public:
	ArriveSteering(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, bool shouldFlee);
	virtual Steering* getSteering();

};