#include <stdio.h>
#include <SDL2/SDL.h>
#include "dbg.h"
#include "consts.h"


int main(int argc, char* args[])
{
    check(SDL_Init(SDL_INIT_VIDEO) == 0, "Couldn't initialise SDL!");

    SDL_Window* window = SDL_CreateWindow("snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W_WIDTH, W_HEIGHT, SDL_WINDOW_SHOWN);
    check(window,"Couldn't create window!");

    SDL_Surface* surf = SDL_GetWindowSurface(window);

    while(TRUE)
    {
        SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, COLOR_BROWN));
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;

    error:
    return -1;
}

