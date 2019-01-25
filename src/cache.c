/*
 * Cache Simuladorea
 * Cachearen funtzioen Fitxategia
 *
 * 2017ko urria
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "extern.h"


/*
 * Cache memoriaren funtzioak eta hasieraketak
 */


// Direktorioa berralokatu
void realloc_directory(void) {
	int i;

	if (dir_info != NULL) free(dir_info);
	dir_info = malloc(mem_info.cache_size * sizeof(Directory));

	for (i=0;i<mem_info.cache_size;i++)
	{
		    dir_info[i].busy = 0;
		    dir_info[i].changed = 0;
		    dir_info[i].tag = 0;
		    dir_info[i].replaced = 0;
		    dir_info[i].block = -1;
            dir_info[i].lru = 0;
            dir_info[i].fifo = 0;
	}
}

// Simuladorearen hasieraketa
void reset_simulator(void) {
	mem_info.cache_size = 8; // Cachea 8 blokekoa
	mem_info.word_size = 4; // Hitzak 4 byte
	mem_info.block_size = 32; // Blokeak 32 byte
	mem_info.set_size = 2; // Multzoen tamaina
	mem_info.replacement = LRU; // LRU ordezkapen-politika
    mem_info.last_lru = -1; // LRU 0tik hasten da

	// Direktorioa reseteatu
	realloc_directory();
}

void show_memory(void) {
	char *replacement = (mem_info.replacement == LRU) ? "LRU" : "FIFO";
	printf("====> Memoriaren informazioa <====\n\n");
	printf("Cache tamaina: \t\t%d bloke (%d byte)\n", mem_info.cache_size, (mem_info.cache_size*mem_info.block_size));
	printf("Hitz tamaina: \t\t%d byte\n", mem_info.word_size);
	printf("Bloke tamaina: \t\t%d byte\n", mem_info.block_size);
	printf("Multzo tamaina: \t%d bloke\n", mem_info.set_size);
	printf("Ordezkapena: \t\t%s\n\n", replacement);


}

void show_directory(void) {
	int i;
	printf("====> Direktorioaren egoera <====\n\n");

	if (mem_info.replacement == FIFO)
		printf("FIFO  ");
	else
		printf("LRU  ");

	printf("  okup  ald  tag  ord  ||  blokea\n");

	printf("----------------------------------------\n");
	for (i=0;i<mem_info.cache_size;i++) {
		// Ordezkatze-politika idatzi
		if (mem_info.replacement == LRU)
			printf(" %d  ", dir_info[i].lru);
		else
			printf(" %d  ", dir_info[i].fifo);

		printf("   %d      %d    %d    %d  ||", dir_info[i].busy, dir_info[i].changed, dir_info[i].tag, dir_info[i].replaced);

		if (dir_info[i].block == -1) // hutsa dago
			printf("  ---\n");
		else {
			printf("  b%d \n", dir_info[i].block);
		}

		if (((i+1)%mem_info.set_size) == 0)
		printf("----------------------------------------\n");

 	}

}

void fifo_update(int start, int end) {
    int i;
    for (i=start; i<end; i++)
        if (dir_info[i].busy == 1)
            dir_info[i].fifo++;
}

int find_fifo(int start, int end) {
    int i, replace_no = 0, replace_block = 0;

    for (i=start; i<end; i++) {
        if (dir_info[i].busy == 1) {
                if (replace_no < dir_info[i].fifo) {
                        replace_no = dir_info[i].fifo;
                        replace_block = i;
                }
        }
    }
    return replace_block;

}

int find_lru(int start, int end) {
    int i, lru, replace_no = 0, replace_block = 0;

    for (lru=0; lru<=mem_info.last_lru; lru++)
        for (i=start; i<end; i++) {
            if (dir_info[i].busy == 1) {
                    if (dir_info[i].lru == lru) {
                            return i;
                    }
            }
        }
}

void dir_update(int i, int block, int tag, int changed, int replaced) {
    dir_info[i].busy = 1;
    dir_info[i].changed = changed;
    dir_info[i].tag = tag;
    dir_info[i].replaced = replaced;
    dir_info[i].block = block;
    dir_info[i].fifo = -1;
    dir_info[i].lru = mem_info.last_lru+1;

}


void cache_store(int address, int block_has_changed, FILE *stream) {
    int i, status = 1;
    int cur_word = (int)(address/mem_info.word_size);
    int cur_block = (int)(cur_word/(mem_info.block_size/mem_info.word_size));
    int cur_block_word = (int)(cur_word%(mem_info.block_size/mem_info.word_size));
    int cur_tag = (int)cur_block/(mem_info.cache_size/mem_info.set_size);
    int cur_set_no = (int)cur_block%(mem_info.cache_size/mem_info.set_size);
    int cur_block_start = (int)cur_block*(mem_info.block_size/mem_info.word_size);
    int block_size = (int)(mem_info.block_size/mem_info.word_size);

    fprintf(stream, "Helbidea: \t@%d\nHitza: \t\t%d\nBlokea: \t%d (%d-%d)\nTag: \t\t%d\nMultzoa: \t%d\n", address, cur_word, cur_block, cur_block_start, (cur_block_start+(mem_info.block_size/mem_info.word_size)-1), cur_tag, cur_set_no);



    // Multzoa kalkulatu
    int block_start, block_end, replace_block = 0;
    block_start = mem_info.set_size*cur_set_no;
    block_end = block_start + mem_info.set_size;
    printf("Block %d-%d set %d\n",block_start, block_end, cur_set_no);

    T_total = 0;
    while (status != 0) {


        for (i=block_start; i<block_end; i++) {
            if (status == 1) // 1. Saiatu blokea idazten (berridatzi)
                if (dir_info[i].block == cur_block)
                {
                    dir_update(i, cur_block, cur_tag, block_has_changed, 1);
                    mem_info.last_lru++;
                    fifo_update(block_start, block_end);
                    status = -1;
                    fprintf(stream, ANSI_COLOR_GREEN "\nASMATZEA" ANSI_COLOR_RESET);
                    fprintf(stream, ", Denborak:\n");
                    fprintf(stream, "\tKatxean eragiketa: %d\n", CACHE_TIME);
                    T_total = CACHE_TIME;
										if (stream != stdout) {
											T_file += T_total;
											T_file_hit++;
										}
                    break;

                }
            if (status == 2) // 2. Saiatu leku hutsean idazten
                if (dir_info[i].busy == 0) {
                    dir_update(i, cur_block, cur_tag, block_has_changed, 0);
                    mem_info.last_lru++;
                    fifo_update(block_start, block_end);
                    status = -1;
                    fprintf(stream, ANSI_COLOR_RED "\nHUTSEGITEA" ANSI_COLOR_RESET);
                    fprintf(stream, ", Denborak:\n\tKatxean bilatzea: %d ziklo\n\tMemoriako transferentzia: %d+%d ziklo\n", CACHE_TIME, MEMORY_TIME, block_size);
                    T_total = CACHE_TIME + MEMORY_TIME + block_size;
										if (stream != stdout) {
											T_file += T_total;
											T_file_fail++;
										}
                    break;
                }
            if (status == 3) // 3. LRU edo FIFOren arabera aldatu
                {
                    // Blokea aukeratu
                    if (mem_info.replacement == LRU)
                        replace_block = find_lru(block_start, block_end);
                    else if (mem_info.replacement == FIFO)
                        replace_block = find_fifo(block_start, block_end);

                    fprintf(stream, ANSI_COLOR_RED "HUTSEGITEA" ANSI_COLOR_RESET);
                    fprintf(stream, ", Denborak:\n\tKatxean bilatzea: %d ziklo\n", CACHE_TIME);

                    if (dir_info[i].changed) {
                         fprintf(stream, "\tMemoriako transferentzia: %d+%d ziklo\n", MEMORY_TIME, block_size);
                         T_total = MEMORY_TIME +block_size;
                    }

                    fprintf(stream, "\tMemoriako transferentzia: %d+%d ziklo\n", MEMORY_TIME, block_size);
                    T_total += MEMORY_TIME +block_size;

                    T_total += CACHE_TIME;
										if (stream != stdout) {
											T_file += T_total;
											T_file_fail++;
										}
                    // Blokearen informazioa berritu
                    dir_update(replace_block, cur_block, cur_tag, block_has_changed, 1);
                    mem_info.last_lru++;
                    fifo_update(block_start, block_end);
                    status = -1;


                    break;
            }
        }

        status++;
    }



    fprintf(stream, "Eragiketaren denbora: %d ziklo\n", T_total);

}

void cache_load(int address, FILE *stream) {
    int i, status = 1;
	int cur_word = (int)(address/mem_info.word_size);
	int cur_block = (int)(cur_word/(mem_info.block_size/mem_info.word_size));
	int cur_block_word = (int)(cur_word%(mem_info.block_size/mem_info.word_size));
	int cur_tag = (int)cur_block/(mem_info.cache_size/mem_info.set_size);
	int cur_set_no = (int)cur_block%(mem_info.cache_size/mem_info.set_size);
	int cur_block_start = (int)cur_block*(mem_info.block_size/mem_info.word_size);


   // Multzoa kalkulatu
  int block_start, block_end, replace_block = 0;
  block_start = mem_info.set_size*cur_set_no;
  block_end = block_start + mem_info.set_size;


    while (status != 0) {


        for (i=block_start; i<block_end; i++) {
            if (status == 1) // 1. Saiatu blokea irakurtzen
                if (dir_info[i].block == cur_block) {
                    status = -1;
                    fprintf(stream, "Helbidea: \t@%d\nHitza: \t\t%d\nBlokea: \t%d (%d-%d)\nTag: \t\t%d\nMultzoa: \t%d\n", address, cur_word, cur_block, cur_block_start, (cur_block_start+(mem_info.block_size/mem_info.word_size)-1), cur_tag, cur_set_no);
                    fprintf(stream, ANSI_COLOR_GREEN "\nASMATZEA" ANSI_COLOR_RESET);
                    fprintf(stream, ", Denborak:\n");
                    fprintf(stream, "\tKatxean irakurtzea: %d\n", CACHE_TIME);
                    T_total = CACHE_TIME;
										if (stream != stdout) {
											T_file += T_total;
											T_file_hit++;
										}
                    dir_info[i].lru = mem_info.last_lru+1;
                    mem_info.last_lru++;
                    
                    break;
                }
            if (status == 2) // 2. Blokea ekarri memoria nagusitik
            {
                status = -1;
                cache_store(address, 0, stream);
                break;
            }

        }
        status++;
    }

}


void file_load(char *file) {
	FILE * fp,*out_file;
	char command[COMMAND_BUFFER_CHARS];
	int address;

  if (file[0] == '\0') {
		printf("Fitxategiaren izena idatzi\n");
		return;
	}
  out_file = fopen("exec_output", "w+");
	fp = fopen (file, "r");
	if ((fp == NULL) || (out_file == NULL)) {
		printf(ANSI_COLOR_RED"Fitxategia ez da existitzen edo ez du baimenik\n"ANSI_COLOR_RESET);
		return;
	}
	printf( ANSI_COLOR_CYAN "Fitxategia exekutatzen\n"ANSI_COLOR_RESET);
	fprintf(out_file, "\n\n%s fitxategiren exekuzioa\n", file);
	fprintf(out_file, "---------------------------\n\n");

	T_file = 0;
	T_file_hit = 0;
	T_file_fail = 0;

	while(!feof(fp)) {
	   fscanf(fp, "%s %d[^\n]", command, &address);
		 if (!strcmp(command, "rd") || !strcmp(command, "wr")) {
		 	printf("\t%s %d\t\t", command, address);
			fprintf(out_file, "\t%s %d\t\t\n", command, address);

			if (!strcmp(command, "rd"))
				cache_load(address, out_file);
			else
				cache_store(address, 1, out_file);

			printf(ANSI_COLOR_GREEN "EXEKUTATUTA\n" ANSI_COLOR_RESET);
		}
	}

	printf(ANSI_COLOR_CYAN "\nExekuzioaren bukaera\n"ANSI_COLOR_RESET);
	printf("Agindu kopurua:\t\t%d\n", (T_file_fail+T_file_hit));
	printf("Exekuzio-denbora:\t%d ziklo\n", T_file);
	printf("Asmatzeak:\t\t%d\n", T_file_hit);
	printf("Hutsegiteak:\t\t%d\n", T_file_fail);


}


void cache_word(int info) {
    switch (info) {
        case 4:
        case 8:
            mem_info.word_size = info;
            printf( "Hitzaren tamaina berria: %d byte\n" ANSI_COLOR_RESET, info);
            break;
        default:
            printf(ANSI_COLOR_RED "Hitzaren tamaina posibleak: 4 edo 8 byte\n" ANSI_COLOR_RESET);
    }

}

void cache_block(int info) {

    switch (info) {
        case 32:
        case 64:
            mem_info.block_size = info;
            printf( "Blokearen tamaina berria: %d byte\n" ANSI_COLOR_RESET, info);
            break;
        default:
            printf(ANSI_COLOR_RED "Blokearen tamaina posibleak: 32 edo 64 byte\n" ANSI_COLOR_RESET);
    }

}
void cache_set(int info) {

    switch (info) {
        case 1:
        case 2:
        case 4:
        case 8:
            mem_info.set_size = info;
            printf( "Multzoen tamaina berria: %d byte\n" ANSI_COLOR_RESET, info);
            break;
        default:
            printf(ANSI_COLOR_RED "Multzoen tamaina posibleak: 1 (zuzenekoa), 2, 4 edo 8 (guztiz elkargarria)\n" ANSI_COLOR_RESET);
    }
}

void cache_replacement(char *info) {
	if (!strcmp(info, "lru"))
		mem_info.replacement = LRU;
	else
		mem_info.replacement = FIFO;

}
