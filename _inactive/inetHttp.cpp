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
		hSession = InternetOpen("file", INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, NULL, NULL, 0);// �v���L�V�g�p
	else{
		csBuf.Format("%s:%d", proxySrv, port);
		hSession = InternetOpen("file", INTERNET_OPEN_TYPE_PROXY, csBuf.GetBuffer(0), "<local>", 0);// �v���L�V�g�p
	}
	if(hSession)
		return 0;
	else
		return -1;
};

/*!-------------------------------------------------------
*	�@�\�F �t�q�k�f�[�^�̎擾���t�@�C���ɕۑ�
*	�����F url               [IN]  �t�q�k��
*		�F fpath             [IN]  �ۑ��t�@�C���p�X��
*	�߂�F 0(����) -1(���s)
*	���l�F
*	�����F [�V�K]2009-07-17 H.Seki
*		�F [�ύX]xxxx-xx-xx xxx
--------------------------------------------------------*/
int INetHttp::Url2File(const char *url, const char *fpath)
{
	HINTERNET hService;
	FILE *fp;
	int fsize;
	unsigned long rbyte;
	char rbuf[1024];
	int ret;

	// URL�̃I�[�v��
	hService = InternetOpenUrl( hSession, url, NULL, 0, INTERNET_FLAG_RELOAD, 0 );
	if ( !hService ) {
		lastErr = GetLastError();
		//�t�q�k�I�[�v�����s
		return -1;
	}
	// �t�q�k�f�[�^�擾���t�@�C���̕ۑ�
	fsize = 0;
	fp = fopen(fpath, "wb");
	if (fp == NULL) {
		//�f�[�^�t�@�C���I�[�v�����s
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
		//�f�[�^�ǂݍ��ݎ��s
		lastErr = GetLastError();
		return -1;
	}

	InternetCloseHandle(hService);

	return 0;
}
