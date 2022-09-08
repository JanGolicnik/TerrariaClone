#pragma once

#include <core.h>

namespace DroppedItems
{
    struct droppedItem {
        const char* item;
        glm::vec2 velocity;
        glm::vec2 pos;
        bool collectable;
    };

    extern GLuint idVA;
    extern GLuint idVB;
    extern GLuint idIB;

    extern const int maxItemDrops;
    extern GLuint itemdropsShaderID;
    extern std::vector<droppedItem> droppedItems;

     void dropItem(int x, int y, const char* name, bool collectable = true, glm::vec2 vel = glm::vec2(0.0f));
     void create();
     void render(glm::mat4 camTrans);
     void update(float grav);

    extern SpriteVertex* idVertices;
};

