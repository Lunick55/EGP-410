#include <stdio.h>
#include <assert.h>

#include <sstream>
#include <SDL.h>

#include "Game.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "Font.h"
#include "GraphicsBufferManager.h"
#include "Event.h"
#include "EventSystem.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Timer.h"
#include "ComponentManager.h"
#include "UnitManager.h"

Game* gpGame = NULL;

const int WIDTH = 1024;
const int HEIGHT = 768;
const Uint32 MAX_UNITS = 100;

Game::Game()
	:mpGraphicsSystem(NULL)
	,mpGraphicsBufferManager(NULL)
	,mpSpriteManager(NULL)
	,mpLoopTimer(NULL)
	,mpMasterTimer(NULL)
	,mpFont(NULL)
	,mShouldExit(false)
	,mBackgroundBufferID("")
	,mpComponentManager(NULL)
	,mpUnitManager(NULL)
{
}

Game::~Game()
{
	cleanup();
}

bool Game::init()
{
	mShouldExit = false;

	//create Timers
	mpLoopTimer = new Timer;
	mpMasterTimer = new Timer;

	//create and init GraphicsSystem
	mpGraphicsSystem = new GraphicsSystem();
	bool goodGraphics = mpGraphicsSystem->init( WIDTH, HEIGHT );
	if(!goodGraphics) 
	{
		fprintf(stderr, "failed to initialize GraphicsSystem object!\n");
		return false;
	}

	mpGraphicsBufferManager = new GraphicsBufferManager(mpGraphicsSystem);
	mpSpriteManager = new SpriteManager();

	mInputSystem;
	//mpMessageManager = new GameMessageManager();
	mpComponentManager = new ComponentManager(MAX_UNITS);
	mpUnitManager = new UnitManager(MAX_UNITS);

	//load buffers
	mpGraphicsBufferManager->loadBuffer(mBackgroundBufferID,"wallpaper.bmp");
	mpGraphicsBufferManager->loadBuffer(mPlayerIconBufferID,"arrow.png");
	mpGraphicsBufferManager->loadBuffer(mEnemyIconBufferID,"enemy-arrow.png");
	mpGraphicsBufferManager->loadBuffer(mTargetBufferID,"target.png");

	//load Font
	mpFont = new Font("cour.ttf", 24);
	

	//load listeners
	EventSystem::initInstance();
	EventSystem::getInstance()->addListener(ESC, this);
	EventSystem::getInstance()->addListener(D_KEY, this);
	EventSystem::getInstance()->addListener(ENTER, this);
	EventSystem::getInstance()->addListener(S_KEY, this);
	EventSystem::getInstance()->addListener(C_KEY, this);
	EventSystem::getInstance()->addListener(A_KEY, this);
	EventSystem::getInstance()->addListener(W_KEY, this);
	EventSystem::getInstance()->addListener(G_KEY, this);
	EventSystem::getInstance()->addListener(DOWN_ARROW, this);
	EventSystem::getInstance()->addListener(UP_ARROW, this);
	EventSystem::getInstance()->addListener(LEFT_ARROW, this);
	EventSystem::getInstance()->addListener(RIGHT_ARROW, this);

	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer( mBackgroundBufferID );
	if( pBackGroundBuffer != NULL )
	{
		mpSpriteManager->createAndManageSprite( BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, (float)pBackGroundBuffer->getWidth(), (float)pBackGroundBuffer->getHeight() );
	}
	GraphicsBuffer* pPlayerBuffer = mpGraphicsBufferManager->getBuffer( mPlayerIconBufferID );
	Sprite* pArrowSprite = NULL;
	if( pPlayerBuffer != NULL )
	{
		pArrowSprite = mpSpriteManager->createAndManageSprite( PLAYER_ICON_SPRITE_ID, pPlayerBuffer, 0, 0, (float)pPlayerBuffer->getWidth(), (float)pPlayerBuffer->getHeight() );
	}
	GraphicsBuffer* pAIBuffer = mpGraphicsBufferManager->getBuffer(mEnemyIconBufferID);
	Sprite* pEnemyArrow = NULL;
	if (pAIBuffer != NULL)
	{
		pEnemyArrow = mpSpriteManager->createAndManageSprite(AI_ICON_SPRITE_ID, pAIBuffer, 0, 0, (float)pAIBuffer->getWidth(), (float)pAIBuffer->getHeight());
	}

	GraphicsBuffer* pTargetBuffer = mpGraphicsBufferManager->getBuffer(mTargetBufferID);
	if (pTargetBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(TARGET_SPRITE_ID, pTargetBuffer, 0, 0, (float)pTargetBuffer->getWidth(), (float)pTargetBuffer->getHeight());
	}

	Vector2D tempLoc = (100, 100);

	loadFlockData();

	//setup units
	//Unit* pUnit = mpUnitManager->createPlayerUnit(*pArrowSprite);
	//pUnit->setShowTarget(true);
	//pUnit->setSteering(Steering::ARRIVE, ZERO_VECTOR2D);

	//create 2 enemies
	//pUnit = mpUnitManager->createUnit(*pEnemyArrow, true, PositionData(Vector2D((float)gpGame->getGraphicsSystem()->getWidth()-1, 0.0f), 0.0f));
	//pUnit->setShowTarget(true);
	//pUnit->setSteering(Steering::WANDER, tempLoc, PLAYER_UNIT_ID);
	//
	//pUnit = mpUnitManager->createUnit(*pEnemyArrow, true, PositionData(Vector2D(0.0f, (float)gpGame->getGraphicsSystem()->getHeight()-1), 0.0f));
	//pUnit->setShowTarget(true);
	//pUnit->setSteering(Steering::WANDER, ZERO_VECTOR2D, PLAYER_UNIT_ID);


	return true;
}

void Game::cleanup()
{
	//delete the timers
	delete mpLoopTimer;
	mpLoopTimer = NULL;
	delete mpMasterTimer;
	mpMasterTimer = NULL;

	delete mpFont;
	mpFont = NULL;

	//delete the graphics system
	delete mpGraphicsSystem;
	mpGraphicsSystem = NULL;

	delete mpGraphicsBufferManager;
	mpGraphicsBufferManager = NULL;
	delete mpSpriteManager;
	mpSpriteManager = NULL;
	delete mpUnitManager;
	mpUnitManager = NULL;
	delete mpComponentManager;
	mpComponentManager = NULL;
}

void Game::beginLoop()
{
	mpLoopTimer->start();
}

const float TARGET_ELAPSED_MS = LOOP_TARGET_TIME / 1000.0f;
	
void Game::processLoop()
{
	mpUnitManager->updateAll(TARGET_ELAPSED_MS);
	mpComponentManager->update(TARGET_ELAPSED_MS);
	
	//draw background
	Sprite* pBackgroundSprite = mpSpriteManager->getSprite( BACKGROUND_SPRITE_ID );
	GraphicsBuffer* pDest = mpGraphicsSystem->getBackBuffer();
	mpGraphicsSystem->draw(*pDest, *pBackgroundSprite, 0.0f, 0.0f);

	//draw units
	mpUnitManager->drawAll();

	int x = 0, y = 0;
	mInputSystem.getMousePos(x, y);

	//create mouse text
	std::stringstream mousePos;
	mousePos << x << ":" << y;

	//write text at mouse position
	mpGraphicsSystem->writeText(*mpFont, (float)x, (float)y, mousePos.str(), BLACK_COLOR);
	mpGraphicsSystem->writeText(*mpFont, (float)x, (float)y - 20, "S Weight: " + std::to_string(mWeightSeparation) + "|", BLACK_COLOR);
	mpGraphicsSystem->writeText(*mpFont, (float)x + 270, (float)y - 20, "S Radius: " + std::to_string((int)mRadiusSeparation), BLACK_COLOR);

	mpGraphicsSystem->writeText(*mpFont, (float)x, (float)y - 40, "C Weight: " + std::to_string(mWeightCohesion) + "|", BLACK_COLOR);
	mpGraphicsSystem->writeText(*mpFont, (float)x + 270, (float)y - 40, "C Radius: " + std::to_string((int)mRadiusCohesion), BLACK_COLOR);

	mpGraphicsSystem->writeText(*mpFont, (float)x, (float)y - 60, "G Weight: " + std::to_string(mWeightGroupAlign) + "|", BLACK_COLOR);
	mpGraphicsSystem->writeText(*mpFont, (float)x + 270, (float)y - 60, "G Radius: " + std::to_string((int)mRadiusGroupAlign), BLACK_COLOR);

	mpGraphicsSystem->writeText(*mpFont, (float)x, (float)y - 80, mWeightMode, BLACK_COLOR);

	//test of fill region the big red square
	//mpGraphicsSystem->fillRegion(*pDest, Vector2D(300, 300), Vector2D(500, 500), RED_COLOR);
	mpGraphicsSystem->swap();

	mInputSystem.update();
}

bool Game::endLoop()
{
	//mpMasterTimer->start();
	mpLoopTimer->sleepUntilElapsed( LOOP_TARGET_TIME );
	return mShouldExit;
}

float genRandomBinomial()
{
	return genRandomFloat() - genRandomFloat();
}

float genRandomFloat()
{
	float r = (float)rand()/(float)RAND_MAX;
	return r;
}

void Game::loadFlockData()
{
	ifstream inFile;
	ofstream outFile;
	float flockValArr[6];
	int j = 0;

	inFile.open(mFlockingDataFilename);
	if (inFile.fail())
	{
		cout << "File does not exist" << endl;
		return;
	}

	while (!inFile.eof())
	{
		string tempLine;
		string numValue;
		bool numFound = false;
		getline(inFile, tempLine);

		for (int i = 0; i < tempLine.length(); i++)
		{
			if (numFound == true)
			{
				numValue+=tempLine[i];
			}
			if (tempLine[i] == '=')
			{
				numFound = true;
			}
		}
	
		flockValArr[j] = stof(numValue);
		j++;
	}

	mWeightCohesion = flockValArr[0];
	mRadiusCohesion = flockValArr[1];
	mWeightSeparation = flockValArr[2];
	mRadiusSeparation = flockValArr[3];
	mWeightGroupAlign = flockValArr[4];
	mRadiusGroupAlign = flockValArr[5];
}

void Game::saveFlockData()
{
	ofstream outFile;

	outFile.open(mFlockingDataFilename);
	if (outFile.fail())
	{
		cout << "File does not exist" << endl;
		return;
	}

	outFile << "mWeightCohesion=" << mWeightCohesion << endl;
	outFile << "mRadiusCohesion=" << mRadiusCohesion << endl;
	outFile << "mWeightSeparation=" << mWeightSeparation << endl;
	outFile << "mRadiusSeparation=" << mRadiusSeparation << endl;
	outFile << "mWeightGroupAlign=" << mWeightGroupAlign << endl;
	outFile << "mRadiusGroupAlign=" << mRadiusGroupAlign;

	cout << "FLOCKING DATA SAVED" << endl;
}

void Game::handleEvent(const Event & theEvent)
{
	if (theEvent.getType() == ESC)
	{
		mShouldExit = true;
	}
	if (theEvent.getType() == MOUSE_LEFT)
	{
		//input manager handles the moveTo message call.
	}
	if (theEvent.getType() == ENTER)
	{
		cout << "SAVING FLOCKING DATA....." << endl;
		saveFlockData();
	}
	if (theEvent.getType() == A_KEY)
	{
		for (int i = 0; i < 10; i++)
		{ 
			Unit* pUnit = mpUnitManager->createRandomUnit(*mpSpriteManager->getSprite(AI_ICON_SPRITE_ID));
			if (pUnit == NULL)
			{
				mpUnitManager->deleteRandomUnit();
			}
		}
	}
	if (theEvent.getType() == UP_ARROW)
	{
		if (mWeightMode != "null")
		{
			if (mWeightMode == "Separation")
			{
				mWeightSeparation += 0.1;
			}
			else if (mWeightMode == "Cohesion")
			{
				mWeightCohesion += 0.1;
			}
			else if (mWeightMode == "Group Align")
			{
				mWeightGroupAlign += 0.1;
			}
		}
	}
	if (theEvent.getType() == DOWN_ARROW)
	{
		if (mWeightMode != "null")
		{
			if (mWeightMode == "Separation")
			{
				mWeightSeparation -= 0.1;
			}
			else if (mWeightMode == "Cohesion")
			{
				mWeightCohesion -= 0.1;
			}
			else if (mWeightMode == "Group Align")
			{
				mWeightGroupAlign -= 0.1;
			}
		}
	}
	if (theEvent.getType() == LEFT_ARROW)
	{
		if (mWeightMode != "null")
		{
			if (mWeightMode == "Separation")
			{
				mRadiusSeparation -= 5;
			}
			else if (mWeightMode == "Cohesion")
			{
				mRadiusCohesion -= 5;
			}
			else if (mWeightMode == "Group Align")
			{
				mRadiusGroupAlign -= 5;
			}
		}
	}
	if (theEvent.getType() == RIGHT_ARROW)
	{
		if (mWeightMode != "null")
		{
			if (mWeightMode == "Separation")
			{
				mRadiusSeparation += 5;
			}
			else if (mWeightMode == "Cohesion")
			{
				mRadiusCohesion += 5;
			}
			else if (mWeightMode == "Group Align")
			{
				mRadiusGroupAlign += 5;
			}
		}
	}
	if (theEvent.getType() == D_KEY)
	{
		mpUnitManager->deleteRandomUnit();
	}
	if (theEvent.getType() == S_KEY)
	{
		if (mWeightMode == "Separation")
		{
			mWeightMode = "null";
		}
		else 
		{
			mWeightMode = "Separation";
		}
	}
	if (theEvent.getType() == C_KEY)
	{
		if (mWeightMode == "Cohesion")
		{
			mWeightMode = "null";
		}
		else
		{
			mWeightMode = "Cohesion";
		}
	}
	if (theEvent.getType() == G_KEY)
	{
		if (mWeightMode == "Group Align")
		{
			mWeightMode = "null";
		}
		else
		{
			mWeightMode = "Group Align";
		}
	}
}