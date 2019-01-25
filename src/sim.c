/*
 * Cache Simuladorea
 * Simuladorearen exekuzio nagusia
 *
 * 2017ko urria
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"
#include "defines.h"


/* Shellaren bufferrak */
char line[MAX_BUFFER];
char command_buffer[COMMAND_BUFFER][COMMAND_BUFFER_CHARS];

/* Memoria-simuladorearen datuak */
Memory mem_info;
Directory *dir_info;
int T_total = 0;
int T_file = 0;
int T_file_hit = 0;
int T_file_fail = 0;

int main(void)
{
	printf(ANSI_COLOR_GREEN "><><><><><><><><><><><><><><\n");
	printf("><>< Cache  Simuladorea <><>\n");
	printf("<><><><><><><><><><><><><><>\n\n" ANSI_COLOR_RESET);
	printf("2017 Konputagailuen Arkitektura\n");
	printf("Aritz Olea & Julen Merino\n\n");

	reset_simulator();

	printf( "Aginduak ikusteko idatzi 'help'\n" );
	while (1) {
		// Prompt-a
		printf(ANSI_COLOR_MAGENTA "CacheSim> " ANSI_COLOR_RESET);
		fflush(NULL); // sarrera-buffera hustu

		// Irakurri agindua
		if (!fgets(line, MAX_BUFFER, stdin))
			return 0;
			
 		get_command(line);
		execute_command();
    clean_cmdbuffer();

	}
	return 0;
}
