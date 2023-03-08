#include "Terraria.h"
#include <Input.h>
#include <utils.h>
#include <textures.h>
#include <BlockManager.h>
#include <animations.h>
#include <resources.h>

#include <StartMenu.h>
#include <MainLoop.h>
#include <Inventory.h>
#include <Player.h>
#include <cam.h>

#include <background.h>
#include <text.h>

#include <map.h>
namespace Terraria {

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
        glfwSetKeyCallback(globals::window, (GLFWkeyfun)Input::key_callback);
        glfwSetScrollCallback(globals::window, (GLFWscrollfun)Input::scroll_callback);
        glfwSetMouseButtonCallback(globals::window, (GLFWmousebuttonfun)Input::mousebutton_callback);
        glfwSetWindowSizeCallback(globals::window, (GLFWwindowsizefun)Input::windowsize_callback);

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
        utils::createFrameBuffer(&globals::lightFB, &globals::lightFBT, { globals::resX, globals::resY });

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

        glBindFramebuffer(GL_FRAMEBUFFER, globals::lightFB);
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
    }
    void endall()
    {
        savesettings();
        ECS::commitQueues();
        UI::destroy();
        Player::clean();
        blocks::clean();
        AnimManager::clean();
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
        glDeleteTextures(1, &globals::broken1Tex);
        glDeleteTextures(1, &globals::broken2Tex);
        glDeleteTextures(1, &globals::broken3Tex);
        background::clear();
        glDeleteFramebuffers(1, &globals::mainFB);
        glDeleteFramebuffers(1, &globals::lightFB);
        glDeleteTextures(1, &globals::mainFBT);
        glDeleteTextures(1, &globals::lightFBT);
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
        Terraria::initGlfw();
        resources::loadTextures();
        resources::registerAssets();
        resources::loadAssets();
        Terraria::initCore();
        Layers::init();
        Player::create();
    }

    void run()
    {
        while (!glfwWindowShouldClose(globals::window)) {
            glfwPollEvents();
            globals::engine->setSoundVolume(globals::volume);
            glfwSetWindowOpacity(globals::window, globals::transparency);
            currResText = std::to_string(globals::resX) + "x" + std::to_string(globals::resY);
            switch (currScene) {
                case STARTMENU:
                    StartMenu::run();
                    break;
                case GAME:
                    MainLoop::run();
                    break;
            }
            if (swapTo != NONE) {
                initBase();
                switch (swapTo) {
                    case STARTMENU:
                        StartMenu::init();
                        break;
                    case GAME:
                        MainLoop::init();
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
        AnimManager::runningAnims.clear();

        drawSystem::mainDrawable = ECS::newEntity();
        ECS::addComponent<drawC>(drawSystem::mainDrawable, { nullptr, "", glm::mat4(0), glm::vec2(0) });

        drawC dc;
        dc.hidden = true;
        dc.parent = drawSystem::mainDrawable;
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

        uiSystem::body = ECS::newEntity();
        uiC bodyc;
        bodyc.func = ui_BODY;
        drawC bodydrawC;
        bodydrawC.hidden = true;
        bodydrawC.parent = drawSystem::mainDrawable;
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

    void drawOverlays()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glUseProgram(globals::displayShaderID);
        glBindVertexArray(globals::overlayVA);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, globals::mainFBT);
        glUniform1i(5, 0);
        glUniform1f(2, globals::zoom);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
            Input::windowsize_callback(globals::window, mode->width, mode->height);
            globals::fullscreen = true;
        }
    }
    void nextResolution()
    {
        currRes++;
        if (currRes > resolutions.size() - 1) {
            currRes = 0;
        }
        Input::windowsize_callback(globals::window, resolutions[currRes].x, resolutions[currRes].y);
    }
}
