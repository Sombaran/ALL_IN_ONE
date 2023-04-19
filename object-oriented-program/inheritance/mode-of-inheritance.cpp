//   Inheritance in C++
//     Modes of Inheritance in C++
//     1. Public inheritance in C++
//         If a derived class is inherited from a public base class
//         The base class public members -> public in the derived class.
//         The base class protected members -> protected in the derived class.

//     2. Protected Inheritance in C++
//         If we inherit a derived class from a protected base class
//         The base class public -> protected in the derived class.
//         The base class protected -> protected in the derived class.

//     3. Private Inheritance in C++
//         If a derived class is inherited from a private base class
//         The base class public and protected -> private in the derived class.

//      Base Class Member	                Mode of Inheritance
//                              Public          Protected           Private
//      Public	                Public	        Protected	        Private
//      Protected	            Protected	    Protected	        Private
//      Private	                Private     	Private	            Private

// Types of Inheritance in C++ / Levels of inhertance
//     There are 5 types of C++ Inheritance:

// 1. Single Inheritance in C++
//     In this type of inheritance, there is only one derived class inherited from one base class.

//     Syntex:

//         class base {
//              //Body of the base class
//         };
//
//         class derived : access_specifier base {
//              //Body of the derived class
//         };

// Example of Single Inheritance in C++

//     #include <iostream>
//     using namespace std;
//     class Animal {
//         public:
//             void fun1() {
//                 cout<<"I am an animal"<<endl;
//             }
//     };
//     class Dog : public Animal {
//         public:
//             void fun2() {
//                 cout<<"I am a dog"<<endl;
//             }
//     };
//     int main() {
//          Dog obj;
//          obj.fun1();
//          obj.fun2();
//          return 0;
// //     }

// 2. Multiple Inheritance in C++
//     In this type of inheritance, one derived class inherits from more than one base class.

//     Syntex:
//         class base1 {
//             //body
//         };
//         class base2 {
//             //body
//         };
//             .
//             .
//             .
//         class derived : access_specifier base1, access_specifier base2, ... {
//             //body of the derived class
//         };

// Example of Multiple Inheritance in C++

//         #include <iostream>
//         using namespace std;

//         class A {
//             protected:
//             int a;
//             public:
//             void seta(int x) {
//                 a = x;
//             }
//         };

//         class B {
//             protected:
//             int b;
//             public:
//             void setb(int y) {
//                 b = y;
//             }
//         };
//         class C : public A, public B {
//             public:
//             int add() {
//                 cout<<"Addition of two numbers = "<<a+b;
//             }
//         };

//         int main() {
//         C obj;
//         obj.seta(4);
//         obj.setb(9);
//         obj.add();
//         return 0;
//         }

// 3. Multilevel Inheritance in C++
//         A derived class inherits from another derived class in this type of inheritance.

// Syntax:

//         class C {    //Base Class
//              //body
//         };
//         class B : access_specifier C {      //Derived from C
//             //body
//         };
//         class A : access_specifier B {      //Derived from B
//                 //body
//         };

// Example of Multilevel Inheritance in C++

//         #include <iostream>
//         using namespace std;
//         class Animal {
//             public:
//             void fun1() {
//                 cout<<"Animal"<<endl;
//             }
//         };
//         class PetAnimal : public Animal {
//             public:
//             void fun2() {
//                 cout<<"Pet animal"<<endl;
//             }
//         };
//         class Dog : public PetAnimal {
//             public:
//             void fun3() {
//                 fun1();
//                 fun2();
//                 cout<<"Dog"<<endl;
//             }
//         };
//         int main() {
//         Dog obj;
//         obj.fun3();
//         return 0;
//         }

// 4. Hierarchical Inheritance in C++
//     Multiple derived classes are inherited from a single base class in this type of inheritance.

// Syntax:

//         class base {
//              //body
//         };
//         class derived1 : access_specifier base {
//              //body;
//         };
//         class derived2 : access_specifier base {
//              //body;
//         };
//
// Example of Hierarchical Inheritance in C++

//         #include <iostream>
//         using namespace std;
//         class Values {
//             protected:
//             double a, b;
//             public:
//             void initialize(double x, double y) {
//                 a = x;
//                 b = y;
//             }
//         };
//         class A : public Values {
//             public:
//             void add() {
//                 cout<<"addition = "<<a+b<<endl;
//             }
//         };
//         class B : public Values {
//             public:
//             void subtract() {
//                 cout<<"subtraction = "<<a-b<<endl;
//             }
//         };
//         int main() {
//              A obj1;
//              B obj2;
//              obj1.initialize(4.5,8.7);
//              obj1.add();
//              obj2.initialize(3.6,11);
//              obj2.subtract();
//              return 0;
// //         }

// 5. Hybrid Inheritance in C++
//     Hybrid inheritance is the combination of two or more types of inheritance.
//     We can make various combinations in hybrid inheritance.
//     For example, a combination of hierarchical and multiple inheritance (commonly called multipath inheritance)

// Example of Hybrid Inheritance in C++

//         #include <iostream>
//         using namespace std;
//         class A {
//             protected:
//             float a;
//             public:
//             void seta(float n1) {
//                 a = n1;
//             }
//         };
//         class B : public A {
//             public:
//             void modifyA() {
//                 a/=2;
//             }
//         };
//         class C {
//             protected:
//             float c;
//             public:
//             void setc(float n2) {
//                 c = n2;
//             }
//         };
//         class D : public B, public C {
//             public:
//             float modify() {
//                 modifyA();
//                 cout<<"Result = "<<a*c;
//             }
//         };
//         int main() {
//             D obj;
//             obj.seta(15.6);
//             obj.setc(9.7);
//             obj.modify();
//             return 0;
//         }