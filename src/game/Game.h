#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "Board.h"
#include "Menu.h"

class Game
{
public:
    Game();
    ~Game();
    bool Init();
    void Run();

private:
    enum class GameState
    {
        Menu,
        InGame
    };

    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running;
    Menu *menu;
    Board *board;
    GameState state;

    void HandleEvents();
    void Render();
    void Update();
    void HandleMenuEvents(SDL_Event &event);
    void HandleInGameEvents(SDL_Event &event);
};

#endif
