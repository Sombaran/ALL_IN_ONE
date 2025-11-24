/*
Creating name space for reading parameters from UCI
*/

#include <iostream>

namespace alexaClientSDK {
namespace avsCommon {
namespace utils {
namespace uci {

/* Function to get/read a parameter from UCI configuration */
int GetparamUCI(std::string configuration, std::string param, std::string& rstring);

/* Function to get/read a parameter from UCI configuration */
int SetparamUCI(std::string configuration, std::string param, std::string rstring);

}
}
}
}
