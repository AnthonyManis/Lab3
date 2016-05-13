// Manis, Anthony: 71461439
// Mukesh, Kastala: 40646351
// Cockins, Ryan: 92859394

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define HEAP_SIZE 400

typedef struct Node {
    char data;
    int block_number;
    int size;
    bool allocated;
} Node;

// the heap array we're maintaining
Node *HeapArray;
int nextBlockNumber = 1;

// Prototypes --- User Functions
int allocate(int number_of_bytes);
void deallocate(int block_number);
void blocklist();
void writeheap(int block_number, char CTW, int copies);
void printheap(int block_number, int number_of_bytes);
void quit();

// Prototypes --- Interface Functions
void promptUser();
int parseCommand(char *line, size_t *n, char ***tokens);

// Prototypes --- Helper Functions
int indexOfBlockNumber(int block_number);
int block_number(Node *p);
void set_block_number(Node *p, int block_number);
bool is_allocated(Node *p);
void set_allocated(Node *p, bool allocated);
int block_size(Node *p);
void set_block_size(Node *p, int size);
void initialize();

int indexOfBlockNumber(int block_number)
{
    if (block_number < 1)
    {
        printf("Block number can not be less than 1");
        return -1;
    }

    if (block_number == 1)
        return 0;

    int i = 0;

    while (HeapArray[i].block_number != block_number)
    {
        if (i+HeapArray[i].size < HEAP_SIZE)
            i += HeapArray[i].size;
        else
        {
            i = -1;
            printf("Block number does not exists");
            break;
        }
    }

    return i;
}

// Anthony
int allocate(int number_of_bytes)
{
    if (number_of_bytes <= 0 ) {
        printf("Could not allocate %d bytes.\n", number_of_bytes);
        return 0;
    }

    Node *p = HeapArray;

    // find a free header that fits number_of_bytes
    while (p < HeapArray + HEAP_SIZE) {
        // if free and enough space
        if ( is_allocated(p) == 0 && block_size(p) >= number_of_bytes) {

            // if we aren't allocating the whole block,
            // it must be split and a new header must be made
            if (number_of_bytes < p->size) {
                Node *next_header = p + number_of_bytes;
                set_allocated(next_header, 0);
                set_block_size(next_header, p->size - number_of_bytes);
                set_block_number(next_header, 0);
            }
            // finally, allocate the header
            set_allocated(p, 1);
            set_block_size(p, number_of_bytes);
            set_block_number(p, nextBlockNumber);

            printf("%d\n", nextBlockNumber);
            return nextBlockNumber++;
        }
        p = p + block_size(p);
    }
    printf("Could not allocate %d bytes.\n", number_of_bytes);
    return 0;
}

//// Sets the data of a node.
// args:
//  *p is a pointer to a node
// data is the value to which the data will be set
void set_data(Node *p, char data) {
    p->data = data;
}

//// Gets the data from a node.
// args: *p is a pointer to a Node
char data(Node *p) {
    return p-> data;
}

//// Sets the block_number of a node.
// args:
//  *p is a pointer to a node
//  block_number is the value to which the block_number will be set
void set_block_number(Node *p, int block_number) {
    p->block_number = block_number;
}

//// Gets the block_number from a node.
// args: *p is a pointer to a node
int block_number(Node *p) {
    return p->block_number;
}

//// Gets the allocated status from a node.
// args: *p is a pointer to a node
// returns: true if the block is allocated, false
// if unallocated
bool is_allocated(Node *p) {
    return p->allocated;
}

//// Sets the allocated status of a node.
// args:
//  *p is a pointer to a node
//  bit is the value to which the allocated bit will be set
void set_allocated(Node *p, bool allocated) {
    p->allocated = allocated;
}

//// Gets the block_size from a node.
// args: *p is a pointer to a node
// returns: the size of the block including header/footer
int block_size(Node *p) {
    return p->size;
}

//// Sets the block_size of a node.
// args:
//  *p is a pointer to a node
//  size is the value to which the block size will be set
void set_block_size(Node *p, int size) {
    p->size = size;
}

// Mukesh
// (free is conflicting with C, needed to rename)
void deallocate(int block_number)
{
    int index = indexOfBlockNumber(block_number);
    HeapArray[index].block_number = 0;
    HeapArray[index].allocated = 0;

}

// Mukesh
void blocklist()
{


    Node *p = HeapArray;

    // print out the heap
    // size allocated   start   end (addresses in Hex)
    printf("%s   %s     %s            %s\n", "Size", "Allocated", "Start", "End");
    int start = 0;
    int end;
    while (p < HeapArray + HEAP_SIZE) {

        printf("%3d", p->size);

        if (is_allocated(p) == 0)
            printf("  %8s", "No");
        else
            printf("  %8s", "Yes");


        printf("      0x%08x", start);
        end = start + p->size - 1;
        printf("      0x%08x\n", end);
        start = end + 1;

        p = p + block_size(p);

    }

}

// Ryan
void writeheap(int block_number, char CTW, int copies)
{
    // write characters to blocks in the heap
    // if the block number exists
    // go to that block number
    int index = indexOfBlockNumber(block_number);
    if (index > -1)
    {
        int i;
        int block_size = HeapArray[index].size;
        // copy as many as the header of the block number says there are spaces
        for (i = 0; index < HEAP_SIZE && i < block_size && i < copies; ++i)
        {
            // convert char to binary number
            HeapArray[index].data = CTW;
            ++index;
        }
    }
}

// Ryan
void printheap(int block_number, int number_of_bytes)
{
    // prints out all the bytes starting at the block number given
    int index = indexOfBlockNumber(block_number);
    if (index > -1)
    {
        int i;
        for (i = 0; index < HEAP_SIZE && i < number_of_bytes; ++i)
        {
            printf("%c",HeapArray[index].data);
            ++index;
        }
        printf("\n");
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
                // first check that there's an appropriate number of arguments
                if (argc == 2) {
                    // int allocate(int number_of_bytes)
                    int number_of_bytes = atoi(argv[1]);
                    if (number_of_bytes > 0)
                        allocate(number_of_bytes);
                }
                else {
                    printf("Usage: allocate [number_of_bytes]\n");
                }
            }
            else if (!strcmp(argv[0], "free")) {
                if (argc == 2) {
                    // void deallocate(int block_number)
                    int block_number = atoi(argv[1]);
                    if (block_number > 0)
                        deallocate(block_number);
                }
                else {
                    printf("Usage: free [block_number]\n");
                }
            }
            else if (!strcmp(argv[0], "blocklist")) {
                // void blocklist()
                blocklist();
            }
            else if (!strcmp(argv[0], "writeheap")) {
                if (argc == 4) {
                    // void writeheap(int the_block_number, char CTW, int copies)
                    int the_block_number = atoi(argv[1]);
                    char CTW = argv[2][0];
                    int copies = atoi(argv[3]);
                    if ( (the_block_number > 0) && (copies > 0) )
                        writeheap(the_block_number, CTW, copies);
                }
                else {
                    printf("Usage: writeheap [block_number] [char] [amount]\n");
                }
            }
            else if (!strcmp(argv[0], "printheap")) {
                if (argc == 3) {
                    // void printheap(int block_number, int number_of_bytes)
                    int block_number = atoi(argv[1]);
                    int number_of_bytes = atoi(argv[2]);
                    if ( (block_number > 0) && (number_of_bytes) > 0)
                        printheap(block_number, number_of_bytes);
                }
                else {
                    printf("Usage: printheap [block_number] [amount]\n");
                }
            }
            else if (!strcmp(argv[0], "quit")) {
                // void quit()
                quit();
                break;
            }
            else {
                printf("Invalid Command\n");
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
    // statically allocate initial array
    HeapArray = malloc(HEAP_SIZE * sizeof(Node));

    int i;
    for (i = 0 ; i < HEAP_SIZE ; i++) {
        HeapArray[i].size = 0;
        HeapArray[i].block_number = 0;
        HeapArray[i].data = '\0';
        HeapArray[i].allocated = 0;
    }

    // HeapArray[0] is Header
    // Initial value is unallocated, size HEAP_SIZE
    set_block_size(HeapArray, HEAP_SIZE);
    set_allocated(HeapArray, 0);
}


// Anthony
int main(int argc, char** argv)
{
    initialize();
    promptUser();
    return 0;
}
