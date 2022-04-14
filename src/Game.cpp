#include "./Game.h"

#include <iostream>

#include "./Constants.h"

Game::Game() { this->isRunning = false; }

Game::~Game() { std::cout << "Destructor Called"; }

bool Game::IsRunning() const { return this->isRunning; }

float ProjectilePosX = 0.0f;
float ProjectilePosY = 0.0f;
float ProjectileVelX = 0.5f;
float ProjectileVelY = 0.5f;

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

    isRunning = true;
    return;
}

void Game::ProcessInput() {
    // Documentation for even:
    // https://www.libsdl.org/release/SDL-1.2.15/docs/html/sdlevent.html
    SDL_Event event;
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
    ProjectilePosX += ProjectileVelX;
    ProjectilePosY += ProjectileVelY;
}

// back buffer vs front buffer.
void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    SDL_Rect projectile{(int)ProjectilePosX, (int)ProjectilePosY, 10, 10

    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &projectile);

    SDL_RenderPresent(renderer);
}

void Game::Destroy(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}