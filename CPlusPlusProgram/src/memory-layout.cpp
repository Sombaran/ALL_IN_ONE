/*
 * memory-layout.cpp
 *
 *  Created on: 17-Mar-2022
 *      Author: Jotirling Swami
 */

// Memory Layout in C/C++
/*

 High Addresses --->                     .----------------------.
					 |      Environment     |
					 |----------------------|
					 |                      |   Functions and variable are declared
					 |         STACK        |   on the stack.
 base pointer ->                         | - - - - - - - - - - -|
					 |           |          |
					 |           v          |
					 :                      :
					 .                      .   The stack grows down into unused space
					 .         Empty        .   while the heap grows up.
					 .                      .
					 .                      .   (other memory maps do occur here, such
					 .                      .    as dynamic libraries, and different memory
					 :                      :    allocate)
					 |           ^          |
					 |           |          |
 brk point ->                            ' - - - - - - - - - - -'   Dynamic memory is declared on the heap
					 |          HEAP        |
					 |                      |
					 |----------------------|
					 |          BSS         |   Uninitialized data -> Block Started by Symbol
					 |----------------------|
					 |          Ds          |   Initialized data (DS)
					 |----------------------|
					 |          Text        |   Binary code
 Low Addresses ---->                     '----------------------'
 
 */

//	Stack:
//		It located at a higher address and grows and shrinks opposite to the heap segment.
//		The stack contains local variables from functions.
//		A stack frame will create in the stack when a function is called, Each function has one stack frame.
//		Stack frames contain the function's local variables arguments and return value.
//		The stack contains a LIFO structure. Function variables are pushed onto the stack when called and
//		functions variables are popped off the stack when return.
//		SP(stack pointer) register tracks the top of the stack.
//		The process Stack contains the temporary data such as method/function parameters, return address and local variables.
//		It is an area of memory allotted for automatic variables and function parameters.
//		It also stores a return address while executing function calls.
//		Stack uses LIFO (Last- In-First-Out) mechanism for storing local or automatic variables,
//			function parameters and storing next address or return address.
//		The return address refers to the address to return after completion of function execution.
//		This segment size is variable as per local variables, function parameters, and function calls.
//		This segment grows from a higher address to a lower address.
//
//	Heap
//		This is dynamically allocated memory to a process during its run time.
//		This is area of memory allotted for dynamic memory storage such as for malloc() and calloc() calls.
//		This segment size is also variable as per user allocation.
//		This segment grows from a lower address to a higher address.
//		Let us now check how the segments (data and bss segments) size vary with a few sample programs.
//		Segment size is known by executing the command "size".
//
//	Data
//		This section contains the global and static variables.
//		It is represented by data section and the bss
//		The data section is used to declare the memory region, where data elements are stored for the program.
//		This section cannot be expanded after the data elements are declared, and it remains static throughout the program.
//		The .bss section is also a static memory section that contains buffers for data to be declared later in the program.
//		This buffer memory is zero-filled.
//
//	Initialized data segment :
//		This is a portion of the object file or program's virtual address space that consists of uninitialized static and global variables.
//		Un-initialized data segment is also called BSS (Block Started by Symbol) segment.
//		It contains all uninitialized global and static variables.
//		All variables in this segment initialized by the zero(0) and pointer with the null pointer.
//		The program loader allocates memory for the BSS section when it loads the program.
//
//
