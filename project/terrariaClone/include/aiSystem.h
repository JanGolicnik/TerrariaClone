#pragma once

#include <ECS/ECS.h>
#include <core.h>
#include <PhysicsSystem.h>

#define adargs aiC*, int, physicsC*, ComponentArray<aiC>*, aiAction*, int 
struct aiC;

struct aiAction {
    int timer;
    std::function<void(adargs)> func;
    int repeat;
    int lastUpdate = 0;
};

union aiStat {
    int intVal;
    float floatVal;
    bool boolVal = 0;
    int* intValp;
    float* floatValp;
    bool* boolValp;
    void* voidp;
    glm::vec2 vec2Val;
    glm::vec3 vec3Val;
    const char* charp;
    glm::vec2* vec2Valp;
    glm::vec3* vec3Valp;
};

struct aiC {
    AI ai;
    std::vector<aiAction> actions;
    std::function<void(adargs)> onupdate;
    std::function<void(adargs)> onclick;
    std::unordered_map<std::string, aiStat> stats;
    int state = 0;
};
#undef adargs

class aiSystem : public SystemBase {
public:
    void Update();
    void onRegister() {};
    aiStat* getStat(aiC* p, std::string statName, aiStat def = { .floatVal = 0 });
    aiStat* getStatFast(aiC* p, std::string statName);
    aiSystem() {
        componentArray = std::make_shared<ComponentArray<aiC>>();
    }
};