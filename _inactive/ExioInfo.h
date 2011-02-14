///////////////////////////////////////////////////////////////////////////
//*	機能  ：装置間入出力情報クラス
//  更新履歴：
//          ：
//          ：
//          ：
//          ：
///////////////////////////////////////////////////////////////////////////
#ifndef _CEXIO_INFO_H_
#define _CEXIO_INFO_H_

#include <stdio.h>
#include <string>
#include <vector>
#include "bcl.h"

M__BEGIN_NAMESPACE_BCL

/********************************************************************
  構造体：ラグタイム情報
  項目  ：st					開始
        ：et					終了
********************************************************************/
typedef struct {
	long	st;										// 開始
	long	et;										// 終了
} exio_info_ragtime_t;

/********************************************************************
  構造体：装置間入出力情報ヘッダ部
  項目  ：officeNo				事務所番号
        ：unitNo				装置番号
        ：houroNo				方路番号
        ：myOfficeName			自事務所名
        ：myUnitName			自装置名
        ：toOfficeName			送信先事務所名
        ：toUnitName			送信先装置名
        ：r_port				受信ポート
        ：s_port				送信先ポート
        ：s_ip					送信先ＩＰ
        ：rag_1m				１分値ラグタイム
        ：rag_hm				正定時ラグタイム
        ：rag_dy				日集計ラグタイム
        ：ioflg_1m				１分値出力有無(0=なし,1=出力)
        ：ioflg_hm				正定時出力方法(0=系,  1=一括)
        ：ioflg_dy				日集計出力有無(0=なし,1=出力)
********************************************************************/
typedef struct {
	int					officeNo;					// 事務所番号
	int					unitNo;						// 装置番号
	int					houroNo;					// 方路番号
	std::string			myOfficeName;				// 自事務所名
	std::string				myUnitName;					// 自装置名
	std::string				toOfficeName;				// 送信先事務所名
	std::string				toUnitName;					// 送信先装置名
	long				r_port;						// 受信ポート
	long				s_port;						// 送信先ポート
	std::string				s_ip;						// 送信先ＩＰ
	exio_info_ragtime_t	rag_1m;						// １分値ラグタイム
	exio_info_ragtime_t	rag_hm;						// 正定時ラグタイム
	exio_info_ragtime_t	rag_dy;						// 日集計ラグタイム
	int					ioflg_1m;					// １分値出力有無
	int					ioflg_hm;					// 正定時出力方法
	int					ioflg_dy;					// 日集計出力有無
} exio_info_head_t;

/********************************************************************
  構造体：装置間入出力情報
  項目  ：io_kind				入出力種別コード    (1=入力,2=出力,以外=設定無し)
        ：io_sno				入出力局番          (0～65535  ,以外=設定無し)
        ：io_ino				入出力項番          (0～65535  ,以外=設定無し)
        ：io_office				入出力管理事務所番号(1～99999  ,以外=設定無し)
        ：io_data				入出力データ種別番号(1～999    ,以外=設定無し)
        ：code					倍長コード          (1=倍長    ,以外=単調)
        ：filetype				ファイル種別        (1=日集計値,以外=時間値)
        ：kno					局種別コード        (1～6      ,以外=設定無し)
        ：sno					内部局番号          (1～999    ,以外=設定無し)
        ：ino					データ項目順番      (1～99     ,以外=設定無し)
        ：scale					スケール補正コード  (1=10倍,2=100倍,3=1000倍,4=1/10倍,5=1/100倍,6=1/1000,以外=1倍)
        ：conversion			データ変換情報
        ：reserve2				予備
********************************************************************/
#define EXIO_KIND_IN		1						// 入力
#define EXIO_KIND_OUT		2						// 出力
#define EXIO_CODE_DOUBLE	1						// 倍長コード
#define EXIO_TYPE_DAILY		1						// 日集計値
#define EXIO_SCALE_10		1						// 10倍
#define EXIO_SCALE_100		2						// 100倍
#define EXIO_SCALE_1000		3						// 1000倍
#define EXIO_SCALE_D10		4						// 1/10倍
#define EXIO_SCALE_D100		5						// 1/100倍
#define EXIO_SCALE_D1000	6						// 1/1000倍
typedef struct {
	int		io_kind;								// 入出力種別コード
	int		io_sno;									// 入出力局番
	int		io_ino;									// 入出力項番
	int		io_office;								// 入出力管理事務所番号
	int		io_data;								// 入出力データ種別番号
	int		code;									// 倍長コード
	int		filetype;								// ファイル種別
	int		kno;									// 局種別コード
	int		sno;									// 内部局番号
	int		ino;									// データ項目番号
	int		scale;									// スケール補正コード
	long	conversion;								// データ変換情報
	int		reserve2;								// 予備
} exio_info_data_t;

/********************************************************************
  構造体：装置間入出力情報
  項目  ：hd					装置間入出力ヘッダ部
        ：dp					装置間入出力データ部
********************************************************************/
typedef struct {
	exio_info_head_t			hd;					// ヘッダ部
	std::vector<exio_info_data_t>	dp;					// データ部
} exio_info_t;

/********************************************************************
  クラス定義
********************************************************************/
class CExioInfo :public exio_info_t{
public:
	std::string						file_path;			// ファイルパス
	time_t						upd_time;			// ファイル更新時刻
	std::vector<exio_info_data_t>	in_dp;				// 入力情報
	std::vector<exio_info_data_t>	out_dp;				// 出力情報

	// コンストラクタ
	CExioInfo();
	// デスラクタ
	~CExioInfo();

	// ローカル情報読み込み
	int get_file(const char *fpath = NULL);
	// 入出力情報作成処理
	int setup();

};

M__END_NAMESPACE_BCL


#endif
