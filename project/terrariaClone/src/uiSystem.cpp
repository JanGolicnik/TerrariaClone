#include <uiSystem.h>

#include <ui.h>
#include <window.h>
#include <camera.h>
#include <layers.h>
#include <input.h>

void uiSystem::onRegister()
{
    body = ECS::newEntity();
    uiC c;
    c.func = ui_BODY;
    c.pos = { 0,0 };
    c.size = { 0,0 };
    c.hidden = true;
    c.parent = -1;
    c.onclick = UI::UI_nothing;
    c.onhover = UI::UI_nothing;
    c.onrender = UI::UI_nothing;
    c.onnothover = UI::UI_nothing;
    c.anchor = anchorMID;
    ECS::addComponent<uiC>(body, c);
}

void uiSystem::updateChildren(uiC* parent, ComponentArray<uiC>* arr, glm::vec2 mc) {

    for (int i = 0; i < parent->children.size(); i++) {
        auto p = &arr->components[arr->entityToComponent[parent->children[i]]];
        auto draw = ECS::getComponent<drawC>(parent->children[i]);

        draw->hidden = p->hidden;

        if (p->anchor != anchorNONE) {
            *draw->position = p->pos / Window::res;
            *draw->position *= ((glm::vec2)Layers::trueBsOnScr / glm::vec2(2));
            *draw->position -= camera::pos;

            UI::applyAnchor(draw->position.get(), p->anchor);
            draw->size = (p->size / Window::res) / globals::fullScale;
        }
        else {
            *draw->position = p->pos;
            draw->size = p->size;
        }

        p->onrender(p, draw, parent->children.at(i), arr);

        if (!p->interactable || p->hidden || p->removed) continue;

        if (!(mc.x > draw->position->x - draw->size.x / 2.0f &&
            mc.x < draw->position->x + draw->size.x / 2.0f &&
            mc.y > draw->position->y - draw->size.y / 2.0f &&
            mc.y < draw->position->y + draw->size.y / 2.0f)) {

            p->onnothover(p, draw, parent->children.at(i), arr);
            p->hovering = false;
            continue;
        }

        if (input::held(k_PRIMARY) && p->holdingtime > 0) {
            if (p->onhold) {
                p->onhold(p, draw, parent->children.at(i), arr);
            }
            p->holdingtime++;
        }
        else if (input::held(k_SECONDARY) && p->holdingtime > 0) {
            if (p->onrighthold) {
                p->onrighthold(p, draw, parent->children.at(i), arr);
            }
            p->holdingtime++;
        }
        else {
            p->holdingtime = -1;
        }

        if (mouseClicked) {
            p->holdingtime = 1;
            if (p->onclick) {
                p->onclick(p, draw, parent->children.at(i), arr);
                if (!p->propagateClick) {
                    mouseClicked = false;
                }
            }
        }
        else if (mouseRightClicked) {
            p->holdingtime = 1;
            p->onrightclick(p, draw, parent->children.at(i), arr);
            if (!p->propagateClick) {
                mouseRightClicked = false;
            }
        }
        else if (p->hovering) {
                p->onhover(p, draw, parent->children.at(i), arr);
        }
        else if (p->onenter)
            p->onenter(p, draw, parent->children.at(i), arr);
            p->hovering = true;
    }

    for (int j = 0; j < parent->children.size(); j++) {
        auto pc = &arr->components[arr->entityToComponent[parent->children.at(j)]];
        if (pc->children.size() > 0) {
            updateChildren(pc, arr, mc);
        }
    }
}

void uiSystem::Update()
{
    auto arr = static_cast<ComponentArray<uiC>*>(componentArray.get());

    auto bodyC = ECS::getComponent<uiC>(body);
    glm::vec2 mc = Window::mouseBlockCoordsGlobal(false);
    updateChildren(bodyC, arr, mc);

    mouseClicked = false;
    mouseRightClicked = false;
}
