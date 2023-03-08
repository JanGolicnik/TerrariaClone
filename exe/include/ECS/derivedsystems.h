#pragma once

#include <ECS/systems.h>
#include <core.h>

struct firstComponent {
    float value;
};

struct secondComponent {
    const char* text;
};

struct thirdComponent{
    const char* text;
};

class firstSystem : public SystemBase {
public:
    void Update();

    firstSystem() {
        componentArray = std::make_shared<ComponentArray<firstComponent>>();
    }
};

class secondSystem : public SystemBase {
public:
    void Update();

    secondSystem() {
        componentArray = std::make_shared<ComponentArray<secondComponent>>();
    }
};

class thirdSystem : public SystemBase {
public:
    void Update();

    thirdSystem() {
        componentArray = std::make_shared<ComponentArray<thirdComponent>>();
    }
};

