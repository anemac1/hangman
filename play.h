/** Game Module Header
 * @file play.h
*/

#ifndef PLAY_H
#define PLAY_H

#include "hangman.h"

int cl_word(char *word, file_words_t list[]);
int get_word(file_words_t list[]);
int find_unused_words(int nwords, file_words_t list[], char *filename);
int play(char word[], int length);
void display_guesses(int wcounter, char *uch);
int rand_no (int *pmin, int *pmax);

#endif
