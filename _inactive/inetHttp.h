#include <wininet.h>

class INetHttp{
private:
	HINTERNET hSession;
	int lastErr;
public:
	const int getlasterr(){return lastErr;};
	int Connect();
	int Disconnect();
	int Connect(const char *proxySrv, const int port);
	int Url2File(const char *url, const char *fpath);
	~INetHttp(){ if(hSession) Disconnect();};
};