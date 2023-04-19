// One Definition Rule (ODR)
//     In C++, declaration does not allocate any memory.
//     Its only when an object (or function) is defined that an instance of it is created.

//     The One definition Rule says that there cannot be more that one definition in a
//     single translation unit of C++.

//     You may skip the definition of an Object (or function) if it is not used but
//     if it is used ONE AND ONLY ONE definition must be provided.

//     In some cases there can be multiple definitions of Templates and inline functions can be defined
//     in multiple translation units, but all the definitions must be exactly same.

//     Achieving ODR
//         1. Defined all the functions and objects in .cpp file and use header files (.h)
//             only to declare them (since multiple declarations never create a problem).
//         2. Use Include guards:

//     Include guards are used to avoid multiple inclusion of header files,
//     this cannot be done only by being cautious over not to include the file twice.
//     For example in the below case, you cannot cut short any

//-----------------------------------------------------------------------------------
/* File: a.h
 */
// Defining int (Not declaring)
int myInt = 29;
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
/* File: b.h
 */
// This file will have definition of myInt variable
#include "a.h"
//-----------------------------------------------------------------------------------

/* File: mymain.cpp
 */
#include "a.h"
#include "b.h"
// This file will have two definition of myInt variable.
// This will give ERROR
//-----------------------------------------------------------------------------------

// Solution :
//     We should use the #include guards. The usage of guards is as follows.

//-----------------------------------------------------------------------------------
/* File: a.h
 */
#ifndef _A_H_
#define _A_H_
int myInt = 29; // Defining int (Not declaring)
#endif
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
/* File: b.h
 */
#ifndef _B_H_
#define _B_H_
#include "a.h"
// we have guards in this file also.
#endif
//-----------------------------------------------------------------------------------

//     Here, the first inclusion of "a.h" causes the macro _A_H_ to be defined.
//     Then, when "mymain.cpp" includes "b.h" the second time, the #ifndef test fails,
//     and the preprocessor skips the definition of myInt variable 2nd time,
//     hence the variable is defined only once.
//     Hence, The program compiles correctly.
//     Ideally, the objects should only be defined in the .cpp (or .c) files and .h files should only have declarations.