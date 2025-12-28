#include <unistd.h>  // for sbrk
#include <stddef.h>

typedef struct Block {
    size_t size;        // size of the block
    int free;           // 1 if free, 0 if allocated
    struct Block *next; // pointer to next block
} Block;

#define BLOCK_SIZE sizeof(Block)

Block *freeList = NULL; // head of our linked list

void *my_malloc(size_t size) {
    Block *curr, *prev;
    curr = freeList;
    prev = NULL;

    // First-fit search in free list
    while (curr) {
        if (curr->free && curr->size >= size) {
            curr->free = 0;
            return (curr + 1); // return pointer after header
        }
        prev = curr;
        curr = curr->next;
    }

    // No suitable block found → request memory from OS
    Block *newBlock = sbrk(size + BLOCK_SIZE);
    if (newBlock == (void*) -1) return NULL; // sbrk failed

    newBlock->size = size;
    newBlock->free = 0;
    newBlock->next = NULL;

    if (prev) prev->next = newBlock;
    else freeList = newBlock;

    return (newBlock + 1);
}

void my_free(void *ptr) {
    if (!ptr) return;

    Block *block = (Block*)ptr - 1; // move back to header
    block->free = 1;

    // Optional: coalesce adjacent free blocks
    Block *curr = freeList;
    while (curr) {
        if (curr->free && curr->next && curr->next->free) {
            curr->size += BLOCK_SIZE + curr->next->size;
            curr->next = curr->next->next;
        }
        curr = curr->next;
    }
}

    
int main() {
    int *a = (int*) my_malloc(sizeof(int));
    *a = 42;

    char *s = (char*) my_malloc(10);
    s[0] = 'H'; s[1] = 'i'; s[2] = '\0';

    my_free(a);
    my_free(s);
    return 0;
}