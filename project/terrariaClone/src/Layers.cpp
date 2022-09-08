#include "Layers.h"
#include <utils.h>
#include <DroppedItems.h>

#include<BlockManager.h>

namespace Layers {

    glm::ivec2 mapSize;
    glm::ivec2 blocksOnScreen;
    glm::ivec2 drawnBlocks;
    int nQuads;
    int nDrawnIndices;
    void addLayer(const char* name, float darkness, int drawingOffset)
    {
        Layer l;

        l.name = name;

        l.mdrawingOffset = drawingOffset;

        l.mvertices = new BlockVertex[nQuads * 4 + 10 * mapSize.y * 4 + drawnBlocks.x * mapSize.y * 4];
        l.mindices = new uint32_t[nDrawnIndices];
        l.mdarkness = darkness;

        glCreateVertexArrays(1, &l.mlayerVA);
        glBindVertexArray(l.mlayerVA);

        glCreateBuffers(1, &l.mlayerVB);
        glBindBuffer(GL_ARRAY_BUFFER, l.mlayerVB);
        glBufferData(GL_ARRAY_BUFFER, sizeof(BlockVertex) * (drawnBlocks.x + 10) * mapSize.y * 4, nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (const void*)offsetof(BlockVertex, Position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (const void*)offsetof(BlockVertex, SpriteCoords));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (const void*)offsetof(BlockVertex, TexCoords));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (const void*)offsetof(BlockVertex, light));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(BlockVertex), (const void*)offsetof(BlockVertex, breaking));

        int offset = 0;
        for (int i = 0; i < nDrawnIndices; i += 6) {
            l.mindices[i + 0] = 0 + offset;
            l.mindices[i + 1] = 1 + offset;
            l.mindices[i + 2] = 2 + offset;

            l.mindices[i + 3] = 2 + offset;
            l.mindices[i + 4] = 3 + offset;
            l.mindices[i + 5] = 0 + offset;

            offset += 4;
        }

        glCreateBuffers(1, &l.mlayerIB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, l.mlayerIB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * nDrawnIndices, &l.mindices[0], GL_STATIC_DRAW);


        layers.push_back(l);

    }

    Layer* getLayer(const char* l)
    {
        for (int i = 0; i < layers.size(); i++) {
            if (layers[i].name == l) {
                return &layers[i];
            }
        }
        return nullptr;
    }

    void setUp(int blocksize, glm::ivec2 fmapSize, glm::ivec2 fscreenRes)
    {
        mapSize = fmapSize;
        nQuads = mapSize.x * mapSize.y;
        blocksOnScreen.x = fscreenRes.x / halfblock;
        blocksOnScreen.y = fscreenRes.y / halfblock;
        drawnBlocks.x = blocksOnScreen.x + 10;
        drawnBlocks.y = blocksOnScreen.y + 1;
        nDrawnIndices = drawnBlocks.x * mapSize.y * 6;
    }

    bool placeBlock(Layer* l, int x, int y, const char* type, float light, int size, bool checkifempty, float setlight)
    {
        if (isBlockValid(l, x, y)) {
            std::array<BlockVertex, 4> quad = utils::CreateQuad(x, y, BlockM::textures.at(type), light, BlockM::info[type].width);

            if (size <= 0) {
                return true;
            }

            if (checkifempty) {
                if (queryBlock(l, x, y) == "empty") {
                    memcpy(l->mvertices + (x * (int)mapSize.y * 4) + (y * 4), quad.data(), 4 * sizeof(BlockVertex));
                    if (setlight != 0.0f) {
                        BlockM::lights.push_back(glm::vec3(x, y, setlight));
                    }
                    if (size > 1) {
                        placeBlock(l, x + 1, y, type, size - 1);
                        placeBlock(l, x - 1, y, type, size - 1);
                        placeBlock(l, x, y + 1, type, size - 1);
                        placeBlock(l, x, y - 1, type, size - 1);
                    }

                    return true;
                }
                else
                    return false;
            }
            else {
                memcpy(l->mvertices + (x * (int)mapSize.y * 4) + (y * 4), quad.data(), 4 * sizeof(BlockVertex));
                if (setlight != 0.0f) {
                    BlockM::lights.push_back(glm::vec3(x, y, setlight));
                }
                if (size > 1) {
                    placeBlock(l, x + 1, y, type, size - 1);
                    placeBlock(l, x - 1, y, type, size - 1);
                    placeBlock(l, x, y + 1, type, size - 1);
                    placeBlock(l, x, y - 1, type, size - 1);
                }

                return true;
            }
        }
        else {
            return false;
        }
    }

    bool placeBlock(int x, int y, const char* type, float light, int size, bool checkifempty, float setlight)
    {
        if (BlockM::info.count(type) == 1) {
            Layer* l = getLayer(BlockM::info[type].layer);
            if (placeBlock(l, x, y, type, light, size, checkifempty, setlight)) {
                return true;
            }
            else
                return false;
        }
        return false;
    }

    void breakBlock(Layer* l, int x, int y, int size, bool dropitem)
    {
        if (size <= 0) {
            return;
        }
        if (isBlockValid(l, x, y)) {
            const char* b = queryBlock(l, x, y);
            if (b != "empty") {
                if (dropitem)
                    DroppedItems::dropItem(x, y, queryBlock(l, x, y));
                auto quad = utils::CreateQuad(x, y, BlockM::textures.at("empty"), 0.0f);
                memcpy(l->mvertices + (x * mapSize.y * 4) + (y * 4), quad.data(), 4 * sizeof(BlockVertex));
            }
            breakBlock(l, x + 1, y, size - 1, dropitem);
            breakBlock(l, x - 1, y, size - 1, dropitem);
            breakBlock(l, x, y + 1, size - 1, dropitem);
            breakBlock(l, x, y - 1, size - 1, dropitem);
        }
    }

    const char* damageBlock(Layer* l, int x, int y, int size, bool dropitem) {
        if (size > 0) {
            if (isBlockValid(l, x, y)) {
                if (l->mvertices[x * mapSize.y * 4 + y * 4].breaking <= 1.0f) {
                    const char* b = queryBlock(l, x, y);
                    breakBlock(l, x, y, 1, dropitem);
                    return b;
                }
                else {
                    l->mvertices[x * mapSize.y * 4 + y * 4].breaking -= 1.0f;
                    l->mvertices[x * mapSize.y * 4 + y * 4 + 1].breaking -= 1.0f;
                    l->mvertices[x * mapSize.y * 4 + y * 4 + 2].breaking -= 1.0f;
                    l->mvertices[x * mapSize.y * 4 + y * 4 + 3].breaking -= 1.0f;
                }
            }
            damageBlock(l, x + 1, y, size - 1, dropitem);
            damageBlock(l, x - 1, y, size - 1, dropitem);
            damageBlock(l, x, y + 1, size - 1, dropitem);
            damageBlock(l, x, y - 1, size - 1, dropitem);

            return "empty";
        }
    }

    bool isBlockValid(Layer* l, int x, int y)
    {
        if (x >= 0 && x < mapSize.x && y >= 0 && y < mapSize.y) {
            return true;
        }
        else {
            return false;
        }
    }

    const char* queryBlock(Layer* l, int x, int y)
    {
        const char* block = "empty";
        if (isBlockValid(l, x, y)) {
            BlockVertex v = l->mvertices[x * mapSize.y * 4 + y * 4];
            block = BlockM::getBFromCoords(v.SpriteCoords, true);
            return block;
        }
        else {
            return "empty";
        }
    }

    std::array<BlockVertex, 4> queryQuad(Layer* l, int x, int y)
    {

        std::array<BlockVertex, 4> quad = {};
        if (isBlockValid(l, x, y)) {
            int c = x * mapSize.y * 4 + y * 4;
            quad[0] = l->mvertices[c];
            quad[1] = l->mvertices[c + 1];
            quad[2] = l->mvertices[c + 2];
            quad[3] = l->mvertices[c + 3];
        }
        return quad;
    }

    BlockVertex queryVert(Layer* l, int x, int y)
    {
        BlockVertex vert = {};
        if (isBlockValid(l, x, y)) {
            int c = x * mapSize.y * 4 + y * 4;
            vert = l->mvertices[c];
        }
        return vert;
    }

    void setLight(Layer* l, int x, int y, float light)
    {
        if (isBlockValid(l, x, y)) {
            l->mvertices[(x * mapSize.y * 4) + (y * 4)].light = light;
            l->mvertices[(x * mapSize.y * 4) + (y * 4 + 1)].light = light;
            l->mvertices[(x * mapSize.y * 4) + (y * 4 + 2)].light = light;
            l->mvertices[(x * mapSize.y * 4) + (y * 4 + 3)].light = light;
        }
    }

    void addLight(Layer* l, int x, int y, float light)
    {
        if (isBlockValid(l, x, y)) {
            int bx = (x * mapSize.y * 4);
            int by = (y * 4);
            l->mvertices[bx + by].light += light;
            if (l->mvertices[bx + by].light > 1.0f)         l->mvertices[bx + by].light = 1.0f;
            if (l->mvertices[bx + by].light < 0.0f)         l->mvertices[bx + by].light = 0.0f;
            l->mvertices[bx + by + 1].light += light;
            if (l->mvertices[bx + by + 1].light > 1.0f)         l->mvertices[bx + by + 1].light = 1.0f;
            if (l->mvertices[bx + by + 1].light < 0.0f)         l->mvertices[bx + by + 1].light = 0.0f;
            l->mvertices[bx + by + 2].light += light;
            if (l->mvertices[bx + by + 2].light > 1.0f)         l->mvertices[bx + by + 2].light = 1.0f;
            if (l->mvertices[bx + by + 2].light < 0.0f)         l->mvertices[bx + by + 2].light = 0.0f;
            l->mvertices[bx + by + 3].light += light;
            if (l->mvertices[bx + by + 3].light > 1.0f)         l->mvertices[bx + by + 3].light = 1.0f;
            if (l->mvertices[bx + by + 3].light < 0.0f)         l->mvertices[bx + by + 3].light = 0.0f;
        }
    }

    void updateBlock(Layer* l, int x, int y)
    {
        if (isBlockValid(l, x, y)) {
            const char* type = queryBlock(l, x, y);

            if (type == "trunk") {
                const char* t2 = queryBlock(l, x, y - 1);
                if (t2 != "trunk" && t2 != "trunkbase1") {
                    breakBlock(l, x, y, 1, true);
                }
            }
            else
                if (type == "trunkbase2") {
                    Layer* blocks = getLayer("blocks");
                    const char* t2 = queryBlock(l, x + 1, y);
                    const char* t3 = queryBlock(blocks, x, y - 1);
                    if (t2 != "trunkbase1" || t3 != "dirt") {
                        breakBlock(l, x, y, 1, true);
                    }
                }
                else
                    if (type == "trunkbase3") {
                        Layer* blocks = getLayer("blocks");
                        const char* t2 = queryBlock(l, x - 1, y);
                        const char* t3 = queryBlock(blocks, x, y - 1);
                        if (t2 != "trunkbase1" || t3 != "dirt") {
                            breakBlock(l, x, y, 1, true);
                        }
                    }
                    else
                        if (type == "trunkbase1") {
                            Layer* blocks = getLayer("blocks");
                            const char* t2 = queryBlock(blocks, x, y - 1);
                            if (t2 != "dirt") {
                                breakBlock(l, x, y, 1, true);
                            }
                        }
                        else
                            if (type == "leaves") {
                                const char* t2 = queryBlock(l, x + 3, y - 1);
                                if (t2 != "trunk") {
                                    breakBlock(l, x, y, 1, true);
                                }
                            }
                            else
                                if (type == "grassdecor" || type == "grassdecor2") {
                                    Layer* blocks = getLayer("blocks");
                                    const char* t2 = queryBlock(blocks, x, y - 1);
                                    if (t2 != "grass" && t2 != "grass2" && t2 != "dirt") {
                                        breakBlock(l, x, y);
                                    }
                                }
                                else
                                    if (type == "acorn") {
                                        placeBlock(x, y, "sapling");
                                    }
                                    else
                                        if (type == "sapling") {
                                            Layer* blocks = getLayer("blocks");
                                            const char* t2 = queryBlock(blocks, x, y - 1);
                                            if (t2 != "grass" && t2 != "grass2" && t2 != "dirt") {
                                                breakBlock(l, x, y);
                                            }
                                            if (rand() % 600 == 0) {
                                                int height = rand() % 5 + 10;
                                                placeBlock(x, y, "trunkbase1");
                                                placeBlock(x - 1, y, "trunkbase2");
                                                placeBlock(x + 1, y, "trunkbase3");
                                                for (int y2 = 1; y2 < height; y2++) {
                                                    placeBlock(x, y2 + y, "trunk");
                                                }
                                                placeBlock(x - 3, y + height, "leaves", 0.0f, 1.0f, false);
                                                placeBlock(x, y - 1, "dirt");
                                                placeBlock(x + 1, y - 1, "dirt");
                                                placeBlock(x - 1, y - 1, "dirt");
                                            }
                                        }
                                        else
                                            if (type == "cactus") {
                                                Layer* decor = getLayer("decor");
                                                Layer* blocks = getLayer("blocks");
                                                const char* t2 = queryBlock(decor, x, y + 1);
                                                const char* t3 = queryBlock(blocks, x, y - 1);
                                                if (t2 != "cactus" && t2 != "cactustop") {
                                                    placeBlock(decor, x, y, "cactustop");
                                                }
                                                t2 = queryBlock(decor, x, y - 1);
                                                if (t3 == "empty" && (t2 != "cactus")) {
                                                    breakBlock(decor, x, y, 1, true);
                                                }
                                            }
                                            else
                                                if (type == "cactustop") {
                                                    Layer* decor = getLayer("decor");
                                                    Layer* blocks = getLayer("blocks");
                                                    const char* t3 = queryBlock(blocks, x, y - 1);
                                                    const char* t2 = queryBlock(decor, x, y - 1);
                                                    if (t3 == "empty" && (t2 != "cactus")) {
                                                        breakBlock(decor, x, y, 1, true);
                                                    }
                                                }
                                                else
                                                    if (type == "sand") {
                                                        Layer* blocks = getLayer("blocks");
                                                        const char* t2 = queryBlock(blocks, x, y - 1);
                                                        if (t2 == "empty") {
                                                            breakBlock(blocks, x, y);
                                                            placeBlock(x, y - 1, "sand");
                                                        }
                                                    }
                                                    else
                                                        if (type == "dirt") {
                                                            Layer* decor = getLayer("decor");
                                                            Layer* bg = getLayer("bg");
                                                            const char* t2 = queryBlock(l, x, y + 1);
                                                            const char* t3 = queryBlock(decor, x, y + 1);
                                                            const char* t4 = queryBlock(bg, x, y + 1);
                                                            if (t2 == "empty" && t3 == "empty" && t4 == "empty") {
                                                                placeBlock(l, x, y, "grass");
                                                            }
                                                        }
                                                        else
                                                            if (type == "grass" || type == "grass2") {
                                                                Layer* decor = getLayer("decor");
                                                                const char* t2 = queryBlock(l, x, y + 1);
                                                                const char* t3 = queryBlock(decor, x, y + 1);
                                                                if (t2 != "empty" || !(t3 == "empty" || t3 == "grassdecor" || t3 == "grassdecor2")) {
                                                                    placeBlock(l, x, y, "dirt");
                                                                }
                                                            }
                                                            else if (type == "corrupttrunk") {
                                                                const char* t2 = queryBlock(l, x, y - 1);
                                                                if (t2 != "corrupttrunk" && t2 != "corrupttrunkbase1") {
                                                                    breakBlock(l, x, y, 1, true);
                                                                }
                                                            }
                                                            else
                                                                if (type == "corrupttrunkbase2") {
                                                                    Layer* blocks = getLayer("blocks");
                                                                    const char* t2 = queryBlock(l, x + 1, y);
                                                                    const char* t3 = queryBlock(blocks, x, y - 1);
                                                                    if (t2 != "corrupttrunkbase1" || t3 != "corruptdirt") {
                                                                        breakBlock(l, x, y, 1, true);
                                                                    }
                                                                }
                                                                else
                                                                    if (type == "corrupttrunkbase3") {
                                                                        Layer* blocks = getLayer("blocks");
                                                                        const char* t2 = queryBlock(l, x - 1, y);
                                                                        const char* t3 = queryBlock(blocks, x, y - 1);
                                                                        if (t2 != "corrupttrunkbase1" || t3 != "corruptdirt") {
                                                                            breakBlock(l, x, y, 1, true);
                                                                        }
                                                                    }
                                                                    else
                                                                        if (type == "corrupttrunkbase1") {
                                                                            Layer* blocks = getLayer("blocks");
                                                                            const char* t2 = queryBlock(blocks, x, y - 1);
                                                                            if (t2 != "corruptdirt") {
                                                                                breakBlock(l, x, y, 1, true);
                                                                            }
                                                                        }
                                                                        else
                                                                            if (type == "corruptleaves") {
                                                                                const char* t2 = queryBlock(l, x + 3, y - 1);
                                                                                if (t2 != "corrupttrunk") {
                                                                                    breakBlock(l, x, y, 1, true);
                                                                                }
                                                                            }
                                                                            else
                                                                                if (type == "corruptgrassdecor" || type == "corruptgrassdecor2") {
                                                                                    Layer* blocks = getLayer("blocks");
                                                                                    const char* t2 = queryBlock(blocks, x, y - 1);
                                                                                    if (t2 != "corruptgrass" && t2 != "corruptgrass2" && t2 != "corruptdirt") {
                                                                                        breakBlock(l, x, y);
                                                                                    }

                                                                                }
        }
    }
}