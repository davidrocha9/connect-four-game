#include "Utils.h"
#include <iostream>

void LoadTexture(const std::string &filePath, SDL_Renderer *renderer, SDL_Texture *&texture)
{
    SDL_Surface *tempSurface = IMG_Load(filePath.c_str());
    if (!tempSurface)
    {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        texture = nullptr;
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if (!texture)
    {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
    }
}
