#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define HEAP_SIZE 2048 // idk how big this needs 2 b

// the heap array we're maintaining
int *HeapArray;

// Anthony
int allocate(int number_of_bytes)
{
	// prints out a unique block number
	// start at 1 and increment
	// return block number
}

// Mukesh 
// (free is conflicting with C, needed to rename)
void deallocate(int block_number)
{
	// frees the memory
}

// Mukesh
void blocklist()
{
	// print out the heap
	// size	allocated	start	end (addresses in Hex)
}

// Ryan
void writeheap(int the_block_number, char CTW, int copies)
{
	// write characters to blocks in the heap
}

// Ryan
void printheap(int block_number, int number_of_bytes)
{
	// prints out all the bytes starting at the block number given
}

void quit() {
	// free the heap we created woohoo!
	free(HeapArray);
}

// shows a prompt to the user, gets an input line,
// calls parseCommand, then determines and calls
// the appropriate function for the command
void promptUser() {

    for (;;) {
        char *line = NULL;
        char **argv = NULL;
        size_t line_size = 0;
        size_t n = 0;

        printf("prompt>");

        getline(&line, &line_size, stdin);
        int argc = parseCommand(line, &n, &argv);
        free(line);
        // printf("%d tokens parsed\n", argc);
        if (argc > 0) {
            // int i;
            // for (i = 0 ; i < argc ; i++) {
            //     printf("argv %d: %s\n", i, argv[i]);
            // }
            if (!strcmp(argv[0], "allocate")) {
            	// int allocate(int number_of_bytes)
            	int number_of_bytes = atoi(argv[1]);
                allocate(number_of_bytes);
            }
            else if (!strcmp(argv[0], "free")) {
            	// void deallocate(int block_number)
            	int block_number = atoi(argv[1]);
                deallocate(block_number);
            }
            else if (!strcmp(argv[0], "blocklist")) {
            	// void blocklist()
                blocklist();
            }
            else if (!strcmp(argv[0], "writeheap")) { 
            	// void writeheap(int the_block_number, char CTW, int copies)
            	int the_block_number = atoi(argv[1]);
            	char CTW = argv[2][0];
            	int copies = atoi(argv[3]);
                writeheap(the_block_number, CTW, copies);
            }
            else if (!strcmp(argv[0], "printheap")) {
            	// void printheap(int block_number, int number_of_bytes)
            	int block_number = atoi(argv[1]);
            	int number_of_bytes = atoi(argv[2]);
                printheap(block_number, number_of_bytes);
            }
            else if (!strcmp(argv[0], "quit")) {
            	// void quit() 
                quit();
                break;
            }
        }
        if (argc != -1) {
            int i;
            for (i = 0 ; i < argc ; i++) {
                free(argv[i]);
            }
        }
        free(argv);
    }
}

// parses a line into command & any arguments,
// storing command and arguments as tokens in the (*tokens) array
// the tokens array is grown dynamically along with its size n
// returns the number of tokens allocated or -1 if allocation error
int parseCommand(char *line, size_t *n, char ***tokens) {
    if (!line)
        return -1;
    if (!*tokens) {
        *n = 2;
        *tokens = malloc(*n * sizeof(char*));
        if (!*tokens)
            return -1;
    }

 
    char *delims = " \t\n";
    char *token_begin = NULL;
    char *token_end = line;
    size_t token_size = 0;
    size_t count = 0;

    while (*token_end) {
        if (count >= *n) {
            *n = *n * 2;
            *tokens = realloc(*tokens, *n * sizeof(char*));
            if (!*tokens)
                return -1;
        }
        if (!strchr(delims, *token_end)) {
            // ignore leading whitespace
            if (!token_begin) {
                token_begin = token_end;
            }
            token_size++;
            token_end++;
        }
        else { // new delimiter found, store the current token
            if (token_begin) {
                (*tokens)[count] = malloc(token_size+1);
                if (!(*tokens)[count])
                    return -1;
                strncpy((*tokens)[count], token_begin, token_size);
                (*tokens)[count][token_size] = '\0';
                token_size = 0;
                count++;
            }
            // advance to next token
            while(*token_end && strchr(delims, *token_end)){
                token_end++;
            }
            token_begin = token_end;
        }
    }
    return count;
}

// Anthony
int main(int argc, char** argv)
{
	// statically allocate initial array
	HeapArray = malloc(HEAP_SIZE * sizeof(int));
	promptUser();
	return 0;
}

