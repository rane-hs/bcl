#ifndef _LOGDLL_
#define _LOGDLL_

#ifdef  __cplusplus
extern "C" {
#endif

/***********************************************
*
*	�萔
*
***********************************************/
#define DllExport				__stdcall


/*
 * �@�\		�F���O�t�@�C����������
 *
 * ����		�F[IN] fpt   - ���O�t�@�C���p�X
 *            [IN] fsize - �t�@�C���T�C�Y
 *            [IN] len   - �������݃f�[�^�T�C�Y
 *		      [IN] buf   - �������݃f�[�^
 *
 * �߂�l	�F�G���[���
 *					[-1] �G���[
 *					[ 0] ����
 */
int DllExport dll_writelog(char *, DWORD, int, char *);

#ifdef  __cplusplus
}
#endif

#endif



