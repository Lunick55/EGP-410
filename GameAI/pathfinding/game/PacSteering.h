#pragma once
#include <Trackable.h>
#include "Steering.h"
#include "DijkstraPathfinder.h"
#include "Path.h"
#include "Grid.h"
#include "Player.h"
#include "SeekSteering.h"

using namespace std;

class DijkstraPathfinder;
class Path;

class PacSteering : public Steering
{
public:
	PacSteering(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, bool shouldFlee);
	virtual Steering* getSteering();

	Player* getPlayerSteering() { return &mPlayerSteering; };

	void setPath(Path* myPath);
	void resetIndex();

private:
	Path mPath;
	Player mPlayerSteering;
	int index;
};