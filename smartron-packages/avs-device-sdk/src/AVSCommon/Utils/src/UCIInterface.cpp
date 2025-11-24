/*
Creating name space for reading parameters from UCI CONFIGURATIONS
*/

#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <cmath>
#include "AVSCommon/Utils/UCIInterface.h"
#include <AVSCommon/Utils/Logger/Logger.h>

namespace alexaClientSDK {
namespace avsCommon {
namespace utils {
namespace uci {

using namespace std;

#define LX(event) alexaClientSDK::avsCommon::utils::logger::LogEntry("UCIInterface", event)

#define CMD_BUF_LENGTH  100
#define VAL_BUF_LENGTH  100

int GetparamUCI(std::string configuration, std::string param, std::string& rstring) {

	char cmd_buf[CMD_BUF_LENGTH] = {0};
	char val_buf[VAL_BUF_LENGTH] = {0};
	
	/*file pointer to open a uci params*/
	FILE *fp;

	/* making command to read from UCI */
	sprintf(cmd_buf, "uci get %s.%s", configuration.c_str(), param.c_str());
	
	/* Opening a file */
	fp = popen(cmd_buf, "r");
	if (fp == NULL) {
		ACSDK_CRITICAL(LX("GetparamUCIfailed").d("reason", "failed to open file"));
		return -1;
	}

	if(fgets(val_buf, VAL_BUF_LENGTH, fp) == NULL)
	{
		ACSDK_CRITICAL(LX("GetparamUCIfailed").d("reason", "failed to read from file"));
		pclose(fp);
		return -1;
	}
	pclose(fp);
	
	/* find the parameter value is empty or not */
	if(!strlen(val_buf))
	{
		ACSDK_CRITICAL(LX("GetparamUCIfailed").d("reason", "empty variable"));
		return -1;
	}
	
	rstring = std::string(val_buf);
	
	return 0;
}

/* Set Shifu specific configuration to UCI system */
int SetparamUCI(std::string configuration, std::string param, std::string rstring)
{
	char cmd_buf[CMD_BUF_LENGTH] = {0};
	
	/* making command to write to UCI */
	sprintf(cmd_buf, "uci set %s.%s=%s", configuration.c_str(), param.c_str(), rstring.c_str());
	
	if (system(cmd_buf) < 0 )
	{
		ACSDK_CRITICAL(LX("SetparamUCIfailed").d("reason", "failed create command ").d("cmd_buf", cmd_buf));
		return -1;
	}
	system("uci commit shifu");
	return 0;
}

}
}
}
}

