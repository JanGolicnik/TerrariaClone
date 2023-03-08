#pragma once

#include <core.h>

namespace background
{
	struct bg {
		GLuint front;
		GLuint mid;
		GLuint back;
		glm::vec2 res;
	};
	extern std::unordered_map<int, bg> bgs;
	extern std::unordered_map<std::string, int> bgNames;
	extern float biomeDiff;
	extern std::string currBg;
	extern std::string swapTo;

	void load();
	void render();
	void renderOne(int id);
	void renderTwo(int id1, int id2, float mixfactor);
	void clear();
};

