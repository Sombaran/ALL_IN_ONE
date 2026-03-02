
#include <bits/stdc++.h>



class testMemory{
    public:

    explicit testMemory(){
        std::cout << __func__ << std::endl;
    }
    ~ testMemory() {
        std::cout << __func__ << std::endl;
    }

    std::unique_ptr<testMemory> createTestMemory() {
        // std::make_unique is the preferred way
        std::cout << __func__ << std::endl; 
        return std::make_unique<testMemory>();
}

};




int main(int argc, char** argv)
{
    std::shared_ptr<testMemory> obj1 = std::make_shared<testMemory>();
    std::shared_ptr<testMemory> obj2 = std::make_shared<testMemory>();

    auto obj3 = std::make_shared<testMemory>();
    auto obj4 = std::make_shared<testMemory>();
    auto obj5 = obj4;
    std::unique_ptr<testMemory> obj6;

    // Use the object
    obj6 -> createTestMemory();

    // Transfer ownership
    std::unique_ptr<testMemory> obj7 = std::move(obj6);
    obj7 -> createTestMemory();

    return (0);
}