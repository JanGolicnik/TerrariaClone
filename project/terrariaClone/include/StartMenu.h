#pragma once

#include <core.h>
#include <uiSystem.h>

namespace StartMenu
{
	extern std::vector<playerData> playerDataBuffer;
	extern std::vector<worldData> worldDataBuffer;
	extern int worldSelectContainer;
	extern int loadingScreen;
	extern int playerSelectContainer;
	extern std::vector<std::pair<glm::vec2, int>> renderPlayerQueue;
	extern bool shouldRefreshPlayers;
	extern bool shouldRefreshWorlds;

	void init();
	void run();
	void end();

	void refreshWorlds();
	void refreshPlayers();

	void handleInput();

	void nextPlayerPage(uiC* p);
	void prevPlayerPage(uiC* p);

	void nextWorldPage(uiC* p);
	void prevWorldPage(uiC* p);

	void openMenu(uiC* p);
	void openMenu(int menu);
	
	void updateSunAndMoon();
};

