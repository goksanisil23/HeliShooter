#include <iostream>

#include "./AssetManager.h"
#include "./Constants.h"
#include "./Game.h"
#include "./Map.h"
#include "./Components/TransformComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/KeyboardControlComponent.h"
#include "./Components/ColliderComponent.h"
#include "./Components/TextLabelComponent.h"
#include "./Components/ProjectileEmitterComponent.h"
#include "../lib/glm/glm.hpp"

EntityManager manager;
SDL_Renderer* Game::renderer; // static class variable cannot be initialized within the class, since there is only 1 instance
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Map* map;
bool ColliderComponent::showColliderBoundary = false; // do not show boundaries of collider by default

Game::Game() {
    isRunning = false;
}

Game::~Game() {

}

bool Game::IsRunning() const {
    return isRunning;
}

void Game::Initialize(int width, int height) {
    if( SDL_Init(SDL_INIT_EVERYTHING) != 0 ) {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }

    if(TTF_Init() != 0) {
        std::cerr << "Error initializing SDL TTF" << std::endl;
        return;
    }

    // window and renderer are created internally created on the heap by SDL
    window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_BORDERLESS);

    if(!window) {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if(!renderer) {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }

    LoadLevel(0);

    // ShowInfo();

    isRunning = true;
    return;

}

void Game::ShowInfo() {
    if(!manager.HasNoEntities()) {
        manager.ListAllEntities();
    }

    if (manager.GetEntities().at(0)->HasComponent<TransformComponent>()) {
        std::cout << "Has transform component!" << std::endl;
    }
}

Entity& playerEntity(manager.AddEntity("chopper", PLAYER_LAYER)); // initialize entity as a reference

void Game::LoadLevel(int levelNumber) {
    // Start including new assets to the assetmanager list
    assetManager->AddTexture("tank-image", std::string("./assets/images/tank-big-right.png").c_str());
    assetManager->AddTexture("chopper-image", std::string("./assets/images/chopper-spritesheet.png").c_str());
    assetManager->AddTexture("radar-image", std::string("./assets/images/radar.png").c_str());
    assetManager->AddTexture("jungle-tiletexture", std::string("./assets/tilemaps/jungle.png").c_str());
    assetManager->AddTexture("collider-image", std::string("./assets/images/collision-texture.png").c_str());
    assetManager->AddTexture("heliport-image", std::string("./assets/images/heliport.png").c_str());
    assetManager->AddTexture("projectile-image", std::string("./assets/images/bullet-enemy.png").c_str());
    assetManager->AddFont("charriot-font", std::string("./assets/fonts/charriot.ttf").c_str(), 14);
    
    map = new Map("jungle-tiletexture", 2, 32); // id, scale, tileSize (with scale 1, this map fits perfectly to 800x600)
    map->LoadMap("./assets/tilemaps/jungle.map", 25, 20); // filepath, map file column size, map file row size
    
    playerEntity.AddComponent<TransformComponent>(240, 106, 0, 0, 32, 32, 1); // posX, posY, velX, velY, w, h, s
    playerEntity.AddComponent<SpriteComponent>("chopper-image", 2, 90, true, false); // id, numFrames, animationSpeed, hasDirections, isFixed
    playerEntity.AddComponent<ColliderComponent>("PLAYER", 240, 106, 32, 32, "collider-image");
    playerEntity.AddComponent<KeyboardControlComponent>("up", "down", "right", "left", "shoot", "showCollider");

    // Start including entities and also components to them
    Entity& tankEntity(manager.AddEntity("tank", ENEMY_LAYER)); // initialize entity as a reference
    tankEntity.AddComponent<TransformComponent>(150,495,5,0,32,32,1); // posX, posY, velX, velY, w, h, s)
    tankEntity.AddComponent<SpriteComponent>("tank-image"); 
    tankEntity.AddComponent<ColliderComponent>("ENEMY", 150,495, 32, 32, "collider-image"); 

    Entity& projectileEntity(manager.AddEntity("projectile", PROJECTILE_LAYER));
    projectileEntity.AddComponent<TransformComponent>(150+16, 495+16, 0, 0, 4, 4, 1);
    projectileEntity.AddComponent<SpriteComponent>("projectile-image");
    projectileEntity.AddComponent<ColliderComponent>("PROJECTILE", 150+16, 495+16, 4, 4, "collider-image");
    projectileEntity.AddComponent<ProjectileEmitterComponent>(50, 270, 200, true); // speed, angle, range, shouldLoop

    Entity& heliportEntity(manager.AddEntity("Heliport", OBSTACLE_LAYER));
    heliportEntity.AddComponent<TransformComponent>(470, 420, 0, 0, 32, 32, 1); // posX, posY, velX, velY, w, h, s
    heliportEntity.AddComponent<SpriteComponent>("heliport-image");
    heliportEntity.AddComponent<ColliderComponent>("LEVEL_COMPLETE", 470, 420, 32, 32, "collider-image");

    Entity& radarEntity(manager.AddEntity("Radar", UI_LAYER)); // initialize entity as a reference    
    radarEntity.AddComponent<TransformComponent>(720, 15, 0, 0, 64, 64, 1);
    radarEntity.AddComponent<SpriteComponent>("radar-image", 8, 150, false, true); // id, numFrames, animationSpeed, hasDirections, isFixed

    Entity& labelLevelNameEntity(manager.AddEntity("LabelLevelName", UI_LAYER));
    labelLevelNameEntity.AddComponent<TextLabelComponent>(10, 10, "First Level ...", "charriot-font", WHITE_COLOR);


}



void Game::ProcessInput() {
    SDL_PollEvent(&event);
    switch(event.type) {
        case SDL_QUIT: {
            isRunning = false;
            break;
        }
        case SDL_KEYDOWN: {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                isRunning = false;
            }
        }
        default: {
            break;
        }
    }
}

void Game::Update() {
    // Wait until FRAME_TARGET_TIME has ellapsed since the last frame (given that machine is faster than desired rate)
    int timeToWait = FRAME_TARGET_TIME -(SDL_GetTicks() - ticksLastFrame);
    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
        SDL_Delay(timeToWait);
    }
    
    // Delta time is the difference in ticks from last frame converted to seconds
    float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f; // in seconds

    // Clamp deltaTime to a maximum value
    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

    // Sets the new ticks for the current frame to be used in the next pass
    ticksLastFrame = SDL_GetTicks(); // in miliseconds

    manager.Update(deltaTime);

    HandleCameraMovement();

    CheckCollisions();
}

void Game::Render() {
    // 1) set the background color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // 2) Clear back buffer (clears the current rendering target with the drawing color)
    SDL_RenderClear(renderer);
    
    // 3) TODO: Here we call the manager.render to render all entitites
    if (manager.HasNoEntities()) {
        return;
    }
    manager.Render();

    // 4) Swap front and back buffers, to update the screen with rendering performed since last render clear
    SDL_RenderPresent(renderer);

}

void Game::Destroy() {
    // all the memory is freed
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// camera is not physically used for rendering. Its just some window boundary parameters that 
// offsets how other entities are rendered (basically shifting)
void Game::HandleCameraMovement() {
    TransformComponent* mainPlayerTransform = playerEntity.GetComponent<TransformComponent>();

    camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH / 2);
    camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

    // Clamping values of the camera
    camera.x = camera.x < 0 ? 0 : camera.x;
    camera.y = camera.y < 0 ? 0 : camera.y;
    camera.x = camera.x > camera.w ? camera.w : camera.x;
    camera.y = camera.y > camera.h ? camera.h : camera.y;
}

void Game::CheckCollisions() {
    CollisionType collisionType = manager.CheckCollisions();
    if(collisionType == PLAYER_ENEMY_COLLISION) {
        ProcessGameOver();
    }
    if(collisionType == PLAYER_LEVEL_COMPLETE_COLLISION) {
        ProcessNextLevel(1);
    }
    if(collisionType == PLAYER_PROJECTILE_COLLISION) {
        ProcessGameOver();
    }

}

void Game::ProcessNextLevel(int levelNumber) {
    std::cout << "Next Level" << std::endl;
    isRunning = false;
}

void Game::ProcessGameOver() {
    std::cout << "Game Over" << std::endl;
    isRunning = false;
}
