/** Menu Module Header
 * @file menu.h
*/

#ifndef MENU_H
#define MENU_H

/** Struct \c menu.
 *
 * Defines a struct for the content of the menu.
 * \param symbol  Number or Character to choose.
 * \param text  Description of menu point.
*/
struct menu_s {
    char symbol; 
    char *text; 
    };
typedef struct menu_s menu_t;

void show_menu();
int parse_input();
void show_instr();
int set_file();

#endif
