#include "Player.h"

#include <utils.h>
#include <BlockManager.h>

#include <animations.h>
#include <Inventory.h>

namespace Player {

    glm::vec2 velocity;
    float ms;
    float jumpms;
    bool digging;
    bool placing;
    int invBlock;
    Layers::Layer* editLayer;
    int editsize;
    float playerVertices[20];
    uint32_t playerIndices[6];
    glm::vec2 pos;
    GLuint playerVB;
    GLuint playerVA;
    GLuint playerIB;
    float editRange;
    float pickupRange;
    glm::mat4 playerTrans;
    float width;
    float height;
    glm::vec2 center;
    int anim;

    void update(float grav)
    {
        center = { pos.x + width / 2, pos.y + height / 2 };

        Layers::Layer* blocks = Layers::getLayer("blocks");

        static glm::vec2 lastPos = glm::vec2(0.0f);

        static glm::vec2 c;
        c = { pos.x, pos.y - 0.1 + velocity.y };
        if (Layers::queryBlock(blocks, c.x, c.y) == "empty"
            && Layers::queryBlock(blocks, c.x + 1.0f, c.y) == "empty") {
            velocity.y -= grav;
        }
        else if (velocity.y < 0) {
            velocity.y = 0;
        }

        c = { pos.x + velocity.x, pos.y };
        if ((Layers::queryBlock(blocks, c.x, c.y) != "empty"
            || Layers::queryBlock(blocks, c.x, c.y + 1) != "empty"
            || Layers::queryBlock(blocks, c.x, c.y + 2) != "empty")
            && velocity.x < 0) {
            pos.x -= velocity.x;
        }

        c = { pos.x + width + velocity.x, pos.y };
        if ((Layers::queryBlock(blocks, c.x, c.y) != "empty"
            || Layers::queryBlock(blocks, c.x, c.y + 1) != "empty"
            || Layers::queryBlock(blocks, c.x, c.y + 2) != "empty")
            && velocity.x > 0) {
            pos.x -= velocity.x;
        }

        c = { pos.x, pos.y + height + velocity.y };
        if ((Layers::queryBlock(blocks, c.x, c.y) != "empty"
            || Layers::queryBlock(blocks, c.x + width, c.y) != "empty")
            && velocity.y > 0) {
            velocity.y = 0;
        }

        pos += glm::vec2(velocity.x, velocity.y);
        playerTrans = glm::translate(playerTrans, glm::vec3((pos.x - lastPos.x), (pos.y - lastPos.y), 0.0f));
        lastPos = pos;

        glBindBuffer(GL_ARRAY_BUFFER, playerVB);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(SpriteVertex), utils::CreateSpriteRect(0, 0, BlockM::textures.at(AnimManager::getFrame(anim)), width, 3).data());

        if (!Inventory::shown) {
            if (invBlock > Inventory::rowLength - 1) {
                invBlock = 0;
            }
            else if (invBlock < 0) {
                invBlock = Inventory::rowLength - 1;
            }
        }
        else {
            if (invBlock < 0) {
                invBlock = 0;
            }
            if (invBlock > Inventory::craftableItems.size() - 1) {
                invBlock = Inventory::craftableItems.size() - 1;
            }
        }

    }

    void render(GLuint spriteShaderID, glm::mat4 camTrans)
    {
        glUseProgram(spriteShaderID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, BlockM::spriteSheet);
        glUniform1i(2, 0);
        glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(playerTrans));
        glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(camTrans));

        glBindVertexArray(playerVA);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void create()
    {
        velocity = glm::vec2(0.0f);
        ms = 0.9f;
        jumpms = 0.6f;
        digging = false;
        placing = false;
        invBlock = 0;
        editLayer = Layers::getLayer("blocks");
        editsize = 1;
        pos = glm::vec2(Layers::mapSize.x/2, Layers::mapSize.y);
        width = 1.5f;
        height = 2.8f;
        editRange = 10.0f;
        pickupRange = 4.0f;
        center = { pos.x + width / 2, pos.y + height / 2 };

        playerTrans = glm::mat4(1.0f);

        glCreateVertexArrays(1, &playerVA);
        glBindVertexArray(playerVA);

        glCreateBuffers(1, &playerVB);
        glBindBuffer(GL_ARRAY_BUFFER, playerVB);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(SpriteVertex), utils::CreateSpriteRect(0, 0, BlockM::textures.at("player"), width, 3).data(), GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, Position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, SpriteCoords));

        uint32_t i[6] = {
            0,1,3,
            1,2,3
        };

        memcpy(playerIndices, i, 6 * sizeof(uint32_t));

        glCreateBuffers(1, &playerIB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, playerIB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(playerIndices), playerIndices, GL_STATIC_DRAW);

        AnimManager::watchAnim("playeridle", &anim);

    }


}