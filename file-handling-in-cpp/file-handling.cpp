// //  File Handling in C++
//         C++ programming language offers various useful and exciting features and functionalities to programmers.
//         It also supports object-oriented programming.
//         With the help of C++, you can also perform file handling.
//         Files and streams are one of the most important and useful concepts in C++.

//         Till now, we were using iostream which provides cin and cout methods
//             for reading from input and writing to output respectively.

//     What is File Handling in C++?
//         With the help of Files, you can store data in a storage device permanently.
//         Using File Handling, you can store the output of a program in a file
//         You can also perform various operations on it.
//         You can say that a stream is an abstraction that represents a device in which input and output operations are performed.

//     In C++, there are 3 file handling methods such as
//         1. ifstream,
//         2. ofstream
//         3. fstream.
//         They are designed to manage the disk files.
//         These are defined in fstream.
//         That’s why you have to include fstream whenever you are working with files in C++.

//     Files are mainly handled by three classes in C++:-

//         ofstream:-  Used to create files and write data into the files.
//         ifstream:-  Used to read information from the file.
//         fstream:-   Used to create files and write data into file, read data from the file.

//     You can perform the below operations through File Handling:-

//         open():-    Create a file
//         read():-    Reading file.
//         write():-   Writing new data.
//         close():-  Closing a file.

//         We can Open a file in C++:
//             Bypass the file name in the constructor at the time of object creation.
//             Then use the open() function.
//                 open() function
//             Syntax:-
//                 void open(const char* NameFile,ios::openmode mode);

//                     First argument of the open() function defines the name and the format of the file.
//                     Second argument describes the mode in which the file has to be opened.

//     Following is the list of modes:-
//             Modes	                                    What it does
//             in	                                Used to open the file for reading
//             out	                                Used to open the file for writing
//             binary	                            Used to open the file in binary mode
//             app		                            Used to append all the outputs at the end by opening the file
//             ate	                                Used to move the control to the end of the file
//             trunc		                        Used to remove the data
//             nocreate		                    Used to open the file only if it exists
//             noreplace	                        Used to open the file only if it does not exist

//     Example:-

//         fstream exp;
//         exp.open("Tech.txt", ios::out);

//     In the above example, exp is an object of type fstream.
//     We have to create an object of this class to use its member functions.
//     We created an exp object and called the open() function.

//     Default Open modes:-
//         fstream ios::in
//         ofstream ios::out
//         fstream ios::in | ios::out

//         You can also combine different modes using pipe(|) or or.
//         With the help of stream insertion operator, you can write data to a file.
//         And with the help of the stream extraction operator, you can read data from a file.

//     Example:- Using the open() function

#include <iostream>
#include <fstream>
using namespace std;
int CreateFile()
{
    fstream exp;
    exp.open("Tech.txt", ios::out);
    if (!exp)
    {
        cout << "File Creation failed!";
    }
    else
    {
        cout << "Created a New File!";
        exp.close();
    }
    return 0;
}

//         In the above, we created an object of the class fstream named exp.
//         Then we apply the open() function on the created object.
//         We used the if and else statement.
//             If the file already exists or if it does not exist then it will
//                 print "File Creation failed!". Otherwise,
//             It will create a new file named and it will
//                 print "Created a New File!" to the screen.

//     Writing to a file in C++
//         You can also write data into a file in C++.
//         Using stream insertion operator(<<), you can write data into a file in C++.

//     Example:- writing to a file

int WriteFile()
{
    fstream exp;
    exp.open("Tech.txt", ios::out);
    if (!exp)
    {
        cout << "File creation failed!";
    }
    else
    {
        cout << "Created a new file!";
        exp << "TechVidvan Tutorial: C++ File Handling!";
        exp.close();
    }
    cout << "Written Data into the new file" << endl;
    return 0;
}

//         In the above example, we created a new file named Tech.txt.
//         We used the stream insertion operator("<<") to put data into that file.
//         And the data is "TechVidvan Tutorial: C++ File Handling!".

//     Reading from a file in C++
//         You can also read data from a file.
//         Using the stream extraction operator(">>"), you can read the contents of the file.

//     Example:- reading from a file

int ReadFile()
{
    fstream exp;
    exp.open("Tech.txt", ios::in);
    if (!exp)
        cout << "No such file";

    else
    {
        char s;
        while (!exp.eof())
        {
            exp >> s;
            cout << s;
        }
    }
    exp.close();
    return 0;
}

//         In the above example, we are reading the contents of the Tech.txt
//         file using the stream extraction operator.
//         If the file does not exist then it will print No such file to the screen.
//         You can see there are no spaces in the output.
//         To correct it, you can use getline() with a character array to print the whole line as it is.

//     Close a file in C++:-
//         You can use C++ close() function to close a file.

//     Example:- Closing a file

int CloseFile()
{
    fstream exp;
    exp.open("Tech.txt", ios::out);
    exp.close();
    return 0;
}

//     Special Operations on a file in C++

//         1. tellp():- Used to tell the current position of the put pointer.
//              Syntax:-
//                  filepointer.tellp()

//         2. tellg():- Used to tell the current position of the get pointer.
//              Syntax:-
//                  filepointer.tellg()

//         3. seekp():- Used to move the put pointer to the mentioned location.
//              Syntax:-
//                  filepointer.seekp(number_of_bytes,reference_mode)

//         4. seekg():- Used to move the get pointer(input) to a specified location.
//              Syntax:-
//                  filepointer.seekg(number_of_bytes,reference_point)

//         5. put():- Used to write a single character to the file.

//         6. get():- Used to read a single character from the file.

int Main()
{
    fstream exp;
    exp.open("Tech.txt", ios::out);
    if (!exp)
    {
        cout << "File creation failed";
    }
    else
    {
        cout << "New file created" << endl;
        exp << "TechVidvan Tutorial!";
        cout << "File Pointer Position: " << exp.tellp() << endl;

        exp.seekp(-1, ios::cur);
        cout << "tellp=>File Pointer Position: " << exp.tellp() << endl;

        exp.close();
    }

    exp.open("Tech.txt", ios::in);
    if (!exp)
    {
        cout << "No such file";
    }
    else
    {
        char s;
        exp.seekg(5, ios::beg);
        cout << "tellg=>File Pointer Position: " << exp.tellg() << endl;
        cout << endl;
        exp.seekg(1, ios::cur);
        cout << "tellg=>File Pointer Position: " << exp.tellg() << endl;
        exp.close(); // Closing file
    }
    return 0;
}

int main()
{
    CreateFile();
    WriteFile();
    ReadFile();
    CloseFile();
    Main();
    return 0;
}
