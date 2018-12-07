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

	//Player* getPlayerSteering() { return &mPlayerSteering; };
	void PacSteering::moveDirection(Vector2D directionX, Vector2D directionY);
	void setPath(Path* myPath);
	void resetIndex();
	void checkDirection(Unit* owner);

	void centerToNode();

private:
	Path mPath;
	//Player mPlayerSteering;
	bool isUnitDown, isUnitRight, isUnitLeft, isUnitUp;
	int mSpeedX, mSpeedY;
	int mIndex;

	string oldDir, newDir; 

	int mSquareIndexShifted;	//the index of the node a few pixels past the sprites edge
	int mSquareIndexAtCenter;	//the index of the node at the center of the sprite
	Vector2D mPositionCentered;//the pixel coord at the center of the sprite
};



