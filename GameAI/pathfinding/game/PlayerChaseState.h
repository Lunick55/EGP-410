#pragma once

#include "StateMachine.h"
#include "Vector2D.h"
#include <vector>
#include <utility>
#include <iostream>
#include "Path.h"

class PlayerChaseState : public StateMachineState
{
public:
	PlayerChaseState(const SM_idType& id) :StateMachineState(id) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	void setId(int myId) { mID = myId; };
	inline void stopRun() { mContinueRun = false; };

private:
	bool mContinueRun;

	int mPlayerXDist;
	int mPlayerYDist;
	Vector2D mPlayerXDir;
	Vector2D mPlayerYDir;
	int timer;
	Path* mpPath;

	int mID, mPathIndex;

	std::pair<Vector2D, Vector2D> mPlayerDir;

	int oldIndex, newIndex;
};
