#include "game.h"

#include <input.h>
#include <utils.h>
#include <textures.h>
#include <blocks.h>
#include <animations.h>
#include <resources.h>
#include <startMenu.h>
#include <gameLoop.h>
#include <ui.h>
#include <player.h>
#include <camera.h>
#include <background.h>
#include <text.h>
#include <world.h>

namespace game {

    uiSystem* uiSys;
    physicsSystem* physSys;
    drawSystem* drawSys;
    aiSystem* aiSys;
    toolSystem* toolSys;
    droppedItemSystem* droppedItemSys;
    mobSystem* mobSys;
    particleEmmiterSystem* particleESys;

    Scene swapTo = NONE;
    Scene currScene = NONE;

    bool showCursor = true;

    std::vector<glm::vec2> resolutions;
    int currRes = 0;
    std::string currResText;

    void glfwErrorCallback(int error, const char* description)
    {
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
        std::cout << error << " " << description << "\n";
    }
    void initGlfw() {
        srand(time(NULL));
        glfwSetErrorCallback(glfwErrorCallback);
        if (!glfwInit()) {
            std::cout << "failed to init glfw" << std::endl;
        }
        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_RESIZABLE, true);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        globals::window = glfwCreateWindow(globals::resX, globals::resY, "airarret", NULL, NULL);
        if (globals::window == NULL) {
            std::cout << "failed to create window" << std::endl;
            glfwTerminate();
        }
        glfwMakeContextCurrent(globals::window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "failed to init glad" << std::endl;
        }
        glfwSetInputMode(globals::window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        glfwSetKeyCallback(globals::window, (GLFWkeyfun)input::key_callback);
        glfwSetScrollCallback(globals::window, (GLFWscrollfun)input::scroll_callback);
        glfwSetMouseButtonCallback(globals::window, (GLFWmousebuttonfun)input::mousebutton_callback);
        glfwSetWindowSizeCallback(globals::window, (GLFWwindowsizefun)input::windowsize_callback);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
        glfwSetWindowOpacity(globals::window, globals::transparency);
        glfwSetWindowSizeLimits(globals::window, 640, 480, GLFW_DONT_CARE, GLFW_DONT_CARE);
        if (globals::fullscreen) {
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            glfwSetWindowMonitor(globals::window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
        }
    }
    void initCore()
    {
        utils::createFrameBuffer(&globals::mainFB, &globals::mainFBT, { globals::resX, globals::resY }, true);
        utils::createFrameBuffer(&globals::tmpFB, &globals::tmpFBT, { globals::resX, globals::resY });

        glGenVertexArrays(1, &globals::overlayVA);
        glBindVertexArray(globals::overlayVA);

        glGenBuffers(1, &globals::overlayVB);
        glBindBuffer(GL_ARRAY_BUFFER, globals::overlayVB);
        glBufferData(GL_ARRAY_BUFFER, sizeof(globals::overlayVert), globals::overlayVert, GL_STATIC_DRAW);

        glGenBuffers(1, &globals::overlayIB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, globals::overlayIB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(globals::overlayEl), globals::overlayEl, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OverlayVertex), 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(OverlayVertex), (void*)sizeof(glm::vec3));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        glBindFramebuffer(GL_FRAMEBUFFER, globals::tmpFB);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        physSys = ECS::registerSystem<physicsSystem, physicsC>();
        drawSys = ECS::registerSystem<drawSystem, drawC>();
        aiSys = ECS::registerSystem<aiSystem, aiC>();
        toolSys = ECS::registerSystem<toolSystem, toolC>();
        droppedItemSys = ECS::registerSystem<droppedItemSystem, droppedItemC>();
        uiSys = ECS::registerSystem<uiSystem, uiC>();
        mobSys = ECS::registerSystem<mobSystem, mobC>();
        particleESys = ECS::registerSystem<particleEmmiterSystem, particleEmmiterC>();
        text::setup();
        globals::fullScale = glm::vec2(globals::blocksizepx / (float)globals::resX, globals::blocksizepx / (float)globals::resY);
        
        globals::emptyid = blocks::nameToID["empty"];

        currRes = 0;
        resolutions.push_back(glm::vec2(1024, 734));
        resolutions.push_back(glm::vec2(1152, 826));
        resolutions.push_back(glm::vec2(1280, 688));
        resolutions.push_back(glm::vec2(1280, 734));
        resolutions.push_back(glm::vec2(1280, 765));
        resolutions.push_back(glm::vec2(1280, 918));
        resolutions.push_back(glm::vec2(1280, 979));
        resolutions.push_back(glm::vec2(1360, 734));
        resolutions.push_back(glm::vec2(1366, 734));
        resolutions.push_back(glm::vec2(1440, 860));
        resolutions.push_back(glm::vec2(1600, 860));
        resolutions.push_back(glm::vec2(1600, 979));
        resolutions.push_back(glm::vec2(1600, 1147));
        resolutions.push_back(glm::vec2(1680, 1004));
        resolutions.push_back(glm::vec2(1920, 1032));
        resolutions.push_back(glm::vec2(1920, 1147));
        resolutions.push_back(glm::vec2(1920, 1377));
        resolutions.push_back(glm::vec2(2560, 1377));

        globals::time = 0;
    }
    void endall()
    {
        savesettings();
        ECS::commitQueues();
        UI::destroy();
        Player::clean();
        blocks::clean();
        animations::clear();
        items::clean();
        ECS::clean();
        textures::clean();
        Layers::clean();
        glDeleteProgram(globals::blockShaderID);
        glDeleteProgram(globals::spriteShaderID);
        glDeleteProgram(globals::rayCastingShaderID);
        glDeleteProgram(globals::testLightingShaderID);
        glDeleteProgram(globals::displayShaderID);
        glDeleteProgram(globals::backgroundShaderID);
        glDeleteProgram(globals::blurShaderID);
        glDeleteProgram(globals::lightShaderID);
        glDeleteProgram(globals::zoomShaderID);
        glDeleteProgram(globals::bloomShaderID);
        glDeleteTextures(1, &globals::broken1Tex);
        glDeleteTextures(1, &globals::broken2Tex);
        glDeleteTextures(1, &globals::broken3Tex);
        background::clear();
        glDeleteFramebuffers(1, &globals::mainFB);
        glDeleteFramebuffers(1, &globals::tmpFB);
        glDeleteTextures(1, &globals::mainFBT);
        glDeleteTextures(1, &globals::tmpFBT);
        glDeleteTextures(1, &textures::spriteSheet);
        glDeleteBuffers(1, &globals::overlayVA);
        glDeleteBuffers(1, &globals::overlayIB);
        glDeleteBuffers(1, &globals::overlayVB);
        glfwDestroyWindow(globals::window);
        glfwTerminate();
    }

    void init()
    {
        loadsettings();
        game::initGlfw();
        resources::loadTextures();
        resources::registerAssets();
        resources::loadAssets();
        game::initCore();
        Layers::init();
        Player::create();
        globals::engine = irrklang::createIrrKlangDevice();
        if (globals::engine) {
            globals::engine->play2D("tehma123.mp3", true);
            globals::engine->setSoundVolume(globals::volume);
        }
    }

    void run()
    {
        static int ltime = 0;
        static int ctime = 0;
        ltime = ctime;
        ctime = glfwGetTime();
        while (!glfwWindowShouldClose(globals::window)) {
            glfwPollEvents();

            globals::engine->setSoundVolume(globals::volume);
            if (globals::transparency != 1) {
                glfwSetWindowOpacity(globals::window, globals::transparency);
            }
            currResText = std::to_string(globals::resX) + "x" + std::to_string(globals::resY);
            switch (currScene) {
                case STARTMENU:
                    StartMenu::run();
                    break;
                case GAME:
                    gameLoop::run();
                    break;
            }
            if (swapTo != NONE) {
                initBase();
                switch (swapTo) {
                    case STARTMENU:
                        StartMenu::init();
                        break;
                    case GAME:
                        gameLoop::init();
                        break;
                }
                currScene = swapTo;
                swapTo = NONE;
            }
        }
    }

    void initBase()
    {
        ECS::empty();
        Layers::clean();
        animations::clearRunning();

        drawSystem::mainDrawable = ECS::newEntity();
        ECS::addComponent<drawC>(drawSystem::mainDrawable, { nullptr, "", glm::mat4(0), glm::vec2(0) });
        drawSystem::behindBackground = ECS::newEntity();
        ECS::addComponent<drawC>(drawSystem::behindBackground, { nullptr, "", glm::mat4(0), glm::vec2(0) });
        drawSystem::front = ECS::newEntity();
        ECS::addComponent<drawC>(drawSystem::front, { nullptr, "", glm::mat4(0), glm::vec2(0) });

        drawC dc;
        dc.hidden = true;
        dc.parent = drawSystem::mainDrawable;
        globals::particleLayer = ECS::newEntity();
        drawSystem::addComponent(globals::particleLayer, &dc);
        globals::mobLayer = ECS::newEntity();
        drawSystem::addComponent(globals::mobLayer, &dc);
        globals::projectileLayer = ECS::newEntity();
        drawSystem::addComponent(globals::projectileLayer, &dc);
        globals::itemLayer = ECS::newEntity();
        drawSystem::addComponent(globals::itemLayer, &dc);
        globals::toolLayer = ECS::newEntity();
        drawSystem::addComponent(globals::toolLayer, &dc);
        globals::uiLayer = ECS::newEntity();
        drawSystem::addComponent(globals::uiLayer, &dc);
        globals::topparticleLayer = ECS::newEntity();
        drawSystem::addComponent(globals::topparticleLayer, &dc);

        uiSystem::body = ECS::newEntity();
        uiC bodyc;
        bodyc.func = ui_BODY;
        drawC bodydrawC;
        bodydrawC.hidden = true;
        bodydrawC.parent = drawSystem::front;
        ECS::addComponent<uiC>(uiSystem::body, bodyc);
        drawSystem::addComponent(uiSystem::body, &bodydrawC);

        Player::entity = ECS::newEntity();
        physicsC pC;
        pC.position = std::make_shared<glm::vec2>(glm::vec2(0));
        pC.size = glm::vec2(Player::width, Player::height);
        ECS::addComponent<physicsC>(Player::entity, pC);

        map::guideEnt = -1;
        map::guideName = "";
    }

    void savesettings()
    {
        std::string filename = "settings.bak";
        remove(filename.c_str());
        std::ofstream file(filename, std::ios::out | std::ios::binary);
        if (!file) { std::cout << "error opening settings for saving\n"; return; }

        file.write((char*)&globals::volume, sizeof(globals::volume));
        file.write((char*)&globals::transparency, sizeof(globals::transparency));
        file.write((char*)&globals::resX, sizeof(globals::resX));
        file.write((char*)&globals::resY, sizeof(globals::resY));
        file.write((char*)&globals::fullscreen, sizeof(globals::fullscreen));

        file.close();
    }

    void loadsettings()
    {
        std::string filename = "settings.bak";
        std::ifstream file(filename, std::ios::in | std::ios::binary);
        if (!file) { std::cout << "error opening settings for leagind\n"; return; }

        file.read((char*)&globals::volume, sizeof(globals::volume));
        file.read((char*)&globals::transparency, sizeof(globals::transparency));
        file.read((char*)&globals::resX, sizeof(globals::resX));
        file.read((char*)&globals::resY, sizeof(globals::resY));
        file.read((char*)&globals::fullscreen, sizeof(globals::fullscreen));

        file.close();
    }

    void changeSceneTo(Scene scene)
    {
        swapTo = scene;
    }

    void drawMain()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, globals::mainFB);
        glUseProgram(globals::zoomShaderID);
        glBindVertexArray(globals::overlayVA);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, globals::tmpFBT);
        glUniform1i(5, 0);
        glUniform1f(2, globals::zoom);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void drawOverlays()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, globals::mainFB);
        glUseProgram(globals::displayShaderID);
        glBindVertexArray(globals::overlayVA);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, globals::tmpFBT);
        glUniform1i(5, 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glUseProgram(globals::bloomShaderID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, globals::mainFBT);
        glUniform1i(5, 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    void updateSunAndMoon(int sun, int moon, glm::vec2 offset)
    {
        auto sundraw = ECS::getComponent<drawC>(sun);
        auto moondraw = ECS::getComponent<drawC>(moon);
        moondraw->tex = map::moonphases[map::moonphase % map::moonphases.size()];

        float a = int(utils::angleOfVector(glm::normalize(offset + glm::vec2(0, - Layers::blocksOnScreen.y / 2.0f) - *sundraw->position)) - 90) % 360;
        sundraw->mat = glm::rotate(glm::mat4(1.0f), float(a * PI / 180.0f), glm::vec3(0, 0, 1));
        a = int(utils::angleOfVector(glm::normalize(offset + glm::vec2(0, Layers::blocksOnScreen.y / 2.0f) - *sundraw->position)) - 90) % 360;
        moondraw->mat = glm::rotate(glm::mat4(1.0f), float(a * PI / 180.0f), glm::vec3(0, 0, 1));

        if (input::mouseHeld(GLFW_MOUSE_BUTTON_LEFT)) {
            glm::vec2 mc = globals::mouseBlockCoords(false);
            if (glm::distance(mc, *sundraw->position) < 5) {
                globals::cdayTime = (sundraw->position->x - offset.x) / Layers::blocksOnScreen.x + 0.5f;
                globals::cdayTime *= 1800;
                *sundraw->position = offset + mc;
                return;
            }
            if (glm::distance(mc, *moondraw->position) < 5) {
                globals::cdayTime = (moondraw->position->x - offset.x) / Layers::blocksOnScreen.x + 0.5f;
                globals::cdayTime = globals::cdayTime * 1800 + 1800;
                *moondraw->position = offset + mc;
                return;
            }
        }


        //hocmo tko da k je cs 0 da je ig na levi polovici ekrana, pr 900 je na sredini zgorne tretine, pr 1800 pa na desni polovici
        if (globals::cdayTime > 0 && globals::cdayTime < globals::dayLength / 2) {
            glm::vec2 target(0);
            float percent = globals::cdayTime / globals::dayLength * 2;
            target.x = percent * Layers::blocksOnScreen.x - Layers::blocksOnScreen.x / 2.0f;
            target.y = sin(percent * PI) * Layers::blocksOnScreen.y / 3.0f;
            *sundraw->position = offset + target;
        }
        else {
            *sundraw->position = glm::vec2(-123, -123);
        }

        if (globals::cdayTime > globals::dayLength / 2 && globals::cdayTime < globals::dayLength) {
            glm::vec2 target(0);
            float percent = (globals::cdayTime - globals::dayLength / 2) / globals::dayLength * 2;
            target.x = percent * Layers::blocksOnScreen.x - Layers::blocksOnScreen.x / 2.0f;
            target.y = sin(percent * PI) * Layers::blocksOnScreen.y / 3.0f;
            *moondraw->position = offset + target;


        }
        else {
            *moondraw->position = glm::vec2(-123, -123);
        }
    }
    void toggleFullscreen()
    {
        static GLFWmonitor* monitor;
        if (glfwGetWindowMonitor(globals::window)) {
            GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
            int monitorWidth = mode->width;
            int monitorHeight = mode->height;
            int posx = (monitorWidth - resolutions[currRes].x) / 2;
            int posy = (monitorHeight- resolutions[currRes].y) / 2;
            glm::vec2 monitorWH;
            glfwSetWindowMonitor(globals::window, NULL, posx, posy, resolutions[currRes].x, resolutions[currRes].y, GLFW_DONT_CARE);
            globals::fullscreen = false;
        }
        else {
            monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(globals::window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
            input::windowsize_callback(globals::window, mode->width, mode->height);
            globals::fullscreen = true;
        }
    }
    void nextResolution()
    {
        currRes++;
        if (currRes > resolutions.size() - 1) {
            currRes = 0;
        }
        input::windowsize_callback(globals::window, resolutions[currRes].x, resolutions[currRes].y);
    }
}
