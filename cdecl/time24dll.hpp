#ifndef BCL_CDECL_TIME24DLL_HPP
#define BCL_CDECL_TIME24DLL_HPP

#include <bcl/Time24.h>
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

}

inline void localtime24(const time_t &src, tm24_t *dest)
{
	if(dest == NULL) return;
	const bcl::time24 src24(src);
	dest->tm_year = src24.year();
	dest->tm_mon  = src24.month();
	dest->tm_mday = src24.day();
	dest->tm_hour = src24.hour();
	dest->tm_min  = src24.minute();
	dest->tm_sec  = src24.second();
}
inline void localtime24_0(const time_t &src, tm24_t *dest)
{
	time_t	src_ = src / 60;
	return localtime24(src_*60, dest);
}


#endif