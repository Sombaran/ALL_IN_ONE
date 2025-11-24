



int CreateTable(void);
int InsertTable(const char *macID,const char *localIP, int internal_port,int external_port);
bool MacIDPresence(char *Mac);
char *GetIpAddress(char * Mac);
bool CheckPortNumberExistOrNot(int port);
bool DeleteRecord(char * Mac);

