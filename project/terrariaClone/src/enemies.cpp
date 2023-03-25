#include "enemies.h"
#include "gameLoop.h"
#include "ui.h"
#include "camera.h"
#include "layers.h"
#include <game.h>
#include <textures.h>
#include <utils.h>
#include <player.h>
#include <world.h>
#include <liquids.h>

namespace enemies {
    std::unordered_map<std::string, enemyBase>enemies;
    std::unordered_map<std::string, npc>npcs;

    int currslots = 0;

    struct queuedEnemy {
        std::string name;
        glm::vec2 pos;
    };
    std::vector<std::pair<queuedEnemy, int>> tospawn;

    int spawnEnemy(std::string name, glm::vec2 pos, bool usesslots, glm::vec2 vel)
    {
        if (game::currScene != GAME) return -1;
        enemyBase base = enemies[name];

        if (base.prespawntimer > 0) {
            tospawn.push_back(std::make_pair(queuedEnemy( name, pos ), base.prespawntimer));
            //if spawntext pokaz spawntext
            return -1;
        }

        float randsize = ((rand() % 1000) / 500.0f - 1) * base.randomsize;
        float randomred = ((rand() % 1000) / 500.0f - 1) * base.randomcolor;
        float randomgreen = ((rand() % 1000) / 500.0f - 1) * base.randomcolor;
        float randomblue = ((rand() % 1000) / 500.0f - 1) * base.randomcolor;
        float randangle = ((rand() % 1000) / 500.0f - 1) * base.randomvelangle;

        base.pC.size += randsize;
        base.dC.size += randsize;
        base.dC.color += glm::vec3(randomred, randomgreen, randomblue);
        base.dC.parent = globals::mobLayer;
        utils::rotateVecByAngle(&base.pC.vel, randangle);
        if (base.possibleSprites.size() > 0) {
            base.dC.tex = base.possibleSprites[rand() % base.possibleSprites.size()];
        }

        if (base.resizeToSprite) {
            glm::vec4 coords = textures::nametocoords[base.dC.tex]->coords;
            glm::vec2 s(abs(coords.b - coords.r), abs(coords.g - coords.a));
            base.dC.size = s * globals::spritesheetWH;
            base.dC.size /= 8;
            base.pC.size = base.dC.size;
            base.dC.autocorrect = false;
        }

        if (base.behindBlocks) {
            base.dC.parent = game::drawSys->behindBlocks;
        }

        auto ppos = std::make_shared<glm::vec2>(pos);
        base.pC.position = ppos;
        if (vel != glm::vec2(-13245, 123432)) {
            base.pC.vel = vel;
        }
        base.dC.position = ppos;
        for (auto& drop : base.drops) {
            if (rand() % int(1 / drop.second.chance) == 0) {
                if (drop.second.randomnum == 0) {
                    base.mC.drops.insert(std::make_pair(drop.first, drop.second.num));
                }
                else {
                    base.mC.drops.insert(std::make_pair(drop.first, drop.second.num + rand() % (drop.second.randomnum * 2) - drop.second.randomnum));
                }
            }
        }
        for (int i = 0; i < base.childrenBases.size(); i++) {
            base.mC.children.push_back(spawnEnemy(base.childrenBases[i], pos, true, vel));
        }

        int e = ECS::newEntity();
        if (base.mC.displayName != "") {
            if (base.bossbar != "") {
                base.mC.hpbar = ECS::newEntity();
                UI::addElement(base.mC.hpbar, ui_BOSSBAR, { 0, 100 }, { 0,0 }, gameLoop::hpbarC, { {"entity", {.intVal = e}} }, { {"head", base.bossbar}}, false, anchorBOT);
            }else
            if(base.hpbar) {
                base.mC.hpbar = ECS::newEntity();
                UI::addElement(base.mC.hpbar, ui_HPBAR, { 0,0 }, { 0,0 }, gameLoop::hpbarC, { {"entity", {.intVal = e}} }, {}, false, anchorNONE);
            }
        }
        drawSystem::addComponent(e, &base.dC, false);
        ECS::queueComponent<physicsC>(e, base.pC);
        ECS::queueComponent<aiC>(e, base.aC);
        ECS::queueComponent<mobC>(e, base.mC);
        if (base.pec.lifespan != 0) {
            ECS::queueComponent<particleEmmiterC>(e, base.pec);
        }

        if (usesslots) {
            currslots += base.slots;
        }
        
        return e;
    }

    void doQueues()
    {
        for (int i = 0; i < tospawn.size(); i++) {
            if (tospawn[i].second <= 0) {
                spawnEnemy(tospawn[i].first.name, tospawn[i].first.pos);
                tospawn.erase(tospawn.begin() + i);
                i--;
            }
            tospawn[i].second--;
        }
    }

    void spawnNPCS()
    {
        const auto info = &Layers::biomes[Layers::currentBiome];

        auto mobs = &info->underworldmobs;
        auto spawns = info->underworldspawns;

        //ceje podzemlo cene pa glede na cs dneva
        if (Player::pos.y > map::surfaceH + map::surfaceScale * 2) {
            mobs = &info->daymobs;
            spawns = info->dayspawns;
        }
        else if (Player::pos.y > map::surfaceH - map::surfaceScale) {
            if (globals::cdayTime > globals::dayLength / 2) {
                mobs = &info->nightmobs;
                spawns = info->nightspawns;
            }
            else {
                mobs = &info->daymobs;
                spawns = info->dayspawns;
            }
        }
        else if (Player::pos.y > map::underworldH + (map::surfaceH - map::underworldH) / 2.0f) {
            mobs = &info->undergroundmobs;
            spawns = info->undergroundspawns;
        }
        else if (Player::pos.y > map::underworldH) {
            mobs = &info->cavernmobs;
            spawns = info->cavernspawns;
        }

        if(Player::spawnCap)
        if (currslots >= spawns.second) return;

        float mult = 1;
        if (currslots <= spawns.second * 0.2) {
            mult = 0.6;
        }
        else if (currslots <= spawns.second * 0.4) {
            mult = 0.7;
        }
        else if (currslots <= spawns.second * 0.6) {
            mult = 0.8;
        }
        else if (currslots <= spawns.second * 0.8) {
            mult = 0.9;
        }

        if(spawns.first != 0)
        if (rand() % int(spawns.first*mult*Player::enemyChance) != 0) return;
        
        float max = 0;
        std::string maxname = "";
        
        std::vector<std::string> candidates;
        
        for (auto& mob : *mobs) {
            float chance = mob.second;
            
            if (chance > max) {
                max = chance;
                maxname = mob.first;
            }

            auto info = &enemies::enemies[mob.first];
            while (chance > 1) {
                candidates.push_back(mob.first);
                chance--;
            }
            if ((rand() % 10000) < chance * 10000 == 0) {
                candidates.push_back(mob.first);
            }
        }
        
        if (candidates.size() > 0) {
            maxname = candidates[rand() % candidates.size()];
        }   
        enemies::spawnEnemy(maxname, enemies::enemies[maxname].spawnFunc(), true);
    }

    glm::vec2 fallenStarSpawnFunc() {
        int startx = -camera::pos.x - Layers::blocksOnScreen.x / 2 - 40;
        int y = -camera::pos.y + Layers::blocksOnScreen.y / 2 + 15;
        std::vector<glm::vec2> candidates;
        auto bs = Layers::getLayer("blocks");
        for (int x = startx; x < startx + Layers::blocksOnScreen.x + 30; x++) {
            if (Layers::queryBlockInfo(bs, { x,y })->notReplacable == false){
                candidates.push_back({ x,y });
            }
        }
        if (candidates.size() <= 0) return glm::vec2(-1020, -1020);
        return candidates[rand() % candidates.size()];
    }
    glm::vec2 defaultSpawnFunc()
    {
        int startx = -camera::pos.x - Layers::blocksOnScreen.x / 2 - 30;
        int starty = -camera::pos.y - Layers::blocksOnScreen.y / 2 - 15;
        auto bs = Layers::getLayer("blocks");
        std::vector<glm::vec2> candidates;
        //sky light
        for (int x = startx; x < startx + 25; x++) {
            for (int y = starty; y < starty + Layers::blocksOnScreen.y + 30; y++) {
                if (Layers::queryBlockInfo(bs, { x, y })->notReplacable == false && Layers::queryBlockInfo(bs, { x,y - 1 })->notReplacable == true) {
                    candidates.push_back({ x,y });
                }
            }
        }

        for (int x = startx + Layers::blocksOnScreen.x + 35; x < startx + Layers::blocksOnScreen.x + 55; x++) {
            for (int y = starty; y < starty + Layers::blocksOnScreen.y + 30; y++) {
                if (Layers::queryBlockInfo(bs, { x, y })->notReplacable == false && Layers::queryBlockInfo(bs, { x,y - 1 })->notReplacable == true) {
                    candidates.push_back({ x,y });
                }
            }
        }
        if (candidates.size() == 0) {
            return glm::vec2(-1200, -1020);
        }
        return candidates[rand() % candidates.size()];
    }
    glm::vec2 flyingSpawnFunc()
    {
        int startx = -camera::pos.x - Layers::blocksOnScreen.x / 2 - 30;
        int starty = -camera::pos.y - Layers::blocksOnScreen.y / 2 - 15;
        auto bs = Layers::getLayer("blocks");
        std::vector<glm::vec2> candidates;
        //sky light
        for (int x = startx; x < startx + 25; x++) {
            for (int y = starty; y < starty + Layers::blocksOnScreen.y + 30; y++) {
                if (Layers::queryBlockInfo(bs, { x, y })->notReplacable == false) {
                    candidates.push_back({ x,y });
                }
            }
        }

        for (int x = startx + Layers::blocksOnScreen.x + 35; x < startx + Layers::blocksOnScreen.x + 55; x++) {
            for (int y = starty; y < starty + Layers::blocksOnScreen.y + 30; y++) {
                if (Layers::queryBlockInfo(bs, { x, y })->notReplacable == false) {
                    candidates.push_back({ x,y });
                }
            }
        }
        if (candidates.size() == 0) {
            return glm::vec2(-1020, -1020);
        }
        return candidates[rand() % candidates.size()];
    }
    glm::vec2 wormSpawnFunc()
    {
        int startx = -camera::pos.x - Layers::blocksOnScreen.x / 2 - 30;
        int starty = -camera::pos.y - Layers::blocksOnScreen.y / 2 - 15;
        auto bs = Layers::getLayer("blocks");
        std::vector<glm::vec2> candidates;
        //sky light
        for (int x = startx; x < startx + 25; x++) {
            for (int y = starty; y < starty + Layers::blocksOnScreen.y + 30; y++) {
                if (Layers::queryBlockInfo(bs, { x, y })->notReplacable) {
                    candidates.push_back({ x,y });
                }
            }
        }

        for (int x = startx + Layers::blocksOnScreen.x + 35; x < startx + Layers::blocksOnScreen.x + 55; x++) {
            for (int y = starty; y < starty + Layers::blocksOnScreen.y + 30; y++) {
                if (Layers::queryBlockInfo(bs, { x, y })->notReplacable) {
                    candidates.push_back({ x,y });
                }
            }
        }
        if (candidates.size() == 0) {
            return glm::vec2(-1020, -1020);
        }
        return candidates[rand() % candidates.size()];
    }
    glm::vec2 vultureSpawnFunc()
    {
        int startx = -camera::pos.x - Layers::blocksOnScreen.x / 2 - 30;
        int starty = -camera::pos.y - Layers::blocksOnScreen.y / 2 - 15;
        auto bs = Layers::getLayer("blocks");
        std::vector<glm::vec2> candidates;
        for (int x = startx; x < startx + 25; x++) {
            for (int y = starty; y < starty + Layers::blocksOnScreen.y + 30; y++) {
                if (*Layers::queryBlockName(bs, { x, y }) == "sand" && Layers::queryBlockInfo(bs, { x,y + 1 })->notReplacable == false) {
                    candidates.push_back({ x,y+2 });
                }
            }
        }

        for (int x = startx + Layers::blocksOnScreen.x + 35; x < startx + Layers::blocksOnScreen.x + 55; x++) {
            for (int y = starty; y < starty + Layers::blocksOnScreen.y + 30; y++) {
                if (*Layers::queryBlockName(bs, { x, y }) == "sand" && Layers::queryBlockInfo(bs, {x,y + 1})->notReplacable == false) {
                    candidates.push_back({ x,y+2 });
                }
            }
        }
        if (candidates.size() == 0) {
            return glm::vec2(-1020, -1002);
        }
        return candidates[rand() % candidates.size()];
    }
    glm::vec2 anywhereSpawnFunc()
    {
        return glm::vec2(-camera::pos.x + rand()%(int)Layers::blocksOnScreen.x - Layers::blocksOnScreen.x/2,
            -camera::pos.y + rand() % (int)Layers::blocksOnScreen.y - Layers::blocksOnScreen.y / 2);
    }
    glm::vec2 liquidSpawnFunc()
    {
        int startx = -camera::pos.x - Layers::blocksOnScreen.x / 2 - 30;
        int starty = -camera::pos.y - Layers::blocksOnScreen.y / 2 - 15;
        auto bs = Layers::getLayer("blocks");
        std::vector<glm::vec2> candidates;
        //sky light
        for (int x = startx; x < startx + 25; x++) {
            for (int y = starty; y < starty + Layers::blocksOnScreen.y + 30; y++) {
                if (liquids::at({x,y}) != 0) {
                    candidates.push_back({ x,y });
                }
            }
        }

        for (int x = startx + Layers::blocksOnScreen.x + 35; x < startx + Layers::blocksOnScreen.x + 55; x++) {
            for (int y = starty; y < starty + Layers::blocksOnScreen.y + 30; y++) {
                if (liquids::at({ x,y }) != 0) {
                    candidates.push_back({ x,y });
                }
            }
        }
        if (candidates.size() == 0) {
            return glm::vec2(-10200, -12000);
        }
        return candidates[rand() % candidates.size()];
    }
    void addEnemyToDay(std::string enemy, std::string biome, float spawnchance)
    {
        Layers::biomes[biome].daymobs.insert(std::make_pair(enemy, spawnchance));
    }
    void addEnemyToNight(std::string enemy, std::string biome, float spawnchance)
    {
        Layers::biomes[biome].nightmobs.insert(std::make_pair(enemy, spawnchance));
    }
    void addEnemyToUnderground(std::string enemy, std::string biome, float spawnchance)
    {
        Layers::biomes[biome].undergroundmobs.insert(std::make_pair(enemy, spawnchance));
    }
    void addEnemyToCavern(std::string enemy, std::string biome, float spawnchance)
    {
        Layers::biomes[biome].cavernmobs.insert(std::make_pair(enemy, spawnchance));
    }
    void addEnemyToUnderworld(std::string enemy, std::string biome, float spawnchance)
    {
        Layers::biomes[biome].underworldmobs.insert(std::make_pair(enemy, spawnchance));
    }
    void addNPCDialogue(std::string npc, std::string dialogue)
    {
        npcs[npc].dialogueOptions.push_back(dialogue);
    }
    void addNPCButton(std::string npc, std::string buttontext, void(*func)(uiC*))
    {
        npcs[npc].buttons.insert(std::make_pair(buttontext, func));
    }

}