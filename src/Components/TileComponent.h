#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include <SDL2/SDL.h>

#include "../EntityManager.h"
#include "../AssetManager.h"

#include "../../lib/glm/glm.hpp"

class TileComponent: public Component {
    public: 
        SDL_Texture* texture;
        SDL_Rect srcRectangle;
        SDL_Rect dstRectangle;
        glm::vec2 position;

        TileComponent(int sourceRectX, int sourceRectY, int destRectx, int destRecty, int tileSize, int tileScale, std::string assetTextureId) {
            texture = Game::assetManager->GetTexture(assetTextureId);

            srcRectangle.x = sourceRectX;
            srcRectangle.y = sourceRectY;
            srcRectangle.w = tileSize;
            srcRectangle.h = tileSize;

            dstRectangle.x = destRectx;
            dstRectangle.y = destRecty;
            dstRectangle.w = tileSize * tileScale;
            dstRectangle.h = tileSize * tileScale;

            position.x = destRectx;
            position.y = destRecty;
            
        }

        ~TileComponent() {
            SDL_DestroyTexture(texture);
        }

        void Update(const float& deltaTime) override {
            // move tile positions (dstRect) based on camera control
            dstRectangle.x = position.x - Game::camera.x;
            dstRectangle.y = position.y - Game::camera.y;
        }

        void Render() override {
            TextureManager::Draw(texture, srcRectangle, dstRectangle, SDL_FLIP_NONE);
        }
};


#endif