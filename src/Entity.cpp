#include "./Entity.h"

Entity::Entity(EntityManager& _manager): manager(_manager) {
    this->isActive = true;
}

Entity::Entity(EntityManager& _manager, std::string _name, LayerType _layer): manager(_manager), name(_name), layer(_layer) {
    this->isActive = true;
}

void Entity::Update(const float& deltaTime) {
    for (auto& component: components) {
        component->Update(deltaTime);
    }
}

void Entity::Render() {
    for (auto& component: components) {
        component->Render();
    }
}

void Entity::Destroy() {
    this->isActive = false;
}

bool Entity::IsActive() const{
    return this->isActive;
}

void Entity::ListAllComponents() const {
    for (auto& mapElement : componentTypeMap) {
        std::cout << "  Component<" << mapElement.first->name() << ">" << std::endl;
    }
}
