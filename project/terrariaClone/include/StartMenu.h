#pragma once

#include <componentsystems.h>

namespace StartMenu
{
	extern std::vector<std::string> availableWorlds;
	extern std::vector<std::string> availablePlayers;
	extern int worldSelectContainer;
	extern int loadingScreen;
	extern int playerSelectContainer;

	void init();
	void run();
	void end();

	void refreshWorlds();
	void refreshPlayers();

	void handleInput();

	void openMenu(uiC* p);
	void openMenu(int menu);

};

