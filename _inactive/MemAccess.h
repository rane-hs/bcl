/********************************************************************
  機能    ：共有メモリアクセス
  更新履歴：2002/04/10  新規作成
          ：2002/04/27  項目単位の書き込み関数廃止
          ：2002/05/13  現況メモリ・最新メモリを追加
          ：2002/06/13  項目単位を局毎にまとめて書き込む関数を追加
          ：2002/06/28  局種別情報をメモリ管理情報内に持つことにした
          ：2002/07/03  ポインタがＮＵＬＬの場合の判定を強化
          ：2002/08/22  １分・正定時の中での最新時刻取得関数を追加
          ：2002/08/30  項目単位書込み関数にて最初の局が初期化されない問題を修正
          ：2002/09/09  現況メモリの更新時刻が書き込まれない事を修正
          ：2002/10/15  selectDataMemory関数のtimのエラー判定を追加
          ：2003/05/29  正常データでも定時間でフラグが異常な項目は書き込まない事になった
          ：----------  これ以降はVer 1.3  ----------
          ：2003/06/10  共有メモリの宣言方法を変更
          ：2003/06/23  既に更新されている局は上書きしない関数を追加
          ：2003/08/06  コメントの修正
********************************************************************/

#ifndef _MEMACCESS_H_
#define _MEMACCESS_H_

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "./include/bousai.h"
#include "./include/sharedMemDll.h"
#include "./include/timeDll.h"
#include "./include/lockMutex.h"
#include "systemConst.h"


#ifdef  __cplusplus
extern "C" {
#endif


//エラー番号
#define ERR_MEM_SUCCESS  0
#define ERR_MEM_MEMORY  -1      //メモリは作成されていない
#define ERR_MEM_MP      -2      //メモリ管理領域がＮＵＬＬ
#define ERR_MEM_KP      -3     //局種別情報管理領域がＮＵＬＬ
#define ERR_MEM_TYPE    -4      //メモリタイプが異常
#define ERR_MEM_KNO     -5      //局種別番号が範囲外
#define ERR_MEM_TIME    -6      //データ時刻が指定されていない
#define ERR_MEM_DATA    -7      //書き込みデータがＮＵＬＬ
#define ERR_MEM_SNO     -8      //局番号が範囲外
#define ERR_MEM_ITEM    -9      //項目番号が範囲外
#define ERR_MEM_ALLOC   -10     //領域確保失敗


/********************************************************************
  構造体：項目書き込み要求
  項目  ：kno           局種別
        ：sno           局番号
        ：ino           項目
        ：dt            データ
********************************************************************/
typedef struct tagRequestData {
	int kno;                        //局種別
	int sno;                        //局番号
	int ino;                        //項目番号
	bf_data_t dt;                   //データ
} mem_reqdata_t;

/********************************************************************
  構造体：データメモリ更新時刻
  項目  ：yy            年
        ：mm            月
        ：dd            日
        ：hh            時
        ：nn            分
********************************************************************/
typedef struct tagObserveTime {
	short yy;                       //年
	char  mm;                       //月
	char  dd;                       //日
	char  hh;                       //時
	char  nn;                       //分
} mem_obstime_t;

/********************************************************************
  構造体：データメモリ情報(１面)
  項目  ：r             更新情報
        ：d             データ
        ：typ           データメモリタイプ
        ：kno           局種別番号
        ：max           最大局数
        ：item          項目数
********************************************************************/
#define MEM_TYPE_INPUT_1M   1       //入力データ(１分)
#define MEM_TYPE_INPUT_HM   2       //入力データ(正定時)
#define MEM_TYPE_DATA_1M    3       //演算データ(１分)
#define MEM_TYPE_DATA_HM    4       //演算データ(正定時)
#define MEM_TYPE_DAILY      5       //演算データ(日集計)
#define MEM_TYPE_NOW        6       //最新データ
#define MEM_TYPE_NOW_1M     7       //１分現況データ
#define MEM_TYPE_NOW_HM     8       //正定時現況データ
typedef struct tagDataMemory {
	sharedMem_t r;                  //更新情報
	sharedMem_t d;                  //データ
	int         typ;                //データメモリタイプ
	int         kno;                //局種別番号
	int         max;                //最大局数
	int         item;               //項目数
} mem_datamem_t;

/********************************************************************
  構造体：データメモリ情報(１種別)
  項目  ：min           １分データ
        ：tim           正定時データ
        ：day           日集計データ(入力データメモリでは未使用)
        ：now           最新データ
        ：now_1m        １分現況データ
        ：now_hm        正定時現況データ
********************************************************************/
typedef struct tagMemoryKind {
//2003.06.10 追加（ここから）----------------------------------------
//共有メモリの宣言方法を変更
	sharedMem_t   mp;               //共有メモリ情報
//2003.06.10 追加（ここまで）----------------------------------------
	mem_datamem_t min[10];          //１分データ
	mem_datamem_t tim[6];           //正定時データ
	mem_datamem_t day;              //日集計データ
	mem_datamem_t now;              //最新データ
	mem_datamem_t now_1m;           //１分現況データ
	mem_datamem_t now_hm;           //正定時現況データ
} mem_kind_t;

/********************************************************************
  構造体：データメモリ管理情報
  項目  ：tmem          入力データメモリ
        ：dmem          演算データメモリ
        ：hLockInput    入力データメモリロック用
        ：hLockData     演算データメモリロック用(１分・正定時)
        ：hLockDaily    演算データメモリロック用(日集計)
        ：kp            局種別管理情報
********************************************************************/
#define LOCK_NAME_INPUT "LOCK_TMEM"
#define LOCK_NAME_DATA  "LOCK_DMEM"
#define LOCK_NAME_DAILY "LOCK_DMEM_DAILY"
typedef struct tagMemoryManager {
	mem_kind_t tmem[99];            //入力データメモリ
	mem_kind_t dmem[99];            //演算データメモリ
	HANDLE     hLockInput;          //入力データメモリロック用
	HANDLE     hLockData;           //演算データメモリロック用(１分・正定時)
	HANDLE     hLockDaily;          //演算データメモリロック用(日集計)
	kind_manager_t *kp;             //局種別管理情報
} mem_manager_t;



/********************************************************************
  プロトタイプ宣言
********************************************************************/
void obsToTime(mem_obstime_t *p, time_t *tm);
void timeToObs(time_t tm, mem_obstime_t *p);

int  openDataMemory   (mem_manager_t *mp, kind_manager_t *kp);
void closeDataMemory  (mem_manager_t *mp);
void initDataMemoryHM (mem_manager_t *mp, int kno, char *p);
void initDataMemoryDay(mem_manager_t *mp, int kno, char *p);
int  writeDataMemory  (mem_manager_t *mp, int typ, int kno, time_t tim, char *dp);
int  writeDataMemoryS (mem_manager_t *mp, int typ, int kno, time_t tim, int sno, char *dp);
int  writeDataMemoryI (mem_manager_t *mp, int typ, time_t tim, int cnt, mem_reqdata_t *dp);
int  readDataMemoryI  (mem_manager_t *mp, int typ, time_t tim, int cnt, mem_reqdata_t *dp);

mem_datamem_t *selectDataMemory(mem_manager_t *mp, int typ, int kno, time_t tim);
mem_datamem_t *checkDataMemory (mem_manager_t *mp, int typ, int kno, time_t tim);
int getUpToDateTime(mem_manager_t *mp, int typ, int kno, time_t *tt);

/**********************************************************
  関数  ：writeDataMemoryS_nOW
  機能  ：共有データメモリ書き込み(１局全項目)
  引数  ：mp                [I/O] データメモリ情報
        ：typ               [IN]  データメモリタイプ
        ：kno               [IN]  局種別番号
        ：tim               [IN]  データ時刻
        ：sno               [IN]  局番号
        ：dp                [IN]  書き込みデータ
  戻り値：
  説明  ：既に更新されている局は上書きしない
**********************************************************/
int writeDataMemoryS_nOW(mem_manager_t *mp, int typ, int kno, time_t tim, int sno, char *dp);

/**********************************************************
  関数  ：writeDataMemoryI_nOW
  機能  ：共有データメモリ書き込み(ｎ局ｎ項目)
  引数  ：mp                [I/O] データメモリ管理情報
        ：typ               [IN]  データメモリタイプ
        ：tim               [IN]  データ時刻
        ：cnt               [IN]  書き込み項目件数
        ：dp                [IN]  書き込みデータ
  戻り値：
  説明  ：既に更新されている局は上書きしない
**********************************************************/
int writeDataMemoryI_nOW(mem_manager_t *mp, int typ, time_t tim, int cnt, mem_reqdata_t *dp);


#ifdef  __cplusplus
}
#endif


#endif
