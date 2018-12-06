#pragma once

/*Game - class to hold all game related info.

Dean Lawson
Champlain College
2011
*/

#include "Game.h"
#include "Vector2D.h"

//forward declarations
class GraphicsBuffer;
class Sprite;
class KinematicUnit;
class GameMessageManager;
class Grid;
class GridVisualizer;
class GridGraph;
class GridPathfinder;
class DebugDisplay;
class Score;
class KinematicUnit;
class Timer;
class ComponentManager;
class UnitManager;
class Coins;
class PathPooling;
class Player;
class Timer;
class Vector2D;

const float LOOP_TARGET_TIME = 33.3f;//how long should each frame of execution take? 30fps = 33.3ms/frame
const IDType PLAYER_ICON_SPRITE_ID = 1;
const IDType AI_ICON_SPRITE_ID = 2;
const IDType TARGET_SPRITE_ID = 3;
const IDType COIN_SPRITE_ID = 4;
const IDType PACMAN_SPRITE_ID = 5;
const IDType RED_SPRITE_ID = 5;
const IDType BLUE_SPRITE_ID = 5;
const IDType PINK_SPRITE_ID = 5;
const IDType ORANGE_SPRITE_ID = 5;

class GameApp: public Game
{
public:
	GameApp();
	~GameApp();

	virtual bool init();
	virtual void cleanup();

	//game loop
	virtual void beginLoop();
	virtual void processLoop();
	virtual bool endLoop();

	//accessors
	inline GameMessageManager* getMessageManager() { return mpMessageManager; };
	inline GridVisualizer* getGridVisualizer() { return mpGridVisualizer; };
	inline GridPathfinder* getPathfinder() { return mpPathfinder; };
	inline Grid* getGrid() { return mpGrid; };
	inline GridGraph* getGridGraph() { return mpGridGraph; };
	inline GraphicsSystem* getGraphicsSystem() const { return mpGraphicsSystem; };
	inline GraphicsBufferManager* getGraphicsBufferManager() const { return mpGraphicsBufferManager; };
	inline SpriteManager* getSpriteManager() const { return mpSpriteManager; };
	inline void setMousePosition(int x, int y) { mousePosX = x, mousePosY = y;};

	void changeHandle(bool myHandle) { canHandle = myHandle; };

	//Added in functions from other project
	inline ComponentManager* getComponentManager() { return mpComponentManager; };
	inline UnitManager* getUnitManager() { return mpUnitManager; };
	inline Timer* getMasterTimer() const { return mpMasterTimer; };
	inline double getCurrentTime() const { return mpMasterTimer->getElapsedTime(); };

	void movePacman();

	void handleEvent(const Event & theEvent);

private:
	GameMessageManager* mpMessageManager;
	Grid* mpGrid;
	GridVisualizer* mpGridVisualizer;
	GridGraph* mpGridGraph;
	DebugDisplay* mpDebugDisplay;
	Score* mpScore;
	Coins* mpCoin;
	InputSystem mInputSystem;
	ComponentManager* mpComponentManager;
	UnitManager* mpUnitManager;
	GridPathfinder* mpPathfinder;
	PathPooling* mpPathPool;
	Timer mTimer;

	GraphicsBufferID mBackgroundBufferID = "woods";
	GraphicsBufferID mPlayerIconBufferID = "player";
	GraphicsBufferID mEnemyIconBufferID = "enemy";
	GraphicsBufferID mTargetBufferID = "target";
	GraphicsBufferID mCoinBufferID = "coins";
	GraphicsBufferID mPacmanBufferID = "pacman";
	GraphicsBufferID mRedGhostBufferID = "redGhost";
	GraphicsBufferID mBlueGhostBufferID = "blueGhost";
	GraphicsBufferID mOrangeGhostBufferID = "redGhost";
	GraphicsBufferID mPinkGhostBufferID = "pinkGhost";

	bool canHandle = false;
	bool mPacCanMove = false;

	int mPacXDist = 0;
	int mPacYDist = 0;

	Vector2D mPacXDir;// = Vector2D(0, 0);
	Vector2D mPacYDir;// = Vector2D(0, 0);

	int mousePosX;
	int mousePosY;

};