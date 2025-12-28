

#include <stdio.h>



typedef struct {
    int data[5];
    int size;
} IntArray;

int main() {
    printf("Hello, World!\n");
    IntArray arr;
    arr.size = 5;
    for (int i = 0; i < arr.size; i++) {
        arr.data[i] = i+1;
    }
    for (int i = 0; i < arr.size; i++) {
        printf("%d ", arr.data[i]);
    }
    printf("\n");


    return 0;
}