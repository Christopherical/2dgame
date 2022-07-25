#include "./Game.h"

#include <iostream>

#include "../lib/glm/glm.hpp"
#include "./AssetManager.h"
#include "./Components/KeyboardControlComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/TransformComponent.h"
#include "./Components/ColliderComponent.h"
#include "./Constants.h"
#include "./Map.h"

EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Map* map;

Game::Game() { this->isRunning = false; }

Game::~Game() { std::cout << "Destructor Called"; }

bool Game::IsRunning() const { return this->isRunning; }

void Game::Initialize(int width, int height) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }
    window = SDL_CreateWindow(
        NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
        SDL_WINDOW_BORDERLESS  // check documentation for borders
    );

    if (!window) {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        std::cerr << "Error creating SDL renderer" << std::endl;
        return;
    }

    LoadLevel(0);

    isRunning = true;
    return;
}

Entity& playerEntity(manager.AddEntity("player", PLAYER_LAYER));

void Game::LoadLevel(int levelNumber) {
    // Start including new assets to the assetmanager list
    assetManager->AddTexture(
        "tank-image",
        std::string("./assets/images/tank-big-right.png").c_str());

    assetManager->AddTexture(
        "player-image",
        std::string("./assets/images/chopper-spritesheet.png").c_str());

    assetManager->AddTexture(
        "jungle-tile-texture",
        std::string("./assets/tilemaps/jungle.png").c_str());

    map = new Map("jungle-tile-texture", 2, 32);
    map->LoadMap("./assets/tilemaps/jungle.map", 25, 20);

    // Start including entities and also components to them

    playerEntity.AddComponent<TransformComponent>(240, 106, 0, 0, 32, 32, 1);
    playerEntity.AddComponent<SpriteComponent>("player-image", 2, 90, true,
                                               false);
    playerEntity.AddComponent<KeyboardControlComponent>("up", "right", "down",
                                                        "left", "space");
    playerEntity.AddComponent<ColliderComponent>("PLAYER", 240, 106, 32, 32);

    Entity& tankEntity(manager.AddEntity("tank", ENEMY_LAYER));
    tankEntity.AddComponent<TransformComponent>(0, 0, 20, 20, 32, 32, 1);
    tankEntity.AddComponent<SpriteComponent>("tank-image");
    tankEntity.AddComponent<ColliderComponent>("ENEMY", 150, 495, 32, 32);
}

void Game::ProcessInput() {
    // Documentation for event:
    // https://www.libsdl.org/release/SDL-1.2.15/docs/html/sdlevent.html
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                isRunning = false;
            }

        default:
            break;
    }
}

void Game::Update() {
    // Wait until 16.6ms has elapsed since the last frame.

    int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - ticksLastFrame);

    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
        SDL_Delay(timeToWait);
    }

    // Delta time is the difference in ticks from last frame converted to
    // seconds.
    float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

    deltaTime = (deltaTime > 0.05f) ? 0.05 : deltaTime;

    // Sets the new ticks for the current frame to be used in the next pass.
    ticksLastFrame = SDL_GetTicks();

    manager.Update(deltaTime);

    HandleCameraMovement();
    CheckCollisions();
}

// back buffer vs front buffer.
void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    if (manager.HasNoEntities()) {
        return;
    }
    manager.Render();

    SDL_RenderPresent(renderer);
}

void Game::HandleCameraMovement() {
    TransformComponent* mainPlayerTransform =
        playerEntity.GetComponent<TransformComponent>();

    camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH / 2);
    camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

    // Clamping.
    camera.x = camera.x < 0 ? 0 : camera.x;
    camera.y = camera.y < 0 ? 0 : camera.y;
    camera.x = camera.x > camera.w ? camera.w : camera.x;
    camera.y = camera.y > camera.h ? camera.h : camera.y;
}

void Game::CheckCollisions() {
    CollisionType collisionType = manager.CheckEntityCollisions();
    if (collisionType == PLAYER_ENEMY_COLLISION) {
        ProcessGameOver();
    }
    if (collisionType == PLAYER_LEVEL_COMPLETE_COLLISION) {
        ProcessNextLevel(1);
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

void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}