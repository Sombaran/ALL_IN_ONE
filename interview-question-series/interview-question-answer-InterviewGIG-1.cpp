
// What is C++?

//     C++ is a general purpose Multi paradigm, high level programming language. It supports both Procedural and
//     object oriented programming. C++ is a superset of C |anguage(C cannot run C++ Code). It is one of the most
//     popular programming languages for creating large scale applications.

// What are the differences between C and C+ +?

//     C programming language:
//     C was developed by Dennis Ritchie between
//     1969 -1973 at AT&T Bell Labs.
//     C is a subset of C++
//     C is a procedural programming language.
//     C doesn't support function and operator overloading
//     C doesn't support reference variables
//     C doesn't support exception handling
//     C doesn't have namespace feature
//     C doesn't support virtual and friend function

//     C++ Programming language:
//     C++ was developed by Bjarne Stroustrup in 1979
//     C++ is superset of C, but C cannot run C++ code
//     C++ is Procedural and Object oriented programming language
//     C++ supports function and operator overloading
//     C++ supports reference variables
//     C++ supports exception handling
//     C++ uses NAMESPACE
//     C++ supports virtual and friend function

// What are the OOPS Concepts?
//     Object Oriented Programming-OOP
//     Object and Class
//     Inheritance
//     Encapsulation
//     Abstraction
//     Polymorphism

// What is a class?
//     A Class is a user defined data-type which has data members and member functions. 
//     A class is defined in C++ using
//     keyword class followed by the name of class.

// What is object?
//     An Object is an instance of a Class. When class is defined, only the specification for the object is defined; no memory
//     or storage is allocated.

// What is friend class?
//     Class members can gain accessibility over other class member by placing class declaration prefixed with the keyword
//     "friend" in the destination class.

// Can you explain storage classes?
//     Storage class are used to specify the visibility/scope and life time of symbols(functions and variables).
//     Storage classes specify where all a variable or function can be accessed
//     Till what time those variables will be available during the execution of program.

// What is Token?
//     A C++ program consists of various tokens and a token is either a keyword, an identifier,
//     a string literal, or a symbol, a constant

// What is Recursion?
//     A function that calls itself is known as recursive function, this is also known as recursion.

// What is function overriding?
//     Function overriding is a way to implement polymorphism.
//     Function overriding is a language feature that  allows a subclass or child class to provide
//         a specific implementation of a method that is already provided by one of its super classes or parent classes.

// What is Virtual function?
//     A virtual function is a member function in base class that you expect to redefine in derived classes.

// What is Volatile Variable?
//     Variables prefixed with the keyword volatile acts as a data type qualifier.
//     The volatile keyword attempts to alter the default way in which the variables are stored
//     The way the compiler handles the variables.

// What are Pointers?
//     Pointers are special type of variables that are used to store the memory address of the other variables.
//     Pointers are declared normally as other variables with difference of * that is present in front of the
//     pointer identifier.

// What is Null Pointer?
//     NULL pointer is not the uninitialized pointer that can point anywhere,
//     The NULL pointers are the one which do not point anywhere that is which do not point to any object or any function.

// What is enumeration?
//     An enumeration is a user-defined data type that consists of integral constants.
//     To define an enumeration, keyword enum is used.

// What are Structures and Unions?
//     While handling real world problems we come across situations when we want to use different data type
//     as one, C allows the user to define it own data type known as structures and unions.
//     Structures and unions gathers together different atoms of information are that comprise a given entity.

// What is inheritance?
//     Inheritance provides reusability.
//     Reusability means that one can use the functionalities of the existing class.
//     It eliminates the redundancy of code.
//     Inheritance is a technique of deriving a new class from the old class.
//     The old class is known as the base class, and the new class is known as derived class.

// What is Encapsulation?
//     Encapsulation is a process of combining data members and functions in a single unit called class.

// What is Abstraction?
//     Abstraction is a one of the technique of showing only essential details without representing the implementation
//     details. If the members are defined with a public keyword, then the members are accessible outside also. If the
//     members are defined with a private keyword, then the members are not accessible by the outside methods.

// What is Data binding?
//     Data binding is a process of binding the application UI and business logic.
//     Any change made in the business logic will reflect directly to the application UI.

// What is Polymorphism?
//     Polymorphism means multiple forms. It means having more than one function with the same name but with
//     different functionalities. Polymorphism is of two types:
//     Static polymorphism is also known as early binding.
//     Dynamic polymorphism is also known as late binding.

// What is Pure Virtual Function?
//     A Pure Virtual Function is a member function in which the base class forces
//         the derived classes to override.
//     Normally this member function has no implementation.
//     Pure virtual functions are equated to zero.

// What is a Copy Constructor?
//     It is the constructor which takes same class object as the parameter.
//     It gets automatically invoked as soon as the objectis initialized with another object of the same class.

// How should runtime errors be handled in C++?
//     The runtime errors in C++ can be handled using exceptions.
//     This exception handling mechanism in C++ is developed to handle the errors in software made up of
//     independently developed components operating in one process and under synchronous control.
//     According to C++, any routine that does not fulfil its promise throws an exception.
//     The caller who knows the way to handle these exceptions can catch it.

// What is namespace?
//     Namespaces are used to group entities like classes, objects and functions under a name.

// What are public, protected, private in C++?
//     Public — Here the data members and functions are accessible outside the class.
//     Protected — Data members and functions are available to derived classes only.
//     Private - Data members and functions are not accessible outside the class.

// How would you find out if a linked-list is a cycle or not?
//     We can find out if the linked—list is not a cycle by using two pointers.
//     One of them goes 2 nodes every time while the second one goes at 1 node each time.
//     If there is a cycle, the one that goes 2 nodes each time will meet the
//     one that goes slower.
//     If this happens, you can say that the linked-list is a cycle else not.

// What is STL?
//     STL stands for Standard Template Library.
//     It is a library of container templates approved by the ANSI committee for inclusion in the standard C++ specification.

// What is Stack unwinding?
//     Stack unwinding is a process during exception handling
//     when the destructor is called for all local objects between the place where the exception was thrown and
//     where it is caught.

// What is Copy Constructor?
//     It is a constructor which initializes it's object member variable with another object of the same class.
//     If you don't implement a copy constructor in your class, the compiler automatically does it.

// What is a pdb file?
//     A program database (PDB) file contains debugging and project state information
//     that allows incremental linking of a Debug configuration of the program.
//     This file is created when you compile a C/C++ program with /ZI or /Zi or
//     a Visual Basic/C#/JScript .NET program with /debug.

// What is dangling pointer?
//     When the address of an object is used after its lifetime is over, dangling pointer comes into existence.
//     Some examples of such situations are:
//         Returning the addresses of the automatic variables from a function or
//         using the address of the memory block after it is freed.