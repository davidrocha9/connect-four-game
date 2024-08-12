#include "Menu.h"
#include <iostream>
#include <unistd.h>
#include <Utils.h>

Menu::Menu(SDL_Renderer *renderer)
    : renderer(renderer), font(nullptr), textColor({0, 0, 0, 255})
{
    if (TTF_Init() == -1)
    {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
    }

    font = TTF_OpenFont("../fonts/font.ttf", 50);
    if (!font)
    {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
    }

    LoadTexture("../assets/play.png", renderer, playBtnTexture);
}

Menu::~Menu()
{
    if (playBtnTexture)
    {
        SDL_DestroyTexture(playBtnTexture);
        playBtnTexture = nullptr;
    }

    if (font)
    {
        TTF_CloseFont(font);
        font = nullptr;
    }
}

void Menu::Render()
{
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    RenderText("Connect Four", windowWidth / 2 - 160, windowHeight / 4);

    playBtnImg = {
        windowWidth / 2 - 100,
        windowHeight / 2 - 50,
        200,
        200};

    SDL_RenderCopy(renderer, playBtnTexture, nullptr, &playBtnImg);
}

bool Menu::CheckIfPlayBtnClicked(SDL_Event &event)
{
    if (event.type == SDL_MOUSEBUTTONUP)
    {
        int mouseX = event.button.x;
        int mouseY = event.button.y;
        if (mouseX >= playBtnImg.x && mouseX <= playBtnImg.x + playBtnImg.w && mouseY >= playBtnImg.y && mouseY <= playBtnImg.y + playBtnImg.h)
        {
            return true;
        }
    }
    return false;
}

void Menu::RenderText(const std::string &text, int x, int y)
{
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if (!textSurface)
    {
        std::cerr << "TTF_RenderText_Solid Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    if (!textTexture)
    {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        return;
    }

    int textWidth = 0, textHeight = 0;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

    SDL_Rect textRect = {x, y, textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_DestroyTexture(textTexture);
}
