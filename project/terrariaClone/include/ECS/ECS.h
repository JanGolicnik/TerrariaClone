#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <assert.h>

#include <ECS\systems.h>

namespace ECS {
    struct Entity {
        std::unordered_map<const char*, int> components;
    };

    extern std::vector<int> idsToServe;
    extern std::unordered_map<const char*, const char*> compToSystem;
    extern std::unordered_map<const char*, std::shared_ptr<SystemBase>> systems;
    extern std::unordered_map<int, Entity> entities;
    extern std::vector<int> entitiesForDeletion;

    int newEntity();
    void Update();
    void deleteEntity(int entityid);
    bool entityExists(int entityid);
    void commitQueues();
    void queueDeletion(int entityid);
    void print();
    void clean();
    void empty();

    template<typename comp>
    void removeComponent(int entityid) {
        assert(compToSystem.count(typeid(comp).name()) > 0 && "system not registerd");
        const char* sys = compToSystem[typeid(comp).name()];
        if (entities.count(entityid) >= 1) {
            systems.at(sys).get()->removeComponent<comp>(entityid);
            entities.at(entityid).components.erase(sys);
        }
    }

    template<typename comp>
    void QremovingComponent(int entityid) {
        assert(compToSystem.count(typeid(comp).name()) > 0 && "system not registerd");
        const char* sys = compToSystem[typeid(comp).name()];
        if (entities.count(entityid) >= 1) {
            entities.at(entityid).components.erase(sys);
        }
        systems.at(sys)->QremovingComponent<comp>(entityid);
    }

    template<typename comp>
    void addComponent(int entityid, comp component) {
        assert(compToSystem.count(typeid(comp).name()) > 0 && "system not registerd");
        const char* sys = compToSystem[typeid(comp).name()];
        if (entities.count(entityid) >= 1) {
            int c = systems.at(sys).get()->addComponent(entityid, component);
            if (c == -1) {
                return;
            }
            else {
                entities.at(entityid).components.insert(std::pair<const char*, int>(sys, c));
            }
        }
    }

    template<typename comp>
    void queueComponent(int entityid, comp component) {
        assert(compToSystem.count(typeid(comp).name()) > 0 && "system not registerd");
        const char* sys = compToSystem[typeid(comp).name()];
        if (entities.count(entityid) >= 1) {
            systems.at(sys).get()->queueComponent(entityid, component);
        }
    }

    template<typename SYS, typename COMP>
    SYS* registerSystem() {
        systems.insert(std::pair<const char*, std::shared_ptr<SystemBase>>(typeid(SYS).name(), std::make_shared<SYS>()));
        compToSystem.insert(std::pair<const char*, const char*>(typeid(COMP).name(), typeid(SYS).name()));
        systems.at(typeid(SYS).name())->onRegister();
        return static_cast<SYS*>(systems.at(typeid(SYS).name()).get());
    }
    template<typename T>
    T* getSystem() {
        auto t = static_cast<T*>(systems.at(typeid(T).name()).get());
        return t;
    }
    template<typename T>
    void deleteSystem() {
        systems.erase(typeid(T).name());
    }
    template<typename T>
    T* getComponent(int entityid) {
        const char* sys = compToSystem.at(typeid(T).name());
        return systems.at(sys).get()->getComponent<T>(entityid);
    }
    template<typename T>
    T* getComponent(int entityid, SystemBase* sys) {
        return sys->getComponent<T>(entityid);
    }
};


