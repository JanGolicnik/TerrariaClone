#include <drawSystem.h>

#include <core.h>
#include <text.h>
#include <textures.h>
#include <layers.h>
#include <camera.h>
#include <utils.h>
#include <Window.h>
#include <animations.h>

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


    uint32_t* indices = new uint32_t[bsize * 6];

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
    auto blocks = &Layers::blocks;

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

void drawSystem::UpdateBehindBackground()
{
    memset(vertices, 0, bsize * 4 * sizeof(SpriteVertex));
    auto main = ECS::getComponent<drawC>(behindBackground);
    auto arr = static_cast<ComponentArray<drawC>*>(componentArray.get());
    auto blocks = &Layers::blocks;

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
    auto blocks = &Layers::blocks;

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
    auto blocks = &Layers::blocks;

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

    if (dc->position == nullptr) {
        dc->position = std::make_shared<glm::vec2>(glm::vec2(0));
    }
    if (force)
        ECS::addComponent<drawC>(entity, *dc);
    else
        ECS::queueComponent<drawC>(entity, *dc);
}

void drawSystem::deleteComponent(int entity)
{
    auto p = ECS::getComponent<drawC>(entity);
    if (p == nullptr) return;
    for (auto i : p->children) {
        ECS::queueDeletion(i);
    }
    if (ECS::entityExists(p->parent)) {
        auto par = ECS::getComponent<drawC>(p->parent);
        if (par != nullptr)
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
    auto blocks = &Layers::blocks;
    for (int i = 0; i < parent->children.size(); i++) {
        auto p = &arr->components[arr->entityToComponent[parent->children[i]]];
        if (!p->hidden) {
            glm::vec3 light = p->color;
            if (p->autolight) {
                //light += Layers::getLight(round(* p->position), {0,0,0});
            }
            glm::vec4 color(light.r, light.g, light.b, p->opacity);
            if (p->spriteCoords != glm::vec4(-1, -1, -1, -1)) {
                memcpy(vertices + (renderi % bsize) * 4, utils::CreateSpriteRect(glm::vec3(p->position->x, p->position->y, 0), p->spriteCoords, p->size, color, p->flipX, p->flipY, p->autocorrect).data(), 4 * sizeof(SpriteVertex));
            }
            else
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
                        chi += counter - 1;
                        scale = val;
                    }
                    else
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

                glm::vec2 size = glm::vec2(g->width / Window::res.x, g->height / Window::res.y);
                size /= globals::fullScale;
                size *= scale;
                float xpos = cursorpos.x + (g->xoffset * scale / (float)globals::blocksizepx);
                float ypos = cursorpos.y + ((text::base - g->yoffset) * scale / Window::res.y) / globals::fullScale.y - size.y;

                memcpy(vertices + (renderi % bsize) * 4, utils::CreateFontRect(
                    glm::vec3(xpos, ypos, 1),
                    sc,
                    size,
                    color,
                    false, true
                ).data(), 4 * sizeof(SpriteVertex));

                cursorpos.x += ((g->xadvance / Window::res.x) * scale) / globals::fullScale.x;

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