#pragma once

#include <memory>
#include <ECS\components.h>
class SystemBase {
public:
    std::shared_ptr<ComponentArrayBase> componentArray;

    virtual void Update() = 0;
    virtual void onRegister() = 0;
    virtual void clean();
    virtual void deleteComponent(int entity);
    virtual void empty();
    void commitQueued();
    void print();

    template<typename T>
    void removeComponent(int entity) {
        auto a = static_cast<ComponentArray<T>*>(componentArray.get());
        a->removeComponent(entity);
    }

    template<typename T>
    void QremovingComponent(int entity) {
        auto a = static_cast<ComponentArray<T>*>(componentArray.get());
        a->QremovingComponent(entity);
    }

    template<typename T>
    void queueComponent(int entity, T component) {
        auto a = static_cast<ComponentArray<T>*>(componentArray.get());
        a->queueComponent(entity, component);
    }

    template<typename T>
    int addComponent(int entity, T component) {
        auto a = static_cast<ComponentArray<T>*>(componentArray.get());
        return a->addComponent(entity, component);
    }
    template<typename T>
    T* getComponent(int entity) {
        auto a = static_cast<ComponentArray<T>*>(componentArray.get());
        return a->getComponent(entity);
    };

    SystemBase() {
    }
    ~SystemBase() {};
};
