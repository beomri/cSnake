#include "dbg.h"
#include "consts.h"
#include "snake.h"
#include <stdio.h>
#include <SDL2/SDL.h>


void tick(Uint32 time)
{
    while(SDL_GetTicks() - time < TICKS_PER_FRAME)
        SDL_Delay(100);
}


void draw_all(SDL_Surface* surf ,snake* s)
{
    SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, COLOR_BROWN));

    SDL_Rect r = {.x = s->head->pos_x * EDGE,
                  .y = s->head->pos_y * EDGE,
                  .w = EDGE,
                  .h = EDGE};
    SDL_FillRect(surf, &r, SDL_MapRGB(surf->format, COLOR_RED));
}

void check_collision(snake* s)
{
    if(s->head->pos_x < 0 || s->head->pos_x > W_WIDTH / EDGE ||
       s->head->pos_y < 0 || s->head->pos_y > W_HEIGHT / EDGE)
    {
        change_dir(s,0);
        s->head->pos_x = 8;
        s->head->pos_y = 8;
    }
}

int main(int argc, char* args[])
{
    snake* s = create_snake(8,8);

    int quit = FALSE;

    Uint32 start_frame = 0;

    SDL_Event e;

    check(SDL_Init(SDL_INIT_VIDEO) == 0, "Couldn't initialise SDL!");

    SDL_Window* window = SDL_CreateWindow("snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W_WIDTH, W_HEIGHT, SDL_WINDOW_SHOWN);
    check(window,"Couldn't create window!");

    SDL_Surface* surf = SDL_GetWindowSurface(window);
    check(surf, "Couldn't find window surface");

    while(!quit)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            switch(e.type){
                case SDL_QUIT:
                    quit = TRUE;
                    break;
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym)
                    {
                        case SDLK_UP:
                            change_dir(s,DIR_UP);
                            break;
                        case SDLK_DOWN:
                            change_dir(s,DIR_DOWN);
                            break;
                        case SDLK_RIGHT:
                            change_dir(s,DIR_RIGHT);
                            break;
                        case SDLK_LEFT:
                            change_dir(s,DIR_LEFT);
                            break;
                        case SDLK_ESCAPE:
                            quit = TRUE;
                            break;
                    }
                    break;
            }
        }

        move_snake(s);
        draw_all(surf, s);
        SDL_UpdateWindowSurface(window);

        tick(start_frame);
        check_collision(s);
        start_frame = SDL_GetTicks();
    }

    free_snake(s);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;

    error:
    return -1;
}

