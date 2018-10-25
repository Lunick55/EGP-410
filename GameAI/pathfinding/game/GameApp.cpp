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

#include <SDL.h>
#include <fstream>
#include <vector>

const int GRID_SQUARE_SIZE = 32;
const std::string gFileName = "pathgrid.txt";

GameApp::GameApp()
:mpMessageManager(NULL)
,mpGrid(NULL)
,mpGridGraph(NULL)
,mpPathfinder(NULL)
,mpDebugDisplay(NULL)
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

	//create and load the Grid, GridBuffer, and GridRenderer
	mpGrid = new Grid(mpGraphicsSystem->getWidth(), mpGraphicsSystem->getHeight(), GRID_SQUARE_SIZE);
	mpGridVisualizer = new GridVisualizer( mpGrid );
	std::ifstream theStream( gFileName );
	mpGrid->load( theStream );

	//create the GridGraph for pathfinding
	mpGridGraph = new GridGraph(mpGrid);
	//init the nodes and connections
	mpGridGraph->init();

	//mpPathfinder = new DijkstraPathfinder(mpGridGraph);
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

	//load buffers
	mpGraphicsBufferManager->loadBuffer(mBackgroundBufferID, "wallpaper.bmp");

	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer( mBackgroundBufferID );
	if( pBackGroundBuffer != NULL )
	{
		mpSpriteManager->createAndManageSprite( BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, (float)pBackGroundBuffer->getWidth(), (float)pBackGroundBuffer->getHeight() );
	}

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
#ifdef VISUALIZE_PATH
	//show pathfinder visualizer
	mpPathfinder->drawVisualization(mpGrid, pBackBuffer);
#endif

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