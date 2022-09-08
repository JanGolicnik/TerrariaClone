#include "DroppedItems.h"

#include <utils.h>
#include <Inventory.h>
#include <Layers.h>
#include <Player.h>

#include <BlockManager.h>

namespace DroppedItems {

    GLuint idVA;
    GLuint idVB;
    GLuint idIB;
    GLuint itemdropsShaderID;
    std::vector<droppedItem> droppedItems;
    SpriteVertex* idVertices;
    const int maxItemDrops = 1000;

    void dropItem(int x, int y, const char* name, bool collectable, glm::vec2 vel)
    {
        float velx = (rand() % 200 - 100) / 1000.0f;
        float vely = (rand() % 100) / 300.0f;
        if (vel.x != 0) {
            velx = vel.x;
        }
        if (vel.y != 0) {
            vely = vel.y;
        }
        droppedItem tmp;
        tmp.collectable = collectable;
        tmp.item = BlockM::info.at(name).drops;
        tmp.velocity = { velx, vely };
        tmp.pos = { x,y };
        for (int i = 0; i < droppedItems.size(); i++) {
            if (droppedItems[i].item == "empty") {
                droppedItems[i] = tmp;
                return;
            }
        }
        droppedItems.push_back(tmp);
    }

    void create()
    {
        glCreateVertexArrays(1, &idVA);
        glBindVertexArray(idVA);

        glCreateBuffers(1, &idVB);
        glBindBuffer(GL_ARRAY_BUFFER, idVB);
        glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteVertex) * maxItemDrops * 4, nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (const void*)offsetof(SpriteVertex, Position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (const void*)offsetof(SpriteVertex, SpriteCoords));

        uint32_t indices[maxItemDrops * 6];

        int offset = 0;
        for (int i = 0; i < maxItemDrops * 6; i += 6) {
            indices[i + 0] = 0 + offset;
            indices[i + 1] = 1 + offset;
            indices[i + 2] = 2 + offset;

            indices[i + 3] = 2 + offset;
            indices[i + 4] = 3 + offset;
            indices[i + 5] = 0 + offset;

            offset += 4;
        }

        glCreateBuffers(1, &idIB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * maxItemDrops * 6, indices, GL_STATIC_DRAW);

        idVertices = new SpriteVertex[maxItemDrops * 4];
        for (int i = 0; i < maxItemDrops; i++) {
            memcpy(idVertices + i * 4, utils::CreateSpriteRect(0, 0, BlockM::textures.at("empty"), 0, 0).data(), 4 * sizeof(SpriteVertex));
        }
    }

    void render(glm::mat4 camTrans)
    {
        glUseProgram(Inventory::shaderID);

        glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(camTrans));

        glActiveTexture(GL_TEXTURE0 + BlockM::spriteSheet);
        glBindTexture(GL_TEXTURE_2D, BlockM::spriteSheet);
        glUniform1i(0, BlockM::spriteSheet);

        glBindBuffer(GL_ARRAY_BUFFER, idVB);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(SpriteVertex) * maxItemDrops * 4, idVertices);

        glBindVertexArray(idVA);
        glDrawElements(GL_TRIANGLES, maxItemDrops * 6, GL_UNSIGNED_INT, nullptr);
    }

    void update(float grav)
    {
        for (int i = 0; i < droppedItems.size(); i++) {
            if (droppedItems[i].item != "empty") {
                glm::vec2 mid = glm::vec2(droppedItems[i].pos.x + 0.5f, droppedItems[i].pos.y + 0.5f);

                droppedItems[i].velocity.x += -droppedItems[i].velocity.x / 10.0f;
                droppedItems[i].velocity.y -= grav;
                if (droppedItems[i].velocity.y < 0 && Layers::queryBlock(Layers::getLayer("blocks"), idVertices[i * 4].Position.x, idVertices[i * 4].Position.y) != "empty") {
                    droppedItems[i].velocity.y = 0;
                    droppedItems[i].collectable = true;
                }
                if (droppedItems[i].collectable == true) {
                    float dist = utils::distance(Player::center, mid);
                    if (dist < 0.5f) {
                        if (Inventory::add(droppedItems[i].item)) {
                            droppedItems[i] = { "empty", {0,0} };
                            memcpy(idVertices + i * 4, utils::CreateSpriteRect(droppedItems[i].pos.x, droppedItems[i].pos.y, BlockM::textures.at("empty"), 1, 1).data(), 4 * sizeof(SpriteVertex));
                        }
                    }
                    if (dist < Player::pickupRange) {
                        droppedItems[i].velocity.x += ((Player::center.x - mid.x) / Player::pickupRange) * 0.3;
                        droppedItems[i].velocity.y += ((Player::center.y - mid.y) / Player::pickupRange) * 0.3;
                    }
                }
                droppedItems[i].pos += droppedItems[i].velocity;
                memcpy(idVertices + i * 4, utils::CreateSpriteRect(droppedItems[i].pos.x, droppedItems[i].pos.y, BlockM::textures.at(droppedItems[i].item), 1, 1).data(), 4 * sizeof(SpriteVertex));
            }
            else {
                memcpy(idVertices + i * 4, utils::CreateSpriteRect(0, 0, BlockM::textures.at("empty"), 0, 0).data(), 4 * sizeof(SpriteVertex));
            }

        }
    }
}