#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include <map>
#include <iostream>

#include "./Constants.h"
#include "./EntityManager.h"
#include "./Component.h"

class EntityManager;
class Component;

class Entity {
    
    private:
        EntityManager& manager; // when the reference is initialized in member initialization list, its OK.
                                // otherwise declaration of reference without initialization is invalid.
                                // NOTE: it can be initialized with another reference or with an actual value
        bool isActive;
        std::vector<Component*> components;
        std::map<const std::type_info*, Component*> componentTypeMap;
    
    public:
        std::string name;
        LayerType layer;

        Entity(EntityManager& _manager);
        Entity(EntityManager& _manager, std::string _name, LayerType _layer);
        
        void Update(const float& deltaTime);
        void Render();
        void Destroy();
        bool IsActive() const;

        /////////////////////////////////////////
        template <typename T,  typename... TArgs>
        T& AddComponent(TArgs&&... args) {
            T* newComponent(new T(std::forward<TArgs>(args)...)); // we forward the variable number of arguments from 
                                                                  // template to the newComponent constructor

            newComponent->owner = this; // this Entity that we are inside owns the added component
            components.emplace_back(newComponent);

            componentTypeMap[&typeid(*newComponent)] = newComponent; // popoulate a map of [type of Component, pointer to Component]

            newComponent->Initialize();

            return *newComponent;
        }
        
        template <typename T>
        T* GetComponent() {
            return static_cast<T*>(componentTypeMap[&typeid(T)]);
        }
        
        template <typename T>
        bool HasComponent() const {
            return componentTypeMap.count(&typeid(T));
        }


        void ListAllComponents() const;
};

#endif