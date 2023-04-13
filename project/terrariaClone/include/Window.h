#pragma once

#include <core.h>

namespace Window
{
	extern glm::vec2 res;
	extern std::string currResText;
	extern float zoom;
	extern float transparency;
	extern bool fullscreen;
	extern uint32_t overlayVA;
	extern GLuint tmpFBT;

	extern bool bloom;

	void init();
	void drawMain();
	void drawOverlays();
	void changeResolution(glm::vec2 res);
	void nextResolution();
	void clean();
	bool open();
	void changeTransparency(float val);
	void toggleFullscreen();

	void bindTemporaryBuffer(glm::vec4 clearcolor = glm::vec4(1, 1, 1, 0));
	void swapBuffers();

	void close();

	glm::vec2 mouseBlockCoordsGlobal(bool round = true, glm::vec2 clampx = glm::vec2(-0.5, 0.5), glm::vec2 clampy = glm::vec2(-0.5, 0.5));
	glm::vec2 mouseBlockCoords(bool round = true, glm::vec2 clampx = glm::vec2(-0.5, 0.5), glm::vec2 clampy = glm::vec2(-0.5, 0.5));
	glm::vec2 normMouseCoords();
	glm::vec2 mouseCoords();

};

