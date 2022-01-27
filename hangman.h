/**
 *
 * @file hangman.h
 * @mainpage HSWE C project
 * @author Andreas Gelbmann, ic21b066
 * @date 19.1.2021
 *
 *  @section sec1 How to play
 *  This program written in C implements a game named "Hangman". 
 *  - use <tt> ./hangman <-w word> </tt> to play simple game
 *  - use <tt> ./hangman <-m> </tt> to play <b> advanced </b> game
 *
 *  @subsection sec1_1 Important: Using words from file!
 *  A file "words.txt" exists in the default directory which shows how to define new words:
 *  <b> <word> </b> followed by a <b> white space </b> and the <b> <number> </b> (0 for unused).
 * 
 *  @section sec2 Features
 *  -# Required Tasks (Main Task)
 * 		- Command line input
 * 		- Guessing character by character
 * 		- Uppercase only
 * 		- Log file (logs appended)
 *
 *  -# Additional Features
 * 		- Multiple game rounds (in <b> advanced </b> mode)
 * 		- Random word from file in each round
 * 		- Specify input file (having default on start and setting to default on error)
 * 		- Flagging of used words (only if guessed right)
 * 		- Exclusion of already used words in multiple rounds
 * 		- Reset of used flags in file
 * 		- Arguments handling by \c getopt(3)
 *
 *  -# Special Features
 * 		- Playing program from command line (simple) or with menu (in <b> advanced </b> mode)
 * 		- Coloured screen output
 * 		- Conversion of words to uppercase in file
 * 		- Instructions
 *
 *  @section sec3 Parts
 *  The program is splitted into the following modules.
 *
 *      @subsection sec3_1 Main (Hangman)
 *      @subsection sec3_2 Menu (and instructions)
 *      @subsection sec3_3 Play
 *      @subsection sec3_4 File_IO
 *      @subsection sec3_5 Logging
 *
 *  @section sec4 Version Control
 *  @version 1.1
 *  <b>Running version for Tasks 1, 3, 4, 5 and 7</b>.
 *  Closed warning on #includes coming up on Linux VM. Added \c getopt handling.
 *  
 *  @version 1.0 
 *  Added comments (Doxygen). 
 *  
 *  @version 0.9 
 *  Running version for Tasks 1, 3, 4 and 5.
 * 
 */

#ifndef HANGMAN_H
#define HANGMAN_H

/** \brief DEBUG_Level */
#define D 0
/** \brief Maximum number of characters of one word and of file name (including \0). */
#define maxchar 26
/** \brief Name of default file to provide words */
#define dfile "words.txt"
/** \brief Name of log file */
#define lfile "log.txt"
/** \brief Maximum number of words in txt-File */
#define maxwords 100
/** \brief Maximum number of wrong guesses */
#define wguesses 5

/** Struct <c> file_words </c>.
 *
 * Defines a struct for the content of the list which is populated when words from a file are loaded.
 * \param word  String of word.
 * \param length Length of word.
 * \param u_flag Used Flag (0 or 1).
 * \param index Index of the array (of pointer) of this struct.
*/
struct file_words_s {
    char word[maxchar];
    int length;
    int u_flag;
    int index;
};
typedef struct file_words_s file_words_t;

#endif
