#include "derivedsystems.h"
#include <ECS/ECS.h>
#include <core.h>

#if 1
#define COUTSYSTEM(name) std::cout << "updating " << name << " system\n";
#else
#define COUTSYSTEM(name) 
#endif


void firstSystem::Update()
{
    COUTSYSTEM("first")
    auto arr = static_cast<ComponentArray<firstComponent>*>(componentArray.get());
    for (int i = 0; i < arr->components.size(); i++) {
        std::cout << arr->components[i].get()->value << "\n";
    }
}

void secondSystem::Update()
{
    COUTSYSTEM("second")
    auto arr = static_cast<ComponentArray<secondComponent>*>(componentArray.get());
    for (int i = 0; i < arr->components.size(); i++) {
        std::cout << arr->components[i].get()->text << "\n";
    }
}

void thirdSystem::Update()
{
    COUTSYSTEM("third")
    auto arr = static_cast<ComponentArray<secondComponent>*>(componentArray.get());
    for (int i = 0; i < arr->components.size(); i++) {
        auto second = ECS::getComponent<secondComponent>(arr->componentToEntity[i]);
        std::cout << second->text << " " <<arr->components[i].get()->text << "\n";
    }
}
