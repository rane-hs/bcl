///////////////////////////////////////////////////////////////////////////
//
//  土木総合防災情報システム－装置間入出力情報クラス
//
////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#pragma warning( disable : 4786 )   // C4786のWarningを無視
#include "ExioInfo.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include "Split.h"
#include "Str.h"

M__USING_BCL

/********************************************************************
*	引数：
*	戻値：
*	説明：
********************************************************************/
CExioInfo::CExioInfo()
{
	dp.clear();
	in_dp.clear();
	out_dp.clear();
}

/********************************************************************
*	機能：デスラクタ
*	引数：
*	戻値：
*	説明：
********************************************************************/
CExioInfo::~CExioInfo()
{

	if (dp.size() > 0)
		dp.clear();
	if (in_dp.size() > 0)
		in_dp.clear();
	if (out_dp.size() > 0)
		out_dp.clear();
}


/********************************************************************
*	機能：装置間入出力情報読み込み処理
*	引数：fpath				[IN]  ファイルパス
*	戻値：成否
        ：  1 = 更新あり
		：  0 = 更新なし
		： -1 = ファイルパスがＮＵＬＬ
		： -2 = ファイルが存在しない
		： -3 = ファイルのオープンに失敗
		： -4 = 内部エラー
*	説明：
********************************************************************/
int CExioInfo::get_file(const char *fpath)
{
	struct _stat		sts;
	exio_info_data_t	wk;
	bcl::CSplit				sp;
	std::ifstream			fs;
	std::string				dt;
	char				st[1024];

	// パラメータの確認
	if (fpath == NULL) {
		if (file_path.empty() == true)
			return -1;
	} else {
		if (file_path.compare(fpath) != 0)
			upd_time = 0;
		file_path = fpath;
	}

	// ファイルの確認
	if ( _stat(file_path.c_str(), &sts) != 0)
		return -2;

	// ファイルの更新なし
	if (upd_time == sts.st_mtime)
		return 0;

	// ファイル更新時刻の取得
	upd_time = sts.st_mtime;

	// 初期化
	if (dp.size() > 0)
		dp.clear();
	memset(&info, 0, sizeof(info));

	// 装置間入出力情報の読み込み
	fs.open(file_path.c_str());
	if (fs.is_open() == false)
		return -3;

	try {
		//１行目
		fs.getline(st, sizeof(st));
		if (st[0] != '\0') {
			sp.split(st, ",");
			// 事務所番号
			dt = sp.at(3);
			hd.officeNo = bcl::CStr::stoi(dt);
			// 受信PORT
			dt = sp.at(5);
			hd.r_port = bcl::CStr::stol(dt);
			// １分値出力
			dt = sp.at(10);
			hd.ioflg_1m = bcl::CStr::stoi(dt);
		}
		// ２行目
		fs.getline(st, sizeof(st));
		if (st[0] != '\0') {
			sp.split(st, ",");
			// 装置番号
			dt = sp.at(3);
			hd.unitNo = bcl::CStr::stoi(dt);
			// 送信先PORT
			dt = sp.at(5);
			hd.s_port = bcl::CStr::stol(dt);
			// ラグタイム１分START
			dt = sp.at(7);
			hd.rag_1m.st = bcl::CStr::stol(dt);
			// ラグタイム１分END
			dt = sp.at(8);
			hd.rag_1m.et = bcl::CStr::stol(dt);
			// 定時出力
			dt = sp.at(10);
			hd.ioflg_hm = bcl::CStr::stoi(dt);
		}
		// ３行目
		fs.getline(st, sizeof(st));
		if (st[0] != '\0') {
			sp.split(st, ",");
			// 方路番号
			dt = sp.at(3);
			hd.houroNo = bcl::CStr::stoi(dt);
			// 送信先ＩＰ
			hd.s_ip = sp.at(5);
			// ラグタイム正定時START
			dt = sp.at(7);
			hd.rag_hm.st = bcl::CStr::stol(dt);
			// ラグタイム正定時END
			dt = sp.at(8);
			hd.rag_hm.et = bcl::CStr::stol(dt);
			// 日集計出力
			dt = sp.at(10);
			hd.ioflg_dy = bcl::CStr::stoi(dt);
		}
		// ４行目
		fs.getline(st, sizeof(st));
		if (st[0] != '\0') {
			sp.split(st, ",");
			// 事務所名
			hd.myOfficeName = sp.at(3);
			// 送信先事務所名
			hd.toOfficeName = sp.at(5);
			// ラグタイム日集計START
			dt = sp.at(7);
			hd.rag_dy.st = bcl::CStr::stol(dt);
			// ラグタイム日集計END
			dt = sp.at(8);
			hd.rag_dy.et = bcl::CStr::stol(dt);
		}
		// ５行目
		fs.getline(st, sizeof(st));
		if (st[0] != '\0') {
			sp.split(st, ",");
			// 装置名
			hd.myUnitName = sp.at(3);
			// 送信先装置名
			hd.toUnitName = sp.at(5);
		}
		// ６行目
		fs.getline(st, sizeof(st));
		// ７行目
		fs.getline(st, sizeof(st));

		while (fs.eof() == false) {
			// １行読み込み
			fs.getline(st, sizeof(st));
			if (st[0] == '\0')
				continue;

			memset(&wk, 0, sizeof(wk));
			sp.split(st, ",");

			// 入出力種別コード
			dt = sp.at(0);
			wk.io_kind = bcl::CStr::stoi(dt);
			if ((wk.io_kind != EXIO_KIND_IN) && (wk.io_kind != EXIO_KIND_OUT))
				continue;
			// 入出力局番
			dt = sp.at(1);
			wk.io_sno = bcl::CStr::stoi(dt);
			// 入出力項番
			dt = sp.at(2);
			wk.io_ino = bcl::CStr::stoi(dt);
			// 入出力管理事務所番号
			dt = sp.at(3);
			wk.io_office = bcl::CStr::stoi(dt);
			// 入出力データ種別番号
			dt = sp.at(4);
			wk.io_data = bcl::CStr::stoi(dt);
			// 倍長コード
			dt = sp.at(5);
			wk.code = bcl::CStr::stoi(dt);
			// ファイル種別
			dt = sp.at(6);
			wk.filetype = bcl::CStr::stoi(dt);
			// 局種別コード
			dt = sp.at(7);
			wk.kno = bcl::CStr::stoi(dt);
			if (wk.kno <= 0)
				continue;
			// 内部局番
			dt = sp.at(8);
			wk.sno = bcl::CStr::stoi(dt);
			if (wk.sno <= 0)
				continue;
			// データ項目順番
			dt = sp.at(9);
			wk.ino = bcl::CStr::stoi(dt);
			// スケール補正コード
			dt = sp.at(10);
			wk.scale = bcl::CStr::stoi(dt);
			// データ補正情報
			dt = sp.at(11);
			wk.conversion = bcl::CStr::stoi(dt);
			// 予備
			dt = sp.at(12);
			wk.reserve2 = bcl::CStr::stoi(dt);

			// 登録
			dp.push_back(wk);
		}
	}
	catch (...) {
		fs.close();
		return -4;
	}
	// ファイルクローズ
	fs.close();
	return 1;
}

/********************************************************************
*	機能：入出力情報作成処理
*	引数：pKind				[IN]  局種別系情報
*	戻値：
*	説明：
********************************************************************/
int CExioInfo::setup()
{
	int	i;
	int	n;

	n = dp.size();
	if (n > 0) {
		// 初期化
		if (in_dp.size() > 0)
			in_dp.clear();
		if (out_dp.size() > 0)
			out_dp.clear();

		// 入出力情報毎に振り分ける
		for (i = 0; i < n; i++) {
			if (dp[i].io_kind == EXIO_KIND_IN) {
				in_dp.push_back(dp[i]);
			} else if (dp[i].io_kind == EXIO_KIND_OUT) {
				out_dp.push_back(dp[i]);
			}
		}
	}

	return 0;
}

