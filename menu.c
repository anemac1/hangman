/** \brief Menu Module
 * 
 * @file menu.c
*/

#include <stdio.h>
#include <stdlib.h> // needed for function exit, dynamic memory mgmt
#include <ctype.h> // needed for function toupper
#include <string.h>

#include "hangman.h"
#include "menu.h"
#include "play.h"  //for list
#include "file_io.h"  //for list



/** Variable for input file used in almost all functions of this module. */
char ifile[maxchar];

/** Function delivers list of menu points on \c stdout. It starts with initialization of struct \c mmenu. */
void show_menu() {
	menu_t mmenu[] = {  //changes here have to be handled in while loop/switch in function parse_input!
		{'1', "Instructions"},
		{'2', "Play stupid (Word entered)"},
		{'3', "Play smart (Word from file)"},
		{'8', "Set input file"},
		{'9', "Reset used flag in file"},
		{'M', "Show Menu"},
		{'X', "Exit Program"}};

    int nr_menu_points = sizeof(mmenu)/sizeof(menu_t);
    printf("Please choose from the following options\n");
    printf("----------------------------------------\n");
    for (int i = 0; i<nr_menu_points; i++) {
        printf("\t(%c) %s\n", mmenu[i].symbol, mmenu[i].text);
    }
    printf("----------------------------------------\n");
	int padlen = (40-strlen(ifile))/2;
	printf("%*s%s%-*s\n", padlen, "--- ", ifile, padlen, " ---");
}

/** Function parses user input (character) on \c stdin while program is on the menu. Uses this input to switch options and calls other functions.
* \return 0 on success.
*/
int parse_input() {
	
	strcpy (ifile, dfile);
	show_menu();
	char input = 0;
	
	while (1) {
		printf("> ");
		input = getchar();
		while ((getchar() != '\n')&&(getchar() != EOF));  //delete input buffer
		switch (toupper(input)) {  //Switch for options in menu
		case '1':
			printf("Your choice: #%c\n", input);
			show_instr();
			break;
		case '2': 
			printf("Your choice: #%c\n", input);
			file_words_t list_short[1] = {0};
			if (get_word(list_short) != 0) {
				continue;
			};
			if (play(list_short[0].word, list_short[0].length) != 0) {
				fprintf(stderr, "ERROR: Unknown problem encountered.\n");
				continue;
			};
			break;
		case '3': 
			printf("Your choice: #%c\n", input);
			file_words_t list[maxwords] = {0};

			int nwords = 0;
			if ((nwords = (file_read(ifile, list)))) {
				printf("%i word(s) loaded.\n", nwords);
			} else {
				fprintf(stderr, "ERROR: Loading of words from %s not successful. Possibly check naming or format.\n", ifile);
				continue;
			}

			if (find_unused_words(nwords, list, ifile)) {
				printf("Choosing unused word randomly... ");
				//randomizer
				int random_num = -1;
				int min = 0;
				do { 			//get element, which lies between 0 and max words but only if not already used
					random_num = rand_no(&min, &nwords);
					#if D
						printf("Nr. %i  ",random_num);
						printf("Flag %i\n",list[random_num].u_flag);
					#endif
				} while (list[random_num].u_flag != 0);
				#if D
					printf("%s\n",list[random_num].word);
				#endif 
				if (play(list[random_num].word, list[random_num].length) != 0) {
					fprintf(stderr, "ERROR: Unknown problem encountered.\n");
					continue;
				};
				file_write(ifile, list, random_num); //Error handling in function
			}
			break;
		case '8': 
			printf("Your choice: #%c\n", input);
			set_file();
			break;
		case '9': 
			printf("Your choice: #%c\n", input);
			word_reset(ifile);
			break;
		case 'M':
            show_menu();
            break;
		case 'X':
			printf("Closing program... Thanks for playing!\n\n");
            return EXIT_SUCCESS;
		default:
			printf("Please use ""'M'"" (for menu) to show viable options.\n");
			break;
		}
	} //end while
}

/** Function delivers instructions on \c stdout. */
void show_instr() {
    printf("------------- Instructions -------------\n");
	printf("The game 'Hangman' is a word guessing game (usually for two players). One player thinks\nof a word and the other tries to guess it by ");
	printf("suggesting letters within a certain number of guesses. \nIf the word is not guessed the player loses.\nIn this program two options are implemented.\n");
	printf("You can either type in the word you want to 'guess' or a random word out of a file needs \nto be guessed by the player.\n");
    printf("\n(Press 'M' to go back to menu.)\n----------------------------------------\n");
}

/** Function to set different file name from where words are imported.
 * \return 0 on success.
 */
int set_file() {
	printf("Please provide file name (max. %d characters):\n", 25);
	int ret = scanf("%s", ifile);
	if (strlen(ifile) >= (maxchar)){
		printf("Invalid number of characters (max. %i)! Setting to default...\n\n", (maxchar-1));
        strcpy(ifile, dfile);
	} else if (ret==1) {
		printf("File name set to: %s\n\n", ifile);
	} else {
		printf("Could not convert string!  Setting to default...\n\n");
		strcpy(ifile, dfile);
	}
	while ((getchar() != '\n')&&(getchar() != EOF));  //delete input buffer
return EXIT_SUCCESS;
}
