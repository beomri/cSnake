#ifndef _SNAKE
#define _SNAKE

typedef enum { S_UP, S_DOWN, S_RIGHT, S_LEFT } direction_type;

typedef struct s_head {int pos_x;
                       int pos_y;
                       direction_type dir;} s_head;


typedef struct s_body {int pos_x;
                       int pos_y;
                       struct s_body* next;
                       struct s_body* prev;} s_body;

typedef struct snake {struct s_head* head;
                      struct s_body* first;} snake;


snake* create_snake(int pos_x, int pos_y);
int add_body_to_snake(snake* c_snake);
void free_snake(snake* new_snake);

#endif
