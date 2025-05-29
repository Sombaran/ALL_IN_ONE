#include <bits/stdc++.h>
using namespace std;


class StatiClass {
    
    static StatiClass* m_instance;
    
    StatiClass () {
        std::cout << "StatiClass::Ctor \n";
    }
    
    ~ StatiClass() {
        std::cout << "StatiClass::Dtor \n";
    }
    public:
    
    static int number;
    
    static int displayNumber() {
        return number;
    }
    
    static StatiClass* getInstance () {
        
        if (m_instance == nullptr) {
            m_instance = new StatiClass();
        }
        return m_instance;
    }
    
    static void deleteInstance() {
        if (m_instance) {
            delete m_instance;
            m_instance = nullptr;
        }
    }
    
};

int StatiClass::number = 185525;
StatiClass* StatiClass::m_instance {nullptr};


int main() {
	// your code goes here
	//std::cout << "Variable static " << StatiClass::number << std::endl;
	
	//StatiClass obj;
	//std::cout << obj.displayNumber() << std::endl;
	
	StatiClass::getInstance(); 
	//obj.deleteInstance();
	StatiClass::deleteInstance();
	

}
