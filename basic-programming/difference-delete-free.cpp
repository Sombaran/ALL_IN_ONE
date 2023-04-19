// Differences between delete and free()
//     The following are the differences between delete and free() in C++ are:

//     1.  The delete is an operator that de-allocates the memory dynamically while the free() is a function that destroys the memory at the runtime.

//     2.  The delete operator is used to delete the pointer, which is either allocated using new operator or a NULL pointer, whereas the free() function is used to delete the pointer that is either allocated using malloc(), calloc() or realloc() function or NULL pointer.

//     3.  When the delete operator destroys the allocated memory, then it calls the destructor of the class in C++, whereas the free() function does not call the destructor; it only frees the memory from the heap.

//     4.  The delete() operator is faster than the free() function.

// Note:
//     The most important reason why free() should not be used for de-allocating memory allocated using new is that, it does not call the destructor of that object while delete operator does.