#include <stdio.h>
#include <time.h>
#include <windows.h>

#ifndef BCL_CDECL_TIME24DLL_H
#define BCL_CDECL_TIME24DLL_H

#ifdef  __cplusplus
#include <bcl/cdecl/time24dll.hpp>
#else 
extern "C" {

#define DllExport				__stdcall

/*****************************************************
*
*	�\����
*
*****************************************************/
typedef struct {
	int			tm_year;
	int			tm_mon;
	int			tm_mday;
	int			tm_hour;
	int			tm_min;
	int			tm_sec;
} tm24_t;


/*****************************************************
*
*	�v���g�^�C�v�錾
*
*****************************************************/
int DllExport dll_getNowTime(time_t *);				/* ���ݎ������擾  �߂� 0=Success  -1=Error */
int DllExport dll_getNowTime24(tm24_t *);			/* ���ݎ������擾  �߂� 0=Success  -1=Error */
int DllExport dll_getNowTime24_0(tm24_t *);			/* ���ݎ������擾�i�b�؂�̂�)  �߂� 0=Success  -1=Error */
int DllExport dll_localtime24(time_t, tm24_t *);	/* Dot����(�����W����)�������\����tm24_t(���{�W����)�ɕύX����  �߂� 0=Success  -1=Error */
int DllExport dll_localtime24_0(time_t, tm24_t *);	/* Dot����(�����W���� �b�؂�̂�)�������\����tm24_t(���{�W����)�ɕύX����  �߂� 0=Success  -1=Error */
int DllExport dll_mktime24(tm24_t *, time_t *);		/* �����\����tm24_t(���{�W����)��Dot����(�����W����)�ɕύX����  �߂� 0=Success  -1=Error */
int DllExport dll_timeAddSec(tm24_t *, int);		/* �b�P�ʂŎ��Ԃ�������  �߂� 0=Success  -1=Error */
int DllExport dll_timeAddMin(tm24_t *, int);		/* ���P�ʂŎ��Ԃ�������  �߂� 0=Success  -1=Error */
int DllExport dll_timeAddHour(tm24_t *, int);		/* ���P�ʂŎ��Ԃ�������  �߂� 0=Success  -1=Error */
int DllExport dll_timeAddDay(tm24_t *, int);		/* ���P�ʂŎ��Ԃ�������  �߂� 0=Success  -1=Error */

int DllExport dll_checkLeapYear(int);				/* �w�肳�ꂽ�N���[�N���`�F�b�N����  �߂� 0=�[�N�ł͂Ȃ�  1=�[�N */
int DllExport dll_getMaxDay(int, int);				/* �w�肳�ꂽ�N���̍ő�������擾����  �߂� 0=<�ő����  -1=Error */

#ifdef  __cplusplus
}
#endif


#endif

#endif // BCL_CDECL_TIME24DLL_H