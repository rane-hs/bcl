#include "stdafx.h"
#include "inetHttp.h"
#pragma comment(lib, "wininet.lib")

int INetHttp::Connect(){
	hSession = InternetOpen("file", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if(hSession != NULL)
		return 0;
	else
		return lastErr = GetLastError();
};
int INetHttp::Disconnect(){
	BOOL	bRet;
	if ( hSession ) {
		bRet = InternetCloseHandle(hSession);
	}
	hSession = NULL;
	if(bRet == TRUE)
		return 0;
	else
		return lastErr = GetLastError();
};

int INetHttp::Connect(const char *proxySrv, const int port){
	CString csBuf;
	if(strcmp("auto", proxySrv) == 0)
		hSession = InternetOpen("file", INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, NULL, NULL, 0);// プロキシ使用
	else{
		csBuf.Format("%s:%d", proxySrv, port);
		hSession = InternetOpen("file", INTERNET_OPEN_TYPE_PROXY, csBuf.GetBuffer(0), "<local>", 0);// プロキシ使用
	}
	if(hSession)
		return 0;
	else
		return -1;
};

/*!-------------------------------------------------------
*	機能： ＵＲＬデータの取得＆ファイルに保存
*	引数： url               [IN]  ＵＲＬ名
*		： fpath             [IN]  保存ファイルパス名
*	戻り： 0(成功) -1(失敗)
*	備考：
*	履歴： [新規]2009-07-17 H.Seki
*		： [変更]xxxx-xx-xx xxx
--------------------------------------------------------*/
int INetHttp::Url2File(const char *url, const char *fpath)
{
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
	// ＵＲＬデータ取得＆ファイルの保存
	fsize = 0;
	fp = fopen(fpath, "wb");
	if (fp == NULL) {
		//データファイルオープン失敗
		return -1;
	}
	while (1) {
		ret = InternetReadFile( hService, rbuf, 1024, &rbyte );
		if ((ret == FALSE) || (rbyte == 0))
			break;
		fwrite( rbuf, rbyte, 1, fp );
		fsize = fsize + rbyte;
	}
	fclose(fp);
	if ((ret == FALSE) || (fsize == 0)) {
		//データ読み込み失敗
		lastErr = GetLastError();
		return -1;
	}

	InternetCloseHandle(hService);

	return 0;
}
