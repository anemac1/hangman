/** \brief File Input_Output Module.
 *
 * @file file_io.c
 *
 * This module handles all the filehandles if words are guessed from file. Logging is a separate module.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>  //toupper
#include <string.h>
#include <errno.h>

#include "hangman.h" //for debug level, for file_words_t
#include "file_io.h"


/** Sets used flag for all words to '0' (unused) and converts words to uppercase.
*
* \param filename  The file to be opened.
* \return 0 on success, 1 on error.
*/
int word_reset (char *filename) {
    errno = 0;
    FILE *fp;
    if ((fp = fopen(filename, "r+"))) {  //fopen function returns a FILE pointer, so true
        char string[maxchar+2]; //+2 for including blank and used flag
        int j = 0;
        while((fgets(string, sizeof(string), fp) != NULL) ) {
            for (unsigned int i = 0; i<(strlen(string)-1); i++) {
                string[i] = toupper(string[i]);
            }
            #if D
                printf("strlen: %d\n", strlen(string));
                printf("string: %s\n", string);
            #endif
            int i = 0;
            while ((string[i] != '0') && (string[i] != '1')) {
            	i++;
            }
            string[i] = '0';
            #if D
                printf("string: %s\n", string);
            #endif
            if (fseek(fp, -(strlen(string)), SEEK_CUR) != 0) {
                fprintf(stderr,"ERROR: Finding position to set word to 'not used' in file.\n");
                return EXIT_FAILURE;
            }
            #if D
                printf("Position: %d\n", ftell(fp));
            #endif
            if (fprintf(fp, "%s", string) < 0) {
                fprintf(stderr,"ERROR: Setting words to 'not used' in file was not successful.\n");
                return EXIT_FAILURE;
            }
            if (fflush(fp) != 0) { 
                perror("ERROR fflush");
                fprintf(stderr,"ERROR: Writing buffer was not successful.\n"); 
                return EXIT_FAILURE;  
            }
            if (j>=maxwords) {
                break;
            }
            j++;
        } //end while
        if (fclose(fp) != 0) {
            perror("ERROR fclose");
            fprintf(stderr,"ERROR: Closing file was not successful.\n");
            return EXIT_FAILURE;
        }
        printf("Reset finished: %d word(s) updated.\n", j);
    } else {
        fprintf(stderr, "ERROR: Opening '%s' not successful. %s\n", filename, strerror(errno));
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/** Writing information of used flag to file.
*
* Errors handled explicitly with \c errno and generates a warning on \p stderr.
* \param filename  The file to be opened.
* \param list  The struct list where details about the word are taken from.
* \param index  To check whether any error has occurred known index from list vs. place in the file is checked.
* \return 0 on success, 1 on error.
*/
int file_write (char *filename, file_words_t list[], int index) {
    errno = 0;
    FILE *fp;
    if ((fp = fopen(filename, "r+"))) {  //fopen function returns a FILE pointer, so true
        char string[maxchar+2]; //+2 for including blank and used flag
        int j = 0;
        while((fgets(string, sizeof(string), fp) != NULL) ) {
            for (int i = 0; i<list[j].length; i++) {
                string[i] = toupper(string[i]);
            }
            if ((strstr(string, list[index].word)) != NULL) {
                if (index == j) { //second check if correct line (Comparison with internal index)
                    #if D
                        printf("strlen: %d\n", strlen(string));
                        printf("string: %s\n", string);
                    #endif
                    int i = 0;
                    while (string[i] != '0') {
                        i++;
                    }
                    string[i] = '1';
                    #if D
                        printf("string: %s\n", string);
                    #endif
                    if (fseek(fp, -(strlen(string)), SEEK_CUR) != 0) {
                        fprintf(stderr,"ERROR: Finding position to set word to 'used' in file.\n");
                        return EXIT_FAILURE;
                    }
                    if (fprintf(fp, "%s", string) < 0) {
                        fprintf(stderr,"ERROR: Setting word to 'used' in file was not successful.\n"); 
                        return EXIT_FAILURE;
                    };
                    if (fflush(fp) != 0) {
                        perror("ERROR fflush");
                        fprintf(stderr,"ERROR: Writing buffer was not successful.\n"); 
                        return EXIT_FAILURE;  
                    }
                    break;
                }
                fprintf(stderr,"ERROR: Internal index and place in file do not match.\n"); 
                return EXIT_FAILURE;
            } 
            j++;
        } //end while
        
        if (fclose(fp) != 0) {
            perror("ERROR fclose");
            fprintf(stderr,"ERROR: Closing file was not successful.\n");
            return EXIT_FAILURE;
        }
        #if D
            printf("Used flag in '%s' updated.\n", filename);
        #endif
        return EXIT_SUCCESS;
    } else {
        fprintf(stderr, "ERROR: Opening '%s' not successful. %s\n", filename, strerror(errno));
        return EXIT_FAILURE;
    }
}


/** Reading information from file and writing to list.
*
* Errors handled explicitly with \c errno and generates a warning on \p stderr.
* \param filename  The file to be opened.
* \param list  The struct list where to be written.
* \return  "Reverse logic" in return. <tt>number_of_words</tt> on success, 0 on error.
*/
int file_read (char *filename, file_words_t list[]) {
    errno = 0;
    FILE *fp;
    if ((fp = fopen(filename, "r"))) {  //fopen function returns a FILE pointer, so true
        char string[maxchar+2];  //including blank and used flag
        int counter = 0;
        
        while (fgets(string, sizeof(string), fp) != NULL) {
            file_words_t *elem = malloc(sizeof(file_words_t));  //pointer to start of struct necessary, as malloc has return value pointer
            if (elem == NULL) {  
                fprintf(stderr, "ERROR: Memory allocation not successful.\n");
                return 0;
            }
            #if D
                printf("\n");
                printf("add: %p\n", &list[counter]);
            #endif
            char temp[sizeof(string)] = {'\0'};
            int tempi = -1;
            sscanf(string, "%s %d", &temp[0], &tempi); // address of first character in string
            elem->length = strlen(temp);
            strcpy(elem->word, temp);
            elem->u_flag = tempi;
            elem->index = counter;

            #if D
                
                printf("i: %i ", (*elem).index);
                printf("word: %s ", (*elem).word);
                printf("length: %d ", elem->length);
                printf("used: %d ", elem->u_flag);
            #endif
            list[counter] = *elem;
            counter++;
            free_struct(elem);
        } 
        if (fflush(fp) != 0) {
            perror("ERROR fflush");
            fprintf(stderr,"ERROR: Writing buffer was not successful.\n");
            return 0;  
        }
        if (fclose(fp) != 0) {
            perror("ERROR fclose");
            fprintf(stderr,"ERROR: Closing file was not successful.\n");
            return 0;
        }
		#if D
		for (int i = 0; i<counter;i++){
			printf("\n");
			printf("i: %i ", list[i].index);
			printf("word: %s ", list[i].word);
			printf("length: %d ", list[i].length);
			printf("used: %d ", list[i].u_flag);
		}
		#endif

        return counter;
    } else {
        //fopen:  Otherwise, NULL is returned and the global variable errno is set to indicate the error.
        fprintf(stderr, "ERROR: Opening '%s' not successful. %s.\n", filename, strerror(errno));
        return 0;
    }
}


/** Free memory after writing to list of words.
*
* \param elem  Pointer to element of struct.
*/
void free_struct(file_words_t *elem) {
    if (elem) {
        free(elem);
        elem = NULL;
    }
}
