#pragma once

#include <core.h>

namespace background
{
	extern float biomeDiff;
	extern std::string currBg;
	extern std::string swapTo;

	void load();
	void render();
	void renderOne(std::string id, float opacity = 1);
	void clear();
};

