#pragma once

#include <ECS/ECS.h>
#include <core.h>

enum itemAnim {
    ia_DEFAULT,
    ia_STAB,
    ia_USE,
    ia_NONE
};

struct toolC {
    float frames;
    float maxframes;
    bool neki;
    itemAnim animType;
};

class toolSystem : public SystemBase {
public:
    void Update();
    void onRegister() {};
    static void makeTool(int ent, glm::vec2 pos, std::string item, float usespeed, glm::mat4 = glm::mat4(1.0f), float damagemult = 1);
    toolSystem() {
        componentArray = std::make_shared<ComponentArray<toolC>>();
    }
};
