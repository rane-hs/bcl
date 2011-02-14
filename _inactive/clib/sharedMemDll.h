#ifndef _SHAREDMEMORYDLL_
#define _SHAREDMEMORYDLL_

#ifdef  __cplusplus
extern "C" {
#endif

#define DllExport				__stdcall

#define DF_CONSOLECLASS  "ConsoleWindowClass"

typedef struct {
	HANDLE		hd;		/* 共有メモリハンドル */
	char		*mem;	/* 共有メモリ先頭ポインタ(各自でキャストする) */
} sharedMem_t;

/*
 *
 *
 *		プロトタイプ宣言
 *
 */
/*
 * 機能		：共有メモリ作成
 *
 * 引数		：[OUT] pt	 - 共有メモリハンドルポインタ
 *			  [IN] nst   - 共有メモリ名
 *			  [IN] len	 - サイズ(サイズ)
 *
 * 戻り値	：[ 0] 成功
 *			  [-1] ID取得エラー
 *                注意！
 *                     共有メモリハンドル領域(sharedMem_t)の実領域は、呼び出し元で
 *                     確保する事。
 * 説明		：
 */
int	DllExport dll_createSharedMemory(sharedMem_t *, char *, int);
/*
 * 機能		：共有メモリオープン
 *
 * 引数		：[OUT] pt	 - 共有メモリハンドルポインタ
 *			  [IN] nst   - 共有メモリ名
 *
 * 戻り値	：[0]  成功
 *			  [-1] 領域確保失敗, ID獲得エラー, ポインタ獲得エラー
 *
 * 説明		：
 */
int DllExport dll_openSharedMemory(sharedMem_t *, char *);
/*
 * 機能		：共有メモリ解放
 *
 * 引数		：[IN] pt	 - 共有メモリハンドルポインタ
 *
 * 戻り値	：[ 0] アンマップ成功
 *			  [-1] アンマップ失敗
 *
 * 説明		：
 */
int DllExport dll_freeSharedMemory(sharedMem_t *);
/*
 * 機能		：BYTE書き込み
 *
 * 引数		：[IN] pt	  - 共有メモリハンドルポインタ
 *            [IN] offset - 先頭ポインタからの距離(BYTE)
 *            [IN] buf    - 書き込む値
 *
 * 戻り値	：なし
 *
 * 説明		：
 */
void DllExport dll_setByte(sharedMem_t *, int, char);
/*
 * 機能		：BYTE読み込み
 *
 * 引数		：[IN]  pt	   - 共有メモリハンドルポインタ
 *            [IN]  offset - 先頭ポインタからの距離(BYTE)
 *            [OUT] buf    - 読み込み値格納領域
 *
 * 戻り値	：なし
 *
 * 説明		：
 */
void DllExport dll_getByte(sharedMem_t *, int, char *);
/*
 * 機能		：WORD書き込み
 *
 * 引数		：[IN] pt	  - 共有メモリハンドルポインタ
 *            [IN] offset - 先頭ポインタからの距離(BYTE)
 *            [IN] buf    - 書き込む値
 *
 * 戻り値	：なし
 *
 * 説明		：
 */
void DllExport dll_setWord(sharedMem_t *, int, WORD);
/*
 * 機能		：WORD読み込み
 *
 * 引数		：[IN]  pt	   - 共有メモリハンドルポインタ
 *            [IN]  offset - 先頭ポインタからの距離(BYTE)
 *            [OUT] buf    - 読み込み値格納領域
 *
 * 戻り値	：なし
 *
 * 説明		：
 */
void DllExport dll_getWord(sharedMem_t *, int, WORD *);
/*
 * 機能		：LONG書き込み
 *
 * 引数		：[IN] pt	  - 共有メモリハンドルポインタ
 *            [IN] offset - 先頭ポインタからの距離(BYTE)
 *            [IN] buf    - 書き込む値
 *
 * 戻り値	：なし
 *
 * 説明		：
 */
void DllExport dll_setLong(sharedMem_t *, int, LONG);
/*
 * 機能		：LONG書き込み
 *
 * 引数		：[IN]  pt	   - 共有メモリハンドルポインタ
 *            [IN]  offset - 先頭ポインタからの距離(BYTE)
 *            [OUT] buf    - 読み込み値格納領域
 *
 * 戻り値	：なし
 *
 * 説明		：
 */
void DllExport dll_getLong(sharedMem_t *, int, LONG *);
/*
 * 機能		：指定バイト書き込み
 *
 * 引数		：[IN] pt	  - 共有メモリハンドルポインタ
 *            [IN] offset - 先頭ポインタからの距離(BYTE)
 *            [IN] len    - 書き込みサイズ(BYTE)
 *            [IN] buf    - 書き込む値
 *
 * 戻り値	：なし
 *
 * 説明		：
 */
void DllExport dll_setSharedMemorys(sharedMem_t *, int, int, char *);
/*
 * 機能		：指定バイト読み込み
 *
 * 引数		：[IN] pt	  - 共有メモリハンドルポインタ
 *            [IN] offset - 先頭ポインタからの距離(BYTE)
 *            [IN] len    - 読み込みサイズ(BYTE)
 *            [IN] buf    - 読み込み値格納領域
 *
 * 戻り値	：なし
 *
 * 説明		：
 */
void DllExport dll_getSharedMemorys(sharedMem_t *, int, int, char *);
/*
 * 機能		：コンソールのウィンドウハンドル取得
 *
 * 引数		：[IN] lpszWindow - ウィンドウタイトル名
 *
 * 戻り値	：ウィンドウハンドル
 *
 * 説明		：本関数は、コンソールアプリケーションの起動直後に読み込んでください。
 */
HWND DllExport dll_getConsoleHandle();
/*
 * 機能		：ウィンドウを表示
 *
 * 引数		：[IN] hwnd - ウィンドウハンドル
 *
 * 戻り値	：[ TRUE] - ウィンドウが以前に表示されていた場合
 *					[FALSE] - 非表示だった場合
 *
 * 説明		：
 */
BOOL DllExport dll_showWindow(HWND);
/*
 * 機能		：ウィンドウ非表示
 *
 * 引数		：[IN] hwnd - ウィンドウハンドル
 *
 * 戻り値	：[ TRUE] - ウィンドウが以前に表示されていた場合
 *					[FALSE] - 非表示だった場合
 *
 * 説明		：
 */
BOOL DllExport dll_hideWindow(HWND);


#ifdef  __cplusplus
}
#endif

#endif


