

#include <iostream>

// final product

class Plane
{
    std::string _plane;
    std::string _body;
    std::string _engine;

public:
    Plane(std::string planeType) : _plane{planeType} {}
    Plane(std::string planeType, std::string bodyType)//: this->_plane,  _body(bodyType)  {
    {
        this->_plane;
        this->_body =bodyType;
    }
    Plane(std::string planeType, std::string bodyType, std::string engineType) 
    {
        this->_plane;
        this->_body;
        this->_engine = engineType;
    }
    void setEngine(std::string type) { _engine = type; }
    void setBody(std::string body) { _body = body; }
    std::string getEngine() { return _engine; }
    std::string getBody() { return _body; }

    void show()
    {
        std::cout << "Plane Type: " << _plane << std::endl
             << "Body Type: " << _body << std::endl
             << "Engine Type: " << _engine << std::endl
             << std::endl;
    }
    Plane(const Plane&) = delete;
    Plane operator=(const Plane&) = delete;
    ~Plane(){ std::cout << "calling Plane destructor" << std::endl; }
};

// PlaneBuilder Abstract interface class or builder class
// Means all builders should have atleast these methods
class PlaneBuilder
{

public:
    virtual void getPartsDone() = 0;
    virtual void buildBody() = 0;
    virtual void buildEngine() = 0;

    virtual Plane *getPlane() = 0;

    virtual ~PlaneBuilder() = default;
};

// PlaneBuilder concrete class
// knows only how to build Propeller Plane
class ConcretPropellerBuilder : public PlaneBuilder
{
    Plane *_plane;

public:
    void getPartsDone() final { _plane = new Plane("Propeller Plane"); }
    void buildEngine() final { _plane->setEngine("Propeller Engine"); }
    void buildBody() final { _plane->setBody("Propeller Body"); }

    Plane *getPlane() final { return _plane; }
    ~ConcretPropellerBuilder() { std::cout << "calling ConcretPropellerBuilder destructor" << std::endl; }
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
    ~Director() { std::cout << "calling Director destructor" << std::endl; }
};

int main()
{

    Director dir;
    ConcretPropellerBuilder concretPropObj;
    {
        Plane *pro = dir.createPlane(&concretPropObj);

        pro->show();

        delete pro;
    }

    return 0;
}
