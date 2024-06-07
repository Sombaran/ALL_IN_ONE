#include <iostream>
#include <functional>
#include <memory>

class someClass {
    int m_var;
    std::string m_str;
    public:
    someClass () = default;
    someClass (const someClass &) = delete;
    someClass (const someClass &&) = delete;
    someClass operator = (const someClass &) = delete;
    someClass operator = (const someClass &&) = delete;
    ~someClass () = default;

    void getVal (int val, std::string str) {
        this->m_var = val;
        this->m_str = str;
    }
    void display () {
        std::cout << "Value : " << m_var << " String : " << m_str << std::endl;
    }

    std::function < void (int, std::string) > functionalObj1 =
        [this] (int val, std::string str) {
            this->m_var = val;
            this->m_str = str;
	        //std::cout << "ValueF1 : " << m_var << " StringF1 : " << m_str << std::endl;
            display ();
    };
};

int
main ()
{
    //someClass obj;
    std::shared_ptr<someClass> obj {std::make_shared<someClass>()};
    obj->getVal (30, "Sombaran");
    obj->display ();
    std::function < void (int, std::string) > functionalObj2 =
    [&](int val, std::string str) {
        std::cout << "ValueF2 : " << val << " StringF2 : " << str << std::endl;
    };
    obj->functionalObj1 (4200, "Sogupta");
    //functionalObj2(223072904, "Som");
    return (0);
}
