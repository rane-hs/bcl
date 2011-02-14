#ifndef __CCASTKIND_H__
#define __CCASTKIND_H__

#include <vector>
#include <string>
#include <fstream>
#include "Split.h"
#include "systemconst.h"
//予測メモリ情報
typedef struct {
	int  kno;                   //局種別
	std::string name;   //局種別名
	std::string jname;	//局種別名(和名)
	int  max;                   //最大局数
	int  titem;                 //観測項目数
	int  ditem;                 //日集計項目数
	int  bitem;                 //演算基礎項目数
	int	 page;					//ページ数
	int  cnowkeep;				//予測現況保存期間
	int  cmtmkeep;				//予測正時保存期間
	int  chtmkeep;				//予測定時保存期間
	std::string memname;			//予測メモリ名称
	std::string fpath;		//ファイルパス
	sys_local_t		*local;
	sys_action_t	*action;
} sys_ckind_t;

class CCastKindInfo{
private:
	/********************************************************************
	  機能	：cast-kind_info.csv取得
	  引数	：
	  戻値	：
	  説明	：
	********************************************************************/
	int GetCastKindInfo(std::vector<sys_ckind_t> &ckind){
		sys_ckind_t buf;
		CSplit		sp;
		std::ifstream	fs;
		std::string		wk_line;

		//init
		ckind.clear();
		//局種別情報の読込
		fs.open("../common_ini/cast_kind_info.csv");
		if (!fs.is_open()){
			return -1;
		}

		//必要ない行を読み飛ばす
		std::getline(fs, wk_line);
		std::getline(fs, wk_line);
		std::getline(fs, wk_line);
		std::getline(fs, wk_line);
		std::getline(fs, wk_line);
		std::getline(fs, wk_line);
		std::getline(fs, wk_line);

		while(!fs.eof()) {
			//１行読み込み
			std::getline(fs, wk_line);
			if (strlen(wk_line.c_str()) <= 0) continue;
			//局種別番号の判定
			sp.split(wk_line.c_str(), ",");
			//局種別番号
			buf.kno = CStr::stoi(sp.at(0));
			//局種別名
			buf.name = sp.at(1);
			//局種別名（和名）
			buf.jname = sp.at(2);
			//最大局数
			buf.max = CStr::stoi(sp.at(3));
			//観測項目数
			buf.titem = CStr::stoi(sp.at(4));
			//日集計項目数
			buf.ditem = CStr::stoi(sp.at(5));
			//演算基礎項目数
			buf.bitem = CStr::stoi(sp.at(6));
			//ページ数
			buf.page = CStr::stoi(sp.at(7));
			//予測現況保存期間
			buf.cnowkeep = CStr::stoi(sp.at(8));
			//予測正時保存期間
			buf.cmtmkeep = CStr::stoi(sp.at(9));
			//予測定時保存期間
			buf.chtmkeep = CStr::stoi(sp.at(10));
			//予測メモリ名称
			buf.memname = sp.at(11);
			//ファイルパス
			buf.fpath = sp.at(12);

			ckind.push_back(buf);
		}
		fs.close();

		return 0;
	};
public:
	std::vector<sys_ckind_t> ckind_data;
	int GetInfo(){
		return GetCastKindInfo(ckind_data);
	};
	/*************************************************************
	[名称] sb_getSysKindInfo
	[機能] 局管理情報取得
	*************************************************************/
	void sb_getSysKindInfo(const char *dpath, int procID, int unitID);
};


#endif //__CCASTKIND_H__
