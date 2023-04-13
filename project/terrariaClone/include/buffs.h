#pragma once

#include <PhysicsSystem.h>
#include <mobSystem.h>

namespace buffs
{
    struct Buff {
        std::function<void(ActiveBuff*, mobC*, physicsC*)> func;
        bool endable;
        std::string tex;
        std::string description;
    };

    extern std::unordered_map<std::string, Buff> buffs;

    void addBuff(std::string name, std::string tex, std::function<void(ActiveBuff* buff, mobC*, physicsC*)> func, bool endable = false, std::string description = "");

    void createUI();

    void onfirebuff(ActiveBuff* buff, mobC* p, physicsC* phys);
    void happybuff(ActiveBuff* buff, mobC* p, physicsC* phys);
    void cozyfirebuff(ActiveBuff* buff, mobC* p, physicsC* phys);
    void nothing(ActiveBuff* buff, mobC* p, physicsC* phys);
    void chilled(ActiveBuff* buff, mobC* p, physicsC* phys);
    void poisoned(ActiveBuff* buff, mobC* p, physicsC* phys);
};

