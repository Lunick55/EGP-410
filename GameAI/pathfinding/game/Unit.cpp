#include "Unit.h"
#include <assert.h>

#include "Game.h"
#include "GraphicsSystem.h"
#include "Component.h"
#include "PositionComponent.h"
#include "PhysicsComponent.h"
#include "SteeringComponent.h"
#include "ComponentManager.h"
#include "SpriteManager.h"
#include "GameApp.h"
#include "UnitManager.h"
#include "Coins.h"



Unit::Unit(const Sprite& sprite)
	:mSprite(sprite)
	, mPositionComponentID(INVALID_COMPONENT_ID)
	, mPhysicsComponentID(INVALID_COMPONENT_ID)
	, mSteeringComponentID(INVALID_COMPONENT_ID)
	, mShowTarget(true)
{
	mpStateMachine = new StateMachine();
	

	//create the states
	mpIdleState = new EnemyIdleState(0);
	mpWanderState = new EnemyWanderState(1);
	mpChaseState = new EnemyChaseState(2);
	//set id
	pChaseTrans = new StateTransition(ENEMY_CHASE_TRANSITION, 2);
	pWanderTrans = new StateTransition(ENEMY_WANDER_TRANSITION, 1);
	pIdleTrans = new StateTransition(ENEMY_IDLE_TRANSTION, 0);

	//add the transitions to the states
	mpIdleState->addTransition(pWanderTrans);
	mpWanderState->addTransition(pChaseTrans);
	mpChaseState->addTransition(pWanderTrans);
	//mpIdleState->addTransition(pIdleTrans);



	mpStateMachine->addState(mpWanderState);
	mpStateMachine->addState(mpChaseState);
	mpStateMachine->addState(mpIdleState);
	mpStateMachine->setInitialStateID(0);

}

Unit::~Unit()
{
	delete mpStateMachine;
	delete mpWanderState;
	delete mpChaseState;
	delete pChaseTrans;
	delete pWanderTrans;
	delete mpIdleState;
	delete pIdleTrans;
}

void Unit::draw() const
{
	PositionComponent* pPosition = getPositionComponent();
	assert(pPosition != NULL);
	const Vector2D& pos = pPosition->getPosition();
	gpGame->getGraphicsSystem()->draw(mSprite, pos.getX(), pos.getY(), pPosition->getFacing());

	if (mShowTarget)
	{
		SteeringComponent* pSteering = getSteeringComponent();
		assert(pSteering != NULL);
		const Vector2D& targetLoc = pSteering->getTargetLoc();
		if (&targetLoc != &ZERO_VECTOR2D)
		{
			Sprite* pTargetSprite = gpGame->getSpriteManager()->getSprite(TARGET_SPRITE_ID);
			assert(pTargetSprite != NULL);
			gpGame->getGraphicsSystem()->draw(*pTargetSprite, targetLoc.getX(), targetLoc.getY());
		}
	}
}

void Unit::drawCoins() const
{
	PositionComponent* pPosition = getPositionComponent();
	assert(pPosition != NULL);
	const Vector2D& pos = pPosition->getPosition();
	gpGame->getGraphicsSystem()->draw(mSprite, pos.getX(), pos.getY(), pPosition->getFacing());

	if (mShowTarget)
	{
		//SteeringComponent* pSteering = getSteeringComponent();
		//assert(pSteering != NULL);
		//const Vector2D& targetLoc = pSteering->getTargetLoc();
		//if (&targetLoc != &ZERO_VECTOR2D)
		//{
			Sprite* pTargetSprite = gpGame->getSpriteManager()->getSprite(TARGET_SPRITE_ID);
			assert(pTargetSprite != NULL);
			gpGame->getGraphicsSystem()->draw(*pTargetSprite, pTargetSprite->getSize().getX(), pTargetSprite->getSize().getX());
		//}
	}
}

float Unit::getFacing() const
{
	PositionComponent* pPosition = getPositionComponent();
	assert(pPosition != NULL);
	return pPosition->getFacing();
}

void Unit::update(float elapsedTime)
{
	//std::map<UnitID, Unit*> unitMap = gpGame->getUnitManager()->getMap();
	if (mID != 0)
	{
		EnemyIdleState* pIdle = dynamic_cast<EnemyIdleState*>(mpIdleState);
		pIdle->setId(mID);
		EnemyWanderState* pWander = dynamic_cast<EnemyWanderState*>(mpWanderState);
		pWander->setId(mID);
		EnemyChaseState* pChase = dynamic_cast<EnemyChaseState*>(mpChaseState);
		pChase->setId(mID);
		mpStateMachine->update();
	}
}

void Unit::updateCoins()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	pGame->getCoins()->setID(mID);
	if(pGame->getCoins()->getID() != NULL)
		pGame->getCoins()->update();
}

PositionComponent* Unit::getPositionComponent() const
{
	return mpPositionComponent;
}

PhysicsComponent* Unit::getPhysicsComponent() const
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	PhysicsComponent* pComponent = pGame->getComponentManager()->getPhysicsComponent(mPhysicsComponentID);
	return pComponent;
}

SteeringComponent* Unit::getSteeringComponent() const
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	SteeringComponent* pComponent = pGame->getComponentManager()->getSteeringComponent(mSteeringComponentID);
	return pComponent;
}

void Unit::setSteering(Steering::SteeringType type, Vector2D targetLoc /*= ZERO_VECTOR2D*/, UnitID targetUnitID /*= INVALID_UNIT_ID*/)
{
	SteeringComponent* pSteeringComponent = getSteeringComponent();
	if (pSteeringComponent != NULL)
	{
		pSteeringComponent->setData(SteeringData(type, targetLoc, mID, targetUnitID));
	}
}