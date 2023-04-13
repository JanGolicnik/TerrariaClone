#pragma once

#include <core.h>

namespace globals {
	extern float gravity;
	extern float friction;
	extern int iframes;

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
	extern GLuint waterShaderID;

	extern GLuint broken1Tex;
	extern GLuint broken2Tex;
	extern GLuint broken3Tex;

	extern GLuint liquidTex;

	extern glm::vec3 dayclr;
	extern glm::vec3 noonclr;
	extern glm::vec3 eveningclr;
	extern glm::vec3 morningclr;
	extern glm::vec3 nightclr;
	extern const int dayLength;
	extern float cdayTime;

	extern glm::vec2 spritesheetWH;

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

	extern float cheaterlight;

	extern float bgoffset;
	extern float bgoffsetY;
	
	extern float fontSize;
	extern glm::vec3 textHightlightColor;
	extern glm::vec3 textColor;
	extern glm::vec3 itemslotColor;
	extern glm::vec3 shopitemslotColor;
	extern glm::vec3 selecteditemslotColor;
	extern glm::vec3 playerBackColor;

	extern unsigned long long time;

	extern glm::vec3 tooltipcolor;

	extern bool pickuptext;
	extern bool tilegrid;
	extern bool hovertext;
	extern bool particles;
	extern float cursorhue;
	extern float cursorborderhue;
	extern float cursorsaturation;
	extern float cursorbordersaturation;
	extern float cursorvalue;
	extern float cursorbordervalue;
	extern float cursorsize;

	extern glm::vec3 backcolor1;
	extern glm::vec3 backcolor2;
	extern glm::vec3 backcolor3;
	extern glm::vec3 buttonhovercolor;
	extern glm::vec3 buttonborderhovercolor;
	extern glm::vec3 blackcolor;
	extern glm::vec3 keybindcolor;

	extern int waterid;

	extern bool background;

}