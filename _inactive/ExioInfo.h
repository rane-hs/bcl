///////////////////////////////////////////////////////////////////////////
//*	�@�\  �F���u�ԓ��o�͏��N���X
//  �X�V�����F
//          �F
//          �F
//          �F
//          �F
///////////////////////////////////////////////////////////////////////////
#ifndef _CEXIO_INFO_H_
#define _CEXIO_INFO_H_

#include <stdio.h>
#include <string>
#include <vector>
#include "bcl.h"

M__BEGIN_NAMESPACE_BCL

/********************************************************************
  �\���́F���O�^�C�����
  ����  �Fst					�J�n
        �Fet					�I��
********************************************************************/
typedef struct {
	long	st;										// �J�n
	long	et;										// �I��
} exio_info_ragtime_t;

/********************************************************************
  �\���́F���u�ԓ��o�͏��w�b�_��
  ����  �FofficeNo				�������ԍ�
        �FunitNo				���u�ԍ�
        �FhouroNo				���H�ԍ�
        �FmyOfficeName			����������
        �FmyUnitName			�����u��
        �FtoOfficeName			���M�掖������
        �FtoUnitName			���M�摕�u��
        �Fr_port				��M�|�[�g
        �Fs_port				���M��|�[�g
        �Fs_ip					���M��h�o
        �Frag_1m				�P���l���O�^�C��
        �Frag_hm				���莞���O�^�C��
        �Frag_dy				���W�v���O�^�C��
        �Fioflg_1m				�P���l�o�͗L��(0=�Ȃ�,1=�o��)
        �Fioflg_hm				���莞�o�͕��@(0=�n,  1=�ꊇ)
        �Fioflg_dy				���W�v�o�͗L��(0=�Ȃ�,1=�o��)
********************************************************************/
typedef struct {
	int					officeNo;					// �������ԍ�
	int					unitNo;						// ���u�ԍ�
	int					houroNo;					// ���H�ԍ�
	std::string			myOfficeName;				// ����������
	std::string				myUnitName;					// �����u��
	std::string				toOfficeName;				// ���M�掖������
	std::string				toUnitName;					// ���M�摕�u��
	long				r_port;						// ��M�|�[�g
	long				s_port;						// ���M��|�[�g
	std::string				s_ip;						// ���M��h�o
	exio_info_ragtime_t	rag_1m;						// �P���l���O�^�C��
	exio_info_ragtime_t	rag_hm;						// ���莞���O�^�C��
	exio_info_ragtime_t	rag_dy;						// ���W�v���O�^�C��
	int					ioflg_1m;					// �P���l�o�͗L��
	int					ioflg_hm;					// ���莞�o�͕��@
	int					ioflg_dy;					// ���W�v�o�͗L��
} exio_info_head_t;

/********************************************************************
  �\���́F���u�ԓ��o�͏��
  ����  �Fio_kind				���o�͎�ʃR�[�h    (1=����,2=�o��,�ȊO=�ݒ薳��)
        �Fio_sno				���o�͋ǔ�          (0�`65535  ,�ȊO=�ݒ薳��)
        �Fio_ino				���o�͍���          (0�`65535  ,�ȊO=�ݒ薳��)
        �Fio_office				���o�͊Ǘ��������ԍ�(1�`99999  ,�ȊO=�ݒ薳��)
        �Fio_data				���o�̓f�[�^��ʔԍ�(1�`999    ,�ȊO=�ݒ薳��)
        �Fcode					�{���R�[�h          (1=�{��    ,�ȊO=�P��)
        �Ffiletype				�t�@�C�����        (1=���W�v�l,�ȊO=���Ԓl)
        �Fkno					�ǎ�ʃR�[�h        (1�`6      ,�ȊO=�ݒ薳��)
        �Fsno					�����ǔԍ�          (1�`999    ,�ȊO=�ݒ薳��)
        �Fino					�f�[�^���ڏ���      (1�`99     ,�ȊO=�ݒ薳��)
        �Fscale					�X�P�[���␳�R�[�h  (1=10�{,2=100�{,3=1000�{,4=1/10�{,5=1/100�{,6=1/1000,�ȊO=1�{)
        �Fconversion			�f�[�^�ϊ����
        �Freserve2				�\��
********************************************************************/
#define EXIO_KIND_IN		1						// ����
#define EXIO_KIND_OUT		2						// �o��
#define EXIO_CODE_DOUBLE	1						// �{���R�[�h
#define EXIO_TYPE_DAILY		1						// ���W�v�l
#define EXIO_SCALE_10		1						// 10�{
#define EXIO_SCALE_100		2						// 100�{
#define EXIO_SCALE_1000		3						// 1000�{
#define EXIO_SCALE_D10		4						// 1/10�{
#define EXIO_SCALE_D100		5						// 1/100�{
#define EXIO_SCALE_D1000	6						// 1/1000�{
typedef struct {
	int		io_kind;								// ���o�͎�ʃR�[�h
	int		io_sno;									// ���o�͋ǔ�
	int		io_ino;									// ���o�͍���
	int		io_office;								// ���o�͊Ǘ��������ԍ�
	int		io_data;								// ���o�̓f�[�^��ʔԍ�
	int		code;									// �{���R�[�h
	int		filetype;								// �t�@�C�����
	int		kno;									// �ǎ�ʃR�[�h
	int		sno;									// �����ǔԍ�
	int		ino;									// �f�[�^���ڔԍ�
	int		scale;									// �X�P�[���␳�R�[�h
	long	conversion;								// �f�[�^�ϊ����
	int		reserve2;								// �\��
} exio_info_data_t;

/********************************************************************
  �\���́F���u�ԓ��o�͏��
  ����  �Fhd					���u�ԓ��o�̓w�b�_��
        �Fdp					���u�ԓ��o�̓f�[�^��
********************************************************************/
typedef struct {
	exio_info_head_t			hd;					// �w�b�_��
	std::vector<exio_info_data_t>	dp;					// �f�[�^��
} exio_info_t;

/********************************************************************
  �N���X��`
********************************************************************/
class CExioInfo :public exio_info_t{
public:
	std::string						file_path;			// �t�@�C���p�X
	time_t						upd_time;			// �t�@�C���X�V����
	std::vector<exio_info_data_t>	in_dp;				// ���͏��
	std::vector<exio_info_data_t>	out_dp;				// �o�͏��

	// �R���X�g���N�^
	CExioInfo();
	// �f�X���N�^
	~CExioInfo();

	// ���[�J�����ǂݍ���
	int get_file(const char *fpath = NULL);
	// ���o�͏��쐬����
	int setup();

};

M__END_NAMESPACE_BCL


#endif
