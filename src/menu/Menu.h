#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

using namespace std;

class Menu
{
public:
    Menu(SDL_Renderer *renderer);
    ~Menu();
    void Render();
    bool CheckIfPlayBtnClicked(SDL_Event &event);

private:
    SDL_Renderer *renderer;
    TTF_Font *font;
    SDL_Color textColor;
    SDL_Texture *playBtnTexture;
    SDL_Rect playBtnImg;

    void RenderText(const string &text, int x, int y);
};

#endif
