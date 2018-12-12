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
#include "AllMightyCandy.h"
#include "Powerup.h"



Unit::Unit(const Sprite& sprite)
	:mSprite(sprite)
	, mPositionComponentID(INVALID_COMPONENT_ID)
	, mPhysicsComponentID(INVALID_COMPONENT_ID)
	, mSteeringComponentID(INVALID_COMPONENT_ID)
	, mShowTarget(false)
{
	mpStateMachine = new StateMachine();
	mOriginalSprite = &mSprite;

	mpWanderState = NULL;
	mpChaseState = NULL;
	mpIdleState = NULL;
	mpFleeState = NULL;
	mpPlayerWanderState = NULL;
	mpPlayerChaseState = NULL;

	pChaseTrans = NULL;
	pWanderTrans = NULL;
	pIdleTrans = NULL;
	pFleeTrans = NULL;
	pPlayerChaseTrans = NULL;
	pPlayerWanderTrans = NULL;
}

Unit::~Unit()
{
	delete mpStateMachine;
	delete mpWanderState;
	delete mpChaseState;
	delete mpFleeState;
	delete pChaseTrans;
	delete pWanderTrans;
	delete mpIdleState;
	delete pIdleTrans;
	delete pFleeTrans;
	delete mpPlayerWanderState;
	delete mpPlayerChaseState;
	delete pPlayerChaseTrans;
	delete pPlayerWanderTrans;
}

void Unit::draw() const
{
	PositionComponent* pPosition = getPositionComponent();
	assert(pPosition != NULL);
	const Vector2D& pos = pPosition->getPosition();
	gpGame->getGraphicsSystem()->draw(mSprite, pos.getX(), pos.getY(), pPosition->getFacing());

}

void Unit::drawCoins() const
{
	PositionComponent* pPosition = getPositionComponent();
	assert(pPosition != NULL);
	const Vector2D& pos = pPosition->getPosition();
	gpGame->getGraphicsSystem()->draw(mSprite, pos.getX(), pos.getY(), pPosition->getFacing());

	if (mShowTarget)
	{
		Sprite* pTargetSprite = gpGame->getSpriteManager()->getSprite(TARGET_SPRITE_ID);
		assert(pTargetSprite != NULL);
		gpGame->getGraphicsSystem()->draw(*pTargetSprite, pTargetSprite->getSize().getX(), pTargetSprite->getSize().getX());

	}
}

void Unit::drawCandy() const
{
	PositionComponent* pPosition = getPositionComponent();
	assert(pPosition != NULL);
	const Vector2D& pos = pPosition->getPosition();
	gpGame->getGraphicsSystem()->draw(mSprite, pos.getX(), pos.getY(), pPosition->getFacing());

	if (mShowTarget)
	{
		Sprite* pTargetSprite = gpGame->getSpriteManager()->getSprite(TARGET_SPRITE_ID);
		assert(pTargetSprite != NULL);
		gpGame->getGraphicsSystem()->draw(*pTargetSprite, pTargetSprite->getSize().getX(), pTargetSprite->getSize().getX());

	}
}

void Unit::drawPowerUp() const
{
	PositionComponent* pPosition = getPositionComponent();
	assert(pPosition != NULL);
	const Vector2D& pos = pPosition->getPosition();
	gpGame->getGraphicsSystem()->draw(mSprite, pos.getX(), pos.getY(), pPosition->getFacing());

	if (mShowTarget)
	{
		Sprite* pTargetSprite = gpGame->getSpriteManager()->getSprite(TARGET_SPRITE_ID);
		assert(pTargetSprite != NULL);
		gpGame->getGraphicsSystem()->draw(*pTargetSprite, pTargetSprite->getSize().getX(), pTargetSprite->getSize().getX());

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
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	if (mID != 0)
	{
		pGame->getPowerUp()->setEnemyID(mID);
		EnemyIdleState* pIdle = dynamic_cast<EnemyIdleState*>(mpIdleState);
		pIdle->setId(mID);
		EnemyWanderState* pWander = dynamic_cast<EnemyWanderState*>(mpWanderState);
		pWander->setId(mID);
		EnemyChaseState* pChase = dynamic_cast<EnemyChaseState*>(mpChaseState);
		pChase->setId(mID);
		EnemyFleeState* pFlee = dynamic_cast<EnemyFleeState*>(mpFleeState);
		pFlee->setId(mID);
		mpStateMachine->update();
	}
	else if (isAIControlled)
	{
		PlayerWanderState* pWander = dynamic_cast<PlayerWanderState*>(mpPlayerWanderState);
		pWander->setId(mID);
		PlayerChaseState* pChase = dynamic_cast<PlayerChaseState*>(mpPlayerChaseState);
		pChase->setId(mID);

		mpStateMachine->update();
	}
}

void Unit::alignStateMachine()
{	
		if (mID != 0)
		{
			//create the states
			mpIdleState = new EnemyIdleState(0);
			mpWanderState = new EnemyWanderState(1);
			mpChaseState = new EnemyChaseState(2);
			mpFleeState = new EnemyFleeState(3);
			//set id
			pChaseTrans = new StateTransition(ENEMY_CHASE_TRANSITION, 2);
			pWanderTrans = new StateTransition(ENEMY_WANDER_TRANSITION, 1);
			pIdleTrans = new StateTransition(ENEMY_IDLE_TRANSTION, 0);
			pFleeTrans = new StateTransition(ENEMY_FLEE_TRANSITION, 3);
	
			//add the transitions to the states
			mpIdleState->addTransition(pWanderTrans);
			mpWanderState->addTransition(pChaseTrans);
			mpWanderState->addTransition(pFleeTrans);
			mpWanderState->addTransition(pIdleTrans);
			mpChaseState->addTransition(pWanderTrans);
			mpChaseState->addTransition(pFleeTrans);
			mpChaseState->addTransition(pIdleTrans);
			mpFleeState->addTransition(pWanderTrans);
	
			mpStateMachine->addState(mpWanderState);
			mpStateMachine->addState(mpChaseState);
			mpStateMachine->addState(mpIdleState);
			mpStateMachine->addState(mpFleeState);
			mpStateMachine->setInitialStateID(0);
		}
		else
		{
			mpPlayerWanderState = new PlayerWanderState(0);
			mpPlayerChaseState = new PlayerChaseState(1);

			pPlayerWanderTrans = new StateTransition(PLAYER_WANDER_TRANSITION, 0);
			pPlayerChaseTrans = new StateTransition(PLAYER_CHASE_TRANSITION, 1);

			mpStateMachine->addState(mpPlayerWanderState);
			mpStateMachine->addState(mpPlayerChaseState);

			mpPlayerWanderState->addTransition(pPlayerChaseTrans);
			mpPlayerChaseState->addTransition(pPlayerWanderTrans);

			mpStateMachine->setInitialStateID(0);
		}

}

void Unit::updateCoins()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	pGame->getCoins()->setID(mID);
	if(pGame->getCoins()->getID() != NULL)
		pGame->getCoins()->update();
}

void Unit::updateCandy()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	pGame->getCandy()->setID(mID);
	if (pGame->getCandy()->getID() != NULL)
		pGame->getCandy()->update();
}

void Unit::updatePowerUp(float elapsedTime)
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	pGame->getPowerUp()->setID(mID);
	if (pGame->getPowerUp()->getID() != NULL)
		pGame->getPowerUp()->update(elapsedTime);
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

void Unit::swapSprites(Sprite * sprite)
{
	mSprite = *sprite;
}

void Unit::setSteering(Steering::SteeringType type, Vector2D targetLoc /*= ZERO_VECTOR2D*/, UnitID targetUnitID /*= INVALID_UNIT_ID*/)
{
	SteeringComponent* pSteeringComponent = getSteeringComponent();
	if (pSteeringComponent != NULL)
	{
		pSteeringComponent->setData(SteeringData(type, targetLoc, mID, targetUnitID));
	}
}