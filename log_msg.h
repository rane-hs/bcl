
#ifndef _MY_LOG_MSG_H_
#define _MY_LOG_MSG_H_

namespace bcl{


#include <string>
#include <iostream>
#include <fstream>

#include <bcl/bclstr.h>
#include <bcl/FileSystem.h>


//ログ出力
#define DG_LOG_ERR          0           //エラー
#define DG_LOG_INFO         1           //詳細
#define DG_LOG_WARN         2           //警告
#define DG_LOG_DEBUG        3           //デバッグ
#define DG_LOG_NON          5           //ログには出力しない


#define MAX_SIZE_MSG     2048           //メッセージサイズ

/********************************************************************
  機能  ：ログメッセージ出力クラス
********************************************************************/
class log_message {
public:

	bcl::str_t dpath;                   //出力フォルダ
	bcl::str_t exe_name;                //ソフト名称
	int    office_no;               //事務所番号
	int    unit_no;                 //装置番号
	int    horo_no;                 //方路番号
	int    level;                   //出力レベル
	int    days;                    //保存日数
	int    max_size_file;           //最大ファイルサイズ
	int    before_day;              //前回ファイル削除日

	//ctor, dtor
	log_message():
		dpath(TEXT("")),exe_name(TEXT("")),
		office_no(0),unit_no(0),horo_no(0),level(0),days(0),
		max_size_file(10000000),before_day(0)
	{
	};
	~log_message(){};

	/********************************************************************
	  機能  ：初期設定
	  引数  ：dpath             [IN]  ログ出力パス
	        ：exe_name          [IN]  ソフト名称
	        ：office_no         [IN]  事務所番号
	        ：unit_no           [IN]  装置番号
	        ：horo_no           [IN]  方路番号
	        ：level             [IN]  出力レベル
	  戻り値：
	  説明  ：
	********************************************************************/
	void init(const bcl::str_t dpath_, const bcl::str_t exe_name_, int office_no_, int unit_no_, int horo_no_, int level_)
	{
		dpath     = dpath_;     //ログ出力パス
		exe_name  = exe_name_;  //ソフト名称
		office_no = office_no_; //事務所番号
		unit_no   = unit_no_;   //装置番号
		horo_no   = horo_no_;   //方路番号
		level     = level_;     //出力レベル
	};

	/********************************************************************
	  関数  ：put_msg
	  機能  ：メッセージ出力
	  引数  ：typ               [IN]  メッセージタイプ
	        ：lvl               [IN]  レベル
	        ：knd               [IN]  情報種別
	        ：buf               [IN]  出力メッセージ
	        ：...               [IN]  パラメータ
	  戻り値：
	  説明  ：
	********************************************************************/
	void put_msg(int typ, int lvl, const bcl::str_t &knd, const bcl::char_t *buf, ...)
	{
		bcl::char_t fname[_MAX_PATH];
		bcl::char_t wk[MAX_SIZE_MSG];
		bcl::str_t  msg;
		bcl::str_t  styp;
		bcl::str_t  sknd;
		va_list ap;
		int wk_typ;
		int ret;


		//可変引数処理
		va_start(ap, buf);

#ifdef _UNICODE
		ret = _vsnwprintf(wk, sizeof(wk)-1, buf, ap);
#else
		ret = _vsnprintf(wk, sizeof(wk)-1, buf, ap);
#endif
		va_end(ap);
		//領域をオーバーした場合の処理
		wk[MAX_SIZE_MSG-1] = '\0';


		//現在時刻の取得
		const bcl::time24 nowTime = bcl::NowTime();
		//ログ出力フォルダ
		const bcl::str_t sday = bcl::format(TEXT("%04d/%02d/%02d"), nowTime.year24(), nowTime.month24(), nowTime.day24() );
		//ログ出力時刻
		const bcl::str_t stim = bcl::format(TEXT("%04d/%02d/%02d %02d:%02d:%02d"), nowTime.year24(), nowTime.month24(), nowTime.day24(), nowTime.hour24(), nowTime.minute(), nowTime.second());

		//情報種別
		sknd = knd;

		//レベル判定
		if ( level < lvl ) {
			wk_typ = DG_LOG_NON;
		} else {
			wk_typ = typ;
		}

		//詳細 
		switch( wk_typ ) {
		case DG_LOG_ERR :
			styp = TEXT("[エラー]");
			break;
		case DG_LOG_INFO :
			styp = TEXT("[ 詳細 ]");
			break;
		case DG_LOG_WARN :
			styp = TEXT("[ 警告 ]");
			break;
		case DG_LOG_NON :
			styp = TEXT("[      ]");
			break;
		default :
			styp = TEXT("[ 情報 ]");
			break;
		}

		//メッセージ編集
		if ( sknd.empty() == true ) {
			msg = stim + TEXT(" ") + styp + TEXT(" ") + wk;
		} else {
			msg = stim + TEXT(" ") + styp + TEXT(" [") + sknd + TEXT("] ") + wk;
		}

		//標準出力
		
#ifdef _UNICODE
		wprintf(TEXT("%s\n"), msg.c_str());
		//ログ出力パス設定
		if ( sknd.empty() == true ) {
			ret = _snwprintf(fname, sizeof(fname)-1, TEXT("%s\\log\\%s_%02d%02d%02d.log"), dpath.c_str(), exe_name.c_str(), office_no, unit_no, horo_no);
		} else {
			ret = _snwprintf(fname, sizeof(fname)-1, TEXT("%s\\log\\%s\\%s_%s_%02d%02d%02d.log"), dpath.c_str(), sday.c_str(), exe_name.c_str(), sknd.c_str(), office_no, unit_no, horo_no);
		}
#else
		printf(TEXT("%s\n"), msg.c_str());
		//ログ出力パス設定
		if ( sknd.empty() == true ) {
			ret = _snprintf(fname, sizeof(fname)-1, TEXT("%s\\log\\%s_%02d%02d%02d.log"), dpath.c_str(), exe_name.c_str(), office_no, unit_no, horo_no);
		} else {
			ret = _snprintf(fname, sizeof(fname)-1, TEXT("%s\\log\\%s\\%s_%s_%02d%02d%02d.log"), dpath.c_str(), sday.c_str(), exe_name.c_str(), sknd.c_str(), office_no, unit_no, horo_no);
		}
#endif

		//領域をオーバーした場合
		if ( ret < 0 ) {
			return;
		}

		//ログ出力
		if ( wk_typ == DG_LOG_NON ) {
			//printf("%s\n", msg.c_str());
		} else {
			//printf("%s\n", msg.c_str());
			write_log(fname, msg.c_str());
		}
	}



	/********************************************************************
	  関数  ：put_msg
	  機能  ：メッセージ出力
	  引数  ：typ               [IN]  メッセージタイプ
	        ：lvl               [IN]  レベル
	        ：knd               [IN]  情報種別
	        ：code              [IN]  エラーコード
	  戻り値：
	  説明  ：
	********************************************************************/
	void put_msg(int typ, int lvl, const bcl::str_t knd, int code)
	{
		LPVOID lpMsgBuf;
		bcl::basic_split<bcl::str_t> sp;
		bcl::str_t wk;

		//システムメッセージ取得
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			code,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // 既定の言語
			(LPTSTR) &lpMsgBuf,
			0,
			NULL);

		//ログ出力
		sp.tokenize((bcl::char_t *)lpMsgBuf, TEXT("\n\r"));
		for (size_t i = 0; i < sp.size(); i++ ) {
			wk = sp.at(i);
			if ( wk.empty() == false ) {
				put_msg(typ, lvl, knd, TEXT("Code(%d)：%s"), code, wk.c_str());
			}
		}

		//領域の解放
		LocalFree(lpMsgBuf);

	};

	/********************************************************************
	  関数  ：write_log
	  機能  ：ログ出力
	  引数  ：fpath             [IN]  ファイルパス
	        ：msg               [IN]  出力メッセージ
	  戻り値：
	  説明  ：
	********************************************************************/
	int write_log(const bcl::char_t *fpath, const bcl::char_t *msg)
	{

		//ファイルパスの確認
#ifdef _UNICODE
		if ( wcslen(fpath) < 1) {	return 0;		}
#else
		if ( strlen(fpath) < 1) {	return 0;		}
#endif

		//フォルダの確認
		if ( bcl::Dir::IsExist(fpath) == false ) {
			bcl::Dir::Make(fpath);
		}


		//ファイルサイズの取得
#ifdef _UNICODE
		struct _stat sts;
		int ret = _wstat(fpath, &sts);
#else
		struct _stat sts;
		int ret = _stat(fpath, &sts);
#endif
		int cnt;
		int idx;
		if ( ret == 0 ) {
			//ファイルサイズが大きくなったなら
			if ( sts.st_size > max_size_file ) {
				cnt = 0;
				idx = 1;
				bcl::char_t wkpath[_MAX_PATH];
				bcl::char_t drive[_MAX_DRIVE];
				bcl::char_t dir[_MAX_DIR];
				bcl::char_t fname[_MAX_FNAME];
				bcl::char_t ext[_MAX_EXT];				//パスの分割


#ifdef _UNICODE
				_wsplitpath(fpath, drive, dir, fname, ext);
				while ( cnt < 100 ) {
					//ファイルパスの設定
					_snwprintf(wkpath, sizeof(wkpath)-1, TEXT("%s%s%s_%d%s"), drive, dir, fname, idx++, ext);
#else
				_splitpath(fpath, drive, dir, fname, ext);
				while ( cnt < 100 ) {
					//ファイルパスの設定
					_snprintf(wkpath, sizeof(wkpath)-1, TEXT("%s%s%s_%d%s"), drive, dir, fname, idx++, ext);
#endif
					//領域をオーバーした場合の処理
					wkpath[_MAX_PATH-1] = '\0';
					if ( bcl::File::IsExist(wkpath) == false ) break;
					cnt++;
				}
				if ( cnt < 100 ) {
					//ファイル名変更
					bcl::File::Move(fpath, wkpath);
				}
			}
		}

		//ファイルオープン

#ifdef _UNICODE
		//ofstreamがwchar対応していないため、bcl::narrowをかます
		std::string filePath = bcl::narrow(fpath);
		std::ofstream ofs(filePath, std::ios::app);
#else
		std::ofstream ofs(fpath, std::ios::app);
#endif
		if (! ofs.is_open()) {
			Sleep(200);
			ofs.open(fpath);
			if ( !ofs.is_open() ) {
				Sleep(500);
				ofs.open(fpath);
			}
		}

		if (! ofs.is_open()) {
			return -3;
		}
		//書き込み
#ifdef _UNICODE
		ofs << bcl::narrow(msg) << std::endl;
#else
		ofs << msg << std::endl;
#endif
		//ファイルクローズ
		ofs.close();

		//ログ削除
		delete_log();

		return 0;
	};

	/********************************************************************
	  関数  ：delete_log
	  機能  ：ログ削除
	  引数  ：
	  戻り値：
	  説明  ：
	********************************************************************/
	void delete_log()
	{
		struct _finddata_t dt;
		std::vector<std::string> fname;
		char   wkpath[_MAX_PATH];
		std::string fpath;
		long hFind;

		//削除対象かチェック
		if ( days <= 0 ) {
			return;
		}

		//現在時刻の取得
		const bcl::time24 nowTime;
		const bcl::time24 delTime = nowTime - clx::days(days);

		//日付けが変わっているなら
		if ( before_day == nowTime.day24() ) {
			return;
		}
		before_day = nowTime.day24();


		//削除対象年月日
		const std::string sdate = bcl::format("%04d%02d%02d", delTime.year24(), delTime.month24(), delTime.day24());

		//フォルダ内のチェック
		_snprintf(wkpath, sizeof(wkpath)-1, "%s\\log\\*", dpath.c_str());
		hFind = _findfirst(wkpath, &dt);
		if ( hFind != -1 ) {
			do {
				if ( strcmp(dt.name, ".") == 0 || strcmp(dt.name, "..") == 0 ) {
					continue;
				}

				//古いディレクトリの判定
				if ( strcmp(sdate.c_str(), dt.name) >= 0 ) {
					fname.push_back(dt.name);
				}

			} while ( _findnext(hFind, &dt) == 0 );
			_findclose(hFind);
		}

		//古いディレクトリの削除
		for (size_t i = 0; i < fname.size(); i++ ) {
			_snprintf(wkpath, sizeof(wkpath)-1, "%s\\log\\%s", dpath.c_str(), fname[i].c_str());
			bcl::Dir::Delete(wkpath);
		}

	};

};

} //bcl::
	
#endif
