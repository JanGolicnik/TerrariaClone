#include "gameLoop.h"
#include <globals.h>
#include <utils.h>
#include <input.h>
#include <ECS/ECS.h>
#include <componentsystems.h>
#include <world.h>
#include <blocks.h>
#include <ui.h>
#include <Player.h>
#include <animations.h>
#include <textures.h>
#include <resources.h>
#include <game.h>
#include <camera.h>
#include <background.h> 
#include <enemies.h>
#include <particles.h>
#include <buffs.h>

namespace gameLoop {

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
    int zoomedcursoritem;
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
    int buffcontainer;
    int pickuptextcontainer;
    int sun;
    int moon;

    void init()
    {
        map::load();

        Player::setPos(map::PlayerSpawn);
        camera::pos = -Player::pos;
        camera::limits = glm::vec2(-map::mapX + globals::resX / (globals::blocksizepx / 2), -map::mapY + globals::resY / (globals::blocksizepx / 2));
        settingsOpen = false;

        createUI();
        createSettingsMenu();

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
        dc.tex = "moon1";
        game::drawSys->addComponent(moon, &dc);

        map::guideEnt = enemies::spawnEnemy("guide", map::PlayerSpawn);
        ECS::commitQueues();
        ECS::getComponent<mobC>(map::guideEnt)->displayName = map::guideName;
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

            //3375-225 jutro
            //225-1575 dan
            //1575-2025 vecer
            //2025-3375 noc

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

            //noonW = glm::clamp(1 - (abs(globals::cdayTime - globals::dayLength / 4.0f) / (globals::dayLength / 4.0f)), 0.0f, 1.0f);
            //eveningW = glm::clamp(1 - (abs(globals::cdayTime - (globals::dayLength / 4.0f * 2)) / (globals::dayLength / 4.0f)), 0.0f, 1.0f);
            //nightW = glm::clamp(1 - (abs(globals::cdayTime - (globals::dayLength / 4.0f * 3)) / (globals::dayLength / 4.0f)), 0.0f, 1.0f);
            //morningW = glm::clamp(1 - (abs(globals::cdayTime - (globals::dayLength / 4.0f * 0)) / (globals::dayLength / 4.0f)), 0.0f, 1.0f);
            //morningW += glm::clamp(1 - (abs(globals::cdayTime - (globals::dayLength / 4.0f * 4)) / (globals::dayLength / 4.0f)), 0.0f, 1.0f);
            //globals::dayclr = globals::noonclr * noonW + globals::eveningclr * eveningW + globals::nightclr * nightW + globals::morningclr * morningW;
            ldayutime = ctime;
        }

        if (dutime > 1.0f / updateRate) {
            UI::craftingStationsInRange.clear();
            UI::craftingStationsInRange.insert("none");

            Layers::updateOnScreen();

            if (rand() % 5000 == 0) {
                map::placeRandomCactus();
                std::cout << "\nplaced cactus :D\n";
            }

            lutime = ctime;
        }

        static int counter = 0;
        static float sum = 0;

        if (dtime > 1.0f / maxFps) {

            sum += dtime * 1000.0f;
            if (++counter % 60 == 0) {
                std::cout << 1000 / (sum / 60.0f )<< "\n";
                counter = 0;
                sum = 0;
            }

            globals::time++;
            Player::calculateStats();
            handleInput();
            camera::updateTowardsPlayer();

            items::info["hermesboots"].stats;

            if (!settingsOpen) {
                animations::updateAnims();
                Player::update();
                Layers::calculateLight();
            }

            glBindFramebuffer(GL_FRAMEBUFFER, globals::tmpFB);
            glClearColor(globals::dayclr.r, globals::dayclr.g, globals::dayclr.b, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            game::drawSys->UpdateBehindBackground();

            background::render();
            
            game::updateSunAndMoon(sun, moon, Player::pos);

            ECS::getComponent<drawC>(defensetext)->text = std::to_string(Player::defense);
            Layers::renderLayers();
            Layers::spawnMobs();
            
            ECS::commitQueues();
            if (!settingsOpen) {
                game::physSys->Update();
                game::aiSys->Update();
                game::toolSys->Update();
                game::droppedItemSys->Update();
                game::mobSys->Update();
                game::particleESys->Update();
            }
            game::uiSys->Update();
            ECS::commitQueues();
            game::drawSys->Update();

            if (!settingsOpen) {
                UI::refreshCrafting();
            }
            
            Player::render();
            game::drawMain();

            glBindFramebuffer(GL_FRAMEBUFFER, globals::tmpFB);
            glClearColor(1, 1, 1, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            game::drawSys->UpdateFront();
            game::drawOverlays();

            globals::bgoffset = -camera::pos.x / map::mapX;
            
            glfwSwapBuffers(globals::window);
            ltime = ctime;
            input::clear();
        }
    }
    void handleInput()
    {
        if (input::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
            uiSystem::mouseClicked = true;
        }
        if (input::mousePressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            uiSystem::mouseRightClicked = true;
        }

        if (settingsOpen) {
            if (input::pressed(GLFW_KEY_ESCAPE)) {
                toggleSettings();
            }
            return;
        }
        auto craftingC = ECS::getComponent<uiC>(gameLoop::crafting);
        if (craftingC->hidden) {
            Player::invBlock -= input::scroll;
        }
        else {
            UI::getStat(craftingC, "craftable")->intVal += input::scroll;
        }

        if (input::mousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
            animations::watchAnim("playeredit", &Player::anim);
        }
        if (input::mouseReleased(GLFW_MOUSE_BUTTON_LEFT)) {
            animations::watchAnim("playeridle", &Player::anim);
        }
        if (input::mousePressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            animations::watchAnim("playeredit", &Player::anim);
        }
        if (input::mouseReleased(GLFW_MOUSE_BUTTON_RIGHT)) {
            animations::watchAnim("playeridle", &Player::anim);
        }

        if (input::shift) {
            if (input::pressed(GLFW_KEY_M)) {
                globals::zoom += 0.05f;
            }
            if (input::pressed(GLFW_KEY_N)) {
                globals::zoom -= 0.05f;
            }
            if (input::pressed(GLFW_KEY_R)) {
                resources::loadAssets(false, true, true);
            }
        }
        else {
            if (input::pressed(GLFW_KEY_A)) {
                animations::watchAnim("playerwalkleft", &Player::anim);
            }
            if (input::held(GLFW_KEY_A)) {
                auto vel = Player::velocity();
                if (vel->x > 0) vel->x += utils::approach(vel->x, -Player::ms, 35);
                vel->x +=  utils::approach(vel->x, -Player::ms, 60);
                Player::friction = false;
                Player::dir = -1;
                animations::watchAnim("playerwalkleft", &Player::anim, false);
            }
            if (input::released(GLFW_KEY_A)) {
                Player::friction = true;
                animations::watchAnim("playeridle", &Player::anim);
            }
            if (input::pressed(GLFW_KEY_D)) {
                animations::watchAnim("playerwalkright", &Player::anim);
            }
            if (input::held(GLFW_KEY_D)) {
                auto vel = Player::velocity();
                if (vel->x < 0) vel->x += utils::approach(vel->x, Player::ms, 35);
                vel->x += utils::approach(vel->x, Player::ms, 60);
                Player::dir = 1;
                Player::friction = false;
                animations::watchAnim("playerwalkright", &Player::anim, false);
            }
            if (input::released(GLFW_KEY_D)) {
                Player::friction = true;
                animations::watchAnim("playeridle", &Player::anim);
            }
            if (input::held(GLFW_KEY_S)) {
                if (Layers::queryBlockInfo(Layers::getLayer("blocks"), Player::pos - glm::vec2(0, Player::height / 2.0f + 0.1f))->skippable) {
                    Player::ignorebot = true;
                }
            }
            if (input::pressed(GLFW_KEY_SPACE)) {
                Player::jump();
                animations::watchAnim("playerjump", &Player::anim);
            }
            if (input::released(GLFW_KEY_SPACE)) {
                Player::currJumpTime = 0;
            }
            if (input::pressed(GLFW_KEY_T)) {
                Player::setPos(globals::mouseBlockCoordsZoomed(false));
            }
            if (input::pressed(GLFW_KEY_ESCAPE)) {
                UI::inventory->toggle();
                UI::chest->toggle(1);
                ECS::getComponent<uiC>(crafting)->hidden = UI::inventory->hidden();
                UI::hideChildren(ECS::getComponent<uiC>(equipmentC), UI::inventory->hidden());
                if (UI::inventory->hidden() == true) {
                    UI::hideChildren(ECS::getComponent<uiC>(NPCDialogue), true, true);
                }
                UI::hideChildren(ECS::getComponent<uiC>(buffcontainer), !UI::inventory->hidden());
            }
            if (input::pressed(GLFW_KEY_1)) {
                Player::invBlock = 0;
            }
            if (input::pressed(GLFW_KEY_2)) {
                Player::invBlock = 1;
            }
            if (input::pressed(GLFW_KEY_3)) {
                Player::invBlock = 2;
            }
            if (input::pressed(GLFW_KEY_4)) {
                Player::invBlock = 3;
            }
            if (input::pressed(GLFW_KEY_V)) {
                Player::editsize++;
            }
            if (input::pressed(GLFW_KEY_B)) {
                Player::editsize--;
            }
            if (input::pressed(GLFW_KEY_P)) {
                ECS::print();
            }
            if (input::pressed(GLFW_KEY_M)) {
                enemies::spawnEnemy("zombie", globals::mouseBlockCoordsZoomed());
            }
            if (input::pressed(GLFW_KEY_I)) {
            }

            if (input::pressed(GLFW_KEY_L)) {
                enemies::spawnEnemy("eyeofcthulu", globals::mouseBlockCoordsZoomed());
            }

            if (input::held(GLFW_KEY_O)) {
                map::spawnDebugSetup(globals::mouseBlockCoordsZoomed());
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
            UI::hideChildren(ECS::getComponent<uiC>(inventoryC), true);
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
        hpbarC = ECS::newEntity();
        realtimeUI = ECS::newEntity();
        settingsUI = ECS::newEntity();
        equipmentC = ECS::newEntity();
        buffcontainer = ECS::newEntity();
        pickuptextcontainer = ECS::newEntity();
        UI::addElement(hpbarC, ui_EMPTY, { 0,0 }, { 0,0 }, uiSystem::body, {}, {}, true, anchorNONE);
        UI::addElement(buffcontainer, ui_EMPTY, { 0,0 }, { 0,0 }, uiSystem::body, {}, {}, true, anchorNONE);
        UI::addElement(realtimeUI, ui_EMPTY, { 0,0 }, { 0,0 }, uiSystem::body, {}, {}, true, anchorNONE);
        UI::addElement(pickuptextcontainer, ui_EMPTY, { 0,0 }, { 0,0 }, uiSystem::body, {}, {}, true, anchorNONE);
        UI::addElement(settingsUI, ui_EMPTY, { 0,0 }, { 0,0 }, uiSystem::body, {}, {}, true, anchorNONE);
        UI::addElement(equipmentC, ui_EMPTY, { 0,0 }, { 0,0 }, uiSystem::body, {}, {}, true, anchorNONE);

        tooltip = ECS::newEntity();
        uiStat textSize; textSize.floatVal = globals::fontSize;
        uiStat padding; padding.floatVal = 0.3;
        UI::addElement(tooltip, ui_TOOLTIP, { 0,0 }, { 0,0 }, uiSystem::body, { {"textSize", textSize}, {"padding", padding} }, { {"item", "empty"} }, false, anchorNONE);
        
        cursoritem = ECS::newEntity();
        uiStat tmpstat;
        tmpstat.itemp = &UI::cursorItem;
        UI::addElement(cursoritem, ui_CURSORITEM, { 0, 0 }, { 2, 2 }, uiSystem::body, { {"item", tmpstat}, {"parent", {.intVal = globals::uiLayer}} }, { {"text", ""} }, false, anchorNONE);

        zoomedcursoritem = ECS::newEntity();
        UI::addElement(zoomedcursoritem, ui_CURSORITEM, { 0, 0 }, { 2, 2 }, uiSystem::body, { {"item", {.itemp = nullptr}}, {"parent", {.intVal = globals::topparticleLayer}} }, { {"text", ""} }, false, anchorNONE);

        inventoryC = ECS::newEntity();
        UI::addElement(inventoryC, ui_EMPTY, { 0, 0 }, { 0 , 0 }, realtimeUI);
        chestInvC = ECS::newEntity();
        UI::addElement(chestInvC, ui_EMPTY, { 0, 0 }, { 0 , 0 }, realtimeUI);

        UI::create();

        buffs::createUI();

        crafting = ECS::newEntity();
        UI::addElement(crafting, ui_CRAFTABLEITEM, { 125, -225 }, { 100,100 }, inventoryC, {}, {}, true, anchorLEFT);
        
        uiStat step; step.intVal = 20;
        uiStat sizex; sizex.intVal = 10;
        uiStat itemsize; itemsize.floatVal = 60;
        uiStat max; max.floatVal = Player::maxhp;
        uiStat value; value.floatValp = &Player::hp;
        hpbar = ECS::newEntity();
        UI::addElement(hpbar, ui_RESOURCEBAR, { -800,0 }, { 0,0 }, inventoryC, { {"step", step}, {"max", max}, {"sizex", sizex},{"value", value},{ "itemsize", itemsize} }, { {"itemtex", "heart"}, {"label", "health"}}, false, anchorTOPRIGHT);

        step.intVal = 20;
        sizex.intVal = 1;
        itemsize.floatVal = 60;
        max.floatVal = Player::maxmana;
        value.floatValp = &Player::mana;
        manabar = ECS::newEntity();
        UI::addElement(manabar, ui_RESOURCEBAR, { -100,0 }, { 30,30 }, inventoryC, { {"step", step}, {"max", max}, {"sizex", sizex},{"value", value},{ "itemsize", itemsize} }, { {"itemtex", "mana"}, {"label", "mana"}}, false, anchorTOPRIGHT);

        NPCDialogue = ECS::newEntity();
        UI::addElement(NPCDialogue, ui_TEXTBOX, { 0,-300 }, { 1,1 }, realtimeUI, { {"minwidth", {.floatVal = 1000}}, {"maxwidth", {.floatVal = 1000}}, {"padding",{.floatVal = 120}}}, {{"text", ""}}, true, anchorTOP);

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
    
        UI::addElement(ECS::newEntity(), ui_RADIAL, { 0,0 }, { 1,1 }, uiSystem::body, {}, {}, false, anchorNONE);

        cursor = ECS::newEntity();
        UI::addElement(cursor, ui_CURSOR, { 0,0 }, { 1,1 }, uiSystem::body, {}, {}, false, anchorNONE);
       
    }

    void openDialogue(std::string npcname)
    {
        auto textboxp = ECS::getComponent<uiC>(NPCDialogue);
        auto npc = &enemies::npcs[npcname];
        UI::setTStat(textboxp, "text", npc->dialogueOptions[rand() % npc->dialogueOptions.size()]);
        for (int i = 2; i < textboxp->children.size(); i++) {
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
        UI::addElement(playerrange, ui_DRAGFLOAT, { 200, -70 }, { 300,30 }, cheatsContainer, { {"textSize", textSize}, {"ref", ref}, {"max", max}, {"roundAt", roundAt} }, { {"label", "veolumes"} }, !settingsOpen);

        roundAt.floatVal = 0.04f;
        ref.floatValp = &globals::cdayTime;
        max.floatVal = 3600.0f;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { 200, -120 }, { 300,30 }, cheatsContainer, { {"textSize", textSize}, {"ref", ref}, {"max", max}, {"roundAt", roundAt} }, { {"label", "dazs"} }, !settingsOpen);

        roundAt.floatVal = 0.04f;
        ref.floatValp = &globals::zoom;
        max.floatVal = 5.0f;
        uiStat min; min.floatVal = 1.0f;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { 200, -170 }, { 300,30 }, cheatsContainer, { {"textSize", textSize}, {"ref", ref}, {"min", min}, {"max", max}, {"roundAt", roundAt} }, { {"label", "zoom"} }, !settingsOpen);


        UI::addElement(ECS::newEntity(), ui_DISPLAY, { 200, 100 }, { 200, 200 }, nekidruzgaContainer, {}, { {"tex", "dirt"} }, !settingsOpen, anchorMID);
    }

    void openMenu(int menu)
    {
        if (ECS::getComponent<uiC>(settingsMainContainer)->hidden) return;

        UI::hideChildren(ECS::getComponent<uiC>(menus), true);
        UI::hideChildren(ECS::getComponent<uiC>(menu), false);
    }

}
