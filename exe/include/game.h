#pragma once
#include <core.h>
#include <componentsystems.h>

enum Scene {
	NONE = 0,
	STARTMENU = 1,
	GAME = 2
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

	extern std::vector<glm::vec2> resolutions;
	extern int currRes;
	extern std::string currResText;
	
	void initGlfw();
	void initCore();
	void endall();

	void init();
	void run();

	void savesettings();
	void loadsettings();

	void initBase();

	void changeSceneTo(Scene scene);

	void drawMain();
	void drawOverlays();
	void updateSunAndMoon(int sun, int moon, glm::vec2 offset = glm::vec2(0));

	void toggleFullscreen();
	void nextResolution();
};

