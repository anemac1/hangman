/** @file hangman.c
 * \brief Program starting module.
 * 
 * Main calls either function to show menu version or function to proceed with the command line version.
 * 
*/

#include <stdio.h>
#include <stdlib.h> // needed for function exit, dynamic memory mgmt
#include <time.h>
#include <getopt.h>
//for CL-Version
#include <string.h>

#include "hangman.h"
#include "menu.h"
//for CL-Version
#include "play.h"


/** Program run with menu - if special argument on start via command line is provided.
 * \return 0 on success.
 */
int main_m(void) {
    printf("\nProgram with menu starting...\n");
    srand(time(NULL)); // plant rand seed
	parse_input();
    return EXIT_SUCCESS;
} // end main

/** Program run with word from command line - if argument <-w word> is provided.
 * \param word The word which is provided by the user in the command line. 
 * \return 0 on success.
 */
int main_cl(char *word){
    printf("\nProgram starting...\n");
    file_words_t list_short[1] = {0};
    
    if (cl_word(word, list_short)) {
        printf("ERROR: Unknown problem in getting the word encountered.\n");
        return EXIT_FAILURE;
    };

    if (play(list_short[0].word, list_short[0].length) != 0) {
        printf("ERROR: Unknown problem in guessing function encountered.\n");
        return EXIT_FAILURE;
    };
    return EXIT_SUCCESS;
}

/** Hangman starting function.
 *
 * Special argument brings program into the advanced mode (with menu).
 * \param argc  Argument counter.
 * \param argv  Argument vector. Only one argument needed/possible which is \c <word> or \c <\--m> for starting advanced (menu) version.
 * \return 0 on success, 1 on error.
 *
*/
int main(int argc, char *argv[]) {
    // parse code from command line
    /* if (argc != 2) {
        const char *exename = strrchr(argv[0], '\\');
        if (exename) {   //correct display of usage works with gcc/command line
            printf("USAGE: %s <-w word>, or add <-m> to start program with menu \n", ++exename);  //vs code: if absolute path is found only print name of the file
        } else {
            printf("USAGE: %s <-w word>, or add <-m> to start program with menu \n", argv[0]);
        }
        exit(EXIT_SUCCESS);
    } */
    
    int option;
    opterr = 0;
    while ((option = getopt(argc, argv, "mw:")) != -1) {
        switch (option)
        {
        case 'm':
            main_m();
            exit(EXIT_SUCCESS);
        case 'w':
            
            /* {int j = 0;
            while (optarg[j] == ' ') {
                j++;
            }
            if (j != 0) {
                int i = 0;
                while (optarg[i] != '\0') {
                    optarg[i] = optarg[i+1];
                    i++;
                }
            } */

            main_cl(optarg);
            exit(EXIT_SUCCESS);//}
        case '?':
            if (optopt == 'w') {
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            } else {
                fprintf (stderr, "Unknown Option\n");
            }
            break;
        /* case ':':
            fprintf(stderr, "Missing argument.\n");
            break; */
        default:
            fprintf (stderr, "Unknown Option\n");
            //exit(EXIT_SUCCESS);
        }
    }
    printf("USAGE: ./hangman <-w word>, or add <-m> to start program with menu \n");
    return EXIT_SUCCESS;

    /* //without getopt
    if (strncmp(argv[1],"--m",2) == 0) { // parse word if game with menu
        main_m();
        exit(EXIT_SUCCESS);
    } else {
        #if D
            printf("\nProgram with command line input starting...\n");
            printf("argv[%d] = \"%s\"\n", 1, argv[1]);
        #endif
        main_cl(argv[1]);
        exit(EXIT_SUCCESS);
    }
    //without getopt */

} // end cl_main

