#include "stdafx.h"
#pragma comment(lib, "lib/logDll.lib")

#include "CLogMsg.h"
using namespace std;



/********************************************************************
  関数  ：CLogMsg
  機能  ：コンストラクタ
  引数  ：
  戻り値：
  説明  ：
********************************************************************/
CLogMsg::CLogMsg()
{
	exe_name  = "";                 //ソフト名称
	office_no = 0;                  //事務所番号
	unit_no   = 0;                  //装置番号
	horo_no   = 0;                  //方路番号
}

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
int CLogMsg::init(const char *my_name, int office, int unit, int horo)
{
	exe_name  = my_name;            //ソフト名称
	office_no = office;             //事務所番号
	unit_no   = unit;               //装置番号
	horo_no   = horo;               //方路番号

	return 0;
}

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
void CLogMsg::put_msg(int typ, const char *knd, const char *buf, ...)
{
	char msg[MAX_SIZE_MSG];
	char fname[_MAX_PATH];
	char wk[MAX_SIZE_MSG];
	char stim[24];
	char styp[24];
	char sknd[24];
	va_list ap;
	tm24_t tm24;


	//可変引数処理
	va_start(ap, buf);
	vsprintf(wk, buf, ap);
	va_end(ap);

	//現在時刻の取得
	dll_getNowTime24(&tm24);
	sprintf(stim, "%04d/%02d/%02d %02d:%02d:%02d ",
		tm24.tm_year, tm24.tm_mon, tm24.tm_mday,
		tm24.tm_hour, tm24.tm_min, tm24.tm_sec);

	//情報種別
	if (knd == NULL)
		sknd[0] = '\0';
	else if (knd[0] == '\0')
		sknd[0] = '\0';
	else
		sprintf(sknd, "[%s] ", knd);

	//詳細 
	switch( typ ){
	case DG_LOG_ERR :
		sprintf(styp, "[エラー] ");
		break;
	case DG_LOG_INFO :
		sprintf(styp, "[ 詳細 ] ");
		break;
	case DG_LOG_WARN :
		sprintf(styp, "[ 警告 ] ");
		break;
	case DG_LOG_NON :
		printf("%s[      ] %s%s\n", stim, sknd, wk);
		return;
	default :
		sprintf(styp, "[ 情報 ] ");
		break;
	}

	//メッセージ出力
	sprintf(msg, "%s%s%s%s", stim, styp, sknd, wk);
	printf("%s\n", msg);

	//ログ出力パス設定
	if (knd == NULL)
		sprintf(fname, "..\\%s\\log\\%s_%02d%02d%02d.log", exe_name.c_str(), exe_name.c_str(), office_no, unit_no, horo_no);
	else if (knd[0] == '\0')
		sprintf(fname, "..\\%s\\log\\%s_%02d%02d%02d.log", exe_name.c_str(), exe_name.c_str(), office_no, unit_no, horo_no);
	else
		sprintf(fname, "..\\%s\\log\\%s_%s_%02d%02d%02d.log", exe_name.c_str(), exe_name.c_str(), knd, office_no, unit_no, horo_no);

	//ログ出力
	dll_writelog(fname, MAX_SIZE_LOGFILE, strlen(msg), msg);
}


/********************************************************************
  関数  ：dbg_typ
  機能  ：メッセージタイプ設定（デバッグ用）
  引数  ：flg               [IN]  デバッグフラグ
        ：lv                [IN]  デバッグレベル
        ：typ               [IN]  メッセージタイプ
  戻り値：
  説明  ：
********************************************************************/
int CLogMsg::dbg_typ(int flg, int lv, int typ)
{
	if (flg >= lv) {
		return typ;
	} else {
		return DG_LOG_NON;
	}
}
