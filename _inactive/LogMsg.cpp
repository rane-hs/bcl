#include "stdafx.h"
#pragma comment(lib, "lib/logDll.lib")

#include "CLogMsg.h"
using namespace std;



/********************************************************************
  �֐�  �FCLogMsg
  �@�\  �F�R���X�g���N�^
  ����  �F
  �߂�l�F
  ����  �F
********************************************************************/
CLogMsg::CLogMsg()
{
	exe_name  = "";                 //�\�t�g����
	office_no = 0;                  //�������ԍ�
	unit_no   = 0;                  //���u�ԍ�
	horo_no   = 0;                  //���H�ԍ�
}

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
int CLogMsg::init(const char *my_name, int office, int unit, int horo)
{
	exe_name  = my_name;            //�\�t�g����
	office_no = office;             //�������ԍ�
	unit_no   = unit;               //���u�ԍ�
	horo_no   = horo;               //���H�ԍ�

	return 0;
}

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
void CLogMsg::put_msg(int typ, const char *knd, const char *buf, ...)
{
	char msg[MAX_SIZE_MSG];
	char fname[_MAX_PATH];
	char wk[MAX_SIZE_MSG];
	char stim[24];
	char styp[24];
	char sknd[24];
	va_list ap;
	tm24_t tm24;


	//�ψ�������
	va_start(ap, buf);
	vsprintf(wk, buf, ap);
	va_end(ap);

	//���ݎ����̎擾
	dll_getNowTime24(&tm24);
	sprintf(stim, "%04d/%02d/%02d %02d:%02d:%02d ",
		tm24.tm_year, tm24.tm_mon, tm24.tm_mday,
		tm24.tm_hour, tm24.tm_min, tm24.tm_sec);

	//�����
	if (knd == NULL)
		sknd[0] = '\0';
	else if (knd[0] == '\0')
		sknd[0] = '\0';
	else
		sprintf(sknd, "[%s] ", knd);

	//�ڍ� 
	switch( typ ){
	case DG_LOG_ERR :
		sprintf(styp, "[�G���[] ");
		break;
	case DG_LOG_INFO :
		sprintf(styp, "[ �ڍ� ] ");
		break;
	case DG_LOG_WARN :
		sprintf(styp, "[ �x�� ] ");
		break;
	case DG_LOG_NON :
		printf("%s[      ] %s%s\n", stim, sknd, wk);
		return;
	default :
		sprintf(styp, "[ ��� ] ");
		break;
	}

	//���b�Z�[�W�o��
	sprintf(msg, "%s%s%s%s", stim, styp, sknd, wk);
	printf("%s\n", msg);

	//���O�o�̓p�X�ݒ�
	if (knd == NULL)
		sprintf(fname, "..\\%s\\log\\%s_%02d%02d%02d.log", exe_name.c_str(), exe_name.c_str(), office_no, unit_no, horo_no);
	else if (knd[0] == '\0')
		sprintf(fname, "..\\%s\\log\\%s_%02d%02d%02d.log", exe_name.c_str(), exe_name.c_str(), office_no, unit_no, horo_no);
	else
		sprintf(fname, "..\\%s\\log\\%s_%s_%02d%02d%02d.log", exe_name.c_str(), exe_name.c_str(), knd, office_no, unit_no, horo_no);

	//���O�o��
	dll_writelog(fname, MAX_SIZE_LOGFILE, strlen(msg), msg);
}


/********************************************************************
  �֐�  �Fdbg_typ
  �@�\  �F���b�Z�[�W�^�C�v�ݒ�i�f�o�b�O�p�j
  ����  �Fflg               [IN]  �f�o�b�O�t���O
        �Flv                [IN]  �f�o�b�O���x��
        �Ftyp               [IN]  ���b�Z�[�W�^�C�v
  �߂�l�F
  ����  �F
********************************************************************/
int CLogMsg::dbg_typ(int flg, int lv, int typ)
{
	if (flg >= lv) {
		return typ;
	} else {
		return DG_LOG_NON;
	}
}
