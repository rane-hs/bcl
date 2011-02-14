/********************************************************************
  機能    ：システム定数読み込み
  更新履歴：2002/04/09  新規作成
          ：2002/06/04  CSVファイルから１行読み込む関数を追加
          ：2002/06/11  装置間通信情報読み込み関数の追加  
          ：2002/06/27  局種別情報を共有メモリ化
          ：2002/07/25  水位演算定数の定義、ダム諸量演算方式構造体の変更
          ：2002/08/26  １分データの保存日数を追加         
          ：2002/10/29  雨量演算方式定義の追加
          ：2002/11/19  ファイル読み込み時に更新日時を比較するように変更
          ：2003/02/27  サーバ初期化ファイルにデータ未受信判定期間と過去データ判定期間を追加
          ：2003/06/12  コメントの修正
		  ：            ～～～ 以降Ver1.3 ～～～
          ：2003/06/04  積雪、気温、路温、堰、排水、視程局の追加
          ：2003/07/02  風向風速演算方式定義の追加
          ：2003/08/21  未実装だった局名称情報、局補足名称情報、局コード情報を実装
          ：2003/08/22  システム定数即時反映に対応した関数を追加
          ：2003/10/10  積雪、気温演算定数、気温、路温警報基準値の変更
          ：2003/11/07  コメントの修正
          ：2003/12/19  csvread()で最初の１文字がNULLの場合に対応
          ：            関数csvread()からcsvread_n()へ置換え
          ：            関数csvsplit()からcsvsplit_n()へ置換え
		  ：2004/01/09  流域平均雨量局の追加
		  ：2004/01/14  予測水位局の追加
********************************************************************/

#ifndef _SYSTEMCONST_H_
#define _SYSTEMCONST_H_

#include <windows.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>


#define SYS_ERR_SUCCESS         0       //正常
#define SYS_ERR_FAILURE         -1      //異常
#define SYS_ERR_POINTER         -10     //ポインタがＮＵＬＬ
#define SYS_ERR_SERVER_INFO     -11     //サーバ初期化ファイルが異常
#define SYS_ERR_FILE_OPEN       -20     //ファイルオープン失敗
#define SYS_ERR_SHARED_MEMORY   -30     //共有メモリオープン失敗
#define SYS_ERR_ALLOC_MEMORY    -31     //メモリ取得失敗
#define SYS_ERR_LOCK_HANDLE     -40     //ロックハンドル取得失敗
#define SYS_ERR_LOCK_TIMEOUT    -41     //ロックタイムアウト


#define SYS_MAX_LINE        2048        //CSVファイル１行の最大サイズ
#define SYS_MAX_KIND        99          //最大局種別数
#define SYS_MAX_MNO         999         //最大管理番号

#define SYS_LOCK_TIMEOUT    5000        //ロックタイムアウト値(ミリ秒)
#define SYS_NAME_LOCK   "SYS_LOCK_KIND" //ロック用名称
#define SYS_NAME_KIND   "SYS_MEM_KIND"  //共有メモリ名



/********************************************************************
  構造体：サーバ情報
  項目  ：strAppPath            アプリケーションパス
        ：strDataPath           データパス
        ：strMO                 ＭＯドライブ
        ：intDays               ファイル保存期間(日)
        ：intDays1M             １分ファイル保存期間(日)
        ：intNowFile            現況ファイル作成有無(1=作成する,以外=作成しない)
        ：intBeFore             データ未受信判定期間(分)
        ：intBeHind             過去データ判定期間(分)
********************************************************************/
#define INI_NOW_MODE_CREATE 1       //現況ファイル作成
typedef struct tagIniServer {
	char    strAppPath[_MAX_PATH];  //アプリケーションパス
	char    strDataPath[_MAX_PATH]; //データパス
	char    strMO[_MAX_DRIVE];      //ＭＯドライブ
	int		intDays;                //ファイル保存期間
	int     intDays1M;              //１分ファイル保存期間
	int     intNowFile;             //現況ファイル作成有無
	int     intBeFore;              //データ未受信判定期間
	int     intBeHind;              //過去データ判定期間
	int     intDaysNow;				// 実況ファイル保存期間
	int     intDaysCast;			// 予測ファイル保存期間
} ini_server_t;


/********************************************************************
  構造体：自装置番号
  項目  ：intOfficeID           自事務所番号
        ：intUnitID             自装置番号
********************************************************************/
typedef struct tagSysMyNumber {
	int     intOfficeID;            //自事務所番号
	int     intUnitID;              //自装置番号
} sys_mynumber_t;


/********************************************************************
  構造体：局種別情報
  項目  ：kno                   局種別
        ：name                  局種別名(ディレクトリ名、ファイル名の元になる)
        ：jname                 局種別名(和名)
        ：max                   最大局数
        ：titem                 観測項目数
        ：ditem                 日集計項目数
        ：bitem                 演算基礎項目数
********************************************************************/
#define LEN_KIND_NAME       16      //局種別名サイズ
#define LEN_KIND_JNAME      32      //局種別(和名)サイズ
typedef struct tagSysKind {
	int     kno;                    //局種別
	char    name[LEN_KIND_NAME];    //局種別名
	char    jname[LEN_KIND_JNAME];  //局種別名(和名)
	int     max;                    //最大局数
	int     titem;                  //観測項目数
	int     ditem;                  //日集計項目数
	int     bitem;                  //演算基礎項目数
} sys_kind_t;


/********************************************************************
  構造体：局実装情報
  項目  ：sno                   局番号            (1～999,     以外:設定値なし)
        ：entry                 実装情報          (1:実装,     以外:非実装)
        ：kei                   系情報            (1～99,      以外:非実装)
        ：cycle                 データ保存周期    (1:1分周期,  以外:10分周期)
        ：tcalc                 演算実施情報      (1:演算する, 以外:演算しない)
        ：dcalc                 日集計演算実施情報(1:演算する, 以外:演算しない)
********************************************************************/
#define SYS_LOCAL_ENTRY_ON 1        //実装
#define SYS_LOCAL_CYCLE_1M 1        //１分周期
#define SYS_LOCAL_TCALC_IP 1        //イベントポーリング演算を行う
#define SYS_LOCAL_TCALC_10 2        //10分周期演算を行う
#define SYS_LOCAL_TCALC_01 3        //１分周期演算を行う
#define SYS_LOCAL_DCALC_DO 1        //日集計演算を行う
typedef struct tagSysLocal {
	int     sno;                    //局番号
	int     entry;                  //実装情報
	int     kei;                    //系情報
	int     cycle;                  //データ保存周期
	int     tcalc;                  //演算実施情報
	int     dcalc;                  //日集計演算実施情報
	int     reserve1;               //予備
	int     reserve2;               //予備
} sys_local_t;


/********************************************************************
  構造体：局稼動情報
  項目  ：sno                   局番号
        ：action                局稼動情報(1:保守中, 2:閉局, 以外:局実装情報に依存)
********************************************************************/
#define SYS_ACTION_ACTION_MAINTE 1  //保守中
#define SYS_ACTION_ACTION_CLOSE  2  //閉局
typedef struct tagSysAction {
	int     sno;                    //局番号
	int     action;                 //局稼動情報
} sys_action_t;


/********************************************************************
  構造体：局名称情報
  項目  ：sno                   局番号
        ：mno1                  管理番号１
        ：mno2                  管理番号２
        ：mno3                  管理番号３
        ：mno4                  管理番号４
        ：acode                 指定観測所コード
        ：pcode1                基準点１コード
        ：pcode2                基準点２コード
        ：mcode                 管理者種別コード
        ：office_name           所轄事務所名
        ：station_name          観測所名
        ：valley_name           流域名
        ：rsys_name             水系名 river system
        ：river_name            河川名
        ：represe_name          代表所在名 represe seat
        ：route                 路線種別
        ：route_name            路線名
        ：sub_name1             観測所名（サブ１）
        ：sub_name2             観測所名（サブ２）
        ：shorts_name           略局名
        ：sabou_block           砂防ブロック名
        ：pref_name             都道府県名 prefecture
        ：city_name             群・市名
        ：area_name             区・町・村名
        ：addr_name             大字・地番等名
        ：lat                   観測所緯度（度）station latitude
        ：lat_min               観測所緯度（分）
        ：lat_sec               観測所緯度（秒）
        ：lon                   観測所経度（度）
        ：lon_min               観測所経度（分）
        ：lon_sec               観測所経度（秒）
        ：reserve1              予備1
        ：reserve2              予備2
********************************************************************/
#define SYS_NAME_ACODE_APPOINT  1   //指定観測所
#define SYS_NAME_PCODE_STANDARD 1   //基準点観測所
#define SYS_NAME_MCODE_KASEN    1   //県河川課
#define SYS_NAME_MCODE_SABOU    2   //県砂防課
#define SYS_NAME_MCODE_ROAD     3   //県道路課
#define SYS_NAME_MCODE_DAM      4   //県ダム課
#define SYS_NAME_MCODE_POST     5   //他部署
#define SYS_NAME_MCODE_MLIT     11  //国土交通省 Ministry of Land,Infrastructure and Transport
#define SYS_NAME_MCODE_JMA      12  //気象庁 Japan Meteorological Agency
#define SYS_NAME_MCODE_CITY     13  //市町村
#define SYS_NAME_MCODE_PREF     14  //他県 prefecture
#define SYS_NAME_MCODE_OTHER    15  //その他
#define SYS_NAME_ROUTE_NATIONAL 1   //国道 national road
#define SYS_NAME_ROUTE_PREF     2   //一般県道
#define SYS_NAME_ROUTE_LOCAL    3   //主要地方道
typedef struct tagSysName {
	int     sno;                    //局番号
	int     mno1;                   //管理番号１
	int     mno2;                   //管理番号２
	int     mno3;                   //管理番号３
	int     mno4;                   //管理番号４
	int     acode;                  //指定観測所コード
	int     pcode1;                 //基準点１コード
	int     pcode2;                 //基準点２コード
	int     mcode;                  //管理者種別コード
	char    office_name[16];        //所轄事務所名
	char    station_name[16];       //観測所名
	char    valley_name[16];        //流域名
	char    rsys_name[16];          //水系名
	char    river_name[16];         //河川名
	char    represe_name[16];       //代表所在名
	int     route;                  //路線種別
	char    route_name[16];         //路線名
	char    sub_name1[16];          //観測所名（サブ１）
	char    sub_name2[16];          //観測所名（サブ２）
	char    shorts_name[16];        //略局名
	char    sabou_block[16];        //砂防ブロック名
	char    pref_name[16];          //都道府県名
	char    city_name[16];          //群・市名
	char    area_name[16];          //区・町・村名
	char    addr_name[24];          //大字・地番等名
	int	    lat;                    //観測所緯度（度）
	int     lat_min;                //観測所緯度（分）
	double  lat_sec;                //観測所緯度（秒）
	int     lon;                    //観測所経度（度）
	int     lon_min;                //観測所経度（分）
	double  lon_sec;                //観測所経度（秒）
	int     reserve1;               //予備1
	int     reserve2;               //予備2
} sys_name_t;


/********************************************************************
  構造体：局補足名称情報
  項目  ：sno                   局番号
        ：altitude              標高
        ：rever_dis             河口からの距離
        ：member                観測員名
        ：telno                 電話番号
        ：ctl_name              管理団体名 
        ：sensor_code           センサー種別コード
        ：kind_code             観測種別コード
        ：start_year            観測開始日付（年）
        ：start_mon             観測開始日付（月）
        ：start_day             観測開始日付（日）
        ：obs_area_suibou       監視対象区域名称（水防）
        ：obs_area_sabo         監視対象区域名称（砂防）
        ：obs_area_road         監視対象区域名称（道路）
        ：reserve1              予備1
        ：reserve2              予備2
********************************************************************/
typedef struct tagSysOptName {
	int     sno;                    //局番号
	double  altitude;               //標高
	long    rever_dis;              //河口からの距離
	char    member[16];             //観測員名
	char    tel_no[16];             //電話番号
	char    ctl_name[32];           //管理団体名 
	char    sensor_code[32];        //センサー種別コード
	char    kind_code[16];          //観測種別コード
	int     start_year;             //観測開始日付（年）
	int     start_mon;              //観測開始日付（月）
	int     start_day;              //観測開始日付（日）
	char    obs_area_suibou[32];    //監視対象区域名称（水防）
	char    obs_area_sabo[32];      //監視対象区域名称（砂防）
	char    obs_area_road[32];      //監視対象区域名称（道路）
	short   reserve1;               //予備1
	short   reserve2;               //予備2
} sys_optname_t;


/********************************************************************
  構造体：局コード情報
  項目  ：sno                   局番号
        ：area_no               エリア番号（国）
        ：office_no             事務所番号（国）
        ：river_sys_code        水系域コード（国）river system  area
        ：river_code            河川コード（国）
        ：observatory_no        観測所番号（国）
        ：pref_code             都道府県コード（国）
        ：city_code             市町村コード（国）
        ：reserve1              予備1
        ：reserve2              /予備2
********************************************************************/
typedef struct tagSysCode {
	int     sno;                    //局番号
	int     area_no;                //エリア番号（国）
	int     office_no;              //事務所番号（国）
	int     river_sys_code;         //水系域コード（国）
	int     river_code;             //河川コード（国）
	int     observatory_no;         //観測所番号（国）
	int     pref_code;              //都道府県コード（国）
	int     city_code;              //市町村コード（国）
	int     reserve1;               //予備1
	int     reserve2;               //予備2
} sys_code_t;


/********************************************************************
  構造体：局構成情報１
  項目  ：sno                   局番号
        ：
        ：    ～～～以下略～～～
        ：
********************************************************************/
typedef struct tagSysComp1 {
	int     sno;                    //局番号
	int     office_no;              //事務所番号
	//センサー１
	char    s1_sensor[24];          //センサー１
	char    s1_form[24];            //形式１
	char    s1_type[24];            //型名１
	char    s1_date_1[32];          //日付１－１
	char    s1_date_2[32];          //日付１－２
	char    s1_date_3[32];          //日付１－３
	char    s1_introduce[24];       //導入業者１
	char    s1_intro_tel[32];       //連絡先１
	char    s1_chacker[24];         //点検業者１
	char    s1_chk_tel[32];         //連絡先１
	char    s1_note_1[32];          //備考１－１
	char    s1_note_2[32];          //備考１－２
	char    s1_note_3[32];          //備考１－３
	char    s1_note_4[32];          //備考１－４
	//センサー２
	char	s2_sensor[24];          //センサー２
	char	s2_form[24];            //形式２
	char	s2_type[24];            //型名２
	char    s2_date_1[32];          //日付２－１
	char    s2_date_2[32];          //日付２－２
	char    s2_date_3[32];          //日付２－３
	char	s2_introduce[24];       //導入業者２
	char	s2_intro_tel[32];       //連絡先２
	char	s2_chacker[24];         //点検業者２
	char	s2_chk_tel[32];         //連絡先２
	char    s2_note_1[32];          //備考２－１
	char    s2_note_2[32];          //備考２－２
	char    s2_note_3[32];          //備考２－３
	char    s2_note_4[32];          //備考２－４
	//センサー３
	char    s3_sensor[24];          //センサー３
	char    s3_form[24];            //形式３
	char    s3_type[24];            //型名３
	char    s3_date_1[32];          //日付３－１
	char    s3_date_2[32];          //日付３－２
	char    s3_date_3[32];          //日付３－３
	char    s3_introduce[24];       //導入業者３
	char    s3_intro_tel[32];       //連絡先３
	char    s3_chacker[24];	        //点検業者３
	char    s3_chk_tel[32];         //連絡先３
	char    s3_note_1[32];          //備考３－１
	char    s3_note_2[32];          //備考３－２
	char    s3_note_3[32];          //備考３－３
	char    s3_note_4[32];          //備考３－４
	//センサー４
	char    s4_sensor[24];          //センサー４
	char    s4_form[24];            //形式４
	char    s4_type[24];            //型名４
	char    s4_date_1[32];          //日付４－１
	char    s4_date_2[32];          //日付４－２
	char    s4_date_3[32];          //日付４－３
	char    s4_introduce[24];		//導入業者４
	char    s4_intro_tel[32];       //連絡先４
	char    s4_checker[24];         //点検業者４
	char    s4_chk_tel[32];         //連絡先４
	char    s4_note_1[32];          //備考４－１
	char    s4_note_2[32];          //備考４－２
	char    s4_note_3[32];          //備考４－３
	char    s4_note_4[32];          //備考４－４
	//センサー５
	char    s5_sensor[24];          //センサー５
	char    s5_form[24];            //形式５
	char    s5_type[24];            //型名５
	char    s5_date_1[32];          //日付５－１
	char    s5_date_2[32];          //日付５－２
	char    s5_date_3[32];          //日付５－３
	char    s5_introduce[24];		//導入業者５
	char    s5_intro_tel[32];       //連絡先５
	char    s5_checker[24];         //点検業者５
	char    s5_chk_tel[32];         //連絡先５
	char    s5_note_1[32];          //備考５－１
	char    s5_note_2[32];          //備考５－２
	char    s5_note_3[32];          //備考５－３
	char    s5_note_4[32];          //備考５－４

	//記録計１
	char    r1_recorder[24];        //記録計１
	char    r1_form[24];            //形式１
	char    r1_type[24];            //型名１
	char    r1_begin[24];           //運用開始日１
	char    r1_update[24];          //更新年月日１
	char    r1_introduce[24];       //導入業者１
	char    r1_intro_tel[32];       //連絡先１
	char    r1_checker[24];         //点検業者１
	char    r1_chk_tel[32];         //連絡先１
	char    r1_reel_day[32];        //記録計の巻日数１
	char    r1_breakdown[32];       //記録計の故障長期欠測有無１
	char    r1_note_1[32];          //備考１－１
	char    r1_note_2[32];          //備考１－２
	//記録計２
	char    r2_recorder[24];        //記録計２
	char    r2_form[24];            //形式２
	char    r2_type[24];            //型名２
	char    r2_begin[24];           //運用開始日２
	char    r2_update[24];          //更新年月日２
	char    r2_introduce[24];       //導入業者２
	char    r2_intro_tel[32];       //連絡先２
	char    r2_checker[24];         //点検業者２
	char    r2_chk_tel[32];         //連絡先２
	char    r2_reel_day[32];        //記録計の巻日数２
	char    r2_breakdown[32];       //記録計の故障長期欠測有無２
	char    r2_note_1[32];          //備考２－１
	char    r2_note_2[32];          //備考２－２
	//記録計３
	char    r3_recorder[24];        //記録計３
	char    r3_form[24];            //形式３
	char    r3_type[24];            //型名３
	char    r3_begin[24];           //運用開始日３
	char    r3_update[24];          //更新年月日３
	char    r3_introduce[24];       //導入業者３
	char    r3_intro_tel[32];       //連絡先３
	char    r3_checker[24];         //点検業者３
	char    r3_chk_tel[32];         //連絡先３
	char    r3_reel_day[32];        //記録計の巻日数３
	char    r3_breakdown[32];       //記録計の故障長期欠測有無３
	char    r3_note_1[32];          //備考３－１
	char    r3_note_2[32];          //備考３－２
	//記録計４
	char    r4_recorder[24];        //記録計４
	char    r4_form[24];            //形式４
	char    r4_type[24];            //型名４
	char    r4_begin[24];           //運用開始日４
	char    r4_update[24];          //更新年月日４
	char    r4_introduce[24];       //導入業者４
	char    r4_intro_tel[32];       //連絡先４
	char    r4_checker[24];         //点検業者４
	char    r4_chk_tel[32];         //連絡先４
	char    r4_reel_day[32];        //記録計の巻日数４
	char    r4_breakdown[32];       //記録計の故障長期欠測有無４
	char    r4_note_1[32];          //備考４－１
	char    r4_note_2[32];          //備考４－２
	//記録計５
	char    r5_recorder[24];        //記録計５
	char    r5_form[24];            //形式５
	char    r5_type[24];            //型名５
	char    r5_begin[24];           //運用開始日５
	char    r5_update[24];          //更新年月日５
	char    r5_introduce[24];       //導入業者５
	char    r5_intro_tel[32];       //連絡先５
	char    r5_checker[24];         //点検業者５
	char    r5_chk_tel[32];         //連絡先５
	char    r5_reel_day[32];        //記録計の巻日数５
	char    r5_breakdown[32];       //記録計の故障長期欠測有無５
	char    r5_note_1[32];          //備考５－１
	char    r5_note_2[32];          //備考５－２
	//伝送回線
	char    t_transmitte[24];       //伝送回線
	char    t_form[24];             //形式
	char    t_type[24];             //型名
	char    t_begin[24];            //運用開始日
	char    t_update[24];           //更新年月日
	char    t_introduce[24];        //導入業者
	char    t_intro_tel[32];        //連絡先
	char    t_checker[24];          //点検業者
	char    t_chk_tel[32];          //連絡先
	char    t_snd_hz[32];           //送信周波数
	char    t_rcv_hz[32];           //受信周波数
	//空中線
	char    a_air_line[32];         //空中線
	char    a_form[32];             //通信方式
	char    a_note_1[32];           //備考
	char    a_note_2[32];           //備考
	//電源
	char    p_power[24];            //電源
	char    p_form[24];             //形式
	char    p_type[24];             //型名
	char    p_begin[24];            //運用開始日
	char    p_update[24];           //更新年月日
	char    p_introduce[24];        //導入業者
	char    p_intro_tel[32];        //連絡先
	char    p_checker[24];          //点検業者
	char    p_chk_tel[32];          //連絡先
	char    p_name[32];             //使用電源
	char    p_capacity[32];         //蓄電池容量
	char    p_note_1[32];           //備考
	char    p_note_2[32];           //備考
	//局舎
	int     o_office;               //局舎
	char    o_kind[24];             //局舎種類
	char    o_site[24];             //局舎敷地
	char    o_space[24];            //局舎建坪
	char    o_others[24];           //その他
	char    o_tenant[24];           //借地人名
	char    o_rent[24];             //借地代
	char    o_utility_pole[32];     //電柱共架
	char    o_rec_keep[32];         //記録の有無保管場所
	int     o_reserve1;             //予備
	int     o_reserve2;             //予備
	//テレメータ設置状況
	char    tm_telemeter[64];       //設置状況
	int     tm_year;                //テレメータ開始日付（年）
	int     tm_mon;                 //テレメータ開始日付（月）
	int     tm_day;                 //テレメータ開始日付（日）
	double  tm_back_basehi;         //背後地地盤高
	char    tm_sign[24];            //量水標
	int     tm_reserve1;            //予備
	int     tm_reserve2;            //予備
} sys_comp1_t;

/********************************************************************
  構造体：局構成情報２
  項目  ：sno                   局番号
        ：
        ：    ～～～以下略～～～
        ：
********************************************************************/
typedef struct tagSysComp2 {
	int     sno;                    //局番号
	int     office_no;              //事務所番号
	//事務所内装置１
	char    u1_unit[24];            //事務所内装置１
	char    u1_form[24];            //形式１
	char    u1_type[24];            //型名１
	char    u1_date_1[24];          //日付１－１
	char    u1_date_2[24];          //日付１－２
	char    u1_date_3[24];          //日付１－３
	char    u1_introduce[24];       //導入業者１
	char    u1_intro_tel[32];       //連絡先１
	char    u1_checker[24];         //点検業者１
	char    u1_chk_tel[32];         //連絡先１
	char    u1_note_1[32];          //備考１－１
	char    u1_note_2[32];          //備考１－２
	char    u1_note_3[32];          //備考１－３
	char    u1_note_4[32];          //備考１－４
	//事務所内装置２
	char    u2_unit[24];            //事務所内装置２
	char    u2_form[24];            //形式２
	char    u2_type[24];            //型名２
	char    u2_date_1[24];          //日付２－１
	char    u2_date_2[24];          //日付２－２
	char    u2_date_3[24];          //日付２－３
	char    u2_introduce[24];       //導入業者２
	char    u2_intro_tel[32];       //連絡先２
	char    u2_checker[24];         //点検業者２
	char    u2_chk_tel[32];         //連絡先２
	char    u2_note_1[32];          //備考２－１
	char    u2_note_2[32];          //備考２－２
	char    u2_note_3[32];          //備考２－３
	char    u2_note_4[32];          //備考２－４
	//事務所内装置３
	char    u3_unit[24];            //事務所内装置３
	char    u3_form[24];            //形式３
	char    u3_type[24];            //型名３
	char    u3_date_1[24];          //日付３－１
	char    u3_date_2[24];          //日付３－２
	char    u3_date_3[24];          //日付３－３
	char    u3_introduce[24];       //導入業者３
	char    u3_intro_tel[32];       //連絡先３
	char    u3_checker[24];         //点検業者３
	char    u3_chk_tel[32];         //連絡先３
	char    u3_note_1[32];          //備考３－１
	char    u3_note_2[32];          //備考３－２
	char    u3_note_3[32];          //備考３－３
	char    u3_note_4[32];          //備考３－４
	//事務所内装置４
	char    u4_unit[24];            //事務所内装置４
	char    u4_form[24];            //形式４
	char    u4_type[24];            //型名４
	char    u4_date_1[24];          //日付４－１
	char    u4_date_2[24];          //日付４－２
	char    u4_date_3[24];          //日付４－３
	char    u4_introduce[24];       //導入業者４
	char    u4_intro_tel[32];       //連絡先４
	char    u4_checker[24];         //点検業者４
	char    u4_chk_tel[32];         //連絡先４
	char    u4_note_1[32];          //備考４－１
	char    u4_note_2[32];          //備考４－２
	char    u4_note_3[32];          //備考４－３
	char    u4_note_4[32];          //備考４－４
	//事務所内装置５
	char    u5_unit[24];            //事務所内装置５
	char    u5_form[24];            //形式５
	char    u5_type[24];            //型名５
	char    u5_date_1[24];          //日付５－１
	char    u5_date_2[24];          //日付５－２
	char    u5_date_3[24];          //日付５－３
	char    u5_introduce[24];       //導入業者５
	char    u5_intro_tel[32];       //連絡先５
	char    u5_checker[24];         //点検業者５
	char    u5_chk_tel[32];         //連絡先５
	char    u5_note_1[32];          //備考５－１
	char    u5_note_2[32];          //備考５－２
	char    u5_note_3[32];          //備考５－３
	char    u5_note_4[32];          //備考５－４
	//事務所内装置６
	char    u6_unit[24];            //事務所内装置６
	char    u6_form[24];            //形式６
	char    u6_type[24];            //型名６
	char    u6_date_1[24];          //日付６－１
	char    u6_date_2[24];          //日付６－２
	char    u6_date_3[24];          //日付６－３
	char    u6_introduce[24];       //導入業者６
	char    u6_intro_tel[32];       //連絡先６
	char    u6_checker[24];         //点検業者６
	char    u6_chk_tel[32];         //連絡先６
	char    u6_note_1[32];          //備考６－１
	char    u6_note_2[32];          //備考６－２
	char    u6_note_3[32];          //備考６－３
	char    u6_note_4[32];          //備考６－４
	//事務所内装置７
	char    u7_unit[24];            //事務所内装置７
	char    u7_form[24];            //形式７
	char    u7_type[24];            //型名７
	char    u7_date_1[24];          //日付７－１
	char    u7_date_2[24];          //日付７－２
	char    u7_date_3[24];          //日付７－３
	char    u7_introduce[24];       //導入業者７
	char    u7_intro_tel[32];       //連絡先７
	char    u7_checker[24];         //点検業者７
	char    u7_chk_tel[32];         //連絡先７
	char    u7_note_1[32];          //備考７－１
	char    u7_note_2[32];          //備考７－２
	char    u7_note_3[32];          //備考７－３
	char    u7_note_4[32];          //備考７－４
	//事務所内装置８
	char    u8_unit[24];            //事務所内装置８
	char    u8_form[24];            //形式８
	char    u8_type[24];            //型名８
	char    u8_date_1[24];          //日付８－１
	char    u8_date_2[24];          //日付８－２
	char    u8_date_3[24];          //日付８－３
	char    u8_introduce[24];       //導入業者８
	char    u8_intro_tel[32];       //連絡先８
	char    u8_checker[24];         //点検業者８
	char    u8_chk_tel[32];         //連絡先８
	char    u8_note_1[32];          //備考８－１
	char    u8_note_2[32];          //備考８－２
	char    u8_note_3[32];          //備考８－３
	char    u8_note_4[32];          //備考８－４
	//事務所内装置９
	char    u9_unit[24];            //事務所内装置９
	char    u9_form[24];            //形式９
	char    u9_type[24];            //型名９
	char    u9_date_1[24];          //日付９－１
	char    u9_date_2[24];          //日付９－２
	char    u9_date_3[24];          //日付９－３
	char    u9_introduce[24];       //導入業者９
	char    u9_intro_tel[32];       //連絡先９
	char    u9_checker[24];         //点検業者９
	char    u9_chk_tel[32];         //連絡先９
	char    u9_note_1[32];          //備考９－１
	char    u9_note_2[32];          //備考９－２
	char    u9_note_3[32];          //備考９－３
	char    u9_note_4[32];          //備考９－４
	//事務所内装置１０
	char    u10_unit[24];           //事務所内装置１０
	char    u10_form[24];           //形式１０
	char    u10_type[24];           //型名１０
	char    u10_date_1[24];         //日付１０－１
	char    u10_date_2[24];         //日付１０－２
	char    u10_date_3[24];         //日付１０－３
	char    u10_introduce[24];      //導入業者１０
	char    u10_intro_tel[32];      //連絡先１０
	char    u10_checker[24];        //点検業者１０
	char    u10_chk_tel[32];        //連絡先１０
	char    u10_note_1[32];         //備考１０－１
	char    u10_note_2[32];         //備考１０－２
	char    u10_note_3[32];         //備考１０－３
	char    u10_note_4[32];         //備考１０－４
	//記事１
	char    n1_notice_date[32];     //記事１年月日
	char    n1_contents[64];        //記事１内容
	char    n1_trader[24];          //記事１対応業者
	char    n1_note[32];            //記事１備考
	char    n1_reserve1[32];        //予備
	char    n1_reserve2[32];        //予備
	//記事２
	char    n2_notice_date[32];     //記事２年月日
	char    n2_contents[64];        //記事２内容
	char    n2_trader[24];          //記事２対応業者
	char    n2_note[32];            //記事２備考
	char    n2_reserve1[32];        //予備
	char    n2_reserve2[32];        //予備
	//記事３
	char    n3_notice_date[32];     //記事３年月日
	char    n3_contents[64];        //記事３内容
	char    n3_trader[24];          //記事３対応業者
	char    n3_note[32];            //記事３備考
	char    n3_reserve1[32];        //予備
	char    n3_reserve2[32];        //予備
	//記事４
	char    n4_notice_date[32];     //記事４年月日
	char    n4_contents[64];        //記事４内容
	char    n4_trader[24];          //記事４対応業者
	char    n4_note[32];            //記事４備考
	char    n4_reserve1[32];        //予備
	char    n4_reserve2[32];        //予備
	//記事５
	char    n5_notice_date[32];     //記事５年月日
	char    n5_contents[64];        //記事５内容
	char    n5_trader[24];          //記事５対応業者
	char    n5_note[32];            //記事５備考
	char    n5_reserve1[32];        //予備
	char    n5_reserve2[32];        //予備
	//記事６
	char    n6_notice_date[32];     //記事６年月日
	char    n6_contents[64];        //記事６内容
	char    n6_trader[24];          //記事６対応業者
	char    n6_note[32];            //記事６備考
	char    n6_reserve1[32];        //予備
	char    n6_reserve2[32];        //予備
	//記事７
	char    n7_notice_date[32];     //記事７年月日
	char    n7_contents[64];        //記事７内容
	char    n7_trader[24];          //記事７対応業者
	char    n7_note[32];            //記事７備考
	char    n7_reserve1[32];        //予備
	char    n7_reserve2[32];        //予備
	//記事８
	char    n8_notice_date[32];     //記事８年月日
	char    n8_contents[64];        //記事８内容
	char    n8_trader[24];          //記事８対応業者
	char    n8_note[32];            //記事８備考
	char    n8_reserve1[32];        //予備
	char    n8_reserve2[32];        //予備
	//記事９
	char    n9_notice_date[32];     //記事９年月日
	char    n9_contents[64];        //記事９内容
	char    n9_trader[24];          //記事９対応業者
	char    n9_note[32];            //記事９備考
	char    n9_reserve1[32];        //予備
	char    n9_reserve2[32];        //予備
	//記事１０
	char    n10_notice_date[32];    //記事１０年月日
	char    n10_contents[64];       //記事１０内容
	char    n10_trader[24];         //記事１０対応業者
	char    n10_note[32];           //記事１０備考
	char    n10_reserve1[32];       //予備
	char    n10_reserve2[32];       //予備
} sys_comp2_t;


/********************************************************************
  構造体：局管理情報(局種別毎)
  項目  ：kind          局種別情報ポインタ
        ：local         局実装情報ポインタ    （先頭局へのポインタ）
        ：action        局稼動情報ポインタ    （先頭局へのポインタ）
        ：name;         局名称情報ポインタ    （先頭局へのポインタ）
        ：opt_name;     局補足名称情報ポインタ（先頭局へのポインタ）
        ：code;         局コード情報ポインタ  （先頭局へのポインタ）
        ：comp1;        局構成情報ポインタ    （先頭局へのポインタ）
        ：comp2;        局構成情報ポインタ    （先頭局へのポインタ）
********************************************************************/
typedef struct tagKindInfo {
	sys_kind_t      *kind;          //局種別情報ポインタ
	sys_local_t     *local;         //局実装情報ポインタ
	sys_action_t    *action;        //局稼動情報ポインタ
	sys_name_t      *name;          //局名称情報ポインタ
	sys_optname_t   *optname;       //局補足名称情報ポインタ
	sys_code_t      *code;          //局コード情報ポインタ
//	sys_comp1_t     *comp1;         //局構成情報ポインタ
//	sys_comp2_t     *comp2;         //局構成情報ポインタ
} kind_info_t;


/********************************************************************
  構造体：局管理情報
  項目  ：max           最大局種別番号
        ：pKind         局種別毎の情報
        ：tim           読み込み日時
        ：hLock         ロック用ハンドル
        ：p             内部管理情報
********************************************************************/
typedef struct tagKindManager {
	int         max;                //最大局種別番号
	kind_info_t *pKind;             //局種別毎の情報
	//内部管理用
	time_t      tim;                //読み込み日時
	HANDLE      hLock;              //ロック用ハンドル
	sharedMem_t p;                  //内部管理情報
} kind_manager_t;


/********************************************************************
  構造体：ラグタイム情報
  項目  ：st            開始
        ：et            終了
********************************************************************/
typedef struct tagSysRagTime {
	long st;                        //開始
	long et;                        //終了
} sys_ragtime_t;


/********************************************************************
  構造体：装置間入出力情報ヘッダ部
  項目  ：officeNo      事務所番号
        ：unitNo        装置番号
        ：houroNo       方路番号
        ：myOfficeName  自事務所名
        ：myUnitName    自装置名
        ：toOfficeName  送信先事務所名
        ：toUnitName    送信先装置名
        ：r_port        受信ポート
        ：s_port        送信先ポート
        ：s_ip          送信先ＩＰ
        ：rag_1m        １分値ラグタイム
        ：rag_hm        正定時ラグタイム
        ：rag_dy        日集計ラグタイム
        ：ioflg_1m      １分値出力有無(0=なし,1=出力)
        ：ioflg_hm      正定時出力方法(0=系,  1=一括)
        ：ioflg_dy      日集計出力有無(0=なし,1=出力)
********************************************************************/
typedef struct tagSysExioHeader {
	int  officeNo;                  //事務所番号
	int  unitNo;                    //装置番号
	int  houroNo;                   //方路番号
	char myOfficeName[256];         //自事務所名
	char myUnitName[256];           //自装置名
	char toOfficeName[256];         //送信先事務所名
	char toUnitName[256];           //送信先装置名
	long r_port;                    //受信ポート
	long s_port;                    //送信先ポート
	char s_ip[16];                  //送信先ＩＰ
	sys_ragtime_t rag_1m;           //１分値ラグタイム
	sys_ragtime_t rag_hm;           //正定時ラグタイム
	sys_ragtime_t rag_dy;           //日集計ラグタイム
	int  ioflg_1m;                  //１分値出力有無
	int  ioflg_hm;                  //正定時出力方法
	int  ioflg_dy;                  //日集計出力有無
} sys_exiohead_t;


/********************************************************************
  構造体：装置間入出力情報
  項目  ：io_kind       入出力種別コード    (1=入力,2=出力,以外=設定無し)
        ：io_sno        入出力局番          (0～65535  ,以外=設定無し)
        ：io_ino        入出力項番          (0～65535  ,以外=設定無し)
        ：io_office     入出力管理事務所番号(1～99999  ,以外=設定無し)
        ：io_data       入出力データ種別番号(1～999    ,以外=設定無し)
        ：code          倍長コード          (1=倍長    ,以外=単調)
        ：filetype      ファイル種別        (1=日集計値,以外=時間値)
        ：kno           局種別コード        (1～6      ,以外=設定無し)
        ：sno           内部局番号          (1～999    ,以外=設定無し)
        ：ino           データ項目順番      (1～99     ,以外=設定無し)
        ：scale         スケール補正コード  (1=10倍,2=100倍,3=1000倍,4=1/10倍,5=1/100倍,6=1/1000,以外=1倍)
        ：reserve1      予備
        ：reserve2      予備
********************************************************************/
#define SYS_EXIO_KIND_IN     1      //入力
#define SYS_EXIO_KIND_OUT    2      //出力
#define SYS_EXIO_CODE_DOUBLE 1      //倍長コード
#define SYS_EXIO_TYPE_DAILY  1      //日集計値
#define SYS_EXIO_SCALE_10    1      //10倍
#define SYS_EXIO_SCALE_100   2      //100倍
#define SYS_EXIO_SCALE_1000  3      //1000倍
#define SYS_EXIO_SCALE_D10   4      //1/10倍
#define SYS_EXIO_SCALE_D100  5      //1/100倍
#define SYS_EXIO_SCALE_D1000 6      //1/1000倍
typedef struct tagSysExio {
	int io_kind;                    //入出力種別コード
	int io_sno;                     //入出力局番
	int io_ino;                     //入出力項番
	int io_office;                  //入出力管理事務所番号
	int io_data;                    //入出力データ種別番号
	int code;                       //倍長コード
	int filetype;                   //ファイル種別
	int kno;                        //局種別コード
	int sno;                        //内部局番号
	int ino;                        //データ項目順番
	int scale;                      //スケール補正コード
	int reserve1;                   //予備
	int reserve2;                   //予備
} sys_exio_t;


/********************************************************************
  構造体：装置間入出力情報
  項目  ：cnt           データ件数
        ：hd            装置間入出力ヘッダ部
        ：dp            装置間入出力データ部
********************************************************************/
typedef struct tagSysExioInfo {
	int            cnt;             //データ件数
	sys_exiohead_t hd;              //ヘッダ部
	sys_exio_t     *dp;             //データ部
} sys_exioinfo_t;


/********************************************************************
  構造体：雨量演算方式
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
#define SYS_RA_ENZAN_MODE_SA    1       //砂防系入力値
#define SYS_RA_ENZAN_MODE_SATM  2       //砂防降雨開始終了時刻のみ演算 2002.10.29

#define SYS_RA_ENZAN_KIND_RO    1       //累加(連続)雨量
#define SYS_RA_ENZAN_KIND_R10   2       //10分雨量
#define SYS_RA_ENZAN_KIND_RH    3       //１時間雨量
#define SYS_RA_ENZAN_RTIME 	    12      //累加雨量リセット時間デフォルト値
#define SYS_RA_ENZAN_DTIME      3       //連続雨量(道路)リセット時間デフォルト値
#define SYS_RA_ENZAN_STIME      24      //降雨終了判定時間デフォルト値
#define SYS_RA_ENZAN_HALF       24      //雨量半減期デフォルト値
#define SYS_RA_ENZAN_HJUDG      4       //変曲点判定雨量デフォルト値
#define SYS_RA_ENZAN_YTIME      3       //変曲点終了判定時間デフォルト値
typedef struct tagSysRainEnzan {
	short   sno;            //内部局番
	char    mode;           //雨量演算方式 (1:砂防系入力値, 以外:積算雨量)
	char    kind;           //受信雨量種別 (1:累加(連続)雨量, 2:10分雨量,
                            //              3:１時間雨量, 以外:カウンタ値)
	short   rtime;          //累加雨量リセット時間       (1～99, 以外:12)
	short   dtime;          //連続雨量(道路)リセット時間 (1～99, 以外:3)
	short   stime;          //降雨終了判定時間           (1～99, 以外:24)
	short   half;           //雨量半減期                 (1～99, 以外:24)
	short   hjudg;          //変曲点判定雨量             (1～99, 以外:4)
	short   ytime;          //変曲点終了判定時間         (1～99, 以外:3)
} sys_ra_enzan_t;


/********************************************************************
  構造体：雨量異常値検定
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
#define SYS_RA_KENTEI_S01   10          //１分積算雨量上限値デフォルト値
#define SYS_RA_KENTEI_S10   50          //10分積算雨量上限値デフォルト値
#define SYS_RA_KENTEI_S60   200         //60分積算雨量上限値デフォルト値
#define SYS_RA_KENTEI_R10   50          //10分雨量上限値デフォルト値
#define SYS_RA_KENTEI_R30   150         //30分雨量上限値デフォルト値
#define SYS_RA_KENTEI_R60   200         //60分雨量上限値デフォルト値
#define SYS_RA_KENTEI_R3H   600         //３時間雨量上限値デフォルト値
#define SYS_RA_KENTEI_R6H   990         //６時間雨量上限値デフォルト値
#define SYS_RA_KENTEI_R24H  990         //24時間雨量上限値デフォルト値
#define SYS_RA_KENTEI_R48H  2000        //48時間雨量上限値デフォルト値
#define SYS_RA_KENTEI_RA    2000        //累加雨量上限値デフォルト値
#define SYS_RA_KENTEI_DA    2000        //連続(道路)雨量上限値デフォルト値
#define SYS_RA_KENTEI_SA    2000        //連続(砂防)雨量上限値デフォルト値
#define SYS_RA_KENTEI_YK    2000        //有効雨量上限値デフォルト値
#define SYS_RA_KENTEI_YKP   200         //有効雨量強度上限値デフォルト値
#define SYS_RA_KENTEI_ZAN   2000        //残留雨量上限値デフォルト値
#define SYS_RA_KENTEI_JKZ   2000        //前期実効雨量上限値デフォルト値
#define SYS_RA_KENTEI_JK    2000        //実効雨量上限値デフォルト値
#define SYS_RA_KENTEI_JK15  2000        //1.5H実効雨量上限値デフォルト値
#define SYS_RA_KENTEI_JK72  2000        //72H実効雨量上限値デフォルト値
typedef struct tagSysRainKentei {
	short   sno;            //内部局番
	short   s01;            //１分積算雨量上限値      (1～2000, 以外:10)
	short   s10;            //10分積算雨量上限値      (1～2000, 以外:50)
	short   s60;            //60分積算雨量上限値      (1～2000, 以外:200)
	short   r10;            //10分雨量上限値          (1～2000, 以外:50)
	short   r30;            //30分雨量上限値          (1～2000, 以外:150)
	short   r60;            //60分雨量上限値          (1～2000, 以外:200)
	short   r3h;            //３時間雨量上限値        (1～2000, 以外:600)
	short   r6h;            //６時間雨量上限値        (1～2000, 以外:990)
	short   r24h;           //24時間雨量上限値        (1～2000, 以外:990)
	short   r48h;           //48時間雨量上限値        (1～2000, 以外:2000)
	short   ra;             //累加雨量上限値          (1～2000, 以外:2000)
	short   da;             //連続(道路)雨量上限値    (1～2000, 以外:2000)
	short   sa;             //連続(砂防)雨量上限値    (1～2000, 以外:2000)
	short   yk;             //有効雨量上限値          (1～2000, 以外:2000)
	short   ykp;            //有効雨量強度上限値      (1～2000, 以外:200)
	short   zan;            //残留雨量上限値          (1～2000, 以外:2000)
	short   jkz;            //前期実効雨量上限値      (1～2000, 以外:2000)
	short   jk;             //実効雨量上限値          (1～2000, 以外:2000)
	short   jk15;           //1.5H実効雨量上限値      (1～2000, 以外:2000)
	short   jk72;           //72H実効雨量上限値       (1～2000, 以外:2000)
} sys_ra_kentei_t;


/********************************************************************
  構造体：雨量警報基準値
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
#define SYS_RA_ALM_NONE      9999        //警報判定なし
typedef struct tagSysRainAlm {
	short   sno;            //内部局番
	short   r10_alm1;       //10分雨量注意値       (1～2000, 以外:判定値なし)
	short   r10_alm2;       //10分雨量警戒値       (1～2000, 以外:判定値なし)
	short   r30_alm1;       //30分雨量注意値       (1～2000, 以外:判定値なし)
	short   r30_alm2;       //30分雨量警戒値       (1～2000, 以外:判定値なし)
	short   r60_alm1;       //60分雨量注意値       (1～2000, 以外:判定値なし)
	short   r60_alm2;       //60分雨量警戒値       (1～2000, 以外:判定値なし)
	short   r3h_alm1;       //３時間雨量注意値     (1～2000, 以外:判定値なし)
	short   r3h_alm2;       //３時間雨量警戒値     (1～2000, 以外:判定値なし)
	short   r6h_alm1;       //６時間雨量注意値     (1～2000, 以外:判定値なし)
	short   r6h_alm2;       //６時間雨量警戒値     (1～2000, 以外:判定値なし)
	short   r24h_alm1;      //24時間雨量注意値     (1～2000, 以外:判定値なし)
	short   r24h_alm2;      //24時間雨量警戒値     (1～2000, 以外:判定値なし)
	short   r48h_alm1;      //48時間雨量注意値     (1～2000, 以外:判定値なし)
	short   r48h_alm2;      //48時間雨量警戒値     (1～2000, 以外:判定値なし)
	short   ra_alm1;        //累加雨量注意値       (1～2000, 以外:判定値なし)
	short   ra_alm2;        //累加雨量警戒値       (1～2000, 以外:判定値なし)
	short   da_alm1;        //連続(道路)雨量注意値 (1～2000, 以外:判定値なし)
	short   da_alm2;        //連続(道路)雨量警戒値 (1～2000, 以外:判定値なし)
	short   sa_alm1;        //連続(砂防)雨量注意値 (1～2000, 以外:判定値なし)
	short   sa_alm2;        //連続(砂防)雨量警戒値 (1～2000, 以外:判定値なし)
} sys_ra_alm_t;


/********************************************************************
  構造体：砂防雨量判定定数
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
#define SYS_RA_SNAKE_NONE    0        //基準雨量デフォルト値
typedef struct tagSysRainSnake {
	short   sno;            //内部局番
	short   mode;           //砂防雨量判定方式       (1～30, 以外:判定なし)
	short   br_a;           //基準雨量ａ             (1～2000, 以外:設定値なし)
	short   br_b;           //基準雨量ｂ             (1～2000, 以外:設定値なし)
	short   br_c;           //基準雨量ｃ             (1～2000, 以外:設定値なし)
	short   br_d;           //基準雨量ｄ             (1～2000, 以外:設定値なし)
	short   br_a1;          //基準雨量ａ１           (1～2000, 以外:設定値なし)
	short   br_b1;          //基準雨量ｂ１           (1～2000, 以外:設定値なし)
	short   br_a2;          //基準雨量ａ２           (1～2000, 以外:設定値なし)
	short   br_b2;          //基準雨量ｂ２           (1～2000, 以外:設定値なし)
	short   br_a3;          //基準雨量ａ３           (1～2000, 以外:設定値なし)
	short   br_b3;          //基準雨量ｂ３           (1～2000, 以外:設定値なし)
	short   br_a4;          //基準雨量ａ４           (1～2000, 以外:設定値なし)
	short   br_b4;          //基準雨量ｂ４           (1～2000, 以外:設定値なし)
	short   br_e1;          //既往最大２時間雨量ｅ１ (1～2000, 以外:設定値なし)
	short   br_e2;          //既往最大１時間雨量ｅ２ (1～2000, 以外:設定値なし)
	short   br_f;           //基準雨量ｆ             (1～2000, 以外:設定値なし)
	short   br_g;           //基準雨量ｇ             (1～2000, 以外:設定値なし)
	short   br_h;           //基準雨量ｈ             (1～2000, 以外:設定値なし)
	short   br_i;           //基準雨量ｉ             (1～2000, 以外:設定値なし)
	short   br_f1;          //基準雨量ｆ１           (1～2000, 以外:設定値なし)
	short   br_g1;          //基準雨量ｇ１           (1～2000, 以外:設定値なし)
	short   br_f2;          //基準雨量ｆ２           (1～2000, 以外:設定値なし)
	short   br_g2;          //基準雨量ｇ２           (1～2000, 以外:設定値なし)
} sys_ra_snake_t;


/********************************************************************
  構造体：雨量既往集計値
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
typedef struct tagSysRainOld {
	short   sno;            //内部局番
} sys_ra_old_t;


/********************************************************************
  構造体：雨量システム定数
  項目  ：enzan         雨量演算方式ポインタ（先頭局へのポインタ）
        ：kentei        雨量異常値検定ポインタ（先頭局へのポインタ）
        ：alm           雨量警報基準値ポインタ（先頭局へのポインタ）
        ：snake         砂防雨量判定定数ポインタ（先頭局へのポインタ）
        ：old           雨量既往集計値ポインタ（先頭局へのポインタ）
********************************************************************/
typedef struct tagRainCnst {
	sys_ra_enzan_t      *enzan;     //雨量演算方式ポインタ
	sys_ra_kentei_t     *kentei;    //雨量異常値検定ポインタ
	sys_ra_alm_t        *alm;       //雨量警報基準値ポインタ
	sys_ra_snake_t      *snake;     //砂防雨量判定定数ポインタ
	sys_ra_old_t        *old;       //雨量既往集計値ポインタ
} rain_cnst_t;


/********************************************************************
  構造体：水位演算定数
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
#define SYS_RV_ENZAN_QMODE_NM   1       //受信流量演算
#define SYS_RV_ENZAN_QMODE_HQ   2       //ＨＱ定数演算
#define	SYS_RV_ENZAN_KEIKAKU	1		//計画高判定
typedef struct tagSysRiverEnzan {
	short   sno;            //内部局番
	char    w_mode;         //水位演算方式 (以外:受信水位)
	double  hosei;          //水位補正値   (-999.99～999.99, 以外:設定値なし)
	double  minus;          //マイナス水位 (1～99.99, 以外:設定値なし)
	char    q_mode;         //流量演算方式 (1:受信流量, 2:ＨＱ定数, 以外:非実装)
	char	alm4_jude;		//計画高判定（1:判定あり,以外:判定なし）
	char	yobi;
} sys_rv_enzan_t;


/********************************************************************
  構造体：水位異常値検定
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
typedef struct tagSysRiverKentei {
	short   sno;            //内部局番
	double  w_upp;          //水位上限値          (-9.99～4000.00, 以外:判定値なし)
	double  w_low;          //水位下限値          (-9.99～4000.00, 以外:判定値なし)
	short   w_fixtime;      //水位一定値判定時間  (1～99, 以外:判定値なし)
	double  w_fix;          //水位一定値判定値    (-9.99～4000.00, 以外:判定値なし)
	double  q_upp;          //流量上限値          (0～99999.99, 以外:判定値なし)
	double  q_low;          //流量下限値          (0～99999.99, 以外:判定値なし)
	short   q_fixtime;      //流量一定値判定時間  (1～99, 以外:判定値なし)
	double  q_fix;          //流量一定値判定値    (0～99999.99, 以外:判定値なし)
	double  w10_max;        //水位10分最大変化量  (0～20.00, 以外:判定値なし)
	double  w30_max;        //水位30分最大変化量  (0～20.00, 以外:判定値なし)
	double  w1h_max;        //水位時間最大変化量  (0～20.00, 以外:判定値なし)
	double  q10_max;        //流量10分最大変化量  (0～999.99, 以外:判定値なし)
	double  q30_max;        //流量30分最大変化量  (0～999.99, 以外:判定値なし)
	double  q1h_max;        //流量時間最大変化量  (0～999.99, 以外:判定値なし)
} sys_rv_kentei_t;


/********************************************************************
  構造体：水位警報基準値
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
typedef struct tagSysRiverAlm {
	short   sno;            //内部局番
	double  w_alm1;         //通報水位   (-9.99～4000.00, 以外:設定値なし)
	double  w_alm2;         //警戒水位   (-9.99～4000.00, 以外:設定値なし)
	double  w_alm3;         //危険水位   (-9.99～4000.00, 以外:設定値なし)
	double  w_alm4;         //計画高水位 (-9.99～4000.00, 以外:設定値なし)
	double  w_regr;         //通常水位   (-9.99～4000.00, 以外:設定値なし)
	double  q_alm1;         //通報流量   (0～999.99, 以外:設定値なし)
	double  q_alm2;         //警戒流量   (0～999.99, 以外:設定値なし)
	double  q_alm3;         //危険流量   (0～999.99, 以外:設定値なし)
	double  q_alm4;         //計画高流量 (0～999.99, 以外:設定値なし)
	double  q_regr;         //通常流量   (0～999.99, 以外:設定値なし)
} sys_rv_alm_t;


/********************************************************************
  構造体：河川断面図
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
typedef struct tagSysRiverDanmen {
	short   sno;            //内部局番
} sys_rv_danmen_t;


/********************************************************************
  構造体：Ｈ－Ｑ定数
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
typedef struct tagSysRiverHQ {
	short   sno;            //内部局番
	double  H0;             //Ｈ０：切替水位       (-9.99～4000.00, 以外:設定値なし)
	double  H1;             //Ｈ１：切替水位       (-9.99～4000.00, 以外:設定値なし)
	double  A1;             //Ａ１：流速補正定数   (+-9999999999, 以外:設定値なし 小数点可変)
	double  B1;             //Ｂ１：断面積補正定数 (+-9999999999, 以外:設定値なし 小数点可変)
	double  H2;             //Ｈ２：切替水位       (-9.99～4000.00, 以外:設定値なし)
	double  A2;             //Ａ２：流速補正定数   (+-9999999999, 以外:設定値なし 小数点可変)
	double  B2;             //Ｂ２：断面積補正定数 (+-9999999999, 以外:設定値なし 小数点可変)
	double  H3;             //Ｈ３：切替水位       (-9.99～4000.00, 以外:設定値なし)
	double  A3;             //Ａ３：流速補正定数   (+-9999999999, 以外:設定値なし 小数点可変)
	double  B3;             //Ｂ３：断面積補正定数 (+-9999999999, 以外:設定値なし 小数点可変)
	double  H4;             //Ｈ４：切替水位       (-9.99～4000.00, 以外:設定値なし)
	double  A4;             //Ａ４：流速補正定数   (+-9999999999, 以外:設定値なし 小数点可変)
	double  B4;             //Ｂ４：断面積補正定数 (+-9999999999, 以外:設定値なし 小数点可変)
	double  H5;             //Ｈ５：切替水位       (-9.99～4000.00, 以外:設定値なし)
	double  A5;             //Ａ５：流速補正定数   (+-9999999999, 以外:設定値なし 小数点可変)
	double  B5;             //Ｂ５：断面積補正定数 (+-9999999999, 以外:設定値なし 小数点可変)
	double  H6;             //Ｈ６：切替水位       (-9.99～4000.00, 以外:設定値なし)
	double  A6;             //Ａ６：流速補正定数   (+-9999999999, 以外:設定値なし 小数点可変)
	double  B6;             //Ｂ６：断面積補正定数 (+-9999999999, 以外:設定値なし 小数点可変)
} sys_rv_hq_t;


/********************************************************************
  構造体：水位既往集計値
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
typedef struct tagSysRiverOld {
	short   sno;            //内部局番
} sys_rv_old_t;


/********************************************************************
  構造体：水位システム定数
  項目  ：enzan         水位演算方式ポインタ（先頭局へのポインタ）
        ：kentei        水位異常値検定ポインタ（先頭局へのポインタ）
        ：keihou        水位警報基準値ポインタ（先頭局へのポインタ）
        ：danmen        河川断面図ポインタ（先頭局へのポインタ）
        ：hq            Ｈ－Ｑ定数ポインタ（先頭局へのポインタ）
        ：old           水位既往集計値ポインタ（先頭局へのポインタ）
********************************************************************/
typedef struct tagRiverCnst {
	sys_rv_enzan_t      *enzan;     //水位演算方式ポインタ
	sys_rv_kentei_t     *kentei;    //水位異常値検定ポインタ
	sys_rv_alm_t        *keihou;    //水位警報基準値ポインタ
	sys_rv_danmen_t     *danmen;    //河川断面図ポインタ
	sys_rv_hq_t         *hq;        //Ｈ－Ｑ定数ポインタ
	sys_rv_old_t        *old;       //水位既往集計値ポインタ
} river_cnst_t;


/********************************************************************
  構造体：ダム諸量演算方式
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
#define SYS_DM_ENZAN_MODE_INPT  1       //入力値
#define SYS_DM_ENZAN_MODE_CALC  2       //演算
typedef struct tagSysDamEnzan {
	short   sno;            //内部局番
	short   chsi_mode;      //貯水位演算方式           (1:入力値, 以外:非実装)
	short   scsr_mode;      //総貯水量演算方式         (1:入力値, 以外:非実装)
	short   zrnr_mode;      //全流入量演算方式         (1:入力値, 以外:非実装)
	short   zhrr_mode;      //全放流量演算方式         (1:入力値, 以外:非実装)
	short   akyr_mode;      //空容量演算方式           (1:入力値, 2:演算値, 以外:非実装)
	short   ycrt_mode;      //有効貯水率演算方式       (1:入力値, 2:演算値, 以外:非実装)
	short   csir_mode;      //調整流量演算方式         (1:入力値, 2:演算値, 以外:非実装)
	short   dmhr_mode;      //ダム放流量演算方式       (1:入力値, 以外:非実装)
	short   gthr_mode;      //ゲート放流量演算方式     (1:入力値, 以外:非実装)
	short   vlhr_mode;      //バルブ放流量演算方式     (1:入力値, 以外:非実装)
	short   rshr_mode;      //利水放流量演算方式       (1:入力値, 以外:非実装)
	short   hdss_mode;      //発電使用水量演算方式     (1:入力値, 以外:非実装)
	short   yosr_mode;      //揚水量演算方式           (1:入力値, 以外:非実装)
	short   hokr_mode;      //補給量演算方式           (1:入力値, 以外:非実装)
	short   akis_mode;      //空水位演算方式           (1:入力値, 2:演算値, 以外:非実装)
	short   ycsr_mode;      //有効貯水量演算方式       (1:入力値, 2:演算値, 以外:非実装)
	short   totl_mode;      //ダム諸量日集計値演算方式 (1:入力値, 2:演算値, 以外:非実装)
} sys_dm_enzan_t;


/********************************************************************
  構造体：ダム諸量異常値検定
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
typedef struct tagSysDamKentei {
	short   sno;            //内部局番
} sys_dm_kentei_t;


/********************************************************************
  構造体：ダム基準値
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
typedef struct tagSysDamAlm {
	short   sno;            //内部局番
	double  skei_kozi;      //設計洪水位                 (0～4000.00, 以外:設定値なし)
	double  surcharge;      //サーチャージ水位           (0～4000.00, 以外:設定値なし)
	double  tadasigaki;     //ただし書き水位             (0～4000.00, 以外:設定値なし)
	double  joji_mansui;    //常時満水位                 (0～4000.00, 以外:設定値なし)
	double  kozi_mansui;    //洪水時満水位               (0～4000.00, 以外:設定値なし)
	double  saitei_w;       //最低水位                   (0～4000.00, 以外:設定値なし)
	double  shi_w;          //死水位                     (0～4000.00, 以外:設定値なし)
	double  skei_kozi_q;    //設計洪水流量               (0～99999.999, 以外:設定値なし)
	double  kei_taka_q;     //計画高水流量               (0～99999.999, 以外:設定値なし)
	double  kei_maxho;      //計画最大放流量             (0～99999.999, 以外:設定値なし)
	double  kihon_taka_q;   //基本高水流量               (0～99999.999, 以外:設定値なし)
	double  kozi_chset_q;   //洪水調節開始流量           (0～99999.999, 以外:設定値なし)
	double  kouzuiryo;      //洪水時流入量(洪水量)       (0～99999.999, 以外:設定値なし)
	double  horyu_kijun;    //放流量通報基準値           (0～99999.999, 以外:設定値なし)
	double  taisha_yoryo;   //たい砂容量                 (0～99999.999, 以外:設定値なし)
	double  srch_yuukou;    //サーチャージ有効貯水容量   (0～99999.999, 以外:設定値なし)
	double  jouj_yuukou;    //常時満水位有効貯水容量     (0～99999.999, 以外:設定値なし)
	double  seigen1;        //制限水位１                 (0～4000.00, 以外:設定値なし)
	double  yuukou1;        //有効貯水容量１             (0～99999.999, 以外:設定値なし)
	short   seigen_st_m1;   //制限水位１期間開始(月)     (1～12, 以外:設定値なし)
	short   seigen_st_d1;   //制限水位１期間開始(日)     (1～31, 以外:設定値なし)
	short   seigen_ed_m1;   //制限水位１期間終了(月)     (1～12, 以外:設定値なし)
	short   seigen_ed_d1;   //制限水位１期間終了(日)     (1～31, 以外:設定値なし)
	double  seigen2;        //制限水位２                 (0～4000.00, 以外:設定値なし)
	double  yuukou2;        //有効貯水容量２             (0～99999.999, 以外:設定値なし)
	short   seigen_st_m2;   //制限水位２期間開始(月)     (1～12, 以外:設定値なし)
	short   seigen_st_d2;   //制限水位２期間開始(日)     (1～31, 以外:設定値なし)
	short   seigen_ed_m2;   //制限水位２期間終了(月)     (1～12, 以外:設定値なし)
	short   seigen_ed_d2;   //制限水位２期間終了(日)     (1～31, 以外:設定値なし)
	double  seigen3;        //制限水位３                 (0～4000.00, 以外:設定値なし)
	double  yuukou3;        //有効貯水容量３             (0～99999.999, 以外:設定値なし)
	short   seigen_st_m3;   //制限水位３期間開始(月)     (1～12, 以外:設定値なし)
	short   seigen_st_d3;   //制限水位３期間開始(日)     (1～31, 以外:設定値なし)
	short   seigen_ed_m3;   //制限水位３期間終了(月)     (1～12, 以外:設定値なし)
	short   seigen_ed_d3;   //制限水位３期間終了(日)     (1～31, 以外:設定値なし)
	double  seigen4;        //制限水位４                 (0～4000.00, 以外:設定値なし)
	double  yuukou4;        //有効貯水容量４             (0～99999.999, 以外:設定値なし)
	short   seigen_st_m4;   //制限水位４期間開始(月)     (1～12, 以外:設定値なし)
	short   seigen_st_d4;   //制限水位４期間開始(日)     (1～31, 以外:設定値なし)
	short   seigen_ed_m4;   //制限水位４期間終了(月)     (1～12, 以外:設定値なし)
	short   seigen_ed_d4;   //制限水位４期間終了(日)     (1～31, 以外:設定値なし)
	double  yobihoryu1;     //予備放流水位１             (0～4000.00, 以外:設定値なし)
	short   yobi_st_m1;     //予備放流水位１期間開始(月) (1～12, 以外:設定値なし)
	short   yobi_st_d1;     //予備放流水位１期間開始(日) (1～31, 以外:設定値なし)
	short   yobi_ed_m1;     //予備放流水位１期間終了(月) (1～12, 以外:設定値なし)
	short   yobi_ed_d1;     //予備放流水位１期間終了(日) (1～31, 以外:設定値なし)
	double  yobihoryu2;     //予備放流水位２             (0～4000.00, 以外:設定値なし)
	short   yobi_st_m2;     //予備放流水位２期間開始(月) (1～12, 以外:設定値なし)
	short   yobi_st_d2;     //予備放流水位２期間開始(日) (1～31, 以外:設定値なし)
	short   yobi_ed_m2;     //予備放流水位２期間終了(月) (1～12, 以外:設定値なし)
	short   yobi_ed_d2;     //予備放流水位２期間終了(日) (1～31, 以外:設定値なし)
	double  yobihoryu3;     //予備放流水位３             (0～4000.00, 以外:設定値なし)
	short   yobi_st_m3;     //予備放流水位３期間開始(月) (1～12, 以外:設定値なし)
	short   yobi_st_d3;     //予備放流水位３期間開始(日) (1～31, 以外:設定値なし)
	short   yobi_ed_m3;     //予備放流水位３期間終了(月) (1～12, 以外:設定値なし)
	short   yobi_ed_d3;     //予備放流水位３期間終了(日) (1～31, 以外:設定値なし)
	double  yobihoryu4;     //予備放流水位４             (0～4000.00, 以外:設定値なし)
	short   yobi_st_m4;     //予備放流水位４期間開始(月) (1～12, 以外:設定値なし)
	short   yobi_st_d4;     //予備放流水位４期間開始(日) (1～31, 以外:設定値なし)
	short   yobi_ed_m4;     //予備放流水位４期間終了(月) (1～12, 以外:設定値なし)
	short   yobi_ed_d4;     //予備放流水位４期間終了(日) (1～31, 以外:設定値なし)
} sys_dm_alm_t;


/********************************************************************
  構造体：ダム諸元
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
typedef struct tagSysDamShogen {
	short   sno;            //内部局番
} sys_dm_shogen_t;


/********************************************************************
  構造体：ダム諸量システム定数
  項目  ：enzan         ダム諸量演算方式ポインタ（先頭局へのポインタ）
        ：kentei        ダム諸量異常値検定ポインタ（先頭局へのポインタ）
        ：alm           ダム基準値ポインタ（先頭局へのポインタ）
        ：shogen        ダム諸元値ポインタ（先頭局へのポインタ）
********************************************************************/
typedef struct tagDamCnst {
	sys_dm_enzan_t      *enzan;     //ダム諸量演算方式ポインタ
	sys_dm_kentei_t     *kentei;    //ダム諸量異常値検定ポインタ
	sys_dm_alm_t        *alm;       //ダム基準値ポインタ
	sys_dm_shogen_t     *shogen;    //ダム諸元ポインタ
} dam_cnst_t;


/********************************************************************
  構造体：潮位演算方式
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
#define SYS_TD_ENZAN_MODE_INPT  1       //入力値
#define SYS_TD_ENZAN_MODE_CNST  2       //定数
#define SYS_TD_ENZAN_MODE_SIOM  3       //汐見表
typedef struct tagSysTideEnzan {
	short   sno;            //内部局番
	char    js_mode;        //実測潮位演算方式 (以外:入力値)
	char    tm_mode;        //天文潮位演算方式 (1:入力値, 2:定数, 3:汐見表, 以外:非実装)
} sys_td_enzan_t;


/********************************************************************
  構造体：潮位異常値検定
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
typedef struct tagSysTideKentei {
	short   sno;            //内部局番
} sys_td_kentei_t;


/********************************************************************
  構造体：潮位警報基準値
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
typedef struct tagSysTideAlm {
	short   sno;            //内部局番
	double  td_alm1;        //通報潮位     (-9.99～99.99, 以外:設定値なし)
	double  td_alm2;        //警戒潮位     (-9.99～99.99, 以外:設定値なし)
	double  td_alm3;        //危険潮位     (-9.99～99.99, 以外:設定値なし)
	double  td_alm1_d;      //通報潮位偏差 (0～99.99, 以外:設定値なし)
	double  td_alm2_d;      //警戒潮位偏差 (0～99.99, 以外:設定値なし)
	double  td_alm3_d;      //危険潮位偏差 (0～99.99, 以外:設定値なし)
} sys_td_alm_t;


/********************************************************************
  構造体：天文潮位演算定数
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
typedef struct tagSysTideTenmon {
	short   sno;            //内部局番
} sys_td_tenmon_t;


/********************************************************************
  構造体：汐見表
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
typedef struct tagSysTideSiomi {
	short   sno;            //内部局番
} sys_td_siomi_t;


/********************************************************************
  構造体：潮位システム定数
  項目  ：enzan         潮位演算方式ポインタ（先頭局へのポインタ）
        ：kentei        潮位異常値検定ポインタ（先頭局へのポインタ）
        ：alm           潮位警報基準値ポインタ（先頭局へのポインタ）
        ：tenmon        天文潮位演算定数ポインタ（先頭局へのポインタ）
        ：siomi         汐見表ポインタ（先頭局へのポインタ）
********************************************************************/
typedef struct tagTideCnst {
	sys_td_enzan_t      *enzan;     //潮位演算方式ポインタ
	sys_td_kentei_t     *kentei;    //潮位異常値検定ポインタ
	sys_td_alm_t        *alm;       //潮位警報基準値ポインタ
	sys_td_tenmon_t     *tenmon;    //天文潮位演算定数ポインタ
	sys_td_siomi_t      *siomi;     //汐見表ポインタ
} tide_cnst_t;


/********************************************************************
  構造体：風向／風速演算方式
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
#define SYS_WD_ENZAN_MODE_INPT  1       //入力値
#define SYS_WD_ENZAN_MODE_AD    2       //ＡＤ変換 [2003.07.02]
typedef struct tagSysWindEnzan {
	short   sno;            //内部局番
	char    avwv_mode;      //平均風速演算方式     (1:入力値, 以外:非実装)
	char    mxwv_mode;      //瞬間最大風速演算方式 (1:入力値, 以外:非実装)
	char    dw16_mode;      //16方位風向演算方式   (1:入力値, 以外:非実装)
	char    mxdw_mode;      //瞬間最大風向演算方式 (1:入力値, 以外:非実装)
} sys_wd_enzan_t;


/********************************************************************
  構造体：風速異常値検定
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
typedef struct tagSysWindKentei {
	short   sno;            //内部局番
} sys_wd_kentei_t;


/********************************************************************
  構造体：風速警報基準値
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
typedef struct tagSysWindAlm {
	short   sno;            //内部局番
	double  av_alm1;        //平均風速注意値     (1～60.0, 以外:判定なし)
	double  av_alm2;        //平均風速警戒値     (1～60.0, 以外:判定なし)
	double  mx_alm1;        //瞬間最大風速注意値 (1～60.0, 以外:判定なし)
	double  mx_alm2;        //瞬間最大風速警戒値 (1～60.0, 以外:判定なし)
} sys_wd_alm_t;


/********************************************************************
  構造体：風向風速システム定数
  項目  ：enzan         風向／風速演算方式ポインタ（先頭局へのポインタ）
        ：kentei        風速異常値検定ポインタ（先頭局へのポインタ）
        ：alm           風速警報基準値ポインタ（先頭局へのポインタ）
********************************************************************/
typedef struct tagWindCnst {
	sys_wd_enzan_t      *enzan;     //風向／風速演算方式ポインタ
	sys_wd_kentei_t     *kentei;    //風速異常値検定ポインタ
	sys_wd_alm_t        *alm;       //風速警報基準値ポインタ
} wind_cnst_t;


/********************************************************************
  構造体：中継状態演算方式
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
typedef struct tagSysRelayEnzan {
	short   sno;            //内部局番
	char    mode;           //中継局状態演算方式
} sys_rl_enzan_t;


/********************************************************************
  構造体：中継情報システム定数
  項目  ：enzan         中継状態演算方式ポインタ（先頭局へのポインタ）
********************************************************************/
typedef struct tagRelayCnst {
	sys_rl_enzan_t      *enzan;     //中継状態演算方式ポインタ
} relay_cnst_t;


/********************************************************************
  構造体：積雪演算方式 [2003.06.04]
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
#define SYS_SN_ENZAN_MODE_INPT  1        //実測値
#define SYS_SN_ENZAN_MODE_AD1   2        //Ａ／Ｄ変換１
#define SYS_SN_ENZAN_MODE_CALC  1        //演算あり
#define SYS_SN_ENZAN_MODE_NONE  9        //演算無し

#define SYS_SN_ENZAN_S_MON      10       //累計降雪量演算開始日付(月)デフォルト値
#define SYS_SN_ENZAN_S_DAY      31       //累計降雪量演算開始日付(日)デフォルト値
#define SYS_SN_ENZAN_S_HOUR     24       //累計降雪量演算開始日付(時)デフォルト値
#define SYS_SN_ENZAN_E_MON      4        //累計降雪量演算終了日付(月)デフォルト値
#define SYS_SN_ENZAN_E_DAY      30       //累計降雪量演算終了日付(日)デフォルト値
#define SYS_SN_ENZAN_E_HOUR     24       //累計降雪量演算終了日付(時)デフォルト値

typedef struct tagSysSnowEnzan {
	short   sno;            //内部局番
	char    ss_mode;        //積雪深演算方式 (1:実測値, 2:Ａ／Ｄ変換１, 以外:実測値)
	short   hosei;          //積雪深補正値  (0～9999, 以外:0)
	char    ks_mode;        //降雪量演算方式 (1:演算あり 9:演算無し, 以外:演算あり)
	short   s_mon;          //累計降雪量演算開始日付(月) (10,11,12, 以外:10)
	short   s_day;          //累計降雪量演算開始日付(日) (1～31,    以外:31)
	short   s_hour;         //累計降雪量演算開始日付(時) (1～24,    以外:24)
	short   e_mon;          //累計降雪量演算終了日付(月) (3,4,5,    以外: 4)
	short   e_day;          //累計降雪量演算終了日付(日) (1～31,    以外:30)
	short   e_hour;         //累計降雪量演算終了日付(時) (1～24,    以外:24)
} sys_sn_enzan_t;


/********************************************************************
  構造体：積雪異常値検定 [2003.06.04]
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
#define SYS_SN_KENTEI_SS_UPP    500      //１積雪深上限値デフォルト値
#define SYS_SN_KENTEI_KS1H_UPP  100      //時間降雪量上限値デフォルト値
#define SYS_SN_KENTEI_KS_UPP    300      //降雪量上限値デフォルト値
typedef struct tagSysSnowKentei {
	short   sno;            //内部局番
	short   ss_upp;         //積雪深量上限値       (1～9999, 以外:500)
	short   ks1h_upp;       //時間降雪量上限値     (1～9999, 以外:100)
	short   ks_upp;         //降雪量上限値         (1～9999, 以外:200)
	short   ss_max;         //積雪深最大変化量     (1～9999, 以外:設定値無し)
	short   ks1h_max;       //時間降雪量最大変化量 (1～9999, 以外:設定値無し)
} sys_sn_kentei_t;


/********************************************************************
  構造体：積雪警報基準値 [2003.06.04]
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
typedef struct tagSysSnowAlm {
	short   sno;            //内部局番
	short   ss_alm1;        //積雪深注意値         (1～9999, 以外:判定値なし)
	short   ss_alm2;        //積雪深警戒値         (1～9999, 以外:判定値なし)
	short   ks1h_alm1;      //時間降雪量注意値     (1～9999, 以外:判定値なし)
	short   ks1h_alm2;      //時間降雪量警戒値     (1～9999, 以外:判定値なし)
} sys_sn_alm_t;


/********************************************************************
  構造体：積雪複合局設定 [2003.06.04]
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
typedef struct tagSysSnowPair {
	short   sno;            //内部局番
	short   temp_st;        //気温対応局           (1～999, 以外:設定値なし)
	short   road_st;        //路面対応局           (1～999, 以外:設定値なし)
} sys_sn_pair_t;


/********************************************************************
  構造体：気温演算方式 [2003.06.04]
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
#define SYS_TP_ENZAN_MODE_INPT  1       //実測値
#define SYS_TP_ENZAN_MODE_AD1   2       //Ａ／Ｄ変換１
typedef struct tagSysTempEnzan {
	short   sno;            //内部局番
	char    mode;           //気温演算方式 (1:実測値, 2:Ａ／Ｄ変換１ 以外:実測値)
} sys_tp_enzan_t;


/********************************************************************
  構造体：気温異常値検定 [2003.06.09]
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
#define SYS_TP_KENTEI_TP_UPP    50       //気温上限値デフォルト値
#define SYS_TP_KENTEI_TP_LOW    -50      //気温下限値デフォルト値
typedef struct tagSysTempKentei {
	short   sno;            //内部局番
	double  tp_upp;         //気温上限値           (-99.9～99.9, 以外:50.0)
	double  tp_low;         //気温下限値           (-99.9～99.9, 以外:-50.0)
	double  tp_max;         //気温最大変化量       (  0.1～99.9, 以外:設定値無し)
} sys_tp_kentei_t;


/********************************************************************
  構造体：気温警報基準値 [2003.06.09]
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
typedef struct tagSysTempAlm {
	short   sno;            //内部局番
	double  tp_l_alm1;      //気温注意値(低温用)   (-99.9～ 0.1, 以外:判定なし)
	double  tp_l_alm2;      //気温警戒値(低温用)   (-99.9～ 0.1, 以外:判定なし)
} sys_tp_alm_t;


/********************************************************************
  構造体：路温演算方式 [2003.06.04]
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
#define SYS_RD_ENZAN_MODE_INPT  1       //実測値
#define SYS_RD_ENZAN_CHNG_21_1  1       //２１号その１
typedef struct tagSysRoadEnzan {
	short   sno;            //内部局番
	char    mode;           //路温演算方式 (1:実測値, 以外:実測値)
	char    chng;           //路面状態変換方式 (1:２１号その１, 以外:非実装)
} sys_rd_enzan_t;


/********************************************************************
  構造体：路温異常値検定 [2003.06.09]
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
#define SYS_RD_KENTEI_TP_UPP    70       //路温上限値デフォルト値
#define SYS_RD_KENTEI_TP_LOW    -50      //路温下限値デフォルト値
typedef struct tagSysRoadKentei {
	short   sno;            //内部局番
	double  rd_upp;         //路温上限値           (-99.9～99.9, 以外:70.0)
	double  rd_low;         //路温下限値           (-99.9～99.9, 以外:-50.0)
	double  rd_max;         //路温最大変化量       (  0.1～99.9, 以外:設定値無し)
} sys_rd_kentei_t;


/********************************************************************
  構造体：路温警報基準値 [2003.06.09]
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
typedef struct tagSysRoadAlm {
	short   sno;            //内部局番
	double  rd_l_alm1;      //路温注意値(低温用)   (-99.9～ 0.1, 以外:判定なし)
	double  rd_l_alm2;      //路温警戒値(低温用)   (-99.9～ 0.1, 以外:判定なし)
} sys_rd_alm_t;


/********************************************************************
  構造体：堰・樋門雨量演算方式 [2003.06.09]
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
#define SYS_SK_ENZAN_MODE_INPT  1       //入力値
#define SYS_SK_ENZAN_MODE_CALC  2       //演算
typedef struct tagSysSekiEnzan {
	short   sno;            //内部局番
	short   chsr_mode;      //貯水量演算方式           (1:入力値, 以外:非実装)
	short   akyr_mode;      //空容量演算方式           (1:入力値, 以外:非実装)
	short   rynr_mode;      //流入量演算方式           (1:入力値, 以外:非実装)
	short   shrr_mode;      //総放流量演算方式         (1:入力値, 以外:非実装)
	short   zssr_mode;      //全取水量演算方式         (1:入力値, 以外:非実装)
	short   ssr1_mode;      //取水量１演算方式         (1:入力値, 以外:非実装)
	short   ssr2_mode;      //取水量２演算方式         (1:入力値, 以外:非実装)
	short   ssr3_mode;      //取水量３演算方式         (1:入力値, 以外:非実装)
	short   hokr_mode;      //補給量演算方式           (1:入力値, 以外:非実装)
	short   rs01_mode;      //予備１演算方式           (1:入力値, 以外:非実装)
	short   rs02_mode;      //予備２演算方式           (1:入力値, 以外:非実装)
	short   rs03_mode;      //予備３演算方式           (1:入力値, 以外:非実装)
	short   rs04_mode;      //予備４演算方式           (1:入力値, 以外:非実装)
	short   rs05_mode;      //予備５演算方式           (1:入力値, 以外:非実装)
	short   rs06_mode;      //予備６演算方式           (1:入力値, 以外:非実装)
	short   rs07_mode;      //予備７演算方式           (1:入力値, 以外:非実装)
	short   rs08_mode;      //予備８演算方式           (1:入力値, 以外:非実装)
	short   rs09_mode;      //予備９演算方式           (1:入力値, 以外:非実装)
	short   rs10_mode;      //予備10演算方式           (1:入力値, 以外:非実装)
	short   rs11_mode;      //予備11演算方式           (1:入力値, 以外:非実装)
	short   rs12_mode;      //予備12演算方式           (1:入力値, 以外:非実装)
	short   rs13_mode;      //予備13演算方式           (1:入力値, 以外:非実装)
	short   rs14_mode;      //予備14演算方式           (1:入力値, 以外:非実装)
	short   rs15_mode;      //予備15演算方式           (1:入力値, 以外:非実装)
	short   rs16_mode;      //予備16演算方式           (1:入力値, 以外:非実装)
} sys_sk_enzan_t;


/********************************************************************
  構造体：堰・樋門雨量異常値検定 [2003.06.04]
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
typedef struct tagSysSekiKentei {
	short   sno;            //内部局番
	double  chsr10_max;     //貯水量10分最大変化量     (未使用)
	double  chsr30_max;     //貯水量30分最大変化量     (未使用)
	double  chsr1h_max;     //貯水量時間最大変化量     (未使用)
	double  akyr10_max;     //空容量10分最大変化量     (未使用)
	double  akyr30_max;     //空容量30分最大変化量     (未使用)
	double  akyr1h_max;     //空容量時間最大変化量     (未使用)
	double  rynr_upp;       //流入量上限値             (0～99999.99, 以外:判定値なし)
	double  rynr_low;       //流入量下限値             (0～99999.99, 以外:判定値なし)
	double  rynr10_max;     //流入量10分最大変化量     (未使用)
	double  rynr30_max;     //流入量30分最大変化量     (未使用)
	double  rynr1h_max;     //流入量時間最大変化量     (未使用)
	double  shrr_upp;       //総放流量上限値           (0～99999.99, 以外:判定値なし)
	double  shrr_low;       //総放流量下限値           (0～99999.99, 以外:判定値なし)
	double  shrr10_max;     //総放流量10分最大変化量   (未使用)
	double  shrr30_max;     //総放流量30分最大変化量   (未使用)
	double  shrr1h_max;     //総放流量時間最大変化量   (未使用)
	double  zssr_upp;       //全取水量上限値           (0～99999.99, 以外:判定値なし)
	double  zssr_low;       //全取水量下限値           (0～99999.99, 以外:判定値なし)
	double  zssr10_max;     //全取水量10分最大変化量   (未使用)
	double  zssr30_max;     //全取水量30分最大変化量   (未使用)
	double  zssr1h_max;     //全取水量時間最大変化量   (未使用)
	double  ssr1_upp;       //取水量１上限値           (0～99999.99, 以外:判定値なし)
	double  ssr1_low;       //取水量１下限値           (0～99999.99, 以外:判定値なし)
	double  ssr110_max;     //取水量１10分最大変化量   (未使用)
	double  ssr130_max;     //取水量１30分最大変化量   (未使用)
	double  ssr11h_max;     //取水量１時間最大変化量   (未使用)
	double  ssr2_upp;       //取水量２上限値           (0～99999.99, 以外:判定値なし)
	double  ssr2_low;       //取水量２下限値           (0～99999.99, 以外:判定値なし)
	double  ssr210_max;     //取水量２10分最大変化量   (未使用)
	double  ssr230_max;     //取水量２30分最大変化量   (未使用)
	double  ssr21h_max;     //取水量２時間最大変化量   (未使用)
	double  ssr3_upp;       //取水量３上限値           (0～99999.99, 以外:判定値なし)
	double  ssr3_low;       //取水量３下限値           (0～99999.99, 以外:判定値なし)
	double  ssr310_max;     //取水量３10分最大変化量   (未使用)
	double  ssr330_max;     //取水量３30分最大変化量   (未使用)
	double  ssr31h_max;     //取水量３時間最大変化量   (未使用)
	double  rs01_upp;       //予備１上限値             (0～99999.99, 以外:判定値なし)
	double  rs01_low;       //予備１下限値             (0～99999.99, 以外:判定値なし)
	double  rs02_upp;       //予備２上限値             (0～99999.99, 以外:判定値なし)
	double  rs02_low;       //予備２下限値             (0～99999.99, 以外:判定値なし)
	double  rs03_upp;       //予備３上限値             (0～99999.99, 以外:判定値なし)
	double  rs03_low;       //予備３下限値             (0～99999.99, 以外:判定値なし)
	double  rs04_upp;       //予備４上限値             (0～99999.99, 以外:判定値なし)
	double  rs04_low;       //予備４下限値             (0～99999.99, 以外:判定値なし)
	double  rs05_upp;       //予備５上限値             (0～99999.99, 以外:判定値なし)
	double  rs05_low;       //予備５下限値             (0～99999.99, 以外:判定値なし)
	double  rs06_upp;       //予備６上限値             (0～99999.99, 以外:判定値なし)
	double  rs06_low;       //予備６下限値             (0～99999.99, 以外:判定値なし)
	double  rs07_upp;       //予備７上限値             (0～99999.99, 以外:判定値なし)
	double  rs07_low;       //予備７下限値             (0～99999.99, 以外:判定値なし)
	double  rs08_upp;       //予備８上限値             (0～99999.99, 以外:判定値なし)
	double  rs08_low;       //予備８下限値             (0～99999.99, 以外:判定値なし)
	double  rs09_upp;       //予備９上限値             (0～99999.99, 以外:判定値なし)
	double  rs09_low;       //予備９下限値             (0～99999.99, 以外:判定値なし)
	double  rs10_upp;       //予備10上限値             (0～99999.99, 以外:判定値なし)
	double  rs10_low;       //予備10下限値             (0～99999.99, 以外:判定値なし)
	double  rs11_upp;       //予備11上限値             (0～99999.99, 以外:判定値なし)
	double  rs11_low;       //予備11下限値             (0～99999.99, 以外:判定値なし)
	double  rs12_upp;       //予備12上限値             (0～99999.99, 以外:判定値なし)
	double  rs12_low;       //予備12下限値             (0～99999.99, 以外:判定値なし)
	double  rs13_upp;       //予備13上限値             (0～99999.99, 以外:判定値なし)
	double  rs13_low;       //予備13下限値             (0～99999.99, 以外:判定値なし)
	double  rs14_upp;       //予備14上限値             (0～99999.99, 以外:判定値なし)
	double  rs14_low;       //予備14下限値             (0～99999.99, 以外:判定値なし)
	double  rs15_upp;       //予備15上限値             (0～99999.99, 以外:判定値なし)
	double  rs15_low;       //予備15下限値             (0～99999.99, 以外:判定値なし)
	double  rs16_upp;       //予備16上限値             (0～99999.99, 以外:判定値なし)
	double  rs16_low;       //予備16下限値             (0～99999.99, 以外:判定値なし)
} sys_sk_kentei_t;


/********************************************************************
  構造体：堰・樋門警報基準値 [2003.06.04]
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
#define SYS_RA_ALM_NONE      9999        //警報判定なし
typedef struct tagSysSekiAlm {
	short   sno;            //内部局番
	double  kouzui;         //洪水量                   (0～99999.999, 以外:設定値なし)
	double  skei_kozi_q;    //設計洪水流量             (0～99999.999, 以外:設定値なし)
} sys_sk_alm_t;


/********************************************************************
  構造体：排水機場演算方式 [2003.06.04]
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
#define SYS_DR_ENZAN_MODE_INPT  1       //入力値
typedef struct tagSysDrainEnzan {
	short   sno;            //内部局番
	short   shsr_mode;      //総排水量演算方式         (1:入力値, 以外:非実装)
	short   chyr_mode;      //貯油量演算方式           (1:入力値, 以外:非実装)
	short   rs01_mode;      //予備１演算方式           (1:入力値, 以外:非実装)
	short   rs02_mode;      //予備２演算方式           (1:入力値, 以外:非実装)
	short   rs03_mode;      //予備３演算方式           (1:入力値, 以外:非実装)
	short   rs04_mode;      //予備４演算方式           (1:入力値, 以外:非実装)
	short   rs05_mode;      //予備５演算方式           (1:入力値, 以外:非実装)
	short   rs06_mode;      //予備６演算方式           (1:入力値, 以外:非実装)
	short   rs07_mode;      //予備７演算方式           (1:入力値, 以外:非実装)
	short   rs08_mode;      //予備８演算方式           (1:入力値, 以外:非実装)
	short   rs09_mode;      //予備９演算方式           (1:入力値, 以外:非実装)
	short   rs10_mode;      //予備10演算方式           (1:入力値, 以外:非実装)
	short   rs11_mode;      //予備11演算方式           (1:入力値, 以外:非実装)
	short   rs12_mode;      //予備12演算方式           (1:入力値, 以外:非実装)
	short   rs13_mode;      //予備13演算方式           (1:入力値, 以外:非実装)
	short   rs14_mode;      //予備14演算方式           (1:入力値, 以外:非実装)
	short   rs15_mode;      //予備15演算方式           (1:入力値, 以外:非実装)
	short   rs16_mode;      //予備16演算方式           (1:入力値, 以外:非実装)
} sys_dr_enzan_t;


/********************************************************************
  構造体：排水機場異常値検定 [2003.06.04]
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
typedef struct tagSysDrainKentei {
	short   sno;            //内部局番
	double  shsr10_max;     //総排水量10分最大変化量   (未使用)
	double  shsr30_max;     //総排水量30分最大変化量   (未使用)
	double  shsr1h_max;     //総排水量時間最大変化量   (未使用)
	double  chyr10_max;     //貯油量10分最大変化量     (未使用)
	double  chyr30_max;     //貯油量30分最大変化量     (未使用)
	double  chyr1h_max;     //貯油量時間最大変化量     (未使用)
	double  rs01_upp;       //予備１上限値             (0～99999.99, 以外:判定値なし)
	double  rs01_low;       //予備１下限値             (0～99999.99, 以外:判定値なし)
	double  rs02_upp;       //予備２上限値             (0～99999.99, 以外:判定値なし)
	double  rs02_low;       //予備２下限値             (0～99999.99, 以外:判定値なし)
	double  rs03_upp;       //予備３上限値             (0～99999.99, 以外:判定値なし)
	double  rs03_low;       //予備３下限値             (0～99999.99, 以外:判定値なし)
	double  rs04_upp;       //予備４上限値             (0～99999.99, 以外:判定値なし)
	double  rs04_low;       //予備４下限値             (0～99999.99, 以外:判定値なし)
	double  rs05_upp;       //予備５上限値             (0～99999.99, 以外:判定値なし)
	double  rs05_low;       //予備５下限値             (0～99999.99, 以外:判定値なし)
	double  rs06_upp;       //予備６上限値             (0～99999.99, 以外:判定値なし)
	double  rs06_low;       //予備６下限値             (0～99999.99, 以外:判定値なし)
	double  rs07_upp;       //予備７上限値             (0～99999.99, 以外:判定値なし)
	double  rs07_low;       //予備７下限値             (0～99999.99, 以外:判定値なし)
	double  rs08_upp;       //予備８上限値             (0～99999.99, 以外:判定値なし)
	double  rs08_low;       //予備８下限値             (0～99999.99, 以外:判定値なし)
	double  rs09_upp;       //予備９上限値             (0～99999.99, 以外:判定値なし)
	double  rs09_low;       //予備９下限値             (0～99999.99, 以外:判定値なし)
	double  rs10_upp;       //予備10上限値             (0～99999.99, 以外:判定値なし)
	double  rs10_low;       //予備10下限値             (0～99999.99, 以外:判定値なし)
	double  rs11_upp;       //予備11上限値             (0～99999.99, 以外:判定値なし)
	double  rs11_low;       //予備11下限値             (0～99999.99, 以外:判定値なし)
	double  rs12_upp;       //予備12上限値             (0～99999.99, 以外:判定値なし)
	double  rs12_low;       //予備12下限値             (0～99999.99, 以外:判定値なし)
	double  rs13_upp;       //予備13上限値             (0～99999.99, 以外:判定値なし)
	double  rs13_low;       //予備13下限値             (0～99999.99, 以外:判定値なし)
	double  rs14_upp;       //予備14上限値             (0～99999.99, 以外:判定値なし)
	double  rs14_low;       //予備14下限値             (0～99999.99, 以外:判定値なし)
	double  rs15_upp;       //予備15上限値             (0～99999.99, 以外:判定値なし)
	double  rs15_low;       //予備15下限値             (0～99999.99, 以外:判定値なし)
	double  rs16_upp;       //予備16上限値             (0～99999.99, 以外:判定値なし)
	double  rs16_low;       //予備16下限値             (0～99999.99, 以外:判定値なし)
} sys_dr_kentei_t;


/********************************************************************
  構造体：排水機場警報基準値 [2003.06.04]
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
typedef struct tagSysDrainAlm {
	short   sno;            //内部局番
} sys_dr_alm_t;


/********************************************************************
  構造体：視程路温演算方式 [2003.07.02]
  項目  ：sno           内部局番        (1～999, 以外:設定値なし)
          以下下記参照
********************************************************************/
#define SYS_SE_ENZAN_MODE_CALC1 1       //演算方式１
#define SYS_SE_ENZAN_MODE_INPT  2       //実測値
typedef struct tagSysSeeEnzan {
	short   sno;            //内部局番
	char    mode;           //視程演算方式 (1:演算方式１,2:実測値, 以外:演算方式１)
} sys_se_enzan_t;



/********************************************************************
  プロトタイプ宣言
********************************************************************/

/**********************************************************
  関数  ：inisplit
  機能  ：文字列分割
  引数  ：sp                [IN]  文字列
        ：p_name            [OUT] 名称
        ：p_value           [OUT] 値
  戻り値：
  説明  ：”名称 = 値”の文字列を名称と値に分割します。
**********************************************************/
void inisplit(char *sp, char *p_name, char *p_value);

/**********************************************************
  関数  ：csvtrim
  機能  ：前後スペース削除
  引数  ：sp                [I/O] 文字列
  戻り値：
  説明  ：文字列から前スペース、後スペースを削除します。
**********************************************************/
void csvtrim(char *sp);

/**********************************************************
  関数  ：csvsplit
  機能  ：CSV文字列分割
  引数  ：sp                [IN]  CSV文字列(カンマ区切り)
        ：n                 [IN]  取得する位置
        ：p_value           [OUT] 取得した文字列
  戻り値：
  説明  ：CSV文字列から指定された位置の文字列を取得します。
**********************************************************/
void csvsplit(char *sp, int n, char *p_value);

/********************************************************************
  関数  ：csvsplit_n
  機能  ：文字列分割(文字数制限あり)
  引数  ：sp                [IN]  文字列
        ：n                 [IN]  取得する位置
        ：p_value           [OUT] 取得した文字列
        ：v_len             [IN]  取得領域のサイズ
  戻り値：取得文字数
  説明  ：区切り文字で分割されてできた文字列で
        ：指定された位置の文字列を取得します。
********************************************************************/
int csvsplit_n(char *sp, int n, char *p_value, int v_len);

/**********************************************************
  関数  ：csvread
  機能  ：１行読み込み
  引数  ：fp                [IN]  ファイル構造体
        ：dp                [OUT] 読み込んだ文字列
  戻り値：
  説明  ：CSVファイルから１行読み込みます。
**********************************************************/
void csvread(FILE *fp, char *dp);

/**********************************************************
  関数  ：csvread_n
  機能  ：１行読み込み(文字数制限あり)
  引数  ：fp                [IN]  ファイル構造体
        ：dp                [OUT] 読み込んだ文字列
        ：d_len             [IN]  取得領域サイズ
  戻り値：
  説明  ：CSVファイルから１行読み込みます。
**********************************************************/
void csvread_n(FILE *fp, char *dp, int d_len);

/**********************************************************
  関数  ：getMyNumber
  機能  ：自装置番号の取得
  引数  ：pt                [OUT] 自装置番号
  戻り値：成否
        ：     0 = 成功
        ：  以外 = 失敗
  説明  ：
**********************************************************/
int  getMyNumber(sys_mynumber_t *pt);

/**********************************************************
  関数  ：getServerIniFile
  機能  ：サーバ初期化ファイル読み込み
  引数  ：officeID          [IN]  自事務所番号
        ：unitID            [IN]  自装置番号
		：pt                [OUT] サーバ情報
  戻り値：成否
        ：     0 = 成功
        ：  以外 = 失敗
  説明  ：
**********************************************************/
int  getServerIniFile(int officeID, int unitID, ini_server_t *pt);

/**********************************************************
  関数  ：getSysConstKindInfo
  機能  ：局種別毎情報の領域初期化
  引数  ：pt                [IN]  局種別管理情報
  戻り値：成否
        ：     0 = 成功
        ：  以外 = 失敗
  説明  ：
**********************************************************/
int  getSysConstKindInfo (kind_manager_t *pt);

/**********************************************************
  関数  ：setSysConstKindInfo
  機能  ：局種別毎情報の取得
  引数  ：pt                [OUT] 局種別管理情報
  戻り値：成否
        ：     0 = 成功
        ：  以外 = 失敗
  説明  ：
**********************************************************/
int  setSysConstKindInfo (kind_manager_t *pt);

/**********************************************************
  関数  ：freeSysConstKindInfo
  機能  ：局種別毎情報の領域開放
  引数  ：pt                [IN]  局種別管理情報
  戻り値：
  説明  ：
**********************************************************/
void freeSysConstKindInfo(kind_manager_t *pt);

/**********************************************************
  関数  ：checkSysConstKindInfo
  機能  ：システム定数即時更新チェック
  引数  ：pt                [IN]  局種別管理情報
        ：dpath             [IN]  データパス
        ：pno               [IN]  ソフト番号
  戻り値：更新フラグ
        ：  TRUE  = 更新
        ：  FALSE = 未更新
  説明  ：
**********************************************************/
BOOL checkSysConstKindInfo(kind_manager_t *pt, char *dpath, int pno);

/**********************************************************
  関数  ：getSysConstExio
  機能  ：装置間通信情報読み込み
  引数  ：app               [IN]  アプリケーションパス
        ：syskind           [IN]  処理種別
        ：officeNo          [IN]  事務所番号
        ：unitNo            [IN]  装置番号
        ：houroNo           [IN]  方路番号
        ：p                 [OUT] 装置間通信情報
  戻り値：成否
        ：     0 = 成功
        ：  以外 = 失敗
  説明  ：
**********************************************************/
int  getSysConstExio(char *app, char *syskind, int officeNo, int unitNo, int houroNo, sys_exioinfo_t *p);


void *getSysConstRainEnzan  (char *path, int max);
void *getSysConstRainKentei (char *path, int max);
void *getSysConstRainAlm    (char *path, int max);
void *getSysConstRainSnake  (char *path, int max);
void *getSysConstRainOld    (char *path, int max);
void *getSysConstRiverEnzan (char *path, int max);
void *getSysConstRiverKentei(char *path, int max);
void *getSysConstRiverAlm   (char *path, int max);
void *getSysConstRiverDanmen(char *path, int max);
void *getSysConstRiverHQ    (char *path, int max);
void *getSysConstRiverOld   (char *path, int max);
void *getSysConstDamEnzan   (char *path, int max);
void *getSysConstDamKentei  (char *path, int max);
void *getSysConstDamAlm     (char *path, int max);
void *getSysConstDamShogen  (char *path, int max);
void *getSysConstTideEnzan  (char *path, int max);
void *getSysConstTideKentei (char *path, int max);
void *getSysConstTideAlm    (char *path, int max);
void *getSysConstTideTenmon (char *path, int max);
void *getSysConstTideSiomi  (char *path, int max);
void *getSysConstWindEnzan  (char *path, int max);
void *getSysConstWindKentei (char *path, int max);
void *getSysConstWindAlm    (char *path, int max);
void *getSysConstRelayEnzan (char *path, int max);
void *getSysConstSnowEnzan  (char *path, int max);	// [2003.06.04]
void *getSysConstSnowKentei (char *path, int max);	// [2003.06.04]
void *getSysConstSnowAlm    (char *path, int max);	// [2003.06.04]
void *getSysConstSnowPair   (char *path, int max);	// [2003.06.04]
void *getSysConstTempEnzan  (char *path, int max);	// [2003.06.04]
void *getSysConstTempKentei (char *path, int max);	// [2003.06.04]
void *getSysConstTempAlm    (char *path, int max);	// [2003.06.04]
void *getSysConstRoadEnzan  (char *path, int max);	// [2003.06.04]
void *getSysConstRoadKentei (char *path, int max);	// [2003.06.04]
void *getSysConstRoadAlm    (char *path, int max);	// [2003.06.04]
void *getSysConstSekiEnzan  (char *path, int max);	// [2003.06.04]
void *getSysConstSekiKentei (char *path, int max);	// [2003.06.04]
void *getSysConstSekiAlm    (char *path, int max);	// [2003.06.04]
void *getSysConstDrainEnzan (char *path, int max);	// [2003.06.04]
void *getSysConstDrainKentei(char *path, int max);	// [2003.06.04]
void *getSysConstDrainAlm   (char *path, int max);	// [2003.06.04]
void *getSysConstSeeEnzan   (char *path, int max);	// [2003.07.02]
void *getSysConstRainCastEnzan  (char *path, int max);	// [2004.01.09]
void *getSysConstRainCastKentei (char *path, int max);	// [2004.01.09]
void *getSysConstRainCastAlm    (char *path, int max);	// [2004.01.09]
void *getSysConstRainCastSnake  (char *path, int max);	// [2004.01.09]
void *getSysConstRiverCastEnzan (char *path, int max);	// [2004.01.14]
void *getSysConstRiverCastKentei(char *path, int max);	// [2004.01.14]
void *getSysConstRiverCastAlm   (char *path, int max);	// [2004.01.14]
void *getSysConstRiverCastHQ    (char *path, int max);	// [2004.01.14]

#endif
