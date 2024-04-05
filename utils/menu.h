#ifndef MENU_H
#define MENU_H

typedef enum Choices
{
    op_play,
    op_options,
    op_exit,
    op_potatis,
    op_N
} Choices;

typedef struct menu
{
    const int n_choices;
    const int height;
    const int width;
    const int y;
    const int x;
} Menu;

extern const char* const title[7];

extern const char* const menu_choices[4];
extern const int outer_menu_width;
extern const int inner_menu_width;

Choices start_menu();
#endif
