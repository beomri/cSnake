#include "dbg.h"
#include "consts.h"
#include "snake.h"
#include "resources/body_img.h"
#include "resources/head_img.h"
#include "resources/food_img.h"
#include <stdio.h>
#include <SDL2/SDL.h>


SDL_Renderer* gRenderer = NULL;
SDL_Texture* text_body = NULL;
SDL_Texture* text_head = NULL;
SDL_Texture* text_food = NULL;


void tick(Uint32 time)
{
    if(SDL_GetTicks() - time < TICKS_PER_FRAME)
        SDL_Delay(time + TICKS_PER_FRAME - SDL_GetTicks());
}

void load_texture(SDL_Texture** textr, const char* img_name, int size)
{
    SDL_RWops *rw = SDL_RWFromConstMem(img_name, size);
    SDL_Surface* temp_surf = SDL_LoadBMP_RW(rw, TRUE);
    SDL_SetColorKey(temp_surf, SDL_TRUE, SDL_MapRGB(temp_surf->format, 130, 130, 130));
    *textr = SDL_CreateTextureFromSurface(gRenderer, temp_surf);
    SDL_FreeSurface(temp_surf);
}

void draw_all(snake* s)
{
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
            SDL_RenderCopy(gRenderer, text_body, NULL, &r);
            sb = sb->next;
        }
        while(sb != s->first);
    r.x = s->head->pos_x * EDGE;
    r.y = s->head->pos_y * EDGE;
    r.w = EDGE;
    r.h = EDGE;
    switch(s->dir)
    {
        case 0:
        case DIR_UP:
            SDL_RenderCopy(gRenderer, text_head, NULL, &r);
            break;
        case DIR_DOWN:
            SDL_RenderCopyEx(gRenderer, text_head, NULL, &r, 180.0, NULL, SDL_FLIP_NONE);
            break;
        case DIR_RIGHT:
            SDL_RenderCopyEx(gRenderer, text_head, NULL, &r, 90.0, NULL, SDL_FLIP_NONE);
            break;
        case DIR_LEFT:
            SDL_RenderCopyEx(gRenderer, text_head, NULL, &r, 270.0, NULL, SDL_FLIP_NONE);
            break;
    }
}

void check_collision(snake* s)
{
    if(s->head->pos_x < 0 || s->head->pos_x > W_WIDTH / EDGE ||
       s->head->pos_y < 0 || s->head->pos_y > W_HEIGHT / EDGE)
    {
        free_body(s);
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

    load_texture(&text_head, img_head, img_head_length);
    load_texture(&text_body, img_body, img_body_length);
    load_texture(&text_food, img_food, img_food_length);

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
        SDL_RenderPresent(gRenderer);

        tick(start_frame);
        check_collision(s);
        start_frame = SDL_GetTicks();
    }

    free_snake(s);
    SDL_DestroyTexture( text_head );
    SDL_DestroyTexture( text_body );
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;

    error:
    return -1;
}
