#ifndef _LOGDLL_
#define _LOGDLL_

#ifdef  __cplusplus
extern "C" {
#endif

/***********************************************
*
*	定数
*
***********************************************/
#define DllExport				__stdcall


/*
 * 機能		：ログファイル書き込み
 *
 * 引数		：[IN] fpt   - ログファイルパス
 *            [IN] fsize - ファイルサイズ
 *            [IN] len   - 書き込みデータサイズ
 *		      [IN] buf   - 書き込みデータ
 *
 * 戻り値	：エラー情報
 *					[-1] エラー
 *					[ 0] 正常
 */
int DllExport dll_writelog(char *, DWORD, int, char *);

#ifdef  __cplusplus
}
#endif

#endif



