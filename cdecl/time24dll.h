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
*	構造体
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
*	プロトタイプ宣言
*
*****************************************************/
int DllExport dll_getNowTime(time_t *);				/* 現在時刻を取得  戻り 0=Success  -1=Error */
int DllExport dll_getNowTime24(tm24_t *);			/* 現在時刻を取得  戻り 0=Success  -1=Error */
int DllExport dll_getNowTime24_0(tm24_t *);			/* 現在時刻を取得（秒切り捨て)  戻り 0=Success  -1=Error */
int DllExport dll_localtime24(time_t, tm24_t *);	/* Dot時刻(万国標準時)を時刻構造体tm24_t(日本標準時)に変更する  戻り 0=Success  -1=Error */
int DllExport dll_localtime24_0(time_t, tm24_t *);	/* Dot時刻(万国標準時 秒切り捨て)を時刻構造体tm24_t(日本標準時)に変更する  戻り 0=Success  -1=Error */
int DllExport dll_mktime24(tm24_t *, time_t *);		/* 時刻構造体tm24_t(日本標準時)をDot時刻(万国標準時)に変更する  戻り 0=Success  -1=Error */
int DllExport dll_timeAddSec(tm24_t *, int);		/* 秒単位で時間を加える  戻り 0=Success  -1=Error */
int DllExport dll_timeAddMin(tm24_t *, int);		/* 分単位で時間を加える  戻り 0=Success  -1=Error */
int DllExport dll_timeAddHour(tm24_t *, int);		/* 時単位で時間を加える  戻り 0=Success  -1=Error */
int DllExport dll_timeAddDay(tm24_t *, int);		/* 日単位で時間を加える  戻り 0=Success  -1=Error */

int DllExport dll_checkLeapYear(int);				/* 指定された年が閏年かチェックする  戻り 0=閏年ではない  1=閏年 */
int DllExport dll_getMaxDay(int, int);				/* 指定された年月の最大日数を取得する  戻り 0=<最大日数  -1=Error */

#ifdef  __cplusplus
}
#endif


#endif

#endif // BCL_CDECL_TIME24DLL_H