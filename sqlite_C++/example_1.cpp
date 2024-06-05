#include <iostream>
#include <sqlite3.h>
#include <memory>

int main(int argc, char** argv) 
{ 
    sqlite3* DB{nullptr};

    int exit {}; 
    exit = sqlite3_open("example.db", &DB); 

    if (exit) { 
        std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl; 
        return (-1); 
    } 
    std::cout << "Opened Database Successfully!" << std::endl; 
    sqlite3_close(DB); 
    return (0); 
}