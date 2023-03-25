#pragma once

#include <vector>
#include <unordered_map>
#include <iostream>
#include <assert.h>
#include <set>

namespace ECS {
    template<typename comp>
    void addComponent(int entityid, comp c);
}

class ComponentArrayBase {
public:
    virtual void EntityDestroyed(int entity) = 0;
    virtual void commitQueued() = 0;
    virtual void print() = 0;
    virtual void empty() = 0;
};


template<typename T>
class ComponentArray : public ComponentArrayBase {
public:
    std::vector<T> components;
    std::unordered_map<int, int> entityToComponent;
    std::unordered_map<int, int> componentToEntity;
    std::vector<T> Q;
    std::unordered_map<int, int> CtEQ;
    std::unordered_map<int, int> EtCQ;
    std::set<int> componentsForRemoval; //entitiji

    void empty() {
        components.clear();
        entityToComponent.clear();
        componentToEntity.clear();
        Q.clear();
        CtEQ.clear();
        EtCQ.clear();
    }

    void print() {
        std::cout << "-------------------------" << typeid(T).name() << "-------------------------\n";
        std::cout << components.size() << "\n";
        std::cout << "-------------------------" << typeid(T).name() << "-------------------------\n";
        /*if (componentToEntity.size() < 1) {
            return;
        }
        std::cout << "-------------------------" << typeid(T).name() << "-------------------------\n";
        for (auto i : componentToEntity) {
            std::cout << "component " << i.first << " is on eneitty " << i.second << "\n";
        }
        std::cout << "-------------------------" << typeid(T).name() << "-------------------------\n";*/
    }

    void removeComponent(int entity) {
        EntityDestroyed(entity);
    }

    void QremovingComponent(int entity) {
        componentsForRemoval.insert(entity);
    }

    int addComponent(int entityid, T component) {
        if (entityToComponent.count(entityid) >= 1) {
            return -1;
        }
        components.push_back(component);
        entityToComponent.insert(std::pair<int, int>(entityid, components.size() - 1));
        componentToEntity.insert(std::pair<int, int>(components.size() - 1, entityid));
        return components.size() - 1;
    }

    void queueComponent(int entityid, T component) {
        if (entityToComponent.count(entityid) >= 1) {
            return;
        }
        Q.push_back(component);
        CtEQ.insert(std::make_pair(Q.size() - 1, entityid));
        EtCQ.insert(std::make_pair(entityid, Q.size() - 1));
    }

    void commitQueued() {
        for (auto ent : componentsForRemoval) {
            removeComponent(ent);
        }
        componentsForRemoval.clear();

        for (int i = 0; i < Q.size(); i++) {
            ECS::template addComponent<T>(CtEQ.at(i), Q[i]);
        }
        CtEQ.clear();
        EtCQ.clear();
        Q.clear();
    }

    void EntityDestroyed(int entity) override {
        if (entityToComponent.count(entity) <= 0) return;

        int lastId = components.size() - 1;
        int delId = entityToComponent[entity];

        components[delId] = components[lastId];
        entityToComponent[componentToEntity[lastId]] = delId;
        entityToComponent.erase(componentToEntity[delId]);
        componentToEntity[delId] = componentToEntity[lastId];
        components.pop_back();
        componentToEntity.erase(lastId);
    };

    T* getComponent(int entity) {

        if (entityToComponent.count(entity) >= 1) {
            return &components[entityToComponent[entity]];
        }
        
        if (EtCQ.count(entity) >= 1) {
            return &Q[EtCQ[entity]];
        }
     
        return nullptr;
    }
};