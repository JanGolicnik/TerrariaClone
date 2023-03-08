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
    int logo;

    int mainSelectContainer;
    
    int settingsContainer;

    int generalContainer;
    int videoContainer;
    int interfaceContainer;
    int cursorContainer;
    int keybindingsContainer;

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

    int worldsizedisplay;

    std::vector<std::string> availableWorlds;
    std::vector<std::string> availablePlayers;

    void init()
    {
        globals::bgoffset = 0;
        camera::pos = glm::vec2(0.0f);

        logo = ECS::newEntity();
        UI::addElement(logo, ui_LOGO, { 0, -300 }, { 1200, 1200 }, uiSystem::body, { {"autolight", {.boolVal = true}} }, { {"tex","logo"} }, false, anchorTOP);
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
        loadingScreen = ECS::newEntity();
        UI::addElement(loadingScreen, ui_EMPTY, { 0,0 }, { 0,0 }, uiSystem::body);
        settingsContainer = ECS::newEntity();
        UI::addElement(settingsContainer, ui_EMPTY, { 0,0 }, { 0,0 }, uiSystem::body);
        generalContainer = ECS::newEntity();
        UI::addElement(generalContainer, ui_EMPTY, { 0,0 }, { 0,0 }, uiSystem::body);
        videoContainer = ECS::newEntity();
        UI::addElement(videoContainer, ui_EMPTY, { 0,0 }, { 0,0 }, uiSystem::body);
        cursorContainer = ECS::newEntity();
        UI::addElement(cursorContainer, ui_EMPTY, { 0,0 }, { 0,0 }, uiSystem::body);
        interfaceContainer = ECS::newEntity();
        UI::addElement(interfaceContainer, ui_EMPTY, { 0,0 }, { 0,0 }, uiSystem::body);
        keybindingsContainer = ECS::newEntity();
        UI::addElement(keybindingsContainer, ui_EMPTY, { 0,0 }, { 0,0 }, uiSystem::body);


        //-----------------------------------mainselect------------------------------------------

        uiStat func; func.funcp = openMenu;
        uiStat textSize; textSize.floatVal = globals::fontSize * 2;
        uiStat fitText; fitText.boolVal = true;
        uiStat padding;  padding.floatVal = 40;
        uiStat menu; menu.intVal = playerSelectContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, 100 }, { 200, 200 }, mainSelectContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding}, {"menu", menu} }, { {"tex", "empty"}, {"text", "Start"} }, false, anchorMID);

        menu.intVal = settingsContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, -50 }, { 200, 200 }, mainSelectContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding}, {"menu", menu} }, { {"tex", "empty"}, {"text", "Settings"} }, false, anchorMID);

        func; func.funcp = UI::uiCfunc_exit;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, -200 }, { 150, 150 }, mainSelectContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding} }, { {"tex", "empty"}, {"text", "Exit"} }, false, anchorMID);

        //-----------------------------------mainselect------------------------------------------

        //-----------------------------------settings------------------------------------------

        float fontsize = globals::fontSize * 1.6;

        menu.intVal = mainSelectContainer;
        func; func.funcp = openMenu;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, -150 }, { 200, 200 }, settingsContainer, { {"func", func}, {"textSize", {.floatVal = fontsize}}, {"fitText", fitText}, {"padding", padding}, {"menu", menu} }, { {"tex", "empty"}, {"text", "Back"} }, true, anchorMID);

        menu.intVal = generalContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, 350 }, { 200, 200 }, settingsContainer, { {"func", func}, {"textSize", {.floatVal = fontsize}}, {"fitText", fitText}, {"menu", menu} },
            { {"tex", "empty"}, {"text", "General"} }, true, anchorMID);

        menu.intVal = interfaceContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, 250 }, { 200, 200 }, settingsContainer, { {"func", func}, {"textSize", {.floatVal = fontsize}}, {"fitText", fitText}, {"menu", menu} },
            { {"tex", "empty"}, {"text", "Interface"} }, true, anchorMID);

        menu.intVal = videoContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, 150 }, { 200, 200 }, settingsContainer, { {"func", func}, {"textSize", {.floatVal = fontsize}}, {"fitText", fitText}, {"menu", menu} },
            { {"tex", "empty"}, {"text", "Video"} }, true, anchorMID);

        menu.intVal = cursorContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, 50 }, { 200, 200 }, settingsContainer, { {"func", func}, {"textSize", {.floatVal = fontsize}}, {"fitText", fitText}, {"menu", menu} },
            { {"tex", "empty"}, {"text", "Cursor"} }, true, anchorMID);

        menu.intVal = keybindingsContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, -50 }, { 200, 200 }, settingsContainer, { {"func", func}, {"textSize", {.floatVal = fontsize}}, {"fitText", fitText}, {"menu", menu} },
            { {"tex", "empty"}, {"text", "Keybindings"} }, true, anchorMID);
        
        //-----------------------------------settings------------------------------------------

        //-----------------------------------general------------------------------------------

        menu.intVal = settingsContainer;
        func; func.funcp = openMenu;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, -150 }, { 200, 200 }, generalContainer, { {"func", func}, {"textSize", {.floatVal = fontsize}}, {"fitText", fitText}, {"padding", padding}, {"menu", menu} }, { {"tex", "empty"}, {"text", "Back"} }, true, anchorMID);

        uiStat ref;  ref.floatValp = &globals::volume;
        uiStat max; max.floatVal = 1.0f;
        uiStat roundAt; roundAt.floatVal = 0.04f;
        textSize; textSize.floatVal = 1.0f;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { 0, -50 }, { 333,333 / 11 }, generalContainer, { {"textSize", {.floatVal = fontsize}}, {"ref", ref}, {"max", max}, {"roundAt", roundAt} }, { {"tex", "slider"},{"label", "veolumes"} }, true);

        ref.floatValp = &globals::zoom;
        max.floatVal = 5.0f;
        textSize; textSize.floatVal = 1.0f; 
        roundAt.floatVal = 0.04f;
        ref.floatValp = &globals::volume;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { 0, 50 }, { 333,333 / 11 }, generalContainer, { {"textSize", {.floatVal = fontsize}}, {"ref", ref}, {"max", max}, {"roundAt", roundAt} }, { {"tex", "slider"},{"label", "zoom"} }, true);

        //-----------------------------------general------------------------------------------
        
        //-----------------------------------interface------------------------------------------

        menu.intVal = settingsContainer;
        func; func.funcp = openMenu;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, -150 }, { 200, 200 }, interfaceContainer, { {"func", func}, {"textSize", {.floatVal = fontsize}}, {"fitText", fitText}, {"padding", padding}, {"menu", menu} }, { {"tex", "empty"}, {"text", "Back"} }, true, anchorMID);

        UI::addElement(ECS::newEntity(), ui_TOGGLE, { 0, -50 }, { 0, 0 }, interfaceContainer, { {"ref", {.boolValp = &globals::pickuptext}}, {"textSize", {.floatVal = fontsize}} }, { {"label", "pickuptext"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_TOGGLE, { 0, 50 }, { 0, 0 }, interfaceContainer, { {"ref", {.boolValp = &globals::tilegrid}}, {"textSize", {.floatVal = fontsize}} }, { {"label", "tilegrid"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_TOGGLE, { 0, 150 }, { 0, 0 }, interfaceContainer, { {"ref", {.boolValp = &globals::hovertext}}, {"textSize", {.floatVal = fontsize}} }, { {"label", "hovertext"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_TOGGLE, { 0, 250 }, { 0, 0 }, interfaceContainer, { {"ref", {.boolValp = &globals::particles}}, {"textSize", {.floatVal = fontsize}} }, { {"label", "particles"} }, true, anchorMID);

        //-----------------------------------interface------------------------------------------

        //-----------------------------------video------------------------------------------

        menu.intVal = settingsContainer;
        func; func.funcp = openMenu;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, -150 }, { 200, 200 }, videoContainer, { {"func", func}, {"textSize", {.floatVal = fontsize}}, {"fitText", fitText}, {"padding", padding}, {"menu", menu} }, { {"tex", "empty"}, {"text", "Back"} }, true, anchorMID);

        uiStat min; min.floatVal = 0.5f;
        max.floatVal = 1.0f;
        ref.floatValp = &globals::transparency;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { 0, 150 }, { 333,333 / 11 }, videoContainer, { {"textSize", {.floatVal = fontsize}}, {"ref", ref}, {"max", max}, {"min", min}, {"roundAt", roundAt} }, { {"tex", "slider"},{"label", "magik"} }, true);

        func; func.funcp = UI::uiCfunc_toggleFullscreen;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, 50 }, { 200, 200 }, videoContainer, { {"func", func},{"textSize", {.floatVal = fontsize}}, {"fitText", fitText}, {"padding", padding}, {"menu", menu} }, { {"tex", "empty"}, {"text", "Toggle fullscreen"} }, true, anchorMID);

        func; func.funcp = UI::uiCfunc_nextResolution;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, -50 }, { 200, 200 }, videoContainer, { {"func", func}, {"textSize", {.floatVal = fontsize}}, {"fitText", fitText}, {"padding", padding}, {"menu", menu}, {"textp", {.stringp = &game::currResText }} }, { {"tex", "empty"}, {"text", ""} }, true, anchorMID);
        //-----------------------------------video------------------------------------------

        //-----------------------------------cursor------------------------------------------

        UI::addElement(ECS::newEntity(), ui_TEXT, { 0, 350 }, { 300,30 }, cursorContainer, { {"textSize", {.floatVal = fontsize}}, {"centered", {.boolVal = true}} }, { {"text", "Border"} }, true);

        max.floatVal = 360;
        min.floatVal = 0;
        ref.floatValp = &globals::cursorborderhue;
        roundAt.intVal = 0.1;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { 0, 275 }, { 333,333 / 11 }, cursorContainer, { {"textSize", {.floatVal = fontsize}}, {"ref", ref}, {"max", max}, {"min", min}, {"roundAt", roundAt} }, { {"tex", "sliderrainbow"},{"label", ""} }, true);

        max.floatVal = 1;
        min.floatVal = 0;
        ref.floatValp = &globals::cursorbordersaturation;
        roundAt.intVal = 0.01;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { 0, 200 }, { 333,333 / 11 }, cursorContainer, { {"textSize", {.floatVal = fontsize}}, {"ref", ref}, {"max", max}, {"min", min}, {"roundAt", roundAt} }, { {"tex", "slider"},{"label", ""} }, true);

        max.floatVal = 1;
        min.floatVal = 0;
        ref.floatValp = &globals::cursorbordervalue;
        roundAt.intVal = 0.01;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { 0, 125 }, { 333,333 / 11 }, cursorContainer, { {"textSize", {.floatVal = fontsize}}, {"ref", ref}, {"max", max}, {"min", min}, {"roundAt", roundAt} }, { {"tex", "slider"},{"label", ""} }, true);

        UI::addElement(ECS::newEntity(), ui_TEXT, { 0, 50 }, { 333,333 / 11 }, cursorContainer, { {"textSize", {.floatVal = fontsize}}, {"centered", {.boolVal = true}} }, { {"text", "Cursor"} }, true);
        
        max.floatVal = 360;
        min.floatVal = 0;
        ref.floatValp = &globals::cursorhue;
        roundAt.intVal = 0.1;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { 0, -25 }, { 333,333 / 11 }, cursorContainer, { {"textSize", {.floatVal = fontsize}}, {"ref", ref}, {"max", max}, {"min", min}, {"roundAt", roundAt} }, { {"tex", "sliderrainbow"},{"label", ""} }, true);

        max.floatVal = 1;
        min.floatVal = 0;
        ref.floatValp = &globals::cursorsaturation;
        roundAt.intVal = 0.01;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { 0, -100 }, { 333,333 / 11 }, cursorContainer, { {"textSize", {.floatVal = fontsize}}, {"ref", ref}, {"max", max}, {"min", min}, {"roundAt", roundAt} }, { {"tex", "slider"},{"label", ""} }, true);

        max.floatVal = 1;
        min.floatVal = 0;
        ref.floatValp = &globals::cursorvalue;
        roundAt.intVal = 0.01;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { 0, -175 }, { 333,333 / 11 }, cursorContainer, { {"textSize", {.floatVal = fontsize}}, {"ref", ref}, {"max", max}, {"min", min}, {"roundAt", roundAt} }, { {"tex", "slider"},{"label", ""} }, true);

        max.floatVal = 5;
        min.floatVal = 1;
        ref.floatValp = &globals::cursorsize;
        roundAt.intVal = 0.01;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { 0, -250 }, { 333,333 / 11 }, cursorContainer, { {"textSize", {.floatVal = fontsize}}, {"ref", ref}, {"max", max}, {"min", min}, {"roundAt", roundAt} }, { {"tex", "slider"},{"label", "Size"} }, true);


        menu.intVal = settingsContainer;
        func; func.funcp = openMenu;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, -350 }, { 200, 200 }, cursorContainer, { {"func", func}, {"textSize", {.floatVal = fontsize}}, {"fitText", fitText}, {"padding", padding}, {"menu", menu} }, { {"tex", "empty"}, {"text", "Back"} }, true, anchorMID);

        //-----------------------------------cursor------------------------------------------

        //-----------------------------------keybindings------------------------------------------

        int width = 800;
        int height = 75;
        int margin = 15;

      
        int container = ECS::newEntity();
        UI::addElement(container, ui_CONTAINER, { 0, 0 }, { 0, 0 }, keybindingsContainer, { {"padding", {.floatVal = 1} } }, {}, true, anchorNONE);
        UI::addElement(ECS::newEntity(), ui_TEXT, { 0, -450 }, { 200, 200 }, container, { {"textSize", {.floatVal = fontsize}},{"centered", {.boolVal= true}} }, { {"text", "Controls"} }, true, anchorTOP);

        fontsize /= 1.3;
        container = ECS::newEntity();
        UI::addElement(container, ui_CONTAINER, { 0, 0 }, { 0, 0 }, keybindingsContainer, { {"padding", {.floatVal = 2.5} }, {"opacity", {.floatVal = 0.8}} }, {}, true, anchorNONE);
        int container2 = ECS::newEntity();
        UI::addElement(container2, ui_CONTAINER, { 0, 0 }, { 0, 0 }, container, { {"padding", {.floatVal = 1} }, {"colorp", {.vec3p = &globals::backcolor2}}, {"opacity", {.floatVal = 0.8}} }, {}, true, anchorNONE);

        UI::addElement(ECS::newEntity(), ui_KEYBIND, { -(width/2 + margin/2), 350 }, { width, height }, container2, { {"key", {.intVal = k_PRIMARY}}, {"textSize", {.floatVal = fontsize}}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Use item"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { (width / 2 + margin / 2), 350 }, { width, height }, container2, { {"key", {.intVal = k_SECONDARY}}, {"textSize", {.floatVal = fontsize}}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Interact"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { -(width / 2 + margin / 2), 250 }, { width, height }, container2, { {"key", {.intVal = k_MIDDLE}}, {"textSize", {.floatVal = fontsize}}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Middle Mouse"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { (width / 2 + margin / 2), 250 }, { width, height }, container2, { {"key", {.intVal = k_MOUSE4}}, {"textSize", {.floatVal = fontsize}}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Mouse 4"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { -(width / 2 + margin / 2), 150 }, { width, height }, container2, { {"key", {.intVal = k_MOUSE5}}, {"textSize", {.floatVal = fontsize}}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Mouse 5"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { (width / 2 + margin / 2), 150 }, { width, height }, container2, { {"key", {.intVal = k_UP}}, {"textSize", {.floatVal = fontsize}}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Up"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { -(width / 2 + margin / 2), 50 }, { width, height }, container2, { {"key", {.intVal = k_DOWN}}, {"textSize", {.floatVal = fontsize}}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Down"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { (width / 2 + margin / 2), 50 }, { width, height }, container2, { {"key", {.intVal = k_LEFT}}, {"textSize", {.floatVal = fontsize}}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Left"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { -(width / 2 + margin / 2), -50 }, { width, height }, container2, { {"key", {.intVal = k_RIGHT}}, {"textSize", {.floatVal = fontsize}}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Right"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { (width / 2 + margin / 2), -50 }, { width, height }, container2, { {"key", {.intVal = k_JUMP}}, {"textSize", {.floatVal = fontsize}}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Jump"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { -(width / 2 + margin / 2), -150 }, { width, height }, container2, { {"key", {.intVal = k_HEAL}}, {"textSize", {.floatVal = fontsize}}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Quick Heal"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { (width / 2 + margin / 2), -150 }, { width, height }, container2, { {"key", {.intVal = k_MANA}}, {"textSize", {.floatVal = fontsize}}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Quick Mana"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { -(width / 2 + margin / 2), -250 }, { width, height }, container2, { {"key", {.intVal = k_THROW}}, {"textSize", {.floatVal = fontsize}}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Throw"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { (width / 2 + margin / 2), -250 }, { width, height }, container2, { {"key", {.intVal = k_INVENTORY}}, {"textSize", {.floatVal = fontsize}}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Inventory"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { -(width / 2 + margin / 2), -350 }, { width, height }, container2, { {"key", {.intVal = k_ZOOMIN}}, {"textSize", {.floatVal = fontsize}}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Zoom In"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { (width / 2 + margin / 2), -350 }, { width, height }, container2, { {"key", {.intVal = k_ZOOMOUT}}, {"textSize", {.floatVal = fontsize}}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Zoom Out"} }, true, anchorMID);

        func; func.funcp = UI::uiCfunc_resetKeybinds;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, -450 }, { width, 75 }, container2, {  {"func", func}, {"textSize", {.floatVal = fontsize}}, {"padding", padding}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex", "keybindbg"}, {"text", "Reset to Default"} }, true, anchorMID);

        menu.intVal = settingsContainer;
        func; func.funcp = openMenu;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, 150 }, { 600, 50 }, keybindingsContainer, { {"borderhover", {.boolVal = true}},  {"textonhover", {.boolVal = false}},{"func", func}, {"textSize", {.floatVal = fontsize}}, {"menu", menu} }, { {"tex", "empty"}, {"text", "Back"} }, true, anchorBOT);


        //-----------------------------------keybindings------------------------------------------

        //-----------------------------------playerselect----------------------------------------

        playersContainer = ECS::newEntity();
        UI::addElement(playersContainer, ui_EMPTY, { 0,0 }, { 0,0 }, playerSelectContainer);

        refreshPlayers();

        padding.floatVal = 40;
        func.funcp = openMenu;
        menu.intVal = playerCreateContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 300, 100 }, { 500, 50 }, playerSelectContainer, { {"borderhover", {.boolVal = true}},  {"textonhover", {.boolVal = false}},{"func", func}, {"textSize", {.floatVal = fontsize}}, {"menu", menu} }, { {"tex", "empty"}, {"text", "New"} }, true, anchorBOT);

        func.funcp = openMenu;
        menu.intVal = mainSelectContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { -300, 100 }, { 500, 50 }, playerSelectContainer, { {"borderhover", {.boolVal = true}},  {"textonhover", {.boolVal = false}},{"func", func}, {"textSize", {.floatVal = fontsize}}, {"menu", menu} }, { {"tex", "empty"}, {"text", "Back"} }, true, anchorBOT);

        //-----------------------------------playerselect----------------------------------------

        //-----------------------------------playercreate----------------------------------------

        container = ECS::newEntity();
        UI::addElement(container, ui_CONTAINER, { 0, 0 }, { 0, 0 }, playerCreateContainer, { {"padding", {.floatVal = 1} }, {"opacity", {.floatVal = 0.8}} }, {}, true, anchorNONE);

        ref; ref.stringp = &Player::name;
        func.funcp = UI::uiCfunc_captureInput;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, 50 }, { 700,40 }, container,
            { { "borderhover", {.boolVal = true} }, { "textonhover", {.boolVal = false} }, { "func", func }, { "ref", ref }, { "textSize", {.floatVal = fontsize} }, { "menu", menu }, {"capturinglength",{.intVal = 24}} },
            { {"tex", "empty"}, {"text", "Enter name"} }, true, anchorMID);

        max.floatVal = 360;
        min.floatVal = 0;
        ref.floatValp = &Player::hue;
        roundAt.intVal = 0.1;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { 150, -125 }, { 333,333 / 11 }, container, { {"textSize", {.floatVal = 0}}, {"ref", ref}, {"max", max}, {"min", min}, {"roundAt", roundAt} }, { {"tex", "sliderrainbow"},{"label", ""} }, true);

        container2 = ECS::newEntity();
        UI::addElement(container2, ui_CONTAINER, { 0, 0 }, { 0, 0 }, container, { {"padding", {.floatVal = 0.5} }, {"opacity", {.floatVal = 0.96}} }, {}, true, anchorNONE);

        UI::addElement(ECS::newEntity(), ui_DISPLAY, { -175, -125 }, { 100, 100 }, container2, { {"huep", {.floatValp = &Player::hue}} }, { {"tex", "player"} }, true);

        menu.intVal = playerSelectContainer;
        func; func.funcp = openMenu;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { -195, -350 }, { 270, 50 }, playerCreateContainer, { {"borderhover", {.boolVal = true}},  {"textonhover", {.boolVal = false}},{"func", func}, {"textSize", {.floatVal = fontsize}}, {"menu", menu} }, { {"tex", "empty"}, {"text", "Back"} }, true, anchorMID);

        func; func.funcp = UI::uiCfunc_createPlayer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 195, -350 }, { 270, 50 }, playerCreateContainer, { {"borderhover", {.boolVal = true}},  {"textonhover", {.boolVal = false}},{"func", func}, {"textSize", {.floatVal = fontsize}}, {"menu", menu} }, { {"tex", "empty"}, {"text", "Create"} }, true, anchorMID);

        //-----------------------------------playercreate----------------------------------------
        

        //-----------------------------------worldselect-----------------------------------------
        
        refreshWorlds();

        func.funcp = openMenu;
        menu.intVal = playerSelectContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { -300, 100 }, { 500, 50 }, worldSelectContainer, { {"borderhover", {.boolVal = true}},  {"textonhover", {.boolVal = false}},{"func", func}, {"textSize", {.floatVal = fontsize}}, {"menu", menu} }, { {"tex", "empty"}, {"text", "Back"} }, true, anchorBOT);

        padding.floatVal = 40;
        func.funcp = openMenu;
        menu.intVal = worldCreateContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 300, 100 }, { 500, 50 }, worldSelectContainer, { {"borderhover", {.boolVal = true}},  {"textonhover", {.boolVal = false}},{"func", func}, {"textSize", {.floatVal = fontsize}}, {"menu", menu} }, { {"tex", "empty"}, {"text", "New"} }, true, anchorBOT);


        //-----------------------------------worldselect----------------------------------------

        //-----------------------------------worldcreate----------------------------------------
        container = ECS::newEntity();
        UI::addElement(container, ui_CONTAINER, { 0, 0 }, { 0, 0 }, worldCreateContainer, { {"padding", {.floatVal = 1} }, {"opacity", {.floatVal = 0.8}} }, {}, true, anchorNONE);

        ref; ref.stringp = &map::name;
        func.funcp = UI::uiCfunc_captureInput;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, 75 }, { 700,50 }, container,
            { { "borderhover", {.boolVal = true} }, { "textonhover", {.boolVal = false} }, { "func", func }, { "ref", ref }, {"capturinglength",{.intVal = 24}}, { "textSize", {.floatVal = fontsize} }, { "menu", menu } },
            { {"tex", "empty"}, {"text", "Enter name"} }, true, anchorMID);

        ref; ref.stringp = &map::seed;
        func.funcp = UI::uiCfunc_captureInput;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, 0 }, { 570,30 }, container,
            { { "borderhover", {.boolVal = true} }, { "textonhover", {.boolVal = false} }, { "func", func }, { "ref", ref }, {"capturinglength",{.intVal = 20}}, { "textSize", {.floatVal = fontsize}}, {"menu", menu} },
            { {"tex", "empty"}, {"text", "Enter seed"}, {"textcolor", {"\\c155155155"}}}, true, anchorMID);

        max; max.intVal = 5000;
        roundAt; roundAt.intVal = 50;
        textSize; textSize.floatVal = 1.0f;
        ref.intValp = &map::mapX;
        UI::addElement(ECS::newEntity(), ui_TEXT, { 200, -100 }, { 300,30 }, container, { {"textSize", {.floatVal = fontsize}}, {"centered", {.boolVal = true}} }, { {"text", "World Width:"} }, true);
        UI::addElement(ECS::newEntity(), ui_DRAGINT, { 200, -150 }, { 300,30 }, container, { {"textSize", textSize}, {"ref", ref}, {"min", {.intVal = 10}}, {"max", max}, {"roundAt", roundAt} }, { {"tex", "slider"},{"label", ""} }, true);
        ref; ref.intValp = &map::mapY;
        UI::addElement(ECS::newEntity(), ui_TEXT, { 200, -200 }, { 300,30 }, container, { {"textSize", {.floatVal = fontsize}}, {"centered", {.boolVal = true}} }, { {"text", "World Height:"} }, true);
        UI::addElement(ECS::newEntity(), ui_DRAGINT, { 200, -250}, { 300,30 }, container, {  {"textSize", textSize}, {"ref", ref}, {"min", {.intVal = 10}}, {"max", max}, {"roundAt", roundAt} }, { {"tex", "slider"},{"label", ""} }, true);
        
        UI::addElement(ECS::newEntity(), ui_DISPLAY, { -225, -190 }, { 200, 200 }, container, {}, { {"tex", "worldcreationbg"} }, true);
        worldsizedisplay = ECS::newEntity();
        UI::addElement(worldsizedisplay, ui_DISPLAY, { -225, -190 }, { 200, 200 }, container, {}, { {"tex", "worldcreationlarge"} }, true);
        UI::addElement(ECS::newEntity(), ui_DISPLAY, { -225, -190 }, { 200, 200 }, container, {}, { {"tex", "worldcreationleft"} }, true);
        UI::addElement(ECS::newEntity(), ui_DISPLAY, { -225, -190 }, { 206, 206 }, container, {}, { {"tex", "worldcreationborder"} }, true);


        func.funcp = UI::uiCfunc_createWorld;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 195, -350 }, { 270, 50 }, worldCreateContainer, { {"borderhover", {.boolVal = true}},  {"textonhover", {.boolVal = false}},{"func", func}, {"textSize", {.floatVal = fontsize}}, {"menu", menu} }, { {"tex", "empty"}, {"text", "Create"} }, true, anchorMID);

        menu.intVal = worldSelectContainer;
        func; func.funcp = openMenu;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { -195, -350 }, { 270, 50 }, worldCreateContainer, { {"borderhover", {.boolVal = true}},  {"textonhover", {.boolVal = false}},{"func", func}, {"textSize", {.floatVal = fontsize}}, {"menu", menu} }, { {"tex", "empty"}, {"text", "Back"} }, true, anchorMID);
        //-----------------------------------worldcreate----------------------------------------

        //-----------------------------------loadingscreen----------------------------------------
        progressText = ECS::newEntity();
        UI::addElement(progressText, ui_TEXT, { 0,0 }, { 300,30 }, loadingScreen, { {"centered", {.boolVal = true}}, {"textSize", {.floatVal = globals::fontSize * 3.75f}} }, { {"text", ""} }, true, anchorMID);
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
            input::clear();
            camera::updateNormal();

            if (map::mapX * map::mapY < 1666*1666) {
                ECS::getComponent<uiC>(worldsizedisplay)->textStats["tex"] = "worldcreationsmall";
            }
            else if (map::mapX * map::mapY < 3332 * 3332) {
                ECS::getComponent<uiC>(worldsizedisplay)->textStats["tex"] = "worldcreationmedium";
            }
            else {
                ECS::getComponent<uiC>(worldsizedisplay)->textStats["tex"] = "worldcreationlarge";
            }

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
            if (map::worldGenProgress != "") {
                UI::hideChildren(ECS::getComponent<uiC>(loadingScreen), false, true);
                UI::setTStat(ECS::getComponent<uiC>(progressText), "text", map::worldGenProgress);
            }
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
        if (input::pressed(k_PRIMARY)) {
            uiSystem::mouseClicked = true;
        }
        if (input::rpressed(GLFW_KEY_P)) {
            ECS::print();
        }
	}
    void openMenu(uiC* p)
    {
        UI::captureInput(nullptr, false);
        Player::name.clear();
        UI::hideChildren(ECS::getComponent<uiC>(uiSystem::body), true);
        UI::hideChildren(ECS::getComponent<uiC>(UI::getStat(p, "menu")->intVal), false);
        ECS::getComponent<uiC>(logo)->hidden = false;
    }

    void openMenu(int menu)
    {
        UI::captureInput(nullptr, false);
        map::name.clear();
        map::seed.clear();
        Player::name.clear();
        UI::hideChildren(ECS::getComponent<uiC>(uiSystem::body), true);
        UI::hideChildren(ECS::getComponent<uiC>(menu), false);
        ECS::getComponent<uiC>(logo)->hidden = false;
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