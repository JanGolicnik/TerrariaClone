#pragma once

#include <core.h>
#include <stb_image/stb_image.h>    

namespace utils {
	std::array<BlockVertex, 4> CreateQuad(float x, float y, glm::vec4 sC, glm::vec3 light = glm::vec3(1.0f), glm::vec2 size = glm::vec2(1.0f));
	std::array<SpriteVertex, 4> CreateSpriteRect(glm::vec3 pos, std::string tex, glm::vec2 size, glm::vec4 color= glm::vec4(1.0f), bool flipX = false, bool flipY = false, bool autocorrect = true);
	std::array<SpriteVertex, 4> CreateSpriteRect(glm::vec3 pos, glm::vec4 sC, glm::vec2 size, glm::vec4 color= glm::vec4(1.0f), bool flipX = false, bool flipY = false, bool autocorrect = true);
	std::array<SpriteVertex, 4> CreateFontRect(glm::vec3 pos, glm::vec4 sC, glm::vec2 size, glm::vec4 color= glm::vec4(1.0f), bool flipX = false, bool flipY = false);
	
	GLuint LoadTexture(const char* path, int channels = 4, bool repeat = false, glm::vec2* size = nullptr);
	GLFWimage LoadImage(const char* path, int channels = 4, bool repeat = false);
	void freeImage(stbi_uc* pixels);
	void createShader(const char* fragPath, const char* vertPath, GLuint* id);
	void createFrameBuffer(GLuint* framebuffer, GLuint* tex, glm::vec2 res, bool nearest = true, bool depth = false);
	void cout2(glm::vec2);

	double approach(float val1, float val2, float step);
	glm::vec2 approach(glm::vec2 val1, glm::vec2 val2, float step);
	glm::vec3 approach(glm::vec3 val1, glm::vec3 val2, float step);
	void rotateSprite(SpriteVertex*, float rotation, glm::vec2 rotpoint, int nverts, bool add);
	void updateSprite(SpriteVertex*, glm::mat4 *mat, int nverts, glm::vec2 offset);
	void moveSprite(SpriteVertex* verts, int numVerts, glm::vec2 offset, bool abs);
	float angleOfVector(glm::vec2 vec);

	float distPointLine(glm::vec2 point, glm::vec2 linestart, glm::vec2 lineend);

	template <typename T> int sign(T val) {
		return (T(0) < val) - (val < T(0));
	}

	glm::vec3 hsvToRgb(glm::vec3 hsv);

	void rotateVecByAngle(glm::vec2* vec, float angle);
	void rotateVecByAngleRad(glm::vec2* vec, float angle);
}
