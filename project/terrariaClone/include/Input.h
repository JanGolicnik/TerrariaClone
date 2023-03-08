#pragma once
#include <core.h>
#include <fstream>

enum keyState {
	DOWN,
	HELD,
	UP
};

enum interactionKey {
	k_UP,
	k_LEFT,
	k_RIGHT,
	k_DOWN,
	k_JUMP,
	k_PRIMARY,
	k_SECONDARY,
	k_MIDDLE,
	k_MOUSE4,
	k_MOUSE5,
	k_HEAL,
	k_MANA,
	k_INVENTORY,
	k_1,
	k_2,
	k_3,
	k_4,
	k_5,
	k_6,
	k_7,
	k_8,
	k_9,
	k_0,
	k_THROW,
	k_ZOOMIN,
	k_ZOOMOUT
};



namespace input
{
	extern std::unordered_map<int, std::string> keyNames;

	extern std::unordered_map<int, keyState> keys;
	extern std::unordered_map<interactionKey, int> map;
	extern int scroll;
	
	extern bool shift;
	extern bool ctrl;
	extern bool alt;
	

	void clear();

	bool pressed(interactionKey key);
	bool held(interactionKey  key);
	bool released(interactionKey key);

	void captureNextKey(std::function<void(int)>func);
	bool rpressed(int key);
	bool rheld(int key);
	bool rreleased(int key);

	void resetMap();
	void remapKey(interactionKey, int key);

	void saveMap(std::ofstream* file);
	void loadMap(std::ifstream *file);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void mousebutton_callback(GLFWwindow* window, int button, int action, int mods);
	void windowsize_callback(GLFWwindow* window, int widht, int height);
};

