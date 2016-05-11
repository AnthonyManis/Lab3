// Manis, Anthony: 71461439
// Mukesh, Kastala: 40646351
// Cockins, Ryan: 92859394

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define HEAP_SIZE 400

// the heap array we're maintaining
int *HeapArray;
int nextBlockNumber = 1;

int indexOfBlockNumber(int block_number)
{
	if (block_number == 1)
		return 0;

	int index = 0;
	int i;
	for (i = 0; i < block_number; ++i)
	{
		if (HeapArray[i] == 0)
		{
			while (HeapArray[i] == 0)
			{
				++index;
			}
		}
		else
			index += HeapArray[i] + 2;
	}
	return index;
}	

// Anthony
int allocate(int number_of_bytes)
{
    if (number_of_bytes <= 0 )
        return;

    int *p = HeapArray;
    int words = (number_of_bytes / 4) + 2;
    if ( (number_of_bytes % 4) != 0)
        words++;

    // find a free header that fits words
    while (p) {
        // if free and enough space
        if ( is_allocated(p) == 0 && block_size(p) >= words ) {
            int *old_footer = p + block_size(p) - 1;
            int *new_footer = p + words - 1;
            set_allocated_bit(p, 1);
            set_block_size(p, words);
            *new_footer = *p;
            // if not all the space was used, gotta split &
            // make a new section for the remaining space
            if (new_footer < old_footer) {
                int *next_header = new_footer + 1;
                set_allocated_bit(next_header, 0);
                set_block_size(next_header, 
                    old_footer - next_header + 1);
                *old_footer = *next_header;
            }

            // prints out a unique block number
            // start at 1 and increment
            // return block number
            // we may need to store a list of block numbers * pointers
            // for the purpose of printing
            printf("%d\n", nextBlockNumber);
            return nextBlockNumber++;
        }
        p = p + block_size(p);
    }
}


bool is_allocated(int *p) {
    return *p & 1;
}

int block_size(int *p) {
    return *p >> 1;
}

void set_allocated_bit(int *p, bool bit) {
    *p = *p & 0xFFFFFFF0;
    if ( bit )
        *p += 1;    
}

void set_block_size(int *p, int size) {
    size = size << 1;
    *p = (*p & 1) + size;
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
void writeheap(int block_number, char CTW, int copies)
{
	// write characters to blocks in the heap
	// if the block number exists
	// go to that block number
	int index = indexOfBlockNumber(block_number);
	int block_size = index;
	int index += 1;
	int i;
	// copy as many as the header of the block number says there are spaces
	for (i = 0; i < block_size || i < copies; ++i)
	{
		// convert char to binary number
		ArrayHeap[index] = (int)CTW;
		++index;
	}
}

// Ryan
void printheap(int block_number, int number_of_bytes)
{
	// prints out all the bytes starting at the block number given
	int index = indexOfBlockNumber(block_number);
	int indice = index + 1;
	int i;
	for (i = 0; i < number_of_bytes; ++i)
	{
		if (ArrayHeap[indice])
		{
			i += index;
			int temp;
			for (temp = 0; temp < index; ++temp)
			{
				putchar((char)ArrayHeap[indice]);
				++indice;
			}
			index = ArrayHeap[indice];
		}
		else
		{
			putchar('0');
			++i;
		}
	}
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
            	if (number_of_bytes > 0)
                	allocate(number_of_bytes);
            }
            else if (!strcmp(argv[0], "free")) {
            	// void deallocate(int block_number)
            	int block_number = atoi(argv[1]);
            	if (block_number > 0)
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
            	if ( (the_block_number > 0) && (copies > 0) )
                	writeheap(the_block_number, CTW, copies);
            }
            else if (!strcmp(argv[0], "printheap")) {
            	// void printheap(int block_number, int number_of_bytes)
            	int block_number = atoi(argv[1]);
            	int number_of_bytes = atoi(argv[2]);
            	if ( (block_number > 0) && (number_of_bytes) > 0)
                	printheap(block_number, number_of_bytes);
            }
            else if (!strcmp(argv[0], "quit")) {
            	// void quit() 
                quit();
                break;
            }
            else {
            	printf("Invalid Command or Arguments\n");
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

void initialize() {
    // HeapArray[0] and [HEAP_SIZE - 1] are H/F
    // Initial value is unallocated, size HEAP_SIZE
    set_block_size(HeapArray, HEAP_SIZE);
    set_allocated_bit(HeapArray, 0);
    *(HeapArray + HEAP_SIZE) = *HeapArray;
}


// Anthony
int main(int argc, char** argv)
{
	// statically allocate initial array
	HeapArray = malloc(HEAP_SIZE * sizeof(int));
    initialize();
	promptUser();
	return 0;
}

