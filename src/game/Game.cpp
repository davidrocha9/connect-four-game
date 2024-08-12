#include "Game.h"
#include <iostream>

using namespace std;

Game::Game() : window(nullptr), renderer(nullptr), running(false) {}

Game::~Game()
{
    delete board;

    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Game::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Connect 4!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 720, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (!renderer)
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    board = new Board(renderer);
    return true;
}

void Game::Run()
{
    Render();

    running = true;
    while (running)
    {
        HandleEvents();
        Render();
        SDL_Delay(16); // Limit to 60 fps
    }
}

void Game::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        switch (event.type)
        {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                board->HandleClick(event.button.x);
            }
            break;
        case SDL_MOUSEMOTION:
            board->HandleMouseMove(event.motion.x);
            break;
        default:
            break;
        }
    }
}

void Game::Render()
{
    SDL_SetRenderDrawColor(renderer, 0xAD, 0xD8, 0xE6, 0xFF);
    SDL_RenderClear(renderer);

    if (board)
    {
        board->Render();
    }

    SDL_RenderPresent(renderer);
}