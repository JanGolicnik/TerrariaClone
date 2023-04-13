#pragma once

#include <core.h>
#include <ECS/ECS.h>
#include <physicsSystem.h>

struct particleEmmiterC {
    float rate = 0;
    float countdown = 0;
    std::string tex = "err";
    float amount = 0;
    float ms = 0;
    glm::vec2 dir = { 0,0 };
    glm::vec2 size = { 1,1 };
    glm::vec2 velovertime = glm::vec2(0);
    int lifespan = 0;
    int particleLifespan = 0;
    float rotation = 0;
    float randrotation = 0;
    float sizeovertime = 0;
    float opacityovertime = 0;
    float randangle = 0;
    float randomizems = 0;
    float randomizeamount = 0;
    float randomizerate = 0;
    float randomizelifespan = 0;
    float randomizescale = 0;
    bool stoponcollision = true;
    bool smallerAsDie = false;
    bool disappearAsDie = false;
    float weight = 0;
    bool killentity = false;
    float opacity = 1;
    bool stopAsDie = false;
    bool setup = true;
    glm::vec3 light = glm::vec3(0);
    float radius = 0;
    float randradius = 0;
    float speedmultiplier = 0;
    int parent = -1;
    glm::vec3 color = glm::vec3(1);
    bool matchparticledir = false;
};

class particleEmmiterSystem : public SystemBase {
public:
    void Update();
    void onRegister() {};
    void spawn(particleEmmiterC* pec, ComponentArray<particleEmmiterC>* arr, int i, physicsC* phys);
    particleEmmiterSystem() {
        componentArray = std::make_shared<ComponentArray<particleEmmiterC>>();
    }
};
