/*
 * Cache Simuladorea
 * Aginduen bideraketa eta tratamendua
 *
 * 2017ko urria
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "functions.h"
#include "defines.h"
#include "extern.h"





/* Aginduen bilaketa */
int get_command(char *cmd) {
	char *found;
	int cb_i = 0;
	int len,i;

	while((found = strsep(&cmd," ")) != NULL ) {
		// Garbitu string-a, new line kendu
		char *p = strchr(found, '\n');
		if (p != NULL) *p = '\0';

		for (i=0; found[i]; i++)
		    found[i] = tolower((unsigned char)found[i]);

		// Agindu-bufferrean gorde
		strcpy(command_buffer[cb_i], found);
		cb_i++;

		// Agindu-bufferra bete da
		if (cb_i == COMMAND_BUFFER) break;
	}

}

void clean_cmdbuffer(void) {
    int i;
    for (i=0;i<COMMAND_BUFFER;i++)
        command_buffer[i][0] = '\0';
}

/* Aginduen exekuzioa */
int execute_command(void) {

	if (strcmp(command_buffer[0], "help") == 0)
			show_help();
    else if (strcmp(command_buffer[0], "") == 0)
			return 0;
	else if (!strcmp(command_buffer[0], "exit") || !strcmp(command_buffer[0], "quit") )
			exit(EXIT_SUCCESS);
	else if (strcmp(command_buffer[0], "wr") == 0) {
      if (command_buffer[1][0] == '\0') return 0;
			cache_store(atoi(command_buffer[1]), 1, stdout);
    } else if (strcmp(command_buffer[0], "rd") == 0) {
      if (command_buffer[1][0] == '\0') return 0;
			cache_load(atoi(command_buffer[1]), stdout);
    } else if (strcmp(command_buffer[0], "word") == 0) {
      if (command_buffer[1][0] == '\0') return 0;
			cache_word(atoi(command_buffer[1]));
    } else if (strcmp(command_buffer[0], "block") == 0) {
      if (command_buffer[1][0] == '\0') return 0;
			cache_block(atoi(command_buffer[1]));
    } else if (strcmp(command_buffer[0], "set") == 0) {
      if (command_buffer[1][0] == '\0') return 0;
			cache_set(atoi(command_buffer[1]));
    } else if (strcmp(command_buffer[0], "replacement") == 0) {
      if (command_buffer[1][0] == '\0') return 0;
			cache_replacement(command_buffer[1]);
    } else if (strcmp(command_buffer[0], "status") == 0)
		{
        show_memory();
				show_directory();
		}
		else if (strcmp(command_buffer[0], "reset") == 0)
		{
        reset_simulator();
        printf("Simuladorea reseteatu da.\n");
		}
		else if (strcmp(command_buffer[0], "source") == 0)
				file_load(command_buffer[1]);
		
		else
        printf("Agindu ezezaguna: %s\n", command_buffer[0]);

}



/* Aginduak */
void show_help(void) {
	printf("--------------------- Cache memoriaren simuladorea ---------------------\n\n");
	printf("reset \t\t\t\t\tsimuladorea hasieratu\n");
	printf("rd <helbidea> \t\t\t\thelbideko datua kargatu\n");
	printf("wr <helbidea> \t\t\t\thelbidean datua gorde\n");
	printf("status \t\t\t\t\tcachearen egoera ikusi\n");
	printf("word <tamaina> \t\t\t\thitzaren tamaina definitu (byte)\n");
	printf("block <tamaina> \t\t\tblokeen tamaina (byte) definitu\n");
	printf("set <kopurua> \t\t\t\tmultzoen tamaina (1,2,4,8) definitu\n");
	printf("replacement <fifo, lru> \t\tordezkapen-politika definitu\n");
	printf("help\t\t\t\t\tlaguntza hau erakutsi\n");
	printf("exit\t\t\t\t\tsimulatzailetik irten\n\n");
}
