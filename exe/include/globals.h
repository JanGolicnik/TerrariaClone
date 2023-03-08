#pragma once

#include <core.h>
#include <irrKlang/irrKlang.h>

namespace globals {
	extern float gravity;
	extern float friction;
	extern int iframes;
	extern int resX;
	extern int resY;
	extern float zoom;

	extern GLFWwindow* window;
	extern GLFWcursor* cursor;

	extern GLuint blockShaderID;
	extern GLuint spriteShaderID;
	extern GLuint rayCastingShaderID;
	extern GLuint testLightingShaderID;
	extern GLuint displayShaderID;
	extern GLuint backgroundShaderID;
	extern GLuint blurShaderID;
	extern GLuint lightShaderID;
	extern GLuint zoomShaderID;
	extern GLuint bloomShaderID;

	extern GLuint broken1Tex;
	extern GLuint broken2Tex;
	extern GLuint broken3Tex;

	extern glm::vec3 dayclr;
	extern glm::vec3 noonclr;
	extern glm::vec3 eveningclr;
	extern glm::vec3 morningclr;
	extern glm::vec3 nightclr;
	extern const int dayLength;
	extern float cdayTime;

	extern GLuint mainFB;
	extern GLuint mainFBT;
	extern GLuint tmpFB;
	extern GLuint tmpFBT;

	extern glm::vec2 spritesheetWH;

	extern bool doslimemerger;

	extern float blockPlaceSpeed;

	extern int emptyid;

	extern int particleLayer;
	extern int mobLayer;
	extern int projectileLayer;
	extern int uiLayer;
	extern int toolLayer;
	extern int itemLayer;
	extern int topparticleLayer;

	extern glm::vec2 fullScale;

	extern float maxLightIntensity;

	extern const int blocksizepx;

	extern const OverlayVertex overlayVert[4];
	extern const int32_t overlayEl[6];

	extern uint32_t overlayVA;
	extern uint32_t overlayVB;
	extern uint32_t overlayIB;

	extern float cheaterlight;

	extern float bgoffset;
	
	extern float fontSize;
	extern glm::vec3 textHightlightColor;
	extern glm::vec3 textColor;
	extern glm::vec3 itemslotColor;
	extern glm::vec3 selecteditemslotColor;
	extern irrklang::ISoundEngine* engine;
	extern float volume;
	extern float transparency;
	extern bool fullscreen;

	extern unsigned long long time;

	extern glm::vec3 tooltipcolor;

	glm::vec2 mouseBlockCoords(bool round = true, glm::vec2 clampx = glm::vec2(-0.5, 0.5), glm::vec2 clampy = glm::vec2(-0.5, 0.5));
	glm::vec2 mouseBlockCoordsZoomed(bool round = true, glm::vec2 clampx = glm::vec2(-0.5, 0.5), glm::vec2 clampy = glm::vec2(-0.5, 0.5));
	glm::vec2 normMouseCoords();
	glm::vec2 mouseCoords();
}