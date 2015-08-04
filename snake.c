#include "snake.h"
#include "dbg.h"
#include "consts.h"

#include <stdio.h>
#include <stdlib.h>

snake* create_snake(int pos_x, int pos_y)
{
    snake* new_snake = malloc(sizeof(snake));
    s_head* new_head = malloc(sizeof(s_head));

    new_head->pos_x = pos_x;
    new_head->pos_y = pos_y;

    new_snake->head = new_head;
    new_snake->first  = NULL;
    new_snake->dir = 0;

    return new_snake;
}

int add_body_to_snake(snake* c_snake)
{
    s_body* new_body = malloc(sizeof(s_body));

    new_body->pos_x = c_snake->head->pos_x;
    new_body->pos_y = c_snake->head->pos_y;

    if (c_snake->first)
    {
        new_body->prev = c_snake->first->prev;
        new_body->prev->next = new_body;

        new_body->next = c_snake->first;
        new_body->next->prev = new_body;
    }
    else
    {
        c_snake->first = new_body;
        new_body->next = new_body;
        new_body->prev = new_body;
    }

    return TRUE;
}

void change_dir(snake* c_snake, int dir)
{
    if(dir != -c_snake->dir && dir + 2 * c_snake->dir != DIR_LEFT)
        c_snake->dir = dir;
}

void move_snake(snake* c_snake)
{
    if (c_snake->dir == 0)
        return;
    int temp_x = c_snake->head->pos_x;
    int temp_y = c_snake->head->pos_y;

    c_snake->head->pos_x += c_snake->dir % 2;
    c_snake->head->pos_y += c_snake->dir / 2;

    if (!c_snake->first)
        return;

    c_snake->first = c_snake->first->prev;
    c_snake->first->pos_x = temp_x;
    c_snake->first->pos_y = temp_y;
}

void free_body(snake* c_snake)
{
    if(c_snake->first)
        c_snake->first->prev->next = NULL;
    s_body* temp = c_snake->first;
    s_body* next = NULL;
    while(temp)
    {
        next = temp->next;
        free(temp);
        temp = next;
    }
    c_snake->first = NULL;
}

void free_snake(snake* c_snake)
{
    free(c_snake->head);
    free_body(c_snake);
    free(c_snake);
}

void print_snake(snake* c_snake)
{
    printf("Head: (%d,%d)\n", c_snake->head->pos_x, c_snake->head->pos_y );
    if (!c_snake->first)
        return;
    printf("Body:\n");
    printf("(%d,%d)\n", c_snake->first->pos_x, c_snake->first->pos_y );
    s_body* temp = c_snake->first->next;
    while(temp != c_snake->first)
    {
        printf("(%d,%d)\n", temp->pos_x, temp->pos_y );
        temp = temp->next;
    }
}
