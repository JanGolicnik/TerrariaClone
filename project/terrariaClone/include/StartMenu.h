#pragma once

#include <componentsystems.h>
#include <core.h>

namespace StartMenu
{
	extern std::vector<std::string> availableWorlds;
	extern std::vector<std::string> availablePlayers;
	extern int worldSelectContainer;
	extern int loadingScreen;
	extern int playerSelectContainer;
	extern std::vector <playerData> playerDataBuffer;

	void init();
	void run();
	void end();

	void refreshWorlds();
	void refreshPlayers();

	void handleInput();

	void openMenu(uiC* p);
	void openMenu(int menu);

};

