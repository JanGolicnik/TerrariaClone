#include <globals.h>

#include <layers.h>
#include <camera.h>

namespace globals {

	float gravity = 0.028f;
	int iframes = 10;
	GLuint spriteShaderID;
    GLFWwindow* window;
    GLFWcursor* cursor;
    int resX = 1920 * 0.75;
    int resY = 1080 * 0.75;
    float zoom = 1.0f;

    glm::vec3 dayclr = glm::vec3(1.0f);
    glm::vec3 noonclr = glm::vec3(0.9f, 0.9f, 1.0f);
    glm::vec3 eveningclr = glm::vec3(0.6f, 0.3f, 0.6f);
    glm::vec3 morningclr = glm::vec3(0.6f, 0.45f, 0.45f);
    glm::vec3 nightclr = glm::vec3(0.05f, 0.05f, 0.1f);
    const int dayLength = 3600;
    float cdayTime = 0;

    GLuint mainFB;
    GLuint mainFBT;
    GLuint tmpFB;
    GLuint tmpFBT;

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

    float fontSize = .67f;

    const OverlayVertex overlayVert[4] = {
    {glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
    {glm::vec3(1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
    {glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
    {glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
    };
    const int32_t overlayEl[6] = {
        0, 1, 3,
        0, 2, 3
    };

    uint32_t overlayVA;
    uint32_t overlayVB;
    uint32_t overlayIB;

    float cheaterlight;

    float volume = 0.3;
    float transparency = 1;

    float bgoffset = 0.0f;
    float bgoffsetY = 0.0f;
    glm::vec3 textHightlightColor = glm::vec3(1, 1, 0);
    glm::vec3 textColor = glm::vec3(1,1,1);
    glm::vec3 itemslotColor = glm::vec3(0.1, 0.1, 0.5);
    glm::vec3 selecteditemslotColor = glm::vec3(1,1,0);

    irrklang::ISoundEngine* engine;
    bool fullscreen;

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

    glm::vec2 mouseBlockCoords(bool round, glm::vec2 clampx, glm::vec2 clampy)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);

        y = resY - y;

        //mouse position from -1 to 1
        x = x / resX;
        y = y / resY;
        x = x - 0.5;
        y = y - 0.5;

        //clamp?
        x = glm::clamp((float)x, clampx.x, clampx.y);
        y = glm::clamp((float)y, clampx.x, clampx.y);
        //find out which block on screen we are
        x = Layers::trueBsOnScr.x * x;
        y = Layers::trueBsOnScr.y * y;

        x = -camera::pos.x + x;
        y = -camera::pos.y + y;

        if (round) {
            x = glm::round(x);
            y = glm::round(y);
        }
        return glm::vec2(x, y);
    }
    glm::vec2 mouseBlockCoordsZoomed(bool round, glm::vec2 clampx, glm::vec2 clampy)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);

        y = resY - y;

        //mouse position from -1 to 1
        x = x / resX;
        y = y / resY;
        x = x - 0.5;
        y = y - 0.5;

        //apply zoom
        x /=zoom;
        y /= zoom;

        //clamp?
        x = glm::clamp((float)x, clampx.x, clampx.y);
        y = glm::clamp((float)y, clampx.x, clampx.y);
        //find out which block on screen we are
        x = Layers::trueBsOnScr.x * x;
        y = Layers::trueBsOnScr.y * y;

        x = -camera::pos.x + x;
        y = -camera::pos.y + y;

        if (round) {
            x = glm::round(x);
            y = glm::round(y);
        }
        return glm::vec2(x, y);
    }

    glm::vec2 mouseCoords() {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return glm::vec2(x, y);
    }

    glm::vec2 normMouseCoords() {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        x /= resX;
        x = ((x - 0.5) / zoom) + 0.5;
        x = x * 2 - 1;

        y /= resY;
        y = ((y - 0.5) / zoom) + 0.5;
        y = y * 2 - 1;

        return glm::vec2(x, -y);
    }

}