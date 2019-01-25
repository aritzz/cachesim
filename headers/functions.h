
/* Funtzio externoak */

extern int get_command(char *);
extern int execute_command(void);
extern void show_help(void);
void reset_simulator(void);
void show_directory(void);
void show_memory(void);
void cache_store(int,int, FILE*);
void cache_load(int, FILE*);
void cache_word(int );
void cache_block(int );
void cache_set(int );
void cache_replacement(char*);
void clean_cmdbuffer();
void file_load(char*);
