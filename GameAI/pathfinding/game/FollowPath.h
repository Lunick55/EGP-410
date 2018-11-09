#pragma once
#include <Trackable.h>
#include "Steering.h"
#include "DijkstraPathfinder.h"
#include "Path.h"
#include "Grid.h"
#include "ArriveAndFaceSteering.h"
using namespace std;

class DijkstraPathfinder;
class Path;

class FollowPath : public Steering
{
public:
	FollowPath(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, bool shouldFlee);
	virtual Steering* getSteering();

	void setPath(Path* myPath);

private:
	Path mPath;
	ArriveAndFaceSteering mArriveAndFaceSteering;
	int index;
};