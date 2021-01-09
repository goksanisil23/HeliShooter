#ifndef KEYBOARDCONTROLCOMPONENT_H
#define KEYBOARDCONTROLCOMPONENT_H

#include "../Game.h"
#include "../EntityManager.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/ColliderComponent.h"

class KeyboardControlComponent: public Component {
    public:
        std::string upKey;
        std::string downKey;
        std::string rightKey;
        std::string leftKey;
        std::string shootKey;
        std::string showColliderKey;

        TransformComponent* transform;
        SpriteComponent* sprite;
        ColliderComponent* collider;

        KeyboardControlComponent() {}

        KeyboardControlComponent(std::string upKey, std::string downKey, std::string rightKey, std::string leftKey, 
                                 std::string shootKey, std::string showColliderKey) {
            this->upKey = GetSDLKeyStringCode(upKey);
            this->downKey = GetSDLKeyStringCode(downKey);
            this->rightKey = GetSDLKeyStringCode(rightKey);
            this->leftKey = GetSDLKeyStringCode(leftKey);
            this->shootKey = GetSDLKeyStringCode(shootKey);
            this->showColliderKey = GetSDLKeyStringCode(showColliderKey);
        }

        std::string GetSDLKeyStringCode(std::string key) {
            if(key.compare("up")==0)
                return "1073741906";
            if(key.compare("down")==0)
                return "1073741905";
            if(key.compare("left")==0)
                return "1073741904";
            if(key.compare("right")==0)
                return "1073741903";     
            if(key.compare("space")==0)
                return "32";
            if(key.compare("showCollider")==0)
                return "99"; // "c" key

            return std::to_string(static_cast<int>(key[0]));
        }

        void Initialize() override {
            transform = owner->GetComponent<TransformComponent>();
            sprite = owner->GetComponent<SpriteComponent>();
            collider = owner->GetComponent<ColliderComponent>(); // make sure ColliderComponent is added before KeyboardControlComponent
        }

        void Update(const float& deltaTime) override {
            if (Game::event.type == SDL_KEYDOWN) {
                std::string keyCode = std::to_string(Game::event.key.keysym.sym);

                if(keyCode.compare(this->upKey) == 0) {
                    transform->velocity.y = -50;
                    transform->velocity.x = 0;
                    sprite->Play("UpAnimation");
                }
                if(keyCode.compare(this->rightKey) == 0) {
                    transform->velocity.y = 0;
                    transform->velocity.x = 50;
                    sprite->Play("RightAnimation");
                }
                if(keyCode.compare(this->downKey) == 0) {
                    transform->velocity.y = 50;
                    transform->velocity.x = 0;
                    sprite->Play("DownAnimation");
                }    
                if(keyCode.compare(this->leftKey) == 0) {
                    transform->velocity.y = 0;
                    transform->velocity.x = -50;
                    sprite->Play("LeftAnimation");
                }                 
                if(keyCode.compare(this->shootKey) == 0) {
                    // TODO: shoot projectile
                }
                if(keyCode.compare(this->showColliderKey) == 0) {
                    collider->showColliderBoundary = !collider->showColliderBoundary; // toggle the value
                }                                                                               
            }

            if (Game::event.type == SDL_KEYUP) {
                std::string keyCode = std::to_string(Game::event.key.keysym.sym);

                if(keyCode.compare(this->upKey) == 0) {
                    transform->velocity.y = 0;
                }
                if(keyCode.compare(this->rightKey) == 0) {
                    transform->velocity.x = 0; 
                }
                if(keyCode.compare(this->downKey) == 0) {
                    transform->velocity.y = 0; 
                }
                if(keyCode.compare(this->leftKey) == 0) {
                    transform->velocity.x = 0;
                }                                                               
            }
        }
};

#endif