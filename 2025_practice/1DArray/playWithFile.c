


// C program to use the fread() function to read the content of binary file into an array.
#include <stdio.h>

int main() {
    FILE *file;
    int buffer[200];

    // Open the binary file for reading
    file = fopen("gdb.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read the integers from the file into the buffer
    size_t read = fread(buffer, sizeof(int),sizeof(buffer)/sizeof(int), file);
    if ( read > 0) {
        printf("Successfully read %zu elements from the file.\n", read);
        buffer[read] = '\0'; // Null-terminate the buffer
        printf("Data read from file: %s\n", buffer);
    } else {
        printf("No elements were read from the file.\n");
    }
    // Print the integers that were read
    for (int i = 0; i < 5; i++) {
        printf("Element %d: %d \n", i + 1, buffer[i]);
    }

    // Close the file
    /*size_t bytesRead = 0;
        // Data is read from the file into the buffer
    while ((bytesRead = fread(buffer, 1, sizeof(buffer) - 1, file)) > 0){

      // Null-terminate the buffer
        buffer[bytesRead] = '\0'; 

        // Print the read data
        printf("%s", buffer);
    }*/
    fclose(file);
    return 0;
}