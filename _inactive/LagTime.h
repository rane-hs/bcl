///////////////////////////////////////////////////////////////////////////
//*	機能  ：ラグタイムクラス
//  更新履歴：
//          ：
//          ：
//          ：
//          ：
///////////////////////////////////////////////////////////////////////////
#ifndef _CLAG_TIME_H_
#define _CLAG_TIME_H_

#include <stdio.h>

#ifdef  __cplusplus
extern "C" {
#endif

#include "./clib/timeDll.h"

#ifdef  __cplusplus
}
#endif



/*-------------------------------------------------------------------
  構造体：ラグタイム情報
  項目  ：flg					ラグタイムフラグ
        ：time					観測時刻
-------------------------------------------------------------------*/
typedef struct {
	int		flg;				// ラグタイムフラグ
	time_t	time;				// 観測時刻(time_t型)
	tm24_t	time24;				// 観測時刻(tm24_t型)
} lag_time_t;

/*-------------------------------------------------------------------
  クラス定義
-------------------------------------------------------------------*/
class CLagTime {
public:
	lag_time_t	inf;			// ラグタイム情報
	int			houroNo;		// 方路番号
	int			lag_minus;		// ラグタイム(−秒)
	int			lag_plus;		// ラグタイム(＋秒)

	// コンストラクタ
	CLagTime();
	// デスラクタ
	~CLagTime();

	// 初期化
	int	init(int no, int m1, int m2);
	// ラグタイム判定
	int	judgment();
};

#endif
