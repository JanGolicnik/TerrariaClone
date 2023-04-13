#include <mobSystem.h>

#include <layers.h>
#include <utils.h>
#include <player.h>
#include <buffs.h>
#include <Window.h>
#include <gameLoop.h>
#include <ui.h>
#include <game.h>
#include <particles.h>
#include <enemies.h>

void mobSystem::Update()
{
    auto arr = static_cast<ComponentArray<mobC>*>(componentArray.get());

    auto blocks = &Layers::blocks;
    for (int i = 0; i < arr->components.size(); i++) {
        auto p = &arr->components[i];
        p->iframes--;

        auto phys1 = ECS::getComponent<physicsC>(arr->componentToEntity[i]);

        if (p->onCollision.size() > 0) {
            for (int sec = 0; sec < arr->components.size(); sec++) {
                if (sec == i) continue;
                auto p2 = &arr->components[sec];
                if (p2->hitboxradius == 0) continue;
                for (auto& oc : p->onCollision) {
                    if (p->hp <= 0) break;
                    if (p2->families.count(oc.first) <= 0) continue;

                    auto phys2 = ECS::getComponent<physicsC>(arr->componentToEntity[sec]);
                    if (utils::distPointLine(*phys2->position, phys1->prevpos, *phys1->position) < (p2->hitboxradius + p->hitboxradius)) {
                        oc.second(p, p2, phys1, phys2, i, sec, arr);
                    }
                }
            }
        }

        //onscreen
        if (phys1->position->x > Player::pos.x - Layers::blocksOnScreen.x/2 &&
            phys1->position->x < Player::pos.x + Layers::blocksOnScreen.x/2 &&
            phys1->position->y > Player::pos.y - Layers::blocksOnScreen.y/2 &&
            phys1->position->y < Player::pos.y + Layers::blocksOnScreen.y/2) {

            if (p->light.r > 0 || p->light.g > 0 || p->light.b > 0)
                Layers::addLight(*(phys1->position), p->light);

            if (p->destroydecor) Layers::damageBlock(blocks, round(*phys1->position), 1, ceil(p->hitboxradius / 2));

            if (p->onPlayerCollision) {
                if (glm::distance(*phys1->position, Player::pos) < (p->hitboxradius + Player::width) / 2.0f) {
                    p->onPlayerCollision(p, phys1, i, arr);
                }
            }

            if (p->displayName != "") {
                glm::vec2 mc = Window::mouseBlockCoords(false);
                if (glm::distance(mc, *phys1->position) < p->hitboxradius) {
                    std::string text = p->displayName + " (" + std::to_string(p->hp) + "/" + std::to_string(p->maxhp) + ")";
                    UI::setTStat(ECS::getComponent<uiC>(gameLoop::zoomedcursoritem), "text", text);
                }
            }
        }

        for (int buff = 0; buff < p->buffs.size(); buff++) {
            buffs::buffs[p->buffs[buff].name].func(&p->buffs[buff], p, phys1);
            p->buffs[buff].countdown--;
            if (p->buffs[buff].countdown == 0) {
                p->buffs.erase(p->buffs.begin() + buff);
                buff--;
            }
        }

        if (p->damageInLava) {
            if (phys1->isinliquid == 1) {
                if (p->iframes < 0) {
                    p->hp -= 50;
                    p->iframes = globals::iframes;
                    p->buffs.push_back({ "onfire", 180, globals::time });
                }
            }
        }

        float distfromplayer = glm::distance(*phys1->position, Player::pos);
        if (distfromplayer > 450) {
            p->despawntimer = 0;
        }
        else if (distfromplayer > 100 + p->hitboxradius) {
            p->despawntimer--;
        }
        if (!p->candespawn) p->despawntimer = -1;
        if (p->despawntimer == 0) {
            ECS::queueDeletion(arr->componentToEntity[i]);
        }
        p->killin--;
        if (p->killin == 0 || p->hp <= 0) {
            for (auto& drop : p->drops) {
                game::droppedItemSys->dropItem(*phys1->position, drop.first, drop.second, true);
            }
            if (p->onDeath) {
                p->onDeath(p, phys1, arr->componentToEntity[i]);
            }
            if (p->gore != "") {
                particles::dropGore(*phys1->position, p->gore);
            }
            if (p->hpbar != -1) {
                UI::deleteElement(p->hpbar);
            }
            enemies::currslots -= p->slots;
            ECS::queueDeletion(arr->componentToEntity[i]);
        }
    }
}