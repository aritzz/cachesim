/*
 * Definizio estatikoak
 */

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


#define MAX_BUFFER 1024
#define COMMAND_BUFFER 4
#define COMMAND_BUFFER_CHARS 50
#define NUM_BLOCKS 8
#define LRU 1
#define FIFO 0
#define CACHE_TIME 2
#define MEMORY_TIME 20

typedef struct {
    int cache_size;
    int word_size;
    int block_size;
    int set_size;
    int replacement;
    int last_lru;
} Memory;

typedef struct {
    int busy;
    int changed;
    int tag;
    int replaced;
    int block;
    int fifo;
    int lru;
} Directory;
