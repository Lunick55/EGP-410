#pragma once

#include <Trackable.h>
#include "UnitManager.h"
#include "Vector2D.h"
#include "Timer.h"

class GraphicsBuffer;


class Player : public Steering
{
public:
	Player(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, bool shouldFlee);
	void init();
	~Player();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	bool rayClear(Node * outputPath, Node * inputPath);

	void update(double dt);

	Vector2D getPosition();

	void draw();
	virtual Steering* getSteering();
private:
	int mPosX;
	int mPosY;
	double mSpeed;
	bool isUnitDown, isUnitRight, isUnitLeft, isUnitUp;
	Timer mTimer;
	int mMovement;

};