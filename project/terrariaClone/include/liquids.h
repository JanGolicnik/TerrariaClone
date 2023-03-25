#pragma once

#include <core.h>
#include <fstream>
#include <world.h>

namespace liquids
{
	void place(std::string id, glm::vec2 pos, int level);
	void remove(glm::vec2 pos);
	void updateOnScreen();
	void renderOnScreen();
	void level(glm::vec2 pos);
	void add(std::string, unsigned int base, int blockid, glm::vec3 light = glm::vec3(0));
	int at(glm::vec2 pos);
	int levelAt(glm::vec2 pos);
	void save(std::ofstream* file);
	void load(std::ifstream* file);
	void settleAll();
	int settle(glm::vec2 pos);
	int settle(uint32_t c, int lastbounce = -1, int lastlastbounce = -1);
	void clean();
};
