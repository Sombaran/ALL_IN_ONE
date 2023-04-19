// site link - https://www.softwaretestinghelp.com/runtime-polymorphism-in-cpp/

Function Overriding
Function overriding is the mechanism using which a function defined in the base class is once again defined in the derived class. In this case, we say the function is overridden in the derived class.

We should remember that function overriding cannot be done within a class. The function is overridden in the derived class only. Hence inheritance should be present for function overriding.

The second thing is that the function from a base class that we are overriding should have the same signature or prototype i.e. it should have the same name, same return type and same argument list.

Let us see an example that demonstrates method overriding.

#include <iostream>
using namespace std;
class Base
{
   public:
   void show_val()
   {
      cout << "Class::Base"<<endl;
   }
};
class Derived:public Base
{
   public:
   void show_val() //function overridden from base
   {
      cout << "Class::Derived"<<endl;
   }
};
int main()
{
   Base b;
   Derived d;
   b.show_val();
   d.show_val();
}
Output:

Class::Base
Class::Derived

In the above program, we have a base class and a derived class. In the base class, we have a function show_val which is overridden in the derived class. In the main function, we create an object each of Base and Derived class and call the show_val function with each object. It produces the desired output.

The above binding of functions using objects of each class is an example of static binding.

Now let us see what happens when we use the base class pointer and assign derived class objects as its contents.

The example program is shown below:

#include <iostream>
using namespace std;
class Base
{
    public:
    void show_val()
    {
        cout << "Class::Base";
    }
};
class Derived:public Base
{
    public:
    void show_val()     //overridden function
    {
        cout << "Class::Derived"; } }; int main() { Base* b; //Base class pointer Derived d; //Derived class object b = &d; b->show_val();   //Early Binding 
}
Output:

Class::Base

Now we see, that the output is "Class:: Base". So irrespective of what type object the base pointer is holding, the program outputs the contents of the function of the class whose base pointer is the type of. In this case, also static linking is carried out.

In order to make the base pointer output, correct contents and proper linking, we go for dynamic binding of functions. This is achieved using Virtual functions mechanism which is explained in the next section.

Virtual Function
For the overridden function should be bound dynamically to the function body, we make the base class function virtual using the "virtual" keyword. This virtual function is a function that is overridden in the derived class and the compiler carries out late or dynamic binding for this function.

Now let us modify the above program to include the virtual keyword as follows:

#include <iostream>
using namespace std;.
class Base
{
    public:
    virtual void show_val()
    {
        cout << "Class::Base";
    }
};
class Derived:public Base
{
    public:
    void show_val()
    {
        cout << "Class::Derived"; } }; int main() { Base* b; //Base class pointer Derived d; //Derived class object b = &d; b->show_val();   //late Binding 
}
Output:

Class::Derived

So in the above class definition of Base, we made show_val function as "virtual". As the base class function is made virtual, when we assign derived class object to base class pointer and call show_val function, the binding happens at runtime.

Thus, as the base class pointer contains derived class object, the show_val function body in the derived class is bound to function show_val and hence the output.

In C++, the overridden function in derived class can also be private. The compiler only checks the type of the object at compile time and binds the function at run time, hence it doesn’t make any difference even if the function is public or private.

Note that if a function is declared virtual in the base class, then it will be virtual in all of the derived classes.

But till now, we haven’t discussed how exactly virtual functions play a part in identifying correct function to be bound or in other words, how late binding actually happens.

The virtual function is bound to the function body accurately at runtime by using the concept of the virtual table (VTABLE) and a hidden pointer called _vptr.

Both these concepts are internal implementation and cannot be used directly by the program.

Working Of Virtual Table And _vptr
First, let us understand what a virtual table (VTABLE) is.

The compiler at compile time sets up one VTABLE each for a class having virtual functions as well as the classes that are derived from classes having virtual functions.

A VTABLE contains entries that are function pointers to the virtual functions that can be called by the objects of the class. There is one function pointer entry for each virtual function.

In the case of pure virtual functions, this entry is NULL. (This the reason why we cannot instantiate the abstract class).

Next entity, _vptr which is called the vtable pointer is a hidden pointer that the compiler adds to the base class. This _vptr points to the vtable of the class. All the classes derived from this base class inherit the _vptr.

Every object of a class containing the virtual functions internally stores this _vptr and is transparent to the user. Every call to virtual function using an object is then resolved using this _vptr.

Let us take an example to demonstrate the working of vtable and _vtr.

#include<iostream>
using namespace std;
class Base_virtual  
 {  
 public:  
    virtual void function1_virtual() {cout<<"Base :: function1_virtual()\n";};  
    virtual void function2_virtual() {cout<<"Base :: function2_virtual()\n";};  
    virtual ~Base_virtual(){};
};  
    
class Derived1_virtual: public Base_virtual  
{  
public:  
   ~Derived1_virtual(){};
   virtual void function1_virtual() { cout<<"Derived1_virtual :: function1_virtual()\n";}; }; int main() { Derived1_virtual *d = new Derived1_virtual; Base_virtual *b = d; b->function1_virtual();
  b->function2_virtual();
  delete (b);
   
  return (0);
}
Output:

Derived1_virtual :: function1_virtual()
Base :: function2_virtual()

In the above program, we have a base class with two virtual functions and a virtual destructor. We have also derived a class from the base class and in that; we have overridden only one virtual function. In the main function, the derived class pointer is assigned to the base pointer.

Then we call both the virtual functions using a base class pointer. We see that the overridden function is called when it is called and not the base function. Whereas in the second case, as the function is not overridden, the base class function is called.

Now let us see how the above program is represented internally using vtable and _vptr.

As per the earlier explanation, as there are two classes with virtual functions, we will have two vtables – one for each class. Also, _vptr will be present for the base class.

vtable layout

Above shown is the pictorial representation of how the vtable layout will be for the above program. The vtable for the base class is straightforward. In the case of the derived class, only function1_virtual is overridden.

Hence we see that in the derived class vtable, function pointer for function1_virtual points to the overridden function in the derived class. On the other hand function pointer for function2_virtual points to a function in the base class.

Thus in the above program when the base pointer is assigned a derived class object, the base pointer points to _vptr of the derived class.

So when the call b->function1_virtual() is made, the function1_virtual from the derived class is called and when the function call b->function2_virtual() is made, as this function pointer points to the base class function, the base class function is called.

Pure Virtual Functions And Abstract Class
We have seen details about virtual functions in C++ in our previous section. In C++, we can also define a "pure virtual function" that is usually equated to zero.

The pure virtual function is declared as shown below.

 virtual return_type function_name(arg list) = 0;
The class which has at least one pure virtual function that is called an "abstract class". We can never instantiate the abstract class i.e. we cannot create an object of the abstract class.

This is because we know that an entry is made for every virtual function in the VTABLE (virtual table). But in case of a pure virtual function, this entry is without any address thus rendering it incomplete. So the compiler doesn’t allow creating an object for the class with incomplete VTABLE entry.

This is the reason for which we cannot instantiate an abstract class.

The below example will demonstrate Pure virtual function as well as Abstract class.

#include <iostream>
using namespace std;
class Base_abstract          
{
    public:
    virtual void print() = 0;    // Pure Virtual Function
};
class Derived_class:public Base_abstract
{
    public:
    void print()
    { 
        cout << "Overriding pure virtual function in derived class\n"; } }; int main() { // Base obj; //Compile Time Error Base_abstract *b; Derived_class d; b = &d; b->print();
}
Output:

Overriding pure virtual function in the derived class

In the above program, we have a class defined as Base_abstract which contains a pure virtual function which makes it an abstract class. Then we derive a class "Derived_class" from Base_abstract and override the pure virtual function print in it.

In the main function, not that first line is commented. This is because if we uncomment it, the compiler will give an error as we cannot create an object for an abstract class.

But the second line onwards the code works. We can successfully create a base class pointer and then we assign derived class object to it. Next, we call a print function which outputs the contents of the print function overridden in the derived class.

Let us list some characteristics of abstract class in brief:

We cannot instantiate an abstract class.
An abstract class contains at least one pure virtual function.
Although we cannot instantiate abstract class, we can always create pointers or references to this class.
An abstract class can have some implementation like properties and methods along with pure virtual functions.
When we derive a class from the abstract class, the derived class should override all the pure virtual functions in the abstract class. If it failed to do so, then the derived class will also be an abstract class.
Virtual Destructors
Destructors of the class can be declared as virtual. Whenever we do upcast i.e. assigning the derived class object to a base class pointer, the ordinary destructors can produce unacceptable results.

For Example, consider the following upcasting of the ordinary destructor.

#include <iostream>
using namespace std;
class Base
{
    public:
    ~Base() 
    {
        cout << "Base Class:: Destructor\n"; 
    }
};
class Derived:public Base
{
    public:
    ~Derived() 
    { 
        cout<< "Derived class:: Destructor\n"; 
    }
}; 
int main()
{
    Base* b = new Derived;     // Upcasting
 delete b;
}
Output:

Base Class:: Destructor

In the above program, we have an inherited derived class from the base class. In the main, we assign an object of the derived class to a base class pointer.

Ideally, the destructor that is called when "delete b" is called should have been that of derived class but we can see from the output that destructor of the base class is called as base class pointer points to that.

Due to this, the derived class destructor is not called and the derived class object remains intact thereby resulting in a memory leak. The solution to this is to make base class constructor virtual so that the object pointer points to correct destructor and proper destruction of objects is carried out.

The use of virtual destructor is shown in the below example.

#include <iostream>
using namespace std;
 
class Base
{
    public:
    virtual ~Base() 
    {
        cout << "Base Class:: Destructor\n"; 
    }
};
class Derived:public Base
{
    public:
    ~Derived() 
    { 
        cout<< "Derived class:: Destructor\n"; 
    }
}; 
int main()
{
    Base* b = new Derived;     // Upcasting
 delete b;
}
Output:

Derived class:: Destructor
Base Class:: Destructor

This is the same program as the previous program except that we have added a virtual keyword in front of the base class destructor. By making base class destructor virtual, we have achieved the desired output.

We can see that when we assign derived class object to base class pointer and then delete the base class pointer, destructors are called in the reverse order of object creation. This means that first the derived class destructor is called and the object is destroyed and then the base class object is destroyed.

Note: In C++, constructors can never be virtual, as constructors are involved in constructing and initializing the objects. Hence we need all the constructors to be executed completely.

Conclusion
Runtime polymorphism is implemented using method overriding. This works fine when we call the methods with their respective objects. But when we have a base class pointer and we call overridden methods using the base class pointer pointing to the derived class objects, unexpected results occur because of static linking.

To overcome this, we use the concept of virtual functions. With the internal representation of vtables and _vptr, virtual functions help us accurately call the desired functions. In this tutorial, we have seen in detail about runtime polymorphism used in C++.

With this, we conclude our tutorials on object-oriented programming in C++. We hope this tutorial will be helpful to gain a better and thorough understanding of object-oriented programming concepts in C++.