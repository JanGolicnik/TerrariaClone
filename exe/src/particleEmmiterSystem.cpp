#include <particleEmmiterSystem.h>

#include <globals.h>
#include <drawSystem.h>
#include <aiSystem.h>
#include <utils.h>
#include <particles.h>
#include <enemyFunctions.h>

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
            if (globals::particles) spawn(p, arr, i, phys);
            p->countdown += p->rate + randrate + 1 / (glm::length(phys->vel)) * p->speedmultiplier;

        }
    }
}

void particleEmmiterSystem::spawn(particleEmmiterC* p, ComponentArray<particleEmmiterC>* arr, int i, physicsC* phys)
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
            dc.mat = glm::rotate(glm::mat4(1.0f), float(a * PI / 180.0f), glm::vec3(0, 0, 1));
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