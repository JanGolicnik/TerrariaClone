#include "componentsystems.h"
#include <glm/gtx/rotate_vector.hpp>
#include <fstream>
#include <ECS/ECS.h>

#include <buffs.h>
#include <core.h>
#include <utils.h>
#include <blocks.h>
#include <globals.h>
#include <player.h>
#include <ui.h>
#include <animations.h>
#include <enemyFunctions.h>
#include <textures.h>
#include <items.h>
#include <camera.h>
#include <memory>
#include <text.h>
#include <gameLoop.h>
#include <particles.h>
#include <layers.h>
#include <game.h>
#include <input.h>
#include <liquids.h>

void physicsSystem::Update()
{
    auto arr = static_cast<ComponentArray<physicsC>*>(componentArray.get());
    for (auto& i : arr->components) {
        auto b = Layers::getLayer("blocks");
        glm::vec2 c;

        i.prevpos = *i.position;

        i.collidebot = false;
        i.collidetop = false;
        i.collideright = false;
        i.collideleft = false;

        float viscosity = 1;
        if (!i.isstatic) {
            if (i.affectedByLiquid) {
                int lastinliquid = i.isinliquid;
                bool inliquid = false;
                int level = 0;
                for (int x = 0; x < i.size.x; x++) {
                    if (level > 8) break;
                    for (int y = 0; y < i.size.y; y++) {
                        glm::vec2 pos = *i.position - i.size / glm::vec2(2);
                        pos += glm::vec2(x, y);
                        pos = round(pos);
                        int data = liquids::at(pos);
                        if (data != 0) {
                            inliquid = true;
                            if (i.isinliquid == -1) {
                                i.isinliquid = (data & 0x00ff0000u) >> 16;
                            }
                            level += data & 0x0000000fu;
                            if (level > 8) break;
                        }
                    }
                }
                if (!inliquid) {
                    i.isinliquid = -1;
                }
                else {
                    if (lastinliquid != i.isinliquid) {
                        if (i.isinliquid == 0) {
                            particles::spawnEffect("watersplash", *i.position - i.size / glm::vec2(2));
                        }else if (i.isinliquid == 0) {
                            particles::spawnEffect("lavasplash", *i.position - i.size / glm::vec2(2));
                        }
                    }
                    if (level > 8)viscosity = 1.8;
                }
            }
            i.vel.y -= globals::gravity * (i.weight/ viscosity);
        }
        int botblock = globals::emptyid;
        glm::vec2 nBs = ceil(abs(i.vel));
        glm::vec2 vec = glm::vec2(0);
        if (nBs.x != 0) vec.x = i.vel.x / nBs.x;
        if (nBs.y != 0) vec.y = i.vel.y / nBs.y;
        glm::vec2 cent = *i.position;
        glm::vec2 points = glm::vec2(ceil(i.size.x), ceil(i.size.y));
        if (!isnan(vec.x) && !isnan(vec.y) && i.size.x && i.size.y) {
            if (!i.dontcollide) {
                while (nBs != glm::vec2(0, 0)) {
                    glm::vec2 c;

                    if ((i.collidetop || i.collidebot) && (i.collideleft || i.collideright)) break;

                    if (!(i.collidetop || i.collidebot)) {
                        //bottom
                        if(!i.ignorebot)
                        if (vec.y < 0) {
                            c = { cent.x - i.size.x * 0.5f + 0.025f, cent.y - i.size.y / 2.0f + vec.y};
                            float ratio = 1.0f / points.x;
                            for (int j = 0; j < points.x + 1; j++) {
                                if (Layers::queryBlockInfo(b, c)->collidableTop) {
                                    botblock = Layers::queryBlock(b, c)->id;
                                    if (i.stoponcollision) i.position->y = round(c.y) + i.size.y / 2.0f + 0.5f;
                                    vec.y = 0;
                                    nBs.y = 0;
                                    i.collidebot = true;
                                }
                                c.x += i.size.x * 0.98f * ratio;
                            }
                        }
                        //top
                        if (!i.ignoretop)
                            if (vec.y > 0) {
                            c = { cent.x - i.size.x * 0.5f,  cent.y + i.size.y / 2.0f + vec.y };
                            float ratio = 1 / points.x;
                            for (int j = 0; j < points.x + 1; j++) {
                                if (Layers::queryBlockInfo(b, c)->collidableBot) {
                                    if (i.stoponcollision) i.position->y = round(c.y) - i.size.y / 2.0f - 0.5f;
                                    nBs.y = 0;
                                    vec.y = 0;
                                    i.collidetop = true;
                                }
                                c.x += i.size.x * 0.9 * ratio;
                            }
                        }
                    }
                    if (!(i.collideleft || i.collideright)) {
                        //left
                        if (!i.ignoreleft)
                            if (vec.x < 0) {
                            c = { cent.x - i.size.x / 2 + vec.x, cent.y - i.size.y * 0.5f };
                            float ratio = 1 / points.y;
                            for (int j = 0; j < points.y + 1; j++) {
                                if (Layers::queryBlockInfo(b, c)->collidableRight) {
                                    if (i.stoponcollision) i.position->x = round(c.x) + 0.5f + i.size.x / 2.0f;
                                    nBs.x = 0;
                                    vec.x = 0;
                                    i.collideleft = true;
                                }
                                c.y += i.size.y * 0.9 * ratio;
                            }
                        }

                        //right
                        if (!i.ignoreright)
                            if (vec.x > 0) {
                            c = { cent.x + i.size.x / 2 + vec.x, cent.y - i.size.y * 0.5f };
                            float ratio = 1 / points.y;
                            for (int j = 0; j < points.y + 1; j++) {
                                if (Layers::queryBlockInfo(b, c)->collidableLeft) {
                                    if (i.stoponcollision) i.position->x = round(c.x) - 0.5f - i.size.x / 2.0f;
                                    nBs.x = 0;
                                    vec.x = 0;
                                    i.collideright = true;
                                }
                                c.y += i.size.y * 0.9 * ratio;
                            }
                        }
                    }
                    if (nBs.y > 0) nBs.y--;
                    if (nBs.x > 0) nBs.x--;
                    if (!i.collidetop && !i.collidebot) { cent.y += vec.y; }
                    if (!i.collideright && !i.collideleft) { cent.x += vec.x; }
                }
            }
        }

        if (i.stoponcollision) {
            if (i.collidetop && i.vel.y > 0) { i.vel.y = 0; }
            if (i.collidebot && i.vel.y < 0) { i.vel.y = 0; }
            if (i.collideleft && i.vel.x < 0) { i.vel.x = 0; }
            if (i.collideright && i.vel.x > 0) { i.vel.x = 0; }
            if (i.friction) {
                float friction = blocks::idToInfo[botblock].friction;
                i.vel.x += utils::approach(i.vel.x, 0, 6 * friction);
            }
        }

        if (!i.isstatic) {
            if (abs(i.vel.x) < 0.000001) {
                i.vel.x = 0;
            }
            if (abs(i.vel.y) < 0.000001) {
                i.vel.y = 0;
            }
            if (i.bounce) {
                bool bouncex = false, bouncey = false;
                if (i.collidebot && i.vel.y < 0) {
                    bouncey = true;
                }
                if (i.collideleft && i.vel.x < 0) {
                    i.position->x -= i.vel.x;
                    bouncex = true;
                }
                if (i.collideright && i.vel.x > 0) {
                    i.position->x -= i.vel.x;
                    bouncex = true;
                }
                if (i.collidetop && i.vel.y > 0) {
                    bouncey = true;
                }

                if (bouncex) {
                    i.vel.x = -i.vel.x * i.bounciness;
                }
                if (bouncey) {
                    i.vel.y = -i.vel.y * i.bounciness;
                }
            }

            *(i.position) += i.vel/ viscosity;
        }
    }
}

void drawSystem::onRegister()
{
    shaderID = globals::spriteShaderID;
    glCreateVertexArrays(1, &VA);
    glBindVertexArray(VA);

    glCreateBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteVertex) * bsize * 4, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (const void*)offsetof(SpriteVertex, Position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (const void*)offsetof(SpriteVertex, SpriteCoords));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (const void*)offsetof(SpriteVertex, color));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (const void*)offsetof(SpriteVertex, spritesheet));


    uint32_t* indices = new uint32_t[bsize*6];

    int offset = 0;
    for (int i = 0; i < bsize * 6; i += 6) {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;

        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;

        offset += 4;
    }

    glCreateBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * bsize * 6, indices, GL_STATIC_DRAW);

    delete[] indices;

    vertices = new SpriteVertex[bsize * 4];
    memset(vertices, 0, bsize * 4 * sizeof(SpriteVertex));
}

void drawSystem::Update()
{
    memset(vertices, 0, bsize * 4 * sizeof(SpriteVertex));
    auto main = ECS::getComponent<drawC>(mainDrawable);
    auto arr = static_cast<ComponentArray<drawC>*>(componentArray.get());
    auto blocks = Layers::getLayer("blocks");

    renderi = 0;

    glUseProgram(shaderID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures::spriteSheet);
    glUniform1i(1, 0);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, text::texture);
    glUniform1i(2, 1);
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(camera::trans));
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBindVertexArray(VA);

    updateChildren(main, arr);
    
    glBufferSubData(GL_ARRAY_BUFFER, 0, (sizeof(SpriteVertex)* bsize * 4), vertices);
    glDrawElements(GL_TRIANGLES, bsize * 6, GL_UNSIGNED_INT, nullptr);
}

void drawSystem::UpdateBehindBackground()
{
    memset(vertices, 0, bsize * 4 * sizeof(SpriteVertex));
    auto main = ECS::getComponent<drawC>(behindBackground);
    auto arr = static_cast<ComponentArray<drawC>*>(componentArray.get());
    auto blocks = Layers::getLayer("blocks");

    renderi = 0;

    glUseProgram(shaderID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures::spriteSheet);
    glUniform1i(1, 0);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, text::texture);
    glUniform1i(2, 1);
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(camera::trans));
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBindVertexArray(VA);

    updateChildren(main, arr);

    glBufferSubData(GL_ARRAY_BUFFER, 0, (sizeof(SpriteVertex) * bsize * 4), vertices);
    glDrawElements(GL_TRIANGLES, bsize * 6, GL_UNSIGNED_INT, nullptr);
}

void drawSystem::UpdateBehindBlocks()
{
    memset(vertices, 0, bsize * 4 * sizeof(SpriteVertex));
    auto main = ECS::getComponent<drawC>(behindBlocks);
    auto arr = static_cast<ComponentArray<drawC>*>(componentArray.get());
    auto blocks = Layers::getLayer("blocks");

    renderi = 0;

    glUseProgram(shaderID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures::spriteSheet);
    glUniform1i(1, 0);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, text::texture);
    glUniform1i(2, 1);
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(camera::trans));
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBindVertexArray(VA);

    updateChildren(main, arr);

    glBufferSubData(GL_ARRAY_BUFFER, 0, (sizeof(SpriteVertex) * bsize * 4), vertices);
    glDrawElements(GL_TRIANGLES, bsize * 6, GL_UNSIGNED_INT, nullptr);
}

void drawSystem::UpdateFront()
{
    memset(vertices, 0, bsize * 4 * sizeof(SpriteVertex));
    auto main = ECS::getComponent<drawC>(front);
    auto arr = static_cast<ComponentArray<drawC>*>(componentArray.get());
    auto blocks = Layers::getLayer("blocks");

    renderi = 0;

    glUseProgram(shaderID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures::spriteSheet);
    glUniform1i(1, 0);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, text::texture);
    glUniform1i(2, 1);
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(camera::trans));
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBindVertexArray(VA);

    updateChildren(main, arr);

    glBufferSubData(GL_ARRAY_BUFFER, 0, (sizeof(SpriteVertex) * bsize * 4), vertices);
    glDrawElements(GL_TRIANGLES, bsize * 6, GL_UNSIGNED_INT, nullptr);
}

void drawSystem::addComponent(int entity, drawC* dc, bool force)
{
    if (dc->parent != -1) {
        ECS::getComponent<drawC>(dc->parent)->children.push_back(entity);
    }
    else {
        dc->parent = mainDrawable;
        ECS::getComponent<drawC>(dc->parent)->children.push_back(entity);
    }

    if (force)
        ECS::addComponent<drawC>(entity, *dc);
    else
        ECS::queueComponent<drawC>(entity, *dc);
}

void drawSystem::deleteComponent(int entity)
{
    auto p = ECS::getComponent<drawC>(entity);
    for (auto i : p->children) {
        ECS::queueDeletion(i);
    }
    if (ECS::entityExists(p->parent)) {
        auto par = ECS::getComponent<drawC>(p->parent);
        for (int i = 0; i < par->children.size(); i++) {
            if (par->children[i] == entity) {
                par->children.erase(par->children.begin() + i);
                break;
            }
        }
    }
    componentArray.get()->EntityDestroyed(entity);
}

void drawSystem::updateChildren(drawC* parent, ComponentArray<drawC>* arr)
{
    auto blocks = Layers::getLayer("blocks");
    for (int i = 0; i < parent->children.size(); i++) {
        auto p = &arr->components[arr->entityToComponent[parent->children[i]]];
        if (!p->hidden) {
            glm::vec3 light = p->color;
            if (p->autolight) {
                if (blocks != nullptr) {
                    light *= Layers::queryBlock(blocks, *p->position)->light;
                }
                else {
                    light *= globals::dayclr;
                }
            }
            glm::vec4 color(light.r, light.g, light.b, p->opacity);
            if (p->spriteCoords != glm::vec4(-1, -1, -1, -1)) {
                memcpy(vertices + (renderi % bsize) * 4, utils::CreateSpriteRect(glm::vec3(p->position->x, p->position->y, 0), p->spriteCoords, p->size, color, p->flipX, p->flipY, p->autocorrect).data(), 4 * sizeof(SpriteVertex));
            }else
            if (p->anim > 0) {
                memcpy(vertices + (renderi % bsize) * 4, utils::CreateSpriteRect(glm::vec3(p->position->x, p->position->y, 0), animations::getFrame(p->anim), p->size, color, p->flipX, p->flipY, p->autocorrect).data(), 4 * sizeof(SpriteVertex));
            }
            else {
                memcpy(vertices + (renderi % bsize) * 4, utils::CreateSpriteRect(glm::vec3(p->position->x, p->position->y, 0), p->tex, p->size, color, p->flipX, p->flipY, p->autocorrect).data(), 4 * sizeof(SpriteVertex));
            }
            if (p->hasmat) {
                utils::updateSprite(vertices + (renderi % bsize) * 4, &p->mat, 4, {});
            }

            if ((renderi + 1) % bsize == 0) {
                
                glBufferSubData(GL_ARRAY_BUFFER, 0, (sizeof(SpriteVertex) * bsize * 4), vertices);
                glDrawElements(GL_TRIANGLES, bsize * 6, GL_UNSIGNED_INT, nullptr);
                memset(vertices, 0, bsize * 4 * sizeof(SpriteVertex));
                renderi = 0;
            }
            renderi++;

            glm::vec2 cursorpos = *p->position + p->textoffset;
            float scale = p->textScale;
            float maxscale = scale;
            for (int chi = 0; chi < p->text.size(); chi++) {
                char ch = p->text[chi];
                if (ch == '\\') {
                    if (p->text[chi + 1] == 's') {
                        std::string tmp;
                        int counter = 2;
                        char c = p->text[chi + counter];
                        while (c != '\n') {
                            if (c != '1' && c != '2' && c != '3' && c != '4' && c != '5' && c != '6' && c != '7' && c != '8' && c != '9' && c != '.') break;
                            c = p->text[chi + counter];
                            tmp += c;
                            counter++;
                        }
                        float val = std::stof(tmp);
                        chi += counter-1;
                        scale = val;
                    }else
                    if (p->text[chi + 1] == 'c') {
                        std::string tmp;
                        tmp += p->text[chi + 2];
                        tmp += p->text[chi + 3];
                        tmp += p->text[chi + 4];
                        int val = std::stoi(tmp);
                        color.r = val / 255.0f;
                        tmp.clear();
                        tmp += p->text[chi + 5];
                        tmp += p->text[chi + 6];
                        tmp += p->text[chi + 7];
                        val = std::stoi(tmp);
                        color.g = val / 255.0f;
                        tmp.clear();
                        tmp += p->text[chi + 8];
                        tmp += p->text[chi + 9];
                        tmp += p->text[chi + 10];
                        val = std::stoi(tmp);
                        color.b = val / 255.0f;
                        tmp.clear();
                        chi += 11;
                    }
                    if (chi < p->text.size()) {
                        ch = p->text[chi];
                    }
                    else {
                        ch = ' ';
                    }
                }
                if (ch == '\n') {
                    cursorpos.x = p->position->x;
                    cursorpos.y -= (text::lineHeight / (float)globals::blocksizepx) * maxscale;
                    continue;
                }
                if (scale > maxscale) maxscale = scale;
                const glyph* g = &text::glyphs[ch];

                glm::vec4 sc = {
                    g->x,
                    text::scaleH - g->y,
                    g->x + g->width,
                    text::scaleW - (g->y + g->height)
                };

                sc /= glm::vec4(text::scaleW, text::scaleH, text::scaleW, text::scaleH);

                //https://www.angelcode.com/products/bmfont/doc/render_text.html

                glm::vec2 size = glm::vec2(g->width / (float)globals::resX, g->height / (float)globals::resY);
                size /= globals::fullScale;
                size *= scale;
                float xpos = cursorpos.x + (g->xoffset * scale / (float)globals::blocksizepx);
                float ypos = cursorpos.y + ((text::base - g->yoffset) * scale / (float)globals::resY) / globals::fullScale.y - size.y;

                memcpy(vertices + (renderi % bsize) * 4, utils::CreateFontRect(
                    glm::vec3(xpos, ypos, 1),
                    sc,
                    size,
                    color,
                    false, true
                ).data(), 4 * sizeof(SpriteVertex));

                cursorpos.x += ((g->xadvance / (float)globals::resX) * scale) / globals::fullScale.x;

                if ((renderi + 1) % bsize == 0) {
                    glBufferSubData(GL_ARRAY_BUFFER, 0, (sizeof(SpriteVertex) * bsize * 4), vertices);
                    glDrawElements(GL_TRIANGLES, bsize * 6, GL_UNSIGNED_INT, nullptr);
                    memset(vertices, 0, bsize * 4 * sizeof(SpriteVertex));
                    renderi = 0;
                }
                renderi++;
            }

        }
        updateChildren(p, arr);
    }
}

void drawSystem::clean()
{
    delete[] vertices;
    glDeleteBuffers(1, &VA);
    glDeleteBuffers(1, &VB);
    glDeleteBuffers(1, &IB);
    glDeleteShader(shaderID);
}

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
                glm::vec2 mc = globals::mouseBlockCoordsZoomed(false);
                if (phys->position->x - phys->size.x / 2.0f < mc.x &&
                    phys->position->x + phys->size.x / 2.0f > mc.x &&
                    phys->position->y + phys->size.y / 2.0f > mc.y &&
                    phys->position->y - phys->size.y / 2.0f < mc.y) {
                    p->onclick(p, arr->componentToEntity[i], phys, arr, nullptr, updated);
                }
            }
        }
            

        if(p->onupdate)
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
                    float rotangle = 120;
                    rotangle -= percentlifespan * 150;
                    *(phys->position) = Player::pos + glm::vec2(phys->size.x / 3 * Player::dir, phys->size.y / 2);
                    *phys->position += glm::vec2(cos(rotangle * PI / 180) * Player::dir, sin(rotangle * PI / 180));
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
                    glm::vec2 vec = globals::mouseBlockCoordsZoomed(false) - (*phys->position);
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

    mobC mc;
    mc.damage = items::getStat(item, "damage").valueFloat * damagemult;
    mc.families = { mf_WEAPON };
    mc.knockback = items::getStat(item, "knockback").valueFloat;
    mc.hp = 10;
    mc.onCollision = { {mf_ENEMY, info->onhit}, {mf_CRITTER, collisionFs::damage} };

    auto spos = std::make_shared<glm::vec2>(pos);
    glm::vec2 size = glm::vec2(1.3) * glm::vec2(info->sizeMod);
    mc.hitboxradius = 1.3 * info->sizeMod;

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
    ECS::addComponent<mobC>(ent, mc);
    if (info->emmiter.rate != 0) {
        particles::emitParticle(glm::vec2(0, 0), &info->emmiter, ent);
    }

}

void droppedItemSystem::Update()
{
    glm::vec2 mc = globals::mouseBlockCoordsZoomed(false);
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

        if (p->timer > 0) continue;
        if (!Player::dead) {
            if (!UI::hotbar->hasRoomFor(p->name) && !UI::inventory->hasRoomFor(p->name)) continue;
            glm::vec2 mid = glm::vec2(phys->position->x + 0.5f, phys->position->y + 0.5f);
            float dist = glm::distance(playerpos, *phys->position);
            if (dist < 1.2f) {
                int prevAmount = p->amount;
                p->amount = Player::pickUp(p->name, p->amount);
            }
            if (dist < Player::pickupRange) {
                phys->dontcollide = true;
                phys->isstatic = false;
                phys->vel += utils::approach(phys->vel, glm::normalize(playerpos - *phys->position) * glm::vec2(1.0f), 1);
                phys->vel = glm::clamp(phys->vel, -1.0f, 1.0f);
            }
            else {
                phys->dontcollide = false;
            }
        }
        if (p->amount == 0) {
            ECS::queueDeletion(arr->componentToEntity[i]);
        }
    }
}

bool droppedItemSystem::dropItem(glm::vec2 pos, std::string_view item, int amount, bool collectable, glm::vec2 vel)
{
    if (item != "empty") {
        if (items::info.count(item.data()) <= 0) return false;
        auto info = &items::info[item.data()];
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

        float velx = (rand() % 200 - 100) / 2000.0f;
        float vely = (rand() % 100) / 600.0f;
        if (vel.x != 0) {
            velx = vel.x;
        }
        if (vel.y != 0) {
            vely = vel.y;
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
        ECS::queueComponent<physicsC>(tmp, { ppos,  { velx, vely }, {1,1}, false });
        drawSystem::addComponent(tmp, &dc);
        ECS::queueComponent<droppedItemC>(tmp, { item.data(), amount, collectable ? 0 : 60 });
        return true;
    }
    return false;
}

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
            *draw->position = p->pos / glm::vec2(globals::resX, globals::resY);
            *draw->position *= ((glm::vec2)Layers::trueBsOnScr / glm::vec2(2));
            *draw->position -= camera::pos;

            UI::applyAnchor(draw->position.get(), p->anchor);
            draw->size = (p->size / glm::vec2(globals::resX, globals::resY)) / globals::fullScale;
        }
        else {
            *draw->position = p->pos;
            draw->size = p->size;
        }

        p->onrender(p, draw, parent->children.at(i), arr);

        if (p->interactable && !p->hidden && !p->removed) {
            if (mc.x > draw->position->x - draw->size.x / 2 && mc.x <  draw->position->x + draw->size.x / 2 && mc.y >  draw->position->y - draw->size.y / 2 && mc.y < draw->position->y + draw->size.y / 2) {
                if (input::held(k_PRIMARY) && p->holding) {
                    if (p->onhold) {
                        p->onhold(p, draw, parent->children.at(i), arr);
                    }
                }
                else {
                    p->holding = false;
                }

                if (mouseClicked) {
                    p->holding = true;
                    p->onclick(p, draw, parent->children.at(i), arr);
                    if (!p->propagateClick) {
                        mouseClicked = false;
                    }
                }
                else if (mouseRightClicked) {
                    p->onrightclick(p, draw, parent->children.at(i), arr);
                    if (!p->propagateClick) {
                        mouseRightClicked = false;
                    }
                }
                else {
                    p->onhover(p, draw, parent->children.at(i), arr);
                    p->hovering = true;
                }
            }
            else {
                p->onnothover(p, draw, parent->children.at(i), arr);
                p->hovering = false;
            }
        }
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
    glm::vec2 mc = globals::mouseBlockCoords(false);
    updateChildren(bodyC, arr, mc);
    
    mouseClicked = false;
    mouseRightClicked = false;
}

void mobSystem::Update()
{
    auto arr = static_cast<ComponentArray<mobC>*>(componentArray.get());
    
    auto blocks = Layers::getLayer("blocks");
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
                    if (utils::distPointLine(*phys2->position, phys1->prevpos, *phys1->position) < (p2->hitboxradius + p->hitboxradius) / 2) {
                        oc.second(p, p2, phys1, phys2, i, sec, arr);
                    }
                }
            }
        }

        //onscreen
        if (phys1->position->x > Player::pos.x - Layers::trueBsOnScr.x &&
            phys1->position->x < Player::pos.x + Layers::trueBsOnScr.x &&
            phys1->position->y > Player::pos.y - Layers::trueBsOnScr.y &&
            phys1->position->y < Player::pos.y + Layers::trueBsOnScr.y) {
           
            if (p->light.r > 0 || p->light.g > 0 || p->light.b > 0)
                Layers::addLight(*(phys1->position), p->light);

            for (int buff = 0; buff < p->buffs.size(); buff++) {
                buffs::buffs[p->buffs[buff].name].func(&p->buffs[buff], p, phys1);
                p->buffs[buff].countdown--;
                if (p->buffs[buff].countdown == 0) {
                    p->buffs.erase(p->buffs.begin() + buff);
                    buff--;
                }
            }

            if (p->destroydecor) Layers::damageBlock(blocks, round(*phys1->position), 1, ceil(p->hitboxradius / 2));
        
            if (p->onPlayerCollision) {
                if (glm::distance(*phys1->position, Player::pos) < (p->hitboxradius + Player::width) / 2.0f) {
                    p->onPlayerCollision(p, phys1, i, arr);
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

            if (p->displayName != "") {
                glm::vec2 mc = globals::mouseBlockCoordsZoomed(false);
                if (glm::distance(mc, *phys1->position) < p->hitboxradius) {
                    std::string text = p->displayName + " (" + std::to_string(p->hp) + "/" + std::to_string(p->maxhp) + ")";
                    UI::setTStat(ECS::getComponent<uiC>(gameLoop::zoomedcursoritem), "text", text);
                }
            }
        }

        float distfromplayer = glm::distance(*phys1->position, Player::pos);
        if (distfromplayer > 450) {
            p->despawntimer = 0;
        }
        else if(distfromplayer > 100 + p->hitboxradius) {
            p->despawntimer--;
        }
        if (!p->candespawn)p->despawntimer = -1;
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
            if (p->hpbar != -1) {
                UI::deleteElement(p->hpbar);
            }
            ECS::queueDeletion(arr->componentToEntity[i]);
        }
    }
}

void particleEmmiterSystem::Update()
{
    auto arr = static_cast<ComponentArray<particleEmmiterC>*>(componentArray.get());

    for (int i = 0; i < arr->components.size(); i++) {
        auto p = &arr->components[i];
        
        if (p->setup) {
            p->countdown = p->rate + 1;
            p->setup = false;
        }

        if (p->lifespan == 0 || p->rate == 0) {
            if (p->killentity) {
                ECS::queueDeletion(arr->componentToEntity[i]);
            }
            else {
                ECS::QremovingComponent<particleEmmiterC>(arr->componentToEntity[i]);
            }
        }
        p->lifespan--;
        auto phys = ECS::getComponent<physicsC>(arr->componentToEntity[i]);
        float randrate = ((rand() % 1000) / 500.0f - 1) * p->randomizerate;
        if (p->rate + randrate <= 0) continue;
        p->countdown--;
        while (p->countdown <= 1) {
            if(globals::particles) spawn(p, arr, i, phys);
            p->countdown += p->rate + randrate + 1/(glm::length(phys->vel)) * p->speedmultiplier;

        }
    }
}

void particleEmmiterSystem::spawn(particleEmmiterC* p, ComponentArray<particleEmmiterC> *arr, int i, physicsC *phys)
{
    int actualamount = ((rand() % 1000) / 500.0f - 1) * p->randomizeamount;
    for (int j = 0; j < p->amount; j++) {
        float randangle = ((rand() % 1000) / 500.0f - 1) * p->randangle;
        float randms = ((rand() % 1000) / 1000.0f) * p->randomizems;
        float randlifespan = ((rand() % 1000) / 500.0f - 1) * p->randomizelifespan;
        float randscale = 1 + ((rand() % 1000) / 500.0f - 1) * p->randomizescale;
        float randrot = ((rand() % 1000) / 500.0f - 1) * p->randrotation;
        if (randscale < 0) randscale = 0;
        glm::vec2 dirN = glm::normalize(p->dir);
        float angleRad = randangle * PI / 180;
        glm::vec2 vel;
        vel.x = cos(angleRad) * dirN.x - sin(angleRad) * dirN.y;
        vel.y = sin(angleRad) * dirN.x + cos(angleRad) * dirN.y;
        vel *= (p->ms + randms);
        glm::vec2 velOverTime = p->velovertime;


        int particle = ECS::newEntity();
        physicsC pc;
        pc.weight = p->weight;
        pc.vel = vel;
        glm::vec2 posdif = *phys->position - phys->prevpos;
        if (posdif.x != 0 && posdif.y != 0) {
            posdif = glm::normalize(posdif) * glm::vec2(p->countdown);
        }
        glm::vec2 pos = *phys->position + posdif;
        if (p->radius != 0) {
            float randradius = ((rand() % 1000) / 500.0f - 1) * p->randradius;
            int val = rand();
            glm::vec2 radius = glm::vec2(cos(val), sin(val));
            radius *= p->radius + randradius;
            pos += radius;
        }
        pc.position = std::make_shared<glm::vec2>(pos);
        pc.size = { 1,1 };
        pc.size *= randscale;
        pc.dontcollide = !p->stoponcollision;
        pc.friction = false;
        pc.affectedByLiquid = false;

        drawC dc;
        dc.position = pc.position;
        if (p->tex.substr(0, 9) == "particle:") {
            dc.spriteCoords = particles::get(p->tex.substr(9));
        }
        dc.tex = p->tex;
        dc.size = p->size;
        if (p->parent == -1) {
            dc.parent = globals::particleLayer;
        }
        else {
            dc.parent = p->parent;
        }
        dc.hidden = false;
        dc.anim = -1;
        dc.opacity = p->opacity;

        if (p->matchparticledir) {
            dc.hasmat = true;
            int a = int(utils::angleOfVector(dirN)) % 360;
            dc.mat = glm::rotate(glm::mat4(1.0f),float( a * PI / 180.0f), glm::vec3(0, 0, 1));
        }
        else {
            dc.mat = glm::mat4(1.0f);
        }

        aiC ac;
        ac.ai = ai_PARTICLE;
        ac.onupdate = mobFunctions::particleOnUpdate;
        aiStat rot; rot.floatVal = p->rotation + randrot;
        aiStat size; size.floatVal = p->sizeovertime;
        if (p->smallerAsDie) {
            size.floatVal = (dc.size.x / p->particleLifespan);
        }
        float opacity = p->opacityovertime;
        if (p->disappearAsDie == true) {
            opacity = (-p->opacity / p->particleLifespan);
        }
        if (p->stopAsDie) {
            velOverTime = -vel / glm::vec2(p->particleLifespan);
        }

        dc.color = p->color;

        ac.stats["rotation"] = rot;
        ac.stats["size"] = size;
        ac.stats["opacity"] = { .floatVal = opacity };
        ac.stats["velovertime"] = { .vec2Val = velOverTime };
        ac.stats["velovertime"] = { .vec2Val = velOverTime };
        ac.stats["killin"] = { .intVal = p->particleLifespan };
        ac.stats["light"] = { .vec3Val = p->light };

        ECS::queueComponent<aiC>(particle, ac);
        drawSystem::addComponent(particle, &dc);
        ECS::queueComponent<physicsC>(particle, pc);
    }
}
