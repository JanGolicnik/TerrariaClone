#include "MainLoop.h"
#include <globals.h>
#include <utils.h>
#include <input.h>
#include <ECS/ECS.h>
#include <componentsystems.h>
#include <map.h>
#include <BlockManager.h>
#include <Inventory.h>
#include <Player.h>
#include <animations.h>
#include <textures.h>
#include <resources.h>
#include <Terraria.h>
#include <cam.h>
#include <DroppedItems.h>
#include <background.h> 
#include <enemies.h>
#include <particles.h>

namespace MainLoop {

    bool newMap = false;
    int WOW;
    int playerms;
    int playerrange;
    int WOW2;
    bool nekikrneki;
    bool settingsOpen = false;
    int realtimeUI;
    int settingsUI;
    int inventoryC;
    int chestInvC;
    int equipmentC;
    int tooltip;
    int crafting;
    int cursoritem;
    int hpbar;
    int manabar;
    int settingsbtn;
    int settingsMainContainer;
    int menus;
    int cheatsContainer;
    int nekidruzgaContainer;
    int helmetslot;
    int breastplateslot;
    int greavesslot;
    int defensetext;
    int accessoryslot1;
    int accessoryslot2;
    int accessoryslot3;
    int accessoryslot4;
    int accessoryslot5;
    int cursor;
    int hpbarC;
    int NPCDialogue;

    void init()
    {
        map::load();

        Player::setPos(map::PlayerSpawn);
        cam::pos = -Player::pos;
        cam::limits = glm::vec2(-map::mapX + globals::resX / (globals::blocksizepx / 2), -map::mapY + globals::resY / (globals::blocksizepx / 2));

        settingsOpen = false;

        createUI();
        createSettingsMenu();

        map::guideEnt = enemies::spawnEnemy("guide", map::PlayerSpawn);
        ECS::commitQueues();
        ECS::getComponent<mobC>(map::guideEnt)->displayName = map::guideName;
        std::cout << " a " << map::guideName << "\n";
    }

    void run()
    {
        static int updateRate = 10;
        static int dayRate = 2;
        static int maxFps = 60;
        static double ctime = 0;
        static double ltime = 0;
        static double lutime = 0;
        static double ldayutime = 0;
        static double dutime = 0;
        static double daydtime = 0;
        static double dtime = 0;
        static float lastTime = 0;

        static float noonW;
        static float eveningW;
        static float nightW;
        static float morningW;

        ctime = glfwGetTime();
        dtime = ctime - ltime;
        dutime = ctime - lutime;
        daydtime = ctime - ldayutime;


        if (daydtime > 1.0f / dayRate) {
            globals::cdayTime++;
            if (globals::cdayTime > globals::dayLength) {
                globals::cdayTime = 0;
            }

            noonW = glm::clamp(1 - (abs(globals::cdayTime - globals::dayLength / 4.0f) / (globals::dayLength / 4.0f)), 0.0f, 1.0f);
            eveningW = glm::clamp(1 - (abs(globals::cdayTime - (globals::dayLength / 4.0f * 2)) / (globals::dayLength / 4.0f)), 0.0f, 1.0f);
            nightW = glm::clamp(1 - (abs(globals::cdayTime - (globals::dayLength / 4.0f * 3)) / (globals::dayLength / 4.0f)), 0.0f, 1.0f);
            morningW = glm::clamp(1 - (abs(globals::cdayTime - (globals::dayLength / 4.0f * 0)) / (globals::dayLength / 4.0f)), 0.0f, 1.0f);
            morningW += glm::clamp(1 - (abs(globals::cdayTime - (globals::dayLength / 4.0f * 4)) / (globals::dayLength / 4.0f)), 0.0f, 1.0f);
            globals::dayclr = globals::noonclr * noonW + globals::eveningclr * eveningW + globals::nightclr * nightW + globals::morningclr * morningW;
            ldayutime = ctime;
        }

        if (dutime > 1.0f / updateRate) {
            UI::craftingStationsInRange.clear();
            UI::craftingStationsInRange.insert("none");

            Layers::updateOnScreen();

            lutime = ctime;
        }

        if (dtime > 1.0f / maxFps) {
            handleInput();
            cam::updateTowardsPlayer();

            if (!settingsOpen) {
                AnimManager::updateAnims();
                Player::update();
                Layers::calculateLight();
            }

            glBindFramebuffer(GL_FRAMEBUFFER, globals::mainFB);
            glClearColor(globals::dayclr.r, globals::dayclr.g, globals::dayclr.b, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            background::render();
            
            UI::setTStat(ECS::getComponent<uiC>(defensetext), "text", std::to_string(Player::defense));
            Layers::renderLayers();
            Layers::spawnMobs();
            
            ECS::commitQueues();
            if (!settingsOpen) {
                Terraria::physSys->Update();
                Terraria::aiSys->Update();
                Terraria::toolSys->Update();
                Terraria::droppedItemSys->Update();
                Terraria::mobSys->Update();
                Terraria::particleESys->Update();
            }
            Terraria::uiSys->Update();
            ECS::commitQueues();
            Terraria::drawSys->Update();

            if (!settingsOpen) {
                UI::refreshCrafting();
            }
            
            Player::render();
            Terraria::drawOverlays();
            
            globals::bgoffset = -cam::pos.x / map::mapX;
            
            glfwSwapBuffers(globals::window);
            ltime = ctime;
            Input::clear();
        }
    }
    void handleInput()
    {
        if (Input::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
            uiSystem::mouseClicked = true;
        }
        if (Input::mousePressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            uiSystem::mouseRightClicked = true;
        }

        if (settingsOpen) {
            if (Input::pressed(GLFW_KEY_ESCAPE)) {
                toggleSettings();
            }
            return;
        }
        auto craftingC = ECS::getComponent<uiC>(MainLoop::crafting);
        if (craftingC->hidden) {
            Player::invBlock -= Input::scroll;
        }
        else {
            UI::getStat(craftingC, "craftable")->intVal += Input::scroll;
        }

        if (Input::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
            Player::leftClick = true;
            AnimManager::watchAnim("playeredit", &Player::anim);
        }
        if (Input::mouseReleased(GLFW_MOUSE_BUTTON_LEFT)) {
            Player::leftClick = false;
            AnimManager::watchAnim("playeridle", &Player::anim);
        }
        if (Input::mousePressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            Player::rightClick = true;
            AnimManager::watchAnim("playeredit", &Player::anim);
        }
        if (Input::mouseReleased(GLFW_MOUSE_BUTTON_RIGHT)) {
            Player::rightClick = false;
            AnimManager::watchAnim("playeridle", &Player::anim);
        }

        if (Input::shift) {
            if (Input::pressed(GLFW_KEY_SLASH)) {
                globals::zoom += 0.05f;
            }
            if (Input::pressed(GLFW_KEY_EQUAL)) {
                globals::zoom -= 0.05f;
            }
            if (Input::pressed(GLFW_KEY_R)) {
                resources::loadAssets(false, true, true);
            }
            if (Input::pressed(GLFW_KEY_M)) {
                map::generateTerraria();
            }
        }
        else {
            if (Input::pressed(GLFW_KEY_A)) {
                AnimManager::watchAnim("playerwalkleft", &Player::anim);
            }
            if (Input::held(GLFW_KEY_A)) {
                auto vel = Player::velocity();
                if (vel->x > 0) vel->x += utils::approach(vel->x, -Player::ms, 35);
                vel->x +=  utils::approach(vel->x, -Player::ms, 60);
                Player::friction = false;
                Player::dir = -1;
                AnimManager::watchAnim("playerwalkleft", &Player::anim, false);
            }
            if (Input::released(GLFW_KEY_A)) {
                Player::friction = true;
                AnimManager::watchAnim("playeridle", &Player::anim);
            }
            if (Input::pressed(GLFW_KEY_D)) {
                AnimManager::watchAnim("playerwalkright", &Player::anim);
            }
            if (Input::held(GLFW_KEY_D)) {
                auto vel = Player::velocity();
                if (vel->x < 0) vel->x += utils::approach(vel->x, Player::ms, 35);
                vel->x += utils::approach(vel->x, Player::ms, 60);
                Player::dir = 1;
                Player::friction = false;
                AnimManager::watchAnim("playerwalkright", &Player::anim, false);
            }
            if (Input::released(GLFW_KEY_D)) {
                Player::friction = true;
                AnimManager::watchAnim("playeridle", &Player::anim);
            }
            if (Input::held(GLFW_KEY_S)) {
                if (Layers::queryBlockInfo(Layers::getLayer("blocks"), Player::pos - glm::vec2(0, Player::height / 2.0f + 0.1f))->skippable) {
                    Player::ignorebot = true;
                }
            }
            if (Input::pressed(GLFW_KEY_SPACE)) {
                Player::jump();
                AnimManager::watchAnim("playerjump", &Player::anim);
            }
            if (Input::released(GLFW_KEY_SPACE)) {
                Player::currJumpTime = 0;
            }
            if (Input::pressed(GLFW_KEY_T)) {
                Player::setPos(globals::mouseBlockCoords(false));
            }
            if (Input::pressed(GLFW_KEY_ESCAPE)) {
                UI::inventory->toggle();
                UI::chest->toggle(1);
                ECS::getComponent<uiC>(crafting)->hidden = UI::inventory->hidden();
                UI::hideChildren(ECS::getComponent<uiC>(equipmentC), UI::inventory->hidden());
                if (UI::inventory->hidden() == true) {
                    UI::hideChildren(ECS::getComponent<uiC>(NPCDialogue), true, true);
                }
            }
            if (Input::pressed(GLFW_KEY_1)) {
                Player::invBlock = 0;
            }
            if (Input::pressed(GLFW_KEY_2)) {
                Player::invBlock = 1;
            }
            if (Input::pressed(GLFW_KEY_3)) {
                Player::invBlock = 2;
            }
            if (Input::pressed(GLFW_KEY_4)) {
                Player::invBlock = 3;
            }
            if (Input::pressed(GLFW_KEY_V)) {
                Player::editsize++;
            }
            if (Input::pressed(GLFW_KEY_B)) {
                Player::editsize--;
            }
            if (Input::pressed(GLFW_KEY_P)) {
                ECS::print();
            }
            if (Input::pressed(GLFW_KEY_I)) {
            }
        }
    }

    void toggleSettings()
    {
        settingsOpen = !settingsOpen;

        if (settingsOpen) {
            UI::hideChildren(ECS::getComponent<uiC>(settingsUI), false);
            ECS::getComponent<uiC>(menus)->hidden = true;
            UI::hideChildren(ECS::getComponent<uiC>(menus), true);
            UI::hideChildren(ECS::getComponent<uiC>(nekidruzgaContainer), false);
            UI::hideChildren(ECS::getComponent<uiC>(equipmentC), true);
            UI::hideChildren(ECS::getComponent<uiC>(realtimeUI), true);
            return;
        }

        UI::hideChildren(ECS::getComponent<uiC>(settingsUI), true);
        UI::hideChildren(ECS::getComponent<uiC>(inventoryC), false);
        UI::hideChildren(ECS::getComponent<uiC>(equipmentC), false);
        auto MANAbar = ECS::getComponent<uiC>(manabar);
        MANAbar->hidden = false;
        UI::hideChildren(MANAbar, false);
        auto HPbar = ECS::getComponent<uiC>(hpbar);
        HPbar->hidden = false;
        UI::hideChildren(HPbar, false);
        UI::chest->toggle(1);
        ECS::getComponent<uiC>(settingsbtn)->hidden = false;
    }

    void createUI()
    {
        realtimeUI = ECS::newEntity();
        settingsUI = ECS::newEntity();
        equipmentC = ECS::newEntity();
        hpbarC = ECS::newEntity();
        UI::addElement(hpbarC, ui_EMPTY, { 0,0 }, { 0,0 }, uiSystem::body, {}, {}, true, anchorNONE);
        UI::addElement(realtimeUI, ui_EMPTY, { 0,0 }, { 0,0 }, uiSystem::body, {}, {}, true, anchorNONE);
        UI::addElement(settingsUI, ui_EMPTY, { 0,0 }, { 0,0 }, uiSystem::body, {}, {}, true, anchorNONE);
        UI::addElement(equipmentC, ui_EMPTY, { 0,0 }, { 0,0 }, uiSystem::body, {}, {}, true, anchorNONE);
        
        tooltip = ECS::newEntity();
        uiStat textSize; textSize.floatVal = globals::fontSize;
        uiStat padding; padding.floatVal = 0.3;
        UI::addElement(tooltip, ui_TOOLTIP, { 0,0 }, { 0,0 }, uiSystem::body, { {"textSize", textSize}, {"padding", padding} }, { {"item", "none"} }, false, anchorNONE);
        
        cursoritem = ECS::newEntity();
        uiStat tmpstat;
        tmpstat.itemp = &UI::cursorItem;
        UI::addElement(cursoritem, ui_CURSORITEM, { 0, 0 }, { 2, 2 }, uiSystem::body, { {"item", tmpstat} }, { {"text", ""}}, false, anchorNONE);

        inventoryC = ECS::newEntity();
        UI::addElement(inventoryC, ui_EMPTY, { 0, 0 }, { 0 , 0 }, realtimeUI);
        chestInvC = ECS::newEntity();
        UI::addElement(chestInvC, ui_EMPTY, { 0, 0 }, { 0 , 0 }, realtimeUI);

        UI::create();

        crafting = ECS::newEntity();
        UI::addElement(crafting, ui_CRAFTABLEITEM, { 125, -225 }, { 100,100 }, inventoryC, {}, {}, true, anchorLEFT);
        
        uiStat step; step.intVal = 20;
        uiStat sizex; sizex.intVal = 10;
        uiStat itemsize; itemsize.floatVal = 60;
        uiStat max; max.floatVal = Player::maxhp;
        uiStat value; value.floatValp = &Player::hp;
        hpbar = ECS::newEntity();
        UI::addElement(hpbar, ui_RESOURCEBAR, { -720,0 }, { 0,0 }, inventoryC, { {"step", step}, {"max", max}, {"sizex", sizex},{"value", value},{ "itemsize", itemsize} }, { {"itemtex", "heart"} }, false, anchorTOPRIGHT);

        NPCDialogue = ECS::newEntity();
        UI::addElement(NPCDialogue, ui_TEXTBOX, { 0,-300 }, { 1,1 }, realtimeUI, { {"colorp", {.vec3p = &globals::itemslotColor }}, {"minwidth", {.floatVal = 1000}}, {"maxwidth", {.floatVal = 1000}}, {"padding",{.floatVal = 120}}}, {{"tex", "invempty"}, {"text", "Never been to a party before? You might want to check around with the others. People sometimes bring out special party favors."}}, true, anchorTOP);

        step.intVal = 20;
        sizex.intVal = 1;
        itemsize.floatVal = 60;
        max.floatVal = Player::maxmana;
        value.floatValp = &Player::mana;
        manabar = ECS::newEntity();
        UI::addElement(manabar, ui_RESOURCEBAR, { -100,0 }, { 30,30 }, inventoryC, { {"step", step}, {"max", max}, {"sizex", sizex},{"value", value},{ "itemsize", itemsize} }, { {"itemtex", "mana"} }, false, anchorTOPRIGHT);

        uiStat fitText; fitText.boolVal = true;
        uiStat func; func; func.funcp = UI::uiCfunc_toggleSettings;
        settingsbtn = ECS::newEntity();
        UI::addElement(settingsbtn, ui_BUTTON, { -300, 100 }, { 200, 200 }, realtimeUI, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding} }, { {"tex", "empty"}, {"text", "Settings"} }, false, anchorBOTRIGHT);

        helmetslot = ECS::newEntity();
        breastplateslot = ECS::newEntity();
        greavesslot = ECS::newEntity();
        accessoryslot1 = ECS::newEntity();
        accessoryslot2 = ECS::newEntity();
        accessoryslot3 = ECS::newEntity();
        accessoryslot4 = ECS::newEntity();
        accessoryslot5 = ECS::newEntity();

        uiStat itemp; itemp.itemp = &UI::helmetItem;
        uiStat textsize; textsize.floatVal = 0;
        uiStat limitFamily; limitFamily.intVal = if_HELMET;
        UI::addElement(helmetslot, ui_ITEMSLOT, { -125, 400 }, { 100,100 }, equipmentC, { {"item", itemp},{"textSize", textsize},{"limitFamily", limitFamily} }, {}, true, anchorRIGHT);
        itemp.itemp = &UI::breastplateItem;
        limitFamily.intVal = if_BREASTPLATE;
        UI::addElement(breastplateslot, ui_ITEMSLOT, { -125, 275 }, { 100,100 }, equipmentC, { {"item", itemp},{"textSize", textsize},{"limitFamily", limitFamily} }, {}, true, anchorRIGHT);
        itemp.itemp = &UI::greavesItem;
        limitFamily.intVal = if_GREAVES;
        UI::addElement(greavesslot, ui_ITEMSLOT, { -125, 150 }, { 100,100 }, equipmentC, { {"item", itemp},{"textSize", textsize},{"limitFamily", limitFamily} }, {}, true, anchorRIGHT);
        itemp.itemp = &UI::accessory1Item;
        limitFamily.intVal = if_ACCESORY;
        UI::addElement(accessoryslot1, ui_ITEMSLOT, { -125, 25 }, { 100,100 }, equipmentC, { {"item", itemp},{"textSize", textsize},{"limitFamily", limitFamily} }, {}, true, anchorRIGHT);
        itemp.itemp = &UI::accessory2Item;
        limitFamily.intVal = if_ACCESORY;
        UI::addElement(accessoryslot2, ui_ITEMSLOT, { -125, -100}, { 100,100 }, equipmentC, { {"item", itemp},{"textSize", textsize},{"limitFamily", limitFamily} }, {}, true, anchorRIGHT);
        itemp.itemp = &UI::accessory3Item;
        limitFamily.intVal = if_ACCESORY;
        UI::addElement(accessoryslot3, ui_ITEMSLOT, { -125, -225}, { 100,100 }, equipmentC, { {"item", itemp},{"textSize", textsize},{"limitFamily", limitFamily} }, {}, true, anchorRIGHT);
        itemp.itemp = &UI::accessory4Item;
        limitFamily.intVal = if_ACCESORY;
        UI::addElement(accessoryslot4, ui_ITEMSLOT, { -125, -350}, { 100,100 }, equipmentC, { {"item", itemp},{"textSize", textsize},{"limitFamily", limitFamily} }, {}, true, anchorRIGHT);
        itemp.itemp = &UI::accessory5Item;
        limitFamily.intVal = if_ACCESORY;
        UI::addElement(accessoryslot5, ui_ITEMSLOT, { -125, -475 }, { 100,100 }, equipmentC, { {"item", itemp},{"textSize", textsize},{"limitFamily", limitFamily} }, {}, true, anchorRIGHT);

        UI::addElement(ECS::newEntity(), ui_DISPLAY, { -215, -475 }, { 70,70 }, equipmentC, {}, { {"tex", "defense"} }, true, anchorRIGHT);
        uiStat centered; centered.boolVal = true;
        defensetext = ECS::newEntity();
        UI::addElement(defensetext, ui_TEXT, { -215, -475 }, { 1.3,1 }, equipmentC, { {"centered", centered } }, { {"text", ""} }, true, anchorRIGHT);
    
        cursor = ECS::newEntity();
        UI::addElement(cursor, ui_CURSOR, { 0,0 }, { 1,1 }, uiSystem::body, {}, {}, false, anchorNONE);
       
    }

    void openDialogue(std::string npcname)
    {
        auto textboxp = ECS::getComponent<uiC>(NPCDialogue);
        auto npc = &enemies::npcs[npcname];
        UI::setTStat(textboxp, "text", npc->dialogueOptions[rand() % npc->dialogueOptions.size()]);
        for (int i = 1; i < textboxp->children.size(); i++) {
            UI::deleteElement(textboxp->children[i]);
        }
        for (auto& i : npc->buttons) {
            UI::addElement(ECS::newEntity(), ui_BUTTON, { 1232131,123132131 }, { 200, 200 }, NPCDialogue, { {"func", {.funcp = i.second}}, {"textSize", {.floatVal = globals::fontSize}} }, { {"tex", "empty"}, {"text", i.first} }, true, anchorTOP);
        }
        UI::hideChildren(ECS::getComponent<uiC>(NPCDialogue), false, true);
    }

    void createSettingsMenu()
    {
        uiStat textSize; textSize.floatVal = globals::fontSize;
        uiStat fitText; fitText.boolVal = true;
        settingsMainContainer = ECS::newEntity();
        uiStat padding; padding.floatVal = 2;
        UI::addElement(settingsMainContainer, ui_CONTAINER, { 0,0 }, { 0,0 }, settingsUI, { {"padding", padding} }, {}, !settingsOpen, anchorNONE);

        menus = ECS::newEntity();
        UI::addElement(menus, ui_EMPTY, { 0,0 }, { 0,0 }, settingsMainContainer, {}, {}, !settingsOpen, anchorNONE);

        cheatsContainer = ECS::newEntity();
        UI::addElement(cheatsContainer, ui_EMPTY, { 0,0 }, { 0,0 }, menus, {}, {}, !settingsOpen, anchorNONE);

        nekidruzgaContainer = ECS::newEntity();
        UI::addElement(nekidruzgaContainer, ui_EMPTY, { 0,0 }, { 0,0 }, menus, {}, {}, !settingsOpen, anchorNONE);

        uiStat func; func.funcp = UI::uiCfunc_openSettingsMenu;
        uiStat menuStat; menuStat.intVal = cheatsContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { -200, 300 }, { 200, 200 }, settingsMainContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding}, {"menu", menuStat} }, { {"tex", "empty"}, {"text", "Cheats"} }, !settingsOpen, anchorMID);

        func; func.funcp = UI::uiCfunc_openSettingsMenu;
        menuStat.intVal = nekidruzgaContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { -200, 200 }, { 200, 200 }, settingsMainContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding}, {"menu", menuStat} }, { {"tex", "empty"}, {"text", "Neki Druzga"} }, !settingsOpen, anchorMID);

        func.funcp = UI::uiCfunc_saveGame; 
        textSize; textSize.floatVal = 1.0f;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { -200, 100 }, { 200, 200 }, settingsMainContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding} }, { {"tex", "empty"}, {"text", "Save"} }, !settingsOpen, anchorMID);

        func.funcp = UI::uiCfunc_saveAndExitGame; 
        textSize; textSize.floatVal = 1.0f;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { -200, 0 }, { 200, 200 }, settingsMainContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding} }, { {"tex", "empty"}, {"text", "Save & exit"} }, !settingsOpen, anchorMID);


        func; func.funcp = UI::uiCfunc_toggleSettings;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { -200, -100 }, { 200, 200 }, settingsMainContainer, { {"func", func}, {"textSize", textSize}, {"fitText", fitText}, {"padding", padding} }, { {"tex", "empty"}, {"text", "Close"} }, !settingsOpen, anchorMID);

        uiStat ref; ref.floatValp = &globals::cheaterlight;
        uiStat max; max.floatVal = 0.05f;
        uiStat roundAt; roundAt.floatVal = 0.001;
        textSize; textSize.floatVal = 1.0f;
        WOW = ECS::newEntity();
        UI::addElement(WOW, ui_DRAGFLOAT, { 200, 100 }, { 300,30 }, cheatsContainer, { {"textSize", textSize}, {"ref", ref}, {"max", max}, {"roundAt", roundAt} }, { {"label", "light"} }, !settingsOpen);

        roundAt.floatVal = 0.05f;
        ref.floatValp = &Player::ms;
        max.floatVal = 10.0f;
        playerms = ECS::newEntity();
        UI::addElement(playerms, ui_DRAGFLOAT, { 200, 40 }, { 300,30 }, cheatsContainer, { {"textSize", textSize}, {"ref", ref}, {"max", max}, {"roundAt", roundAt} }, { {"label", "ms"} }, !settingsOpen);

        roundAt.floatVal = 1.0f;
        ref.floatValp = &Player::editRange;
        max.floatVal = 40.0f;
        playerrange = ECS::newEntity();
        UI::addElement(playerrange, ui_DRAGFLOAT, { 200, -20 }, { 300,30 }, cheatsContainer, { {"textSize", textSize}, {"ref", ref}, {"max", max}, {"roundAt", roundAt} }, { {"label", "ranger"} }, !settingsOpen);

        roundAt.floatVal = 0.04f;
        ref.floatValp = &globals::volume;
        max.floatVal = 1.0f;
        playerrange = ECS::newEntity();
        UI::addElement(playerrange, ui_DRAGFLOAT, { 200, -60 }, { 300,30 }, cheatsContainer, { {"textSize", textSize}, {"ref", ref}, {"max", max}, {"roundAt", roundAt} }, { {"label", "veolumes"} }, !settingsOpen);


        UI::addElement(ECS::newEntity(), ui_DISPLAY, { 200, 100 }, { 200, 200 }, nekidruzgaContainer, {}, { {"tex", "dirt"} }, !settingsOpen, anchorMID);
    }

    void openMenu(int menu)
    {
        if (ECS::getComponent<uiC>(settingsMainContainer)->hidden) return;

        UI::hideChildren(ECS::getComponent<uiC>(menus), true);
        UI::hideChildren(ECS::getComponent<uiC>(menu), false);
    }

}
