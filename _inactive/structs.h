#include "bcl.h"

M__BEGIN_NAMESPACE_BCL

/********************************************************************
  �\���́F�ŐV�������
  ����  �Fyear          �N(1�`20XX)
        �Fmonth         ��(1�`12)
        �Fday           ��(1�`31)
        �Fhour          ��(0�`24)
        �Fminute        ��(0�`59)
********************************************************************/
typedef struct tagNowTime {
	short year;                     //�N
	char  month;                    //��
	char  day;                      //��
	char  hour;                     //��
	char  minute;                   //��
} bf_nowtime_t;

/********************************************************************
  �\���́F�X�V���
  ����  �Fmonth         ��(1�`12)
        �Fday           ��(1�`31)
        �Fhour          ��(0�`24)
        �Fminute        ��(0�`59)
********************************************************************/
typedef union tagObserverTime {
	{
		char month;                     //��
		char day;                       //��
		char hour;                      //��
		char minute;                    //��
	}
	unsigned char data[4];
	operator=(long dat){*(&dat[0]) = (unsigned char *)data;};
} bf_obstime_t;

/********************************************************************
  �\���́F�I�t�Z�b�g���
  ����  �Fcnt           �f�[�^���̎����ǐ�
        �Fsize          �����ǂP�ǂ�����̑��o�C�g��
********************************************************************/
typedef struct tagFileOffset {
  unsigned short cnt;               //�f�[�^���̎����ǐ�
  unsigned short size;              //�����ǂP�ǂ�����̑��o�C�g��
} bf_offset_t;

/********************************************************************
  �\���́F�C���f�b�N�X���
  ����  �Foffset        �I�t�Z�b�g���
        �Fst            �ǂ̎����Ǐ���(0 = �����)
********************************************************************/
typedef struct tagFileIndex {
  bf_offset_t    offset;            //�I�t�Z�b�g���
  unsigned short *st;              //�ǂ̎����Ǐ���
} bf_index_t;

/********************************************************************
  �\���́F�f�[�^
  ����  �Fdt            �f�[�^
        �Ff             �t���O
********************************************************************/
typedef struct tagFileData {
  long          dt;                 //�f�[�^
  unsigned char f[4];               //�t���O
  unsigned char f1(){return f[0];};
  unsigned char f2(){return f[1];};
  unsigned char f3(){return f[2];};
  unsigned char f4(){return f[3];};
} bf_data_t;

/********************************************************************
  �\���́F����w�߃t�@�C�����{���
  ����  �Forder         ����w�ߎ��{�t���O�i1=���{����,0=���{�ς�,��=�w�ߍ��ڈُ�j
        �Freserve1      �\��
        �Freserve2      �\��
        �Freserve3      �\��
        �Ftim           ����w�ߎ��{����
********************************************************************/
typedef struct tagSystemControl {
	char order;                     //����w�ߎ��{�t���O
	char reserve1;                  //�\��
	char reserve2;                  //�\��
	char reserve3;                  //�\��
	bf_obstime_t tim;               //����w�ߎ��{����
} bf_syscontrol_t;

M__END_NAMESPACE_BCL
