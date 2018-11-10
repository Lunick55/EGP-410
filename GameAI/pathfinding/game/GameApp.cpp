#include "Game.h"
#include "GameApp.h"
#include "GameMessageManager.h"
#include "PathToMessage.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "GraphicsBufferManager.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include <Vector2D.h>
#include "Grid.h"
#include "GridGraph.h"
#include "Connection.h"
#include "Path.h"
#include "DepthFirstPathfinder.h"
#include "DijkstraPathfinder.h"
#include "AStarPathFinder.h"
#include "Pathfinder.h"
#include "GridPathfinder.h"
#include "GridVisualizer.h"
#include "DebugDisplay.h"
#include "PathfindingDebugContent.h"
#include "UnitManager.h"
#include "Unit.h"
#include "ComponentManager.h"
#include "FollowPath.h"

#include <SDL.h>
#include <fstream>
#include <vector>

const int GRID_SQUARE_SIZE = 32;
const std::string gFileName = "pathgrid.txt";
const float TARGET_ELAPSED_MS = LOOP_TARGET_TIME / 1000.0f;
const Uint32 MAX_UNITS = 100;

GameApp::GameApp()
:mpMessageManager(NULL)
,mpGrid(NULL)
,mpGridGraph(NULL)
,mpPathfinder(NULL)
,mpDebugDisplay(NULL)
, mpUnitManager(NULL)
, mpComponentManager(NULL)
{
}

GameApp::~GameApp()
{
	cleanup();
}

bool GameApp::init()
{
	bool retVal = Game::init();
	if( retVal == false )
	{

		return false;
	}

	mpMessageManager = new GameMessageManager();
	mpUnitManager = new UnitManager(MAX_UNITS);
	mpComponentManager = new ComponentManager(MAX_UNITS);
	//create and load the Grid, GridBuffer, and GridRenderer
	mpGrid = new Grid(mpGraphicsSystem->getWidth(), mpGraphicsSystem->getHeight(), GRID_SQUARE_SIZE);
	mpGridVisualizer = new GridVisualizer( mpGrid );
	std::ifstream theStream( gFileName );
	mpGrid->load( theStream );

	//create the GridGraph for pathfinding
	mpGridGraph = new GridGraph(mpGrid);
	//init the nodes and connections
	mpGridGraph->init();

	mpPathfinder = new AStarPathfinder(mpGridGraph);
	//mpPathfinder = new DepthFirstPathfinder(mpGridGraph);

	//load listeners
	EventSystem::initInstance();
	EventSystem::getInstance()->addListener(ESC, this);
	EventSystem::getInstance()->addListener(D_KEY, this);
	EventSystem::getInstance()->addListener(ENTER, this);
	EventSystem::getInstance()->addListener(S_KEY, this);
	EventSystem::getInstance()->addListener(F_KEY, this);
	EventSystem::getInstance()->addListener(A_KEY, this);
	EventSystem::getInstance()->addListener(W_KEY, this);
	EventSystem::getInstance()->addListener(G_KEY, this);
	EventSystem::getInstance()->addListener(DOWN_ARROW, this);
	EventSystem::getInstance()->addListener(UP_ARROW, this);
	EventSystem::getInstance()->addListener(LEFT_ARROW, this);
	EventSystem::getInstance()->addListener(RIGHT_ARROW, this);
	EventSystem::getInstance()->addListener(MOUSE_LEFT, this);

	//load buffers
	mpGraphicsBufferManager->loadBuffer(mBackgroundBufferID, "wallpaper.bmp");
	mpGraphicsBufferManager->loadBuffer(mPlayerIconBufferID, "arrow.png");
	mpGraphicsBufferManager->loadBuffer(mEnemyIconBufferID, "enemy-arrow.png");
	mpGraphicsBufferManager->loadBuffer(mTargetBufferID, "target.png");


	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer( mBackgroundBufferID );
	if( pBackGroundBuffer != NULL )
	{
		mpSpriteManager->createAndManageSprite( BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, (float)pBackGroundBuffer->getWidth(), (float)pBackGroundBuffer->getHeight() );
	}
	GraphicsBuffer* pPlayerBuffer = mpGraphicsBufferManager->getBuffer(mPlayerIconBufferID);
	Sprite* pArrowSprite = NULL;
	if (pPlayerBuffer != NULL)
	{
		pArrowSprite = mpSpriteManager->createAndManageSprite(PLAYER_ICON_SPRITE_ID, pPlayerBuffer, 0, 0, (float)pPlayerBuffer->getWidth(), (float)pPlayerBuffer->getHeight());
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

	/*Unit* pUnit = mpUnitManager->createRandomUnit(*mpSpriteManager->getSprite(AI_ICON_SPRITE_ID));
	if (pUnit == NULL)
	{
		mpUnitManager->deleteRandomUnit();
	}*/
	//debug display
	PathfindingDebugContent* pContent = new PathfindingDebugContent( mpPathfinder );
	mpDebugDisplay = new DebugDisplay( Vector2D(0,12), pContent );

	mpMasterTimer->start();
	return true;
}

void GameApp::cleanup()
{
	delete mpMessageManager;
	mpMessageManager = NULL;

	delete mpGrid;
	mpGrid = NULL;

	delete mpGridVisualizer;
	mpGridVisualizer = NULL;

	delete mpGridGraph;
	mpGridGraph = NULL;

	delete mpPathfinder;
	mpPathfinder = NULL;

	delete mpDebugDisplay;
	mpDebugDisplay = NULL;

	delete mpUnitManager;
	mpUnitManager = NULL;

	delete mpComponentManager;
	mpComponentManager = NULL;
}

void GameApp::beginLoop()
{
	//should be the first thing done
	Game::beginLoop();
}

void GameApp::processLoop()
{
	//get back buffer
	GraphicsBuffer* pBackBuffer = mpGraphicsSystem->getBackBuffer();
	//copy to back buffer
	mpGridVisualizer->draw( *pBackBuffer );
	mpUnitManager->updateAll(TARGET_ELAPSED_MS);
	mpComponentManager->update(TARGET_ELAPSED_MS);
#ifdef VISUALIZE_PATH
	//show pathfinder visualizer
	mpPathfinder->drawVisualization(mpGrid, pBackBuffer);
#endif
	//draw units
	mpUnitManager->drawAll();
	mpDebugDisplay->draw( pBackBuffer );

	mpMessageManager->processMessagesForThisframe();

	//should be last thing in processLoop
	mInputSystem.update();
	Game::processLoop();
}

bool GameApp::endLoop()
{
	return Game::endLoop();
}

void GameApp::handleEvent(const Event & theEvent)
{
	if (theEvent.getType() == ESC)
	{
		markForExit();
	}
	if (theEvent.getType() == MOUSE_LEFT)
	{
		//input manager handles the moveTo message call.
		//need to change this to start pathfinding towards click

		GridPathfinder* pPathfinder = this->getPathfinder();

		for (int i = 1; i <= mpUnitManager->getUnitCount(); i++)
		{
			GridGraph* pGridGraph = this->getGridGraph();
			Grid* pGrid = this->getGrid();
			int fromIndex = pGrid->getSquareIndexFromPixelXY((int)mpUnitManager->getUnit(i)->getPositionComponent()->getPosition().getX(), (int)mpUnitManager->getUnit(i)->getPositionComponent()->getPosition().getY());
			int toIndex = pGrid->getSquareIndexFromPixelXY((int)mousePosX, (int)mousePosY);
			Node* pFromNode = pGridGraph->getNode(fromIndex);
			Node* pToNode = pGridGraph->getNode(toIndex);
			pPathfinder->findPath(pFromNode, pToNode);
			//set path
			FollowPath* pFollowSteering = dynamic_cast<FollowPath*>(mpUnitManager->getUnit(i)->getSteeringComponent()->getSteeringBehavior());
			Path* newPath = pPathfinder->findPath(pFromNode, pToNode);
			pFollowSteering->resetIndex();
			pFollowSteering->setPath(newPath);			
		}

	}
	if (theEvent.getType() == S_KEY)
	{
		for (int i = 0; i < 10; i++)
		{
			Unit* pUnit = mpUnitManager->createRandomUnit(*mpSpriteManager->getSprite(AI_ICON_SPRITE_ID));
			if (pUnit == NULL)
			{
				mpUnitManager->deleteRandomUnit();
			}

		}
		cout << "Add random unit" << endl;
	}
	if (theEvent.getType() == D_KEY)
	{
		//dkitsra
		delete mpPathfinder; 
		mpPathfinder = new DijkstraPathfinder(mpGridGraph);

		delete mpDebugDisplay;
		PathfindingDebugContent* pContent = new PathfindingDebugContent(mpPathfinder);
		mpDebugDisplay = new DebugDisplay(Vector2D(0, 12), pContent);
	}
	if (theEvent.getType() == A_KEY)
	{
		//aaaaaaa
		delete mpPathfinder;
		mpPathfinder = new AStarPathfinder(mpGridGraph);

		delete mpDebugDisplay;
		PathfindingDebugContent* pContent = new PathfindingDebugContent(mpPathfinder);
		mpDebugDisplay = new DebugDisplay(Vector2D(0, 12), pContent);
	}
	if (theEvent.getType() == F_KEY)
	{
		//DFS
		delete mpPathfinder;
		mpPathfinder = new DepthFirstPathfinder(mpGridGraph);

		delete mpDebugDisplay;
		PathfindingDebugContent* pContent = new PathfindingDebugContent(mpPathfinder);
		mpDebugDisplay = new DebugDisplay(Vector2D(0, 12), pContent);
	}
}