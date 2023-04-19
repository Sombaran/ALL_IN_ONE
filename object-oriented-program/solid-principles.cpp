// SOLID Principle in C++
/*


S = Single Responsibility Principle
        A class should only have a single responsibility.

    Example -
        If you are working on the class of Employee. Then we will have only employee related information and the single method should be responciple for the changing or modifying the smae entity, It should not modify other entity or it should not depend on the other parameters.

    Example -
        If you want to search the bank in the specific area then we should be focusing on the finding or naming the bank in that aread, We should not focus on the How many accounts are there and who is working working in that branch.

Bad Example: Task
        +dnloadFile(String Location)
        +parseTheFile(File file)
        +persistTheData(Data data)

Good Example: Report
        +printReport()
        +getReportData()
        +formatReport

O = Open-Closed Principle
        Any application should be open for extension and closed for modification.
    Example - Using interface
        When you define the interdface we will defind the requirement, We are not implemention the requirements. As per the requirement we can extends with class and inherite with the interface class. We can have multiple extends of the interfaxce as the requirements.

        If you have class and ots representing the one functinality and we are modifyng the same class for implememnting difference funcionality its not good way to inplementation.

            // Open-Close Principle - Bad example
            class GraphicEditor {
            public:
                void drawShape(Shape s) {
                    if (s.m_type == 1)
                        drawRectangle(s);
                    else if (s.m_type == 2)
                        drawCircle(s);
                }
                void drawCircle(Circle r) {
                    //...
                }
                void drawRectangle(Rectang1e r) {
                    //...
                }
            };

            // Open-Close Principle — Good example
            class Shape {
            public:
                int m_type;
            };
            class Rectangle: public Shape {
            public:
                Rectangle() {
                    m_type = 1;
                }
            };
            class Circle: public Shape {
            public:
                Circle() {
                    m_type = 2;
                }
            };


L = Liskov Substitution Principle (Inheritance)
       Child class should be able to substitute Parent class anytime with same behaviour.


    Every derived class should inherite the base class property.
    Means Derived class should properlly substitute the base class.

    Good Example
    
                Class Vehicle
                + fuelAmount()
                + capacity()
                + applyBrakes()

    Class Bus     Class Car     Class Truck

    Bad Example

                Class Vehicle
                + fuelAmount()
                + capacity()
                + applyBrakes()

    Class Bus     Class Car     Class helicoptor


I = Interface Segregation Principle
        Client should not be forced to implement interface even if it is not required.
        Create as small interface as possible.
        Many client-specific interfaces better than one general-purpose interface.

class Animal
{
    Void feed() = 0;
    void play() = 0 ;
};

class Dog : public Animal
{
    Void feed();
    void play();
};

class Tiger : public Animal
{
    Void feed();
    void play(); 
}

// here in class Tiger
// here we can not play with tiger 
// but still we need to write the function
// to avoid the compiler error.

Instead of that we can have the another small interface class as

class Pet{
    Void feed();
    void play();
}

class Animal
{
    Void feed() = 0;
    void play() = 0 ;
};

class Dog : public Animal
{
    Void feed();
    void play();
};

class Tiger : public Animal
{
    Void feed();
    void play(); 
}

D = Dependency Inversion/Injection Principle
        Dependencies should be abstract rather than concrete.

    It helps to implement the loose coupling 

    DIP — Dependency Inversion Principle
        Depend upon Interfaces or abstract classes rather than
        Concrete classes.

    Example :

        Earlier One Employer was managing everything like
        employees, its payroll, its attendance..... But then we
        have Payroll team, HR team etc....



*/