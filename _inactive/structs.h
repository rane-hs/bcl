#include "bcl.h"

M__BEGIN_NAMESPACE_BCL

/********************************************************************
  構造体：最新時刻情報
  項目  ：year          年(1～20XX)
        ：month         月(1～12)
        ：day           日(1～31)
        ：hour          時(0～24)
        ：minute        分(0～59)
********************************************************************/
typedef struct tagNowTime {
	short year;                     //年
	char  month;                    //月
	char  day;                      //日
	char  hour;                     //時
	char  minute;                   //分
} bf_nowtime_t;

/********************************************************************
  構造体：更新情報
  項目  ：month         月(1～12)
        ：day           日(1～31)
        ：hour          時(0～24)
        ：minute        分(0～59)
********************************************************************/
typedef union tagObserverTime {
	{
		char month;                     //月
		char day;                       //日
		char hour;                      //時
		char minute;                    //分
	}
	unsigned char data[4];
	operator=(long dat){*(&dat[0]) = (unsigned char *)data;};
} bf_obstime_t;

/********************************************************************
  構造体：オフセット情報
  項目  ：cnt           データ部の実装局数
        ：size          実装局１局あたりの総バイト数
********************************************************************/
typedef struct tagFileOffset {
  unsigned short cnt;               //データ部の実装局数
  unsigned short size;              //実装局１局あたりの総バイト数
} bf_offset_t;

/********************************************************************
  構造体：インデックス情報部
  項目  ：offset        オフセット情報
        ：st            局の実装局順番(0 = 非実装)
********************************************************************/
typedef struct tagFileIndex {
  bf_offset_t    offset;            //オフセット情報
  unsigned short *st;              //局の実装局順番
} bf_index_t;

/********************************************************************
  構造体：データ
  項目  ：dt            データ
        ：f             フラグ
********************************************************************/
typedef struct tagFileData {
  long          dt;                 //データ
  unsigned char f[4];               //フラグ
  unsigned char f1(){return f[0];};
  unsigned char f2(){return f[1];};
  unsigned char f3(){return f[2];};
  unsigned char f4(){return f[3];};
} bf_data_t;

/********************************************************************
  構造体：制御指令ファイル実施情報
  項目  ：order         制御指令実施フラグ（1=実施する,0=実施済み,負=指令項目異常）
        ：reserve1      予備
        ：reserve2      予備
        ：reserve3      予備
        ：tim           制御指令実施時刻
********************************************************************/
typedef struct tagSystemControl {
	char order;                     //制御指令実施フラグ
	char reserve1;                  //予備
	char reserve2;                  //予備
	char reserve3;                  //予備
	bf_obstime_t tim;               //制御指令実施時刻
} bf_syscontrol_t;

M__END_NAMESPACE_BCL
