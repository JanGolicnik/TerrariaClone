#include "DroppedItems.h"

#include <utils.h>
#include <Inventory.h>
#include <Layers.h>
#include <globals.h>
#include <Player.h>

#include <BlockManager.h>
#include <ECS/ECS.h>
#include <componentsystems.h>

#include <animations.h>

namespace DroppedItems {
    bool dropItem(int x, int y, std::string_view item, int amount, bool collectable, glm::vec2 vel)
    {
        if (item != "empty") {
            float velx = (rand() % 200 - 100) / 2000.0f;
            float vely = (rand() % 100) / 600.0f;
            if (vel.x != 0) {
                velx = vel.x;
            }
            if (vel.y != 0) {
                vely = vel.y;
            }
            int tmp = ECS::newEntity();
            auto ppos = std::make_shared<glm::vec2>(x, y);
            auto info = &items::info[item.data()];

            drawC dc;
            dc.position = ppos;
            dc.tex = info->tex;
            dc.mat = glm::mat4(1.0f);
            dc.size = { 1,1 };
            dc.hidden = false;
            dc.parent = globals::itemLayer;
            if (info->anim != "") AnimManager::watchAnim(info->anim, &dc.anim);

            ECS::queueComponent<physicsC>(tmp, { ppos,  { velx, vely }, {1,1}, false});
            drawSystem::addComponent(tmp, &dc);
            ECS::queueComponent<droppedItemC>(tmp, { item.data(), amount, collectable ? 0 : 60 });
            return true;
        }
        return false;
    }
}