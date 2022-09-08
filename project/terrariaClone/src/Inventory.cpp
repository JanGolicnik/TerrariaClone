#include "Inventory.h"

#include <utils.h>
#include <Layers.h>

#include <BlockManager.h>
#include <Player.h>

#include <text.h>

namespace Inventory {

    GLuint VA;
    GLuint VB;
    GLuint IB;
    const int rowLength = 9;
    const int numRows = 5;
    bool shown;
    int shownRows;
    const int recipeSize = 5;
    const int invBSize = (rowLength + 4) * numRows + 2 + recipeSize;
    //4 so za border pa hover kvadratk
    //numRows je za iteme
    //1 za item k ga craftas 
    //5 za recept
    //zadn za item k ga drzis v roki
    GLuint midTex;
    GLuint leftTex;
    GLuint rightTex;
    GLuint selectTex;
    GLuint slotTex;
    GLuint shaderID;
    glm::mat4 trans;
    SpriteVertex* vertices;
    std::array<InvItem, rowLength* numRows + 1> blocks;
    glm::vec2 hoverpos;
    float size;
    float invY;
    float startOffset;
    float invX;
    float spacing;
    float il;
    float im;
    float ir;
    float ew;
    float mw;
    float ih;
    float selSize;
    float marginBot;
    float marginLeft;
    float craftingW;
    float craftingY;
    float craftingX;
    std::vector<const char*> craftableItems;

    void render()
    {
        glUseProgram(shaderID);
        glActiveTexture(GL_TEXTURE0 + BlockM::spriteSheet);
        glBindTexture(GL_TEXTURE_2D, BlockM::spriteSheet);
        glUniform1i(0, BlockM::spriteSheet);
        glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(trans));
        glBindBuffer(GL_ARRAY_BUFFER, VB);
        glBufferSubData(GL_ARRAY_BUFFER, 0, (sizeof(SpriteVertex) * invBSize * 4), vertices);
        glBindVertexArray(VA);
        glDrawElements(GL_TRIANGLES, invBSize * 6, GL_UNSIGNED_INT, nullptr);
    }

    void refresh(double ax, double ay)
    {
        for (int i = 0; i < recipeSize; i++) {
            memcpy(vertices + (invBSize - 2 - recipeSize + i + 1) * 4, utils::CreateSpriteRect(-11, -123, BlockM::textures.at("empty"), 0, 0).data(), 4 * sizeof(SpriteVertex));
        }
        if (shown) {

            shownRows = numRows;
            if (craftableItems.size() == 0) {
                memcpy(vertices + 3 * 4, utils::CreateSpriteRect(-1231, -12, BlockM::textures.at("empty"), 0, 0).data(), 4 * sizeof(SpriteVertex));
                memcpy(vertices + (invBSize - 2 - recipeSize) * 4, utils::CreateSpriteRect(-1231, -12, BlockM::textures.at("empty"), 0, 0).data(), 4 * sizeof(SpriteVertex));
            }
            else {
                BlockInfo* info = &BlockM::info[craftableItems[Player::invBlock]];
                for (int i = 0; i < info->recipeFrom.size(); i++) {
                    memcpy(vertices + (invBSize - 2 - recipeSize + i + 1) * 4, utils::CreateSpriteRect(craftingX + (craftingW / 4 * (i * 3 + 4)), craftingY + craftingW / 4, BlockM::textures.at(info->recipeFrom[i].name), craftingW / 2, craftingW / 2).data(), 4 * sizeof(SpriteVertex));
                }
                memcpy(vertices + 3 * 4, utils::CreateSpriteRect(craftingX, craftingY, BlockM::textures.at("invempty"), craftingW * (info->recipeFrom.size() + 1), craftingW).data(), 4 * sizeof(SpriteVertex));
                memcpy(vertices + (invBSize - 2 - recipeSize) * 4, utils::CreateSpriteRect(craftingX + craftingW / 4, craftingY + craftingW / 4, BlockM::textures.at(craftableItems[Player::invBlock]), craftingW / 2, craftingW / 2).data(), 4 * sizeof(SpriteVertex));
            }
        }
        else {
            shownRows = 1;
            memcpy(vertices + (invBSize - 2 - recipeSize) * 4, utils::CreateSpriteRect(-10, -10, BlockM::textures.at("empty"), 0, 0).data(), 4 * sizeof(SpriteVertex));
            memcpy(vertices + 3 * 4, utils::CreateSpriteRect(marginLeft + Player::invBlock * (size + spacing), invY + marginBot, BlockM::textures.at("invselect"), selSize, selSize).data(), 4 * sizeof(SpriteVertex));
        }


        for (int row = 0; row < numRows; row++) {
            int rowi = row * (rowLength + 4);
            int bi = row * rowLength;
            if (row >= shownRows) {
                for (int i = 0; i < rowLength; i++) {
                    memcpy(vertices + (4 * (4 + i + rowi)), utils::CreateSpriteRect(-1, -1, BlockM::textures.at("empty"), 0, 0).data(), 4 * sizeof(SpriteVertex));
                }
                int ro = row * (rowLength + 4) * 4;
                memcpy(vertices + 0 * 4 + ro, utils::CreateSpriteRect(-1, -1, BlockM::textures.at("empty"), 0, 0).data(), 4 * sizeof(SpriteVertex));
                memcpy(vertices + 1 * 4 + ro, utils::CreateSpriteRect(-1, -1, BlockM::textures.at("empty"), 0, 0).data(), 4 * sizeof(SpriteVertex));
                memcpy(vertices + 2 * 4 + ro, utils::CreateSpriteRect(-1, -1, BlockM::textures.at("empty"), 0, 0).data(), 4 * sizeof(SpriteVertex));

            }
            else {
                for (int i = 0; i < rowLength; i++) {
                    if (blocks[i + bi].num == 0) {
                        blocks[i + bi].item = "invempty";
                    }
                    memcpy(vertices + (4 * (4 + i + rowi)), utils::CreateSpriteRect(startOffset + i * (size + spacing), invY + (ih - size) / 2 + row * -ih, BlockM::textures.at(blocks[i + bi].item), size, size).data(), 4 * sizeof(SpriteVertex));
                }
                int ro = row * (rowLength + 4) * 4;
                memcpy(vertices + 0 * 4 + ro, utils::CreateSpriteRect(il, invY + row * -ih, BlockM::textures.at("invleft"), ew, ih).data(), 4 * sizeof(SpriteVertex));
                memcpy(vertices + 1 * 4 + ro, utils::CreateSpriteRect(im, invY + row * -ih, BlockM::textures.at("invmid"), mw, ih).data(), 4 * sizeof(SpriteVertex));
                memcpy(vertices + 2 * 4 + ro, utils::CreateSpriteRect(ir, invY + row * -ih, BlockM::textures.at("invright"), ew, ih).data(), 4 * sizeof(SpriteVertex));

            }
            float mx = ax * 2 - 1;
            float my = ay * 2 - 1;

            hoverpos = { mx,my };
            updateText(hoverpos);
            if (blocks[blocks.size() - 1].item == "invempty") {
                memcpy(vertices + (invBSize - 1) * 4, utils::CreateSpriteRect(-1, -1, BlockM::textures.at("empty"), 0, 0).data(), 4 * sizeof(SpriteVertex));
            }
            else {
                memcpy(vertices + (invBSize - 1) * 4, utils::CreateSpriteRect(hoverpos.x, hoverpos.y, BlockM::textures.at(blocks[blocks.size() - 1].item), size, size).data(), 4 * sizeof(SpriteVertex));
            }

        }
    }

    void create()
    {
        utils::createShader("src/shaders/inventoryFrag.frag", "src/shaders/inventoryVert.vert", &shaderID);

        glCreateVertexArrays(1, &VA);
        glBindVertexArray(VA);

        glCreateBuffers(1, &VB);
        glBindBuffer(GL_ARRAY_BUFFER, VB);
        glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteVertex) * invBSize * 4, nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (const void*)offsetof(SpriteVertex, Position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (const void*)offsetof(SpriteVertex, SpriteCoords));

        uint32_t indices[invBSize * 6];

        int offset = 0;
        for (int i = 0; i < invBSize * 6; i += 6) {
            indices[i + 0] = 0 + offset;
            indices[i + 1] = 1 + offset;
            indices[i + 2] = 2 + offset;

            indices[i + 3] = 2 + offset;
            indices[i + 4] = 3 + offset;
            indices[i + 5] = 0 + offset;

            offset += 4;
        }

        glCreateBuffers(1, &IB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * invBSize * 6, indices, GL_STATIC_DRAW);

        vertices = new SpriteVertex[invBSize * 4];
        blocks.fill({ "invempty", 0 });

        trans = glm::mat4(1.0f);

        shown = false;

        refresh(0, 0);

        size = 0.04f;
        invY = 0.85f;
        startOffset = -0.85;
        invX = 0.5;
        spacing = (invX - (rowLength * size)) / (rowLength - 1);

        il = -0.95; // left pos
        im = -0.85; // mid pos
        ir = -0.35; // right edge pos
        ew = 0.1; // edge widths
        mw = 0.5; // mid width
        ih = 0.1; // inv height

        selSize = (ih - size);
        marginBot = (ih - selSize) / 2;
        marginLeft = im - size / 4;

        craftingY = 0;
        craftingX = -0.9;
        craftingW = 0.12;

        text::addLine("cursortext", "", { 0,0 }, 1.5f);
        text::addLine("cursortext2", "", { 0,0 }, 1.5f);

        int c = 0;
        add("ironpickaxe", 1);
        add("wood", 14);

        for (int i = 0; i < invBSize; i++) {
            memcpy(vertices + i * 4, utils::CreateSpriteRect(0, 0, BlockM::textures.at("empty"), 0, 0).data(), 4 * sizeof(SpriteVertex));
        }
    }

    bool add(const char* block, int num)
    {
        for (int i = 0; i < blocks.size(); i++) {
            if (blocks[i].item == block) {
                blocks[i].num += num;
                refreshCrafting();
                return true;
            }
        }

        for (int i = 0; i < blocks.size(); i++) {
            if (blocks[i].item == "invempty") {
                blocks[i].item = block;
                blocks[i].num = num;
                refreshCrafting();
                return true;
            }
        }
        return false;
    }

    bool addToSpot(const char* block, glm::vec2 pos, int num)
    {
        int index = pos.y * rowLength + pos.x;
        if (blocks[index].item == "invempty") {
            blocks[index].item = block;
            blocks[index].num = num;

            return true;
        }
        return false;
    }

    void remove(int index, int num)
    {
        if (index >= 0 && index < rowLength) {
            blocks[index].num -= num;
        }
        refreshCrafting();
    }

    int mouseClicked(float x, float y)
    {
        float mx = x * 2 - 1;
        float my = y * 2 - 1;

        float min = 1.0f;

        if (shown) {
            for (int i = 0; i < blocks.size() - 1; i++) {
                int x = i / 9;
                int y = i % 9;
                float ox;
                float oy;

                oy = invY + (ih - size) / 2 + x * -ih + size / 2;
                ox = startOffset + y * (size + spacing) + size / 2; //remappamo na zgorn lev kot

                if (glm::distance(glm::vec2(ox, oy), glm::vec2(mx, my)) < size / 2) {
                    if (blocks[i].item == blocks[blocks.size() - 1].item) {
                        blocks[i].num += blocks[blocks.size() - 1].num;
                        blocks[blocks.size() - 1] = { "invempty", 0 };
                    }
                    else {
                        InvItem tmp = blocks[i];
                        blocks[i] = blocks[blocks.size() - 1];
                        blocks[blocks.size() - 1] = tmp;
                        hoverpos = { mx,my };
                    }return i;
                }
            }

            if (craftableItems.size() > 0) {
                refreshCrafting();
                if (blocks[blocks.size() - 1].item == "invempty") {
                    float y = craftingY + craftingW / 2;
                    float x = craftingX + craftingW / 2;

                    if (glm::distance(glm::vec2(x, y), glm::vec2(mx, my)) < craftingW / 3) {
                        if (canCraft(craftableItems[Player::invBlock])) {
                            craft(craftableItems[Player::invBlock]);
                            blocks[blocks.size() - 1] = { craftableItems[Player::invBlock], BlockM::info.at(craftableItems[Player::invBlock]).craftedAmount };
                        }
                    }
                }
                else if (blocks[blocks.size() - 1].item == craftableItems[Player::invBlock] && craftableItems[Player::invBlock] != "invempty") {
                    float y = craftingY + craftingW / 2;
                    float x = craftingX + craftingW / 2;

                    if (glm::distance(glm::vec2(x, y), glm::vec2(mx, my)) < craftingW / 3) {
                        if (canCraft(craftableItems[Player::invBlock])) {
                            craft(craftableItems[Player::invBlock]);
                            blocks[blocks.size() - 1].num += BlockM::info.at(craftableItems[Player::invBlock]).craftedAmount;
                        }
                    }
                }
                refreshCrafting();
            }

        }
    }

    bool canCraft(const char* block)
    {
        for (int i = 0; i < BlockM::info[block].recipeFrom.size(); i++) {
            bool found = false;
            int num = BlockM::info[block].recipeFrom[i].num;
            for (int j = 0; j < blocks.size(); j++) {
                if (blocks[j].item == BlockM::info[block].recipeFrom[i].name) {
                    found = true;
                    num -= blocks[j].num;
                }
            }
            if (found == false || num > 0) {
                return false;
            }
        }
        return true;
    }

    void refreshCrafting()
    {
        craftableItems.clear();
        for (int i = 0; i < blocks.size(); i++) {
            if (blocks[i].item != "invEmpty") {
                for (int j = 0; j < BlockM::info[blocks[i].item].recipeFor.size(); j++) {
                    if (canCraft(BlockM::info[blocks[i].item].recipeFor[j])) {
                        craftableItems.push_back(BlockM::info[blocks[i].item].recipeFor[j]);
                    }
                }
            }
        }
    }

    void craft(const char* block) {
        for (int i = 0; i < BlockM::info[block].recipeFrom.size(); i++) {
            for (int m = 0; m < BlockM::info[block].recipeFrom[i].num; m++) {
                for (int j = 0; j < blocks.size(); j++) {
                    if (BlockM::info[block].recipeFrom[i].name == blocks[j].item) {
                        blocks[j].num--;
                        break;
                    }
                }
            }
        }
    }

    void updateText(glm::vec2 m)
    {
        text::lines["cursortext"].pos = glm::vec2(m.x + size / 2, m.y - size / 2);
        text::lines["cursortext2"].pos = glm::vec2(m.x + size / 2, m.y - size * 1.5f);
        if (shown) {
            for (int i = 0; i < blocks.size(); i++) {
                int x = i / 9;
                int y = i % 9;
                float ox;
                float oy;

                oy = invY + (ih - size) / 2 + x * -ih + size / 2;
                ox = startOffset + y * (size + spacing) + size / 2; //remappamo na zgorn lev kot

                if (glm::distance(glm::vec2(ox, oy), glm::vec2(m.x, m.y)) < size / 2) {
                    if (blocks[i].item != "invempty") {
                        text::updateText("cursortext", blocks[i].item);
                        text::updateText("cursortext2", std::to_string(blocks[i].num).c_str());
                    }
                    break;
                }
                else {
                    text::updateText("cursortext", "        ");
                    text::updateText("cursortext2", "        ");
                }
            }

            if (craftableItems.size() > 0) {
                if (blocks[blocks.size() - 1].item == "invempty") {
                    float y = craftingY + craftingW / 2;
                    float x = craftingX + craftingW / 2;

                    if (glm::distance(glm::vec2(x, y), glm::vec2(m.x, m.y)) < craftingW / 3) {
                        text::updateText("cursortext", craftableItems[Player::invBlock]);
                    }
                }
            }

        }
        else {
            text::updateText("cursortext", "              ");
            text::updateText("cursortext2", "              ");
        }

        text::refresh();

    }
}