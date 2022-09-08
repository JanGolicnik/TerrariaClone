#include "BlockManager.h"

namespace BlockM {
	std::unordered_map<const char*, glm::vec4> textures;
	GLuint spriteSheet;
	std::unordered_map<const char*, BlockInfo> info;
	std::vector<glm::vec3> lights;
	extern const int ss = 8;
	extern const int w = 16000;
	extern const int h = 32;

	void addTex(const char* name, glm::vec4 c)
	{
		if (textures.count(name) >= 1) {
			textures[name] = c;
		}else
		textures.insert(std::pair<const char*, glm::vec4>(name, c));
	}

	glm::vec4 getAdjustedCoords(const char* name)
	{
		float ssh = (double)ss / h;
		float ssw = (double)ss / w;
		if (name == "empty") {
			return { -1,-1,-1,-1 };
		}
		glm::vec4 a = textures.at(name);
		return { a.r * ssw, a.g * ssh, a.b * ssw, a.a * ssh };
	}

	glm::vec4 adjustCoords(glm::vec4 coords) {

		double ssh = (double)ss / h;
		double ssw = (double)ss / w;
		glm::vec4 c = coords;
		c = { c.x * ssw, c.y * ssh, c.b * ssw, c.a * ssh };
		return c;
	}

	const char* getBFromCoords(glm::vec4 coords, bool adjust)
	{
		double ssh = 1;
		double ssw = 1;
		if (adjust) {
			ssh = (double)ss / h;
			ssw = (double)ss / w;
		}
		coords = { coords.x / ssw, coords.y / ssh, coords.b / ssw, coords.a / ssh };
		for (auto& it : textures) {
			if (it.second == coords) {
				return it.first;
			}
		}
		return {};
	}

	const char* getBFromCoords(glm::vec2 coords, bool adjust)
	{
		double ssh = 1;
		double ssw = 1;
		if (adjust) {
			ssh = (double)ss / h;
			ssw = (double)ss / w;
		}
		coords = { coords.x / ssw, coords.y / ssh };
		glm::vec2 c = glm::vec2(coords.x, coords.y);
		for (auto& it : textures) {
			if (glm::vec2(it.second) == c) {
				return it.first;
			}
		}
		return "empty";
	}
}