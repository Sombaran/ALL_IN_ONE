// Builder Design Pattern
/*
    1. Because I want to create an Object (Plane) with composing other complex objects
            (body engine)
    2. AND::: main point here..............
        I want to create those complex objects step by
        step....... (Director will handle that)

    Question: WHY Builder Design Pattern
    Answer  : Because i want to build an object(plane) and it is compossed of complex
            objects(body, engine) step by step.
*/

#include <iostream>
using namespace std;

// Your end product
class Plane
{
    string _plane;
    string _body;
    string _engine;

public:
    Plane(string planeType) : _plane{planeType} {}
    void setEngine(string type) { _engine = type; }
    void setBody(string body) { _body = body; }
    string getEngine() { return _engine; }
    string getBody() { return _body; }

    void show()
    {
        cout << "Plane Type: " << _plane << endl
             << "Body Type: " << _body << endl
             << "Engine Type: " << _engine << endl
             << endl;
    }
    ~Plane() { cout << "calling Plane destructor" << endl; }
};

// PlaneBuilder Abstract Class
// Means all builders should have atleast these methods
class PlaneBuilder
{

public:
    virtual void getPartsDone() = 0;
    virtual void buildBody() = 0;
    virtual void buildEngine() = 0;

    virtual Plane *getPlane() = 0;

    virtual ~PlaneBuilder() {}
};

// PlaneBuilder concrete class
// knows only how to build Propeller Plane
class ConcretPropellerBuilder : public PlaneBuilder
{
    Plane *_plane;

public:
    void getPartsDone() { _plane = new Plane("Propeller Plane"); }
    void buildEngine() { _plane->setEngine("Propeller Engine"); }
    void buildBody() { _plane->setBody("Propeller Body"); }

    Plane *getPlane() { return _plane; }
    ~ConcretPropellerBuilder() { cout << "calling ConcretPropellerBuilder destructor" << endl; }
};

// PlaneBuilder concrete class
// Knows only how to build Jet Plane
class ConcretJetBuilder : public PlaneBuilder
{
    Plane *_plane;

public:
    void getPartsDone() { _plane = new Plane("Jet Plane"); }
    void buildEngine() { _plane->setEngine("Jet Engine"); }
    void buildBody() { _plane->setBody("Jet Body"); }

    Plane *getPlane() { return _plane; }
    ~ConcretJetBuilder() { cout << "calling ConcretJetBuilder destructor" << endl; }
};

// Defines steps and tells to the builder that build in given order.
class Director
{
    PlaneBuilder *builder;

public:
    Plane *createPlane(PlaneBuilder *builder)
    {
        builder->getPartsDone();
        builder->buildBody();
        builder->buildEngine();
        return builder->getPlane();
    }
    ~Director() { cout << "calling Director destructor" << endl; }
};

int main()
{

    Director dir;
    ConcretJetBuilder concretJetObj;
    ConcretPropellerBuilder concretPropObj;
    {
        Plane *jet = dir.createPlane(&concretJetObj);
        Plane *pro = dir.createPlane(&concretPropObj);

        jet->show();
        pro->show();

        delete pro;
        delete jet;
    }

    return 0;
}
