// Tcl stands for Tool Command Language 
// Its a scripting language.
/*
What is Tcl?
    - Easy to learn but a powerful dynamic programming language.
    - Initial idea of its development began at the University of California (Berkeley) in the early 1980's.
    - Used in many different applications ranging from web & desktop applications, networking, administration,
    testing, EDA tool development and interface.
    - It is an open-source language and is maintained by large community of developers worldwide.
    - Commercial support is provided by ActiveState and several other companies.\

Why Tcl?
    - Allow rapid development of complex applications quickly.
    - Tcl scripts can be easily embedded into other applications which are written in C or C++.
    — Hence leveraging the most interesting functionality of this language i.e., to integrate or glue together various extensions and applications.
    - Tcl has simple syntax and basic language rules which makes it an ideal tool for building robust and portable
    Tcl applications.

    - Tcl - can work on multiple platform Windows/Linux
    - Tcl - Simple syntex and basic language rules for building roboust applicatuion.
    - Tcl - can use for GUI based application

Tcl Shell (tclsh):
    - Command-line interface (or) command-line interactive interpreter
    — "tclsh" executable is just one of the way to start a Tcl interpreter.
    - Another common executable is the command called "wish" which stands for Windowing Shell.



Variables:
- No need to declare the 'type' for the variables.
- Variables store string values (Tcl is type less)
- Use 'set' command to create a variable and assign or retrieve its value.
- Use 'unset' command to delete one or more variables.
- Use 'info exist <variable>' command to check if a variable exist.
- Use 'info vars' command to list all existing variables.
- The dollar (S) sign before a variable tells Tcl to use the value of the variable.

    % set myvar "Good Morning"
    Good Morning
    % set myvar
    Good Morning
    % set myvar2
    can 't read "myvar2": no such variable
    % info exist myvar
    1
    % info exist myvar2
    0
    % info vars
    myvar tcl_rcFileName tcl_version arng argv...
    % puts Smyvar
    Good Morning
    % unset myvar

TCl - type less 

Variable Names and Values

- Variable names are case sensitive
- We can use the special charactor o define the variable name.
- It may compose of any character, but the recommendation is to use only letters, digits and underscores in
variable names to maintain readability.

*/





