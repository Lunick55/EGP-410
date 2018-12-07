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
#include "PathSmoothing.h"
#include "PathPooling.h"
#include "PacSteering.h"
#include "Score.h"
#include "Coins.h"
#include "Player.h"
#include "Timer.h"

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

	//create new pathPool
	mpPathPool = new PathPooling;

	//load listeners
	EventSystem::initInstance();
	EventSystem::getInstance()->addListener(ESC, this);
	EventSystem::getInstance()->addListener(D_KEY, this);
	EventSystem::getInstance()->addListener(S_KEY, this);
	EventSystem::getInstance()->addListener(A_KEY, this);
	EventSystem::getInstance()->addListener(W_KEY, this);
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
	mpGraphicsBufferManager->loadBuffer(mCoinBufferID, "Coin.png");
	mpGraphicsBufferManager->loadBuffer(mPacmanBufferID, "PacmanOpen.png");
	mpGraphicsBufferManager->loadBuffer(mBlueGhostBufferID, "BlueGhost.png");
	mpGraphicsBufferManager->loadBuffer(mRedGhostBufferID, "RedGhost.png");
	mpGraphicsBufferManager->loadBuffer(mPinkGhostBufferID, "PinkGhost.png");
	mpGraphicsBufferManager->loadBuffer(mOrangeGhostBufferID, "OrangeGhost.png");



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

	GraphicsBuffer* pCoinBuffer = mpGraphicsBufferManager->getBuffer(mCoinBufferID);
	Sprite* pCoins = NULL;
	if (pCoinBuffer != NULL)
	{
		pCoins = mpSpriteManager->createAndManageSprite(COIN_SPRITE_ID, pCoinBuffer, 0, 0, (float)pCoinBuffer->getWidth(), (float)pCoinBuffer->getHeight());
	}

	mpCoin = new Coins(*pCoins);
	//mpCoin->addCoins(10);
	//mpCoin->draw();

	//setup red ghost
	GraphicsBuffer* pRedGhost = mpGraphicsBufferManager->getBuffer(mRedGhostBufferID);
	Sprite* pRGhost = NULL;
	if (pRedGhost != NULL)
	{
		pRGhost = mpSpriteManager->createAndManageSprite(RED_SPRITE_ID, pRedGhost, 0, 0, (float)pRedGhost->getWidth(), (float)pRedGhost->getHeight());
	}
	Unit* pRdGhost = mpUnitManager->createUnit(*pRGhost, true, PositionData(Vector2D(256, 130), 0));
	//set up orange ghost
	GraphicsBuffer* pOrangeGhost = mpGraphicsBufferManager->getBuffer(mOrangeGhostBufferID);
	Sprite* pOGhost = NULL;
	if (pOrangeGhost != NULL)
	{
		pOGhost = mpSpriteManager->createAndManageSprite(ORANGE_SPRITE_ID, pOrangeGhost, 0, 0, (float)pOrangeGhost->getWidth(), (float)pOrangeGhost->getHeight());
	}
	Unit* pOrGhost = mpUnitManager->createUnit(*pOGhost, true, PositionData(Vector2D(224, 130), 0));
	//set up pink ghost
	GraphicsBuffer* pPinkGhost = mpGraphicsBufferManager->getBuffer(mPinkGhostBufferID);
	Sprite* pPGhost = NULL;
	if (pPinkGhost != NULL)
	{
		pPGhost = mpSpriteManager->createAndManageSprite(PINK_SPRITE_ID, pPinkGhost, 0, 0, (float)pPinkGhost->getWidth(), (float)pPinkGhost->getHeight());
	}
	Unit* pPkGhost = mpUnitManager->createUnit(*pPGhost, true, PositionData(Vector2D(288, 130), 0));
	//set up blue ghost
	GraphicsBuffer* pBlueGhost = mpGraphicsBufferManager->getBuffer(mBlueGhostBufferID);
	Sprite* pBlue = NULL;
	if (pBlueGhost != NULL)
	{
		pBlue = mpSpriteManager->createAndManageSprite(BLUE_SPRITE_ID, pBlueGhost, 0, 0, (float)pBlueGhost->getWidth(), (float)pBlueGhost->getHeight());
	}
	Unit* pBlueBoi = mpUnitManager->createUnit(*pBlue, true, PositionData(Vector2D(192, 130), 0));

	GraphicsBuffer* pPacmanBuffer = mpGraphicsBufferManager->getBuffer(mPacmanBufferID);
	Sprite* pPacman = NULL;
	if (pPacmanBuffer != NULL)
	{
		pPacman = mpSpriteManager->createAndManageSprite(PACMAN_SPRITE_ID, pPacmanBuffer, 0, 0, (float)pPacmanBuffer->getWidth(), (float)pPacmanBuffer->getHeight());
	}

	Unit* pPlayer = mpUnitManager->createPlayerUnit(*pPacman,true,PositionData(Vector2D(256,320), 0));
	pPlayer->setSteering(Steering::PAC_STEER, Vector2D(256, 320));
	pPlayer->setShowTarget(true);


	PathfindingDebugContent* pContent = new PathfindingDebugContent( mpPathfinder );
	mpDebugDisplay = new DebugDisplay( Vector2D(0,12), pContent );

	//Needs to be data driven
	mpScore = new Score(Vector2D(5, 5));

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

	delete mpScore;
	mpScore = NULL;

	delete mpCoin;
	mpCoin = NULL;

	delete mpUnitManager;
	mpUnitManager = NULL;

	delete mpComponentManager;
	mpComponentManager = NULL;

	delete mpPathPool;
	mpPathPool = NULL;


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
	//mpUnitManager->updateAll(TARGET_ELAPSED_MS);
	mpComponentManager->update(TARGET_ELAPSED_MS);

	movePacman();

#ifdef VISUALIZE_PATH
	//show pathfinder visualizer
	mpPathfinder->drawVisualization(mpGrid, pBackBuffer);
#endif
	//draw units
	mpUnitManager->drawAll();
	//mpDebugDisplay->draw( pBackBuffer );
	mpScore->draw(pBackBuffer);

	mpMessageManager->processMessagesForThisframe();

	//should be last thing in processLoop
	mInputSystem.update();
	Game::processLoop();
}

bool GameApp::endLoop()
{
	return Game::endLoop();
}

void GameApp::movePacman()
{
	if (mPacCanMove)
	{
		GridPathfinder* pPathfinder = this->getPathfinder();

		GridGraph* pGridGraph = this->getGridGraph();
		Grid* pGrid = this->getGrid();
		//get the from and to index from the grid
		float playerX = mpUnitManager->getPlayerUnit()->getPositionComponent()->getPosition().getX() + 16;
		float playerY = mpUnitManager->getPlayerUnit()->getPositionComponent()->getPosition().getY() + 16;

		int fromIndex = pGrid->getSquareIndexFromPixelXY((int)playerX, (int)playerY);
		int toIndex = pGrid->getSquareIndexFromPixelXY((int)playerX + mPacXDist, (int)playerY + mPacYDist);

		//Look for any intersections or walls in a straight line
		for (int i = 1; pGrid->getValueAtIndex(toIndex) != INTERSECTION_VALUE && pGrid->getValueAtIndex(toIndex) != BLOCKING_VALUE; i++)
		{
			toIndex = pGrid->getSquareIndexFromPixelXY((int)playerX + mPacXDist*i, (int)playerY + mPacYDist*i);
		}

		//If we're going into a wall, stop where you are
		if (pGrid->getValueAtIndex(toIndex) == BLOCKING_VALUE)
		{
			toIndex = pGrid->getSquareIndexFromPixelXY((int)playerX, (int)playerY);
		}

		Node* pFromNode = pGridGraph->getNode(fromIndex);
		Node* pToNode = pGridGraph->getNode(toIndex);

		PacSteering* pPacSteer = dynamic_cast<PacSteering*>(mpUnitManager->getPlayerUnit()->getSteeringComponent()->getSteeringBehavior());
		pPacSteer->moveDirection(mPacXDir, mPacYDir);

		Path*	newPath = pPathfinder->findPath(pFromNode, pToNode);

		//smooth the path
		PathSmoothing mySmooth(pGridGraph);
		newPath = mySmooth.smoothPath(newPath);

		//reset the index every click
		pPacSteer->resetIndex();
		pPacSteer->setPath(newPath);

		mPacCanMove = false;
	}
}

void GameApp::handleEvent(const Event & theEvent)
{
	
		if (theEvent.getType() == ESC)
		{
			markForExit();
		}
		if (theEvent.getType() == MOUSE_LEFT)
		{

			GridPathfinder* pPathfinder = this->getPathfinder();

			//continue for every unit
			for (int i = 1; i <= mpUnitManager->getUnitCount(); i++)
			{
				GridGraph* pGridGraph = this->getGridGraph();
				Grid* pGrid = this->getGrid();
				//ge the from and to index from the grid
				int fromIndex = pGrid->getSquareIndexFromPixelXY((int)mpUnitManager->getUnit(i)->getPositionComponent()->getPosition().getX(), (int)mpUnitManager->getUnit(i)->getPositionComponent()->getPosition().getY());
				int toIndex = pGrid->getSquareIndexFromPixelXY((int)mousePosX, (int)mousePosY);
				Node* pFromNode = pGridGraph->getNode(fromIndex);
				Node* pToNode = pGridGraph->getNode(toIndex);

				//set path
				FollowPath* pFollowSteering = dynamic_cast<FollowPath*>(mpUnitManager->getUnit(i)->getSteeringComponent()->getSteeringBehavior());
				Path* newPath;

				//check to see wheher there is already a path made from the pool
				if (mpPathPool->getPath(pFromNode, pToNode) != nullptr)
				{
					//get that path from the pool
					newPath = mpPathPool->getPath(pFromNode, pToNode);
				}
				else
				{
					//otherwise assign it to the pool so that it doesnt need to research it again
					newPath = pPathfinder->findPath(pFromNode, pToNode);
					mpPathPool->storePath(pFromNode, pToNode, newPath);
				}
				//smooth the path
				PathSmoothing mySmooth(pGridGraph);
				newPath = mySmooth.smoothPath(newPath);

				//reset the index every click
				pFollowSteering->resetIndex();
				pFollowSteering->setPath(newPath);
			}

		}
		if (theEvent.getType() == S_KEY || theEvent.getType() == DOWN_ARROW)
		{
			if (canHandle)
			{
				cout << "move down" << endl;

				mPacXDist = 0;
				mPacYDist = 32;
				mPacXDir = Vector2D(0, 0);
				mPacYDir = Vector2D(0, 1);

				mPacCanMove = true;
				//movePacman();

				canHandle = false;
			}
		}
		if (theEvent.getType() == D_KEY || theEvent.getType() == RIGHT_ARROW)
		{
			if (canHandle)
			{
				cout << "move left" << endl;

				mPacXDist = 32;
				mPacYDist = 0;
				mPacXDir = Vector2D(0, 1);
				mPacYDir = Vector2D(0, 0);

				mPacCanMove = true;
				//movePacman();

				canHandle = false;
			}
		}
		if (theEvent.getType() == A_KEY || theEvent.getType() == LEFT_ARROW)
		{
			
			if (canHandle)
			{
				cout << "move left" << endl;
				
				mPacXDist = -32;
				mPacYDist = 0;
				mPacXDir = Vector2D(1, 0);
				mPacYDir = Vector2D(0, 0);

				mPacCanMove = true;
				//movePacman();

				canHandle = false;
			}
			
		}
		if (theEvent.getType() == W_KEY || theEvent.getType() == UP_ARROW)
		{
			//DFS
			
			if (canHandle)
			{
				cout << "move up" << endl;

				mPacXDist = 0;
				mPacYDist = -32;
				mPacXDir = Vector2D(0, 0);
				mPacYDir = Vector2D(1, 0);

				mPacCanMove = true;
				//movePacman();

				canHandle = false;
			}
			
		}

}