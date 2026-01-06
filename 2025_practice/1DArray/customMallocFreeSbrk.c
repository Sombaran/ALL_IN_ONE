#include <unistd.h>
#include <stddef.h>
#include <stdio.h>

typedef struct block {
    size_t size;          // size of the allocated block
    int free;             // free flag
    struct block *next;   // next block in the list
} block_t;

#define BLOCK_SIZE sizeof(block_t)

static block_t *head = NULL;  // start of our block list

// Find a free block (first-fit)
static block_t *find_free_block(size_t size) {
    block_t *curr = head;
    while (curr) {
        if (curr->free && curr->size >= size) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

// Request memory from OS using sbrk
static block_t *request_space(size_t size) {
    void *ptr = sbrk(size + BLOCK_SIZE);
    if (ptr == (void *)-1) {
        return NULL; // sbrk failed
    }
    block_t *blk = (block_t *)ptr;
    blk->size = size;
    blk->free = 0;
    blk->next = NULL;
    return blk;
}

void *my_malloc(size_t size) {
    if (size == 0) return NULL;

    block_t *blk;
    if (!head) {
        // First allocation
        blk = request_space(size);
        if (!blk) return NULL;
        head = blk;
    } else {
        blk = find_free_block(size);
        if (blk) {
            blk->free = 0;
        } else {
            // No free block found, request new space
            blk = request_space(size);
            if (!blk) return NULL;
            // Append to list
            block_t *curr = head;
            while (curr->next) curr = curr->next;
            curr->next = blk;
        }
    }
    return (blk + 1); // return pointer after header
}

void my_free(void *ptr) {
    if (!ptr) return;
    block_t *blk = (block_t *)ptr - 1; // get header
    blk->free = 1; // mark as free (no coalescing)
}

// Demo
int main() {
    int *arr = (int *)my_malloc(5 * sizeof(int));
    for (int i = 0; i < 5; i++) arr[i] = i * 10;

    for (int i = 0; i < 5; i++) printf("%d ", arr[i]);
    printf("\n");

    my_free(arr);

    char *str = (char *)my_malloc(20);
    snprintf(str, 20, "Hello custom malloc!");
    printf("%s\n", str);

    my_free(str);
    return 0;
}