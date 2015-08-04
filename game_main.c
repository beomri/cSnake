#include "dbg.h"
#include "consts.h"
#include "snake.h"
#include "resources/body_img.h"
#include "resources/head_img.h"
#include "resources/food_img.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_Window* g_window = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* text_body = NULL;
SDL_Texture* text_head = NULL;
SDL_Texture* text_food = NULL;
snake* s = NULL;
Uint32 start_frame = 0;
SDL_Event e;
int food_x = 0;
int food_y = 0;
int lock_dir = FALSE;

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

void draw_all()
{
    SDL_SetRenderDrawColor( gRenderer, COLOR_BROWN, 0xFF );
    SDL_RenderClear(gRenderer);

    SDL_Rect r;
    r.w = EDGE;
    r.h = EDGE;
    s_body* sb = s->first;
    if(sb)
        do
        {
            r.x = sb->pos_x * EDGE;
            r.y = sb->pos_y * EDGE;
            SDL_RenderCopy(gRenderer, text_body, NULL, &r);
            sb = sb->next;
        }
        while(sb != s->first);

    r.x = food_x * EDGE;
    r.y = food_y * EDGE;
    SDL_RenderCopy(gRenderer, text_food, NULL ,&r);

    r.x = s->head->pos_x * EDGE;
    r.y = s->head->pos_y * EDGE;
    switch(s->dir)
    {
        case DIR_UP:
            SDL_RenderCopy(gRenderer, text_head, NULL, &r);
            break;
        case DIR_DOWN:
            SDL_RenderCopyEx(gRenderer, text_head, NULL, &r, 180.0, NULL, SDL_FLIP_NONE);
            break;
        case 0:
        case DIR_RIGHT:
            SDL_RenderCopyEx(gRenderer, text_head, NULL, &r, 90.0, NULL, SDL_FLIP_NONE);
            break;
        case DIR_LEFT:
            SDL_RenderCopyEx(gRenderer, text_head, NULL, &r, 270.0, NULL, SDL_FLIP_NONE);
            break;
    }
}

int check_collision_body(int x, int y)
{
    s_body* sb = s->first;
    if(sb)
        do
        {
            if (x == sb->pos_x && y == sb->pos_y)
                return TRUE;
            sb = sb->next;
        }
        while(sb != s->first);
    return FALSE;
}

int check_collision()
{
    if(s->head->pos_x < 0 || s->head->pos_x + 1 > G_WIDTH ||
       s->head->pos_y < 0 || s->head->pos_y + 1 > G_HEIGHT)
        return TRUE;
    return check_collision_body(s->head->pos_x, s->head->pos_y);
}

void place_food()
{
    do
    {
    food_x = rand() % G_WIDTH;
    food_y = rand() % G_HEIGHT;
    }
    while(check_collision_body(food_x, food_y));
}

int check_food()
{
    if(s->head->pos_x == food_x && s->head->pos_y == food_y)
        return TRUE;
    return FALSE;
}

void send_change_dir(int dir)
{
    if (!lock_dir)
    {
        change_dir(s, dir);
        lock_dir = TRUE;
    }
}

void exit_game()
{
    free_snake(s);
    SDL_DestroyTexture(text_head);
    SDL_DestroyTexture(text_body);
    SDL_DestroyTexture(text_food);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(g_window);
    SDL_Quit();
}

int run_game()
{
    int lost = FALSE;
    int points = 0;
    s = create_snake((G_WIDTH / 2) - 2, G_HEIGHT / 2);
    s->dir = DIR_RIGHT;
    add_body_to_snake(s);
    move_snake(s);
    add_body_to_snake(s);
    move_snake(s);
    s->dir = 0;
    place_food();
    while(!lost)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            switch(e.type){
                case SDL_QUIT:
                    return -1;
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
                            return -1;
                            break;
                        case SDLK_SPACE:
                            add_body_to_snake(s);
                            break;
                    }
                    break;
            }
        }

        move_snake(s);
        lock_dir = FALSE;
        draw_all();
        SDL_RenderPresent(gRenderer);
        lost = check_collision();
        if (check_food())
        {
            points++;
            place_food();
            add_body_to_snake(s);
        }

        tick(start_frame);
        start_frame = SDL_GetTicks();
    }
    free_snake(s);
    s = NULL;
    return points;
}

int main(int argc, char* args[])
{
    check(SDL_Init(SDL_INIT_VIDEO) == 0, "Couldn't initialise SDL!");

    g_window = SDL_CreateWindow("snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W_WIDTH, W_HEIGHT, SDL_WINDOW_SHOWN);
    check(g_window,"Couldn't create window!");

    gRenderer = SDL_CreateRenderer( g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    check(gRenderer,"couldnt create renderer");

    load_texture(&text_head, img_head, img_head_length);
    load_texture(&text_body, img_body, img_body_length);
    load_texture(&text_food, img_food, img_food_length);

    int g_status = 0;

    while(TRUE)
    {
        g_status = run_game();
        if (g_status == -1)
        {
            exit_game();
            return 0;
        }
    }

    exit_game();
    return 0;

    error:
    return -1;
}
