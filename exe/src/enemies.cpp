#include "enemies.h"
#include "gameLoop.h"
#include "ui.h"
#include "camera.h"
#include "layers.h"

namespace enemies {
    std::unordered_map<std::string, enemyBase>enemies;
    std::unordered_map<std::string, npc>npcs;

    int spawnEnemy(std::string name, glm::vec2 pos, glm::vec2 vel)
    {
        auto ppos = std::make_shared<glm::vec2>(pos);
        int e = ECS::newEntity();
        auto base = enemies[name];

        float randsize = ((rand() % 1000) / 500.0f - 1) * base.randomsize;
        float randomred = ((rand() % 1000) / 500.0f - 1) * base.randomcolor;
        float randomgreen = ((rand() % 1000) / 500.0f - 1) * base.randomcolor;
        float randomblue = ((rand() % 1000) / 500.0f - 1) * base.randomcolor;

        base.pC.size += randsize;
        base.dC.size += randsize;
        base.dC.color += glm::vec3(randomred, randomgreen, randomblue);
        base.dC.parent = globals::mobLayer;

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
            base.mC.children.push_back(spawnEnemy(base.childrenBases[i], pos, vel));
        }
        if (base.mC.displayName != "") {
            base.mC.hpbar = ECS::newEntity();
            if (base.bossbar != "") {
                UI::addElement(base.mC.hpbar, ui_BOSSBAR, { 0, 100 }, { 900,900 }, gameLoop::hpbarC, { {"entity", {.intVal = e}} }, { {"head", base.bossbar}}, false, anchorBOT);
            }
            else {
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
        
        return e;
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
    void addEnemyToBiome(std::string enemy, std::string biome, float spawnchance)
    {
        Layers::biomes[biome].mobs.insert(std::make_pair(enemy, spawnchance));
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