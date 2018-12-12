#pragma once

/*Game - class to hold all game related info.

Dean Lawson
Champlain College
2011
*/

#include "Game.h"
#include "Vector2D.h"
#include <vector>

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
class AllMightyCandy;
class Powerup;

const float LOOP_TARGET_TIME = 33.3f;//how long should each frame of execution take? 30fps = 33.3ms/frame
const IDType PLAYER_ICON_SPRITE_ID = 1;
const IDType AI_ICON_SPRITE_ID = 2;
const IDType TARGET_SPRITE_ID = 3;
const IDType COIN_SPRITE_ID = 4;
const IDType PACMAN_SPRITE_ID = 5;
const IDType RED_SPRITE_ID = 6;
const IDType BLUE_SPRITE_ID = 7;
const IDType PINK_SPRITE_ID = 8;
const IDType ORANGE_SPRITE_ID = 9;
const IDType CHERRY_SPRITE_ID = 10;
const IDType POWER_UP_ID = 11;

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
	inline GridPathfinder* getPathFleeing() { return mpPathFleeing; };
	inline Grid* getGrid() { return mpGrid; };
	inline GridGraph* getGridGraph() { return mpGridGraph; };
	inline GraphicsSystem* getGraphicsSystem() const { return mpGraphicsSystem; };
	inline GraphicsBufferManager* getGraphicsBufferManager() const { return mpGraphicsBufferManager; };
	inline SpriteManager* getSpriteManager() const { return mpSpriteManager; };
	inline void setMousePosition(int x, int y) { mousePosX = x, mousePosY = y;};
	Coins* getCoins() { return mpCoin; };
	AllMightyCandy* getCandy() { return mpMightyCandy; };
	Score* getScore() { return mpScore; };
	Powerup* getPowerUp() { return mpPowerUp; };
	std::vector<Sprite*> getEnemySprite() { return mEnemySprites; };
	void setCanDestroyEnemies(bool myBool) { mCanDestroyEnemies = myBool; };
	bool getCanDestroyEnemies() { return mCanDestroyEnemies; };
	void markForExiting() { startingToExit = true; };
	void resetTimer();

	void setEnemySpeed(float mySpeed) { enemySpeed = mySpeed; };
	float getEnemySpeed() { return enemySpeed; };

	int getEnemyHitPoints() { return enemyHitPoints; };
	int getPowerUpRespawn() { return powerUpRespawnRate; };
	int getDamageRadiusEnemy() { return damageRadiusEnemy; };
	int getDamageRadiusPlayer() { return damageRadiusEnemy; };
	int getTransitionRadiusEnemy() { return transitionRadius; };
	int getIdleTimeRespawn() { return idleTime; };

	void changeHandle(bool myHandle) { canHandle = myHandle; };
	void checkForExit();

	//Added in functions from other project
	inline ComponentManager* getComponentManager() { return mpComponentManager; };
	inline UnitManager* getUnitManager() { return mpUnitManager; };
	inline Timer* getMasterTimer() const { return mpMasterTimer; };
	inline double getCurrentTime() const { return mpMasterTimer->getElapsedTime(); };

	void movePacman();
	void moveEnemy();
	inline void SetPacCanMove(bool moveStatus){mPacCanMove = moveStatus;};


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
	GridPathfinder* mpPathFleeing;
	PathPooling* mpPathPool;
	AllMightyCandy* mpMightyCandy;
	Powerup* mpPowerUp;
	Timer mTimer;
	vector<Sprite*> mEnemySprites;
	int timer;
	float candyRespawn;
	bool startingToExit;
	float enemySpeed;

	int enemyHitPoints;
	int powerUpRespawnRate;
	int damageRadiusEnemy;
	int transitionRadius;
	int idleTime;
	int damageRadiusPlayer;


	GraphicsBufferID mBackgroundBufferID = "woods";
	GraphicsBufferID mPlayerIconBufferID = "player";
	GraphicsBufferID mEnemyIconBufferID = "enemy";
	GraphicsBufferID mTargetBufferID = "target";
	GraphicsBufferID mCoinBufferID = "coins";
	GraphicsBufferID mPacmanBufferID = "pacman";
	GraphicsBufferID mRedGhostBufferID = "redGhost";
	GraphicsBufferID mBlueGhostBufferID = "blueGhost";
	GraphicsBufferID mOrangeGhostBufferID = "orangeGhost";
	GraphicsBufferID mPinkGhostBufferID = "pinkGhost";
	GraphicsBufferID mCherryBufferID = "allmightyCandy";
	GraphicsBufferID mPowerUpID = "powerUp";
	

	bool canHandle = false;
	bool mPacCanMove = false;
	bool mCanDestroyEnemies;

	int mPacXDist = 0;
	int mPacYDist = 0;

	Vector2D mPacXDir;// = Vector2D(0, 0);
	Vector2D mPacYDir;// = Vector2D(0, 0);

	int mousePosX;
	int mousePosY;

};