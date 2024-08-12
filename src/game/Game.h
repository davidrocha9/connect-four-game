#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <Board.h>

class Game
{
public:
    Game();
    ~Game();
    bool Init();
    void Run();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running;
    Board *board;

    void HandleEvents();
    void Render();
};

#endif