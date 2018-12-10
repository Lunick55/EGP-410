#pragma once

#include "StateMachine.h"
#include "Vector2D.h"
#include "Path.h"
#include <vector>
#include <utility>
#include <iostream>

class EnemyIdleState : public StateMachineState
{
public:
	EnemyIdleState(const SM_idType& id) :StateMachineState(id) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	void setId(int myId) { mID = myId; };
	inline void stopRun() { mContinueRun = false; };

private:
	bool mContinueRun;

	int mEnemyXDist;
	int mEnemyYDist;
	Vector2D mEnemyXDir;
	Vector2D mEnemyYDir;
	Path* mpPath;
	int mID, mPathIndex;


	std::pair<Vector2D, Vector2D> mEnemyDir;

	int oldIndex, newIndex;

};