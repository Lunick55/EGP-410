#pragma once

#include "Trackable.h"
#include "PerformanceTracker.h"
#include "Defines.h"
#include "EventListener.h"
#include "InputSystem.h"
#include <fstream>
#include <string>

class GraphicsSystem;
class GraphicsBuffer;
class Font;
class GraphicsBufferManager;
class SpriteManager;
class KinematicUnit;
class Timer;
class ComponentManager;
class UnitManager;

const IDType BACKGROUND_SPRITE_ID = 0;
const IDType PLAYER_ICON_SPRITE_ID = 1;
const IDType AI_ICON_SPRITE_ID = 2;
const IDType TARGET_SPRITE_ID = 3;

const float LOOP_TARGET_TIME = 33.3f;//how long should each frame of execution take? 30fps = 33.3ms/frame

class Game:public EventListener
{
public:
	Game();
	~Game();

	bool init();//returns true if no errors, false otherwise
	void cleanup();

	//game loop
	void beginLoop();
	void processLoop();
	bool endLoop();

	inline GraphicsSystem* getGraphicsSystem() const { return mpGraphicsSystem; };
	inline GraphicsBufferManager* getGraphicsBufferManager() const { return mpGraphicsBufferManager; };
	inline SpriteManager* getSpriteManager() const { return mpSpriteManager; };
	inline ComponentManager* getComponentManager() { return mpComponentManager; };
	inline UnitManager* getUnitManager() { return mpUnitManager; };
	inline Timer* getMasterTimer() const { return mpMasterTimer; };
	inline double getCurrentTime() const { return mpMasterTimer->getElapsedTime(); };
	inline float getGroupAlignWeight(){return mWeightGroupAlign;};
	inline float getSeparationWeight(){return mWeightSeparation;};
	inline float getCohesionWeight(){return mWeightCohesion;};
	inline float getGroupAlignRadius() { return mRadiusGroupAlign; };
	inline float getSeparationRadius() { return mRadiusSeparation; };
	inline float getCohesionRadius() { return mRadiusCohesion; };

	void loadFlockData();
	void saveFlockData();
	void handleEvent(const Event &theEvent);

private:
	GraphicsSystem* mpGraphicsSystem;
	GraphicsBufferManager* mpGraphicsBufferManager;
	SpriteManager* mpSpriteManager;
	ComponentManager* mpComponentManager;
	UnitManager* mpUnitManager;
	Font* mpFont;
	Timer* mpLoopTimer;
	Timer* mpMasterTimer;
	bool mShouldExit;
	InputSystem mInputSystem;

	string mWeightMode = "null";
	string mFlockingDataFilename = "..//file.txt";
	float mWeightCohesion = 1.0f;
	float mRadiusCohesion = 100.0f;
	float	mWeightSeparation = 1.0f;
	float mRadiusSeparation = 25.0f;
	float	mWeightGroupAlign = 1.0f;
	float mRadiusGroupAlign = 125.0f;

	//should be somewhere else
	GraphicsBufferID mBackgroundBufferID = "woods";
	GraphicsBufferID mPlayerIconBufferID = "player";
	GraphicsBufferID mEnemyIconBufferID = "enemy";
	GraphicsBufferID mTargetBufferID = "target";

};

float genRandomBinomial();//range -1:1 from "Artificial Intelligence for Games", Millington and Funge
float genRandomFloat();//range 0:1 from "Artificial Intelligence for Games", Millington and Funge

extern Game* gpGame;
extern PerformanceTracker* gpPerformanceTracker;

