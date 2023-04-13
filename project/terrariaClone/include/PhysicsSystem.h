#pragma once

#include <core.h>
#include <ECS/ECS.h>

struct physicsC {
    std::shared_ptr<glm::vec2> position;
    glm::vec2 vel = { 0,0 };
    glm::vec2 size = { 0,0 };
    bool isstatic = false;
    bool dontcollide = false;
    bool stoponcollision = true;
    bool collideleft = false;
    bool collideright = false;
    bool collidebot = false;
    bool collidetop = false;
    bool bounce = false;
    bool friction = true;
    float weight = 1;
    glm::vec2 prevpos = glm::vec2(0);
    float bounciness = 0;
    bool ignorebot = false;
    bool ignoretop = false;
    bool ignoreleft = false;
    bool ignoreright = false;
    bool wantstoskip = false;
    bool affectedByLiquid = true;
    int isinliquid = -1;
};

class physicsSystem : public SystemBase {
public:
    void Update();
    void onRegister() {};
    physicsSystem() {
        componentArray = std::make_shared<ComponentArray<physicsC>>();
    }
};