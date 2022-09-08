#pragma once

#include <core.h>

namespace utils {
	std::array<BlockVertex, 4> CreateQuad(float x, float y, glm::vec4 sC, float light, glm::vec2 size = glm::vec2(1.0f));
	std::array<SpriteVertex, 4> CreateSpriteRect(float x, float y, glm::vec4 spriteCoords, float sizeX, float sizeY);
	std::array<SpriteVertex, 4> CreateSpriteRect(float x, float y, glm::vec4 spriteCoords, float sizeX, float sizeY, glm::vec2 wh, bool invert = false);
	GLuint LoadTexture(const char* path, int channels = 4, bool repeat = false);
	void createShader(const char* fragPath, const char* vertPath, GLuint* id);
	void createFrameBuffer(GLuint* framebuffer, GLuint* tex, glm::vec2 res, bool nearest = true);
	void cout2(glm::vec2);
	template <typename T>
	void cout(T x);
	template<typename T>
	void cout(T x)
	{
		std::cout << x << "\n";
	}

	float distance(glm::vec2, glm::vec2);
}


