#include "Layers.h"

#include <ECS/ECS.h>
#include <fstream>

#include <blocks.h>
#include <utils.h>
#include <componentsystems.h>
#include <globals.h>
#include <textures.h>
#include <blocks.h>
#include <core.h>
#include <textures.h>
#include <player.h>
#include <world.h>
#include <camera.h>
#include <background.h>
#include <enemies.h>
#include <particles.h>
#include <game.h>

namespace Layers {
    glm::vec2 blocksOnScreen;
    glm::vec2 trueBsOnScr;
    int nQuads;
    int nDrawnIndices;
    std::vector<Layer> layers;

    int nDrawnBlocks;

    BlockVertex *drawnVertices;
    int offsets = 10;

    blockRelationship* childParent;

    std::unordered_map<int, Chest> chests;

    std::unordered_map<std::string, Biome> biomes;
    std::set<std::string> allBiomes;
    std::unordered_map<std::string, int> biomeCounter;

    GLuint layersVA;
    GLuint layersVB;
    GLuint layersIB;

    std::string currentBiome = "forest";

    std::unordered_map<int, glm::vec3> lights;

    void addLayer(const char* name, float darkness, int drawingOffset)
    {
        Layer l;
        l.mname = name;
        l.mdrawingOffset = drawingOffset;
        l.mblocks = new Block[nQuads];
        l.mdarkness = darkness;
        layers.push_back(l);
    }

    Layer* getLayer(std::string_view l)
    {
        for (int i = 0; i < layers.size(); i++) {
            if (layers[i].mname == l) {
                return &layers[i];
            }
        }
        return nullptr;
    }

    void reschanged()
    {
        glDeleteVertexArrays(1, &layersVA);
        glDeleteBuffers(1, &layersVB);
        glDeleteBuffers(1, &layersIB);
        init();
        delete[] drawnVertices;
        nQuads = map::mapX * map::mapY;
        drawnVertices = new BlockVertex[nDrawnBlocks * 4];
    }

    void init()
    {
        blocksOnScreen.x = globals::resX / (globals::blocksizepx / 2) + offsets;
        blocksOnScreen.y = globals::resY / (globals::blocksizepx / 2) + offsets;
        trueBsOnScr.x = globals::resX / (globals::blocksizepx / 2);
        trueBsOnScr.y = globals::resY / (globals::blocksizepx / 2);
        nDrawnBlocks = blocksOnScreen.x * blocksOnScreen.y;
        nDrawnIndices = nDrawnBlocks * 6;

        glCreateVertexArrays(1, &layersVA);
        glBindVertexArray(layersVA);

        glCreateBuffers(1, &layersVB);
        glBindBuffer(GL_ARRAY_BUFFER, layersVB);
        glBufferData(GL_ARRAY_BUFFER, sizeof(BlockVertex) * nDrawnBlocks * 4, nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (const void*)offsetof(BlockVertex, Position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (const void*)offsetof(BlockVertex, SpriteCoords));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (const void*)offsetof(BlockVertex, TexCoords));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (const void*)offsetof(BlockVertex, light));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (const void*)offsetof(BlockVertex, breaking));

        uint32_t* indices = new uint32_t[nDrawnIndices];
        int offset = 0;
        for (int i = 0; i < nDrawnIndices; i += 6) {
            indices[i + 0] = 0 + offset;
            indices[i + 1] = 1 + offset;
            indices[i + 2] = 2 + offset;

            indices[i + 3] = 2 + offset;
            indices[i + 4] = 3 + offset;
            indices[i + 5] = 0 + offset;

            offset += 4;
        }

        glCreateBuffers(1, &layersIB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, layersIB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * nDrawnIndices, indices, GL_STATIC_DRAW);

        delete[] indices;
    }

    void setUp()
    {
        nQuads = map::mapX * map::mapY;
        drawnVertices = new BlockVertex[nDrawnBlocks * 4];
        addLayer("bg", 1.0f);
        addLayer("blocks", 1.0f);
        childParent = new blockRelationship("blocks");
    }

    void clean()
    {
        delete childParent;
        childParent = nullptr;
        delete drawnVertices;
        drawnVertices = nullptr;
        for (auto& layer : layers) {
            delete[] layer.mblocks;
            layer.mblocks = nullptr;
        }
        layers.clear();
    }

    bool placeBlock(Layer* l, glm::vec2 pos, std::string type, int size, std::vector<std::function<bool(BlockConditionArgs)>>* conditions, glm::vec3 setlight, bool update)
    {
        if (verifyBlock(l, &pos, false)) {
            auto info = &blocks::nameToInfo[type];
            auto bg = Layers::getLayer("bg");
            auto bs = Layers::getLayer("blocks");

            if (size > 1) {
                for (int x2 = -size; x2 < size; x2++) {
                    for (int y2 = -size; y2 < size; y2++) {
                        if (glm::distance(pos + glm::vec2(x2, y2), pos) < size / 2) {
                            placeBlock(l, pos + glm::vec2(x2, y2), type, 1, conditions);
                        }
                    }
                }
            }

            if (conditions != nullptr) {
                for (int i = 0; i < conditions->size(); i++) {
                    if(!conditions->at(i)(l, bg, bs, pos, info)) return false;
                }
            }

            for (int x = 0; x < info->size.x; x++) {
                for (int y = 0; y < info->size.y; y++) {
                    glm::vec2 newpos = pos + glm::vec2(x, y);
                    breakBlock(l, newpos, 1, false, true);
                }
            }

            int wholestate = 0;
            if (info->spriteType == st_POT) {
                wholestate = (rand() % 2) * 4;
            }
            for (int x = 0; x < info->size.x; x++) {
                int state = 0;
                switch (info->spriteType) {
                case st_SINGLE:
                    state = int(x * info->size.y);
                    break;
                case st_GRASS:
                    state = (rand() % 5) * 2;
                    break;
                case st_STALAKTIT:
                    state = (rand() % 3) * 2;
                    break;
                case st_STALAGMIT:
                    state = (rand() % 3) * 2;
                    break;
                case st_SMALLROCK:
                    state = rand() % 6;
                    break;
                case st_MEDIUMROCK:
                    state = (rand() % 3) * 2;
                    break;
                case st_VINES:
                    state = (rand() % 5);
                    break;
                case st_POT:
                    state = int(x * info->size.y);
                    break;
                }
                for (int y = 0; y < info->size.y; y++) {
                    glm::vec2 newpos = pos + glm::vec2(x, y);
                    breakBlock(l, newpos);
                    int c = vecToInt(newpos);
                    l->mblocks[c].id = blocks::nameToID.at(type);
                    l->mblocks[c].state = wholestate + state + y;
                    l->mblocks[c].breaking = 1;
                    if (x != 0 || y != 0) {
                        childParent->addSub(newpos, pos, true);
                    }
                }
            }


            blocks::nameToInfo[type].onPlace(pos, info);

            if (update) {
                pos.x = round(pos.x);
                pos.y = round(pos.y);
                autoSprite(l, { pos.x,pos.y });
                autoSprite(l, { pos.x + 1,pos.y });
                autoSprite(l, { pos.x - 1,pos.y });
                autoSprite(l, { pos.x,pos.y + 1 });
                autoSprite(l, { pos.x,pos.y - 1 });
            }

            return true;
                
        }
        else {
            return false;
        }
    }

    void fastPlaceBlock(glm::vec2 pos, std::string type, Layer* layer, int size)
    {
        if (layer == nullptr) layer = getLayer(blocks::nameToInfo[type].layer);
        if (verifyBlock(layer, &pos, false)) {
            int c = vecToInt(pos);
            layer->mblocks[c] = { blocks::nameToID.at(type), glm::vec3(0), 0, 1.0f};
            if (size == 1) return;

            for (int x2 = -size; x2 < size; x2++) {
                for (int y2 = -size; y2 < size; y2++) {
                    if (glm::distance(pos + glm::vec2(x2, y2), pos) < size / 2.0f) {
                        fastPlaceBlock(pos + glm::vec2(x2, y2), type, layer, 1);
                    }
                }
            }
        }
    }

    bool placeBlock(glm::vec2 pos, std::string type, int size, std::vector<std::function<bool(BlockConditionArgs)>>* conditions, glm::vec3 setlight)
    {
        if (blocks::nameToInfo.count(type.data()) <= 0)  return false;

        Layer* l = getLayer(blocks::nameToInfo[type].layer);
        if (placeBlock(l, pos, type, size, conditions, setlight)) {
            return true;
        }

        return false;
    }

    void breakBlock(Layer* l, glm::vec2 pos, int size, bool dropitem, bool checkforchildren, bool execute, bool particles)
    {
        if (verifyBlock(l, &pos, checkforchildren)) {
            auto bs = getLayer("blocks");
            auto bg = getLayer("bg");

            auto info = queryBlockInfo(l, pos);
            if(dropitem) game::droppedItemSys->dropItem(pos, info->drops, 1);
            if (particles) spawnParticlesFromBlock(pos, info);
            
            int c = vecToInt(pos);
            l->mblocks[c].id = globals::emptyid;
            l->mblocks[c].state = 0;
            l->mblocks[c].breaking = 4;

            if (execute) info->onBreak(pos, info);

            clearRelationships(l, pos);
            autoSprite(l, pos);
            autoSprite(bg, { pos.x + 1,pos.y });
            autoSprite(bg, { pos.x - 1,pos.y });
            autoSprite(bg, { pos.x,pos.y + 1 });
            autoSprite(bg, { pos.x,pos.y - 1 });
            autoSprite(bs, { pos.x + 1,pos.y });
            autoSprite(bs, { pos.x - 1,pos.y });
            autoSprite(bs, { pos.x,pos.y + 1 });
            autoSprite(bs, { pos.x,pos.y - 1 });
            if (size > 1) {
                for (int x2 = -size; x2 < size; x2++) {
                    for (int y2 = -size; y2 < size; y2++) {
                        if (glm::distance(pos + glm::vec2(x2, y2), pos) < size/2) {
                            breakBlock(l, pos + glm::vec2(x2, y2), 1, dropitem);
                        }
                    }
                }
            }
        }
    }

    bool damageBlock(Layer* l, glm::vec2 pos, float strength, int size, bool dropitem, itemFamily fam) {
        if (size > 0) {
            if (size > 1) {
                for (int x2 = -size; x2 < size; x2++) {
                    for (int y2 = -size; y2 < size; y2++) {
                        if (glm::distance(pos + glm::vec2(x2, y2), pos) < size) {
                            damageBlock(l, pos + glm::vec2(x2, y2), strength, dropitem, fam);
                        }
                    }
                }
            }
            if (verifyBlock(l, &pos)) {
                int c = vecToInt(pos);
                auto info = queryBlockInfo(l, pos);
                if (canDamage(l, pos)) {
                    if (fam != if_ANY) {
                        if (fam != info->damagableWith) {
                            return false;
                        }
                    }
                    if (info->minPickaxeStrength > strength) return false;
                    spawnParticlesFromBlock(pos, info);
                    float curlife = l->mblocks[c].breaking * info->life;
                    if (curlife - strength <= 0.0f) {
                        breakBlock(l, pos, 1, dropitem);
                    }
                    else {
                        curlife -= strength;
                        l->mblocks[c].breaking = curlife / info->life;
                    }
                }
                return true;
            }
        }
        return false;
    }

    void renderLayers()
    {
        glUseProgram(globals::blockShaderID);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures::spriteSheet);
        glUniform1i(1, textures::spriteSheet);

        glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(camera::trans));

        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, globals::broken1Tex);
        glUniform1i(6, 1);

        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, globals::broken2Tex);
        glUniform1i(7, 2);

        glActiveTexture(GL_TEXTURE0 + 3);
        glBindTexture(GL_TEXTURE_2D, globals::broken3Tex);
        glUniform1i(8, 3);

        for (auto& l : layers) {

            glUniform1f(2, l.mdarkness);

            memset(drawnVertices, 0, sizeof(BlockVertex) * 4 * nDrawnBlocks);

            glm::vec2 camC = -camera::pos;

            camC -= blocksOnScreen/glm::vec2(2.0f);

            int n = 0;
            for (int x = 0; x < blocksOnScreen.x; x++) {
                for (int y = 0; y < blocksOnScreen.y; y++) {

                    glm::vec2 pos = floor(camC + glm::vec2(x, y));
                    int c = vecToInt(pos);

                    if (c < 0 || c >= nQuads) continue;
                    blocks::PremadeBlock tmp = blocks::blockBuffer[l.mblocks[c].id][l.mblocks[c].state];
                    tmp.v0.Position += glm::vec2(pos.x, pos.y);
                    tmp.v1.Position += glm::vec2(pos.x, pos.y);
                    tmp.v2.Position += glm::vec2(pos.x, pos.y);
                    tmp.v3.Position += glm::vec2(pos.x, pos.y);

                    tmp.v0.light += l.mblocks[c].light;
                    tmp.v1.light += l.mblocks[c].light;
                    tmp.v2.light += l.mblocks[c].light;
                    tmp.v3.light += l.mblocks[c].light;

                    tmp.v0.breaking = l.mblocks[c].breaking;
                    tmp.v1.breaking = l.mblocks[c].breaking;
                    tmp.v2.breaking = l.mblocks[c].breaking;
                    tmp.v3.breaking = l.mblocks[c].breaking;

                    memcpy(drawnVertices + n * 4, &tmp, 4 * sizeof(BlockVertex));
                    n++;
                }
            }

            glBindBuffer(GL_ARRAY_BUFFER, layersVB);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(BlockVertex) * nDrawnBlocks * 4, drawnVertices);

            glBindVertexArray(layersVA);
            glDrawElements(GL_TRIANGLES, nDrawnIndices, GL_UNSIGNED_INT, nullptr);
        }
    }

    void autoSprite(Layer* l, glm::vec2 pos)
    {
        if (!verifyBlock(l, &pos, false)) return;
        int n = 0;
        auto b = queryBlock(l, pos);
        switch (blocks::idToInfo[b->id].spriteType) {
        case st_SINGLE:
            break;
        case st_BLOCK:
            if (queryBlockInfo(l, { pos.x, pos.y + 1 })->updatesBot) {
                n += 1;
            }
            if (queryBlockInfo(l, { pos.x + 1, pos.y })->updatesLeft) {
                n += 2;
            }
            if (queryBlockInfo(l, { pos.x, pos.y - 1 })->updatesTop) {
                n += 4;
            }
            if (queryBlockInfo(l, { pos.x - 1, pos.y })->updatesRight) {
                n += 8;
            }
            b->state = n;
            break;
        case st_WALL:
            if (queryBlockInfo(l, { pos.x, pos.y + 1 })->updatesBot) {
                n += 1;
            }
            if (queryBlockInfo(l, { pos.x + 1, pos.y })->updatesLeft) {
                n += 2;
            }
            if (queryBlockInfo(l, { pos.x, pos.y - 1 })->updatesTop) {
                n += 4;
            }
            if (queryBlockInfo(l, { pos.x - 1, pos.y })->updatesRight) {
                n += 8;
            }
            b->state = n;
            break;
        case st_TORCH:
            if (queryBlockInfo(l, { pos.x, pos.y - 1 })->updatesBot) {
                n = 0;
            }
            else
                if (queryBlockInfo(l, { pos.x - 1, pos.y })->updatesLeft) {
                    n = 1;
                }
                else
                    if (queryBlockInfo(l, { pos.x + 1, pos.y })->updatesTop) {
                        n = 2;
                    }
                    else
                        if (queryBlockInfo(l, { pos.x, pos.y + 1 })->updatesRight) {
                            n = 3;
                        }
            b->state = n;
            break;
        case st_PLATFORM: {
            auto left = queryBlockInfo(l, { pos.x - 1, pos.y });
            auto right = queryBlockInfo(l, { pos.x + 1, pos.y });
            b->state = 0;
            if (left->spriteType == st_PLATFORM) {
                if (right->spriteType == st_PLATFORM) {
                    b->state = 5;
                }
                else if (right->updatesLeft) {
                    b->state = 1;
                }
                else {
                    b->state = 4;
                }
            }
            else
                if (right->spriteType == st_PLATFORM) {
                    if (left->updatesLeft) {
                        b->state = 2;
                    }
                    else {
                        b->state = 3;
                    }
                }
                else if (left->updatesRight) {
                    b->state = 6;
                }
                else if (right->updatesLeft) {
                    b->state = 7;
                }
            break;
        }
        case st_WATER:
            {
                bool bot = queryBlockInfo(l, { pos.x, pos.y - 1 })->updatesTop;
                bool top = queryBlockInfo(l, { pos.x, pos.y + 1 })->updatesBot;
                bool right = queryBlockInfo(l, { pos.x + 1, pos.y })->updatesLeft;
                bool left = queryBlockInfo(l, { pos.x - 1, pos.y })->updatesRight;
                if (!bot && !top && !right && !left) {
                    b->state = 0;
                    return;
                }
                if (bot && top && !right && left) {
                    b->state = 1;
                    return;
                }
                if (bot && !top && !right && left) {
                    b->state = 2;
                    return;
                }
                if (top && !right && !left) {
                    b->state = 3;
                    return;
                }
                if (!top && !right && !left) {
                    b->state = 4;
                    return;
                }
                if (bot && !top && right && left) {
                    b->state = 5;
                    return;
                }
                if (bot && top && right && left) {
                    b->state = 6;
                    return;
                }
                if (!bot && !top && !right && left) {
                    b->state = 7;
                    return;
                }
                if (bot && !top && right && !left) {
                    b->state = 8;
                    return;
                }
                break;
            }
        case st_VINES:
        {
            if (*queryBlockName(l, { pos.x, pos.y - 1 }) == "normalvines" || *queryBlockName(l, { pos.x, pos.y - 1 }) == "junglevines") {
                b->state = (rand() % 5) + 5;
            }
            else {
                b->state = rand() % 5;
            }
            break;
        }
        }
    }

    const std::string* queryBlockName(Layer* l, glm::vec2 pos, bool checkForChildren)
    {
        pos.x = round(pos.x);
        pos.y = round(pos.y);
        if (verifyBlock(l, &pos, checkForChildren)) {
            int c = vecToInt(pos);
            return &blocks::idToName.at(l->mblocks[c].id);
        }
        return &blocks::idToName.at(globals::emptyid);
    }

    Block* queryBlock(Layer* l, glm::vec2 pos, bool checkForChildren)
    {
        pos.x = round(pos.x);
        pos.y = round(pos.y);
        if (verifyBlock(l, &pos, checkForChildren)) {
            int c = vecToInt(pos);
            return &l->mblocks[c];
        }
        if (l == nullptr) {
            return nullptr;
        }
        return &l->mblocks[0];
    }

    Block* fastQueryBlock(Layer* l, glm::vec2 pos)
    {
        int c = vecToInt(pos);
        return &l->mblocks[c];
    }

    blocks::BlockInfo* queryBlockInfo(Layer* l, glm::vec2 pos, bool checkforchildren)
    {
        pos = round(pos);
        if (verifyBlock(l, &pos, checkforchildren)) {
            int c = vecToInt(pos);
            return &blocks::idToInfo[l->mblocks[c].id];
        }
        return &blocks::idToInfo[globals::emptyid];
    }

    blocks::BlockInfo* fastQueryBlockInfo(Layer* l, glm::vec2 pos)
    {
        int c = vecToInt(pos);
        return &blocks::idToInfo[l->mblocks[c].id];
    }

    bool canLiquidGoThru(Layer* l,glm::vec2 pos)
    {
        if (!verifyBlock(l, &pos, false)) return false;
        int c = vecToInt(pos);
        return blocks::idToInfo[l->mblocks[c].id].canpassliquid;
    }

    bool canLiquidGoThru(Layer* l, int c)
    {
        if (c < 0 || c > map::mapX * map::mapY - 1) return false;
        return blocks::idToInfo[l->mblocks[c].id].canpassliquid;
    }

    void moveBlockTo(Layer* l, glm::vec2 from, glm::vec2 to)
    {
        int cfrom = vecToInt(from);
        int cto = vecToInt(to);
        l->mblocks[cto] = l->mblocks[cfrom];
        l->mblocks[cfrom].id = globals::emptyid;
        l->mblocks[cfrom].state = 0;
        autoSprite(l, { to.x + 1,to.y });
        autoSprite(l, { to.x - 1,to.y });
        autoSprite(l, { to.x,to.y + 1 });
        autoSprite(l, { to.x,to.y - 1 });
        autoSprite(l, { from.x + 1,from.y });
        autoSprite(l, { from.x - 1,from.y });
        autoSprite(l, { from.x,from.y + 1 });
        autoSprite(l, { from.x,from.y - 1 });
    }

    void swapBlocks(Layer* l, glm::vec2 from, glm::vec2 to)
    {
        Block tmp;
        int cfrom = vecToInt(from);
        int cto = vecToInt(to);
        tmp = l->mblocks[cfrom];
        l->mblocks[cfrom] = l->mblocks[cto];
        l->mblocks[cto] = tmp;
    }

    void setLight(Layer* l, glm::vec2 pos, glm::vec3 light)
    {
        if (verifyBlock(l, &pos, false)) {
            int c = vecToInt(pos);
            light = glm::clamp(light, 0.0f, globals::maxLightIntensity);
            l->mblocks[c].light = light;
        }
    }

    void fastSetLight(Layer* l, glm::vec2 pos, glm::vec3 light)
    {
        int c = vecToInt(pos);
        light = glm::clamp(light, 0.0f, globals::maxLightIntensity);
        l->mblocks[c].light = light;
    }

    void addLight(glm::vec2 pos, glm::vec3 light)
    {
        int c = vecToInt(round(pos));
        if (lights.count(c) <= 0) lights[c] = glm::vec3(0);
        lights[c] += light;
    }

    void keepLightAbove(Layer* l, glm::vec2 pos, glm::vec3 light)
    {
        if (verifyBlock(l, &pos, false)) {
            int c = vecToInt(pos);
            if (l->mblocks[c].light.r < light.r)     l->mblocks[c].light.r = light.r;

            if (l->mblocks[c].light.g < light.g)     l->mblocks[c].light.g = light.g;

            if (l->mblocks[c].light.b < light.b)     l->mblocks[c].light.b = light.b;
        }
    }

    void updateOnScreen()
    {
        biomeCounter.clear();

        int startx = abs(camera::pos.x) - (blocksOnScreen.x / 2) - 5;
        int starty = abs(camera::pos.y) - (blocksOnScreen.y / 2) - 5;

        auto blocks = getLayer("blocks");

        for (int x = 0; x < blocksOnScreen.x + 10; x++) {
            for (int y = 0; y < blocksOnScreen.y + 10; y++) {
                updateBlock(blocks, {x + startx, y + starty});
            }
        }

        int max = 0;
        allBiomes.clear();
        if (biomeCounter.size() == 0) {
            currentBiome = "none";
        }
        for (auto& i : Layers::biomeCounter) {
            if (i.second < biomes[i.first].numNeededBlocks) continue;
            allBiomes.insert(i.first);
            if (i.first != "none") {
                if (i.second > max) {
                    max = i.second;
                    currentBiome = i.first;
                }
            }
        }

        if (Player::pos.y > map::surfaceH + map::surfaceScale * 2) {
            allBiomes.insert("space");
            currentBiome = "space";
        }else
        if (Player::pos.y > map::surfaceH - map::surfaceScale) {
            allBiomes.insert("surface");
        }else
        if (Player::pos.y > map::underworldH) {
            allBiomes.insert("underground");
            currentBiome = "underground";
        }else
        {
            allBiomes.insert("underworld");
            currentBiome = "underworld";
        }
        biomeCounter.clear();
    }

    void updateBlock(Layer* l, glm::vec2 pos)
    {
        pos = round(pos);
        if (verifyBlock(l, &pos, false)) {
            blocks::BlockInfo* info = queryBlockInfo(l, pos);
            int c = vecToInt(pos);
            l->mblocks[c].timealive++;
            if (info->animated) {
                if (l->mblocks[c].timealive % info->animrate == 0) {
                    l->mblocks[c].state += info->animoffset;
                    if (l->mblocks[c].state >= (info->numframes) * info->animoffset) {
                        l->mblocks[c].state = l->mblocks[c].state % info->animoffset;
                    }
                }
            }
            if (info->emitsparticle) {
                particles::emitParticle(pos, &info->emmiter);
            }
        
            if (l->mname == "blocks") {
                if (childParent->isSub(pos)) {
                    pos = childParent->subs[c];
                    info = queryBlockInfo(l, pos);
                }
            }

            if (!info->onUpdate(pos, info)) return;

            for (auto& rule : info->rules) {
                bool execute = true;
                for (int cond = 0; cond < rule.conditions.size(); cond++) {
                    execute = execute && evaluateCondition(&rule.conditions[cond], pos);
                }
                if (execute) {
                    rule.func(&rule, pos);
                }
            }
            if (biomeCounter[info->biome] == 0) {
                biomeCounter.insert(std::make_pair(info->biome, 0));
            }
            biomeCounter[info->biome]++;
        }
    }

    bool evaluateCondition(BlockRuleCond* c, glm::vec2 pos)
    {
        switch (c->type) {
            case br_IS:
                if (c->arg == bs_STAT) {
                    if (c->block == "updates") {
                        if (blocks::idToInfo[Layers::queryBlock(Layers::getLayer(c->layer), { pos.x + c->pos.x, pos.y + c->pos.y })->id].updates == true) {
                            return true;
                        }
                    }
                }else if(*Layers::queryBlockName(Layers::getLayer(c->layer), { pos.x + c->pos.x, pos.y + c->pos.y }) == c->block) {
                    return true;
                }
                break;
            case br_ISNT:
                if (c->arg == bs_STAT) {
                    if (c->block == "updates") {
                        if (blocks::idToInfo[Layers::queryBlock(Layers::getLayer(c->layer), { pos.x + c->pos.x, pos.y + c->pos.y })->id].updates == false) {
                            return true;
                        }
                    }
                }
                else if (*Layers::queryBlockName(Layers::getLayer(c->layer), { pos.x + c->pos.x, pos.y + c->pos.y }) != c->block) {
                    return true;
                }
                break;
            case br_ALWAYS:
                return true;
                break;
            case br_RANDOM:
                if (rand() % (int)c->randomInterval == 0) {
                    return true;
                }
                break;
            }
        return false;
    }

    inline int vecToInt(glm::vec2 pos)
    {
        return pos.x * map::mapY + pos.y;
    }

    glm::vec2 intToVec(int pos)
    {
        return { floor(pos / map::mapY), pos % map::mapY };
    }

    bool verifyBlock(Layer* l, glm::vec2* pos, bool checkForChildren)
    {
        if (pos->x >= 0 && pos->x < map::mapX && pos->y >= 0 && pos->y < map::mapY && l != nullptr) {
            if (checkForChildren) {
                if (l->mname == "blocks") {
                    if (childParent->isSub(*pos)) {
                        int c = vecToInt(*pos);
                        *pos = childParent->subs[c];
                    }
                }
            }
            return true;
        }
        return false;
    }

    bool doBlockFunction(glm::vec2 pos)
    {
        if (verifyBlock(getLayer("blocks"), &pos)) {
            auto info = queryBlockInfo(getLayer("blocks"), pos);
            if (info->function(pos, info)) {
                return true;
            }
        }
        return false;
    }

    void clearRelationships(Layer* l, glm::vec2 pos, bool Break)
    {
        childParent->clearRelationships(pos, Break);
    }

    void calculateLight() {
        Layer* bs = getLayer("blocks");
        Layer* bg = getLayer("bg");

        int startx = -camera::pos.x - blocksOnScreen.x / 2 - 15;
        int starty = -camera::pos.y - blocksOnScreen.y / 2 - 15;
        int endx = startx + blocksOnScreen.x + 30;
        int endy = starty + blocksOnScreen.y + 30;

        startx = glm::clamp(startx, 1, map::mapX-1);
        starty = glm::clamp(starty, 1, map::mapY-1);
        endx = glm::clamp(endx, 1, map::mapX-1);
        endy = glm::clamp(endy, 1, map::mapY-1);

        for (int x = startx; x < endx; x++) {
            for (int y = starty; y < endy; y++) {
                int c = vecToInt({ x,y });
                if (blocks::idToInfo[bg->mblocks[c].id].emitskylight) {
                    keepLightAbove(bs, { x,y }, globals::dayclr);
                }
            }
        }

        for (int x = startx; x < endx; x++) {
            for (int y = starty; y < endy; y++) {
                int c = vecToInt({ x,y });
                blocks::BlockInfo* info = fastQueryBlockInfo(bs, { x,y });
                glm::vec3 currL = info->light;
                currL += fastQueryBlock(bs, { x - 1, y })->light;
                currL += fastQueryBlock(bs, { x + 1, y })->light;
                currL += fastQueryBlock(bs, { x, y - 1 })->light;
                currL += fastQueryBlock(bs, { x, y + 1 })->light;
                if(lights.count(c) >= 1)
                currL += lights[c];
                currL = (currL / glm::vec3(4)) * 0.995f;
                currL += glm::vec3(globals::cheaterlight);
                fastSetLight(bg, { x, y }, currL);
                fastSetLight(bs, { x, y }, currL);

            }
        }
        lights.clear();
    }

    void saveChests(std::ofstream* file)
    {
        int chestsSize = chests.size();
        file->write((char*)(&chestsSize), sizeof(chestsSize));

        for (auto& i : chests) {

            file->write((char*)(&i.first), sizeof(i.first));

            int itemsSize = i.second.items.size();
            file->write((char*)(&itemsSize), sizeof(itemsSize));

            for (int j = 0; j < itemsSize; j++) {
                int itemnameSize = i.second.items[j]->item.size();
                file->write((char*)(&itemnameSize), sizeof(itemnameSize));
                file->write(i.second.items[j]->item.c_str(), itemnameSize);

                file->write((char*)(&i.second.items[j]->num), sizeof(i.second.items[j]->num));
                file->write((char*)(&i.second.items[j]->pos), sizeof(i.second.items[j]->pos));

            }
            int nameSize = i.second.name.size();
            file->write((char*)(&nameSize), sizeof(nameSize));
            file->write(i.second.name.c_str(), nameSize);
        }
    }
    void loadChests(std::ifstream* file)
    {
        int chestsSize;
        file->read((char*)(&chestsSize), sizeof(chestsSize));

        for (int i = 0; i < chestsSize; i++) {
            Chest tmpChest;
            int id = 0;

            file->read((char*)(&id), sizeof(id));

            int itemsSize;
            file->read((char*)(&itemsSize), sizeof(itemsSize));
            for (int j = 0; j < itemsSize; j++) {
                InventoryItem tmpItem;
                int itemnameSize;
                file->read((char*)(&itemnameSize), sizeof(itemnameSize));

                char c;
                for (int h = 0; h < itemnameSize; h++) {
                    file->get(c);
                    tmpItem.item += c;
                }

                file->read((char*)(&tmpItem.num), sizeof(tmpItem.num));
                file->read((char*)(&tmpItem.pos), sizeof(tmpItem.pos));

                tmpChest.items.push_back(std::make_shared<InventoryItem>(tmpItem));
            }

            int nameSize = chests[i].name.size();
            file->read((char*)(&nameSize), sizeof(nameSize));
            char c;
            for (int h = 0; h < nameSize; h++) {
                file->get(c);
                tmpChest.name += c;
            }
            chests.insert(std::make_pair(id, tmpChest));
        }
    }

    bool isAreaEmpty(glm::vec2 pos, glm::vec2 size)
    {
        auto bs = Layers::getLayer("blocks");
        for (int x = pos.x; x < pos.x + size.x; x++) {
            for (int y = pos.y; y < pos.y + size.y; y++) {
                if (Layers::queryBlockInfo(bs, { x,y })->notReplacable){
                return false;
               }
            }
        }
        return true;
    }

    void spawnMobs()
    {
        for (auto& biome : allBiomes) {
            for (auto& mob : biomes[biome].mobs) {
                float chance = mob.second;
                auto info = &enemies::enemies[mob.first];
                if (info->mC.families.count(mf_ENEMY) >= 1) {
                    chance *= Player::enemyChance;
                }
                if (info->mC.families.count(mf_CRITTER) >= 1) {
                    chance *= Player::critterChance;
                }
                int min = info->spawntime - info->spawnrange;
                int max = info->spawntime + info->spawnrange;
                max %= globals::dayLength + 1;
                if (min < 0) min = globals::dayLength + min;
                if (globals::cdayTime < min || globals::cdayTime > max) continue;

                while (chance > 1) {
                    enemies::spawnEnemy(mob.first, enemies::enemies[mob.first].spawnFunc());
                    chance--;
                }
                if (rand() % int(1 / chance) == 0) {
                    enemies::spawnEnemy(mob.first, enemies::enemies[mob.first].spawnFunc());
                    return;
                }
            }
        }
    }

    glm::vec2 findEmptySpot()
    {
        int startx = -camera::pos.x - blocksOnScreen.x / 2 - 30;
        int starty = -camera::pos.y - blocksOnScreen.y / 2 - 15;
        auto bs= Layers::getLayer("blocks");
        std::vector<glm::vec2> candidates;
        //sky light
        for (int x = startx; x < startx + 25; x++) {
            for (int y = starty; y < starty + blocksOnScreen.y + 30; y++) {
                if (queryBlockInfo(bs, { x, y })->notReplacable == false && queryBlockInfo(bs, { x,y - 1 })->notReplacable == true) {
                    candidates.push_back({ x,y });
                }
            }
        }

        for (int x = startx + blocksOnScreen.x + 35; x < startx + blocksOnScreen.x + 55; x++) {
            for (int y = starty; y < starty + blocksOnScreen.y + 30; y++) {
                if (queryBlockInfo(bs, { x, y })->notReplacable == false && queryBlockInfo(bs, { x,y - 1 })->notReplacable == true) {
                    candidates.push_back({ x,y });
                }
            }
        }

        return candidates[rand() % candidates.size()];
    }

    void spawnParticlesFromBlock(glm::vec2 pos, blocks::BlockInfo* info)
    {
        if (info->damageParticle == "") return;
        particleEmmiterC pec;
        pec.amount = 4;
        pec.randomizeamount = 2;
        pec.lifespan = 1;
        pec.ms = 0.14;
        pec.particleLifespan = 55;
        pec.dir = glm::vec2(0, 1);
        pec.randangle = 60;
        pec.randomizelifespan = 10;
        pec.randomizems = 0.1;
        pec.randrotation = 3;
        pec.rate = 1;
        pec.rotation = 6;
        pec.size = glm::vec2(.4, .4);
        pec.stoponcollision = false;
        pec.tex = info->damageParticle;
        pec.weight = 0.65;
        pec.killentity = true;
        physicsC pC;
        pC.position = std::make_shared<glm::vec2>(pos);
        pC.vel = glm::vec2(0);
        int ent = ECS::newEntity();
        ECS::queueComponent<particleEmmiterC>(ent, pec);
        ECS::queueComponent<physicsC>(ent, pC);
    }

    bool canDamage(Layer* l, glm::vec2 pos)
    {
        auto self = Layers::queryBlockInfo(l, pos);
        auto above = Layers::queryBlockInfo(l, { pos.x, pos.y + 1 });
        if (self == above) {
            return true;
        }
        if (above->canBreakBelow) {
            return true;
        }
        return false;
    }

}

blockRelationship::blockRelationship(std::string LAYERNAME, std::unordered_map<int, glm::vec2> SUBS, std::unordered_map<int, std::set<int>> DOMS)
{
    l = Layers::getLayer(LAYERNAME);
    layerName = LAYERNAME;
    subs = SUBS;
    doms = DOMS;
}

blockRelationship::blockRelationship()
{
    l = nullptr;
}

void blockRelationship::addSub(glm::vec2 pos, glm::vec2 dom, bool removeprev)
{
    int c = Layers::vecToInt(pos);
    if (removeprev) {
        if (subs.count(c) >= 1) {
            int p = Layers::vecToInt(subs[c]);
            doms[p].erase(c);
            if (doms[p].size() == 0) {
                doms.erase(p);
            }
            subs.erase(c);
        }
    }
    subs.insert(std::make_pair(c, dom));
    doms[Layers::vecToInt(dom)].insert(c);
}

void blockRelationship::addDom(glm::vec2 pos, std::set<int> subs, bool removeprev)
{
    int c = Layers::vecToInt(pos);
    if (removeprev) {
        if (doms.count(c) >= 1) {
            for (auto i : doms[c]) {
                subs.erase(i);
            }
            doms.erase(c);
        }
    }
    doms[Layers::vecToInt(pos)] = subs;
}

void blockRelationship::clearRelationships(glm::vec2 pos, bool Break)
{
    int c = Layers::vecToInt(pos);
    if (subs.count(c) >= 1) {
        int p = Layers::vecToInt(subs[c]);
        doms[p].erase(c);
        if (doms[p].size() == 0) {
            doms.erase(p);
        }
        subs.erase(c);
    }
    if (doms.count(c) >= 1) {
        for (auto i : doms[c]) {
            subs.erase(i);
            if (Break) {
                Layers::breakBlock(l, Layers::intToVec(i), 1, false, false, false);
            }
        }
        doms.erase(c);
    }
}

void blockRelationship::breakRelationships(glm::vec2 pos)
{
    int c = Layers::vecToInt(pos);
    if (subs.count(c) >= 1) {
        Layers::breakBlock(l, subs[c]);
        return;
    }
    if (doms.count(c) >= 1) {
        Layers::breakBlock(l, pos);
    }
}

void blockRelationship::clearSubs(glm::vec2 pos, bool Break)
{
    int c = Layers::vecToInt(pos);
    if (doms.count(c) >= 1) {
        for (auto i : doms[c]) {
            subs.erase(i);
            if (Break) {
                Layers::breakBlock(l, Layers::intToVec(i));
            }
        }
        doms.erase(c);
    }
}

void blockRelationship::clearDoms(glm::vec2 pos, bool Break, bool breakdom)
{
    int c = Layers::vecToInt(pos);
    if (subs.count(c) >= 1) {
        doms[Layers::vecToInt(subs[c])].erase(c);
        if (Break) {
            Layers::breakBlock(l, pos);
        }
        if (breakdom) {
            Layers::breakBlock(l, subs[c]);
        }
    }
}

bool blockRelationship::isSub(glm::vec2 pos)
{
    return subs.count(Layers::vecToInt(pos));
}

bool blockRelationship::isDom(glm::vec2 pos)
{
    return doms.count(Layers::vecToInt(pos));
}

void blockRelationship::save(std::ofstream* file)
{
    int nameSize = layerName.size();
    file->write((char*)(&nameSize), sizeof(nameSize));
    file->write(layerName.c_str(), layerName.size());

    int subsSize = subs.size();
    file->write((char*)(&subsSize), sizeof(subsSize));

    for (const auto& sub : subs) {
        file->write((char*)(&sub.first), sizeof(sub.first));
        file->write((char*)(&sub.second), sizeof(sub.second));
    }

    int domSize = doms.size();
    file->write((char*)(&domSize), sizeof(domSize));

    for (const auto& dom : doms) {
        file->write((char*)(&dom.first), sizeof(dom.first));

        int setSize = dom.second.size();
        file->write((char*)(&setSize), sizeof(setSize));

        for (int val : dom.second) {
            file->write((char*)(&val), sizeof(val));
        }
    }
}

void blockRelationship::load(std::ifstream* file)
{
    int nameSize;
    file->read((char*)(&nameSize), sizeof(nameSize));
    std::string LAYERNAME;
    char c;
    for(int i = 0; i < nameSize; i++){
        file->get(c);
        LAYERNAME += c;
    }
    layerName = LAYERNAME;

    int subsSize;
    file->read((char*)(&subsSize), sizeof(subsSize));
    for (int i = 0; i < subsSize; i++) {
        int key;
        glm::vec2 val;

        file->read((char*)(&key), sizeof(key));
        file->read((char*)(&val), sizeof(val));

        subs.insert(std::make_pair(key, val));
    }

    int domsSize;
    file->read((char*)(&domsSize), sizeof(domsSize));

    for (int i = 0; i < domsSize; i++) {
        int key;
        file->read((char*)(&key), sizeof(key));

        int setSize;
        file->read((char*)(&setSize), sizeof(setSize));

        std::set<int> val;
        for (int j = 0; j < setSize; j++) {
            int setValue;
            file->read((char*)(&setValue), sizeof(setValue));
            val.insert(setValue);
        }

        doms.insert(std::make_pair(key, val));
    }

    l = Layers::getLayer(layerName);
}

