#ifndef BCL_WIN32_CONSOLE_WINDOW_H
#define BCL_WIN32_CONSOLE_WINDOW_H

namespace bcl{

/*
 * �@�\		�F�R���\�[���̃E�B���h�E�n���h���擾
 * �߂�l	�F�E�B���h�E�n���h��
 */
inline HWND getConsoleHandle() /* char *lpszWindow) */
{
	/* �v���Z�XID�擾 */
	DWORD myPid = GetCurrentProcessId();
	
	/* �擪�E�B���h�E�n���h���擾 */
	HWND hWnd = GetTopWindow(NULL);
	while (hWnd){
		DWORD wkPid;
		/* �E�B���h�E�n���h���̃v���Z�XID�擾 */
		GetWindowThreadProcessId(hWnd,&wkPid);
		if (myPid == wkPid)
			break;
		
		/* ���̃E�B���h�E�n���h���擾 */
		hWnd = GetWindow(hWnd,GW_HWNDNEXT);
	}
	
	return hWnd;

	/* return(FindWindow(DF_CONSOLECLASS, lpszWindow)); */
}

/*
 * �@�\		�F�E�B���h�E��\��
 * ����		�F[IN] hwnd - �E�B���h�E�n���h��
 */
inline bool showWindow(HWND hwnd)
{
	return (TRUE==ShowWindow(hwnd,SW_RESTORE)) ? true:false;
}

/*
 * �@�\		�F�E�B���h�E��\��
 * ����		�F[IN] hwnd - �E�B���h�E�n���h��
 */
inline bool hideWindow(HWND hwnd)
{
	return (TRUE == ShowWindow(hwnd,SW_HIDE))? true:false;
}

};	//bcl::


#endif // BCL_WIN32_CONSOLE_WINDOW_H