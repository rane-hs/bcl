#ifndef __CCASTKIND_H__
#define __CCASTKIND_H__

#include <vector>
#include <string>
#include <fstream>
#include "Split.h"
#include "systemconst.h"
//\ͺξρ
typedef struct {
	int  kno;                   //ΗνΚ
	std::string name;   //ΗνΚΌ
	std::string jname;	//ΗνΚΌ(aΌ)
	int  max;                   //ΕεΗ
	int  titem;                 //ΟͺΪ
	int  ditem;                 //ϊWvΪ
	int  bitem;                 //ZξbΪ
	int	 page;					//y[W
	int  cnowkeep;				//\ͺ»΅ΫΆϊΤ
	int  cmtmkeep;				//\ͺ³ΫΆϊΤ
	int  chtmkeep;				//\ͺθΫΆϊΤ
	std::string memname;			//\ͺΌΜ
	std::string fpath;		//t@CpX
	sys_local_t		*local;
	sys_action_t	*action;
} sys_ckind_t;

class CCastKindInfo{
private:
	/********************************************************************
	  @\	Fcast-kind_info.csvζΎ
	  ψ	F
	  ίl	F
	  ΰΎ	F
	********************************************************************/
	int GetCastKindInfo(std::vector<sys_ckind_t> &ckind){
		sys_ckind_t buf;
		CSplit		sp;
		std::ifstream	fs;
		std::string		wk_line;

		//init
		ckind.clear();
		//ΗνΚξρΜΗ
		fs.open("../common_ini/cast_kind_info.csv");
		if (!fs.is_open()){
			return -1;
		}

		//KvΘ’sπΗέςΞ·
		std::getline(fs, wk_line);
		std::getline(fs, wk_line);
		std::getline(fs, wk_line);
		std::getline(fs, wk_line);
		std::getline(fs, wk_line);
		std::getline(fs, wk_line);
		std::getline(fs, wk_line);

		while(!fs.eof()) {
			//PsΗέέ
			std::getline(fs, wk_line);
			if (strlen(wk_line.c_str()) <= 0) continue;
			//ΗνΚΤΜ»θ
			sp.split(wk_line.c_str(), ",");
			//ΗνΚΤ
			buf.kno = CStr::stoi(sp.at(0));
			//ΗνΚΌ
			buf.name = sp.at(1);
			//ΗνΚΌiaΌj
			buf.jname = sp.at(2);
			//ΕεΗ
			buf.max = CStr::stoi(sp.at(3));
			//ΟͺΪ
			buf.titem = CStr::stoi(sp.at(4));
			//ϊWvΪ
			buf.ditem = CStr::stoi(sp.at(5));
			//ZξbΪ
			buf.bitem = CStr::stoi(sp.at(6));
			//y[W
			buf.page = CStr::stoi(sp.at(7));
			//\ͺ»΅ΫΆϊΤ
			buf.cnowkeep = CStr::stoi(sp.at(8));
			//\ͺ³ΫΆϊΤ
			buf.cmtmkeep = CStr::stoi(sp.at(9));
			//\ͺθΫΆϊΤ
			buf.chtmkeep = CStr::stoi(sp.at(10));
			//\ͺΌΜ
			buf.memname = sp.at(11);
			//t@CpX
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
	[ΌΜ] sb_getSysKindInfo
	[@\] ΗΗξρζΎ
	*************************************************************/
	void sb_getSysKindInfo(const char *dpath, int procID, int unitID);
};


#endif //__CCASTKIND_H__
