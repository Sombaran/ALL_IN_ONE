#include <iostream>
#include <memory>
using namespace std;

enum Country
{
    INDIA = 1,
    CHINA,
    JAPAN
};

// mobileCharging is main class
class mobileCharging
{
public:
    mobileCharging() { cout << "mobileCharging Constructor" << endl; }
    virtual ~mobileCharging() { cout << "mobileCharging Destructor" << endl; }
    virtual void chargeMobile(Country) = 0;
};

class IndiaSocket
{
public:
    IndiaSocket() { cout << "IndiaSocket Constructor" << endl; }
    ~IndiaSocket() { cout << "IndiaSocket Destructor" << endl; }
    void India_charging()
    {
        cout << "You are in INDIA, you are charging with indian charging"
             << endl;
    }
};

class ChinaSocket
{
public:
    ChinaSocket() { cout << "ChinaSocket Constructor" << endl; }
    ~ChinaSocket() { cout << "ChinaSocket Destructor" << endl; }
    void China_charging()
    {
        cout << "You are in CHINA, you are charging with chinese charging"
             << endl;
    }
};

class JapanSocket
{
public:
    JapanSocket() { cout << "JapanSocket Constructor" << endl; }
    ~JapanSocket() { cout << "JapanSocket Destructor" << endl; }
    void Japan_charging()
    {
        cout << "You are in JAPAN, you are charging with japanese charging"
             << endl;
    }
};

// Adaptor is concrete class of mobileCharging
class Adaptor : public mobileCharging,
                public IndiaSocket,
                public ChinaSocket,
                public JapanSocket
{
public:
    void chargeMobile(Country country)
    {
        switch (country)
        {
        case INDIA:
            India_charging();
            break;
        case CHINA:
            China_charging();
            break;
        case JAPAN:
            Japan_charging();
            break;
        default:
            cout << "Adaptor need to implement for the country" << endl;
            break;
        }
        cout << endl;
    }
};

int main()
{
    {
        unique_ptr<mobileCharging> socket = make_unique<Adaptor>();
        socket->chargeMobile(INDIA);
        socket->chargeMobile(CHINA);
        socket->chargeMobile(JAPAN);
    }
}
