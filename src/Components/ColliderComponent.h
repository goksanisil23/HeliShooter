#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <SDL2/SDL.h>

#include "../Game.h"
#include "../AssetManager.h"
#include "../EntityManager.h"
#include "./TransformComponent.h"

class ColliderComponent : public Component {
    public: 
        std::string colliderTag;
        SDL_Rect collider;
        SDL_Texture* texture;
        SDL_Rect srcRectangle;
        SDL_Rect dstRectangle;
        TransformComponent* transform;    

        static bool showColliderBoundary; // only 1 instance is wanted to toggle all at once    

        ColliderComponent(std::string colliderTag, int x, int y, int width, int height, std::string assetTextureId) {
            this->colliderTag = colliderTag;
            this->collider = {x, y, width, height};
            
            // texture representing the boundaries of the collider
            this->texture = Game::assetManager->GetTexture(assetTextureId);
        }

        void Initialize() override {
            if( owner->HasComponent<TransformComponent>()) {
                transform = owner->GetComponent<TransformComponent>();
                srcRectangle = {0, 0, transform->width, transform->height}; // loading the entire texture initially
                dstRectangle = {collider.x, collider.y, collider.w, collider.h};
            }
        }

        void Update(const float& deltaTime) override {
            collider.x = static_cast<int>(transform->position.x);
            collider.y = static_cast<int>(transform->position.y);
            collider.w = transform->width * transform->scale;
            collider.h = transform->height * transform->scale;

            // note that collider's size does not change, with the assumption that the object does not change size
            // we just update where the "box" starts from, based on translation of the transform component
            dstRectangle.x = collider.x - Game::camera.x;
            dstRectangle.y = collider.y - Game::camera.y;
        }

        void Render() override {
            if(showColliderBoundary) {
                // moves the initially loaded texture (at srcRectangle), to the next position based on the updated info in dstRectangle
                TextureManager::Draw(texture, srcRectangle, dstRectangle, SDL_FLIP_NONE);
            }          
        }
};

#endif