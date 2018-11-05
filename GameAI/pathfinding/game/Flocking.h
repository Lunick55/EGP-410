#pragma once
#include <Trackable.h>
#include "Steering.h"
#include "GroupAlignment.h"
#include "Seperation.h"
#include "Cohesion.h"
#include "WanderSteering.h"
#include "FaceSteering.h"

class Flocking : public Steering
{
public:
	Flocking(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, bool shouldFlee);
	virtual Steering* getSteering();
	void updateBoidWeight();

private:
	GroupAlignment mAlignment;
	Cohesion mCohesion;
	Seperation mSeperation;
	WanderSteering mWander;

	float mGroupAlignmentWeight;
	float mCohesionWeight;
	float mSeperationWeight;
	float mWanderWeight;

	int mRadius = 100;
	int mRotation = rand() % 360;
	int mOffset = 100;
	float degrees = -90.0f * 3.14f / 180.0f;
	Vector2D mDirection;
	Vector2D mTargetDirection;
};