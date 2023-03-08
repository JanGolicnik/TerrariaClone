#include "startMenu.h"

#include <filesystem>

#include <core.h>
#include <globals.h>
#include <ui.h>
#include <game.h>
#include <player.h>
#include <animations.h>
#include <world.h>
#include <input.h>
#include <camera.h>
#include <background.h>
#include <utils.h>
#include <world.h>

namespace StartMenu {
    int worldNameDisplay;
    int playerNameDisplay;

    int mainSelectContainer;
    
    int settingsContainer;

    int worldSelectContainer;
    int worldCreateContainer;
    int worldsContainer;

    int playerSelectContainer;
    int playerCreateContainer;
    int playersContainer;

    int loadingScreen;
    int progressText;

    int cursor;

    int sun;
    int moon;

    std::vector<std::string> availableWorlds;
    std::vector<std::string> availablePlayers;

    void init()
    {
        globals::bgoffset = 0;
        camera::pos = glm::vec2(0.0f);

        mainSelectContainer = ECS::newEntity();
        UI::addElement(mainSelectContainer, ui_EMPTY, { 0,0 }, { 0,0 }, uiSystem::body);
        worldSelectContainer = ECS::newEntity();
        UI::addElement(worldSelectContainer, ui_EMPTY, { 0,0 }, { 0,0 }, uiSystem::body);
        worldCreateContainer = ECS::newEntity();
        UI::addElement(worldCreateContainer, ui_EMPTY, { 0,0 }, { 0,0 }, uiSystem::body);
        worldsContainer = ECS::newEntity();
        UI::addElement(worldsContainer, ui_EMPTY, { 0,0 }, { 0,0 }, worldSelectContainer);
        playerSelectContainer = ECS::newEntity();
        UI::addElement(playerSelectContainer, ui_EMPTY, { 0,0 }, { 0,0 }, uiSystem::body);
        playerCreateContainer = ECS::newEntity();
        UI::addElement(playerCreateContainer, ui_EMPTY, { 0,0 }, { 0,0 }, uiSystem::body);
        playersContainer = ECS::newEntity();
        UI::addElement(playersContainer, ui_EMPTY, { 0,0 }, { 0,0 }, playerSelectContainer);
        loadingScreen = ECS::newEntity();
        UI::addElement(loadingScreen, ui_EMPTY, { 0,0 }, { 0,0 }, playerSelectContainer);
        settingsContainer = ECS::newEntity();
        UI::addElement(settingsContainer, ui_EMPTY, { 0,0 }, { 0,0 }, uiSystem::body);

        //-----------------------------------mainselect------------------------------------------
        UI::addElement(ECS::newEntity(), ui_DISPLAY, { 0,-350 }, { 1200, 1200 }, mainSelectContainer, { }, { {"tex","logo"} }, false, anchorTOP);

        uiStat func; func.funcp = openMenu;
        uiStat textSize; textSize.floatVal = globals::fontSize * 2;
        uiStat fitText; fitText.boolVal = true;
        uiStat padding;  padding.floatVal = 40;
        uiStat menu; menu.intVal = playerSelectContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, 100 }, { 200, 200 }, mainSelectContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding}, {"menu", menu} }, { {"tex", "empty"}, {"text", "S\\c180100100tart"} }, false, anchorMID);

        menu.intVal = settingsContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, -50 }, { 200, 200 }, mainSelectContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding}, {"menu", menu} }, { {"tex", "empty"}, {"text", "Settings"} }, false, anchorMID);

        func; func.funcp = UI::uiCfunc_exit;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, -200 }, { 150, 150 }, mainSelectContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding} }, { {"tex", "empty"}, {"text", "Exit"} }, false, anchorMID);

        //-----------------------------------mainselect------------------------------------------

        //-----------------------------------settings------------------------------------------
        UI::addElement(ECS::newEntity(), ui_DISPLAY, { 0,-350 }, { 1200, 1200 }, settingsContainer, { }, { {"tex","logo"} }, true, anchorTOP);

        menu.intVal = mainSelectContainer;
        func; func.funcp = openMenu;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, -300 }, { 200, 200 }, settingsContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding}, {"menu", menu} }, { {"tex", "empty"}, {"text", "Back"} }, true, anchorMID);

        uiStat ref; ref.floatValp = &globals::cheaterlight;
        uiStat max; max.floatVal = 1.0f;
        uiStat roundAt; roundAt.floatVal = 0.001;
        textSize; textSize.floatVal = 1.0f;
        roundAt.floatVal = 0.04f;
        ref.floatValp = &globals::volume;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { 0, 100 }, { 300,30 }, settingsContainer, { {"textSize", textSize}, {"ref", ref}, {"max", max}, {"roundAt", roundAt} }, { {"label", "veolumes"} }, true);

        uiStat min; min.floatVal = 0.5f;
        ref.floatValp = &globals::transparency;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { 0, 50 }, { 300,30 }, settingsContainer, { {"textSize", textSize}, {"ref", ref}, {"max", max}, {"min", min}, {"roundAt", roundAt} }, { {"label", "magik"} }, true);

        func; func.funcp = UI::uiCfunc_toggleFullscreen;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, 0 }, { 200, 200 }, settingsContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding}, {"menu", menu} }, { {"tex", "empty"}, {"text", "Toggle fullscreen"} }, true, anchorMID);

        func; func.funcp = UI::uiCfunc_nextResolution;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, -150 }, { 200, 200 }, settingsContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding}, {"menu", menu}, {"textp", {.stringp = &game::currResText }} }, { {"tex", "empty"}, {"text", ""} }, true, anchorMID);

        //-----------------------------------settings------------------------------------------


        //-----------------------------------playerselect----------------------------------------

        refreshPlayers();

        padding.floatVal = 40;
        func.funcp = openMenu;
        menu.intVal = playerCreateContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 300, 100 }, { 200, 200 }, playerSelectContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding}, {"menu", menu} }, { {"tex", "empty"}, {"text", "New"} }, true, anchorBOT);

        func.funcp = openMenu;
        menu.intVal = mainSelectContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { -300, 100 }, { 200, 200 }, playerSelectContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding}, {"menu", menu} }, { {"tex", "empty"}, {"text", "Back"} }, true, anchorBOT);

        //-----------------------------------playerselect----------------------------------------

        //-----------------------------------playercreate----------------------------------------
        playerNameDisplay = ECS::newEntity();
        uiStat centered; centered.boolVal = true;
        UI::addElement(playerNameDisplay, ui_TEXT, { 0, 50 }, { 1,1 }, playerCreateContainer, { {"centered", centered}}, {{"text", ""}}, true, anchorMID);

        ref; ref.stringp = &Player::name;
        func.funcp = UI::uiCfunc_captureInput;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, 250 }, { 200,200 }, playerCreateContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding}, {"ref", ref} }, { {"tex", "empty"}, {"text", "player name"} }, true, anchorMID);

        func.funcp = UI::uiCfunc_createPlayer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, -150 }, { 200, 200 }, playerCreateContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding} }, { {"tex", "empty"}, {"text", "Create"} }, true, anchorMID);

        func.funcp = openMenu;
        menu.intVal = playerSelectContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, -250 }, { 200, 200 }, playerCreateContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding}, {"menu", menu} }, { {"tex", "empty"}, {"text", "Back"} }, true, anchorMID);

        //-----------------------------------playercreate----------------------------------------
        
        //-----------------------------------worldselect-----------------------------------------

        refreshWorlds();

        padding.floatVal = 40;
        func.funcp = openMenu;
        menu.intVal = worldCreateContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 300, 100 }, { 200, 200 }, worldSelectContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding}, {"menu", menu}}, {{"tex", "empty"}, {"text", "New"}}, true, anchorBOT);

        func.funcp = openMenu;
        menu.intVal = playerSelectContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { -300, 100 }, { 200, 200 }, worldSelectContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding}, {"menu", menu} }, { {"tex", "empty"}, {"text", "Back"} }, true, anchorBOT);

        //-----------------------------------worldselect----------------------------------------

        //-----------------------------------worldcreate----------------------------------------
        worldNameDisplay = ECS::newEntity();
        UI::addElement(worldNameDisplay, ui_TEXT, { 0, 50 }, { 1,1 }, worldCreateContainer, { {"centered", centered}, {"textSize", textSize} }, { {"text", ""} }, true, anchorMID);

        ref; ref.stringp = &map::name;
        func.funcp = UI::uiCfunc_captureInput;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, 250 }, { 200,200 }, worldCreateContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding}, {"ref", ref} }, { {"tex", "empty"}, {"text", "world name"} }, true, anchorMID);

        max; max.intVal = 5000;
        roundAt; roundAt.intVal = 30;
        textSize; textSize.floatVal = 1.0f;
        ref.intValp = &map::mapX;
        UI::addElement(ECS::newEntity(), ui_DRAGINT, { 0,350 }, { 300,30 }, worldCreateContainer, {  {"textSize", textSize}, {"ref", ref}, {"max", max}, {"roundAt", roundAt} }, { {"label", "mapX"} }, true);
        ref; ref.intValp = &map::mapY;
        UI::addElement(ECS::newEntity(), ui_DRAGINT, { 0,400 }, { 300,30 }, worldCreateContainer, {  {"textSize", textSize}, {"ref", ref}, {"max", max}, {"roundAt", roundAt} }, { {"label", "mapy"} }, true);
        
        func.funcp = UI::uiCfunc_createWorld;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, -150 }, { 200, 200 }, worldCreateContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding} }, { {"tex", "empty"}, {"text", "Create"} }, true, anchorMID);

        func.funcp = openMenu;
        menu.intVal = worldSelectContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, -250 }, { 200, 200 }, worldCreateContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding}, {"menu", menu} }, { {"tex", "empty"}, {"text", "Back"} }, true, anchorMID);

        //-----------------------------------worldcreate----------------------------------------

        //-----------------------------------loadingscreen----------------------------------------
        progressText = ECS::newEntity();
        UI::addElement(progressText, ui_TEXT, { 0,0 }, { 3,3 }, loadingScreen, { {"centered", centered}, {"textsize", {.floatVal = globals::fontSize}} }, { {"text", ""} }, true, anchorMID);
        //-----------------------------------loadingscreen----------------------------------------

        cursor = ECS::newEntity();
        UI::addElement(cursor, ui_CURSOR, { 0,0 }, { 1,1 }, uiSystem::body, {}, {}, false, anchorNONE);

        sun = ECS::newEntity();
        drawC dc;
        dc.position = std::make_shared<glm::vec2>(glm::vec2(0, 0));
        dc.size = glm::vec2(5, 5);
        dc.hasmat = true;
        dc.tex = "sun";
        dc.parent = game::drawSys->behindBackground;
        game::drawSys->addComponent(sun, &dc);
        moon = ECS::newEntity();
        dc.position = std::make_shared<glm::vec2>(glm::vec2(0, 0));
        dc.tex = "moon";
        game::drawSys->addComponent(moon, &dc);
    }

	void run()
	{
        static int dayRate = 120;
        static int maxFps = 120;
        static double ctime = 0;
        static double ltime = 0;
        static double dtime = 0;
        static double ldayutime = 0;
        static double dutime = 0;
        static double daydtime = 0;

        ctime = glfwGetTime();
        dtime = ctime - ltime;
        daydtime = ctime - ldayutime;

        if (daydtime > 1.0f / dayRate) {

            globals::cdayTime++;
            if (globals::cdayTime > globals::dayLength) {
                globals::cdayTime = 0;
            }

            if ((globals::cdayTime > 3375 && globals::cdayTime < 3600) || (globals::cdayTime < 225 && globals::cdayTime>0)) {
                globals::dayclr += utils::approach(globals::dayclr, globals::morningclr, 225);
            }

            if (globals::cdayTime > 225 && globals::cdayTime < 1575) {
                globals::dayclr += utils::approach(globals::dayclr, globals::noonclr, 225);
            }

            if (globals::cdayTime > 1575 && globals::cdayTime < 2025) {
                globals::dayclr += utils::approach(globals::dayclr, globals::eveningclr, 225);
            }

            if (globals::cdayTime > 2025 && globals::cdayTime < 3375) {
                globals::dayclr += utils::approach(globals::dayclr, globals::nightclr, 225);
            }

            if (globals::cdayTime == 0) {
                map::moonphase++;
            }

            ldayutime = ctime;
        }

        if (dtime > 1.0f / maxFps) {
            globals::time++;
            handleInput();
            UI::setTStat(ECS::getComponent<uiC>(worldNameDisplay), "text", map::name);
            UI::setTStat(ECS::getComponent<uiC>(playerNameDisplay), "text", Player::name);
            input::clear();
            camera::updateNormal();

            glBindFramebuffer(GL_FRAMEBUFFER, globals::tmpFB);
            glClearColor(globals::dayclr.r, globals::dayclr.g, globals::dayclr.b, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            game::updateSunAndMoon(sun, moon);

            game::drawSys->UpdateBehindBackground();
            background::renderOne(background::bgNames["forest"]);

            game::uiSys->Update();
            game::drawSys->Update();

            game::drawMain();

            glBindFramebuffer(GL_FRAMEBUFFER, globals::tmpFB);
            glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            game::drawSys->UpdateFront();

            game::drawOverlays();

            globals::bgoffset+= 0.0002;

            glfwSwapBuffers(globals::window);
            ltime = ctime;

            std::lock_guard<std::mutex> lock(map::worldGenProgress_mutex);
            UI::setTStat(ECS::getComponent<uiC>(progressText), "text", map::worldGenProgress);
            if (map::worldGenProgress == "finished") {
                StartMenu::refreshWorlds();
                map::worldGenProgress = "";
                openMenu(worldSelectContainer);
                map::worldgenthread.join();
            }
        }
	}

	void handleInput()
	{
        if (input::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
            uiSystem::mouseClicked = true;
        }
        if (input::pressed(GLFW_KEY_P)) {
            ECS::print();
        }
	}
    void openMenu(uiC* p)
    {
        UI::captureInput(nullptr, false);
        Player::name.clear();
        UI::hideChildren(ECS::getComponent<uiC>(uiSystem::body), true);
        UI::hideChildren(ECS::getComponent<uiC>(UI::getStat(p, "menu")->intVal), false);
    }

    void openMenu(int menu)
    {
        UI::captureInput(nullptr, false);
        map::name.clear();
        Player::name.clear();
        UI::hideChildren(ECS::getComponent<uiC>(uiSystem::body), true);
        UI::hideChildren(ECS::getComponent<uiC>(menu), false);
    }

    void refreshWorlds()
    {
        uiStat textSize; textSize.floatVal = globals::fontSize * 2;
        uiStat func; func.funcp = UI::uiCfunc_loadSpecificWorld;
        uiStat fitText; fitText.boolVal = true;
        uiStat padding;  padding.floatVal = 40;
        UI::deleteElement(worldsContainer, false);
        ECS::commitQueues();
        std::filesystem::path current_path = std::filesystem::current_path();
        std::filesystem::path target_path = current_path / "worlds";
        int y = -100;
        int i = 0;
        availableWorlds.clear();
        for (const auto& entry : std::filesystem::directory_iterator(target_path)) {
            std::string filename = entry.path().filename().string();
            if (filename.substr(filename.length() - 4, 4) == ".bak") {
                std::string world = filename.substr(0, filename.length() - 4);
                availableWorlds.push_back(world);
                uiStat worldIndex; worldIndex.intVal = i;
                UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, y }, { 200, 200 }, worldsContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding}, {"index", worldIndex} }, { {"tex", "tooltip"}, {"text", world} }, true, anchorTOP);
                i++;
                y -= 200;
            }
        }
    }

    void refreshPlayers()
    {
        uiStat textSize; textSize.floatVal = globals::fontSize * 2;
        uiStat func; func.funcp = UI::uiCfunc_loadSpecificPlayer;
        uiStat fitText; fitText.boolVal = true;
        uiStat padding;  padding.floatVal = 40;
        uiStat menu;  menu.intVal = worldSelectContainer;
        UI::deleteElement(playersContainer, false);
        ECS::commitQueues();
        std::filesystem::path current_path = std::filesystem::current_path();
        std::filesystem::path target_path = current_path / "players";
        int y = -100;
        int i = 0;
        availablePlayers.clear();
        for (const auto& entry : std::filesystem::directory_iterator(target_path)) {
            std::string filename = entry.path().filename().string();
            if (filename.substr(filename.length() - 4, 4) == ".bak") {
                std::string world = filename.substr(0, filename.length() - 4);
                availablePlayers.push_back(world);
                uiStat playerIndex; playerIndex.intVal = i;
                UI::addElement(ECS::newEntity(),  ui_BUTTON, { 0, y }, { 200, 200 }, playersContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding}, {"index", playerIndex}, {"menu", menu} }, { {"tex", "tooltip"}, {"text", world} }, true, anchorTOP);
                i++;
                y -= 200;
            }
        }
    }


}