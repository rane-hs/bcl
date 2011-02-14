#include <stdio.h>
#include <windows.h>
#include "timeDll.h"

#ifndef _SYSLOGDLLL_H_
#define _SYSLOGDLLL_H_

#define DllExport				__stdcall

/*****************************************************
*
*	構造体
*
*****************************************************/
typedef struct {
	short ino;   /* エラー項目番号 */
	struct {
	   short yyyy;       /* 年 */
	   char  mm;         /* 月 */
	   char  dd;         /* 日 */
	   char  hh;         /* 時 */  /* 装置異常は 0時制、機器状態、警報状態は 24時制 */
	   char  nn;         /* 分 */
       char  ss;         /* 秒 */
       char  dummy;
    }t[2];               /* [0]:発生時刻 [1]:復帰時刻 */
}slg_hist_t;

/*****************************************************
*
*	プロトタイプ宣言
*
*****************************************************/
int DllExport dll_slg_set_m(int , time_t );			/* 装置異常 登録 */
int DllExport dll_slg_reset_m(int, time_t);			/* 装置異常 復帰 */
int DllExport dll_slg_getbit_m(unsigned char *);	/* 装置異常 状況発生ビット取得 */
int DllExport dll_slg_get_m(int, time_t *);			/* 装置異常 状況取得（詳細）   */
int DllExport dll_slg_gethist_m(slg_hist_t *, int);	/* 装置異常 履歴取得 */

int DllExport dll_slg_set_h(int, int, int, time_t );	/* 機器状態 登録 */
int DllExport dll_slg_reset_h(int, int, int, time_t);	/* 機器状態 復帰 */
int DllExport dll_slg_getbit_h(int, unsigned char *);	/* 機器状態 状況発生ビット取得 */
int DllExport dll_slg_get_h(int, int, int, time_t *);	/* 機器状態 状況取得（詳細）   */
int DllExport dll_slg_gethist_h(int, int, slg_hist_t *, int);	/* 機器状態 履歴取得 */

int DllExport dll_slg_set_a(int, int, int, time_t );	/* 警報状態 登録 */
int DllExport dll_slg_reset_a(int, int, int, time_t);	/* 警報状態 復帰 */
int DllExport dll_slg_getbit_a(int, unsigned char *);	/* 警報状態 状況発生ビット取得 */
int DllExport dll_slg_get_a(int, int, int, time_t *);	/* 警報状態 状況取得（詳細）   */
int DllExport dll_slg_gethist_a(int, int, slg_hist_t *, int);	/* 警報状態 履歴取得 */

#endif
