#include <toolSystem.h>

#include <PhysicsSystem.h>
#include <drawSystem.h>
#include <player.h>
#include <Window.h>
#include <utils.h>
#include <particles.h>

void toolSystem::Update()
{
    auto arr = static_cast<ComponentArray<toolC>*>(componentArray.get());
    for (int i = 0; i < arr->components.size(); i++) {
        if (i == 0) {
            auto draw = ECS::getComponent<drawC>(arr->componentToEntity[i]);
            auto phys = ECS::getComponent<physicsC>(arr->componentToEntity[i]);
            auto p = &arr->components[i];
            if (p->frames == 0) {
                ECS::queueDeletion(arr->componentToEntity[i]);
            }
            else {
                draw->hidden = false;
                draw->hasmat = false;
                switch (p->animType) {
                case ia_DEFAULT: {
                    draw->flipX = false;
                    if (Player::dir < 0) {
                        draw->flipX = true;
                    }
                    float percentlifespan = ((p->maxframes - p->frames) / p->maxframes);
                    float rotangle = 105;
                    rotangle -= percentlifespan * 150;
                    *(phys->position) = Player::pos + glm::vec2(phys->size.x / 3 * Player::dir, phys->size.y / 2);
                    *phys->position += glm::vec2(cos(rotangle * PI / 180) * Player::dir * 0.9f, sin(rotangle * PI / 180) * 0.9f);
                    draw->hasmat = true;
                    float angle = Player::dir * (-135.0f * percentlifespan + 90);
                    draw->mat = glm::translate(glm::mat4(1.0f), glm::vec3(Player::dir * -phys->size.x / 2, -phys->size.y / 2, 0));
                    draw->mat = glm::rotate(draw->mat, float(angle * PI / 180.0f), glm::vec3(0, 0, 1));
                    draw->mat = glm::translate(draw->mat, glm::vec3(Player::dir * phys->size.x / 2, phys->size.y / 2, 0));
                }
                               break;
                case ia_STAB:
                    //*(phys->position) = Player::pos + glm::vec2(Player::width / 3 * Player::dir, Player::height / 3);
                    //phys->position->x += (1 * ((p->maxframes - p->frames) / p->maxframes)) * Player::dir;
                    break;
                case ia_USE: {
                    float offset = 0;
                    draw->hasmat = true;

                    *(phys->position) = Player::pos;
                    glm::vec2 vec = Window::mouseBlockCoords(false) - (*phys->position);
                    float angle = int(utils::angleOfVector(vec) - 180) % 360;
                    phys->position->x += cos(angle * PI / 180.0f) * 2;
                    phys->position->y += sin(angle * PI / 180.0f) * 2;

                    if (angle > 90 || angle < -90) {
                        draw->flipY = true;
                    }
                    else {
                        draw->flipY = false;
                    }

                    draw->mat = glm::rotate(glm::mat4(1.0f), float(angle * PI / 180.0f), glm::vec3(0, 0, 1));

                }
                           break;
                case ia_NONE:
                    draw->hidden = true;
                    break;

                }
                p->frames--;
            }
        }
        else {
            ECS::queueDeletion(arr->componentToEntity[i]);
        }
    }
}

void toolSystem::makeTool(int ent, glm::vec2 pos, std::string item, float usespeed, glm::mat4 mat, float damagemult)
{
    toolC tmp;
    itemInfo* info = items::getInfo(item);
    tmp.animType = info->animType;
    tmp.frames = usespeed;
    tmp.maxframes = usespeed;
    tmp.neki = false;



    auto spos = std::make_shared<glm::vec2>(pos);
    glm::vec2 size = glm::vec2(1.3) * glm::vec2(info->sizeMod);

    drawC dc;
    dc.position = spos;
    dc.tex = info->tex;
    dc.mat = glm::mat4(1.0f);
    dc.size = size;
    dc.hidden = true;
    dc.parent = globals::toolLayer;

    drawSystem::addComponent(ent, &dc);
    ECS::addComponent<physicsC>(ent, { spos, {}, size, true, false, false, false, false });
    ECS::addComponent<toolC>(ent, tmp);

    float damage = items::getStat(item, "damage", { .valueFloat = 0 }).valueFloat;
    if (damage != 0) {
        mobC mc;
        mc.damage = damage * damagemult;
        mc.families = { mf_WEAPON };
        mc.knockback = items::getStat(item, "knockback").valueFloat;
        mc.hp = 10;
        mc.onCollision = { {mf_ENEMY, info->onhit}, {mf_CRITTER, collisionFs::damage} };
        mc.destroydecor = true;
        mc.hitboxradius = 1.3 * info->sizeMod;
        ECS::addComponent<mobC>(ent, mc);
    }

    if (info->emmiter.rate != 0) {
        particles::emitParticle(glm::vec2(0, 0), &info->emmiter, ent);
    }

}

