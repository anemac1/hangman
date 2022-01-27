/** \brief Logging Module
 * 
 * @file log.c
 * 
 * Module includes the function to log the status of the game to a file which is called by the play function.
*/

#include <stdio.h>
#include <stdlib.h>  // needed for function exit, dynamic memory mgmt
#include <ctype.h>  //toupper
#include <string.h>
#include <errno.h>
#include <time.h>

#include "hangman.h" //for D
#include "log.h"

/** Function gets time from OS and logs user input in program-defined logfile (log.txt).
 * 
 * \param filename Name of logfile.
 * \param switcher Parameter defines, if start \c (-1), end \c (+1) or body sequence \c (0) is written to logfile.
 * The last two parameters are only needed in the body sequence and ignored otherwise.
 * \param ch Typed-in character is written to logfile.
 * \param word Guessed word is written to logfile.
 * \return 0 on success, 1 on error.
 *
 */
int log_user_input(char *filename, int switcher, char ch, char *word) {
    errno = 0;
    FILE *fp;
    if ((fp = fopen(filename, "a"))){
        //https://www.tutorialspoint.com/c_standard_library/c_function_strftime.htm
        time_t now = time(0); //Get system time
        char buffer[25];
        strftime(buffer,25,"%d.%m.%Y - %X", localtime(&now));
        #if D
            printf("%s\n", buffer);
        #endif

        switch (switcher) {
        case -1:
            fprintf(fp, "Game started at   <%s>\n", buffer);
            break;
        case 0:
        	fprintf(fp, "Word status: %s\n", word);
        	fprintf(fp, "User input '%c' at <%s>\n", ch, buffer);
            break;
        case 1:
            fprintf(fp, "Game ended at     <%s>\n", buffer);
            if (ch == 'y') {
                fprintf(fp, "User won! Word: %s\n", word);
            } else {
                fprintf(fp, "User lost! Word: %s\n", word);
            }
            int i = 0;
            while (i < 41) {
                fprintf(fp, "=");
                i++;
            }
            fprintf(fp, "\n");
            break;
        default:
            break;
        }

        if (fflush(fp) != 0) {
            perror("ERROR fflush");
            fprintf(stderr,"ERROR: Writing buffer was not successful.\n");
            return EXIT_FAILURE;  
        }
        if (fclose(fp) != 0) {
            perror("ERROR fclose");
            fprintf(stderr,"ERROR: Closing file was not successful.\n");
            return EXIT_FAILURE;
        }
        return (EXIT_SUCCESS);
    } else {
        fprintf(stderr, "ERROR: Opening '%s' not successful. %s\n", filename, strerror(errno));
        return (EXIT_FAILURE);
    }
}
