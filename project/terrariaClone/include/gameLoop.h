#pragma once

#include <core.h>
#include <componentsystems.h>

namespace gameLoop {

	extern bool newMap;
	extern int inventoryC;
	extern int tooltip;
	extern int crafting;
	extern int cursoritem;
	extern int chestInvC;
	extern int equipmentC;
	extern int hpbarC;
	extern int breathbar;
	extern int NPCDialogue;
	extern int buffcontainer;
	extern int pickuptextcontainer;
	extern int zoomedcursoritem;
	extern int droppedCoinsText;
	extern int guideslot;

	void init();
	void run();
	void end();

	void createUI();
	void createSettingsMenu();

	void respawned();

	void handleInput();

	void openDialogue(std::string npc);

	void toggleSettings();
	void openMenu(int menu);
}