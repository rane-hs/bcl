#include <windows.h>
#include <stdio.h>

#ifndef _LOCKMUTEX_
#define _LOCKMUTEX_

#ifdef  __cplusplus
extern "C" {
#endif

/***********************************************
*
*	定数
*
***********************************************/
#define DllExport				__stdcall

#define	DF_INFINITE			0xFFFFFFFF		//タイムアウトなし

/***********************************************
*
*	プロトタイプ宣言
*
***********************************************/
/********************************************
*
*	Lock作成
*
*	sa		: セキュリティ属性 (デフォルト:NULL)
*	lName	: Lock名前 最大128文字
*	pLock	: Mutexハンドル
*
*	戻り	: FALSE:error	TRUE:success
*
********************************************/
BOOL DllExport dll_createLock(SECURITY_ATTRIBUTES *, char *, HANDLE *);
/********************************************
*
*	CloseLock
*
*	pLock	: Mutexハンドル
*
********************************************/
void DllExport dll_closeLock(HANDLE);
/********************************************
*
*	SetLock
*
*	pLock	: Mutexハンドル
*	timeout	: 待ち時間(ﾐﾘ秒)
*
*	戻り	: TRUE:sucees	FALSE:error
*
********************************************/
BOOL DllExport dll_setLock(HANDLE, DWORD);
/********************************************
*
*	LockRelease
*
*	pLock	: Mutexハンドル
*
*	戻り	: TRUE:sucees	FALSE:error
*
********************************************/
BOOL DllExport dll_releaseLock(HANDLE);

#ifdef  __cplusplus
}
#endif

#endif
