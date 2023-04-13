#pragma once

#include <PhysicsSystem.h>
#include <ECS/ECS.h>
#include <core.h>
#include <sounds.h>

enum mobFam {
    mf_ENEMY,
    mf_NEUTRAL,
    mf_CRITTER,
    mf_PLAYERPROJECTILE,
    mf_WEAPON,
    mf_SUMMON
};

struct ActiveBuff {
    std::string name;
    int countdown;
    unsigned long long starttime;
};

#define mobCargs mobC* p1, mobC* p2, physicsC* phys1, physicsC* phys2, int p1_i, int p2_i, ComponentArray<mobC>* arr
#define mobCWPargs mobC* p, physicsC* phys, int i, ComponentArray<mobC>* arr
#define mobDeathargs mobC* p, physicsC* phys, int ent
struct mobC {
    int hp = 1;
    int maxhp;
    int damage;
    std::set<mobFam> families;
    std::unordered_map<mobFam, std::function<void(mobCargs)> > onCollision;
    std::function<void(mobCWPargs)> onPlayerCollision;
    std::function<void(mobDeathargs)> onDeath;
    int iframes = 0;
    float knockback = 0;
    float kbResist = 1;
    glm::vec3 light = glm::vec3(0);
    int killin = -1;
    int hpbar = -1;
    int defense = 0;
    bool destroydecor = false;
    std::string displayName = "";
    std::unordered_map<std::string, int> drops;
    std::vector<int>children;
    int despawntimer = -1;
    bool candespawn = true;
    float hitboxradius = 0;
    std::vector<ActiveBuff> buffs;
    int directDamageTo = -1;
    bool damageInLava = true;
    void(*damageSound)() = sounds::normalhit;
    void(*deathSound)() = sounds::normaldie;
    std::string gore = "";
    int slots = 1;
};


class mobSystem : public SystemBase {
public:
    void Update();
    void onRegister() {};
    mobSystem() {
        componentArray = std::make_shared<ComponentArray<mobC>>();
    }
};