#pragma once
#include <core.h>
#include <PhysicsSystem.h>
#include <drawSystem.h>
#include <droppedItemSystem.h>
#include <uiSystem.h>
#include <toolSystem.h>
#include <aiSystem.h>
#include <mobSystem.h>
#include <particleEmmiterSystem.h>

enum Scene {
	NONE = 0,
	STARTMENU = 1,
	GAMELOOP = 2
};

namespace game
{
	extern uiSystem* uiSys;
	extern physicsSystem* physSys;
	extern drawSystem* drawSys;
	extern aiSystem* aiSys;
	extern toolSystem* toolSys;
	extern droppedItemSystem* droppedItemSys;
	extern mobSystem* mobSys;
	extern particleEmmiterSystem* particleESys;

	extern Scene swapTo;
	extern Scene currScene;

	extern bool showCursor;

	
	void initCore();
	void endall();

	void init();
	void run();

	void savesettings();
	bool loadsettings();

	void initBase();

	void changeSceneTo(Scene scene);


	void resChanged();
};

