#include "stdafx.h"
#include "memAccess.h"


/*-------------------------------------------------------------------
  ローカル関数宣言
-------------------------------------------------------------------*/
int  timeCmp(time_t t1, mem_obstime_t t2);
void obsToTime(mem_obstime_t *p, time_t *tm);
void timeToObs(time_t tm, mem_obstime_t *p);

mem_datamem_t *selectDataMemory(mem_manager_t *mp, int typ, int kno, time_t tim);
int  openDataMemory (mem_manager_t *mp, kind_manager_t *kp);
void closeDataMemory(mem_manager_t *mp);

void initDataMemoryHM  (mem_manager_t *mp, int kno, char *p);
void initDataMemoryDay (mem_manager_t *mp, int kno, char *p);
void initDataMemory    (mem_manager_t *mp, mem_datamem_t *dp, time_t tim);
int  writeLatestMemory (mem_manager_t *mp, int typ, int kno, time_t tim, int sno, char *dp);
int  writeDataMemoryNow(mem_manager_t *mp, int typ, int kno, time_t tim, mem_datamem_t *dp);
int  writeDataMemory   (mem_manager_t *mp, int typ, int kno, time_t tim, char *dp);
int  writeDataMemoryS  (mem_manager_t *mp, int typ, int kno, time_t tim, int sno, char *dp);
int  writeDataMemoryI  (mem_manager_t *mp, int typ, time_t tim, int cnt, mem_reqdata_t *dp);
int  readDataMemoryI   (mem_manager_t *mp, int typ, time_t tim, int cnt, mem_reqdata_t *dp);

mem_datamem_t *checkDataMemory (mem_manager_t *mp, int typ, int kno, time_t tim);
int getUpToDateTime(mem_manager_t *mp, int typ, int kno, time_t *);

//2003.06.23 追加（ここから）----------------------------------------
//既に更新されている局は上書きしないバージョン
int  writeDataMemoryS_nOW(mem_manager_t *mp, int typ, int kno, time_t tim, int sno, char *dp);
int  writeDataMemoryI_nOW(mem_manager_t *mp, int typ, time_t tim, int cnt, mem_reqdata_t *dp);
//2003.06.23 追加（ここまで）----------------------------------------


/*-------------------------------------------------------------------
  モジュール内変数
-------------------------------------------------------------------*/


/*---------------------------------------------------------
*	機能：
*	引数：t1            [IN]  指定時刻
        ：t2            [IN]  更新時刻
*	戻値：比較結果
        ：  -1 = 指定時刻 < 更新時刻
        ：   0 = 指定時刻 = 更新時刻
        ：   1 = 指定時刻 > 更新時刻
        ：   2 = 指定時刻 > 正時 > 更新時刻
*	説明：書き込むデータの指定時刻を
        ：更新情報メモリ内の更新時刻と比較します。
---------------------------------------------------------*/
int timeCmp(time_t t1, mem_obstime_t t2)
{
	tm24_t tm24;
	time_t tim;
	time_t ntim;
	int ret;

	//更新時刻が０なら
	if (t2.mm == 0 || t2.dd == 0) {
//2005.03.04 修正（ここから）----------------------------------------
//更新時刻が０なら正時を跨いでいる事とする
//		ntim = 0;
		return 2;
//2005.03.04 修正（ここまで）----------------------------------------
	//更新時刻が設定されている
	} else {
		tm24.tm_year = t2.yy;
		tm24.tm_mon  = t2.mm;
		tm24.tm_mday = t2.dd;
		tm24.tm_hour = t2.hh;
		tm24.tm_min  = t2.nn;
		tm24.tm_sec  = 0;
		ntim = 0;
		dll_mktime24(&tm24, &ntim);
	}
	//比較
	ret = t1 - ntim;
	//新しい時刻
	if (ret > 0) {
		//更新時刻を正時に切り上げ
		tm24.tm_hour++;
		tm24.tm_min = 0;
		ntim = 0;
		dll_mktime24(&tm24, &ntim);
		//指定時刻を正時に切り捨て
		dll_localtime24(t1, &tm24);
		tm24.tm_min = 0;
		tim = 0;
		dll_mktime24(&tm24, &tim);

		//正時を跨いでいる
		if (tim >= ntim)
			ret = 2;
		else
			ret = 1;
	//同じ時刻
	} else if (ret == 0) {
		dll_localtime24(t1, &tm24);
		if (tm24.tm_min == 0)
			ret = 2;
		else
			ret = 1;
	//古い時刻
	} else {
		ret = -1;
	}
	return ret;
}

/*---------------------------------------------------------
*	機能：時刻変換(更新時刻型→time_t型)
*	引数：p                 [IN]  変換元の時刻
        ：tm                [OUT] 変換後の時刻
*	戻値：
*	説明：
---------------------------------------------------------*/
void obsToTime(mem_obstime_t *p, time_t *tm)
{
	tm24_t tm24;

	*tm = 0;
	tm24.tm_year = p->yy;
	tm24.tm_mon  = p->mm;
	tm24.tm_mday = p->dd;
	tm24.tm_hour = p->hh;
	tm24.tm_min  = p->nn;
	tm24.tm_sec  = 0;
	dll_mktime24(&tm24, tm);
}

/*---------------------------------------------------------
*	機能：時刻変換(time_t型→更新時刻型)
*	引数：tm                [IN]  変換元の時刻
        ：p                 [OUT] 変換後の時刻
*	戻値：
*	説明：
---------------------------------------------------------*/
void timeToObs(time_t tm, mem_obstime_t *p)
{
	tm24_t tm24;

	dll_localtime24(tm, &tm24);
	p->yy = tm24.tm_year;
	p->mm = tm24.tm_mon;
	p->dd = tm24.tm_mday;
	p->hh = tm24.tm_hour;
	p->nn = tm24.tm_min;
}

/*---------------------------------------------------------
*	機能：共有データメモリ選択
*	引数：mp                [I/O] データメモリ管理情報
        ：typ               [IN]  データメモリタイプ
        ：kno               [IN]  局種別番号
        ：tim               [IN]  データ時刻
*	戻値：共有データメモリ情報
*	説明：
---------------------------------------------------------*/
mem_datamem_t *selectDataMemory(mem_manager_t *mp, int typ, int kno, time_t tim)
{
	mem_datamem_t *pp;
	tm24_t tm24;
	div_t  dv;

	//時刻が異常ならエラー
	if (tim <= 0)
		return NULL;

	//分を取得
	dll_localtime24_0(tim, &tm24);
	dv = div(tm24.tm_min, 10);

	//タイプによりメモリを選択
	switch (typ) {
	case MEM_TYPE_INPUT_1M:
		pp = &mp->tmem[kno-1].min[dv.rem];
		break;
	case MEM_TYPE_INPUT_HM:
		pp = &mp->tmem[kno-1].tim[dv.quot];
		break;
	case MEM_TYPE_DATA_1M:
		pp = &mp->dmem[kno-1].min[dv.rem];
		break;;
	case MEM_TYPE_DATA_HM:
		pp = &mp->dmem[kno-1].tim[dv.quot];
		break;
	case MEM_TYPE_DAILY:
		pp = &mp->dmem[kno-1].day;
		break;
	default:
		pp = NULL;
		break;
	}
	return pp;
}

/*---------------------------------------------------------
*	機能：共有データメモリオープン
*	引数：mp                [OUT] 共有データメモリ情報
*	戻値：
*	説明：
  備考  ：2003.06.10  Ver 1.3 に対応のため変更
---------------------------------------------------------*/
int openDataMemory(mem_manager_t *mp, kind_manager_t *kp)
{
	mem_manager_t mMem;
	char mname[32];
	int  ret;
	int  i,j;
	char *p;
	int  r_hm, r_day, r_new, r_now;
	int  d_hm, d_day, d_new, d_now;


	//パラメータの確認
	if (mp == NULL)
		return ERR_MEM_MP;
	if (kp == NULL)
		return ERR_MEM_KP;
	if (kp->pKind == NULL)
		return ERR_MEM_KP;


	//戻り値の初期化
	memset(mp, 0, sizeof(mem_manager_t));

	//編集用領域の初期化
	memset(&mMem, 0, sizeof(mem_manager_t));
	//局種別情報の設定
	mMem.kp = kp;

	//入力データメモリ
	for (i = 0; i < mMem.kp->max; i++) {
		if (mMem.kp->pKind[i].kind == NULL) continue;

		//共有メモリのオープン
		sprintf(mname, "INPMEM_%d", i+1);
		ret = dll_openSharedMemory(&mMem.tmem[i].mp, mname);

		//ポインタとサイズの設定
		p = mMem.tmem[i].mp.mem;
		r_hm  = sizeof(mem_obstime_t) + sizeof(char) * mMem.kp->pKind[i].kind->max;
		d_hm  = sizeof(bf_data_t) * mMem.kp->pKind[i].kind->max * mMem.kp->pKind[i].kind->titem;
		r_day = 0;
		d_day = 0;
		r_new = (sizeof(mem_obstime_t) + sizeof(char)) * mMem.kp->pKind[i].kind->max;
		d_new = sizeof(bf_data_t) * mMem.kp->pKind[i].kind->max * mMem.kp->pKind[i].kind->titem;
		r_now = 0;
		d_now = 0;

		//１分データ
		for (j = 0; j < 10; j++) {
			mMem.tmem[i].min[j].typ  = MEM_TYPE_INPUT_1M;
			mMem.tmem[i].min[j].kno  = i + 1;
			mMem.tmem[i].min[j].max  = mMem.kp->pKind[i].kind->max;
			mMem.tmem[i].min[j].item = mMem.kp->pKind[i].kind->titem;
			if (mMem.tmem[i].mp.mem != NULL && d_hm > 0) {
				mMem.tmem[i].min[j].r.mem = p;
				mMem.tmem[i].min[j].d.mem = p + r_hm;
				p += r_hm + d_hm;
			}
		}
		//正定時データ
		for (j = 0; j < 6; j++) {
			mMem.tmem[i].tim[j].typ  = MEM_TYPE_INPUT_HM;
			mMem.tmem[i].tim[j].kno  = i + 1;
			mMem.tmem[i].tim[j].max  = mMem.kp->pKind[i].kind->max;
			mMem.tmem[i].tim[j].item = mMem.kp->pKind[i].kind->titem;
			if (mMem.tmem[i].mp.mem != NULL && d_hm > 0) {
				mMem.tmem[i].tim[j].r.mem = p;
				mMem.tmem[i].tim[j].d.mem = p + r_hm;
				p += r_hm + d_hm;
			}
		}
		//日集計データは未使用

		//最新データ
		mMem.tmem[i].now.typ  = MEM_TYPE_NOW;
		mMem.tmem[i].now.kno  = i + 1;
		mMem.tmem[i].now.max  = mMem.kp->pKind[i].kind->max;
		mMem.tmem[i].now.item = mMem.kp->pKind[i].kind->titem;
		if (mMem.tmem[i].mp.mem != NULL && d_new > 0 ) {
			mMem.tmem[i].now.r.mem = p;
			mMem.tmem[i].now.d.mem = p + r_new;
			p += r_new + d_new;
		}

		//１分現況データは未使用
		//正定時現況データは未使用

	}

	//演算データメモリ
	for (i = 0; i < mMem.kp->max; i++) {
		if (mMem.kp->pKind[i].kind == NULL) continue;

		//共有メモリのオープン
		sprintf(mname, "OPEMEM_%d", i+1);
		ret = dll_openSharedMemory(&mMem.dmem[i].mp, mname);

		//ポインタとサイズの設定
		p = mMem.dmem[i].mp.mem;
		r_hm  = sizeof(mem_obstime_t) + sizeof(char) * mMem.kp->pKind[i].kind->max;
		d_hm  = sizeof(bf_data_t) * mMem.kp->pKind[i].kind->max * mMem.kp->pKind[i].kind->titem;
		r_day = sizeof(mem_obstime_t) + sizeof(char) * mMem.kp->pKind[i].kind->max;
		d_day = sizeof(bf_data_t) * mMem.kp->pKind[i].kind->max * mMem.kp->pKind[i].kind->ditem;
		r_new = (sizeof(mem_obstime_t) + sizeof(char)) * mMem.kp->pKind[i].kind->max;
		d_new = sizeof(bf_data_t) * mMem.kp->pKind[i].kind->max * mMem.kp->pKind[i].kind->titem;
//2005.03.04 修正（ここから）----------------------------------------
//更新情報メモリのサイズ計算が間違っている
//		r_now = sizeof(mem_obstime_t) + sizeof(char) * mMem.kp->pKind[i].kind->max;
		r_now = sizeof(mem_obstime_t) * mMem.kp->pKind[i].kind->max;
//2005.03.04 修正（ここまで）----------------------------------------
		d_now = sizeof(bf_data_t) * mMem.kp->pKind[i].kind->max * mMem.kp->pKind[i].kind->titem;

		//１分データ
		for (j = 0; j < 10; j++) {
			mMem.dmem[i].min[j].typ  = MEM_TYPE_DATA_1M;
			mMem.dmem[i].min[j].kno  = i + 1;
			mMem.dmem[i].min[j].max  = mMem.kp->pKind[i].kind->max;
			mMem.dmem[i].min[j].item = mMem.kp->pKind[i].kind->titem;
			if (mMem.dmem[i].mp.mem != NULL && d_hm > 0) {
				mMem.dmem[i].min[j].r.mem = p;
				mMem.dmem[i].min[j].d.mem = p + r_hm;
				p += r_hm + d_hm;
			}
		}
		//正定時データ
		for (j = 0; j < 6; j++) {
			mMem.dmem[i].tim[j].typ  = MEM_TYPE_DATA_HM;
			mMem.dmem[i].tim[j].kno  = i + 1;
			mMem.dmem[i].tim[j].max  = mMem.kp->pKind[i].kind->max;
			mMem.dmem[i].tim[j].item = mMem.kp->pKind[i].kind->titem;
			if (mMem.dmem[i].mp.mem != NULL && d_hm) {
				mMem.dmem[i].tim[j].r.mem = p;
				mMem.dmem[i].tim[j].d.mem = p + r_hm;
				p += r_hm + d_hm;
			}
		}

		//日集計データ
		mMem.dmem[i].day.typ  = MEM_TYPE_DAILY;
		mMem.dmem[i].day.kno  = i + 1;
		mMem.dmem[i].day.max  = mMem.kp->pKind[i].kind->max;
		mMem.dmem[i].day.item = mMem.kp->pKind[i].kind->ditem;
		if (mMem.dmem[i].mp.mem != NULL && d_day) {
			mMem.dmem[i].day.r.mem = p;
			mMem.dmem[i].day.d.mem = p + r_day;
			p += r_day + d_day;
		}

		//最新データ
		mMem.dmem[i].now.typ  = MEM_TYPE_NOW;
		mMem.dmem[i].now.kno  = i + 1;
		mMem.dmem[i].now.max  = mMem.kp->pKind[i].kind->max;
		mMem.dmem[i].now.item = mMem.kp->pKind[i].kind->titem;
		if (mMem.dmem[i].mp.mem != NULL && d_new) {
			mMem.dmem[i].now.r.mem = p;
			mMem.dmem[i].now.d.mem = p + r_new;
			p += r_new + d_new;
		}

		//１分現況データ
		mMem.dmem[i].now_1m.typ  = MEM_TYPE_NOW_1M;
		mMem.dmem[i].now_1m.kno  = i + 1;
		mMem.dmem[i].now_1m.max  = mMem.kp->pKind[i].kind->max;
		mMem.dmem[i].now_1m.item = mMem.kp->pKind[i].kind->titem;
		if (mMem.dmem[i].mp.mem != NULL && d_now) {
			mMem.dmem[i].now_1m.r.mem = p;
			mMem.dmem[i].now_1m.d.mem = p + r_now;
			p += r_now + d_now;
		}

		//正定時現況データ
		mMem.dmem[i].now_hm.typ  = MEM_TYPE_NOW_HM;
		mMem.dmem[i].now_hm.kno  = i + 1;
		mMem.dmem[i].now_hm.max  = mMem.kp->pKind[i].kind->max;
		mMem.dmem[i].now_hm.item = mMem.kp->pKind[i].kind->titem;
		if (mMem.dmem[i].mp.mem != NULL && d_now > 0) {
			mMem.dmem[i].now_hm.r.mem = p;
			mMem.dmem[i].now_hm.d.mem = p + r_now;
			p += r_now + d_now;
		}

	}

	//ロック用ハンドルの取得
	dll_createLock(NULL, LOCK_NAME_INPUT, &mMem.hLockInput);
	dll_createLock(NULL, LOCK_NAME_DATA,  &mMem.hLockData);
	dll_createLock(NULL, LOCK_NAME_DAILY, &mMem.hLockDaily);

	//戻り値の設定
	memcpy(mp, &mMem, sizeof(mMem));

	return ERR_MEM_SUCCESS;
}

/*---------------------------------------------------------
*	機能：共有データメモリクローズ
*	引数：mp                [IN]  共有データメモリ情報
*	戻値：
*	説明：
  備考  ：2003.06.10  Ver 1.3 に対応のため変更
---------------------------------------------------------*/
void closeDataMemory(mem_manager_t *mp)
{
	int i;

	//パラメータの確認
	if (mp == NULL)
		return;
	if (mp->kp == NULL)
		return;
	if (mp->kp->pKind == NULL)
		return;

	//入力データメモリ
	for (i = 0; i < mp->kp->max; i++) {
		if (mp->kp->pKind[i].kind == NULL) continue;
		//メモリの解放
		dll_freeSharedMemory(&mp->tmem[i].mp);
	}

	//演算データメモリ
	for (i = 0; i < mp->kp->max; i++) {
		if (mp->kp->pKind[i].kind == NULL) continue;
		//メモリの解放
		dll_freeSharedMemory(&mp->dmem[i].mp);
	}

	//ロック用ハンドルの開放
	dll_closeLock(mp->hLockInput);
	dll_closeLock(mp->hLockData);
	dll_closeLock(mp->hLockDaily);

}

/*---------------------------------------------------------
*	関数：initDataMemoryHM
*	機能：共有データメモリ初期化(１分・正定時用)
*	引数：kno               [IN]  局種別番号
        ：p                 [I/O] データメモリポインタ
*	戻値：
*	説明：データメモリを初期化します。
        ：局種別番号により局稼動情報・局実装情報を参照し
		：データのフラグを初期化します。
---------------------------------------------------------*/
void initDataMemoryHM(mem_manager_t *mp, int kno, char *p)
{
	bf_data_t *dp = (bf_data_t *)p;
	int max_item;
	int i,j;
	unsigned char f1;

	//パラメータの確認
	if (mp == NULL)
		return;
	if (mp->kp == NULL)
		return;
	if (mp->kp->pKind == NULL)
		return;
	if (kno < 1 || kno > mp->kp->max)
		return;
	if (mp->kp->pKind[kno-1].kind == NULL)
		return;
	if (p == NULL)
		return;

	//最大項目数の決定
	max_item = mp->kp->pKind[kno-1].kind->titem;

	//メモリのクリア
	memset(dp,  0, sizeof(bf_data_t) * mp->kp->pKind[kno-1].kind->max * max_item);

	//データ部のフラグ初期化
	for (i = 0; i < mp->kp->pKind[kno-1].kind->max; i++) {
		//実装
		if (mp->kp->pKind[kno-1].local[i].entry == SYS_LOCAL_ENTRY_ON) {
			//保守中
			if (mp->kp->pKind[kno-1].action[i].action == SYS_ACTION_ACTION_MAINTE) {
				f1 = BF_MAINTE;
			//閉局
			} else if (mp->kp->pKind[kno-1].action[i].action == SYS_ACTION_ACTION_CLOSE) {
				f1 = BF_CLOSE;
			//ラグタイムアウト(未入力)
			} else {
				f1 = BF_TOUT;
			}
		//非実装
		} else {
			f1= BF_DISABLE;
		}
		//フラグの初期化
		for (j = 0; j < max_item; j++) {
			dp[i*max_item+j].f[0] = f1;
		}
	}
}

/*---------------------------------------------------------
*	関数：initDataMemoryDay
*	機能：共有データメモリ初期化(日集計用)
*	引数：kno               [IN]  局種別番号
        ：p                 [I/O] データメモリポインタ
*	戻値：
*	説明：データメモリを初期化します。
        ：局種別番号により局稼動情報・局実装情報を参照し
		：データのフラグを初期化します。
---------------------------------------------------------*/
void initDataMemoryDay(mem_manager_t *mp, int kno, char *p)
{
	bf_data_t *dp = (bf_data_t *)p;
	int max_item;
	int i,j;
	unsigned char f1;

	//パラメータの確認
	if (mp == NULL)
		return;
	if (mp->kp == NULL)
		return;
	if (mp->kp->pKind == NULL)
		return;
	if (kno < 1 || kno > mp->kp->max)
		return;
	if (mp->kp->pKind[kno-1].kind == NULL)
		return;
	if (p == NULL)
		return;

	//最大項目数の決定
	max_item = mp->kp->pKind[kno-1].kind->ditem;

	//メモリのクリア
	memset(dp,  0, sizeof(bf_data_t) * mp->kp->pKind[kno-1].kind->max * max_item);

	//データ部のフラグ初期化
	for (i = 0; i < mp->kp->pKind[kno-1].kind->max; i++) {
		//実装
		if (mp->kp->pKind[kno-1].local[i].entry == SYS_LOCAL_ENTRY_ON) {
			//保守中
			if (mp->kp->pKind[kno-1].action[i].action == SYS_ACTION_ACTION_MAINTE) {
				f1 = BF_MAINTE;
			//閉局
			} else if (mp->kp->pKind[kno-1].action[i].action == SYS_ACTION_ACTION_CLOSE) {
				f1 = BF_CLOSE;
			//ラグタイムアウト(未入力)
			} else {
				f1 = BF_TOUT;
			}
		//非実装
		} else {
			f1= BF_DISABLE;
		}
		//フラグの初期化
		for (j = 0; j < max_item; j++) {
			dp[i*max_item+j].f[0] = f1;
		}
	}
}

/*---------------------------------------------------------
*	関数：initDataMemory
*	機能：共有データメモリ初期化(１分・正定時用)
*	引数：kno               [IN]  局種別番号
        ：tim               [IN]  データ時刻
        ：p                 [I/O] データメモリポインタ
*	戻値：
*	説明：更新情報メモリとデータメモリを初期化します。
---------------------------------------------------------*/
void initDataMemory(mem_manager_t *mp, mem_datamem_t *dp, time_t tim)
{
	//更新情報メモリのクリア
	memset(dp->r.mem, 0, sizeof(mem_obstime_t) + sizeof(char) * dp->max);
	//時刻の設定
	timeToObs(tim, (mem_obstime_t *)dp->r.mem);

	//データメモリの初期化
	if (dp->typ == MEM_TYPE_DAILY)
		initDataMemoryDay(mp, dp->kno, dp->d.mem);
	else
		initDataMemoryHM(mp, dp->kno, dp->d.mem);
}

/*---------------------------------------------------------
*	関数：writeLatestMemory
*	機能：最新データメモリ書き込み
*	引数：mp                [I/O] データメモリ管理情報
        ：typ               [IN]  データメモリタイプ
        ：kno               [IN]  局種別番号
        ：tim               [IN]  データ時刻
        ：sno               [IN]  局番号
        ：dp                [IN]  書き込みデータ
*	戻値：
*	説明：
---------------------------------------------------------*/
int writeLatestMemory(mem_manager_t *mp, int typ, int kno, time_t tim, int sno, char *dp)
{
	mem_datamem_t *pp;
	mem_obstime_t *tp;
	char      *rp;
	bf_data_t *bp;
	tm24_t    tm24;

	//パラメータの確認
	if (mp == NULL)
		return ERR_MEM_MP;
	if (mp->kp == NULL)
		return ERR_MEM_MP;
	if (mp->kp->pKind == NULL)
		return ERR_MEM_MP;
	//データメモリタイプ
	if (typ < MEM_TYPE_INPUT_1M || typ > MEM_TYPE_DAILY)
		return ERR_MEM_TYPE;
	//局種別番号
	if (kno < 1 || kno > mp->kp->max)
		return ERR_MEM_KNO;
	if (mp->kp->pKind[kno-1].kind == NULL)
		return ERR_MEM_KNO;
	//データ時刻
	if (tim == 0)
		return ERR_MEM_TIME;
	//書き込みデータ
	if (dp == NULL)
		return ERR_MEM_DATA;

	//入力データメモリ
	if (typ == MEM_TYPE_INPUT_1M || typ == MEM_TYPE_INPUT_HM)
		pp = (mem_datamem_t *)&mp->tmem[kno-1].now;
	//演算データメモリ
	else if (typ == MEM_TYPE_DATA_1M || typ == MEM_TYPE_DATA_HM)
		pp = (mem_datamem_t *)&mp->dmem[kno-1].now;
	else
		return 0;

	tp = (mem_obstime_t *)pp->r.mem;
	rp = pp->r.mem + sizeof(mem_obstime_t) * pp->max;
	bp = (bf_data_t *)pp->d.mem;

	//メモリは作成されていない
	if (pp->r.mem == NULL)
		return ERR_MEM_MEMORY;
	if (pp->d.mem == NULL)
		return ERR_MEM_MEMORY;
	//局番号
	if (sno < 1 || sno > pp->max)
		return ERR_MEM_SNO;

	//秒を０にする
	dll_localtime24_0(tim, &tm24);

	//メモリに書き込み
	memcpy(&bp[(sno-1) * pp->item], dp, sizeof(bf_data_t) * pp->item);
	tp[sno-1].yy = tm24.tm_year;
	tp[sno-1].mm = tm24.tm_mon;
	tp[sno-1].dd = tm24.tm_mday;
	tp[sno-1].hh = tm24.tm_hour;
	tp[sno-1].nn = tm24.tm_min;
	if (rp[sno-1] >= 255)
		rp[sno-1] = 1;
	else
		rp[sno-1]++;

	return 0;
}

/*---------------------------------------------------------
*	関数：writeDataMemoryNow
*	機能：現況データメモリ書き込み
*	引数：mp                [I/O] データメモリ管理情報
        ：typ               [IN]  データメモリタイプ
        ：kno               [IN]  局種別番号
        ：tim               [IN]  データ時刻
        ：dp                [IN]  書き込みデータ
*	戻値：
*	説明：
---------------------------------------------------------*/
int writeDataMemoryNow(mem_manager_t *mp, int typ, int kno, time_t tim, mem_datamem_t *dp)
{
	mem_datamem_t *pp;
	mem_obstime_t *s_ip;
	bf_data_t     *s_dp;
	char          *d_ip;
	bf_data_t     *d_dp;
	tm24_t tm24;
	time_t wtim;
	int i, j;
	int flg;
	int ret;


	//パラメータの確認
	if (mp == NULL)
		return ERR_MEM_MP;
	if (mp->kp == NULL)
		return ERR_MEM_MP;
	if (mp->kp->pKind == NULL)
		return ERR_MEM_MP;
	//データメモリタイプ
	if (typ < MEM_TYPE_INPUT_1M || typ > MEM_TYPE_DAILY)
		return ERR_MEM_TYPE;
	//局種別番号
	if (kno < 1 || kno > mp->kp->max)
		return ERR_MEM_KNO;
	if (mp->kp->pKind[kno-1].kind == NULL)
		return ERR_MEM_KNO;
	//データ時刻
	if (tim == 0)
		return ERR_MEM_TIME;
	//書き込みデータ
	if (dp == NULL)
		return ERR_MEM_DATA;

	//現況データメモリ
	switch (typ) {
	case MEM_TYPE_INPUT_1M:
		return 0;
		break;
	case MEM_TYPE_INPUT_HM:
		return 0;
		break;
	case MEM_TYPE_DATA_1M:
		pp = (mem_datamem_t *)&mp->dmem[kno-1].now_1m;
		break;
	case MEM_TYPE_DATA_HM:
		pp = (mem_datamem_t *)&mp->dmem[kno-1].now_hm;
		break;
	default:
		return 0;
		break;
	}
	s_ip = (mem_obstime_t *)pp->r.mem;
	s_dp = (bf_data_t *)pp->d.mem;
	d_ip = (char *)dp->r.mem + sizeof(mem_obstime_t);
	d_dp = (bf_data_t *)dp->d.mem;

	//メモリは作成されていない
	if (pp->r.mem == NULL)
		return ERR_MEM_MEMORY;
	if (pp->d.mem == NULL)
		return ERR_MEM_MEMORY;


	//秒を０にする
	wtim = 0;
	dll_localtime24_0(tim, &tm24);
	dll_mktime24(&tm24, &wtim);

	//非実装局の判定
	for (i = 0; i < pp->max; i++) {
		if (mp->kp->pKind[kno-1].local[i].entry != SYS_LOCAL_ENTRY_ON) {
			for (j = 0; j < pp->item; j++) {
				s_dp[i * pp->item + j].dt = 0;
				s_dp[i * pp->item + j].f[0] = BF_DISABLE;
				s_dp[i * pp->item + j].f[1] = 0;
				s_dp[i * pp->item + j].f[2] = 0;
				s_dp[i * pp->item + j].f[3] = 0;
			}
			memset(&s_ip[i], 0, sizeof(mem_obstime_t));
		}
	}
	for (i = 0; i < pp->max; i++) {
		//時刻の判定
		ret = timeCmp(wtim, s_ip[i]);
		//正時もしくは正時を跨いでいる
		if (ret == 2) {
			//更新
			if (d_ip[i] == 0) continue;
			//実装
			if (mp->kp->pKind[kno-1].local[i].entry == SYS_LOCAL_ENTRY_ON) {
				memcpy(&s_dp[i * pp->item], &d_dp[i * pp->item], sizeof(bf_data_t) * pp->item);
				s_ip[i].yy = tm24.tm_year;
				s_ip[i].mm = tm24.tm_mon;
				s_ip[i].dd = tm24.tm_mday;
				s_ip[i].hh = tm24.tm_hour;
				s_ip[i].nn = tm24.tm_min;
			}
		//更新情報ファイルの時刻より新しい
		} else if (ret >= 0) {
			//更新
			if (d_ip[i] == 0) continue;
			//実装
			if (mp->kp->pKind[kno-1].local[i].entry == SYS_LOCAL_ENTRY_ON) {
				//フラグ１の確認
				flg = FALSE;
				for (j = 0; j < pp->item; j++) {
					if (d_dp[i * pp->item + j].f[0] < 128) {
						flg = TRUE;
						break;
					}
				}
				//正常なデータがあれば書き込み
				if (flg == TRUE) {
//2003.05.29（修正）ここから-----------------------------------------
//正常データでも定時間でフラグが異常な項目は書き込まない事になった
//					memcpy(&s_dp[i * pp->item], &d_dp[i * pp->item], sizeof(bf_data_t) * pp->item);
					for (j = 0; j < pp->item; j++) {
						if (d_dp[i * pp->item + j].f[0] < 128) {
							memcpy(&s_dp[i * pp->item + j], &d_dp[i * pp->item + j], sizeof(bf_data_t));
						}
					}
//2003.05.29（修正）ここまで-----------------------------------------
					s_ip[i].yy = tm24.tm_year;
					s_ip[i].mm = tm24.tm_mon;
					s_ip[i].dd = tm24.tm_mday;
					s_ip[i].hh = tm24.tm_hour;
					s_ip[i].nn = tm24.tm_min;
				}
			}
		}
	}
	return 0;
}

/*---------------------------------------------------------
*	関数：writeDataMemory
*	機能：共有データメモリ書き込み(全局全項目)
*	引数：mp                [I/O] データメモリ情報
        ：typ               [IN]  データメモリタイプ
        ：kno               [IN]  局種別番号
        ：tim               [IN]  データ時刻
        ：dp                [IN]  書き込みデータ
*	戻値：
*	説明：
---------------------------------------------------------*/
int writeDataMemory(mem_manager_t *mp, int typ, int kno, time_t tim, char *dp)
{
	mem_datamem_t *pp;
	mem_obstime_t *tp;
	char      *rp;
	bf_data_t *bp;
	tm24_t    tm24;
	time_t    rtim;
	time_t    wtim;
	int       i;

	//パラメータの確認
	if (mp == NULL)
		return ERR_MEM_MP;
	if (mp->kp == NULL)
		return ERR_MEM_MP;
	if (mp->kp->pKind == NULL)
		return ERR_MEM_MP;
	//データメモリタイプ
	if (typ < MEM_TYPE_INPUT_1M || typ > MEM_TYPE_DAILY)
		return ERR_MEM_TYPE;
	//局種別番号
	if (kno < 1 || kno > mp->kp->max)
		return ERR_MEM_KNO;
	if (mp->kp->pKind[kno-1].kind == NULL)
		return ERR_MEM_KNO;
	//データ時刻
	if (tim == 0)
		return ERR_MEM_TIME;
	//書き込みデータ
	if (dp == NULL)
		return ERR_MEM_DATA;

	//各種ポインタの設定
	pp = selectDataMemory(mp, typ, kno, tim);
	tp = (mem_obstime_t *)pp->r.mem;
	rp = pp->r.mem + sizeof(mem_obstime_t);
	bp = (bf_data_t *)pp->d.mem;

	//メモリは作成されていない
	if (pp->r.mem == NULL)
		return ERR_MEM_MEMORY;
	if (pp->d.mem == NULL)
		return ERR_MEM_MEMORY;

	//秒を０にする
	wtim = 0;
	dll_localtime24_0(tim, &tm24);
	dll_mktime24(&tm24, &wtim);

	//更新時刻が変化したならメモリの初期化
	obsToTime(tp, &rtim);
	if (rtim != wtim)
		initDataMemory(mp, pp, wtim);

	//メモリに書き込み
	memcpy(bp, dp, sizeof(bf_data_t) * pp->max * pp->item);
	for (i = 0; i < pp->max; i++) {
		rp[i] = 1;
	}

	//最新データに書き込み
	for (i = 0; i < pp->max; i++) {
		writeLatestMemory(mp, typ, kno, wtim, i+1, dp + sizeof(bf_data_t) * i);
	}
	//現況データに書き込み
	writeDataMemoryNow(mp, typ, kno, wtim, pp);

	return 0;
}

/*---------------------------------------------------------
*	関数：writeDataMemoryS
*	機能：共有データメモリ書き込み(１局全項目)
*	引数：mp                [I/O] データメモリ情報
        ：typ               [IN]  データメモリタイプ
        ：kno               [IN]  局種別番号
        ：tim               [IN]  データ時刻
        ：sno               [IN]  局番号
        ：dp                [IN]  書き込みデータ
*	戻値：
*	説明：
---------------------------------------------------------*/
int writeDataMemoryS(mem_manager_t *mp, int typ, int kno, time_t tim, int sno, char *dp)
{
	mem_datamem_t *pp;
	mem_obstime_t *tp;
	char      *rp;
	bf_data_t *bp;
	tm24_t    tm24;
	time_t    rtim;
	time_t    wtim;

	//パラメータの確認
	if (mp == NULL)
		return ERR_MEM_MP;
	if (mp->kp == NULL)
		return ERR_MEM_MP;
	if (mp->kp->pKind == NULL)
		return ERR_MEM_MP;
	//データメモリタイプ
	if (typ < MEM_TYPE_INPUT_1M || typ > MEM_TYPE_DAILY)
		return ERR_MEM_TYPE;
	//局種別番号
	if (kno < 1 || kno > mp->kp->max)
		return ERR_MEM_KNO;
	if (mp->kp->pKind[kno-1].kind == NULL)
		return ERR_MEM_KNO;
	//データ時刻
	if (tim == 0)
		return ERR_MEM_TIME;
	//書き込みデータ
	if (dp == NULL)
		return ERR_MEM_DATA;

	//各種ポインタの設定
	pp = selectDataMemory(mp, typ, kno, tim);
	tp = (mem_obstime_t *)pp->r.mem;
	rp = pp->r.mem + sizeof(mem_obstime_t);
	bp = (bf_data_t *)pp->d.mem;

	//メモリは作成されていない
	if (pp->r.mem == NULL)
		return ERR_MEM_MEMORY;
	if (pp->d.mem == NULL)
		return ERR_MEM_MEMORY;
	//局番号
	if (sno < 1 || sno > pp->max)
		return ERR_MEM_SNO;

	//秒を０にする
	wtim = 0;
	dll_localtime24_0(tim, &tm24);
	dll_mktime24(&tm24, &wtim);

	//更新時刻が変化したならメモリの初期化
	obsToTime(tp, &rtim);
	if (rtim != wtim)
		initDataMemory(mp, pp, wtim);

	//メモリに書き込み
	memcpy(&bp[(sno-1) * pp->item], dp, sizeof(bf_data_t) * pp->item);
	rp[sno-1] = 1;

	//最新データに書き込み
	writeLatestMemory(mp, typ, kno, wtim, sno, dp);
	//現況データに書き込み
	writeDataMemoryNow(mp, typ, kno, wtim, pp);

	return 0;
}

/*---------------------------------------------------------
*	関数：writeDataMemoryI
*	機能：共有データメモリ書き込み(ｎ局ｎ項目)
*	引数：mp                [I/O] データメモリ管理情報
        ：typ               [IN]  データメモリタイプ
        ：tim               [IN]  データ時刻
        ：cnt               [IN]  書き込み項目件数
        ：dp                [IN]  書き込みデータ
*	戻値：
*	説明：
---------------------------------------------------------*/
int writeDataMemoryI(mem_manager_t *mp, int typ, time_t tim, int cnt, mem_reqdata_t *dp)
{
	mem_datamem_t *pp;
	mem_obstime_t *tp;
	char          *rp;
	bf_data_t     *bp;
	bf_data_t     *wp;
	tm24_t tm24;
	time_t rtim;
	time_t wtim;
	char kidx[99] = {0};
	int i, j, k;
	int ret;
	BOOL flg;


	//パラメータの確認
	if (mp == NULL)
		return ERR_MEM_MP;
	if (mp->kp == NULL)
		return ERR_MEM_MP;
	if (mp->kp->pKind == NULL)
		return ERR_MEM_MP;
	//データメモリタイプ
	if (typ < MEM_TYPE_INPUT_1M || typ > MEM_TYPE_DAILY)
		return ERR_MEM_TYPE;
	//データ時刻
	if (tim == 0)
		return ERR_MEM_TIME;
	//書き込みデータ
	if (dp == NULL)
		return ERR_MEM_DATA;

	//秒を０にする
	wtim = 0;
	dll_localtime24_0(tim, &tm24);
	dll_mktime24(&tm24, &wtim);

	//局種別・局番号・項目番号・メモリ
	for (k = 0; k < cnt; k++) {
		//局種別
		if (dp[k].kno < 1 || dp[k].kno > mp->kp->max)
			return ERR_MEM_KNO;
		if (mp->kp->pKind[dp[k].kno-1].kind == NULL)
			return ERR_MEM_KNO;
		//メモリ選択
		pp = selectDataMemory(mp, typ, dp[k].kno, wtim);
		//メモリは作成されていない
		if (pp->r.mem == NULL)
			return ERR_MEM_MEMORY;
		if (pp->d.mem == NULL)
			return ERR_MEM_MEMORY;
		//局番号
		if (dp[k].sno < 1 || dp[k].sno > pp->max)
			return ERR_MEM_SNO;
		//項目番号
		if (dp[k].ino < 1 || dp[k].ino > pp->item)
			return ERR_MEM_ITEM;

		//局種別に書き込み要求あり
		kidx[dp[k].kno-1] = 1;
	}

	//全局種別
	for (i = 0; i < mp->kp->max; i++) {
		if (kidx[i] == 0) continue;

		//各種ポインタの設定
		pp = selectDataMemory(mp, typ, i+1, wtim);
		tp = (mem_obstime_t *)pp->r.mem;
		rp = pp->r.mem + sizeof(mem_obstime_t);
		bp = (bf_data_t *)pp->d.mem;

		//更新時刻が変化したならメモリの初期化
		obsToTime(tp, &rtim);
		if (rtim != wtim)
			initDataMemory(mp, pp, wtim);

		//編集領域の確保
		wp = new bf_data_t[pp->item];
		if (wp == NULL)
			return ERR_MEM_ALLOC;

		//最大局数まで
		for (j = 0; j < pp->max; j++) {
			flg = FALSE;
			memcpy(wp, &bp[j * pp->item], sizeof(bf_data_t) * pp->item);

			for (k = 0; k < cnt; k++) {
				if (dp[k].kno == i+1 && dp[k].sno == j+1) {
					//編集領域に書き込み
					memcpy(&wp[dp[k].ino-1], &dp[k].dt, sizeof(bf_data_t));
					flg = TRUE;
				}
			}

			//更新があれば共有メモリに書き込み
			if ( flg ) {
				ret = writeDataMemoryS(mp, typ, i+1, wtim, j+1, (char *)wp);
				if (ret != 0) {
					if (wp != NULL) free(wp);
					return ret;
				}
			}
		}
		if (wp != NULL) free(wp);
	}
	return 0;
}

/*---------------------------------------------------------
*	関数：readDataMemoryI
*	機能：共有データメモリ読み込み(ｎ局ｎ項目)
*	引数：mp                [I/O] データメモリ管理情報
        ：typ               [IN]  データメモリタイプ
        ：tim               [IN]  データ時刻
        ：cnt               [IN]  読み込み項目件数
        ：dp                [IN]  読み込みデータ
*	戻値：
*	説明：
---------------------------------------------------------*/
int readDataMemoryI(mem_manager_t *mp, int typ, time_t tim, int cnt, mem_reqdata_t *dp)
{
	mem_datamem_t *pp;
	mem_obstime_t *tp;
	char          *rp;
	bf_data_t     *bp;
	tm24_t tm24;
	time_t mtim;
	time_t wtim;
	char kidx[99] = {0};
	int i, j, k;


	//パラメータの確認
	if (mp == NULL)
		return ERR_MEM_MP;
	if (mp->kp == NULL)
		return ERR_MEM_MP;
	if (mp->kp->pKind == NULL)
		return ERR_MEM_MP;
	//データメモリタイプ
	if (typ < MEM_TYPE_INPUT_1M || typ > MEM_TYPE_DAILY)
		return ERR_MEM_TYPE;
	//データ時刻
	if (tim == 0)
		return ERR_MEM_TIME;
	//書き込みデータ
	if (dp == NULL)
		return ERR_MEM_DATA;

	//秒を０にする
	wtim = 0;
	dll_localtime24_0(tim, &tm24);
	dll_mktime24(&tm24, &wtim);

	//局種別・局番号・項目番号・メモリ
	for (k = 0; k < cnt; k++) {
		//局種別
		if (dp[k].kno < 1 || dp[k].kno > mp->kp->max)
			return ERR_MEM_KNO;
		if (mp->kp->pKind[dp[k].kno-1].kind == NULL)
			return ERR_MEM_KNO;
		//メモリ選択
		pp = selectDataMemory(mp, typ, dp[k].kno, wtim);
		//メモリは作成されていない
		if (pp->r.mem == NULL)
			return ERR_MEM_MEMORY;
		if (pp->d.mem == NULL)
			return ERR_MEM_MEMORY;
		//局番号
		if (dp[k].sno < 1 || dp[k].sno > pp->max)
			return ERR_MEM_SNO;
		//項目番号
		if (dp[k].ino < 1 || dp[k].ino > pp->item)
			return ERR_MEM_ITEM;
		//時刻
		obsToTime((mem_obstime_t *)pp->r.mem, &mtim);
		if (wtim != mtim)
			return ERR_MEM_TIME;

		//局種別に読み込み要求あり
		kidx[dp[k].kno-1] = 1;
	}

	//全局種別
	for (i = 0; i < mp->kp->max; i++) {
		if (kidx[i] == 0) continue;

		//各種ポインタの設定
		pp = selectDataMemory(mp, typ, i+1, wtim);
		tp = (mem_obstime_t *)pp->r.mem;
		rp = pp->r.mem + sizeof(mem_obstime_t);
		bp = (bf_data_t *)pp->d.mem;

		//最大局数まで
		for (j = 0; j < pp->max; j++) {
			for (k = 0; k < cnt; k++) {
				if (dp[k].kno == i+1 && dp[k].sno == j+1) {
					//編集領域に読み込み
					memcpy(&dp[k].dt, &bp[(dp[k].sno-1)*pp->item+dp[k].ino-1], sizeof(bf_data_t));
				}
			}
		}
	}
	return 0;
}

/*---------------------------------------------------------
*	関数：checkDataMemory
*	機能：共有データメモリ選択
*	引数：mp                [I/O] データメモリ管理情報
        ：typ               [IN]  データメモリタイプ
        ：kno               [IN]  局種別番号
        ：tim               [IN]  データ時刻
*	戻値：共有データメモリ情報
*	説明：
---------------------------------------------------------*/
mem_datamem_t *checkDataMemory(mem_manager_t *mp, int typ, int kno, time_t tim)
{
	mem_datamem_t *pp;
	tm24_t tm24;
	time_t mtim;
	div_t  dv;

	//分を取得
	dll_localtime24_0(tim, &tm24);
	dv = div(tm24.tm_min, 10);

	//タイプによりメモリを選択
	switch (typ) {
	case MEM_TYPE_INPUT_1M:
		pp = &mp->tmem[kno-1].min[dv.rem];
		break;
	case MEM_TYPE_INPUT_HM:
		pp = &mp->tmem[kno-1].tim[dv.quot];
		break;
	case MEM_TYPE_DATA_1M:
		pp = &mp->dmem[kno-1].min[dv.rem];
		break;;
	case MEM_TYPE_DATA_HM:
		pp = &mp->dmem[kno-1].tim[dv.quot];
		break;
	case MEM_TYPE_DAILY:
		pp = &mp->dmem[kno-1].day;
		break;
	default:
		pp = NULL;
		break;
	}
	//時刻の比較
	obsToTime((mem_obstime_t *)pp->r.mem, &mtim);
	if (tim != mtim)
		pp = NULL;

	return pp;
}

/*---------------------------------------------------------
*	関数：getUpToDateTime
*	機能：最新時刻取得
*	引数：mp                [I/O] データメモリ管理情報
        ：typ               [IN]  データメモリタイプ
        ：kno               [IN]  局種別番号
*	戻値：最新時刻
*	説明：
---------------------------------------------------------*/
int getUpToDateTime(mem_manager_t *mp, int typ, int kno, time_t *tt)
{
	mem_obstime_t *tp;
	time_t tim, wtim;
	int i;

	//パラメータの確認
	if (mp == NULL)
		return ERR_MEM_MP;
	if (mp->kp == NULL)
		return ERR_MEM_MP;
	if (mp->kp->pKind == NULL)
		return ERR_MEM_MP;
	//データメモリタイプ
	if (typ < MEM_TYPE_INPUT_1M || typ > MEM_TYPE_DAILY)
		return ERR_MEM_TYPE;
	//局種別番号
	if (kno < 1 || kno > mp->kp->max)
		return ERR_MEM_KNO;
	if (mp->kp->pKind[kno-1].kind == NULL)
		return ERR_MEM_KNO;

	//変数の初期化
	tim = 0;

	//入力メモリ
	if (typ == MEM_TYPE_INPUT_1M || typ == MEM_TYPE_INPUT_HM) {
		//１分
		for (i = 0; i < 10; i++) {
			tp = (mem_obstime_t *)mp->dmem[kno-1].min[i].r.mem;
			if (tp == NULL) continue;
			obsToTime(tp, &wtim);
			if (tim < wtim) tim = wtim;
		}
		//正定時
		for (i = 0; i < 6; i++) {
			tp = (mem_obstime_t *)mp->dmem[kno-1].tim[i].r.mem;
			if (tp == NULL) continue;
			obsToTime(tp, &wtim);
			if (tim < wtim) tim = wtim;
		}
	//演算メモリ
	} else if (typ == MEM_TYPE_DATA_1M || MEM_TYPE_DATA_HM) {
		//１分
		for (i = 0; i < 10; i++) {
			tp = (mem_obstime_t *)mp->dmem[kno-1].min[i].r.mem;
			if (tp == NULL) continue;
			obsToTime(tp, &wtim);
			if (tim < wtim) tim = wtim;
		}
		//正定時
		for (i = 0; i < 6; i++) {
			tp = (mem_obstime_t *)mp->dmem[kno-1].tim[i].r.mem;
			if (tp == NULL) continue;
			obsToTime(tp, &wtim);
			if (tim < wtim) tim = wtim;
		}
	}

	*tt = tim;

	return ERR_MEM_SUCCESS;
}


//2003.06.23 追加（ここから）----------------------------------------
//既に更新されている局は上書きしないバージョン

/*---------------------------------------------------------
*	関数：writeDataMemoryS_nOW
*	機能：共有データメモリ書き込み(１局全項目)
*	引数：mp                [I/O] データメモリ情報
        ：typ               [IN]  データメモリタイプ
        ：kno               [IN]  局種別番号
        ：tim               [IN]  データ時刻
        ：sno               [IN]  局番号
        ：dp                [IN]  書き込みデータ
*	戻値：
*	説明：既に更新されている局は上書きしない
---------------------------------------------------------*/
int writeDataMemoryS_nOW(mem_manager_t *mp, int typ, int kno, time_t tim, int sno, char *dp)
{
	mem_datamem_t *pp;
	mem_obstime_t *tp;
	char      *rp;
	bf_data_t *bp;
	tm24_t    tm24;
	time_t    rtim;
	time_t    wtim;

	//パラメータの確認
	if (mp == NULL)
		return ERR_MEM_MP;
	if (mp->kp == NULL)
		return ERR_MEM_MP;
	if (mp->kp->pKind == NULL)
		return ERR_MEM_MP;
	//データメモリタイプ
	if (typ < MEM_TYPE_INPUT_1M || typ > MEM_TYPE_DAILY)
		return ERR_MEM_TYPE;
	//局種別番号
	if (kno < 1 || kno > mp->kp->max)
		return ERR_MEM_KNO;
	if (mp->kp->pKind[kno-1].kind == NULL)
		return ERR_MEM_KNO;
	//データ時刻
	if (tim == 0)
		return ERR_MEM_TIME;
	//書き込みデータ
	if (dp == NULL)
		return ERR_MEM_DATA;

	//各種ポインタの設定
	pp = selectDataMemory(mp, typ, kno, tim);
	tp = (mem_obstime_t *)pp->r.mem;
	rp = pp->r.mem + sizeof(mem_obstime_t);
	bp = (bf_data_t *)pp->d.mem;

	//メモリは作成されていない
	if (pp->r.mem == NULL)
		return ERR_MEM_MEMORY;
	if (pp->d.mem == NULL)
		return ERR_MEM_MEMORY;
	//局番号
	if (sno < 1 || sno > pp->max)
		return ERR_MEM_SNO;

	//秒を０にする
	wtim = 0;
	dll_localtime24_0(tim, &tm24);
	dll_mktime24(&tm24, &wtim);

	//更新時刻が変化したならメモリの初期化
	obsToTime(tp, &rtim);
	if (rtim != wtim)
		initDataMemory(mp, pp, wtim);

	if (rp[sno-1] == 0) {
		//メモリに書き込み
		memcpy(&bp[(sno-1) * pp->item], dp, sizeof(bf_data_t) * pp->item);
		rp[sno-1] = 1;

		//最新データに書き込み
		writeLatestMemory(mp, typ, kno, wtim, sno, dp);
		//現況データに書き込み
		writeDataMemoryNow(mp, typ, kno, wtim, pp);
	}

	return 0;
}

/*---------------------------------------------------------
*	関数：writeDataMemoryI_nOW
*	機能：共有データメモリ書き込み(ｎ局ｎ項目)
*	引数：mp                [I/O] データメモリ管理情報
        ：typ               [IN]  データメモリタイプ
        ：tim               [IN]  データ時刻
        ：cnt               [IN]  書き込み項目件数
        ：dp                [IN]  書き込みデータ
*	戻値：
*	説明：既に更新されている局は上書きしない
---------------------------------------------------------*/
int writeDataMemoryI_nOW(mem_manager_t *mp, int typ, time_t tim, int cnt, mem_reqdata_t *dp)
{
	mem_datamem_t *pp;
	mem_obstime_t *tp;
	char          *rp;
	bf_data_t     *bp;
	bf_data_t     *wp;
	tm24_t tm24;
	time_t rtim;
	time_t wtim;
	char kidx[99] = {0};
	int i, j, k;
	int ret;
	BOOL flg;


	//パラメータの確認
	if (mp == NULL)
		return ERR_MEM_MP;
	if (mp->kp == NULL)
		return ERR_MEM_MP;
	if (mp->kp->pKind == NULL)
		return ERR_MEM_MP;
	//データメモリタイプ
	if (typ < MEM_TYPE_INPUT_1M || typ > MEM_TYPE_DAILY)
		return ERR_MEM_TYPE;
	//データ時刻
	if (tim == 0)
		return ERR_MEM_TIME;
	//書き込みデータ
	if (dp == NULL)
		return ERR_MEM_DATA;

	//秒を０にする
	wtim = 0;
	dll_localtime24_0(tim, &tm24);
	dll_mktime24(&tm24, &wtim);

	//局種別・局番号・項目番号・メモリ
	for (k = 0; k < cnt; k++) {
		//局種別
		if (dp[k].kno < 1 || dp[k].kno > mp->kp->max)
			return ERR_MEM_KNO;
		if (mp->kp->pKind[dp[k].kno-1].kind == NULL)
			return ERR_MEM_KNO;
		//メモリ選択
		pp = selectDataMemory(mp, typ, dp[k].kno, wtim);
		//メモリは作成されていない
		if (pp->r.mem == NULL)
			return ERR_MEM_MEMORY;
		if (pp->d.mem == NULL)
			return ERR_MEM_MEMORY;
		//局番号
		if (dp[k].sno < 1 || dp[k].sno > pp->max)
			return ERR_MEM_SNO;
		//項目番号
		if (dp[k].ino < 1 || dp[k].ino > pp->item)
			return ERR_MEM_ITEM;

		//局種別に書き込み要求あり
		kidx[dp[k].kno-1] = 1;
	}

	//全局種別
	for (i = 0; i < mp->kp->max; i++) {
		if (kidx[i] == 0) continue;

		//各種ポインタの設定
		pp = selectDataMemory(mp, typ, i+1, wtim);
		tp = (mem_obstime_t *)pp->r.mem;
		rp = pp->r.mem + sizeof(mem_obstime_t);
		bp = (bf_data_t *)pp->d.mem;

		//更新時刻が変化したならメモリの初期化
		obsToTime(tp, &rtim);
		if (rtim != wtim)
			initDataMemory(mp, pp, wtim);

		//編集領域の確保
		wp = new bf_data_t[pp->item];
		if (wp == NULL)
			return ERR_MEM_ALLOC;

		//最大局数まで
		for (j = 0; j < pp->max; j++) {
			flg = FALSE;
			memcpy(wp, &bp[j * pp->item], sizeof(bf_data_t) * pp->item);

			for (k = 0; k < cnt; k++) {
				if (dp[k].kno == i+1 && dp[k].sno == j+1) {
					//編集領域に書き込み
					memcpy(&wp[dp[k].ino-1], &dp[k].dt, sizeof(bf_data_t));
					flg = TRUE;
				}
			}

			//更新があれば共有メモリに書き込み
			if ( flg ) {
				ret = writeDataMemoryS_nOW(mp, typ, i+1, wtim, j+1, (char *)wp);
				if (ret != 0) {
					if (wp != NULL) free(wp);
					return ret;
				}
			}
		}
		if (wp != NULL) free(wp);
	}
	return 0;
}

//2003.06.23 追加（ここまで）----------------------------------------
