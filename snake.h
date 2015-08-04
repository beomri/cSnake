#ifndef _SNAKE
#define _SNAKE


typedef struct s_head {int pos_x;
                       int pos_y;} s_head;


typedef struct s_body {int pos_x;
                       int pos_y;
                       struct s_body* next;
                       struct s_body* prev;} s_body;

typedef struct snake {struct s_head* head;
                      struct s_body* first;
                      int dir;} snake;


snake* create_snake(int pos_x, int pos_y);
int add_body_to_snake(snake* c_snake);
void free_snake(snake* new_snake);
void move_snake(snake* c_snake);
void print_snake(snake* c_snake);
void change_dir(snake* c_snake, int dir);
void free_body(snake* c_snake);

#endif
