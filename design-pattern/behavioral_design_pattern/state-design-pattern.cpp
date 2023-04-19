//  State Design Pattern
//     State one of the type of a behavioral design pattern. 
//     Intent
//         1. Allow an object to alter its behavior when its internal state changes.
//         2. The object will appear to change its class.
//         3. An object-oriented state machine
//             wrapper + polymorphic wrappee + collaboration

//     When to use?
//         1. when an object's behavior depends on its state, and it must change its
//             behavior at run-time depending on that state
//         2. Operations have large, multipart conditional statements that depend on 
//             the object's state


//     Context (Account): 
//         Defines an interface to client to interact. 
//         It maintains references to concrete state object which may be used to define
//             current state of object.
    
//     State: (State)
//         Defines interface for declaring what each concrete state should do.
    
//     ConcreteState: (SilverState, GoldState, PlatinumState)
//         Provides implementation for methods defined in State.

#include <iostream>
using namespace std;

class State
{
public:
    virtual void getBenifits() = 0;
};

class SilverState : public State
{
public:
    void getBenifits()
    {
        cout << "----~-~----~———-~--~---~--~----~——-----------------——---"
             << endl;
        cout << "Account is in Silver state." << endl;
        cout << "Your Benefits are listed below :" << endl;
        cout << "10% of on groceries with the use of debit card" << endl;
        cout << "15% of on beauty products with the use of debit card" << endl;
        cout << "----~-~----~———-~--~---~--~----~——-~---------------——---\n"
             << endl;
    }
};

class GoldState : public State
{
public:
    void getBenifits()
    {
        cout << "----~-~----~———-~--~---~--~----~——-----------------——---"
             << endl;
        cout << "Account is in Gold state." << endl;
        cout << "Your Benefits are listed below :" << endl;
        cout << "15% of on groceries with the use of debit card" << endl;
        cout << "20% of on beauty products with the use of debit card" << endl;
        cout << "----~-~----~———-~--~---~--~----~——-~---------------——---\n"
             << endl;
    }
};

class PlatinumState : public State
{
public:
    void getBenifits()
    {
        cout << "----~-~----~———-~--~---~--~----~——-----------------——---"
             << endl;
        cout << "Account is in Platinum state." << endl;
        cout << "Your Benefits are listed below :" << endl;
        cout << "20% of on groceries with the use of debit card" << endl;
        cout << "25% of on beauty products with the use of debit card" << endl;
        cout << "----~-~----~———-~--~---~--~----~——-~---------------——---\n"
             << endl;
    }
};


class Account
{
private:
    State *state;
    double balance;

public:
    Account()
    {
        state = new SilverState;
        balance = 0;
    }

    void Deposit(double amount)
    {
        balance += amount;
        EvaluateState();
    }

    void Withdraw(double amount)
    {
        balance -= amount;
        EvaluateState();
    }

    void GetCurrentBenefits()
    {
        cout << "Your Account Balance :" << balance << endl
             << endl;
        state->getBenifits();
    }

    void EvaluateState()
    {
        if (balance <= 5000)
        {
            delete state;
            state = new SilverState();
        }

        if (balance > 5000 && balance < 10000)
        {

            delete state;
            state = new GoldState();
        }
        if (balance >= 10000)
        {
            delete state;
            state = new PlatinumState();
        }
    }
};

int main()
{
    Account *account = new Account();
    double option;
    while (true)
    {
        cout << "Please Enter 1. Deposite 2. Withdraw:";
        cin >> option;
        if (option == 1)
        {
            cout << "Please Enter amount between 1000 to 10000 to Deposite:";
            cin >> option;
            cout << endl;
            account->Deposit(option);
            account->GetCurrentBenefits();
        }
        else
        {
            cout << "Please Enter amount to Withdrawn:";
            cin >> option;
            cout << endl;
            account->Withdraw(option);
            account->GetCurrentBenefits();
        }
    }

    return 0;
}

//   State objects should be singletons?
//   Yes. Always try to make state objects singletons.

//   Advantages of state design pattern?
//   We can easily add new states and new behaviors in the application 
//      without impacting other components Or class.

//   It also helps in reducing complexity by reducing the use of if-else statements
//      or switch/case conditional logic.

//   Disadvantages of state design pattern?
//   The state pattern is also known as objects for states. 
//      So, we can assume that more states need more codes, 
//      and the obvious side effect is difficult maintenance for us.