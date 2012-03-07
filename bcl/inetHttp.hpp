#ifndef BCL_INET_HTTP__H
#define BCL_INET_HTTP__H

#include <windows.h>
#include <wininet.h>

#include <string>

#ifdef BCL_INET_HTTP_AUTOLINK
#pragma comment(lib, "wininet.lib")
#endif

namespace bcl{

	class INetHttp{
	private:
		HINTERNET hSession;
		int lastErr;
		std::string		agent;
	public:
		//ctor, dtor
		INetHttp(const char *setAgent = "inet http connect"){agent = setAgent;}
		~INetHttp(){ if(hSession) Disconnect();};
		//accessor
		const char getlasterr(){return lastErr;};
		//connect, disconnect
		const char Connect(){
			hSession = InternetOpen(agent.c_str(), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
			if(hSession != NULL)
				return 0;
			else
				return lastErr = GetLastError();
		};
		const char Disconnect(){
			BOOL	bRet = TRUE;
			if ( hSession ) {
				bRet = InternetCloseHandle(hSession);
			}
			hSession = NULL;
			if(bRet == TRUE)
				return 0;
			else
				return lastErr = GetLastError();
		};
		const char Connect(const char *proxySrv, const int port){
			if(strcmp(agent.c_str(), proxySrv) == 0)
				hSession = InternetOpen(agent.c_str(), INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, NULL, NULL, 0);// プロキシ使用
			else{
				std::string strBuf = bcl::format("%s:%d", proxySrv, port);
				hSession = InternetOpen(agent.c_str(), INTERNET_OPEN_TYPE_PROXY, strBuf.c_str(), "<local>", 0);// プロキシ使用
			}
			if(hSession)
				return 0;
			else
				return -1;
		};
	;
		//download files
		const char Url2File(const char *url, const char *fpath = NULL){
			HINTERNET hService;
			FILE *fp;
			int fsize;
			unsigned long rbyte;
			char rbuf[1024];
			int ret;

			// URLのオープン
			hService = InternetOpenUrl( hSession, url, NULL, 0, INTERNET_FLAG_RELOAD, 0 );
			if ( !hService ) {
				lastErr = GetLastError();
				//ＵＲＬオープン失敗
				return -1;
			}
			if( fpath != NULL){
				// ＵＲＬデータ取得＆ファイルの保存
				fsize = 0;
				fp = fopen(fpath, "wb");
				if (fp == NULL) {
					InternetCloseHandle(hService);
					//データファイルオープン失敗
					return -1;
				}
				while (1) {
					ret = InternetReadFile( hService, rbuf, 1024, &rbyte );
					if ((ret == FALSE) || (rbyte == 0))
						break;
					fwrite( rbuf, rbyte, 1, fp );
					fsize = fsize + rbyte;
					Sleep(0);
				}
				fclose(fp);
				if ((ret == FALSE) || (fsize == 0)) {
					//データ読み込み失敗
					lastErr = GetLastError();
					InternetCloseHandle(hService);
					return -1;
				}
			}

			InternetCloseHandle(hService);
			Sleep(5);

			return 0;
		};
		const char Url2File2(const char *domain, const char *url, const char *fpath){
			FILE *fp;
			int fsize;
			unsigned long rbyte;
			char rbuf[1024];
			int ret;
			HINTERNET hHttpSession;
			HINTERNET hHttpRequest;

			// サーバへ接続
			hHttpSession = InternetConnect(
				hSession,
				domain,
				INTERNET_DEFAULT_HTTP_PORT,
				NULL,
				NULL,
				INTERNET_SERVICE_HTTP,
				0,
				0);
			if ( !hHttpSession ) {
				lastErr = GetLastError();
				return -1;
			}

			// HTTP要求の作成
			hHttpRequest = HttpOpenRequest(
				hHttpSession,
				"GET",
				url,
				NULL,
				NULL,
				NULL,
				INTERNET_FLAG_RELOAD,
				0);
			if ( !hHttpRequest ) {
				lastErr = GetLastError();
				InternetCloseHandle(hHttpSession);
				return -1;
			}

			// 作成したHTTP要求の発行
			ret = HttpSendRequest(
				hHttpRequest,
				NULL,
				0,
				NULL,
				0);

			if (ret) {
				// ＵＲＬデータ取得＆ファイルの保存
				fsize = 0;
				fp = fopen(fpath, "wb");
				if (fp == NULL) {
					lastErr = GetLastError();
					// ハンドルクローズ
					InternetCloseHandle(hHttpRequest);
					InternetCloseHandle(hHttpSession);
					return -1;
				}
				while (1) {
					ret = InternetReadFile( hHttpRequest, rbuf, 1024, &rbyte );
					if ((ret == FALSE) || (rbyte == 0))
						break;
					fwrite( rbuf, rbyte, 1, fp );
					fsize = fsize + rbyte;
				}
				fclose(fp);
				if ((ret == FALSE) || (fsize == 0)) {
					lastErr = GetLastError();
					// ハンドルクローズ
					InternetCloseHandle(hHttpRequest);
					InternetCloseHandle(hHttpSession);
					return -1;
				}
			}

			// ハンドルクローズ
			InternetCloseHandle(hHttpRequest);
			InternetCloseHandle(hHttpSession);

			return 0;
		};
	};
};		//bcl::
#endif // BCL_INET_HTTP__H