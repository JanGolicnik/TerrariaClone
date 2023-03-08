#include "Player.h"

#include <utils.h>
#include <blocks.h>
#include <animations.h>
#include <ui.h>
#include <componentsystems.h>
#include <globals.h>
#include <textures.h>
#include <camera.h>
#include <world.h>
#include <particles.h>
#include <input.h>
#include <buffs.h>
#include <gameLoop.h>
#include <liquids.h>
#include <game.h>

namespace Player {

    const int numsprites = 5;
    std::string name = "";
    glm::vec2 vel = glm::vec2(0.0f);
    glm::vec2 tvel;
    int dir = -1.0f;
    bool rightClick = false;
    bool leftClick = false;
    int invBlock = 0;
    SpriteVertex vertices[numsprites * 4];
    glm::vec2 pos = glm::vec2(0, 0);
    GLuint playerVB;
    GLuint playerVA;
    GLuint playerIB;
    glm::mat4 playerTrans = glm::mat4(1.0f);
    float width = 2.0f;
    float height = 2.9f;
    glm::vec2 center = { pos.x + width / 2, pos.y + height / 2 };
    int anim;
    float craftingDist = 8;

    float ms = 0.3f;
    float msBASE = 0.3f;
    float jumpms = 0.5f;
    float jumpmsBASE = 0.5f;
    int editsize = 1;
    int editsizeBASE = 1;
    float editRange = 11.0f;
    float editRangeBASE = 11.0f;
    float pickupRange = 5.2f;
    float pickupRangeBASE = 5.2f;
    float hp = 100;
    float currmaxhp = 100;
    float currmaxhpBASE = 100;
    float mana = 20;
    float currmaxmana = 20;
    float currmaxmanaBASE = 20;
    int defense = 0;
    float jumpSpeed = 0.28;
    float jumpSpeedBASE = 0.28;
    int jumpDuration = 16;
    int jumpDurationBASE = 16;
    float regeneration = 0;
    int regentimer = 0;
    int manaregentimer = 0;
    float manaregeneration = 0;

    float maxhp = 400;
    float maxmana = 200;
    bool ignorebot;
    bool collidetop;
    bool collideright;
    bool collideleft;
    int entity;
    bool wantstojump = false;
    int currJumpTime = 0;
    int stoptimer = 0;
    bool friction = true;
    int iframes = 0;
    float itemtimer = 0;
    std::string curritem = "";

    int boomerangsout = 0;
    int tool = -1;
    struct ExtraJump {
        std::string effect;
        int jumpDuration;
    };

    float maxsummons = 1;
    float currsummons = 0;

    std::vector<ExtraJump> extrajump;
    std::vector<ExtraJump> availablejumps;

    std::array<ActiveBuff, 20> buffs;

    bool meeleAutoswing = false;
    float meeleAttackSpeed = 1;
    float meeleDamage = 1;
    float thorns = 0;

    int heartcrystals;

    float enemyChance;
    float critterChance;

    bool wantToAutoUse = false;
    std::string activeSetBonus = "";

    float hue = 0;

    float breathtimer = 2000;

    bool dead = false;
    int deathtimer = 0;

    std::string timeToRespawn;

    void update()
    {
        auto bs = Layers::getLayer("blocks");
        auto phys = ECS::getComponent<physicsC>(entity);

        if (dead) {
            hp = 0;
            mana = 0;
            timeToRespawn = "\\c255100100Respawning in: " + std::to_string((int)deathtimer / 60) + " seconds";
            for (int i = 0; i < buffs.size(); i++) {
                buffs[i] = {"nothing", -1, globals::time};
            }
            if (deathtimer <= 0) {
                timeToRespawn = "";
                *phys->position = map::PlayerSpawn;
                dead = false;
                hp = 100;
                mana = currmaxmana;
                gameLoop::respawned();
            }
            deathtimer--;
            return;
        }
        dead = false;

        hp += regeneration / 60;
        mana += manaregeneration / 60;

        iframes--;
        if (mana > currmaxmana) {
            mana = currmaxmana;
        }
        if (hp > currmaxhp) {
            hp = currmaxhp;
        }
        if (hp < 0) {
            dead = true;
            deathtimer = 600;
            dropCoins();
            return;
        }

        itemtimer--;
        phys->ignorebot = ignorebot;
        ignorebot = false;
        center = pos + glm::vec2(width / 2, height / 2);
        phys->weight = 1;
        vel = phys->vel;
        phys->stoponcollision = true;
        phys->friction = friction;
        bool goUp = true;
        if (phys->vel.y == 0 && phys->vel.x != 0) {
            if (!Layers::queryBlockInfo(bs, *phys->position + glm::vec2(0, phys->size.y / 2.0f + 1))->collidableBot) {
                if (phys->vel.x < 0) {
                    for (int i = 0; i < height - 1; i++) {
                        if (Layers::queryBlockInfo(bs, *phys->position + glm::vec2(-(phys->size.x / 2.0f + 0.2), -phys->size.y / 2.0f + 1 + i))->collidableRight) {
                            goUp = false;
                            break;
                        }
                    }
                    if (goUp) {
                        if (Layers::queryBlockInfo(bs, *phys->position + glm::vec2(-1 * (phys->size.x / 2.0f + 0.2), -phys->size.y / 2.0f))->collidableRight) {
                            phys->weight = 0;
                            phys->stoponcollision = false;
                            phys->position->y += 0.26;
                        }
                    }
                }
                else {
                    for (int i = 0; i < height - 1; i++) {
                        if (Layers::queryBlockInfo(bs, *phys->position + glm::vec2((phys->size.x / 2.0f + 0.2), -phys->size.y / 2.0f + 1 + i))->collidableLeft) {
                            goUp = false;
                            break;
                        }
                    }
                    if (goUp) {
                        if (Layers::queryBlockInfo(bs, *phys->position + glm::vec2((phys->size.x / 2.0f + 0.2), -phys->size.y / 2.0f))->collidableLeft) {
                            phys->weight = 0;
                            phys->stoponcollision = false;
                            phys->position->y += 0.26;
                        }
                    }
                }
            }
        }

        if (wantstojump) {
            if (phys->collidebot) {
                currJumpTime = jumpDuration;
                availablejumps = extrajump;
            }
            else if (availablejumps.size() > 0) {
                particles::spawnEffect(availablejumps[0].effect, Player::pos - glm::vec2(0, height / 2));
                currJumpTime = availablejumps[0].jumpDuration;
                availablejumps.erase(availablejumps.begin());
            }
            wantstojump = false;
        }

        if (currJumpTime > 0) {
            phys->vel.y = jumpSpeed + globals::gravity;
        }
        currJumpTime--;

        if (invBlock > 9) invBlock = invBlock % 10;
        if (invBlock < 0) invBlock = 10 + invBlock;
        UI::hotbar->markAsSelected(invBlock);
        static std::string lastitem = "";
        lastitem = curritem;
        curritem = UI::hotbar->at({ invBlock, 0 })->item;
        itemInfo* curriteminfo = items::getInfo(curritem);
        Layers::addLight(pos, curriteminfo->light);
        itemtimer--;
        if (itemtimer < 0) itemtimer = 0;
        pos = *phys->position;

        int topleft = liquids::at(round(* phys->position + glm::vec2(-phys->size.x / 4, phys->size.y / 2)));
        int topright = liquids::at(round(* phys->position + glm::vec2(phys->size.x / 4, phys->size.y / 2)));
        if (topleft != 0x00000000u || topright != 0x00000000u) {
            if (breathtimer > 1440) breathtimer = 1440;
            breathtimer--;
            ECS::getComponent<uiC>(gameLoop::breathbar)->stats["hide"].boolVal = false;
        }
        else {
            breathtimer+=10;
            if (breathtimer >= 1440) {
                ECS::getComponent<uiC>(gameLoop::breathbar)->stats["hide"].boolVal = true;
            }
        }
        
        if (breathtimer < 0) {
            hp -= 2;
        }

        if (iframes < 0) {
            if (phys->isinliquid == 1) {
                hp -= 50;
                iframes = globals::iframes;
                addBuff("onfire", 180);
            }
        }

    }
    void doPrimary(std::string item)
    {
        auto info = items::getInfo(item);
        wantToAutoUse = true;
        while (itemtimer < 1) {
            useItem(item, info);
        }
    }
    void doPrimaryHold(std::string item)
    {
        auto info = items::getInfo(item);
        bool canAutouse = info->autouse;
        if (info->families.count(if_MEELE) >= 1 && meeleAutoswing) {
            canAutouse = true;
        }

        if (wantToAutoUse && canAutouse) {
            while (itemtimer < 1) {
                useItem(item, info);
            }
        }
        else {
            wantToAutoUse = false;
        }
    }
    void doSecondary(std::string item)
    {
        glm::vec2 vec = globals::mouseBlockCoordsZoomed();
        if (input::pressed(k_SECONDARY)) {
            Layers::doBlockFunction(vec);
            itemtimer = 20;
        }
    }

    void render()
    {
        if (dead) return;
        glUseProgram(globals::spriteShaderID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures::spriteSheet);
        glUniform1i(2, 0);
        glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(camera::trans));
        glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f)));

        glBindVertexArray(playerVA);
        glBindBuffer(GL_ARRAY_BUFFER, playerVB);
        bool flip = dir < 0 ? true : false;
        glm::vec3 light =  Layers::queryBlock(Layers::getLayer("blocks"), pos)->light;
        
        memset(vertices, 0, numsprites * 4 * sizeof(SpriteVertex));
        memcpy(vertices, utils::CreateSpriteRect(glm::vec3(pos, 0), animations::getFrame(anim), glm::vec2(width, height), glm::vec4(utils::hsvToRgb(glm::vec3(hue, 1, 1)) * light,1), flip).data(), 4 * sizeof(SpriteVertex));
        memcpy(vertices + 4, utils::CreateSpriteRect(glm::vec3(pos + glm::vec2(0, 1), 0), textures::nametocoords[UI::helmetItem.item]->coords, glm::vec2(1, 1), glm::vec4(light, 1), flip).data(), 4 * sizeof(SpriteVertex));
        memcpy(vertices + 8, utils::CreateSpriteRect(glm::vec3(pos + glm::vec2(0, 0), 0), textures::nametocoords[UI::breastplateItem.item]->coords, glm::vec2(2, 2), glm::vec4(light, 1), flip).data(), 4 * sizeof(SpriteVertex));
        memcpy(vertices + 12, utils::CreateSpriteRect(glm::vec3(pos + glm::vec2(0, -1), 0), textures::nametocoords[UI::greavesItem.item]->coords, glm::vec2(1, 1), glm::vec4(light, 1), flip).data(), 4 * sizeof(SpriteVertex));
        
        glBufferSubData(GL_ARRAY_BUFFER, 0, numsprites * 4 * sizeof(SpriteVertex), vertices);
        glDrawElements(GL_TRIANGLES, numsprites * 6, GL_UNSIGNED_INT, 0);
    }
    void clean()
    {
        glDeleteBuffers(1, &playerVB);
        glDeleteBuffers(1, &playerVA);
        glDeleteBuffers(1, &playerIB);
    }
    void reset() {
        UI::accessory1Item.item = "empty";
        UI::accessory2Item.item = "empty";
        UI::accessory3Item.item = "empty";
        UI::accessory4Item.item = "empty";
        UI::accessory5Item.item = "empty";
        UI::helmetItem.item = "empty";
        UI::breastplateItem.item = "empty";
        UI::greavesItem.item = "empty";
    }
    void calculateStats()
    {
        meeleAutoswing = false;
        ms = msBASE;
        jumpms = jumpmsBASE;
        editsize = editsizeBASE;
        pickupRange = pickupRangeBASE;
        currmaxhp = currmaxhpBASE + heartcrystals*20;
        currmaxmana = currmaxmanaBASE;
        jumpSpeed = jumpSpeedBASE;
        jumpDuration = jumpDurationBASE;
        defense = 0;
        meeleAttackSpeed = 1;
        enemyChance = 1;
        critterChance = 1;
        meeleDamage = 1;
        thorns = 0;
        extrajump.clear();

        regentimer++;
        manaregentimer++;
        regeneration = 0.5 * ((currmaxhp / 400) * 0.85 + 0.15);
        float regenmod = 0;
        if (regentimer < 0) {
        }else
        if (regentimer < 30 * 60) {
            regenmod = regentimer / 60.0f;
            regenmod /= 5;
            regenmod = floor(regenmod);
        }
        else {
            regenmod = 6;
            float help = regentimer - 30 * 60.0f;
            help /= 60;
            help = floor(help / 10);
            regenmod += help;
        }
        regeneration *= regenmod;

        manaregeneration = 0;
        if (manaregentimer > 0) {
            if(vel.x != 0) {
                manaregeneration = (currmaxmana / 7 + 1) * (mana / currmaxmana * 0.8 + 0.2) * 1.15;
            }
            else {
                manaregeneration = (currmaxmana / 7 + 1 + currmaxmana/2) * (mana / currmaxmana * 0.8 + 0.2) * 1.15;
            }
        }

        addStatsFromItem(UI::breastplateItem.item);
        addStatsFromItem(UI::greavesItem.item);
        addStatsFromItem(UI::helmetItem.item);
        addStatsFromItem(UI::accessory1Item.item);
        addStatsFromItem(UI::accessory2Item.item);
        addStatsFromItem(UI::accessory3Item.item);
        addStatsFromItem(UI::accessory4Item.item);
        addStatsFromItem(UI::accessory5Item.item);

        if (items::info[UI::breastplateItem.item].set == items::info[UI::greavesItem.item].set && items::info[UI::greavesItem.item].set == items::info[UI::helmetItem.item].set) {
            items::setBonuses[items::info[UI::breastplateItem.item].set].func();
            activeSetBonus = items::info[UI::breastplateItem.item].set;
        }
        else {
            activeSetBonus = "";
        }

        if (velocity()->x == 0) {
            regeneration *= 1.25;
        }
        else {
            regeneration *= 0.5;
        }

        auto phys = ECS::getComponent<physicsC>(entity);
        for (int i = 0; i < buffs.size(); i++) {
            buffs::buffs[buffs[i].name].func(&buffs[i], nullptr, phys);
            buffs[i].countdown--;
            if (buffs[i].countdown == 0) {
                buffs[i].name = "nothing";
            }
        }
    }
    void addStatsFromItem(std::string item)
    {
        defense += items::getStat(item, "defense", { .valueInt = 0 }).valueInt;
        ms += items::getStat(item, "ms", { .valueFloat = 0 }).valueFloat;
        ms *= items::getStat(item, "mspercent", { .valueFloat = 1 }).valueFloat;
        currmaxmana += items::getStat(item, "mana", { .valueInt = 0 }).valueInt;
        regeneration += items::getStat(item, "regen", { .valueFloat = 0 }).valueFloat;
        currmaxhp += items::getStat(item, "hp", { .valueInt = 0 }).valueInt;
        meeleAutoswing = meeleAutoswing || items::getStat(item, "meeleautoswing", { .valueBool = false }).valueBool;
        meeleAttackSpeed += items::getStat(item, "meeleas", { .valueFloat = 0 }).valueFloat;
        meeleDamage += items::getStat(item, "meeledamagepercent", { .valueFloat = 0 }).valueFloat;

        glm::vec3 light = items::getStat(item, "lightstat", { .valueVec3 = glm::vec3(0) }).valueVec3;
        if (light != glm::vec3(0)) {
            Layers::addLight(round(Player::pos), light);
        }

        std::string str;
        str = items::getStatT(item, "extrajump", "");
        if (str != "") {
            int duration = items::getStat(item, "extrajumpDuration", { .valueInt = jumpDurationBASE }).valueInt;
            extrajump.push_back({ str, duration });
        }
    }
    bool save()
    {
        std::string filename = "players/" + name + ".bak";
        remove(filename.c_str());
        std::ofstream file(filename, std::ios::out | std::ios::binary);
        if (!file) { std::cout << "error opening file for saving player\n"; return false; }

        file.write((char*)(&hp), sizeof(hp));
        file.write((char*)(&currmaxhp), sizeof(currmaxhp));
        file.write((char*)(&mana), sizeof(mana));
        file.write((char*)(&currmaxmana), sizeof(currmaxmana));
        file.write((char*)(&heartcrystals), sizeof(heartcrystals));
        file.write((char*)(&hue), sizeof(hue));

        UI::saveInvItemVector(&UI::PlayerHotbar, &file);
        UI::saveInvItemVector(&UI::PlayerInventory, &file);

        UI::saveInvItem(&UI::helmetItem, &file);
        UI::saveInvItem(&UI::breastplateItem, &file);
        UI::saveInvItem(&UI::greavesItem, &file);

        UI::saveInvItem(&UI::accessory1Item, &file);
        UI::saveInvItem(&UI::accessory2Item, &file);
        UI::saveInvItem(&UI::accessory3Item, &file);
        UI::saveInvItem(&UI::accessory4Item, &file);
        UI::saveInvItem(&UI::accessory5Item, &file);

        file.close();
        return true;
    }
    bool load()
    {
        std::string filename = "players/" + name + ".bak";
        std::ifstream file(filename, std::ios::out | std::ios::binary);
        if (!file) { std::cout << "error opening file for loading player\n"; return false; }

        file.read((char*)(&hp), sizeof(hp));
        file.read((char*)(&currmaxhp), sizeof(currmaxhp));
        file.read((char*)(&mana), sizeof(mana));
        file.read((char*)(&currmaxmana), sizeof(currmaxmana));
        file.read((char*)(&heartcrystals), sizeof(heartcrystals));
        file.read((char*)(&hue), sizeof(hue));

        UI::loadInvItemVector(&UI::PlayerHotbar, &file);
        UI::loadInvItemVector(&UI::PlayerInventory, &file);

        UI::loadInvItem(&UI::helmetItem, &file);
        UI::loadInvItem(&UI::breastplateItem, &file);
        UI::loadInvItem(&UI::greavesItem, &file);

        UI::loadInvItem(&UI::accessory1Item, &file);
        UI::loadInvItem(&UI::accessory2Item, &file);
        UI::loadInvItem(&UI::accessory3Item, &file);
        UI::loadInvItem(&UI::accessory4Item, &file);
        UI::loadInvItem(&UI::accessory5Item, &file);

        file.close();
        Player::currsummons = 0;
        return true;
    }
    void useItem(std::string item, itemInfo* iteminfo)
    {
        if (dead) return;
        glm::vec2 vec = globals::mouseBlockCoordsZoomed();

        for (auto& cond : items::getInfo(item)->conditions) {
            if (!cond(item)) {
                itemtimer = 1;
                return;
            }
        }
        tool = -1;

        auto info = items::getInfo(item);
        float usetime = info->useSpeed;
        float damagemult = 1;
        if (info->families.count(if_MEELE) >= 1) {
            usetime /= meeleAttackSpeed;
            damagemult *= meeleDamage;
        }
        itemtimer += usetime;

        glm::vec2 pos = Player::pos;
        vec -= pos;
        float angle = int(utils::angleOfVector(vec) - 180) % 360;
        pos.x += cos(angle * PI / 180.0f) * 2;
        pos.y += sin(angle * PI / 180.0f) * 2;

        if (itemtimer > 0) {
            tool = ECS::newEntity();
            toolSystem::makeTool(tool, pos, item, ceil(itemtimer), glm::rotate(glm::mat4(1.0f), float(45 * PI / 180.0f), glm::vec3(0, 0, 1)));
        }
        for (auto& func : iteminfo->onleftclick) {
            func(item, tool);
        }
    }
    void setPos(glm::vec2 val)
    {
        *ECS::getComponent<physicsC>(entity)->position = val;
    }
    void setVel(glm::vec2 val)
    {
        ECS::getComponent<physicsC>(entity)->vel = val;
    }
    void jump() {
        wantstojump = true;
    }
    glm::vec2* position() {
        return ECS::getComponent<physicsC>(entity)->position.get();
    }
    glm::vec2* velocity() {
        return &ECS::getComponent<physicsC>(entity)->vel;
    }
    void addBuff(std::string name, int timer)
    {
        if (timer == 0) return;
        for (int i = 0; i < buffs.size(); i++) {
            if (buffs[i].name == "nothing" || buffs[i].name == name) {
                buffs[i].name = name;
                if(buffs[i].countdown < timer) buffs[i].countdown = timer;
                buffs[i].starttime = globals::time;
                return;
            }
        }

        buffs[0].name = name;
        buffs[0].countdown = timer;
    }
    bool hasBuff(std::string name)
    {
        for (int i = 0; i < buffs.size(); i++) {
            if (buffs[i].name == name) {
                return true;
            }
        }
        return false;
    }
    int pickUp(std::string item, int num)
    {
        int prevamount = num;
        num = UI::hotbar->add(item, num);
        num = UI::inventory->add(item, num);
        if (prevamount - num > 0) {
            bool found = false;
            if (globals::pickuptext) {
                auto container = ECS::getComponent<uiC>(gameLoop::pickuptextcontainer);
                for (int i = 0; i < container->children.size(); i++) {
                    auto child = ECS::getComponent<uiC>(container->children[i]);
                    if (UI::getTStat(child, "text") == item) {
                        UI::getStat(child, "num")->intVal += prevamount - num;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    int e = ECS::newEntity();
                    UI::addElement(e, ui_PICKUPTEXT, Player::center, { 0,0 }, gameLoop::pickuptextcontainer, { {"num", {.intVal = prevamount - num }} }, { {"text", item} }, false, anchorNONE);
                    ECS::getComponent<drawC>(e)->color = items::getRarityColor(items::getInfo(item)->rarity);
                }
            }
            auto func = &items::info[item].onpickup;
            if (*func) {
                (*func)();
            }
        }

        return num;
    }
    void dropCoins()
    {
        int numPlat = 0;
        int numSilver = 0;
        int numGold = 0;
        int numCopper = 0;
        for (auto& i : *UI::inventory->items) {
            if (i->item == "platinumcoin" || i->item == "goldcoin" || i->item =="silvercoin" ||i->item == "coppercoin") {
                game::droppedItemSys->dropItem(pos, i->item, ceil(i->num/2.0f), false);
                if (i->item == "platinumcoin") {
                    numPlat += ceil(i->num / 2.0f);
                }else if(i->item == "platinumcoin") {
                    numGold += ceil(i->num / 2.0f);
                }else if (i->item == "platinumcoin") {
                    numSilver += ceil(i->num / 2.0f);
                }
                else {
                    numCopper += ceil(i->num / 2.0f);
                }
                i->num /= 2;
            }
        }
        std::string text = "\\c255100100";
        if (numPlat + numSilver + numGold + numCopper == 0) {
            text += "You didnt drop any coins :D";
        }
        else {
            text += "You dropped " + (numPlat > 0 ? (std::to_string(numPlat) + " platinum ") : "")
                + (numGold > 0 ? (std::to_string(numGold) + " gold ") : "")
                + (numSilver > 0 ? (std::to_string(numSilver) + " silver ") : "")
                + (numCopper > 0 ? (std::to_string(numCopper) + " copper ") : "") + " coins :(";
        }

        UI::setTStat(ECS::getComponent<uiC>(gameLoop::droppedCoinsText), "text", text);
    }
    void create()
    {
        glCreateVertexArrays(1, &playerVA);
        glBindVertexArray(playerVA);

        glCreateBuffers(1, &playerVB);
        glBindBuffer(GL_ARRAY_BUFFER, playerVB);
        glBufferData(GL_ARRAY_BUFFER, numsprites * 4 * sizeof(SpriteVertex), nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, Position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, SpriteCoords));
        
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, color));

        uint32_t indices[numsprites * 6];
        int offset = 0;
        for (int i = 0; i < numsprites * 6; i += 6) {
            indices[i + 0] = 0 + offset;
            indices[i + 1] = 1 + offset;
            indices[i + 2] = 3 + offset;

            indices[i + 3] = 1 + offset;
            indices[i + 4] = 2 + offset;
            indices[i + 5] = 3 + offset;

            offset += 4;
        }

        glCreateBuffers(1, &playerIB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, playerIB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        animations::watchAnim("playeridle", &anim);

        for (int i = 0; i < buffs.size(); i++) {
            buffs[i].name = "nothing";
            buffs[i].countdown = -1;
        }
    }
}

