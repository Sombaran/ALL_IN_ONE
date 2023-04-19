// Relationships between C++ Classes:

//     1. inheritance :

//         Relationship:

//             D "is-a" B

//         Code:

//             class D : public B{ };

//         Explanation:
//             Derived class D is a specialization of the Base class B.
//             D inherits all the members of B

//     2. Composition

//         Relationship:  D "has-a" B

//             Ownership, D is "part—of" B

//         Code:

//         class B{};

//         class D
//         {
//             private:
//                 B b;
//         };

//         Explanation:
//             Composite class D owns, or contains, a part class B.
//                  B is created and destroyed with D.
//             The interface of B is visible only to D, not its clients.

//     3. Aggregation

//         Relationship:  D "has-a" B

//             Ownership, B is "part-of" D

//         Code:

//             class B{};

//             class D
//             {
//                 void fun()
//                 {
//                     B* ptr =  new B();
//                 }
//             }

//         Explanation:
//             The Aggregator class D owns a part class B.
//             B is created by a member function of D,
//                  and so its lifetime is strictly less than that of D.
//             D is expected to destroy B.

//     4. Using:

//         Relationship:

//         Referral: U uses R through a reference

//         Code:

//             public class U
//             {
//                 ...
//                 public:
//                 void register (R& r)
//                 {
//                 // use r
//                 }
//             };

//         Explanation:
//             A class U uses instance of class R, to which it holds a reference.
//             R is created by some other entity
//             and a reference to it is passed to some member function of class U.

// Relationships between objects

// There are many different kinds of relationships two objects may have in real-life, and we use specific “relation type” words to describe these relationships.
// For example:
//     a square “is-a” shape.
//     A car “has-a” steering wheel.
//     A computer programmer “uses-a” keyboard.
//     A flower “depends-on” a bee for pollination.
//     A student is a “member-of” a class.
//     Your brain exists as “part-of” you

// ASSOCIATION — By Value 
//      I have a relationship with an object.
//      Foo uses Bar
//      It dose not tell about the life cycle of the Bar

class Bar
{
};
class F00
{
    void Baz(Bar bar)
    {
    }
};

//  Aggregation — By Reference
//      I have an object which I've borrowed from someone else.
//      When Foo dies, Bar may live on.

class Bar
{
};

class Foo
{
    Bar _bar;
    Foo(const Bar &bar)
    {
        _bar = bar;
    }

} :

// Composition — if 1 composite of 2 Means if 1 dies  then 2 dies also
//      I own an object and I am responsible for its lifetime.
//      When Foo dies, so does Bar

class Bar
{
};
class Foo
{
private:
    Bar *bar = new Bar();
};