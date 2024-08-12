#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

void LoadTexture(const std::string &filePath, SDL_Renderer *renderer, SDL_Texture *&texture);

#endif
