#include <windows.h>
#include <stdio.h>

#ifndef _LOCKMUTEX_
#define _LOCKMUTEX_

#ifdef  __cplusplus
extern "C" {
#endif

/***********************************************
*
*	�萔
*
***********************************************/
#define DllExport				__stdcall

#define	DF_INFINITE			0xFFFFFFFF		//�^�C���A�E�g�Ȃ�

/***********************************************
*
*	�v���g�^�C�v�錾
*
***********************************************/
/********************************************
*
*	Lock�쐬
*
*	sa		: �Z�L�����e�B���� (�f�t�H���g:NULL)
*	lName	: Lock���O �ő�128����
*	pLock	: Mutex�n���h��
*
*	�߂�	: FALSE:error	TRUE:success
*
********************************************/
BOOL DllExport dll_createLock(SECURITY_ATTRIBUTES *, char *, HANDLE *);
/********************************************
*
*	CloseLock
*
*	pLock	: Mutex�n���h��
*
********************************************/
void DllExport dll_closeLock(HANDLE);
/********************************************
*
*	SetLock
*
*	pLock	: Mutex�n���h��
*	timeout	: �҂�����(�ؕb)
*
*	�߂�	: TRUE:sucees	FALSE:error
*
********************************************/
BOOL DllExport dll_setLock(HANDLE, DWORD);
/********************************************
*
*	LockRelease
*
*	pLock	: Mutex�n���h��
*
*	�߂�	: TRUE:sucees	FALSE:error
*
********************************************/
BOOL DllExport dll_releaseLock(HANDLE);

#ifdef  __cplusplus
}
#endif

#endif
