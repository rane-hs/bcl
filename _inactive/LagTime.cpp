///////////////////////////////////////////////////////////////////////////
//
//  土木総合防災情報システム－ラグタイムクラス
//
////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CLagTime.h"


/*-------------------------------------------------------------------
*	機能：コンストラクタ
*	引数：
*	戻値：
*	説明：
-------------------------------------------------------------------*/
CLagTime::CLagTime()
{

	memset(&inf, 0, sizeof(inf));
	lag_minus = 0;
	lag_plus  = 0;
}

/*-------------------------------------------------------------------
*	機能：デスラクタ
*	引数：
*	戻値：
*	説明：
-------------------------------------------------------------------*/
CLagTime::~CLagTime()
{

}


/*-------------------------------------------------------------------
*	機能：初期化
*	引数：no				[IN]  方路番号
        ：m1				[IN]  ラグタイム(－秒)
        ：m2				[IN]  ラグタイム(＋秒)
*	戻値：成否
        ：   0 = 成功
*	説明：
-------------------------------------------------------------------*/
int CLagTime::init(int no, int m1, int m2)
{

	// 方路番号を格納
	houroNo = no;

	// ラグタイム情報を格納する
	lag_minus = m1;
	lag_plus  = m2;

	return 0;
}

/*-------------------------------------------------------------------
*	機能：判定処理
*	引数：
*	戻値：成否
        ：   1 = ラグタイム中
        ：   0 = ラグタイム以外
*	説明：
-------------------------------------------------------------------*/
int CLagTime::judgment()
{
	time_t	now_dot;
	time_t	lag_dot;
	time_t	start_dot;
	time_t	end_dot;
	tm24_t	tm24_now;
	tm24_t	tm24_wk;
	time_t	tm;

	// 現在時刻取得
	dll_getNowTime24(&tm24_now);
	// 現在時刻よりラグタイム・チェックを行う。
	memcpy(&tm24_wk, &tm24_now, sizeof(tm24_wk));
	// 分を10, 20, 30, 40, 50, 00にする
	if ((tm24_wk.tm_min % 10) > (lag_plus / 60)) {
		tm24_wk.tm_min = tm24_wk.tm_min + (10 - (tm24_wk.tm_min % 10));
		if (tm24_wk.tm_min >= 60) {
			dll_mktime24(&tm24_wk, &tm);
			dll_localtime24_0(tm, &tm24_wk);
		}
	} else {
		tm24_wk.tm_min = ((tm24_wk.tm_min / 10) * 10);
	}
	tm24_wk.tm_sec = 0;
	if ((tm24_wk.tm_hour == 0) && (tm24_wk.tm_min == 0)) {
		// 24:00にする
		dll_timeAddMin(&tm24_wk, -10);
		tm24_wk.tm_hour = 24;
		tm24_wk.tm_min  = 00;
	}
	// 現在時刻をDOTへ変換
	dll_mktime24(&tm24_now, &now_dot);
	// 正定時時刻をDOTへ変換
	dll_mktime24(&tm24_wk, &lag_dot);
	// 開始、終了ラグタイムを作成
	start_dot = lag_dot + lag_minus;
	end_dot   = lag_dot + lag_plus;
	if ((start_dot <= now_dot) && (now_dot <= end_dot)) {
		// ラグタイム中
		inf.flg = 1;
	} else {
		// ラグタイム外
		inf.flg = 0;
	}

	// 観測時刻
	inf.time24 = tm24_wk;
	dll_mktime24(&inf.time24, &inf.time);
	return inf.flg;
}
