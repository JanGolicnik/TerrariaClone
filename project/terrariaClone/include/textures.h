#pragma once

#include <core.h>

struct Sprite {
	glm::vec4 coords;
	glm::vec2 size;
};

namespace textures
{
	extern std::unordered_map<std::string, std::shared_ptr<Sprite>> nametocoords;
	extern std::unordered_map<int, std::string> idtoname;
	extern std::unordered_map<std::string, int> nameToID;
	extern std::unordered_map<int, std::shared_ptr<Sprite>> idtocoords;
	extern std::vector<glm::vec4> playerhair;
	extern GLuint spriteSheet;
	extern GLuint playerSheet;
	extern int texidcounter;

	void clean();
};

