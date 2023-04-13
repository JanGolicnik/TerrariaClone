#pragma once

#include <core.h>
#include <ECS/ECS.h>

struct droppedItemC {
    std::string name;
    int amount;
    int timer = 60;
    bool rotate = true;
};


class droppedItemSystem : public SystemBase {
public:
    void Update();
    void onRegister() {};

    bool dropItem(glm::vec2 pos, std::string_view name, int amount, bool collectable = true, glm::vec2 vel = glm::vec2(0.0f));

    droppedItemSystem() {
        componentArray = std::make_shared<ComponentArray<droppedItemC>>();
    }
};
