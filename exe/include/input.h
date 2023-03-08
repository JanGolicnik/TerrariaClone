#pragma once
#include <core.h>

enum keyState {
	DOWN,
	HELD,
	UP
};

namespace input
{
	extern std::unordered_map<int, keyState> keys;
	extern std::unordered_map<int, keyState> mousebuttons;
	extern int scroll;
	
	extern bool shift;
	extern bool ctrl;
	extern bool alt;
	
	void clear();

	bool pressed(int key);
	bool held(int key);
	bool released(int key);

	bool mousePressed(int button);
	bool mouseHeld(int button);
	bool mouseReleased(int button);

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void mousebutton_callback(GLFWwindow* window, int button, int action, int mods);
	void windowsize_callback(GLFWwindow* window, int widht, int height);
};

