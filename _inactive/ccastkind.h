#ifndef __CCASTKIND_H__
#define __CCASTKIND_H__

#include <vector>
#include <string>
#include <fstream>
#include "Split.h"
#include "systemconst.h"
//�\�����������
typedef struct {
	int  kno;                   //�ǎ��
	std::string name;   //�ǎ�ʖ�
	std::string jname;	//�ǎ�ʖ�(�a��)
	int  max;                   //�ő�ǐ�
	int  titem;                 //�ϑ����ڐ�
	int  ditem;                 //���W�v���ڐ�
	int  bitem;                 //���Z��b���ڐ�
	int	 page;					//�y�[�W��
	int  cnowkeep;				//�\�������ۑ�����
	int  cmtmkeep;				//�\�������ۑ�����
	int  chtmkeep;				//�\���莞�ۑ�����
	std::string memname;			//�\������������
	std::string fpath;		//�t�@�C���p�X
	sys_local_t		*local;
	sys_action_t	*action;
} sys_ckind_t;

class CCastKindInfo{
private:
	/********************************************************************
	  �@�\	�Fcast-kind_info.csv�擾
	  ����	�F
	  �ߒl	�F
	  ����	�F
	********************************************************************/
	int GetCastKindInfo(std::vector<sys_ckind_t> &ckind){
		sys_ckind_t buf;
		CSplit		sp;
		std::ifstream	fs;
		std::string		wk_line;

		//init
		ckind.clear();
		//�ǎ�ʏ��̓Ǎ�
		fs.open("../common_ini/cast_kind_info.csv");
		if (!fs.is_open()){
			return -1;
		}

		//�K�v�Ȃ��s��ǂݔ�΂�
		std::getline(fs, wk_line);
		std::getline(fs, wk_line);
		std::getline(fs, wk_line);
		std::getline(fs, wk_line);
		std::getline(fs, wk_line);
		std::getline(fs, wk_line);
		std::getline(fs, wk_line);

		while(!fs.eof()) {
			//�P�s�ǂݍ���
			std::getline(fs, wk_line);
			if (strlen(wk_line.c_str()) <= 0) continue;
			//�ǎ�ʔԍ��̔���
			sp.split(wk_line.c_str(), ",");
			//�ǎ�ʔԍ�
			buf.kno = CStr::stoi(sp.at(0));
			//�ǎ�ʖ�
			buf.name = sp.at(1);
			//�ǎ�ʖ��i�a���j
			buf.jname = sp.at(2);
			//�ő�ǐ�
			buf.max = CStr::stoi(sp.at(3));
			//�ϑ����ڐ�
			buf.titem = CStr::stoi(sp.at(4));
			//���W�v���ڐ�
			buf.ditem = CStr::stoi(sp.at(5));
			//���Z��b���ڐ�
			buf.bitem = CStr::stoi(sp.at(6));
			//�y�[�W��
			buf.page = CStr::stoi(sp.at(7));
			//�\�������ۑ�����
			buf.cnowkeep = CStr::stoi(sp.at(8));
			//�\�������ۑ�����
			buf.cmtmkeep = CStr::stoi(sp.at(9));
			//�\���莞�ۑ�����
			buf.chtmkeep = CStr::stoi(sp.at(10));
			//�\������������
			buf.memname = sp.at(11);
			//�t�@�C���p�X
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
	[����] sb_getSysKindInfo
	[�@�\] �ǊǗ����擾
	*************************************************************/
	void sb_getSysKindInfo(const char *dpath, int procID, int unitID);
};


#endif //__CCASTKIND_H__
