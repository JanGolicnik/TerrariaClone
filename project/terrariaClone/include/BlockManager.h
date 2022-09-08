#pragma once
#include <core.h>

namespace BlockM
{
	extern const int ss;
	extern const int w;
	extern const int h;

	extern std::vector<glm::vec3> lights;
	extern std::unordered_map<const char*, glm::vec4> textures;
	extern std::unordered_map<const char*, BlockInfo> info;
	extern GLuint spriteSheet;

	 void addTex(const char* name, glm::vec4 coords);
	 glm::vec4 getAdjustedCoords(const char* name);
	 glm::vec4 adjustCoords(glm::vec4 coords);
	 const char* getBFromCoords(glm::vec4 coords, bool adjust = false);
	 const char* getBFromCoords(glm::vec2 coords, bool adjust = false);
};

