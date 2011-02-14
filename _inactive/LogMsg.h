
#ifndef _CLOG_MSG_H_
#define _CLOG_MSG_H_


#include <string>
#include <stdio.h>

#ifdef  __cplusplus
extern "C" {
#endif

#include "./clib/timeDll.h"
#include "./clib/logDll.h"

#ifdef  __cplusplus
}
#endif


//���O�o��
#define DG_LOG_ERR          0           //�G���[
#define DG_LOG_INFO         1           //�ڍ�
#define DG_LOG_WARN         2           //�x��
#define DG_LOG_DEBUG        3           //�f�o�b�O
#define DG_LOG_NON          5           //���O�ɂ͏o�͂��Ȃ�


#define MAX_SIZE_KIND       64          //���O��ʖ��T�C�Y
#define MAX_SIZE_MSG        1024        //���O���b�Z�[�W�T�C�Y
#define MAX_SIZE_LOGFILE    1000000     //���O�t�@�C���T�C�Y�iByte�j





/********************************************************************
  �N���X�FCLogMsg
  �@�\  �F���O���b�Z�[�W�o�̓N���X
  ����  �F
********************************************************************/
class CLogMsg {
public:

	int    office_no;               //�������ԍ�
	int    unit_no;                 //���u�ԍ�
	int    horo_no;                 //���H�ԍ�
	std::string exe_name;                //�\�t�g����



	/********************************************************************
	  �֐�  �FCLogMsg
	  �@�\  �F�R���X�g���N�^
	  ����  �F
	  �߂�l�F
	  ����  �F
	********************************************************************/
	CLogMsg();

	/********************************************************************
	  �֐�  �Finit
	  �@�\  �F������
	  ����  �Fmy_name           [IN]  �\�t�g����
	        �Foffice            [IN]  �������ԍ�
	        �Funit              [IN]  ���u�ԍ�
	        �Fhoro              [IN]  ���H�ԍ�
	  �߂�l�F
	  ����  �F
	********************************************************************/
	int init(const char *my_name, int office, int unit, int horo);

	/********************************************************************
	  �֐�  �Fput_msg
	  �@�\  �F���b�Z�[�W�o��
	  ����  �Ftyp               [IN]  ���b�Z�[�W�^�C�v
	        �Fknd               [IN]  �����
	        �Fbuf               [IN]  �o�̓��b�Z�[�W
	        �F...               [IN]  �p�����[�^
	  �߂�l�F
	  ����  �F
	********************************************************************/
	void put_msg(int typ, const char *knd, const char *buf, ...);

	/********************************************************************
	  �֐�  �Fdbg_typ
	  �@�\  �F���b�Z�[�W�^�C�v�ݒ�i�f�o�b�O�p�j
	  ����  �Fflg               [IN]  �f�o�b�O�t���O
	        �Flv                [IN]  �f�o�b�O���x��
	        �Ftyp               [IN]  ���b�Z�[�W�^�C�v
	  �߂�l�F
	  ����  �F
	********************************************************************/
	int dbg_typ(int flg, int lv, int typ);
};


#endif
