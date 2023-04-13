#include <droppedItemSystem.h>

#include <items.h>
#include <enemies.h>
#include <textures.h>
#include <animations.h>
#include <Window.h>
#include <player.h>
#include <gameLoop.h>
#include <ui.h>
#include <utils.h>

void droppedItemSystem::Update()
{
    glm::vec2 mc = Window::mouseBlockCoords(false);
    auto arr = static_cast<ComponentArray<droppedItemC>*>(componentArray.get());
    auto sys = ECS::getSystem<physicsSystem>();
    auto drawsys = ECS::getSystem<drawSystem>();
    glm::vec2 playerpos = *Player::position();
    for (int i = 0; i < arr->components.size(); i++) {
        auto p = &(arr->components[i]);
        auto phys = ECS::getComponent<physicsC>(arr->componentToEntity[i], sys);
        p->timer--;
        auto anim = items::info[p->name].anim;
        if (p->rotate) {
            auto draw = ECS::getComponent<drawC>(arr->componentToEntity[i], drawsys);
            float len = glm::length(phys->vel);
            if (len > 0.001f) {
                draw->hasmat = true;
                const float maxangle = 50;
                float strength = phys->vel.x / 0.5f;
                strength = glm::clamp(strength, -0.5f, 0.5f);
                float angle = strength * maxangle;
                draw->mat = glm::rotate(glm::mat4(1.0f), float(angle * PI / 180.0f), glm::vec3(0, 0, 1));
            }
            else {
                draw->hasmat = false;
            }
        }

        if (glm::distance(mc, *phys->position) < 1) {
            std::string text = p->name + " (" + std::to_string(p->amount) + ")";
            UI::setTStat(ECS::getComponent<uiC>(gameLoop::zoomedcursoritem), "text", text);
        }

        if (phys->isinliquid == 1 && p->name != "hellstone") {
            ECS::queueDeletion(arr->componentToEntity[i]);
        }

        phys->dontcollide = false;
        if (p->timer > 0) continue;
        if (Player::dead) continue;
        if (!UI::hotbar->hasRoomFor(p->name) && !UI::inventory->hasRoomFor(p->name)) continue;

        glm::vec2 mid = glm::vec2(phys->position->x + 0.5f, phys->position->y + 0.5f);
        float dist = glm::distance(playerpos, *phys->position);
        if (dist < 1.2f) {
            int prevAmount = p->amount;
            p->amount = Player::pickUp(p->name, p->amount);
            if (prevAmount != p->amount) {
                sounds::pickup();
            }
        }
        if (dist < Player::pickupRange) {
            phys->dontcollide = true;
            phys->isstatic = false;
            phys->vel += utils::approach(phys->vel, glm::normalize(playerpos - *phys->position) * glm::vec2(1.0f), 1);
            phys->vel = glm::clamp(phys->vel, -1.0f, 1.0f);
        }

        if (p->amount == 0) {
            ECS::queueDeletion(arr->componentToEntity[i]);
        }
    }
}

bool droppedItemSystem::dropItem(glm::vec2 pos, std::string_view item, int amount, bool collectable, glm::vec2 vel)
{
    if (item == "empty") return false;
    if (amount == 0) return false;
    if (items::info.count(item.data()) <= 0) return false;

    auto info = &items::info[item.data()];

    float velx = (rand() % 200 - 100) / 2000.0f;
    float vely = (rand() % 100) / 600.0f;
    if (vel.x != 0) {
        velx = vel.x;
    }
    if (vel.y != 0) {
        vely = vel.y;
    }

    if (info->entity != "") {
        int ent = enemies::spawnEnemy(info->entity, pos, true, { velx, vely });
        ECS::queueComponent<droppedItemC>(ent, { item.data(), amount, collectable ? 0 : 60 });
        return true;
    }


    if (textures::nametocoords.count(info->tex) <= 0) return false;
    glm::vec4 sC;
    sC = textures::nametocoords.at(info->tex)->coords;
    glm::vec2 siz;
    float mod;
    siz.x = (sC.b - sC.r);
    siz.y = (sC.a - sC.g);
    if (siz.x > siz.y) {
        mod = siz.x / (8.0f / globals::spritesheetWH.x);
    }
    else {
        mod = siz.y / (8.0f / globals::spritesheetWH.y);
    }


    auto ppos = std::make_shared<glm::vec2>(pos);

    drawC dc;
    dc.position = ppos;
    dc.tex = info->tex;
    dc.mat = glm::mat4(1.0f);
    dc.size = { mod,mod };
    dc.hidden = false;
    dc.parent = globals::itemLayer;
    if (info->anim != "") animations::watchAnim(info->anim, &dc.anim);


    int tmp = ECS::newEntity();
    ECS::queueComponent<physicsC>(tmp, { ppos,  { velx, vely }, {mod,mod} });
    drawSystem::addComponent(tmp, &dc);
    ECS::queueComponent<droppedItemC>(tmp, { item.data(), amount, collectable ? 0 : 60 });
    return true;
}
