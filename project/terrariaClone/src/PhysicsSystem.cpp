#include <PhysicsSystem.h>

#include <blocks.h>
#include <utils.h>
#include <Layers.h>
#include <liquids.h>
#include <particles.h>

void physicsSystem::Update()
{
    auto arr = static_cast<ComponentArray<physicsC>*>(componentArray.get());
    for (auto& i : arr->components) {
        auto b = &Layers::blocks;
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
                            sounds::watersplash();
                        }
                        else if (i.isinliquid == 1) {
                            particles::spawnEffect("lavasplash", *i.position - i.size / glm::vec2(2));
                            sounds::lavasplash();
                        }
                    }
                    if (level > 8)viscosity = 1.8;
                }
            }

            i.vel.y -= globals::gravity * (i.weight / viscosity);
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
                        if (!i.ignorebot)
                            if (vec.y < 0) {
                                c = { cent.x - i.size.x * 0.5f + 0.025f, cent.y - i.size.y / 2.0f + vec.y };
                                float ratio = 1.0f / points.x;
                                for (int j = 0; j < points.x + 1; j++) {
                                    auto botinfo = Layers::queryBlockInfo(b, c);
                                    if (!(i.wantstoskip && botinfo->skippable))
                                        if (botinfo->collidableTop) {
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

            *(i.position) += i.vel / viscosity;
        }
    }
}
