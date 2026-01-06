
#include <stdio.h>

int main(int argc, char **argv) {
    int array[5] = {1, 2, 3, 4, 5};

    // Addresses (use %p and cast to (void*))
    printf("1) &array      = %p (type: int (*)[5])\n", (void*)&array);
    printf("2) array       = %p (type: int *)\n",      (void*)array);
    printf("3) &array[0]   = %p (same as array)\n",    (void*)&array[0]);

    // First element value
    printf("4) *array      = %d (value of array[0])\n", *array);

    // Demonstrate correct indexing
    for (size_t i = 0; i < 5; ++i) {
        printf("array[%zu]     = %d\n", i, array[i]);
    }

    // Show the difference between *(array + i) vs (*array + i)
    // *(array + i) -> element at index i
    // (*array + i) -> value of first element plus i
    for (size_t i = 0; i <= 6; ++i) {
        printf("*(array + %zu) (index)   -> ", i);
        if (i < 5) {
            printf("%d\n", *(array + i));  // safe indexing
        } else {
            printf("OUT OF BOUNDS (don't dereference)\n");
        }
    }

    for (size_t i = 0; i <= 6; ++i) {
        printf("(*array + %zu) (value+N) -> %d\n", i, (*array + (int)i));
    }

    for (size_t i = 0; i <5; ++i) {
        printf("%d \n", array[i]);
    }


    int *ptr = array; // Pointer to the first element of the array
    printf("Using pointer to traverse the array: %d \n", *(ptr+3));
    return 0;
}
