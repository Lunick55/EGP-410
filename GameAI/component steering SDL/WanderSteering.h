#include <Trackable.h>
#include "Steering.h"
#include "FaceSteering.h"

class WanderSteering : public Steering
{
public:
	WanderSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);
	Vector2D asVector(float num);

protected:
	virtual Steering* getSteering();
	FaceSteering mFaceSteering;
	float mWanderOrientation;

};