#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "./Entity.h"
#include "./Component.h"

#include <vector>
#include <iostream>

class EntityManager {
    private:
        std::vector<Entity*> entities;
    public:
        void Update(const float& deltaTime);
        void Render();
        bool HasNoEntities();
        void ClearData();

        Entity& AddEntity(const std::string& entityName, LayerType layer);
        std::vector<Entity*> GetEntities() const;
        std::vector<Entity*> GetEntitiesByLayer(LayerType layer) const;
        unsigned int GetEntityCount();
        void ListAllEntities() const;
        CollisionType CheckCollisions() const;
};

#endif