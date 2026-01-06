#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>
#include <iostream>

typedef struct block {
    size_t size;          // size of the allocated block
    int free;             // free flag
    struct block *next;   // next block in the list
} block_t;

constexpr size_t BLOCK_SIZE = sizeof(block_t);

block_t *head = nullptr;  // start of our block list

// Find a free block (first-fit)
block_t *find_free_block(size_t size) {
    block_t *curr = head;
    while (curr) {
        if (curr->free && curr->size >= size) {
            return curr;
        }
        curr = curr->next;
    }
    return nullptr;
}

// Request memory from OS using mmap
block_t *request_space(size_t size) {
    void *ptr = mmap(nullptr, size + BLOCK_SIZE,
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        return nullptr; // mmap failed
    }
    block_t *blk = (block_t *)ptr;
    blk->size = size;
    blk->free = 0;
    blk->next = nullptr;
    return blk;
}

void *my_malloc(size_t size) {
    if (size == 0) return nullptr;

    block_t *blk;
    if (head == nullptr) {
        // First allocation
        blk = request_space(size);
        if (blk == nullptr) {
            return nullptr;
        }
        head = blk;
    } else {
        blk = find_free_block(size);
        if (blk == nullptr) {
            blk->free = 0;
        } else {
            // No free block found, request new space
            blk = request_space(size);
            if (blk == nullptr) {
                return nullptr;
            }
            // Append to list
            block_t *curr = head;
            while (curr->next != nullptr) {
                curr = curr->next;
            }
            curr->next = blk;
        }
    }
    return (blk + 1); // return pointer after header
}

void my_free(void *ptr) {
    if (ptr == nullptr) {
        return;
    }
    block_t *blk = (block_t *)ptr - 1; // get header
    blk->free = 1; // mark as free

    // Optional: actually release memory back to OS
    // munmap(blk, blk->size + BLOCK_SIZE);
}

// Demo
int main() {
    int *arr = (int *)my_malloc(5 * sizeof(int));
    for (int i = 0; i < 5; i++) arr[i] = i * 10;

    for (int i = 0; i < 5; i++) printf("%d ", arr[i]);
    printf("\n");

    my_free(arr);

    //char *str = (char *)my_malloc(20);
    //snprintf(str, 20, "Hello mmap malloc!");
    //printf("%s\n", str);

    //my_free(str);
    return 0;
}