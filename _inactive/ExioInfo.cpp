///////////////////////////////////////////////////////////////////////////
//
//  �y�ؑ����h�Џ��V�X�e���|���u�ԓ��o�͏��N���X
//
////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#pragma warning( disable : 4786 )   // C4786��Warning�𖳎�
#include "ExioInfo.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include "Split.h"
#include "Str.h"

M__USING_BCL

/********************************************************************
*	�����F
*	�ߒl�F
*	�����F
********************************************************************/
CExioInfo::CExioInfo()
{
	dp.clear();
	in_dp.clear();
	out_dp.clear();
}

/********************************************************************
*	�@�\�F�f�X���N�^
*	�����F
*	�ߒl�F
*	�����F
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
*	�@�\�F���u�ԓ��o�͏��ǂݍ��ݏ���
*	�����Ffpath				[IN]  �t�@�C���p�X
*	�ߒl�F����
        �F  1 = �X�V����
		�F  0 = �X�V�Ȃ�
		�F -1 = �t�@�C���p�X���m�t�k�k
		�F -2 = �t�@�C�������݂��Ȃ�
		�F -3 = �t�@�C���̃I�[�v���Ɏ��s
		�F -4 = �����G���[
*	�����F
********************************************************************/
int CExioInfo::get_file(const char *fpath)
{
	struct _stat		sts;
	exio_info_data_t	wk;
	bcl::CSplit				sp;
	std::ifstream			fs;
	std::string				dt;
	char				st[1024];

	// �p�����[�^�̊m�F
	if (fpath == NULL) {
		if (file_path.empty() == true)
			return -1;
	} else {
		if (file_path.compare(fpath) != 0)
			upd_time = 0;
		file_path = fpath;
	}

	// �t�@�C���̊m�F
	if ( _stat(file_path.c_str(), &sts) != 0)
		return -2;

	// �t�@�C���̍X�V�Ȃ�
	if (upd_time == sts.st_mtime)
		return 0;

	// �t�@�C���X�V�����̎擾
	upd_time = sts.st_mtime;

	// ������
	if (dp.size() > 0)
		dp.clear();
	memset(&info, 0, sizeof(info));

	// ���u�ԓ��o�͏��̓ǂݍ���
	fs.open(file_path.c_str());
	if (fs.is_open() == false)
		return -3;

	try {
		//�P�s��
		fs.getline(st, sizeof(st));
		if (st[0] != '\0') {
			sp.split(st, ",");
			// �������ԍ�
			dt = sp.at(3);
			hd.officeNo = bcl::CStr::stoi(dt);
			// ��MPORT
			dt = sp.at(5);
			hd.r_port = bcl::CStr::stol(dt);
			// �P���l�o��
			dt = sp.at(10);
			hd.ioflg_1m = bcl::CStr::stoi(dt);
		}
		// �Q�s��
		fs.getline(st, sizeof(st));
		if (st[0] != '\0') {
			sp.split(st, ",");
			// ���u�ԍ�
			dt = sp.at(3);
			hd.unitNo = bcl::CStr::stoi(dt);
			// ���M��PORT
			dt = sp.at(5);
			hd.s_port = bcl::CStr::stol(dt);
			// ���O�^�C���P��START
			dt = sp.at(7);
			hd.rag_1m.st = bcl::CStr::stol(dt);
			// ���O�^�C���P��END
			dt = sp.at(8);
			hd.rag_1m.et = bcl::CStr::stol(dt);
			// �莞�o��
			dt = sp.at(10);
			hd.ioflg_hm = bcl::CStr::stoi(dt);
		}
		// �R�s��
		fs.getline(st, sizeof(st));
		if (st[0] != '\0') {
			sp.split(st, ",");
			// ���H�ԍ�
			dt = sp.at(3);
			hd.houroNo = bcl::CStr::stoi(dt);
			// ���M��h�o
			hd.s_ip = sp.at(5);
			// ���O�^�C�����莞START
			dt = sp.at(7);
			hd.rag_hm.st = bcl::CStr::stol(dt);
			// ���O�^�C�����莞END
			dt = sp.at(8);
			hd.rag_hm.et = bcl::CStr::stol(dt);
			// ���W�v�o��
			dt = sp.at(10);
			hd.ioflg_dy = bcl::CStr::stoi(dt);
		}
		// �S�s��
		fs.getline(st, sizeof(st));
		if (st[0] != '\0') {
			sp.split(st, ",");
			// ��������
			hd.myOfficeName = sp.at(3);
			// ���M�掖������
			hd.toOfficeName = sp.at(5);
			// ���O�^�C�����W�vSTART
			dt = sp.at(7);
			hd.rag_dy.st = bcl::CStr::stol(dt);
			// ���O�^�C�����W�vEND
			dt = sp.at(8);
			hd.rag_dy.et = bcl::CStr::stol(dt);
		}
		// �T�s��
		fs.getline(st, sizeof(st));
		if (st[0] != '\0') {
			sp.split(st, ",");
			// ���u��
			hd.myUnitName = sp.at(3);
			// ���M�摕�u��
			hd.toUnitName = sp.at(5);
		}
		// �U�s��
		fs.getline(st, sizeof(st));
		// �V�s��
		fs.getline(st, sizeof(st));

		while (fs.eof() == false) {
			// �P�s�ǂݍ���
			fs.getline(st, sizeof(st));
			if (st[0] == '\0')
				continue;

			memset(&wk, 0, sizeof(wk));
			sp.split(st, ",");

			// ���o�͎�ʃR�[�h
			dt = sp.at(0);
			wk.io_kind = bcl::CStr::stoi(dt);
			if ((wk.io_kind != EXIO_KIND_IN) && (wk.io_kind != EXIO_KIND_OUT))
				continue;
			// ���o�͋ǔ�
			dt = sp.at(1);
			wk.io_sno = bcl::CStr::stoi(dt);
			// ���o�͍���
			dt = sp.at(2);
			wk.io_ino = bcl::CStr::stoi(dt);
			// ���o�͊Ǘ��������ԍ�
			dt = sp.at(3);
			wk.io_office = bcl::CStr::stoi(dt);
			// ���o�̓f�[�^��ʔԍ�
			dt = sp.at(4);
			wk.io_data = bcl::CStr::stoi(dt);
			// �{���R�[�h
			dt = sp.at(5);
			wk.code = bcl::CStr::stoi(dt);
			// �t�@�C�����
			dt = sp.at(6);
			wk.filetype = bcl::CStr::stoi(dt);
			// �ǎ�ʃR�[�h
			dt = sp.at(7);
			wk.kno = bcl::CStr::stoi(dt);
			if (wk.kno <= 0)
				continue;
			// �����ǔ�
			dt = sp.at(8);
			wk.sno = bcl::CStr::stoi(dt);
			if (wk.sno <= 0)
				continue;
			// �f�[�^���ڏ���
			dt = sp.at(9);
			wk.ino = bcl::CStr::stoi(dt);
			// �X�P�[���␳�R�[�h
			dt = sp.at(10);
			wk.scale = bcl::CStr::stoi(dt);
			// �f�[�^�␳���
			dt = sp.at(11);
			wk.conversion = bcl::CStr::stoi(dt);
			// �\��
			dt = sp.at(12);
			wk.reserve2 = bcl::CStr::stoi(dt);

			// �o�^
			dp.push_back(wk);
		}
	}
	catch (...) {
		fs.close();
		return -4;
	}
	// �t�@�C���N���[�Y
	fs.close();
	return 1;
}

/********************************************************************
*	�@�\�F���o�͏��쐬����
*	�����FpKind				[IN]  �ǎ�ʌn���
*	�ߒl�F
*	�����F
********************************************************************/
int CExioInfo::setup()
{
	int	i;
	int	n;

	n = dp.size();
	if (n > 0) {
		// ������
		if (in_dp.size() > 0)
			in_dp.clear();
		if (out_dp.size() > 0)
			out_dp.clear();

		// ���o�͏�񖈂ɐU�蕪����
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

