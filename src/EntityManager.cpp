#include "./EntityManager.h"
#include "./Collision.h"
#include "./Components/ColliderComponent.h"

void EntityManager::ClearData() {
    for (auto& entity : entities) {
        entity->Destroy();
    }
}

bool EntityManager::HasNoEntities() {
    return entities.size() == 0;
}

void EntityManager::Update(const float& deltaTime) {

    auto itrEntity = entities.begin();
    
    while(itrEntity != entities.end()) {
        if (!(*itrEntity)->IsActive()) {
            itrEntity = entities.erase(itrEntity);
        }
        else {
            (*itrEntity)->Update(deltaTime);
            itrEntity++;  
        }
    } 
}

void EntityManager::Render() {
    // for (auto& entity : entities) {
    //     entity->Render();
    // }    
    for(int layerNumber = 0; layerNumber < NUM_LAYERS; layerNumber++) {
        for(auto& entity : GetEntitiesByLayer(static_cast<LayerType>(layerNumber))) {
            entity->Render();
        }
    }
}

Entity& EntityManager::AddEntity(const std::string& entityName, LayerType layer) {
    Entity* entity = new Entity(*this, entityName, layer);
    entities.emplace_back(entity);
    return *entity; // dereference the pointer since we return a reference not a pointer
}

std::vector<Entity*> EntityManager::GetEntities() const {
    return entities;
}

unsigned int EntityManager::GetEntityCount() {
    return entities.size();
}

void EntityManager::ListAllEntities() const {
    unsigned int i = 0;
    for (auto& entity : entities) {
        std::cout << "Entity[" << i << "]:" << entity->name << std::endl;
        entity->ListAllComponents();
        i++;
    }
}

std::vector<Entity*> EntityManager::GetEntitiesByLayer(LayerType layer) const {
    std::vector<Entity*> selectedEntities;
    for (auto& entity : entities) {
        if (entity->layer == layer) {
            selectedEntities.emplace_back(entity);
        }
    }
    return selectedEntities;    
}

// checks for all collisions among all entities
CollisionType EntityManager::CheckCollisions() const {
    for (int i = 0; i < entities.size()-1; i++) {
        auto& thisEntity = entities.at(i);
        if (thisEntity->HasComponent<ColliderComponent>()) {
            ColliderComponent* thisCollider = thisEntity->GetComponent<ColliderComponent>();
            for (int j = i+1; j < entities.size(); j++) {
                auto& thatEntity = entities.at(j);
                if(thisEntity->name.compare(thatEntity->name) !=0 && thatEntity->HasComponent<ColliderComponent>()) {
                    ColliderComponent* thatCollider = thatEntity->GetComponent<ColliderComponent>();
                    if(Collision::CheckRectangleCollision(thisCollider->collider, thatCollider->collider)) {
                        if(thisCollider->colliderTag.compare("PLAYER") == 0 && thatCollider->colliderTag.compare("ENEMY") == 0) {
                            return PLAYER_ENEMY_COLLISION;
                        }
                        if(thisCollider->colliderTag.compare("PLAYER") == 0 && thatCollider->colliderTag.compare("PROJECTILE") == 0) {
                            return PLAYER_PROJECTILE_COLLISION;
                        }
                        if(thisCollider->colliderTag.compare("ENEMY") == 0 && thatCollider->colliderTag.compare("FRIENDLY_PROJECTILE") == 0) {
                            return ENEMY_PROJECTILE_COLLISION;
                        }                        
                        if(thisCollider->colliderTag.compare("PLAYER") == 0 && thatCollider->colliderTag.compare("LEVEL_COMPLETE") == 0) {
                            return PLAYER_LEVEL_COMPLETE_COLLISION;
                        }                        
                    }
                }
            }
        }
    }
    return NO_COLLISION;
}