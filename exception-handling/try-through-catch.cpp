//  Why Exception Handling?
//     Here, are the reason for using Exception Handling in C++:

//     You will separate your error handling code from your normal code.
//     The code will be more readable and easier to maintain.
//     Functions can handle the exceptions they choose.
//     Even if a function throws many exceptions, it will only handle some.
//     The caller will handle the uncaught exceptions.

//  Exception Handling Keywords
//          Exception handling in C++ revolves around these three keywords:

//     throw:
//         when a program encounters a problem, it throws an exception. The throw keyword helps the program perform the throw.

//     catch:
//         a program uses an exception handler to catch an exception.
//         It is added to the section of a program where you need to handle the problem. It’s done using the catch keyword.

//     try:
//         the try block identifies the code block for which certain exceptions will be activated. 
//         It should be followed by one/more catch blocks.

// Summary:
//     1. With exception handling in C++, you can handle runtime errors.
//     2. Runtime errors are the errors that occur during program execution.
//     3. Exception handling helps you handle any unexpected circumstances in your program.
//     4. When the unexpected circumstance occurs, program control is transferred to handlers.
//     5. To catch an exception, you place a section of code under the try-catch block.
//     6. The throw keyword helps the program throw exceptions, helping the program to handle the problem.
//     7. The try keyword helps identify the code block for which certain exceptions will be activated.
//     8. We can overwrite the what() function of the exception header file to define our exceptions.