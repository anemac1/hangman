/** File Input_Output Module Header
 * @file file_io.h
*/

#ifndef FILE_IO_H
#define FILE_IO_H

#include "hangman.h"

int word_reset (char *filename);
int file_write (char *filename, file_words_t list[], int index);
int file_read (char *filename, file_words_t list[]);
void free_struct(file_words_t *);

#endif