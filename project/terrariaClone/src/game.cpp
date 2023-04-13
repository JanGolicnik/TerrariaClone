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
#include <sounds.h>
#include <liquids.h>
#include <Window.h>
#include <droppedItemSystem.h>
#include <aiSystem.h>
#include <mobSystem.h>
#include <particleEmmiterSystem.h>

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

    void initCore()
    {
        physSys = ECS::registerSystem<physicsSystem, physicsC>();
        drawSys = ECS::registerSystem<drawSystem, drawC>();
        aiSys = ECS::registerSystem<aiSystem, aiC>();
        toolSys = ECS::registerSystem<toolSystem, toolC>();
        droppedItemSys = ECS::registerSystem<droppedItemSystem, droppedItemC>();
        uiSys = ECS::registerSystem<uiSystem, uiC>();
        mobSys = ECS::registerSystem<mobSystem, mobC>();
        particleESys = ECS::registerSystem<particleEmmiterSystem, particleEmmiterC>();
        text::setup();
        globals::fullScale = glm::vec2(globals::blocksizepx) / Window::res;
        
        globals::emptyid = blocks::nameToID["empty"];

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
        Window::clean();
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
        glDeleteTextures(1, &textures::spriteSheet);
        background::clear();
        glfwTerminate();
    }
    void init()
    {
        if(!loadsettings()) {
            input::resetMap();
        }
        Window::init();
        resources::loadTextures();
        resources::registerAssets();
        resources::loadAssets();
        initCore();
        Layers::init();
        Player::create();
        sounds::init();
    }
    void run()
    {
        static int ltime = 0;
        static int ctime = 0;
        ltime = ctime;
        ctime = glfwGetTime();
        while (Window::open()) {
            glfwPollEvents();

            sounds::music->setSoundVolume(sounds::musicvolume * sounds::mastervolume);
            sounds::sounds->setSoundVolume(sounds::soundsvolume * sounds::mastervolume);

            Window::currResText = std::to_string(Window::res.x) + "x" + std::to_string(Window::res.y);

            Window::changeTransparency(Window::transparency);

            if (currScene == STARTMENU) StartMenu::run();
            else if (currScene == GAMELOOP) gameLoop::run();

            if (swapTo != NONE) {
                currScene = swapTo;
                initBase();
                if (currScene == STARTMENU) StartMenu::init();
                else if(currScene == GAMELOOP) gameLoop::init();
                swapTo = NONE;
            }
        }
    }
    void initBase()
    {
        ECS::empty();
        Layers::clean();
        liquids::clean();
        animations::clearRunning();

        drawSystem::behindBackground = ECS::newEntity();
        ECS::addComponent<drawC>(drawSystem::behindBackground, { std::make_shared<glm::vec2>(glm::vec2(0)), "", glm::mat4(0), glm::vec2(0) });
        drawSystem::behindBlocks = ECS::newEntity();
        ECS::addComponent<drawC>(drawSystem::behindBlocks, { std::make_shared<glm::vec2>(glm::vec2(0)), "", glm::mat4(0), glm::vec2(0) });
        drawSystem::mainDrawable = ECS::newEntity();
        ECS::addComponent<drawC>(drawSystem::mainDrawable, { std::make_shared<glm::vec2>(glm::vec2(0)), "", glm::mat4(0), glm::vec2(0) });
        drawSystem::front = ECS::newEntity();
        ECS::addComponent<drawC>(drawSystem::front, { std::make_shared<glm::vec2>(glm::vec2(0)), "", glm::mat4(0), glm::vec2(0) });

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
    }
    void savesettings()
    {
        std::string filename = "settings.bak";
        remove(filename.c_str());
        std::ofstream file(filename, std::ios::out | std::ios::binary);
        if (!file) { std::cout << "error opening settings for saving\n"; return; }

        file.write((char*)&sounds::mastervolume, sizeof(sounds::mastervolume));
        file.write((char*)&sounds::musicvolume, sizeof(sounds::musicvolume));
        file.write((char*)&sounds::soundsvolume, sizeof(sounds::soundsvolume));
        file.write((char*)&Window::transparency, sizeof(Window::transparency));
        file.write((char*)&Window::res, sizeof(Window::res));
        file.write((char*)&Window::fullscreen, sizeof(Window::fullscreen));
        file.write((char*)&Window::zoom, sizeof(Window::zoom));
        file.write((char*)&globals::pickuptext, sizeof(globals::pickuptext));
        file.write((char*)&globals::tilegrid, sizeof(globals::tilegrid));
        file.write((char*)&globals::hovertext, sizeof(globals::hovertext));
        file.write((char*)&globals::particles, sizeof(globals::particles));
        file.write((char*)&globals::cursorhue, sizeof(globals::cursorhue));
        file.write((char*)&globals::cursorsaturation, sizeof(globals::cursorsaturation));
        file.write((char*)&globals::cursorvalue, sizeof(globals::cursorvalue));
        file.write((char*)&globals::cursorborderhue, sizeof(globals::cursorborderhue));
        file.write((char*)&globals::cursorbordersaturation, sizeof(globals::cursorbordersaturation));
        file.write((char*)&globals::cursorbordervalue, sizeof(globals::cursorbordervalue));
        file.write((char*)&globals::cursorsize, sizeof(globals::cursorsize));
        file.write((char*)&globals::background, sizeof(globals::background));

        input::saveMap(&file);

        file.close();
    }
    bool loadsettings()
    {
        std::string filename = "settings.bak";
        std::ifstream file(filename, std::ios::in | std::ios::binary);
        if (!file) { std::cout << "error opening settings for leagind\n"; return false; }

        file.read((char*)&sounds::mastervolume, sizeof(sounds::mastervolume));
        file.read((char*)&sounds::musicvolume, sizeof(sounds::musicvolume));
        file.read((char*)&sounds::soundsvolume, sizeof(sounds::soundsvolume));
        file.read((char*)&Window::transparency, sizeof(Window::transparency));
        file.read((char*)&Window::res, sizeof(Window::res));
        file.read((char*)&Window::fullscreen, sizeof(Window::fullscreen));
        file.read((char*)&Window::zoom, sizeof(Window::zoom));
        file.read((char*)&globals::pickuptext, sizeof(globals::pickuptext));
        file.read((char*)&globals::tilegrid, sizeof(globals::tilegrid));
        file.read((char*)&globals::hovertext, sizeof(globals::hovertext));
        file.read((char*)&globals::particles, sizeof(globals::particles));
        file.read((char*)&globals::cursorhue, sizeof(globals::cursorhue));
        file.read((char*)&globals::cursorsaturation, sizeof(globals::cursorsaturation));
        file.read((char*)&globals::cursorvalue, sizeof(globals::cursorvalue));
        file.read((char*)&globals::cursorborderhue, sizeof(globals::cursorborderhue));
        file.read((char*)&globals::cursorbordersaturation, sizeof(globals::cursorbordersaturation));
        file.read((char*)&globals::cursorbordervalue, sizeof(globals::cursorbordervalue));
        file.read((char*)&globals::cursorsize, sizeof(globals::cursorsize));
        file.read((char*)&globals::background, sizeof(globals::background));

        input::loadMap(&file);

        file.close();

        return true;
    }
    void changeSceneTo(Scene scene)
    {
        swapTo = scene;
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

        if (input::held(k_PRIMARY)) {
            glm::vec2 mc = Window::mouseBlockCoordsGlobal(false);
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
    void resChanged() {
        globals::fullScale = glm::vec2(globals::blocksizepx) / Window::res;
        Layers::reschanged();
        StartMenu::shouldRefreshPlayers = true;
        StartMenu::shouldRefreshWorlds = true;
    }
}
