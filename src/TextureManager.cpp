#include "./TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* fileName) {
    SDL_Surface* surface = IMG_Load(fileName);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_FreeSurface(surface);

    return texture;

}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect srcRectangle, SDL_Rect destRectangle, SDL_RendererFlip flip) {

    // src: what it originally looks like
    // dest: how it ends up looking like on the screen (bigger, shifted, cropped, moved, etc.)
    SDL_RenderCopyEx(Game::renderer, texture, &srcRectangle, &destRectangle, 0.0, NULL, flip);
    // SDL_RenderCopy(Game::renderer, texture, NULL, &destRectangle);

}