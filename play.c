/** \brief Game Module
 * @file play.c
*/

#include <stdio.h>
#include <stdlib.h>  // needed for function exit, dynamic memory mgmt
#include <ctype.h>  //toupper
#include <string.h> //memset & strupr
#include <errno.h>

#include "hangman.h" //for file_words_t 
#include "play.h"
#include "file_io.h"
#include "log.h"

/**Fonts colors. */
#define FRED        "\033[1;31m"
#define FGREEN      "\033[1;32m"
#define FYELLOW     "\033[0;33m"
#define NONE        "\033[0m" //default
// Escape is: \033 , Color code is: [0;31m
//https://stackoverflow.com/questions/5412761/using-colors-with-printf

/** Declaration of enum boolean and also definitions of three variables of this type.  */
enum boolean_e {F, T} bool_match, bool_used, bool_faulty_word;

/** Function for game started on command line. Checks word from command line or handles string errors (and exits game).
* \param word Word provided on command line.
* \param list Pointer to array of struct to save details of the word provided.
* \return 0 on success, 1 on error.
*/
int cl_word(char *word, file_words_t list[]) {
    int i = 0;
    while (word[i] != '\0') {
        if (isalpha(word[i]) == 0) {
            fprintf(stderr, "ERROR: Could not read string!\n");
            exit(EXIT_FAILURE);
        };
        i++;
    }
    if (strlen(word) >= (maxchar)) {
        printf("Invalid number of characters (max. %i)!\n", (maxchar-1));
        exit(EXIT_FAILURE);
    }

    list[0].length = strlen(word);
    strcpy(list[0].word, word);
    list[0].index = 0;
    printf("You provided the word: ");
    for (unsigned int j = 0; j< strlen(word); j++) {
        printf(FYELLOW"%c"NONE, toupper(word[j]));
    }
    printf("... ");
    return EXIT_SUCCESS;

}

/** Function for simple game. Parses word from \c stdin or handles string errors (and exits game).
* \param list Pointer to array of struct to save details of the word provided.
* \return 0 on success, 1 on error.
*/
int get_word(file_words_t list[]) {
    char word[maxchar];
    printf("Please provide a word to 'guess' (max. %d characters): ", maxchar-1);
    int ret = scanf("%s", word); //&-operator not needed in case of strings
    
    // Check if conversion was successful
    int i = 0;
    bool_faulty_word = F;
    while (word[i] != '\0') {
        if ((isalpha(word[i]) == 0) || (strlen(word) >= maxchar)) {
            bool_faulty_word = T;
            break;
        };
        i++;
    }
    while ((getchar() != '\n')&&(getchar() != EOF));  //delete input buffer
    if ((ret==1) && (bool_faulty_word == F)) {
        list[0].length = strlen(word);
        strcpy(list[0].word, word);
        list[0].index = 0;
        printf("You provided the word: ");
        for (unsigned int j = 0; j<strlen(word); j++) {
            printf(FYELLOW"%c"NONE, toupper(word[j]));
        }
        printf("... ");
    } else {
        fprintf(stderr, "ERROR: Could not read word! Take care to use characters only (a-z, A-Z) \n");
        fprintf(stderr, "and mind the maximum number (max. %d characters).\n\n", maxchar-1);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/** Function finds words from file which are not used already or returns 0 and exits game.
* \param nwords Number of words found in file.
* \param list Pointer to array of struct to check for the used flag.
* \return "Reversed logic." <c> Number </c>(equal or greater as 1) on success, 0 on error.
*/
int find_unused_words(int nwords, file_words_t list[], char *filename) {
    printf("Trying to find unused words... ");
    int j = 0;
    for (int i = 0; i<nwords; i++) {
        if (list[i].u_flag == 0)
            j++;
    }
    if (j == 0) {
        fprintf(stdout, "\nWARNING: All words in '%s' already used. Please define new ones and start program again or reset flags.\n\n", filename);
        return EXIT_SUCCESS;
    }
    printf("%i unused word(s) found!\n", j);
    return j;
}

/** <b> Important game function </b> where actual guessing takes place.
 * Also logging is called by this function.
 * \param word Words which has to be guessed by user.
 * \param length Length of word which is guessed.
 * \return 0 on success, 1 on error.
 *
 *  */
int play(char word[], int length) {
    errno = 0;
    printf(FGREEN"the game is on!\n\n"NONE);
    log_user_input(lfile, -1, '0', '\0');
    char *guessed = malloc((length+1) * sizeof(char)); //sizeof(char) * (length+1));  //length+1
    if (guessed == NULL) { 
        fprintf (stderr, "ERROR: Memory allocation not successful.\n");
        return EXIT_FAILURE;
    }
    for (int j = 0; j<length; j++) {
        guessed[j] = '?';       //if character is changed here it also has to be changed for right guess check! See/Search for "strstr(guessed"
    }
    guessed[length] = '\0';
    for (int j = 0; j<length; j++) {
        word[j] = toupper(word[j]);
    }

    char ch = '0';
    int wcounter = 0;
    char uch[wguesses] = {'\0'}; //char uch[wguesses] = {" "};
    while (wcounter < wguesses) {
        bool_match = F;
        for (int j = 0; j<length; j++) {
            printf(FYELLOW"%c "NONE, guessed[j]);
        }
        printf("\n");

        printf("Please enter character (a-z, A-Z): ");
        //getchar();
        ch = toupper(getchar());
        
        log_user_input(lfile, 0, ch, guessed);  //error handling in function, errors shall not interfere game experience
        while ((getchar() != '\n')&&(getchar() != EOF));  //throwing the input away until it finds a new line character or EOF
        if (!isalpha(ch)){
            printf("No valid character!\n\n");
            continue;
        }
        for (int j = 0; j<length; j++) {  //find out if there is one or more matches in string and set bool to true
            if (word[j] == ch) {
                guessed[j] = word[j];
                bool_match = T;
            }  
        }
        if (bool_match == T) {
            printf("Match!\n");
            display_guesses(wcounter, uch);

            if (strstr(guessed, "?") == NULL) {
                printf(FGREEN"Well done! You correctly guessed the word \"%s\"!\n"NONE, word);
                printf("You needed %i wrong guess(es).\n\n\n", wcounter);
                
                log_user_input(lfile, 1, 'y', word); //error handling in function
                if (guessed) {
                    free(guessed);
                    guessed = NULL;
                }
                return EXIT_SUCCESS;
            }
        } else {
            bool_used = F;
            for (int j = 0; j<wcounter; j++) {
                if (toupper(uch[j]) == ch) {
                    printf("You already guessed the letter '%c'.\n", ch);
                    bool_used = T;
                    break;
                }
            } 
            if (bool_used == F) { 
                uch[wcounter] = ch;
                wcounter++;
            }
            printf("No match.\n");
            display_guesses(wcounter, uch);
        }
        
    } //end while
    printf(FRED"Sorry, you did not manage to correctly guess the word \"%s\".\n\n"NONE, word);
    
    log_user_input(lfile, 1, 'n', word); //error handling in function
    if (guessed) {
        free(guessed);
        guessed = NULL;
    }
    return EXIT_SUCCESS;
}

/** Function prints information about no of guesses and wrongly guessed characters to \c stdout. 
 * \param wcounter Number of already wrongly guessed character in this game.
 * \param uch Array of already used characters.
 */
void display_guesses(int wcounter, char *uch) {
    printf("Guesses left: %i. ", (wguesses-wcounter));
    if (wcounter) {  //if > 0
        printf("Already wrong guessed characters: ");
        for (int j = 0; j<wcounter; j++) {
            printf("%c ", uch[j]);
        } 
    } 
    printf("\n\n");
}

/** Function delivers random integer between \c pmin and \c pmax.
 * \param pmin Minimum number for random int.
 * \param pmax Maximum number for random int.
 * \return <c> Number </c>.
 */
int rand_no(int *pmin, int *pmax) {
    //return (rand() % ((*pmax-*pmin)+1)+*pmin); 
    return (int)(*pmin + (*pmax * (double)rand()/RAND_MAX)); //(RAND_MAX + 1.0)
}
