#pragma once

#include <Trackable.h>
#include <DeanLibDefines.h>
#include <limits>
#include <Vector2D.h>

#include "Component.h"
#include "PositionComponent.h"
#include "Sprite.h"
#include "Steering.h"
#include "StateMachine.h"
#include "EnemyWanderState.h"
#include "EnemyChaseState.h"
#include "EnemyIdleState.h"
#include <typeinfo>

class PhysicsComponent;
class SteeringComponent;
class Sprite;
class UnitManager;

const Uint32 DEFAULT_QUEUE_CAPACITY = 8;


//class Unit : public TransactionProcessor
class Unit : public Trackable
{
	
public:
	void draw() const;
	void drawCoins() const;
	float getFacing() const;
	void update(float elapsedTime);
	void updateCoins();

	PositionComponent* getPositionComponent() const;
	PhysicsComponent* getPhysicsComponent() const;
	SteeringComponent* getSteeringComponent() const;
	float getMaxAcc() const { return mMaxAcc; };
	float getMaxSpeed() const { return mMaxSpeed; };
	float getMaxRotAcc() const { return mMaxRotAcc; };
	float getMaxRotVel() const { return mMaxRotVel; };
	void setShowTarget(bool val) { mShowTarget = val; };


	void setSteering(Steering::SteeringType type, Vector2D targetLoc = ZERO_VECTOR2D, UnitID targetUnitID = INVALID_UNIT_ID);
	void whatIsState()
	{
		if (typeid(mpWanderState) == typeid(EnemyWanderState))
		{
			std::cout << "Wander State";
		}
	}

private:
	UnitID mID;
	ComponentID mPhysicsComponentID;
	ComponentID mPositionComponentID;
	ComponentID mSteeringComponentID;
	StateMachine* mpStateMachine;
	StateMachineState* mpWanderState;
	StateMachineState* mpChaseState;
	StateMachineState* mpIdleState;
	StateTransition* pChaseTrans;
	StateTransition* pWanderTrans;
	StateTransition* pIdleTrans;


	PositionComponent* mpPositionComponent = NULL;
	Sprite mSprite;
	float mMaxAcc;
	float mMaxSpeed;
	float mMaxRotAcc;
	float mMaxRotVel;
	bool mShowTarget;

	int mHealth;
	int mDamageRadius;

	Unit(const Sprite& sprite);
	virtual ~Unit();

	Unit(Unit&);//invalidate copy constructor
	void operator=(Unit&);//invalidate assignment operator

	friend class UnitManager;
};
