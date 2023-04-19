// //  Strategy Design Pattern
//     Application: When to use the Strategy Design pattern:
//     1. When you want to use different variants of an algorithm within an
//          object and be able to switch from one algorithm to another during runtime.

//     2. Use this pattern when you have a lot of similar classes that only differ in the way they execute some behavior.

//     3.  Use this pattern to isolate the business logic of a class from the
//          implementation details of algorithms that may not be as important
//          in the context of that logic.

//     4. Use the pattern when your class has a massive conditional operator that
//          switches between different variants of the same algorithm.

#include <iostream>
using namespace std;

class Items
{
public:
    string name;
    float price;
};

class DiscountStrategy
{
public:
    virtual float getDiscountPercentage() = 0;
};

class ItemContext
{
private:
    DiscountStrategy *discountStrategy;

public:
    ItemContext(DiscountStrategy *discountStrategy) : discountStrategy(discountStrategy)
    {
    }

    float getItemNetPrice(Items *items)
    {
        return items->price - (items->price * discountStrategy->getDiscountPercentage() / 100);
    }
};

class DiwaliStrategy : public DiscountStrategy
{
public:
    float getDiscountPercentage()
    {
        return 20;
    }
};

class NewYearStrategy : public DiscountStrategy
{
public:
    float getDiscountPercentage()
    {
        return 10;
    }
};

class OffStrategy : public DiscountStrategy
{
public:
    float getDiscountPercentage()
    {
        return 0;
    }
};

int main()
{

    Items *items = new Items{"Smartphone", 50000.00};
    cout << "The " << items->name << " price is: " << items->price << endl;

    ItemContext *diwaliContext = new ItemContext(new DiwaliStrategy);
    cout << "The price in DiwaliStrategy: "
         << diwaliContext->getItemNetPrice(items) << endl;

    ItemContext *newYearContext1 = new ItemContext(new NewYearStrategy);
    cout << "The price in NewYearStrategy: "
         << newYearContext1->getItemNetPrice(items) << endl;

    ItemContext *offSeasonContext1 = new ItemContext(new OffStrategy);
    cout << "The price in OffStrategy: "
         << offSeasonContext1->getItemNetPrice(items) << endl;

    return 0;
}
