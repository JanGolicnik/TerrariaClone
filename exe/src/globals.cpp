#include <globals.h>

#include <layers.h>
#include <camera.h>
#include <Window.h>

namespace globals {

	float gravity = 0.028f;
	int iframes = 10;

    glm::vec3 dayclr = glm::vec3(1.0f);
    glm::vec3 noonclr = glm::vec3(0.9f, 0.9f, 1.0f);
    glm::vec3 eveningclr = glm::vec3(0.6f, 0.3f, 0.6f);
    glm::vec3 morningclr = glm::vec3(0.6f, 0.45f, 0.45f);
    glm::vec3 nightclr = glm::vec3(0.05f, 0.05f, 0.1f);
    const int dayLength = 3600;
    float cdayTime = 0;

    GLuint spriteShaderID;
    GLuint blockShaderID;
    GLuint rayCastingShaderID;
    GLuint testLightingShaderID;
    GLuint displayShaderID;
    GLuint backgroundShaderID;
    GLuint blurShaderID;
    GLuint lightShaderID;
    GLuint zoomShaderID;
    GLuint bloomShaderID;
    GLuint waterShaderID;

    GLuint broken1Tex;
    GLuint broken2Tex;
    GLuint broken3Tex;

    GLuint liquidTex;

    bool doslimemerger = false;

    glm::vec2 spritesheetWH = glm::vec2(900,900);

    float friction = 1.07f;
    float blockPlaceSpeed = 20;

    int emptyid;

    int particleLayer;
    int mobLayer;
    int projectileLayer;
    int uiLayer;
    int toolLayer;
    int itemLayer;
    int topparticleLayer;

    glm::vec2 fullScale;

    float maxLightIntensity = 123131323315.0f;

    const int blocksizepx = 32;

    float fontSize = .18f;

    float cheaterlight;

    float bgoffset = 0.0f;
    float bgoffsetY = 0.0f;
    glm::vec3 textHightlightColor = glm::vec3(1, 1, 0);
    glm::vec3 textColor = glm::vec3(1,1,1);
    glm::vec3 itemslotColor = glm::vec3(0.1, 0.1, 0.5);
    glm::vec3 shopitemslotColor = glm::vec3(0.5, 0.5, 0.1);
    glm::vec3 selecteditemslotColor = glm::vec3(1,1,0);
    glm::vec3 playerBackColor = glm::vec3(0.65, 0.65, 0.67);

    unsigned long long time;
    
    bool lag = false;

    bool pickuptext = true;
    bool tilegrid = true;
    bool hovertext = true;
    bool particles = true;

    float cursorhue = 360;
    float cursorborderhue = 360;
    float cursorsaturation = 1;
    float cursorbordersaturation = 1;
    float cursorvalue = 1;
    float cursorbordervalue = 1;
    float cursorsize = 1;
    glm::vec3 tooltipcolor = glm::vec3(0.7, 0.7, 0.7);

    glm::vec3 backcolor2 = glm::vec3(0.0, 1, 0.2);
    glm::vec3 backcolor3 = glm::vec3(0.7);
    glm::vec3 buttonhovercolor = glm::vec3(0.4);
    glm::vec3 buttonborderhovercolor = glm::vec3(1, 1, 0);
    glm::vec3 blackcolor = glm::vec3(0);
    glm::vec3 keybindcolor = glm::vec3(0,0.25,0.045);

    int waterid;

    bool background = true;
}