#include "snake.h"
#include "dbg.h"
#include "consts.h"

#include <stdlib.h>

snake* create_snake(int pos_x, int pos_y)
{
    snake* new_snake = malloc(sizeof(snake));
    s_head* new_head = malloc(sizeof(s_head));

    new_head->pos_x = pos_x;
    new_head->pos_y = pos_y;
    new_head->dir = S_UP;

    new_snake->head = new_head;
    new_snake->first  = NULL;

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

void free_snake(snake* new_snake)
{
    free(new_snake->head);
    new_snake->first->prev->next = NULL;
    s_body* temp = new_snake->first;
    s_body* next = NULL;
    while(temp)
    {
        next = temp->next;
        free(temp);
        temp = next;
    }
    free(new_snake);
}
