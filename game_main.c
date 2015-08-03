#include "dbg.h"
#include "consts.h"
#include "snake.h"
#include <stdio.h>
#include <SDL2/SDL.h>


SDL_Renderer* gRenderer = NULL;


void tick(Uint32 time)
{
    if(SDL_GetTicks() - time < TICKS_PER_FRAME)
        SDL_Delay(time + TICKS_PER_FRAME - SDL_GetTicks());
}


void draw_all(snake* s)
{
    //SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, COLOR_BROWN));
    SDL_SetRenderDrawColor( gRenderer, COLOR_BROWN, 0xFF );
    SDL_RenderClear(gRenderer);

    SDL_Rect r;
    s_body* sb = s->first;
    if(sb)
        do
        {
            r.x = sb->pos_x * EDGE;
            r.y = sb->pos_y * EDGE;
            r.w = EDGE;
            r.h = EDGE;
            //SDL_FillRect(surf, &r, SDL_MapRGB(surf->format, COLOR_LIGHT_RED));
            SDL_SetRenderDrawColor(gRenderer, COLOR_LIGHT_RED, 0xFF);
            SDL_RenderFillRect(gRenderer, &r);
            sb = sb->next;
        }
        while(sb != s->first);
    r.x = s->head->pos_x * EDGE;
    r.y = s->head->pos_y * EDGE;
    r.w = EDGE;
    r.h = EDGE;
    //SDL_FillRect(surf, &r, SDL_MapRGB(surf->format, COLOR_RED));
    SDL_SetRenderDrawColor(gRenderer, COLOR_RED, 0xFF);
    SDL_RenderFillRect(gRenderer, &r);
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

    gRenderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    check(gRenderer,"couldnt create renderer");

    //SDL_Surface* surf = SDL_GetWindowSurface(window);
    //check(surf, "Couldn't find window surface");

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
                        case SDLK_SPACE:
                            add_body_to_snake(s);
                            break;
                    }
                    break;
            }
        }

        move_snake(s);
        draw_all(s);
        //SDL_UpdateWindowSurface(window);
        SDL_RenderPresent(gRenderer);

        tick(start_frame);
        check_collision(s);
        start_frame = SDL_GetTicks();
    }

    free_snake(s);
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;

    error:
    return -1;
}
