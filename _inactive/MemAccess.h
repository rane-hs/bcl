/********************************************************************
  �@�\    �F���L�������A�N�Z�X
  �X�V�����F2002/04/10  �V�K�쐬
          �F2002/04/27  ���ڒP�ʂ̏������݊֐��p�~
          �F2002/05/13  �����������E�ŐV��������ǉ�
          �F2002/06/13  ���ڒP�ʂ��ǖ��ɂ܂Ƃ߂ď������ފ֐���ǉ�
          �F2002/06/28  �ǎ�ʏ����������Ǘ������Ɏ����Ƃɂ���
          �F2002/07/03  �|�C���^���m�t�k�k�̏ꍇ�̔��������
          �F2002/08/22  �P���E���莞�̒��ł̍ŐV�����擾�֐���ǉ�
          �F2002/08/30  ���ڒP�ʏ����݊֐��ɂčŏ��̋ǂ�����������Ȃ������C��
          �F2002/09/09  �����������̍X�V�������������܂�Ȃ������C��
          �F2002/10/15  selectDataMemory�֐���tim�̃G���[�����ǉ�
          �F2003/05/29  ����f�[�^�ł��莞�ԂŃt���O���ُ�ȍ��ڂ͏������܂Ȃ����ɂȂ���
          �F----------  ����ȍ~��Ver 1.3  ----------
          �F2003/06/10  ���L�������̐錾���@��ύX
          �F2003/06/23  ���ɍX�V����Ă���ǂ͏㏑�����Ȃ��֐���ǉ�
          �F2003/08/06  �R�����g�̏C��
********************************************************************/

#ifndef _MEMACCESS_H_
#define _MEMACCESS_H_

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "./include/bousai.h"
#include "./include/sharedMemDll.h"
#include "./include/timeDll.h"
#include "./include/lockMutex.h"
#include "systemConst.h"


#ifdef  __cplusplus
extern "C" {
#endif


//�G���[�ԍ�
#define ERR_MEM_SUCCESS  0
#define ERR_MEM_MEMORY  -1      //�������͍쐬����Ă��Ȃ�
#define ERR_MEM_MP      -2      //�������Ǘ��̈悪�m�t�k�k
#define ERR_MEM_KP      -3     //�ǎ�ʏ��Ǘ��̈悪�m�t�k�k
#define ERR_MEM_TYPE    -4      //�������^�C�v���ُ�
#define ERR_MEM_KNO     -5      //�ǎ�ʔԍ����͈͊O
#define ERR_MEM_TIME    -6      //�f�[�^�������w�肳��Ă��Ȃ�
#define ERR_MEM_DATA    -7      //�������݃f�[�^���m�t�k�k
#define ERR_MEM_SNO     -8      //�ǔԍ����͈͊O
#define ERR_MEM_ITEM    -9      //���ڔԍ����͈͊O
#define ERR_MEM_ALLOC   -10     //�̈�m�ێ��s


/********************************************************************
  �\���́F���ڏ������ݗv��
  ����  �Fkno           �ǎ��
        �Fsno           �ǔԍ�
        �Fino           ����
        �Fdt            �f�[�^
********************************************************************/
typedef struct tagRequestData {
	int kno;                        //�ǎ��
	int sno;                        //�ǔԍ�
	int ino;                        //���ڔԍ�
	bf_data_t dt;                   //�f�[�^
} mem_reqdata_t;

/********************************************************************
  �\���́F�f�[�^�������X�V����
  ����  �Fyy            �N
        �Fmm            ��
        �Fdd            ��
        �Fhh            ��
        �Fnn            ��
********************************************************************/
typedef struct tagObserveTime {
	short yy;                       //�N
	char  mm;                       //��
	char  dd;                       //��
	char  hh;                       //��
	char  nn;                       //��
} mem_obstime_t;

/********************************************************************
  �\���́F�f�[�^���������(�P��)
  ����  �Fr             �X�V���
        �Fd             �f�[�^
        �Ftyp           �f�[�^�������^�C�v
        �Fkno           �ǎ�ʔԍ�
        �Fmax           �ő�ǐ�
        �Fitem          ���ڐ�
********************************************************************/
#define MEM_TYPE_INPUT_1M   1       //���̓f�[�^(�P��)
#define MEM_TYPE_INPUT_HM   2       //���̓f�[�^(���莞)
#define MEM_TYPE_DATA_1M    3       //���Z�f�[�^(�P��)
#define MEM_TYPE_DATA_HM    4       //���Z�f�[�^(���莞)
#define MEM_TYPE_DAILY      5       //���Z�f�[�^(���W�v)
#define MEM_TYPE_NOW        6       //�ŐV�f�[�^
#define MEM_TYPE_NOW_1M     7       //�P�������f�[�^
#define MEM_TYPE_NOW_HM     8       //���莞�����f�[�^
typedef struct tagDataMemory {
	sharedMem_t r;                  //�X�V���
	sharedMem_t d;                  //�f�[�^
	int         typ;                //�f�[�^�������^�C�v
	int         kno;                //�ǎ�ʔԍ�
	int         max;                //�ő�ǐ�
	int         item;               //���ڐ�
} mem_datamem_t;

/********************************************************************
  �\���́F�f�[�^���������(�P���)
  ����  �Fmin           �P���f�[�^
        �Ftim           ���莞�f�[�^
        �Fday           ���W�v�f�[�^(���̓f�[�^�������ł͖��g�p)
        �Fnow           �ŐV�f�[�^
        �Fnow_1m        �P�������f�[�^
        �Fnow_hm        ���莞�����f�[�^
********************************************************************/
typedef struct tagMemoryKind {
//2003.06.10 �ǉ��i��������j----------------------------------------
//���L�������̐錾���@��ύX
	sharedMem_t   mp;               //���L���������
//2003.06.10 �ǉ��i�����܂Łj----------------------------------------
	mem_datamem_t min[10];          //�P���f�[�^
	mem_datamem_t tim[6];           //���莞�f�[�^
	mem_datamem_t day;              //���W�v�f�[�^
	mem_datamem_t now;              //�ŐV�f�[�^
	mem_datamem_t now_1m;           //�P�������f�[�^
	mem_datamem_t now_hm;           //���莞�����f�[�^
} mem_kind_t;

/********************************************************************
  �\���́F�f�[�^�������Ǘ����
  ����  �Ftmem          ���̓f�[�^������
        �Fdmem          ���Z�f�[�^������
        �FhLockInput    ���̓f�[�^���������b�N�p
        �FhLockData     ���Z�f�[�^���������b�N�p(�P���E���莞)
        �FhLockDaily    ���Z�f�[�^���������b�N�p(���W�v)
        �Fkp            �ǎ�ʊǗ����
********************************************************************/
#define LOCK_NAME_INPUT "LOCK_TMEM"
#define LOCK_NAME_DATA  "LOCK_DMEM"
#define LOCK_NAME_DAILY "LOCK_DMEM_DAILY"
typedef struct tagMemoryManager {
	mem_kind_t tmem[99];            //���̓f�[�^������
	mem_kind_t dmem[99];            //���Z�f�[�^������
	HANDLE     hLockInput;          //���̓f�[�^���������b�N�p
	HANDLE     hLockData;           //���Z�f�[�^���������b�N�p(�P���E���莞)
	HANDLE     hLockDaily;          //���Z�f�[�^���������b�N�p(���W�v)
	kind_manager_t *kp;             //�ǎ�ʊǗ����
} mem_manager_t;



/********************************************************************
  �v���g�^�C�v�錾
********************************************************************/
void obsToTime(mem_obstime_t *p, time_t *tm);
void timeToObs(time_t tm, mem_obstime_t *p);

int  openDataMemory   (mem_manager_t *mp, kind_manager_t *kp);
void closeDataMemory  (mem_manager_t *mp);
void initDataMemoryHM (mem_manager_t *mp, int kno, char *p);
void initDataMemoryDay(mem_manager_t *mp, int kno, char *p);
int  writeDataMemory  (mem_manager_t *mp, int typ, int kno, time_t tim, char *dp);
int  writeDataMemoryS (mem_manager_t *mp, int typ, int kno, time_t tim, int sno, char *dp);
int  writeDataMemoryI (mem_manager_t *mp, int typ, time_t tim, int cnt, mem_reqdata_t *dp);
int  readDataMemoryI  (mem_manager_t *mp, int typ, time_t tim, int cnt, mem_reqdata_t *dp);

mem_datamem_t *selectDataMemory(mem_manager_t *mp, int typ, int kno, time_t tim);
mem_datamem_t *checkDataMemory (mem_manager_t *mp, int typ, int kno, time_t tim);
int getUpToDateTime(mem_manager_t *mp, int typ, int kno, time_t *tt);

/**********************************************************
  �֐�  �FwriteDataMemoryS_nOW
  �@�\  �F���L�f�[�^��������������(�P�ǑS����)
  ����  �Fmp                [I/O] �f�[�^���������
        �Ftyp               [IN]  �f�[�^�������^�C�v
        �Fkno               [IN]  �ǎ�ʔԍ�
        �Ftim               [IN]  �f�[�^����
        �Fsno               [IN]  �ǔԍ�
        �Fdp                [IN]  �������݃f�[�^
  �߂�l�F
  ����  �F���ɍX�V����Ă���ǂ͏㏑�����Ȃ�
**********************************************************/
int writeDataMemoryS_nOW(mem_manager_t *mp, int typ, int kno, time_t tim, int sno, char *dp);

/**********************************************************
  �֐�  �FwriteDataMemoryI_nOW
  �@�\  �F���L�f�[�^��������������(���ǂ�����)
  ����  �Fmp                [I/O] �f�[�^�������Ǘ����
        �Ftyp               [IN]  �f�[�^�������^�C�v
        �Ftim               [IN]  �f�[�^����
        �Fcnt               [IN]  �������ݍ��ڌ���
        �Fdp                [IN]  �������݃f�[�^
  �߂�l�F
  ����  �F���ɍX�V����Ă���ǂ͏㏑�����Ȃ�
**********************************************************/
int writeDataMemoryI_nOW(mem_manager_t *mp, int typ, time_t tim, int cnt, mem_reqdata_t *dp);


#ifdef  __cplusplus
}
#endif


#endif
