#pragma once

#include <core.h>
#include <Layers.h>

namespace Player
{
    extern glm::vec2 pos;
    extern glm::vec2 velocity;
    extern float ms;
    extern float jumpms;
    extern glm::mat4 playerTrans;

    extern float pickupRange;
    extern float editRange;
    extern int editsize;
    extern int invBlock;
    
    extern bool digging;
    extern bool placing;
    extern Layers::Layer* editLayer;

    extern float width;
    extern float height;
    extern glm::vec2 center;

    extern float playerVertices[20];
    extern uint32_t playerIndices[6];
    extern GLuint playerVB;
    extern GLuint playerVA;
    extern GLuint playerIB;

    extern int anim;

    void create();
    void update(float grav);
    void render(GLuint spriteShaderID, glm::mat4 camTrans);
    void updateSprite();
    void editMap();
};

