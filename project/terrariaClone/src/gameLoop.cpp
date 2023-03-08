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
#include <liquids.h>

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
    int right;
    int generalContainer;
    int videoContainer;
    int interfaceContainer;
    int cursorContainer;
    int controlsContainer;
    int breathbar;
    int deathtextC;
    int droppedCoinsText;
    int guideslot;

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
        static int updateRate = 15;
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

            if (!settingsOpen) {
                Layers::updateOnScreen();
                liquids::updateOnScreen();
                if (rand() % 5000 == 0) {
                    map::placeRandomCactus();
                    std::cout << "\nplaced cactus :D\n";
                }
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

            items::info["hermesboots"].stats;

            if (!settingsOpen) {
                UI::refreshCrafting();
                Layers::spawnMobs();
                animations::updateAnims();
                game::physSys->Update();
                Player::update();
                ECS::commitQueues();
            }

            if (Player::dead) {
                UI::hideChildren(ECS::getComponent<uiC>(deathtextC), false, true);
                UI::hideChildren(ECS::getComponent<uiC>(equipmentC), true);
                UI::hideChildren(ECS::getComponent<uiC>(realtimeUI), true);
                UI::hideChildren(ECS::getComponent<uiC>(inventoryC), true);
            }
            else {
                UI::hideChildren(ECS::getComponent<uiC>(deathtextC), true, true);
            }

            globals::bgoffset = -camera::pos.x / map::mapX;
            globals::bgoffsetY = -camera::pos.y / map::mapY;

            glBindFramebuffer(GL_FRAMEBUFFER, globals::tmpFB);
            glClearColor(globals::dayclr.r, globals::dayclr.g, globals::dayclr.b, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            game::drawSys->UpdateBehindBackground();

            if(globals::background) background::render();

            game::drawSys->UpdateBehindBlocks();
            camera::updateTowardsPlayer();

            game::updateSunAndMoon(sun, moon, Player::pos);

            ECS::getComponent<drawC>(defensetext)->text = std::to_string(Player::defense);
            
            Layers::renderLayers();

            ECS::commitQueues();
            if (!settingsOpen) {
                game::aiSys->Update();
                game::toolSys->Update();
                game::droppedItemSys->Update();
                game::mobSys->Update();
                game::particleESys->Update();
                Layers::calculateLight();
            }
            game::uiSys->Update();
            ECS::commitQueues();
            game::drawSys->Update();

            
            Player::render();
            liquids::renderOnScreen();
            game::drawMain();

            glBindFramebuffer(GL_FRAMEBUFFER, globals::tmpFB);
            glClearColor(1, 1, 1, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            game::drawSys->UpdateFront();
            game::drawOverlays();

            
            glfwSwapBuffers(globals::window);
            ltime = ctime;
            input::clear();
        }
    }
    void handleInput()
    {
        if (input::pressed(k_PRIMARY)) {
            uiSystem::mouseClicked = true;
        }
        if (input::pressed(k_SECONDARY)) {
            uiSystem::mouseRightClicked = true;
        }

        if (settingsOpen) {
            if (input::pressed(k_INVENTORY)) {
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

        if (input::pressed(k_PRIMARY)) {
            animations::watchAnim("playeredit", &Player::anim);
        }
        if (input::released(k_PRIMARY)) {
            animations::watchAnim("playeridle", &Player::anim);
        }
        if (input::pressed(k_SECONDARY)) {
            animations::watchAnim("playeredit", &Player::anim);
        }
        if (input::released(k_SECONDARY)) {
            animations::watchAnim("playeridle", &Player::anim);
        }

        if (input::shift) {
            if (input::rpressed(GLFW_KEY_R)) {
                resources::loadAssets(true, true, true);
            }
        }
        else {
            if (input::pressed(k_LEFT)) {
                animations::watchAnim("playerwalkleft", &Player::anim);
            }
            if (input::held(k_LEFT)) {
                auto vel = Player::velocity();
                if (vel->x > 0) vel->x += utils::approach(vel->x, -Player::ms, 35);
                vel->x +=  utils::approach(vel->x, -Player::ms, 60);
                Player::friction = false;
                Player::dir = -1;
                animations::watchAnim("playerwalkleft", &Player::anim, false);
            }
            if (input::released(k_LEFT)) {
                Player::friction = true;
                animations::watchAnim("playeridle", &Player::anim);
            }
            if (input::pressed(k_RIGHT)) {
                animations::watchAnim("playerwalkright", &Player::anim);
            }
            if (input::held(k_RIGHT)) {
                auto vel = Player::velocity();
                if (vel->x < 0) vel->x += utils::approach(vel->x, Player::ms, 35);
                vel->x += utils::approach(vel->x, Player::ms, 60);
                Player::dir = 1;
                Player::friction = false;
                animations::watchAnim("playerwalkright", &Player::anim, false);
            }
            if (input::released(k_RIGHT)) {
                Player::friction = true;
                animations::watchAnim("playeridle", &Player::anim);
            }
            if (input::held(k_DOWN)) {
                if (Layers::queryBlockInfo(Layers::getLayer("blocks"), Player::pos - glm::vec2(0, Player::height / 2.0f + 0.1f))->skippable) {
                    Player::ignorebot = true;
                }
            }
            if (input::pressed(k_JUMP)) {
                Player::jump();
                animations::watchAnim("playerjump", &Player::anim);
            }
            if (input::released(k_JUMP)) {
                Player::currJumpTime = 0;
            }
            if (input::rpressed(GLFW_KEY_T)) {
                Player::setPos(globals::mouseBlockCoordsZoomed(false));
            }
            if (input::pressed(k_INVENTORY)) {
                if (Player::dead)
                {
                    toggleSettings();
                }
                else {
                    UI::inventory->toggle();
                    UI::chest->toggle(1);
                    bool state = UI::inventory->hidden();
                    ECS::getComponent<uiC>(crafting)->hidden = state;
                    UI::hideChildren(ECS::getComponent<uiC>(equipmentC), state);
                    if (state) {
                        UI::hideChildren(ECS::getComponent<uiC>(NPCDialogue), true, true);
                        game::droppedItemSys->dropItem(Player::pos, UI::guideItem.item, UI::guideItem.num);
                    }
                    UI::hideChildren(ECS::getComponent<uiC>(buffcontainer), !state);
                }
            }
            if (input::pressed(k_1)) {
                Player::invBlock = 0;
            }
            if (input::pressed(k_2)) {
                Player::invBlock = 1;
            }
            if (input::pressed(k_3)) {
                Player::invBlock = 2;
            }
            if (input::pressed(k_4)) {
                Player::invBlock = 3;
            }
            if (input::pressed(k_5)) {
                Player::invBlock = 4;
            }
            if (input::pressed(k_6)) {
                Player::invBlock = 5;
            }
            if (input::pressed(k_7)) {
                Player::invBlock = 6;
            }
            if (input::pressed(k_8)) {
                Player::invBlock = 7;
            }
            if (input::pressed(k_9)) {
                Player::invBlock = 8;
            }
            if (input::pressed(k_0)) {
                Player::invBlock = 9;
            }
            if (input::rpressed(GLFW_KEY_P)) {
                ECS::print();
            }
            if (input::rheld(GLFW_KEY_M)) {
                enemies::spawnEnemy("tombstone", globals::mouseBlockCoordsZoomed());
            }
            if (input::rheld(GLFW_KEY_L)) {
                liquids::place("water", globals::mouseBlockCoordsZoomed(), 3);
            }
            if (input::rheld(GLFW_KEY_U)) {
                liquids::place("lava", globals::mouseBlockCoordsZoomed(), 2);
            }
            if (input::rpressed(GLFW_KEY_K)) {
                liquids::remove(globals::mouseBlockCoordsZoomed());
            }
            if (input::rheld(GLFW_KEY_J)) {
                Layers::placeBlock(globals::mouseBlockCoordsZoomed(), "pot");
            }
            if (input::rheld(GLFW_KEY_O)) {
                map::spawnDebugSetup(globals::mouseBlockCoordsZoomed());
            }
        }
    }

    void toggleSettings()
    {
        settingsOpen = !settingsOpen;

        if (settingsOpen) {
            UI::hideChildren(ECS::getComponent<uiC>(settingsUI), false);
            UI::hideChildren(ECS::getComponent<uiC>(right), true);
            UI::hideChildren(ECS::getComponent<uiC>(generalContainer), false, true);
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

        guideslot = ECS::newEntity();
        UI::addElement(guideslot, ui_ITEMSLOT, { 225, -125 }, { 100,100 }, inventoryC, { {"item", {.itemp = &UI::guideItem}}, {"guideslot", {.boolVal = true}}, {"limitFamily", {.intVal = (int)if_MATERIAL}} }, {}, true, anchorLEFT);

        deathtextC = ECS::newEntity();
        UI::addElement(deathtextC, ui_EMPTY, { 0, 0 }, { 0,0 }, uiSystem::body, { }, {  }, false, anchorMID);
        UI::addElement(ECS::newEntity(), ui_TEXT, { 0, 0 }, { 0,0 }, deathtextC, { {"opacity", {.floatVal = 0.85}}, {"textSize", {.floatVal = globals::fontSize * 3}}, {"centered", {.boolVal = true} } }, { {"text", "\\c255100100You died!? Ugh ._."} }, false, anchorMID);
        droppedCoinsText = ECS::newEntity();
        UI::addElement(droppedCoinsText, ui_TEXT, { 0, -150 }, { 0,0 }, deathtextC, { {"opacity", {.floatVal = 0.85}}, {"textSize", {.floatVal = globals::fontSize * 1.2f}}, {"centered", {.boolVal = true}} }, { {"text", ""} }, false, anchorMID);
        UI::addElement(ECS::newEntity(), ui_TEXT, { 0, -250 }, { 0,0 }, deathtextC, { {"opacity", {.floatVal = 0.85}}, {"textSize", {.floatVal = globals::fontSize * 1.2f}}, {"centered", {.boolVal = true}}, {"textp", {.stringp = &Player::timeToRespawn}} }, { {"text", ""} }, false, anchorMID);

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
        UI::addElement(hpbar, ui_RESOURCEBAR, { -800,0 }, { 0,0 }, inventoryC, { {"step", step}, {"max", max}, {"sizex", sizex},{"value", value},{ "itemsize", itemsize} }, { {"itemtex", "heart"}, {"label", "health"} }, false, anchorTOPRIGHT);

        step.intVal = 20;
        sizex.intVal = 1;
        itemsize.floatVal = 60;
        max.floatVal = Player::maxmana;
        value.floatValp = &Player::mana;
        manabar = ECS::newEntity();
        UI::addElement(manabar, ui_RESOURCEBAR, { -100,0 }, { 30,30 }, inventoryC, { {"step", step}, {"max", max}, {"sizex", sizex},{"value", value},{ "itemsize", itemsize} }, { {"itemtex", "mana"}, {"label", "mana"} }, false, anchorTOPRIGHT);

        step.intVal = 140;
        sizex.intVal = 10;
        itemsize.floatVal = 45;
        max.floatVal = 1440;
        value.floatValp = &Player::breathtimer;
        breathbar = ECS::newEntity();
        UI::addElement(breathbar, ui_RESOURCEBAR, { -225, 175 }, {  }, inventoryC, { {"step", step}, {"max", max}, {"sizex", sizex},{"value", value},{ "itemsize", itemsize} }, { {"itemtex", "bubble"}, {"label", "mana"} }, false, anchorMID);

        NPCDialogue = ECS::newEntity();
        UI::addElement(NPCDialogue, ui_TEXTBOX, { 0,-300 }, { 1,1 }, realtimeUI, { {"minwidth", {.floatVal = 1000}}, {"maxwidth", {.floatVal = 1000}}, {"padding",{.floatVal = 120}} }, { {"text", ""} }, true, anchorTOP);

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
        UI::addElement(accessoryslot2, ui_ITEMSLOT, { -125, -100 }, { 100,100 }, equipmentC, { {"item", itemp},{"textSize", textsize},{"limitFamily", limitFamily} }, {}, true, anchorRIGHT);
        itemp.itemp = &UI::accessory3Item;
        limitFamily.intVal = if_ACCESORY;
        UI::addElement(accessoryslot3, ui_ITEMSLOT, { -125, -225 }, { 100,100 }, equipmentC, { {"item", itemp},{"textSize", textsize},{"limitFamily", limitFamily} }, {}, true, anchorRIGHT);
        itemp.itemp = &UI::accessory4Item;
        limitFamily.intVal = if_ACCESORY;
        UI::addElement(accessoryslot4, ui_ITEMSLOT, { -125, -350 }, { 100,100 }, equipmentC, { {"item", itemp},{"textSize", textsize},{"limitFamily", limitFamily} }, {}, true, anchorRIGHT);
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
        settingsMainContainer = ECS::newEntity();
        right = ECS::newEntity();
        cheatsContainer = ECS::newEntity();
        generalContainer = ECS::newEntity();
        interfaceContainer = ECS::newEntity();
        videoContainer = ECS::newEntity();
        cursorContainer = ECS::newEntity();
        controlsContainer = ECS::newEntity();
        
        uiStat textSize; textSize.floatVal = globals::fontSize * 1.2;
        UI::addElement(settingsMainContainer, ui_CONTAINER, { 0,0 }, { 0,0 }, settingsUI, { {"padding", {.floatVal = 1.5}}, {"opacity", {.floatVal = 0.87}} }, {}, !settingsOpen, anchorNONE);


        int pos = 400;
        float width = 700;
        float height = 900;
        float halfheight = height/2.0f;
        float sliderpos = pos + width / 5;
        float sliderTextSize = globals::fontSize * 0.95;

        UI::addElement(ECS::newEntity(), ui_TEXT, { 0, height/2 + 100 }, { 0,0 }, settingsMainContainer, { {"centered", {.boolVal = true}}, {"textSize", {.floatVal = globals::fontSize} } }, { {"text", "Settings Menu"} }, !settingsOpen, anchorMID);

        //------------------------------------------left-----------------------------------------------

        menus = ECS::newEntity();
        UI::addElement(menus, ui_BACK, { -pos,0 }, { width,height}, settingsMainContainer, {{"opacity", {.floatVal = 0.97}} }, {}, !settingsOpen, anchorMID);

        uiStat func; func.funcp = UI::uiCfunc_openSettingsMenu;
        uiStat menuStat; menuStat.intVal = cheatsContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { -pos, 300 }, { 200, 200 }, menus, { {"func", func}, {"fitText", {.boolVal = true} }, {"textSize", textSize}, { "menu", menuStat } }, { {"tex", "empty"}, {"text", "Cheats"} }, !settingsOpen, anchorMID);

        menuStat.intVal = generalContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { -pos, 225 }, { 200, 200 }, menus, { {"func", func}, {"fitText", {.boolVal = true} }, {"textSize", textSize}, {"menu", menuStat} }, { {"tex", "empty"}, {"text", "General"} }, !settingsOpen, anchorMID);

        menuStat.intVal = interfaceContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { -pos, 150 }, { 200, 200 }, menus, { {"func", func}, {"fitText", {.boolVal = true} }, {"textSize", textSize}, {"menu", menuStat} }, { {"tex", "empty"}, {"text", "Interface"} }, !settingsOpen, anchorMID);

        menuStat.intVal = videoContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { -pos, 75 }, { 200, 200 }, menus, { {"func", func}, {"fitText", {.boolVal = true} }, {"textSize", textSize}, {"menu", menuStat} }, { {"tex", "empty"}, {"text", "Video"} }, !settingsOpen, anchorMID);

        menuStat.intVal = cursorContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { -pos, 0}, {200, 200}, menus, {{"func", func}, {"fitText", {.boolVal = true} }, {"textSize", textSize}, {"menu", menuStat}}, {{"tex", "empty"}, {"text", "Cursor"}}, !settingsOpen, anchorMID);

        menuStat.intVal = controlsContainer;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { -pos, -75 }, { 200, 200 }, menus, { {"func", func}, {"fitText", {.boolVal = true} }, {"textSize", textSize}, {"menu", menuStat} }, { {"tex", "empty"}, {"text", "Controls"} }, !settingsOpen, anchorMID);

        UI::addElement(ECS::newEntity(), ui_BUTTON, { -pos, -150 }, { 200, 200 }, menus, { {"func", {.funcp = UI::uiCfunc_toggleSettings}}, {"fitText", {.boolVal = true} }, {"textSize", textSize} }, { {"tex", "empty"}, {"text", "Close"} }, !settingsOpen, anchorMID);

        func.funcp = UI::uiCfunc_saveAndExitGame;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { -pos, -225 }, { 200, 200 }, menus, { {"func", func}, {"fitText", {.boolVal = true} }, {"textSize", textSize} }, { {"tex", "empty"}, {"text", "Save & exit"} }, !settingsOpen, anchorMID);

        //------------------------------------------left-----------------------------------------------

        UI::addElement(right, ui_EMPTY, { 0,0 }, { 0,0 }, settingsMainContainer, {}, {}, !settingsOpen, anchorNONE);
        UI::addElement(cheatsContainer, ui_BACK, { pos,0 }, { width,height }, right, { {"opacity", {.floatVal = 0.97}} }, {}, !settingsOpen, anchorMID);
        UI::addElement(generalContainer, ui_BACK, { pos,0 }, { width,height }, right, { {"opacity", {.floatVal = 0.97}} }, {}, !settingsOpen, anchorMID);
        UI::addElement(interfaceContainer, ui_BACK, { pos,0 }, { width,height }, right, { {"opacity", {.floatVal = 0.97}} }, {}, !settingsOpen, anchorMID);
        UI::addElement(videoContainer, ui_BACK, { pos,0 }, { width,height }, right, { {"opacity", {.floatVal = 0.97}} }, {}, !settingsOpen, anchorMID);
        UI::addElement(cursorContainer, ui_BACK, { pos,0 }, { width,height }, right, { {"opacity", {.floatVal = 0.97}} }, {}, !settingsOpen, anchorMID);
        UI::addElement(controlsContainer, ui_EMPTY, { 0,0 }, { 0,0 }, right, {  }, {}, !settingsOpen, anchorMID);


        //------------------------------------------cheats-----------------------------------------------
        uiStat ref; ref.floatValp = &globals::cheaterlight;
        uiStat max; max.floatVal = 0.05f;
        uiStat roundAt; roundAt.floatVal = 0.001;
        textSize; textSize.floatVal = globals::fontSize * 0.95;
        WOW = ECS::newEntity();
        UI::addElement(WOW, ui_DRAGFLOAT, { sliderpos, 100 }, { 333,333/11 }, cheatsContainer, { {"textSize", textSize}, {"ref", ref}, {"max", max}, {"roundAt", roundAt} }, { {"tex", "slider"}, {"label", "light"} }, !settingsOpen);

        roundAt.floatVal = 0.05f;
        ref.floatValp = &Player::ms;
        max.floatVal = 10.0f;
        playerms = ECS::newEntity();
        UI::addElement(playerms, ui_DRAGFLOAT, { sliderpos, 40 }, { 333,333 / 11 }, cheatsContainer, { {"textSize", textSize}, {"ref", ref}, {"max", max}, {"roundAt", roundAt} }, { {"tex", "slider"},{"label", "ms"} }, !settingsOpen);

        roundAt.floatVal = 1.0f;
        ref.floatValp = &Player::editRange;
        max.floatVal = 40.0f;
        playerrange = ECS::newEntity();
        UI::addElement(playerrange, ui_DRAGFLOAT, { sliderpos, -20 }, { 333,333 / 11 }, cheatsContainer, { {"textSize", textSize}, {"ref", ref}, {"max", max}, {"roundAt", roundAt} }, { {"tex", "slider"},{"label", "ranger"} }, !settingsOpen);

        roundAt.floatVal = 0.04f;
        ref.floatValp = &globals::volume;
        max.floatVal = 1.0f;
        playerrange = ECS::newEntity();
        UI::addElement(playerrange, ui_DRAGFLOAT, { sliderpos, -70 }, { 333,333 / 11 }, cheatsContainer, { {"textSize", textSize}, {"ref", ref}, {"max", max}, {"roundAt", roundAt} }, { {"tex", "slider"},{"label", "veolumes"} }, !settingsOpen);

        roundAt.floatVal = 0.04f;
        ref.floatValp = &globals::cdayTime;
        max.floatVal = 3600.0f;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { sliderpos, -120 }, { 333,333 / 11 }, cheatsContainer, { {"textSize", textSize}, {"ref", ref}, {"max", max}, {"roundAt", roundAt} }, { {"tex", "slider"},{"label", "dazs"} }, !settingsOpen);


        //------------------------------------------cheats-----------------------------------------------

        textSize.floatVal = globals::fontSize * 1.2;

        //------------------------------------------general-----------------------------------------------

        UI::addElement(ECS::newEntity(), ui_TEXT, { pos, halfheight - 100 }, { 0,0 }, generalContainer, { {"centered", {.boolVal = true}}, {"textSize", {.floatVal = globals::fontSize} } }, { {"text", "Volume"} }, !settingsOpen, anchorMID);
        
        ref.floatValp = &globals::volume;
        max.floatVal = 1.0f;
        roundAt.floatVal = 0.04f;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { sliderpos, halfheight - 175 }, { 333,333 / 11 }, generalContainer, { {"textSize", {.floatVal = sliderTextSize}}, {"ref", ref}, {"max", max}, {"roundAt", roundAt} }, { {"tex", "slider"},{"label", "Master: "} }, true);

        UI::addElement(ECS::newEntity(), ui_TEXT, { pos, halfheight - 375 }, { 0,0 }, generalContainer, { {"centered", {.boolVal = true}}, {"textSize", {.floatVal = globals::fontSize} } }, { {"text", "Zoom"} }, !settingsOpen, anchorMID);
        
        max.floatVal = 5.0f;
        roundAt.floatVal = .09f;
        ref.floatValp = &globals::zoom;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { sliderpos, halfheight - 450 }, { 333,333 / 11 }, generalContainer, { {"textSize",  {.floatVal = sliderTextSize}}, {"ref", ref}, {"max", max}, {"min", {.floatVal = 1.0f}}, {"roundAt", roundAt} }, { {"tex", "slider"},{"label", "Main: "} }, true);

        //------------------------------------------general-----------------------------------------------
        
        //------------------------------------------interface-----------------------------------------------

        UI::addElement(ECS::newEntity(), ui_TOGGLE, { pos, halfheight - 75 }, { 0, 0 }, interfaceContainer, { {"ref", {.boolValp = &globals::pickuptext}}, {"textSize", textSize} }, { {"label", "Pickuptext"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_TOGGLE, { pos, halfheight - 150 }, { 0, 0 }, interfaceContainer, { {"ref", {.boolValp = &globals::tilegrid}}, {"textSize", textSize} }, { {"label", "Tilegrid"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_TOGGLE, { pos, halfheight - 225 }, { 0, 0 }, interfaceContainer, { {"ref", {.boolValp = &globals::hovertext}}, {"textSize", textSize} }, { {"label", "Hovertext"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_TOGGLE, { pos, halfheight - 300 }, { 0, 0 }, interfaceContainer, { {"ref", {.boolValp = &globals::particles}}, {"textSize", textSize} }, { {"label", "Particles"} }, true, anchorMID);

        //------------------------------------------interface-----------------------------------------------

        //------------------------------------------video----------------------------------------------- 
        func.funcp = UI::uiCfunc_toggleFullscreen;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { pos, halfheight - 75 }, { 0, 0 }, videoContainer, { {"func", func}, {"textSize", textSize}, {"fitText", {.boolVal = true}} }, { {"tex", "empty"}, {"text", "Toggle fullscreen"} }, true, anchorMID);

        func.funcp = UI::uiCfunc_nextResolution;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { pos, halfheight - 150 }, { 0, 0 }, videoContainer, { {"func", func}, {"textSize", textSize}, {"fitText", {.boolVal = true}}, {"textp", {.stringp = &game::currResText}} }, { {"tex", "empty"}, {"text", ""} }, true, anchorMID);

        UI::addElement(ECS::newEntity(), ui_TOGGLE, { pos, halfheight - 225 }, { 0, 0 }, videoContainer, { {"ref", {.boolValp = &globals::background}}, {"textSize", textSize} }, { {"label", "Background"} }, true, anchorMID);

        //------------------------------------------video-----------------------------------------------

        //------------------------------------------cursor-----------------------------------------------
        
        UI::addElement(ECS::newEntity(), ui_TEXT, { sliderpos, halfheight - 75 }, { 333,333 / 11 }, cursorContainer, { {"textSize", textSize}, {"centered", {.boolVal = true}} }, { {"text", "Border"} }, true);

        max.floatVal = 360;
        uiStat min; min.floatVal = 0;
        ref.floatValp = &globals::cursorborderhue;
        roundAt.intVal = 1;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { sliderpos, halfheight - 150 }, { 333,333 / 11 }, cursorContainer, { {"textSize", textSize}, {"ref", ref}, {"max", max}, {"min", min}, {"roundAt", roundAt} }, { {"tex", "sliderrainbow"},{"label", ""} }, true);

        max.floatVal = 1;
        min.floatVal = 0;
        ref.floatValp = &globals::cursorbordersaturation;
        roundAt.intVal = 0.01;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { sliderpos, halfheight - 225 }, { 333,333 / 11 }, cursorContainer, { {"textSize", textSize}, {"ref", ref}, {"max", max}, {"min", min}, {"roundAt", roundAt} }, { {"tex", "slider"},{"label", ""} }, true);

        max.floatVal = 1;
        min.floatVal = 0;
        ref.floatValp = &globals::cursorbordervalue;
        roundAt.intVal = 0.01;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { sliderpos, halfheight - 300 }, { 333,333 / 11 }, cursorContainer, { {"textSize", textSize}, {"ref", ref}, {"max", max}, {"min", min}, {"roundAt", roundAt} }, { {"tex", "slider"},{"label", ""} }, true);

        UI::addElement(ECS::newEntity(), ui_TEXT, { sliderpos, halfheight - 375 }, { 333,333 / 11 }, cursorContainer, { {"textSize", textSize}, {"centered", {.boolVal = true}} }, { {"text", "Cursor"} }, true);

        max.floatVal = 360;
        min.floatVal = 0;
        ref.floatValp = &globals::cursorhue;
        roundAt.intVal = 0.1;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { sliderpos, halfheight - 450 }, { 333,333 / 11 }, cursorContainer, { {"textSize", textSize}, {"ref", ref}, {"max", max}, {"min", min}, {"roundAt", roundAt} }, { {"tex", "sliderrainbow"},{"label", ""} }, true);

        max.floatVal = 1;
        min.floatVal = 0;
        ref.floatValp = &globals::cursorsaturation;
        roundAt.intVal = 0.01;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { sliderpos, halfheight - 525 }, { 333,333 / 11 }, cursorContainer, { {"textSize", textSize}, {"ref", ref}, {"max", max}, {"min", min}, {"roundAt", roundAt} }, { {"tex", "slider"},{"label", ""} }, true);

        max.floatVal = 1;
        min.floatVal = 0;
        ref.floatValp = &globals::cursorvalue;
        roundAt.intVal = 0.01;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { sliderpos, halfheight - 600 }, { 333,333 / 11 }, cursorContainer, { {"textSize", textSize}, {"ref", ref}, {"max", max}, {"min", min}, {"roundAt", roundAt} }, { {"tex", "slider"},{"label", ""} }, true);

        max.floatVal = 5;
        min.floatVal = 1;
        ref.floatValp = &globals::cursorsize;
        roundAt.intVal = 0.01;
        UI::addElement(ECS::newEntity(), ui_DRAGFLOAT, { sliderpos, halfheight - 675 }, { 333,333 / 11 }, cursorContainer, { {"textSize", textSize}, {"ref", ref}, {"max", max}, {"min", min}, {"roundAt", roundAt} }, { {"tex", "slider"},{"label", "Size: "} }, true);

       
        //------------------------------------------cursor-----------------------------------------------

        //------------------------------------------controls-----------------------------------------------
        
        int container = ECS::newEntity();
        UI::addElement(container, ui_CONTAINER, { 0, 0 }, { 0, 0 }, controlsContainer, { {"padding", {.floatVal = 2.5} }, {"opacity", {.floatVal = 0.8}} }, {}, true, anchorNONE);
        int container2 = ECS::newEntity();
        UI::addElement(container2, ui_CONTAINER, { 0, 0 }, { 0, 0 }, container, { {"padding", {.floatVal = 1} }, {"colorp", {.vec3p = &globals::backcolor2}}, {"opacity", {.floatVal = 0.8}} }, {}, true, anchorNONE);
        int kw = 800;
        int kh = 75;
        int km = 15;
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { -(kw / 2 + km / 2), 500 }, { kw, kh }, container2, { {"key", {.intVal = k_PRIMARY}}, {"textSize", textSize}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Use item"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { (kw / 2 + km / 2), 500 }, { kw, kh }, container2, { {"key", {.intVal = k_SECONDARY}}, {"textSize", textSize}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Interact"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { -(kw / 2 + km / 2), 400 }, { kw, kh }, container2, { {"key", {.intVal = k_MIDDLE}}, {"textSize", textSize}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Middle Mouse"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { (kw / 2 + km / 2), 400 }, { kw, kh }, container2, { {"key", {.intVal = k_MOUSE4}}, {"textSize", textSize}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Mouse 4"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { -(kw / 2 + km / 2), 300 }, { kw, kh }, container2, { {"key", {.intVal = k_MOUSE5}}, {"textSize", textSize}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Mouse 5"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { (kw / 2 + km / 2), 300 }, { kw, kh }, container2, { {"key", {.intVal = k_UP}}, {"textSize", textSize}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Up"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { -(kw / 2 + km / 2), 200 }, { kw, kh }, container2, { {"key", {.intVal = k_DOWN}}, {"textSize", textSize}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Down"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { (kw / 2 + km / 2), 200 }, { kw, kh }, container2, { {"key", {.intVal = k_LEFT}}, {"textSize", textSize}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Left"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { -(kw / 2 + km / 2), 100 }, { kw, kh }, container2, { {"key", {.intVal = k_RIGHT}}, {"textSize", textSize}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Right"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { (kw / 2 + km / 2), 100 }, { kw, kh }, container2, { {"key", {.intVal = k_JUMP}}, {"textSize", textSize}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Jump"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { -(kw / 2 + km / 2), 0 }, { kw, kh }, container2, { {"key", {.intVal = k_HEAL}}, {"textSize", textSize}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Quick Heal"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { (kw / 2 + km / 2), 0 }, { kw, kh }, container2, { {"key", {.intVal = k_MANA}}, {"textSize", textSize}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Quick Mana"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { -(kw / 2 + km / 2), -100 }, { kw, kh }, container2, { {"key", {.intVal = k_THROW}}, {"textSize", textSize}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Throw"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { (kw / 2 + km / 2), -100 }, { kw, kh }, container2, { {"key", {.intVal = k_INVENTORY}}, {"textSize", textSize}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Inventory"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { -(kw / 2 + km / 2), -200 }, { kw, kh }, container2, { {"key", {.intVal = k_ZOOMIN}}, {"textSize", textSize}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Zoom In"} }, true, anchorMID);
        UI::addElement(ECS::newEntity(), ui_KEYBIND, { (kw / 2 + km / 2), -200 }, { kw, kh }, container2, { {"key", {.intVal = k_ZOOMOUT}}, {"textSize", textSize}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex","keybindbg"}, {"text", "Zoom Out"} }, true, anchorMID);

        func; func.funcp = UI::uiCfunc_resetKeybinds;
        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, -300 }, { kw, 75 }, container2, { {"func", func}, {"textSize", textSize}, {"color", {.vec3p = &globals::keybindcolor}} }, { {"tex", "keybindbg"}, {"text", "Reset to Default"} }, true, anchorMID);

        UI::addElement(ECS::newEntity(), ui_BUTTON, { 0, -600 }, { 200, 200 }, controlsContainer, { {"func", {.funcp = UI::uiCfunc_toggleSettings}}, {"fitText", {.boolVal = true} }, {"textSize", textSize} }, { {"tex", "empty"}, {"text", "Close"} }, !settingsOpen, anchorMID);



        //------------------------------------------controls-----------------------------------------------
    }

    void respawned()
    {
        settingsOpen = false;

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

    void openMenu(int menu)
    {
        if (ECS::getComponent<uiC>(settingsMainContainer)->hidden) return;
        if (menu == controlsContainer) {
            UI::hideChildren(ECS::getComponent<uiC>(settingsUI), true, true);
            UI::hideChildren(ECS::getComponent<uiC>(menu), false, true);
            return;
        }
        UI::hideChildren(ECS::getComponent<uiC>(right), true);
        UI::hideChildren(ECS::getComponent<uiC>(menu), false, true);
    }

}
