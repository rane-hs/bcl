#ifndef BCL_WIN32_CONSOLE_WINDOW_H
#define BCL_WIN32_CONSOLE_WINDOW_H

namespace bcl{

/*
 * 機能		：コンソールのウィンドウハンドル取得
 * 戻り値	：ウィンドウハンドル
 */
inline HWND getConsoleHandle() /* char *lpszWindow) */
{
	/* プロセスID取得 */
	DWORD myPid = GetCurrentProcessId();
	
	/* 先頭ウィンドウハンドル取得 */
	HWND hWnd = GetTopWindow(NULL);
	while (hWnd){
		DWORD wkPid;
		/* ウィンドウハンドルのプロセスID取得 */
		GetWindowThreadProcessId(hWnd,&wkPid);
		if (myPid == wkPid)
			break;
		
		/* 次のウィンドウハンドル取得 */
		hWnd = GetWindow(hWnd,GW_HWNDNEXT);
	}
	
	return hWnd;

	/* return(FindWindow(DF_CONSOLECLASS, lpszWindow)); */
}

/*
 * 機能		：ウィンドウを表示
 * 引数		：[IN] hwnd - ウィンドウハンドル
 */
inline bool showWindow(HWND hwnd)
{
	return (TRUE==ShowWindow(hwnd,SW_RESTORE)) ? true:false;
}

/*
 * 機能		：ウィンドウ非表示
 * 引数		：[IN] hwnd - ウィンドウハンドル
 */
inline bool hideWindow(HWND hwnd)
{
	return (TRUE == ShowWindow(hwnd,SW_HIDE))? true:false;
}

};	//bcl::


#endif // BCL_WIN32_CONSOLE_WINDOW_H