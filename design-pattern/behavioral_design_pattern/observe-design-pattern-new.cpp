// Observer Pattern

//     Observer is a behavioral design pattern that lets you define a subscription mechanism to notify multiple
//         objects about any events that happen to the object they’re observing.

//     Observer defines a one-to-many dependency between objects so that when one object changes state,
//         all its dependents are notified and updated automatically.
//     The pattern has a behavioral purpose and applies to the objects.
/**
 * Observer Design Pattern
 *
 * Intent: Lets you define a subscription mechanism to notify multiple objects
 * about any events that happen to the object they're observing.
 *
 * Note that there's a lot of different terms with similar meaning associated
 * with this pattern. Just remember that the Subject is also called the
 * Publisher and the Observer is often called the Subscriber and vice versa.
 * Also the verbs "observe", "listen" or "track" usually mean the same thing.
 */
/**
 * ConcreteObserver Design Pattern
 *
 * Intent: Lets you define a subscription mechanism to notify multiple objects
 * about any events that happen to the object they're observing.
 *
 * Note that there's a lot of different terms with similar meaning associated
 * with this pattern. Just remember that the
 *
 * ConcreteSubject is also called the Publisher
 * ConcreteObserver is often called the Subscriber
 * and vice versa.
 */

#include <iostream>
#include <list>
#include <string>

class Observer
{
public:
    virtual void Update(int status) = 0;
};

class Subject
{
public:
    virtual void Attach(Observer *ConcreteObserver) = 0;
    virtual void Detach(Observer *ConcreteObserver) = 0;
    virtual void Notify() = 0;
};

class ConcreteSubject : public Subject
{

private:
    std::list<Observer *> list_observer_;
    int status;

public:
    void Attach(Observer *ConcreteObserver) override
    {
        list_observer_.push_back(ConcreteObserver);
    }
    void Detach(Observer *ConcreteObserver) override
    {
        list_observer_.remove(ConcreteObserver);
    }

    void setValue(int status)
    {
        this->status = status;
        Notify();
    }

    void Notify() override
    {
        std::list<Observer *>::iterator iterator = list_observer_.begin();
        while (iterator != list_observer_.end())
        {
            (*iterator)->Update(status);
            ++iterator;
        }
    }
};

class ConcreteObserver : public Observer
{
private:
    std::string Obsrver_Name;
    ConcreteSubject *subject_;

public:
    ConcreteObserver(ConcreteSubject *ConcreteSubject, std::string ObsrverName) : subject_(ConcreteSubject)
    {

        Obsrver_Name = ObsrverName;
        std::cout << "Observer is attached: " << Obsrver_Name << std::endl;
        this->subject_->Attach(this);
    }
    virtual ~ConcreteObserver()
    {
    }

    void Update(int status) override
    {
        std::cout << "Downloading status: " << status << std::endl;
    }
    void RemoveMeFromTheList()
    {
        std::cout << "Observer is removed: " << Obsrver_Name << std::endl;
        subject_->Detach(this);
    }
};

void ClientCode()
{

    ConcreteSubject *mainsubject = new ConcreteSubject;
    ConcreteObserver *observer1 = new ConcreteObserver(mainsubject, "joti");
    ConcreteObserver *observer2 = new ConcreteObserver(mainsubject, "Prajot");
    ConcreteObserver *observer3 = new ConcreteObserver(mainsubject, "Swami");

    int value = 0;

    while (value != 100)
    {
        std::cin >> value;
        try
        {
            if (value >= 100)
                throw value;
            mainsubject->setValue(value);
        }
        catch (int value)
        {
            if (value == -1)
                std::cout << "invalid input" << std::endl;
            else
                std::cout << "Value must be <= 100% " << std::endl;
        }
    }

    observer3->RemoveMeFromTheList();
    observer2->RemoveMeFromTheList();
    observer1->RemoveMeFromTheList();

    delete observer3;
    delete observer2;
    delete observer1;
    delete mainsubject;
}

int main()
{
    ClientCode();
    return 0;
}
