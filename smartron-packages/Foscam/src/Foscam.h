#include <json-c/json.h>
#include<stdbool.h>

extern json_object* FoscamSearch(int);

extern bool RemoveFoscam(const char *mac);

extern bool RemoveFirewallEntry(char *ip);
