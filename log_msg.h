
#ifndef _MY_LOG_MSG_H_
#define _MY_LOG_MSG_H_

namespace bcl{


#include <string>
#include <iostream>
#include <fstream>

#include <bcl/bclstr.h>
#include <bcl/FileSystem.h>


//���O�o��
#define DG_LOG_ERR          0           //�G���[
#define DG_LOG_INFO         1           //�ڍ�
#define DG_LOG_WARN         2           //�x��
#define DG_LOG_DEBUG        3           //�f�o�b�O
#define DG_LOG_NON          5           //���O�ɂ͏o�͂��Ȃ�


#define MAX_SIZE_MSG     2048           //���b�Z�[�W�T�C�Y

/********************************************************************
  �@�\  �F���O���b�Z�[�W�o�̓N���X
********************************************************************/
class log_message {
public:

	bcl::str_t dpath;                   //�o�̓t�H���_
	bcl::str_t exe_name;                //�\�t�g����
	int    office_no;               //�������ԍ�
	int    unit_no;                 //���u�ԍ�
	int    horo_no;                 //���H�ԍ�
	int    level;                   //�o�̓��x��
	int    days;                    //�ۑ�����
	int    max_size_file;           //�ő�t�@�C���T�C�Y
	int    before_day;              //�O��t�@�C���폜��

	//ctor, dtor
	log_message():
		dpath(TEXT("")),exe_name(TEXT("")),
		office_no(0),unit_no(0),horo_no(0),level(0),days(0),
		max_size_file(10000000),before_day(0)
	{
	};
	~log_message(){};

	/********************************************************************
	  �@�\  �F�����ݒ�
	  ����  �Fdpath             [IN]  ���O�o�̓p�X
	        �Fexe_name          [IN]  �\�t�g����
	        �Foffice_no         [IN]  �������ԍ�
	        �Funit_no           [IN]  ���u�ԍ�
	        �Fhoro_no           [IN]  ���H�ԍ�
	        �Flevel             [IN]  �o�̓��x��
	  �߂�l�F
	  ����  �F
	********************************************************************/
	void init(const bcl::str_t dpath_, const bcl::str_t exe_name_, int office_no_, int unit_no_, int horo_no_, int level_)
	{
		dpath     = dpath_;     //���O�o�̓p�X
		exe_name  = exe_name_;  //�\�t�g����
		office_no = office_no_; //�������ԍ�
		unit_no   = unit_no_;   //���u�ԍ�
		horo_no   = horo_no_;   //���H�ԍ�
		level     = level_;     //�o�̓��x��
	};

	/********************************************************************
	  �֐�  �Fput_msg
	  �@�\  �F���b�Z�[�W�o��
	  ����  �Ftyp               [IN]  ���b�Z�[�W�^�C�v
	        �Flvl               [IN]  ���x��
	        �Fknd               [IN]  �����
	        �Fbuf               [IN]  �o�̓��b�Z�[�W
	        �F...               [IN]  �p�����[�^
	  �߂�l�F
	  ����  �F
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


		//�ψ�������
		va_start(ap, buf);

#ifdef _UNICODE
		ret = _vsnwprintf(wk, sizeof(wk)-1, buf, ap);
#else
		ret = _vsnprintf(wk, sizeof(wk)-1, buf, ap);
#endif
		va_end(ap);
		//�̈���I�[�o�[�����ꍇ�̏���
		wk[MAX_SIZE_MSG-1] = '\0';


		//���ݎ����̎擾
		const bcl::time24 nowTime = bcl::NowTime();
		//���O�o�̓t�H���_
		const bcl::str_t sday = bcl::format(TEXT("%04d/%02d/%02d"), nowTime.year24(), nowTime.month24(), nowTime.day24() );
		//���O�o�͎���
		const bcl::str_t stim = bcl::format(TEXT("%04d/%02d/%02d %02d:%02d:%02d"), nowTime.year24(), nowTime.month24(), nowTime.day24(), nowTime.hour24(), nowTime.minute(), nowTime.second());

		//�����
		sknd = knd;

		//���x������
		if ( level < lvl ) {
			wk_typ = DG_LOG_NON;
		} else {
			wk_typ = typ;
		}

		//�ڍ� 
		switch( wk_typ ) {
		case DG_LOG_ERR :
			styp = TEXT("[�G���[]");
			break;
		case DG_LOG_INFO :
			styp = TEXT("[ �ڍ� ]");
			break;
		case DG_LOG_WARN :
			styp = TEXT("[ �x�� ]");
			break;
		case DG_LOG_NON :
			styp = TEXT("[      ]");
			break;
		default :
			styp = TEXT("[ ��� ]");
			break;
		}

		//���b�Z�[�W�ҏW
		if ( sknd.empty() == true ) {
			msg = stim + TEXT(" ") + styp + TEXT(" ") + wk;
		} else {
			msg = stim + TEXT(" ") + styp + TEXT(" [") + sknd + TEXT("] ") + wk;
		}

		//�W���o��
		
#ifdef _UNICODE
		wprintf(TEXT("%s\n"), msg.c_str());
		//���O�o�̓p�X�ݒ�
		if ( sknd.empty() == true ) {
			ret = _snwprintf(fname, sizeof(fname)-1, TEXT("%s\\log\\%s_%02d%02d%02d.log"), dpath.c_str(), exe_name.c_str(), office_no, unit_no, horo_no);
		} else {
			ret = _snwprintf(fname, sizeof(fname)-1, TEXT("%s\\log\\%s\\%s_%s_%02d%02d%02d.log"), dpath.c_str(), sday.c_str(), exe_name.c_str(), sknd.c_str(), office_no, unit_no, horo_no);
		}
#else
		printf(TEXT("%s\n"), msg.c_str());
		//���O�o�̓p�X�ݒ�
		if ( sknd.empty() == true ) {
			ret = _snprintf(fname, sizeof(fname)-1, TEXT("%s\\log\\%s_%02d%02d%02d.log"), dpath.c_str(), exe_name.c_str(), office_no, unit_no, horo_no);
		} else {
			ret = _snprintf(fname, sizeof(fname)-1, TEXT("%s\\log\\%s\\%s_%s_%02d%02d%02d.log"), dpath.c_str(), sday.c_str(), exe_name.c_str(), sknd.c_str(), office_no, unit_no, horo_no);
		}
#endif

		//�̈���I�[�o�[�����ꍇ
		if ( ret < 0 ) {
			return;
		}

		//���O�o��
		if ( wk_typ == DG_LOG_NON ) {
			//printf("%s\n", msg.c_str());
		} else {
			//printf("%s\n", msg.c_str());
			write_log(fname, msg.c_str());
		}
	}



	/********************************************************************
	  �֐�  �Fput_msg
	  �@�\  �F���b�Z�[�W�o��
	  ����  �Ftyp               [IN]  ���b�Z�[�W�^�C�v
	        �Flvl               [IN]  ���x��
	        �Fknd               [IN]  �����
	        �Fcode              [IN]  �G���[�R�[�h
	  �߂�l�F
	  ����  �F
	********************************************************************/
	void put_msg(int typ, int lvl, const bcl::str_t knd, int code)
	{
		LPVOID lpMsgBuf;
		bcl::basic_split<bcl::str_t> sp;
		bcl::str_t wk;

		//�V�X�e�����b�Z�[�W�擾
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			code,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // ����̌���
			(LPTSTR) &lpMsgBuf,
			0,
			NULL);

		//���O�o��
		sp.tokenize((bcl::char_t *)lpMsgBuf, TEXT("\n\r"));
		for (size_t i = 0; i < sp.size(); i++ ) {
			wk = sp.at(i);
			if ( wk.empty() == false ) {
				put_msg(typ, lvl, knd, TEXT("Code(%d)�F%s"), code, wk.c_str());
			}
		}

		//�̈�̉��
		LocalFree(lpMsgBuf);

	};

	/********************************************************************
	  �֐�  �Fwrite_log
	  �@�\  �F���O�o��
	  ����  �Ffpath             [IN]  �t�@�C���p�X
	        �Fmsg               [IN]  �o�̓��b�Z�[�W
	  �߂�l�F
	  ����  �F
	********************************************************************/
	int write_log(const bcl::char_t *fpath, const bcl::char_t *msg)
	{

		//�t�@�C���p�X�̊m�F
#ifdef _UNICODE
		if ( wcslen(fpath) < 1) {	return 0;		}
#else
		if ( strlen(fpath) < 1) {	return 0;		}
#endif

		//�t�H���_�̊m�F
		if ( bcl::Dir::IsExist(fpath) == false ) {
			bcl::Dir::Make(fpath);
		}


		//�t�@�C���T�C�Y�̎擾
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
			//�t�@�C���T�C�Y���傫���Ȃ����Ȃ�
			if ( sts.st_size > max_size_file ) {
				cnt = 0;
				idx = 1;
				bcl::char_t wkpath[_MAX_PATH];
				bcl::char_t drive[_MAX_DRIVE];
				bcl::char_t dir[_MAX_DIR];
				bcl::char_t fname[_MAX_FNAME];
				bcl::char_t ext[_MAX_EXT];				//�p�X�̕���


#ifdef _UNICODE
				_wsplitpath(fpath, drive, dir, fname, ext);
				while ( cnt < 100 ) {
					//�t�@�C���p�X�̐ݒ�
					_snwprintf(wkpath, sizeof(wkpath)-1, TEXT("%s%s%s_%d%s"), drive, dir, fname, idx++, ext);
#else
				_splitpath(fpath, drive, dir, fname, ext);
				while ( cnt < 100 ) {
					//�t�@�C���p�X�̐ݒ�
					_snprintf(wkpath, sizeof(wkpath)-1, TEXT("%s%s%s_%d%s"), drive, dir, fname, idx++, ext);
#endif
					//�̈���I�[�o�[�����ꍇ�̏���
					wkpath[_MAX_PATH-1] = '\0';
					if ( bcl::File::IsExist(wkpath) == false ) break;
					cnt++;
				}
				if ( cnt < 100 ) {
					//�t�@�C�����ύX
					bcl::File::Move(fpath, wkpath);
				}
			}
		}

		//�t�@�C���I�[�v��

#ifdef _UNICODE
		//ofstream��wchar�Ή����Ă��Ȃ����߁Abcl::narrow�����܂�
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
		//��������
#ifdef _UNICODE
		ofs << bcl::narrow(msg) << std::endl;
#else
		ofs << msg << std::endl;
#endif
		//�t�@�C���N���[�Y
		ofs.close();

		//���O�폜
		delete_log();

		return 0;
	};

	/********************************************************************
	  �֐�  �Fdelete_log
	  �@�\  �F���O�폜
	  ����  �F
	  �߂�l�F
	  ����  �F
	********************************************************************/
	void delete_log()
	{
		struct _finddata_t dt;
		std::vector<std::string> fname;
		char   wkpath[_MAX_PATH];
		std::string fpath;
		long hFind;

		//�폜�Ώۂ��`�F�b�N
		if ( days <= 0 ) {
			return;
		}

		//���ݎ����̎擾
		const bcl::time24 nowTime;
		const bcl::time24 delTime = nowTime - clx::days(days);

		//���t�����ς���Ă���Ȃ�
		if ( before_day == nowTime.day24() ) {
			return;
		}
		before_day = nowTime.day24();


		//�폜�Ώ۔N����
		const std::string sdate = bcl::format("%04d%02d%02d", delTime.year24(), delTime.month24(), delTime.day24());

		//�t�H���_���̃`�F�b�N
		_snprintf(wkpath, sizeof(wkpath)-1, "%s\\log\\*", dpath.c_str());
		hFind = _findfirst(wkpath, &dt);
		if ( hFind != -1 ) {
			do {
				if ( strcmp(dt.name, ".") == 0 || strcmp(dt.name, "..") == 0 ) {
					continue;
				}

				//�Â��f�B���N�g���̔���
				if ( strcmp(sdate.c_str(), dt.name) >= 0 ) {
					fname.push_back(dt.name);
				}

			} while ( _findnext(hFind, &dt) == 0 );
			_findclose(hFind);
		}

		//�Â��f�B���N�g���̍폜
		for (size_t i = 0; i < fname.size(); i++ ) {
			_snprintf(wkpath, sizeof(wkpath)-1, "%s\\log\\%s", dpath.c_str(), fname[i].c_str());
			bcl::Dir::Delete(wkpath);
		}

	};

};

} //bcl::
	
#endif
