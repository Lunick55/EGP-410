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
#include "EnemyFleeState.h"
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
	void drawCandy() const;
	void drawPowerUp() const;
	float getFacing() const;
	void update(float elapsedTime);
	void updateCoins();
	void updateCandy();
	void updatePowerUp(float elapsedTime);

	PositionComponent* getPositionComponent() const;
	PhysicsComponent* getPhysicsComponent() const;
	SteeringComponent* getSteeringComponent() const;
	float getMaxAcc() const { return mMaxAcc; };
	float getMaxSpeed() const { return mMaxSpeed; };
	float getMaxRotAcc() const { return mMaxRotAcc; };
	float getMaxRotVel() const { return mMaxRotVel; };
	void setShowTarget(bool val) { mShowTarget = val; };
	int getHealth() { return mHealth; };
	void subtractHealth(int subtractNumber) { mHealth -= subtractNumber; };


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
	StateMachineState* mpFleeState;
	//EnemyFleeState*	mpFleeState;

	StateTransition* pChaseTrans;
	StateTransition* pWanderTrans;
	StateTransition* pIdleTrans;
	StateTransition* pFleeTrans;
	//StateTransition*
	int mHealth;
	int mDamageRadius;

	PositionComponent* mpPositionComponent = NULL;
	Sprite mSprite;
	float mMaxAcc;
	float mMaxSpeed;
	float mMaxRotAcc;
	float mMaxRotVel;
	bool mShowTarget;

	Unit(const Sprite& sprite);
	virtual ~Unit();

	Unit(Unit&);//invalidate copy constructor
	void operator=(Unit&);//invalidate assignment operator

	friend class UnitManager;
};
