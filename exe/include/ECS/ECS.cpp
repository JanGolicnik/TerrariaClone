#include "ECS/ECS.h"

namespace ECS {
    std::vector<int> idsToServe;
    std::unordered_map<const char*, const char*> compToSystem;
    std::unordered_map<const char*, std::shared_ptr<SystemBase>> systems;
    std::unordered_map<int, Entity> entities;
    std::vector<int> entitiesForDeletion;

    void Update()
    {
        for (auto& sys : systems) {
            sys.second.get()->Update();
        }
    }

    void deleteEntity(int entityid)
    {
        if (entities.count(entityid) >= 1) {
            for (auto& a : entities.at(entityid).components) {
                systems.at(a.first).get()->deleteComponent(entityid);
            }
            entities.erase(entityid);
            idsToServe.push_back(entityid);
        }
    }

    bool entityExists(int entityid)
    {
        if (entities.count(entityid) >= 1) {
            return true;
        }
        return false;
    }

    void commitQueues()
    {
        for (auto& sys : systems) {
            sys.second.get()->commitQueued();
        }

        for (int i = 0; i < entitiesForDeletion.size(); i++) {
            ECS::deleteEntity(entitiesForDeletion[i]);
        }
        entitiesForDeletion.clear();
    }

    void queueDeletion(int entityid)
    {
        bool found = false;
        for (int i = 0; i < entitiesForDeletion.size(); i++) {
            if (entitiesForDeletion[i] == entityid) {
                found = true;
                break;
            }
        }
        if (!found) {
            entitiesForDeletion.push_back(entityid);
        }
    }

    void print()
    {
        for (auto& sys : systems) {
            sys.second->print();
        }
    }

    void clean()
    {
        empty();
        for (auto& sys : systems) {
            sys.second.get()->empty();
            sys.second.get()->clean();
        }
    }

    void empty()
    {
        for (auto& ent : entities) {
            queueDeletion(ent.first);
        }
        commitQueues();
        for (auto& system : systems) {
            system.second->empty();
        }
    }

    int newEntity()
    {
        static int id = 0;
        int tmpid = 0;

        if (idsToServe.size() > 0) {
            tmpid = idsToServe[idsToServe.size() - 1];
            idsToServe.pop_back();
        }
        else {
            tmpid = id;
            id++;
        }

        entities.insert(std::pair<int, Entity>(tmpid, {}));
        return tmpid;
    }

}
