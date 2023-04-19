

// Why strcpy and strncpy are not safe to use?

// strcpy() function

//     The strcpy() function is used to copy the source string to destination string.
//     If the buffer size of destination string is more then source string,
//     Then copy the source string to destination string with terminating NULL character.
//     But if destination buffer is less, then source string will be copy
//          to destination the content without terminating NULL character.
//     The strings may not overlap, and the destination string must be large enough to
//          receive the copy.

//     Syntax:
//         char *strcpy( char *destination, const char *source )

// Parameters: This function accepts two parameters as described below:
//     src: The string which will be copied.
//     dest: Pointer to the destination array where the content is to be copied.
// Return Value: It returns a pointer to the destination string.

#include <iostream>
#include <cstring>

using namespace std;

int perfecr_strcpy()
{
    char src[] = "geeksforgeeks";

    // Here destination is large enough
    // to store the src with Null
    // character at the end
    char dest[14];

    // copying src into dest.
    strcpy(dest, src);
    cout << "Copied string: " << dest << endl;

    return 0;
}

//     Problem with strcpy():
//         The strcpy() function does not specify the size of the destination array,
//     So buffer overrun is often a risk.
//     Using strcpy() function to copy from source of large character array into
//         smaller character array destination dangerous,
//     But if the string will fit, then it will not be worth the risk.
//     If the destination string is not large enough to store the source string then
//     the behavior of strcpy() is unspecified or undefined.

int main()
{
    perfecr_strcpy();

    char src[] = "geeksforgeeks";

    // Here destination is not large
    // enough to store the src. so the
    // behaviour of strcpy is unspecified.
    // program may crashed, but its
    // printing geeksforgeeks
    char dest[2];

    // copying src into dest.
    strcpy(dest, src);
    printf("Copied string: %s\n", dest);

    return 0;
}