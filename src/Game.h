#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "./Entity.h"
#include "./EntityManager.h"
#include "./Component.h"

class AssetManager;

class Game {
    private:
        bool isRunning;
        SDL_Window* window;
    public:
        Game();
        ~Game();
        
        unsigned int ticksLastFrame; // in miliseconds
        bool IsRunning() const;
        
        static SDL_Renderer* renderer; // since it's shared across multiple components 
        static AssetManager* assetManager; // since it's shared across multiple components 
        static SDL_Event event;
        static SDL_Rect camera;
        
        void LoadLevel(int levelNumber);
        void ShowInfo();
        void Initialize(int width, int height);
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();
        void HandleCameraMovement();
        void CheckCollisions();
        void ProcessGameOver();
        void ProcessNextLevel(int levelNumber);
};

#endif