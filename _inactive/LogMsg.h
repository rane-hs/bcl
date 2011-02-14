
#ifndef _CLOG_MSG_H_
#define _CLOG_MSG_H_


#include <string>
#include <stdio.h>

#ifdef  __cplusplus
extern "C" {
#endif

#include "./clib/timeDll.h"
#include "./clib/logDll.h"

#ifdef  __cplusplus
}
#endif


//ログ出力
#define DG_LOG_ERR          0           //エラー
#define DG_LOG_INFO         1           //詳細
#define DG_LOG_WARN         2           //警告
#define DG_LOG_DEBUG        3           //デバッグ
#define DG_LOG_NON          5           //ログには出力しない


#define MAX_SIZE_KIND       64          //ログ種別名サイズ
#define MAX_SIZE_MSG        1024        //ログメッセージサイズ
#define MAX_SIZE_LOGFILE    1000000     //ログファイルサイズ（Byte）





/********************************************************************
  クラス：CLogMsg
  機能  ：ログメッセージ出力クラス
  説明  ：
********************************************************************/
class CLogMsg {
public:

	int    office_no;               //事務所番号
	int    unit_no;                 //装置番号
	int    horo_no;                 //方路番号
	std::string exe_name;                //ソフト名称



	/********************************************************************
	  関数  ：CLogMsg
	  機能  ：コンストラクタ
	  引数  ：
	  戻り値：
	  説明  ：
	********************************************************************/
	CLogMsg();

	/********************************************************************
	  関数  ：init
	  機能  ：初期化
	  引数  ：my_name           [IN]  ソフト名称
	        ：office            [IN]  事務所番号
	        ：unit              [IN]  装置番号
	        ：horo              [IN]  方路番号
	  戻り値：
	  説明  ：
	********************************************************************/
	int init(const char *my_name, int office, int unit, int horo);

	/********************************************************************
	  関数  ：put_msg
	  機能  ：メッセージ出力
	  引数  ：typ               [IN]  メッセージタイプ
	        ：knd               [IN]  情報種別
	        ：buf               [IN]  出力メッセージ
	        ：...               [IN]  パラメータ
	  戻り値：
	  説明  ：
	********************************************************************/
	void put_msg(int typ, const char *knd, const char *buf, ...);

	/********************************************************************
	  関数  ：dbg_typ
	  機能  ：メッセージタイプ設定（デバッグ用）
	  引数  ：flg               [IN]  デバッグフラグ
	        ：lv                [IN]  デバッグレベル
	        ：typ               [IN]  メッセージタイプ
	  戻り値：
	  説明  ：
	********************************************************************/
	int dbg_typ(int flg, int lv, int typ);
};


#endif
