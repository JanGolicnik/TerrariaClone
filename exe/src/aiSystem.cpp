#include <aiSystem.h>

#include <input.h>
#include <Window.h>

void aiSystem::Update()
{
    static int updated = 0;
    updated++;
    auto arr = static_cast<ComponentArray<aiC>*>(componentArray.get());
    for (int i = 0; i < arr->components.size(); i++) {
        auto phys = ECS::getComponent<physicsC>(arr->componentToEntity[i]);
        auto* p = &arr->components[i];

        for (int j = 0; j < p->actions.size(); j++) {
            if (p->actions[j].lastUpdate == 0) p->actions[j].lastUpdate = updated + p->actions[j].timer;
            if ((updated - p->actions[j].lastUpdate) > p->actions[j].timer) {
                p->actions[j].repeat--;
                p->actions[j].func(p, arr->componentToEntity[i], phys, arr, &(p->actions[j]), updated);
                if (p->actions[j].repeat == 0) {
                    p->actions.erase(p->actions.begin() + j);
                    j--;
                    continue;
                }
                p->actions[j].lastUpdate = updated;
            }
        }

        if (p->onclick) {
            if (input::pressed(k_SECONDARY)) {
                glm::vec2 mc = Window::mouseBlockCoords(false);
                if (phys->position->x - phys->size.x / 2.0f < mc.x &&
                    phys->position->x + phys->size.x / 2.0f > mc.x &&
                    phys->position->y + phys->size.y / 2.0f > mc.y &&
                    phys->position->y - phys->size.y / 2.0f < mc.y) {
                    p->onclick(p, arr->componentToEntity[i], phys, arr, nullptr, updated);
                }
            }
        }


        if (p->onupdate)
            p->onupdate(p, arr->componentToEntity[i], phys, arr, nullptr, updated);
    }
}

aiStat* aiSystem::getStat(aiC* p, std::string name, aiStat def) {
    if (p->stats.count(name) <= 0) {
        p->stats[name] = def;
    }
    return &p->stats[name];
}

aiStat* aiSystem::getStatFast(aiC* p, std::string name) {
    return &p->stats[name];
}