#include <stdio.h>
#include <windows.h>
#include "timeDll.h"

#ifndef _SYSLOGDLLL_H_
#define _SYSLOGDLLL_H_

#define DllExport				__stdcall

/*****************************************************
*
*	�\����
*
*****************************************************/
typedef struct {
	short ino;   /* �G���[���ڔԍ� */
	struct {
	   short yyyy;       /* �N */
	   char  mm;         /* �� */
	   char  dd;         /* �� */
	   char  hh;         /* �� */  /* ���u�ُ�� 0�����A�@���ԁA�x���Ԃ� 24���� */
	   char  nn;         /* �� */
       char  ss;         /* �b */
       char  dummy;
    }t[2];               /* [0]:�������� [1]:���A���� */
}slg_hist_t;

/*****************************************************
*
*	�v���g�^�C�v�錾
*
*****************************************************/
int DllExport dll_slg_set_m(int , time_t );			/* ���u�ُ� �o�^ */
int DllExport dll_slg_reset_m(int, time_t);			/* ���u�ُ� ���A */
int DllExport dll_slg_getbit_m(unsigned char *);	/* ���u�ُ� �󋵔����r�b�g�擾 */
int DllExport dll_slg_get_m(int, time_t *);			/* ���u�ُ� �󋵎擾�i�ڍׁj   */
int DllExport dll_slg_gethist_m(slg_hist_t *, int);	/* ���u�ُ� �����擾 */

int DllExport dll_slg_set_h(int, int, int, time_t );	/* �@���� �o�^ */
int DllExport dll_slg_reset_h(int, int, int, time_t);	/* �@���� ���A */
int DllExport dll_slg_getbit_h(int, unsigned char *);	/* �@���� �󋵔����r�b�g�擾 */
int DllExport dll_slg_get_h(int, int, int, time_t *);	/* �@���� �󋵎擾�i�ڍׁj   */
int DllExport dll_slg_gethist_h(int, int, slg_hist_t *, int);	/* �@���� �����擾 */

int DllExport dll_slg_set_a(int, int, int, time_t );	/* �x���� �o�^ */
int DllExport dll_slg_reset_a(int, int, int, time_t);	/* �x���� ���A */
int DllExport dll_slg_getbit_a(int, unsigned char *);	/* �x���� �󋵔����r�b�g�擾 */
int DllExport dll_slg_get_a(int, int, int, time_t *);	/* �x���� �󋵎擾�i�ڍׁj   */
int DllExport dll_slg_gethist_a(int, int, slg_hist_t *, int);	/* �x���� �����擾 */

#endif
