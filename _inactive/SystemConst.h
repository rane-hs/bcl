/********************************************************************
  �@�\    �F�V�X�e���萔�ǂݍ���
  �X�V�����F2002/04/09  �V�K�쐬
          �F2002/06/04  CSV�t�@�C������P�s�ǂݍ��ފ֐���ǉ�
          �F2002/06/11  ���u�ԒʐM���ǂݍ��݊֐��̒ǉ�  
          �F2002/06/27  �ǎ�ʏ������L��������
          �F2002/07/25  ���ʉ��Z�萔�̒�`�A�_�����ʉ��Z�����\���̂̕ύX
          �F2002/08/26  �P���f�[�^�̕ۑ�������ǉ�         
          �F2002/10/29  �J�ʉ��Z������`�̒ǉ�
          �F2002/11/19  �t�@�C���ǂݍ��ݎ��ɍX�V�������r����悤�ɕύX
          �F2003/02/27  �T�[�o�������t�@�C���Ƀf�[�^����M������ԂƉߋ��f�[�^������Ԃ�ǉ�
          �F2003/06/12  �R�����g�̏C��
		  �F            �`�`�` �ȍ~Ver1.3 �`�`�`
          �F2003/06/04  �ϐ�A�C���A�H���A���A�r���A�����ǂ̒ǉ�
          �F2003/07/02  �����������Z������`�̒ǉ�
          �F2003/08/21  �������������ǖ��̏��A�Ǖ⑫���̏��A�ǃR�[�h��������
          �F2003/08/22  �V�X�e���萔�������f�ɑΉ������֐���ǉ�
          �F2003/10/10  �ϐ�A�C�����Z�萔�A�C���A�H���x���l�̕ύX
          �F2003/11/07  �R�����g�̏C��
          �F2003/12/19  csvread()�ōŏ��̂P������NULL�̏ꍇ�ɑΉ�
          �F            �֐�csvread()����csvread_n()�֒u����
          �F            �֐�csvsplit()����csvsplit_n()�֒u����
		  �F2004/01/09  ���敽�ωJ�ʋǂ̒ǉ�
		  �F2004/01/14  �\�����ʋǂ̒ǉ�
********************************************************************/

#ifndef _SYSTEMCONST_H_
#define _SYSTEMCONST_H_

#include <windows.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>


#define SYS_ERR_SUCCESS         0       //����
#define SYS_ERR_FAILURE         -1      //�ُ�
#define SYS_ERR_POINTER         -10     //�|�C���^���m�t�k�k
#define SYS_ERR_SERVER_INFO     -11     //�T�[�o�������t�@�C�����ُ�
#define SYS_ERR_FILE_OPEN       -20     //�t�@�C���I�[�v�����s
#define SYS_ERR_SHARED_MEMORY   -30     //���L�������I�[�v�����s
#define SYS_ERR_ALLOC_MEMORY    -31     //�������擾���s
#define SYS_ERR_LOCK_HANDLE     -40     //���b�N�n���h���擾���s
#define SYS_ERR_LOCK_TIMEOUT    -41     //���b�N�^�C���A�E�g


#define SYS_MAX_LINE        2048        //CSV�t�@�C���P�s�̍ő�T�C�Y
#define SYS_MAX_KIND        99          //�ő�ǎ�ʐ�
#define SYS_MAX_MNO         999         //�ő�Ǘ��ԍ�

#define SYS_LOCK_TIMEOUT    5000        //���b�N�^�C���A�E�g�l(�~���b)
#define SYS_NAME_LOCK   "SYS_LOCK_KIND" //���b�N�p����
#define SYS_NAME_KIND   "SYS_MEM_KIND"  //���L��������



/********************************************************************
  �\���́F�T�[�o���
  ����  �FstrAppPath            �A�v���P�[�V�����p�X
        �FstrDataPath           �f�[�^�p�X
        �FstrMO                 �l�n�h���C�u
        �FintDays               �t�@�C���ۑ�����(��)
        �FintDays1M             �P���t�@�C���ۑ�����(��)
        �FintNowFile            �����t�@�C���쐬�L��(1=�쐬����,�ȊO=�쐬���Ȃ�)
        �FintBeFore             �f�[�^����M�������(��)
        �FintBeHind             �ߋ��f�[�^�������(��)
********************************************************************/
#define INI_NOW_MODE_CREATE 1       //�����t�@�C���쐬
typedef struct tagIniServer {
	char    strAppPath[_MAX_PATH];  //�A�v���P�[�V�����p�X
	char    strDataPath[_MAX_PATH]; //�f�[�^�p�X
	char    strMO[_MAX_DRIVE];      //�l�n�h���C�u
	int		intDays;                //�t�@�C���ۑ�����
	int     intDays1M;              //�P���t�@�C���ۑ�����
	int     intNowFile;             //�����t�@�C���쐬�L��
	int     intBeFore;              //�f�[�^����M�������
	int     intBeHind;              //�ߋ��f�[�^�������
	int     intDaysNow;				// �����t�@�C���ۑ�����
	int     intDaysCast;			// �\���t�@�C���ۑ�����
} ini_server_t;


/********************************************************************
  �\���́F�����u�ԍ�
  ����  �FintOfficeID           ���������ԍ�
        �FintUnitID             �����u�ԍ�
********************************************************************/
typedef struct tagSysMyNumber {
	int     intOfficeID;            //���������ԍ�
	int     intUnitID;              //�����u�ԍ�
} sys_mynumber_t;


/********************************************************************
  �\���́F�ǎ�ʏ��
  ����  �Fkno                   �ǎ��
        �Fname                  �ǎ�ʖ�(�f�B���N�g�����A�t�@�C�����̌��ɂȂ�)
        �Fjname                 �ǎ�ʖ�(�a��)
        �Fmax                   �ő�ǐ�
        �Ftitem                 �ϑ����ڐ�
        �Fditem                 ���W�v���ڐ�
        �Fbitem                 ���Z��b���ڐ�
********************************************************************/
#define LEN_KIND_NAME       16      //�ǎ�ʖ��T�C�Y
#define LEN_KIND_JNAME      32      //�ǎ��(�a��)�T�C�Y
typedef struct tagSysKind {
	int     kno;                    //�ǎ��
	char    name[LEN_KIND_NAME];    //�ǎ�ʖ�
	char    jname[LEN_KIND_JNAME];  //�ǎ�ʖ�(�a��)
	int     max;                    //�ő�ǐ�
	int     titem;                  //�ϑ����ڐ�
	int     ditem;                  //���W�v���ڐ�
	int     bitem;                  //���Z��b���ڐ�
} sys_kind_t;


/********************************************************************
  �\���́F�ǎ������
  ����  �Fsno                   �ǔԍ�            (1�`999,     �ȊO:�ݒ�l�Ȃ�)
        �Fentry                 �������          (1:����,     �ȊO:�����)
        �Fkei                   �n���            (1�`99,      �ȊO:�����)
        �Fcycle                 �f�[�^�ۑ�����    (1:1������,  �ȊO:10������)
        �Ftcalc                 ���Z���{���      (1:���Z����, �ȊO:���Z���Ȃ�)
        �Fdcalc                 ���W�v���Z���{���(1:���Z����, �ȊO:���Z���Ȃ�)
********************************************************************/
#define SYS_LOCAL_ENTRY_ON 1        //����
#define SYS_LOCAL_CYCLE_1M 1        //�P������
#define SYS_LOCAL_TCALC_IP 1        //�C�x���g�|�[�����O���Z���s��
#define SYS_LOCAL_TCALC_10 2        //10���������Z���s��
#define SYS_LOCAL_TCALC_01 3        //�P���������Z���s��
#define SYS_LOCAL_DCALC_DO 1        //���W�v���Z���s��
typedef struct tagSysLocal {
	int     sno;                    //�ǔԍ�
	int     entry;                  //�������
	int     kei;                    //�n���
	int     cycle;                  //�f�[�^�ۑ�����
	int     tcalc;                  //���Z���{���
	int     dcalc;                  //���W�v���Z���{���
	int     reserve1;               //�\��
	int     reserve2;               //�\��
} sys_local_t;


/********************************************************************
  �\���́F�ǉғ����
  ����  �Fsno                   �ǔԍ�
        �Faction                �ǉғ����(1:�ێ璆, 2:��, �ȊO:�ǎ������Ɉˑ�)
********************************************************************/
#define SYS_ACTION_ACTION_MAINTE 1  //�ێ璆
#define SYS_ACTION_ACTION_CLOSE  2  //��
typedef struct tagSysAction {
	int     sno;                    //�ǔԍ�
	int     action;                 //�ǉғ����
} sys_action_t;


/********************************************************************
  �\���́F�ǖ��̏��
  ����  �Fsno                   �ǔԍ�
        �Fmno1                  �Ǘ��ԍ��P
        �Fmno2                  �Ǘ��ԍ��Q
        �Fmno3                  �Ǘ��ԍ��R
        �Fmno4                  �Ǘ��ԍ��S
        �Facode                 �w��ϑ����R�[�h
        �Fpcode1                ��_�P�R�[�h
        �Fpcode2                ��_�Q�R�[�h
        �Fmcode                 �Ǘ��Ҏ�ʃR�[�h
        �Foffice_name           ������������
        �Fstation_name          �ϑ�����
        �Fvalley_name           ���於
        �Frsys_name             ���n�� river system
        �Friver_name            �͐얼
        �Freprese_name          ��\���ݖ� represe seat
        �Froute                 �H�����
        �Froute_name            �H����
        �Fsub_name1             �ϑ������i�T�u�P�j
        �Fsub_name2             �ϑ������i�T�u�Q�j
        �Fshorts_name           ���ǖ�
        �Fsabou_block           ���h�u���b�N��
        �Fpref_name             �s���{���� prefecture
        �Fcity_name             �Q�E�s��
        �Farea_name             ��E���E����
        �Faddr_name             �厚�E�n�ԓ���
        �Flat                   �ϑ����ܓx�i�x�jstation latitude
        �Flat_min               �ϑ����ܓx�i���j
        �Flat_sec               �ϑ����ܓx�i�b�j
        �Flon                   �ϑ����o�x�i�x�j
        �Flon_min               �ϑ����o�x�i���j
        �Flon_sec               �ϑ����o�x�i�b�j
        �Freserve1              �\��1
        �Freserve2              �\��2
********************************************************************/
#define SYS_NAME_ACODE_APPOINT  1   //�w��ϑ���
#define SYS_NAME_PCODE_STANDARD 1   //��_�ϑ���
#define SYS_NAME_MCODE_KASEN    1   //���͐��
#define SYS_NAME_MCODE_SABOU    2   //�����h��
#define SYS_NAME_MCODE_ROAD     3   //�����H��
#define SYS_NAME_MCODE_DAM      4   //���_����
#define SYS_NAME_MCODE_POST     5   //������
#define SYS_NAME_MCODE_MLIT     11  //���y��ʏ� Ministry of Land,Infrastructure and Transport
#define SYS_NAME_MCODE_JMA      12  //�C�ے� Japan Meteorological Agency
#define SYS_NAME_MCODE_CITY     13  //�s����
#define SYS_NAME_MCODE_PREF     14  //���� prefecture
#define SYS_NAME_MCODE_OTHER    15  //���̑�
#define SYS_NAME_ROUTE_NATIONAL 1   //���� national road
#define SYS_NAME_ROUTE_PREF     2   //��ʌ���
#define SYS_NAME_ROUTE_LOCAL    3   //��v�n����
typedef struct tagSysName {
	int     sno;                    //�ǔԍ�
	int     mno1;                   //�Ǘ��ԍ��P
	int     mno2;                   //�Ǘ��ԍ��Q
	int     mno3;                   //�Ǘ��ԍ��R
	int     mno4;                   //�Ǘ��ԍ��S
	int     acode;                  //�w��ϑ����R�[�h
	int     pcode1;                 //��_�P�R�[�h
	int     pcode2;                 //��_�Q�R�[�h
	int     mcode;                  //�Ǘ��Ҏ�ʃR�[�h
	char    office_name[16];        //������������
	char    station_name[16];       //�ϑ�����
	char    valley_name[16];        //���於
	char    rsys_name[16];          //���n��
	char    river_name[16];         //�͐얼
	char    represe_name[16];       //��\���ݖ�
	int     route;                  //�H�����
	char    route_name[16];         //�H����
	char    sub_name1[16];          //�ϑ������i�T�u�P�j
	char    sub_name2[16];          //�ϑ������i�T�u�Q�j
	char    shorts_name[16];        //���ǖ�
	char    sabou_block[16];        //���h�u���b�N��
	char    pref_name[16];          //�s���{����
	char    city_name[16];          //�Q�E�s��
	char    area_name[16];          //��E���E����
	char    addr_name[24];          //�厚�E�n�ԓ���
	int	    lat;                    //�ϑ����ܓx�i�x�j
	int     lat_min;                //�ϑ����ܓx�i���j
	double  lat_sec;                //�ϑ����ܓx�i�b�j
	int     lon;                    //�ϑ����o�x�i�x�j
	int     lon_min;                //�ϑ����o�x�i���j
	double  lon_sec;                //�ϑ����o�x�i�b�j
	int     reserve1;               //�\��1
	int     reserve2;               //�\��2
} sys_name_t;


/********************************************************************
  �\���́F�Ǖ⑫���̏��
  ����  �Fsno                   �ǔԍ�
        �Faltitude              �W��
        �Frever_dis             �͌�����̋���
        �Fmember                �ϑ�����
        �Ftelno                 �d�b�ԍ�
        �Fctl_name              �Ǘ��c�̖� 
        �Fsensor_code           �Z���T�[��ʃR�[�h
        �Fkind_code             �ϑ���ʃR�[�h
        �Fstart_year            �ϑ��J�n���t�i�N�j
        �Fstart_mon             �ϑ��J�n���t�i���j
        �Fstart_day             �ϑ��J�n���t�i���j
        �Fobs_area_suibou       �Ď��Ώۋ�於�́i���h�j
        �Fobs_area_sabo         �Ď��Ώۋ�於�́i���h�j
        �Fobs_area_road         �Ď��Ώۋ�於�́i���H�j
        �Freserve1              �\��1
        �Freserve2              �\��2
********************************************************************/
typedef struct tagSysOptName {
	int     sno;                    //�ǔԍ�
	double  altitude;               //�W��
	long    rever_dis;              //�͌�����̋���
	char    member[16];             //�ϑ�����
	char    tel_no[16];             //�d�b�ԍ�
	char    ctl_name[32];           //�Ǘ��c�̖� 
	char    sensor_code[32];        //�Z���T�[��ʃR�[�h
	char    kind_code[16];          //�ϑ���ʃR�[�h
	int     start_year;             //�ϑ��J�n���t�i�N�j
	int     start_mon;              //�ϑ��J�n���t�i���j
	int     start_day;              //�ϑ��J�n���t�i���j
	char    obs_area_suibou[32];    //�Ď��Ώۋ�於�́i���h�j
	char    obs_area_sabo[32];      //�Ď��Ώۋ�於�́i���h�j
	char    obs_area_road[32];      //�Ď��Ώۋ�於�́i���H�j
	short   reserve1;               //�\��1
	short   reserve2;               //�\��2
} sys_optname_t;


/********************************************************************
  �\���́F�ǃR�[�h���
  ����  �Fsno                   �ǔԍ�
        �Farea_no               �G���A�ԍ��i���j
        �Foffice_no             �������ԍ��i���j
        �Friver_sys_code        ���n��R�[�h�i���jriver system  area
        �Friver_code            �͐�R�[�h�i���j
        �Fobservatory_no        �ϑ����ԍ��i���j
        �Fpref_code             �s���{���R�[�h�i���j
        �Fcity_code             �s�����R�[�h�i���j
        �Freserve1              �\��1
        �Freserve2              /�\��2
********************************************************************/
typedef struct tagSysCode {
	int     sno;                    //�ǔԍ�
	int     area_no;                //�G���A�ԍ��i���j
	int     office_no;              //�������ԍ��i���j
	int     river_sys_code;         //���n��R�[�h�i���j
	int     river_code;             //�͐�R�[�h�i���j
	int     observatory_no;         //�ϑ����ԍ��i���j
	int     pref_code;              //�s���{���R�[�h�i���j
	int     city_code;              //�s�����R�[�h�i���j
	int     reserve1;               //�\��1
	int     reserve2;               //�\��2
} sys_code_t;


/********************************************************************
  �\���́F�Ǎ\�����P
  ����  �Fsno                   �ǔԍ�
        �F
        �F    �`�`�`�ȉ����`�`�`
        �F
********************************************************************/
typedef struct tagSysComp1 {
	int     sno;                    //�ǔԍ�
	int     office_no;              //�������ԍ�
	//�Z���T�[�P
	char    s1_sensor[24];          //�Z���T�[�P
	char    s1_form[24];            //�`���P
	char    s1_type[24];            //�^���P
	char    s1_date_1[32];          //���t�P�|�P
	char    s1_date_2[32];          //���t�P�|�Q
	char    s1_date_3[32];          //���t�P�|�R
	char    s1_introduce[24];       //�����Ǝ҂P
	char    s1_intro_tel[32];       //�A����P
	char    s1_chacker[24];         //�_���Ǝ҂P
	char    s1_chk_tel[32];         //�A����P
	char    s1_note_1[32];          //���l�P�|�P
	char    s1_note_2[32];          //���l�P�|�Q
	char    s1_note_3[32];          //���l�P�|�R
	char    s1_note_4[32];          //���l�P�|�S
	//�Z���T�[�Q
	char	s2_sensor[24];          //�Z���T�[�Q
	char	s2_form[24];            //�`���Q
	char	s2_type[24];            //�^���Q
	char    s2_date_1[32];          //���t�Q�|�P
	char    s2_date_2[32];          //���t�Q�|�Q
	char    s2_date_3[32];          //���t�Q�|�R
	char	s2_introduce[24];       //�����Ǝ҂Q
	char	s2_intro_tel[32];       //�A����Q
	char	s2_chacker[24];         //�_���Ǝ҂Q
	char	s2_chk_tel[32];         //�A����Q
	char    s2_note_1[32];          //���l�Q�|�P
	char    s2_note_2[32];          //���l�Q�|�Q
	char    s2_note_3[32];          //���l�Q�|�R
	char    s2_note_4[32];          //���l�Q�|�S
	//�Z���T�[�R
	char    s3_sensor[24];          //�Z���T�[�R
	char    s3_form[24];            //�`���R
	char    s3_type[24];            //�^���R
	char    s3_date_1[32];          //���t�R�|�P
	char    s3_date_2[32];          //���t�R�|�Q
	char    s3_date_3[32];          //���t�R�|�R
	char    s3_introduce[24];       //�����Ǝ҂R
	char    s3_intro_tel[32];       //�A����R
	char    s3_chacker[24];	        //�_���Ǝ҂R
	char    s3_chk_tel[32];         //�A����R
	char    s3_note_1[32];          //���l�R�|�P
	char    s3_note_2[32];          //���l�R�|�Q
	char    s3_note_3[32];          //���l�R�|�R
	char    s3_note_4[32];          //���l�R�|�S
	//�Z���T�[�S
	char    s4_sensor[24];          //�Z���T�[�S
	char    s4_form[24];            //�`���S
	char    s4_type[24];            //�^���S
	char    s4_date_1[32];          //���t�S�|�P
	char    s4_date_2[32];          //���t�S�|�Q
	char    s4_date_3[32];          //���t�S�|�R
	char    s4_introduce[24];		//�����Ǝ҂S
	char    s4_intro_tel[32];       //�A����S
	char    s4_checker[24];         //�_���Ǝ҂S
	char    s4_chk_tel[32];         //�A����S
	char    s4_note_1[32];          //���l�S�|�P
	char    s4_note_2[32];          //���l�S�|�Q
	char    s4_note_3[32];          //���l�S�|�R
	char    s4_note_4[32];          //���l�S�|�S
	//�Z���T�[�T
	char    s5_sensor[24];          //�Z���T�[�T
	char    s5_form[24];            //�`���T
	char    s5_type[24];            //�^���T
	char    s5_date_1[32];          //���t�T�|�P
	char    s5_date_2[32];          //���t�T�|�Q
	char    s5_date_3[32];          //���t�T�|�R
	char    s5_introduce[24];		//�����Ǝ҂T
	char    s5_intro_tel[32];       //�A����T
	char    s5_checker[24];         //�_���Ǝ҂T
	char    s5_chk_tel[32];         //�A����T
	char    s5_note_1[32];          //���l�T�|�P
	char    s5_note_2[32];          //���l�T�|�Q
	char    s5_note_3[32];          //���l�T�|�R
	char    s5_note_4[32];          //���l�T�|�S

	//�L�^�v�P
	char    r1_recorder[24];        //�L�^�v�P
	char    r1_form[24];            //�`���P
	char    r1_type[24];            //�^���P
	char    r1_begin[24];           //�^�p�J�n���P
	char    r1_update[24];          //�X�V�N�����P
	char    r1_introduce[24];       //�����Ǝ҂P
	char    r1_intro_tel[32];       //�A����P
	char    r1_checker[24];         //�_���Ǝ҂P
	char    r1_chk_tel[32];         //�A����P
	char    r1_reel_day[32];        //�L�^�v�̊������P
	char    r1_breakdown[32];       //�L�^�v�̌̏ᒷ�������L���P
	char    r1_note_1[32];          //���l�P�|�P
	char    r1_note_2[32];          //���l�P�|�Q
	//�L�^�v�Q
	char    r2_recorder[24];        //�L�^�v�Q
	char    r2_form[24];            //�`���Q
	char    r2_type[24];            //�^���Q
	char    r2_begin[24];           //�^�p�J�n���Q
	char    r2_update[24];          //�X�V�N�����Q
	char    r2_introduce[24];       //�����Ǝ҂Q
	char    r2_intro_tel[32];       //�A����Q
	char    r2_checker[24];         //�_���Ǝ҂Q
	char    r2_chk_tel[32];         //�A����Q
	char    r2_reel_day[32];        //�L�^�v�̊������Q
	char    r2_breakdown[32];       //�L�^�v�̌̏ᒷ�������L���Q
	char    r2_note_1[32];          //���l�Q�|�P
	char    r2_note_2[32];          //���l�Q�|�Q
	//�L�^�v�R
	char    r3_recorder[24];        //�L�^�v�R
	char    r3_form[24];            //�`���R
	char    r3_type[24];            //�^���R
	char    r3_begin[24];           //�^�p�J�n���R
	char    r3_update[24];          //�X�V�N�����R
	char    r3_introduce[24];       //�����Ǝ҂R
	char    r3_intro_tel[32];       //�A����R
	char    r3_checker[24];         //�_���Ǝ҂R
	char    r3_chk_tel[32];         //�A����R
	char    r3_reel_day[32];        //�L�^�v�̊������R
	char    r3_breakdown[32];       //�L�^�v�̌̏ᒷ�������L���R
	char    r3_note_1[32];          //���l�R�|�P
	char    r3_note_2[32];          //���l�R�|�Q
	//�L�^�v�S
	char    r4_recorder[24];        //�L�^�v�S
	char    r4_form[24];            //�`���S
	char    r4_type[24];            //�^���S
	char    r4_begin[24];           //�^�p�J�n���S
	char    r4_update[24];          //�X�V�N�����S
	char    r4_introduce[24];       //�����Ǝ҂S
	char    r4_intro_tel[32];       //�A����S
	char    r4_checker[24];         //�_���Ǝ҂S
	char    r4_chk_tel[32];         //�A����S
	char    r4_reel_day[32];        //�L�^�v�̊������S
	char    r4_breakdown[32];       //�L�^�v�̌̏ᒷ�������L���S
	char    r4_note_1[32];          //���l�S�|�P
	char    r4_note_2[32];          //���l�S�|�Q
	//�L�^�v�T
	char    r5_recorder[24];        //�L�^�v�T
	char    r5_form[24];            //�`���T
	char    r5_type[24];            //�^���T
	char    r5_begin[24];           //�^�p�J�n���T
	char    r5_update[24];          //�X�V�N�����T
	char    r5_introduce[24];       //�����Ǝ҂T
	char    r5_intro_tel[32];       //�A����T
	char    r5_checker[24];         //�_���Ǝ҂T
	char    r5_chk_tel[32];         //�A����T
	char    r5_reel_day[32];        //�L�^�v�̊������T
	char    r5_breakdown[32];       //�L�^�v�̌̏ᒷ�������L���T
	char    r5_note_1[32];          //���l�T�|�P
	char    r5_note_2[32];          //���l�T�|�Q
	//�`�����
	char    t_transmitte[24];       //�`�����
	char    t_form[24];             //�`��
	char    t_type[24];             //�^��
	char    t_begin[24];            //�^�p�J�n��
	char    t_update[24];           //�X�V�N����
	char    t_introduce[24];        //�����Ǝ�
	char    t_intro_tel[32];        //�A����
	char    t_checker[24];          //�_���Ǝ�
	char    t_chk_tel[32];          //�A����
	char    t_snd_hz[32];           //���M���g��
	char    t_rcv_hz[32];           //��M���g��
	//�󒆐�
	char    a_air_line[32];         //�󒆐�
	char    a_form[32];             //�ʐM����
	char    a_note_1[32];           //���l
	char    a_note_2[32];           //���l
	//�d��
	char    p_power[24];            //�d��
	char    p_form[24];             //�`��
	char    p_type[24];             //�^��
	char    p_begin[24];            //�^�p�J�n��
	char    p_update[24];           //�X�V�N����
	char    p_introduce[24];        //�����Ǝ�
	char    p_intro_tel[32];        //�A����
	char    p_checker[24];          //�_���Ǝ�
	char    p_chk_tel[32];          //�A����
	char    p_name[32];             //�g�p�d��
	char    p_capacity[32];         //�~�d�r�e��
	char    p_note_1[32];           //���l
	char    p_note_2[32];           //���l
	//�ǎ�
	int     o_office;               //�ǎ�
	char    o_kind[24];             //�ǎɎ��
	char    o_site[24];             //�ǎɕ~�n
	char    o_space[24];            //�ǎɌ���
	char    o_others[24];           //���̑�
	char    o_tenant[24];           //�ؒn�l��
	char    o_rent[24];             //�ؒn��
	char    o_utility_pole[32];     //�d������
	char    o_rec_keep[32];         //�L�^�̗L���ۊǏꏊ
	int     o_reserve1;             //�\��
	int     o_reserve2;             //�\��
	//�e�����[�^�ݒu��
	char    tm_telemeter[64];       //�ݒu��
	int     tm_year;                //�e�����[�^�J�n���t�i�N�j
	int     tm_mon;                 //�e�����[�^�J�n���t�i���j
	int     tm_day;                 //�e�����[�^�J�n���t�i���j
	double  tm_back_basehi;         //�w��n�n�Ս�
	char    tm_sign[24];            //�ʐ��W
	int     tm_reserve1;            //�\��
	int     tm_reserve2;            //�\��
} sys_comp1_t;

/********************************************************************
  �\���́F�Ǎ\�����Q
  ����  �Fsno                   �ǔԍ�
        �F
        �F    �`�`�`�ȉ����`�`�`
        �F
********************************************************************/
typedef struct tagSysComp2 {
	int     sno;                    //�ǔԍ�
	int     office_no;              //�������ԍ�
	//�����������u�P
	char    u1_unit[24];            //�����������u�P
	char    u1_form[24];            //�`���P
	char    u1_type[24];            //�^���P
	char    u1_date_1[24];          //���t�P�|�P
	char    u1_date_2[24];          //���t�P�|�Q
	char    u1_date_3[24];          //���t�P�|�R
	char    u1_introduce[24];       //�����Ǝ҂P
	char    u1_intro_tel[32];       //�A����P
	char    u1_checker[24];         //�_���Ǝ҂P
	char    u1_chk_tel[32];         //�A����P
	char    u1_note_1[32];          //���l�P�|�P
	char    u1_note_2[32];          //���l�P�|�Q
	char    u1_note_3[32];          //���l�P�|�R
	char    u1_note_4[32];          //���l�P�|�S
	//�����������u�Q
	char    u2_unit[24];            //�����������u�Q
	char    u2_form[24];            //�`���Q
	char    u2_type[24];            //�^���Q
	char    u2_date_1[24];          //���t�Q�|�P
	char    u2_date_2[24];          //���t�Q�|�Q
	char    u2_date_3[24];          //���t�Q�|�R
	char    u2_introduce[24];       //�����Ǝ҂Q
	char    u2_intro_tel[32];       //�A����Q
	char    u2_checker[24];         //�_���Ǝ҂Q
	char    u2_chk_tel[32];         //�A����Q
	char    u2_note_1[32];          //���l�Q�|�P
	char    u2_note_2[32];          //���l�Q�|�Q
	char    u2_note_3[32];          //���l�Q�|�R
	char    u2_note_4[32];          //���l�Q�|�S
	//�����������u�R
	char    u3_unit[24];            //�����������u�R
	char    u3_form[24];            //�`���R
	char    u3_type[24];            //�^���R
	char    u3_date_1[24];          //���t�R�|�P
	char    u3_date_2[24];          //���t�R�|�Q
	char    u3_date_3[24];          //���t�R�|�R
	char    u3_introduce[24];       //�����Ǝ҂R
	char    u3_intro_tel[32];       //�A����R
	char    u3_checker[24];         //�_���Ǝ҂R
	char    u3_chk_tel[32];         //�A����R
	char    u3_note_1[32];          //���l�R�|�P
	char    u3_note_2[32];          //���l�R�|�Q
	char    u3_note_3[32];          //���l�R�|�R
	char    u3_note_4[32];          //���l�R�|�S
	//�����������u�S
	char    u4_unit[24];            //�����������u�S
	char    u4_form[24];            //�`���S
	char    u4_type[24];            //�^���S
	char    u4_date_1[24];          //���t�S�|�P
	char    u4_date_2[24];          //���t�S�|�Q
	char    u4_date_3[24];          //���t�S�|�R
	char    u4_introduce[24];       //�����Ǝ҂S
	char    u4_intro_tel[32];       //�A����S
	char    u4_checker[24];         //�_���Ǝ҂S
	char    u4_chk_tel[32];         //�A����S
	char    u4_note_1[32];          //���l�S�|�P
	char    u4_note_2[32];          //���l�S�|�Q
	char    u4_note_3[32];          //���l�S�|�R
	char    u4_note_4[32];          //���l�S�|�S
	//�����������u�T
	char    u5_unit[24];            //�����������u�T
	char    u5_form[24];            //�`���T
	char    u5_type[24];            //�^���T
	char    u5_date_1[24];          //���t�T�|�P
	char    u5_date_2[24];          //���t�T�|�Q
	char    u5_date_3[24];          //���t�T�|�R
	char    u5_introduce[24];       //�����Ǝ҂T
	char    u5_intro_tel[32];       //�A����T
	char    u5_checker[24];         //�_���Ǝ҂T
	char    u5_chk_tel[32];         //�A����T
	char    u5_note_1[32];          //���l�T�|�P
	char    u5_note_2[32];          //���l�T�|�Q
	char    u5_note_3[32];          //���l�T�|�R
	char    u5_note_4[32];          //���l�T�|�S
	//�����������u�U
	char    u6_unit[24];            //�����������u�U
	char    u6_form[24];            //�`���U
	char    u6_type[24];            //�^���U
	char    u6_date_1[24];          //���t�U�|�P
	char    u6_date_2[24];          //���t�U�|�Q
	char    u6_date_3[24];          //���t�U�|�R
	char    u6_introduce[24];       //�����Ǝ҂U
	char    u6_intro_tel[32];       //�A����U
	char    u6_checker[24];         //�_���Ǝ҂U
	char    u6_chk_tel[32];         //�A����U
	char    u6_note_1[32];          //���l�U�|�P
	char    u6_note_2[32];          //���l�U�|�Q
	char    u6_note_3[32];          //���l�U�|�R
	char    u6_note_4[32];          //���l�U�|�S
	//�����������u�V
	char    u7_unit[24];            //�����������u�V
	char    u7_form[24];            //�`���V
	char    u7_type[24];            //�^���V
	char    u7_date_1[24];          //���t�V�|�P
	char    u7_date_2[24];          //���t�V�|�Q
	char    u7_date_3[24];          //���t�V�|�R
	char    u7_introduce[24];       //�����Ǝ҂V
	char    u7_intro_tel[32];       //�A����V
	char    u7_checker[24];         //�_���Ǝ҂V
	char    u7_chk_tel[32];         //�A����V
	char    u7_note_1[32];          //���l�V�|�P
	char    u7_note_2[32];          //���l�V�|�Q
	char    u7_note_3[32];          //���l�V�|�R
	char    u7_note_4[32];          //���l�V�|�S
	//�����������u�W
	char    u8_unit[24];            //�����������u�W
	char    u8_form[24];            //�`���W
	char    u8_type[24];            //�^���W
	char    u8_date_1[24];          //���t�W�|�P
	char    u8_date_2[24];          //���t�W�|�Q
	char    u8_date_3[24];          //���t�W�|�R
	char    u8_introduce[24];       //�����Ǝ҂W
	char    u8_intro_tel[32];       //�A����W
	char    u8_checker[24];         //�_���Ǝ҂W
	char    u8_chk_tel[32];         //�A����W
	char    u8_note_1[32];          //���l�W�|�P
	char    u8_note_2[32];          //���l�W�|�Q
	char    u8_note_3[32];          //���l�W�|�R
	char    u8_note_4[32];          //���l�W�|�S
	//�����������u�X
	char    u9_unit[24];            //�����������u�X
	char    u9_form[24];            //�`���X
	char    u9_type[24];            //�^���X
	char    u9_date_1[24];          //���t�X�|�P
	char    u9_date_2[24];          //���t�X�|�Q
	char    u9_date_3[24];          //���t�X�|�R
	char    u9_introduce[24];       //�����Ǝ҂X
	char    u9_intro_tel[32];       //�A����X
	char    u9_checker[24];         //�_���Ǝ҂X
	char    u9_chk_tel[32];         //�A����X
	char    u9_note_1[32];          //���l�X�|�P
	char    u9_note_2[32];          //���l�X�|�Q
	char    u9_note_3[32];          //���l�X�|�R
	char    u9_note_4[32];          //���l�X�|�S
	//�����������u�P�O
	char    u10_unit[24];           //�����������u�P�O
	char    u10_form[24];           //�`���P�O
	char    u10_type[24];           //�^���P�O
	char    u10_date_1[24];         //���t�P�O�|�P
	char    u10_date_2[24];         //���t�P�O�|�Q
	char    u10_date_3[24];         //���t�P�O�|�R
	char    u10_introduce[24];      //�����Ǝ҂P�O
	char    u10_intro_tel[32];      //�A����P�O
	char    u10_checker[24];        //�_���Ǝ҂P�O
	char    u10_chk_tel[32];        //�A����P�O
	char    u10_note_1[32];         //���l�P�O�|�P
	char    u10_note_2[32];         //���l�P�O�|�Q
	char    u10_note_3[32];         //���l�P�O�|�R
	char    u10_note_4[32];         //���l�P�O�|�S
	//�L���P
	char    n1_notice_date[32];     //�L���P�N����
	char    n1_contents[64];        //�L���P���e
	char    n1_trader[24];          //�L���P�Ή��Ǝ�
	char    n1_note[32];            //�L���P���l
	char    n1_reserve1[32];        //�\��
	char    n1_reserve2[32];        //�\��
	//�L���Q
	char    n2_notice_date[32];     //�L���Q�N����
	char    n2_contents[64];        //�L���Q���e
	char    n2_trader[24];          //�L���Q�Ή��Ǝ�
	char    n2_note[32];            //�L���Q���l
	char    n2_reserve1[32];        //�\��
	char    n2_reserve2[32];        //�\��
	//�L���R
	char    n3_notice_date[32];     //�L���R�N����
	char    n3_contents[64];        //�L���R���e
	char    n3_trader[24];          //�L���R�Ή��Ǝ�
	char    n3_note[32];            //�L���R���l
	char    n3_reserve1[32];        //�\��
	char    n3_reserve2[32];        //�\��
	//�L���S
	char    n4_notice_date[32];     //�L���S�N����
	char    n4_contents[64];        //�L���S���e
	char    n4_trader[24];          //�L���S�Ή��Ǝ�
	char    n4_note[32];            //�L���S���l
	char    n4_reserve1[32];        //�\��
	char    n4_reserve2[32];        //�\��
	//�L���T
	char    n5_notice_date[32];     //�L���T�N����
	char    n5_contents[64];        //�L���T���e
	char    n5_trader[24];          //�L���T�Ή��Ǝ�
	char    n5_note[32];            //�L���T���l
	char    n5_reserve1[32];        //�\��
	char    n5_reserve2[32];        //�\��
	//�L���U
	char    n6_notice_date[32];     //�L���U�N����
	char    n6_contents[64];        //�L���U���e
	char    n6_trader[24];          //�L���U�Ή��Ǝ�
	char    n6_note[32];            //�L���U���l
	char    n6_reserve1[32];        //�\��
	char    n6_reserve2[32];        //�\��
	//�L���V
	char    n7_notice_date[32];     //�L���V�N����
	char    n7_contents[64];        //�L���V���e
	char    n7_trader[24];          //�L���V�Ή��Ǝ�
	char    n7_note[32];            //�L���V���l
	char    n7_reserve1[32];        //�\��
	char    n7_reserve2[32];        //�\��
	//�L���W
	char    n8_notice_date[32];     //�L���W�N����
	char    n8_contents[64];        //�L���W���e
	char    n8_trader[24];          //�L���W�Ή��Ǝ�
	char    n8_note[32];            //�L���W���l
	char    n8_reserve1[32];        //�\��
	char    n8_reserve2[32];        //�\��
	//�L���X
	char    n9_notice_date[32];     //�L���X�N����
	char    n9_contents[64];        //�L���X���e
	char    n9_trader[24];          //�L���X�Ή��Ǝ�
	char    n9_note[32];            //�L���X���l
	char    n9_reserve1[32];        //�\��
	char    n9_reserve2[32];        //�\��
	//�L���P�O
	char    n10_notice_date[32];    //�L���P�O�N����
	char    n10_contents[64];       //�L���P�O���e
	char    n10_trader[24];         //�L���P�O�Ή��Ǝ�
	char    n10_note[32];           //�L���P�O���l
	char    n10_reserve1[32];       //�\��
	char    n10_reserve2[32];       //�\��
} sys_comp2_t;


/********************************************************************
  �\���́F�ǊǗ����(�ǎ�ʖ�)
  ����  �Fkind          �ǎ�ʏ��|�C���^
        �Flocal         �ǎ������|�C���^    �i�擪�ǂւ̃|�C���^�j
        �Faction        �ǉғ����|�C���^    �i�擪�ǂւ̃|�C���^�j
        �Fname;         �ǖ��̏��|�C���^    �i�擪�ǂւ̃|�C���^�j
        �Fopt_name;     �Ǖ⑫���̏��|�C���^�i�擪�ǂւ̃|�C���^�j
        �Fcode;         �ǃR�[�h���|�C���^  �i�擪�ǂւ̃|�C���^�j
        �Fcomp1;        �Ǎ\�����|�C���^    �i�擪�ǂւ̃|�C���^�j
        �Fcomp2;        �Ǎ\�����|�C���^    �i�擪�ǂւ̃|�C���^�j
********************************************************************/
typedef struct tagKindInfo {
	sys_kind_t      *kind;          //�ǎ�ʏ��|�C���^
	sys_local_t     *local;         //�ǎ������|�C���^
	sys_action_t    *action;        //�ǉғ����|�C���^
	sys_name_t      *name;          //�ǖ��̏��|�C���^
	sys_optname_t   *optname;       //�Ǖ⑫���̏��|�C���^
	sys_code_t      *code;          //�ǃR�[�h���|�C���^
//	sys_comp1_t     *comp1;         //�Ǎ\�����|�C���^
//	sys_comp2_t     *comp2;         //�Ǎ\�����|�C���^
} kind_info_t;


/********************************************************************
  �\���́F�ǊǗ����
  ����  �Fmax           �ő�ǎ�ʔԍ�
        �FpKind         �ǎ�ʖ��̏��
        �Ftim           �ǂݍ��ݓ���
        �FhLock         ���b�N�p�n���h��
        �Fp             �����Ǘ����
********************************************************************/
typedef struct tagKindManager {
	int         max;                //�ő�ǎ�ʔԍ�
	kind_info_t *pKind;             //�ǎ�ʖ��̏��
	//�����Ǘ��p
	time_t      tim;                //�ǂݍ��ݓ���
	HANDLE      hLock;              //���b�N�p�n���h��
	sharedMem_t p;                  //�����Ǘ����
} kind_manager_t;


/********************************************************************
  �\���́F���O�^�C�����
  ����  �Fst            �J�n
        �Fet            �I��
********************************************************************/
typedef struct tagSysRagTime {
	long st;                        //�J�n
	long et;                        //�I��
} sys_ragtime_t;


/********************************************************************
  �\���́F���u�ԓ��o�͏��w�b�_��
  ����  �FofficeNo      �������ԍ�
        �FunitNo        ���u�ԍ�
        �FhouroNo       ���H�ԍ�
        �FmyOfficeName  ����������
        �FmyUnitName    �����u��
        �FtoOfficeName  ���M�掖������
        �FtoUnitName    ���M�摕�u��
        �Fr_port        ��M�|�[�g
        �Fs_port        ���M��|�[�g
        �Fs_ip          ���M��h�o
        �Frag_1m        �P���l���O�^�C��
        �Frag_hm        ���莞���O�^�C��
        �Frag_dy        ���W�v���O�^�C��
        �Fioflg_1m      �P���l�o�͗L��(0=�Ȃ�,1=�o��)
        �Fioflg_hm      ���莞�o�͕��@(0=�n,  1=�ꊇ)
        �Fioflg_dy      ���W�v�o�͗L��(0=�Ȃ�,1=�o��)
********************************************************************/
typedef struct tagSysExioHeader {
	int  officeNo;                  //�������ԍ�
	int  unitNo;                    //���u�ԍ�
	int  houroNo;                   //���H�ԍ�
	char myOfficeName[256];         //����������
	char myUnitName[256];           //�����u��
	char toOfficeName[256];         //���M�掖������
	char toUnitName[256];           //���M�摕�u��
	long r_port;                    //��M�|�[�g
	long s_port;                    //���M��|�[�g
	char s_ip[16];                  //���M��h�o
	sys_ragtime_t rag_1m;           //�P���l���O�^�C��
	sys_ragtime_t rag_hm;           //���莞���O�^�C��
	sys_ragtime_t rag_dy;           //���W�v���O�^�C��
	int  ioflg_1m;                  //�P���l�o�͗L��
	int  ioflg_hm;                  //���莞�o�͕��@
	int  ioflg_dy;                  //���W�v�o�͗L��
} sys_exiohead_t;


/********************************************************************
  �\���́F���u�ԓ��o�͏��
  ����  �Fio_kind       ���o�͎�ʃR�[�h    (1=����,2=�o��,�ȊO=�ݒ薳��)
        �Fio_sno        ���o�͋ǔ�          (0�`65535  ,�ȊO=�ݒ薳��)
        �Fio_ino        ���o�͍���          (0�`65535  ,�ȊO=�ݒ薳��)
        �Fio_office     ���o�͊Ǘ��������ԍ�(1�`99999  ,�ȊO=�ݒ薳��)
        �Fio_data       ���o�̓f�[�^��ʔԍ�(1�`999    ,�ȊO=�ݒ薳��)
        �Fcode          �{���R�[�h          (1=�{��    ,�ȊO=�P��)
        �Ffiletype      �t�@�C�����        (1=���W�v�l,�ȊO=���Ԓl)
        �Fkno           �ǎ�ʃR�[�h        (1�`6      ,�ȊO=�ݒ薳��)
        �Fsno           �����ǔԍ�          (1�`999    ,�ȊO=�ݒ薳��)
        �Fino           �f�[�^���ڏ���      (1�`99     ,�ȊO=�ݒ薳��)
        �Fscale         �X�P�[���␳�R�[�h  (1=10�{,2=100�{,3=1000�{,4=1/10�{,5=1/100�{,6=1/1000,�ȊO=1�{)
        �Freserve1      �\��
        �Freserve2      �\��
********************************************************************/
#define SYS_EXIO_KIND_IN     1      //����
#define SYS_EXIO_KIND_OUT    2      //�o��
#define SYS_EXIO_CODE_DOUBLE 1      //�{���R�[�h
#define SYS_EXIO_TYPE_DAILY  1      //���W�v�l
#define SYS_EXIO_SCALE_10    1      //10�{
#define SYS_EXIO_SCALE_100   2      //100�{
#define SYS_EXIO_SCALE_1000  3      //1000�{
#define SYS_EXIO_SCALE_D10   4      //1/10�{
#define SYS_EXIO_SCALE_D100  5      //1/100�{
#define SYS_EXIO_SCALE_D1000 6      //1/1000�{
typedef struct tagSysExio {
	int io_kind;                    //���o�͎�ʃR�[�h
	int io_sno;                     //���o�͋ǔ�
	int io_ino;                     //���o�͍���
	int io_office;                  //���o�͊Ǘ��������ԍ�
	int io_data;                    //���o�̓f�[�^��ʔԍ�
	int code;                       //�{���R�[�h
	int filetype;                   //�t�@�C�����
	int kno;                        //�ǎ�ʃR�[�h
	int sno;                        //�����ǔԍ�
	int ino;                        //�f�[�^���ڏ���
	int scale;                      //�X�P�[���␳�R�[�h
	int reserve1;                   //�\��
	int reserve2;                   //�\��
} sys_exio_t;


/********************************************************************
  �\���́F���u�ԓ��o�͏��
  ����  �Fcnt           �f�[�^����
        �Fhd            ���u�ԓ��o�̓w�b�_��
        �Fdp            ���u�ԓ��o�̓f�[�^��
********************************************************************/
typedef struct tagSysExioInfo {
	int            cnt;             //�f�[�^����
	sys_exiohead_t hd;              //�w�b�_��
	sys_exio_t     *dp;             //�f�[�^��
} sys_exioinfo_t;


/********************************************************************
  �\���́F�J�ʉ��Z����
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
#define SYS_RA_ENZAN_MODE_SA    1       //���h�n���͒l
#define SYS_RA_ENZAN_MODE_SATM  2       //���h�~�J�J�n�I�������̂݉��Z 2002.10.29

#define SYS_RA_ENZAN_KIND_RO    1       //�݉�(�A��)�J��
#define SYS_RA_ENZAN_KIND_R10   2       //10���J��
#define SYS_RA_ENZAN_KIND_RH    3       //�P���ԉJ��
#define SYS_RA_ENZAN_RTIME 	    12      //�݉��J�ʃ��Z�b�g���ԃf�t�H���g�l
#define SYS_RA_ENZAN_DTIME      3       //�A���J��(���H)���Z�b�g���ԃf�t�H���g�l
#define SYS_RA_ENZAN_STIME      24      //�~�J�I�����莞�ԃf�t�H���g�l
#define SYS_RA_ENZAN_HALF       24      //�J�ʔ������f�t�H���g�l
#define SYS_RA_ENZAN_HJUDG      4       //�ϋȓ_����J�ʃf�t�H���g�l
#define SYS_RA_ENZAN_YTIME      3       //�ϋȓ_�I�����莞�ԃf�t�H���g�l
typedef struct tagSysRainEnzan {
	short   sno;            //�����ǔ�
	char    mode;           //�J�ʉ��Z���� (1:���h�n���͒l, �ȊO:�ώZ�J��)
	char    kind;           //��M�J�ʎ�� (1:�݉�(�A��)�J��, 2:10���J��,
                            //              3:�P���ԉJ��, �ȊO:�J�E���^�l)
	short   rtime;          //�݉��J�ʃ��Z�b�g����       (1�`99, �ȊO:12)
	short   dtime;          //�A���J��(���H)���Z�b�g���� (1�`99, �ȊO:3)
	short   stime;          //�~�J�I�����莞��           (1�`99, �ȊO:24)
	short   half;           //�J�ʔ�����                 (1�`99, �ȊO:24)
	short   hjudg;          //�ϋȓ_����J��             (1�`99, �ȊO:4)
	short   ytime;          //�ϋȓ_�I�����莞��         (1�`99, �ȊO:3)
} sys_ra_enzan_t;


/********************************************************************
  �\���́F�J�ʈُ�l����
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
#define SYS_RA_KENTEI_S01   10          //�P���ώZ�J�ʏ���l�f�t�H���g�l
#define SYS_RA_KENTEI_S10   50          //10���ώZ�J�ʏ���l�f�t�H���g�l
#define SYS_RA_KENTEI_S60   200         //60���ώZ�J�ʏ���l�f�t�H���g�l
#define SYS_RA_KENTEI_R10   50          //10���J�ʏ���l�f�t�H���g�l
#define SYS_RA_KENTEI_R30   150         //30���J�ʏ���l�f�t�H���g�l
#define SYS_RA_KENTEI_R60   200         //60���J�ʏ���l�f�t�H���g�l
#define SYS_RA_KENTEI_R3H   600         //�R���ԉJ�ʏ���l�f�t�H���g�l
#define SYS_RA_KENTEI_R6H   990         //�U���ԉJ�ʏ���l�f�t�H���g�l
#define SYS_RA_KENTEI_R24H  990         //24���ԉJ�ʏ���l�f�t�H���g�l
#define SYS_RA_KENTEI_R48H  2000        //48���ԉJ�ʏ���l�f�t�H���g�l
#define SYS_RA_KENTEI_RA    2000        //�݉��J�ʏ���l�f�t�H���g�l
#define SYS_RA_KENTEI_DA    2000        //�A��(���H)�J�ʏ���l�f�t�H���g�l
#define SYS_RA_KENTEI_SA    2000        //�A��(���h)�J�ʏ���l�f�t�H���g�l
#define SYS_RA_KENTEI_YK    2000        //�L���J�ʏ���l�f�t�H���g�l
#define SYS_RA_KENTEI_YKP   200         //�L���J�ʋ��x����l�f�t�H���g�l
#define SYS_RA_KENTEI_ZAN   2000        //�c���J�ʏ���l�f�t�H���g�l
#define SYS_RA_KENTEI_JKZ   2000        //�O�������J�ʏ���l�f�t�H���g�l
#define SYS_RA_KENTEI_JK    2000        //�����J�ʏ���l�f�t�H���g�l
#define SYS_RA_KENTEI_JK15  2000        //1.5H�����J�ʏ���l�f�t�H���g�l
#define SYS_RA_KENTEI_JK72  2000        //72H�����J�ʏ���l�f�t�H���g�l
typedef struct tagSysRainKentei {
	short   sno;            //�����ǔ�
	short   s01;            //�P���ώZ�J�ʏ���l      (1�`2000, �ȊO:10)
	short   s10;            //10���ώZ�J�ʏ���l      (1�`2000, �ȊO:50)
	short   s60;            //60���ώZ�J�ʏ���l      (1�`2000, �ȊO:200)
	short   r10;            //10���J�ʏ���l          (1�`2000, �ȊO:50)
	short   r30;            //30���J�ʏ���l          (1�`2000, �ȊO:150)
	short   r60;            //60���J�ʏ���l          (1�`2000, �ȊO:200)
	short   r3h;            //�R���ԉJ�ʏ���l        (1�`2000, �ȊO:600)
	short   r6h;            //�U���ԉJ�ʏ���l        (1�`2000, �ȊO:990)
	short   r24h;           //24���ԉJ�ʏ���l        (1�`2000, �ȊO:990)
	short   r48h;           //48���ԉJ�ʏ���l        (1�`2000, �ȊO:2000)
	short   ra;             //�݉��J�ʏ���l          (1�`2000, �ȊO:2000)
	short   da;             //�A��(���H)�J�ʏ���l    (1�`2000, �ȊO:2000)
	short   sa;             //�A��(���h)�J�ʏ���l    (1�`2000, �ȊO:2000)
	short   yk;             //�L���J�ʏ���l          (1�`2000, �ȊO:2000)
	short   ykp;            //�L���J�ʋ��x����l      (1�`2000, �ȊO:200)
	short   zan;            //�c���J�ʏ���l          (1�`2000, �ȊO:2000)
	short   jkz;            //�O�������J�ʏ���l      (1�`2000, �ȊO:2000)
	short   jk;             //�����J�ʏ���l          (1�`2000, �ȊO:2000)
	short   jk15;           //1.5H�����J�ʏ���l      (1�`2000, �ȊO:2000)
	short   jk72;           //72H�����J�ʏ���l       (1�`2000, �ȊO:2000)
} sys_ra_kentei_t;


/********************************************************************
  �\���́F�J�ʌx���l
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
#define SYS_RA_ALM_NONE      9999        //�x�񔻒�Ȃ�
typedef struct tagSysRainAlm {
	short   sno;            //�����ǔ�
	short   r10_alm1;       //10���J�ʒ��Ӓl       (1�`2000, �ȊO:����l�Ȃ�)
	short   r10_alm2;       //10���J�ʌx���l       (1�`2000, �ȊO:����l�Ȃ�)
	short   r30_alm1;       //30���J�ʒ��Ӓl       (1�`2000, �ȊO:����l�Ȃ�)
	short   r30_alm2;       //30���J�ʌx���l       (1�`2000, �ȊO:����l�Ȃ�)
	short   r60_alm1;       //60���J�ʒ��Ӓl       (1�`2000, �ȊO:����l�Ȃ�)
	short   r60_alm2;       //60���J�ʌx���l       (1�`2000, �ȊO:����l�Ȃ�)
	short   r3h_alm1;       //�R���ԉJ�ʒ��Ӓl     (1�`2000, �ȊO:����l�Ȃ�)
	short   r3h_alm2;       //�R���ԉJ�ʌx���l     (1�`2000, �ȊO:����l�Ȃ�)
	short   r6h_alm1;       //�U���ԉJ�ʒ��Ӓl     (1�`2000, �ȊO:����l�Ȃ�)
	short   r6h_alm2;       //�U���ԉJ�ʌx���l     (1�`2000, �ȊO:����l�Ȃ�)
	short   r24h_alm1;      //24���ԉJ�ʒ��Ӓl     (1�`2000, �ȊO:����l�Ȃ�)
	short   r24h_alm2;      //24���ԉJ�ʌx���l     (1�`2000, �ȊO:����l�Ȃ�)
	short   r48h_alm1;      //48���ԉJ�ʒ��Ӓl     (1�`2000, �ȊO:����l�Ȃ�)
	short   r48h_alm2;      //48���ԉJ�ʌx���l     (1�`2000, �ȊO:����l�Ȃ�)
	short   ra_alm1;        //�݉��J�ʒ��Ӓl       (1�`2000, �ȊO:����l�Ȃ�)
	short   ra_alm2;        //�݉��J�ʌx���l       (1�`2000, �ȊO:����l�Ȃ�)
	short   da_alm1;        //�A��(���H)�J�ʒ��Ӓl (1�`2000, �ȊO:����l�Ȃ�)
	short   da_alm2;        //�A��(���H)�J�ʌx���l (1�`2000, �ȊO:����l�Ȃ�)
	short   sa_alm1;        //�A��(���h)�J�ʒ��Ӓl (1�`2000, �ȊO:����l�Ȃ�)
	short   sa_alm2;        //�A��(���h)�J�ʌx���l (1�`2000, �ȊO:����l�Ȃ�)
} sys_ra_alm_t;


/********************************************************************
  �\���́F���h�J�ʔ���萔
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
#define SYS_RA_SNAKE_NONE    0        //��J�ʃf�t�H���g�l
typedef struct tagSysRainSnake {
	short   sno;            //�����ǔ�
	short   mode;           //���h�J�ʔ������       (1�`30, �ȊO:����Ȃ�)
	short   br_a;           //��J�ʂ�             (1�`2000, �ȊO:�ݒ�l�Ȃ�)
	short   br_b;           //��J�ʂ�             (1�`2000, �ȊO:�ݒ�l�Ȃ�)
	short   br_c;           //��J�ʂ�             (1�`2000, �ȊO:�ݒ�l�Ȃ�)
	short   br_d;           //��J�ʂ�             (1�`2000, �ȊO:�ݒ�l�Ȃ�)
	short   br_a1;          //��J�ʂ��P           (1�`2000, �ȊO:�ݒ�l�Ȃ�)
	short   br_b1;          //��J�ʂ��P           (1�`2000, �ȊO:�ݒ�l�Ȃ�)
	short   br_a2;          //��J�ʂ��Q           (1�`2000, �ȊO:�ݒ�l�Ȃ�)
	short   br_b2;          //��J�ʂ��Q           (1�`2000, �ȊO:�ݒ�l�Ȃ�)
	short   br_a3;          //��J�ʂ��R           (1�`2000, �ȊO:�ݒ�l�Ȃ�)
	short   br_b3;          //��J�ʂ��R           (1�`2000, �ȊO:�ݒ�l�Ȃ�)
	short   br_a4;          //��J�ʂ��S           (1�`2000, �ȊO:�ݒ�l�Ȃ�)
	short   br_b4;          //��J�ʂ��S           (1�`2000, �ȊO:�ݒ�l�Ȃ�)
	short   br_e1;          //�����ő�Q���ԉJ�ʂ��P (1�`2000, �ȊO:�ݒ�l�Ȃ�)
	short   br_e2;          //�����ő�P���ԉJ�ʂ��Q (1�`2000, �ȊO:�ݒ�l�Ȃ�)
	short   br_f;           //��J�ʂ�             (1�`2000, �ȊO:�ݒ�l�Ȃ�)
	short   br_g;           //��J�ʂ�             (1�`2000, �ȊO:�ݒ�l�Ȃ�)
	short   br_h;           //��J�ʂ�             (1�`2000, �ȊO:�ݒ�l�Ȃ�)
	short   br_i;           //��J�ʂ�             (1�`2000, �ȊO:�ݒ�l�Ȃ�)
	short   br_f1;          //��J�ʂ��P           (1�`2000, �ȊO:�ݒ�l�Ȃ�)
	short   br_g1;          //��J�ʂ��P           (1�`2000, �ȊO:�ݒ�l�Ȃ�)
	short   br_f2;          //��J�ʂ��Q           (1�`2000, �ȊO:�ݒ�l�Ȃ�)
	short   br_g2;          //��J�ʂ��Q           (1�`2000, �ȊO:�ݒ�l�Ȃ�)
} sys_ra_snake_t;


/********************************************************************
  �\���́F�J�ʊ����W�v�l
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
typedef struct tagSysRainOld {
	short   sno;            //�����ǔ�
} sys_ra_old_t;


/********************************************************************
  �\���́F�J�ʃV�X�e���萔
  ����  �Fenzan         �J�ʉ��Z�����|�C���^�i�擪�ǂւ̃|�C���^�j
        �Fkentei        �J�ʈُ�l����|�C���^�i�擪�ǂւ̃|�C���^�j
        �Falm           �J�ʌx���l�|�C���^�i�擪�ǂւ̃|�C���^�j
        �Fsnake         ���h�J�ʔ���萔�|�C���^�i�擪�ǂւ̃|�C���^�j
        �Fold           �J�ʊ����W�v�l�|�C���^�i�擪�ǂւ̃|�C���^�j
********************************************************************/
typedef struct tagRainCnst {
	sys_ra_enzan_t      *enzan;     //�J�ʉ��Z�����|�C���^
	sys_ra_kentei_t     *kentei;    //�J�ʈُ�l����|�C���^
	sys_ra_alm_t        *alm;       //�J�ʌx���l�|�C���^
	sys_ra_snake_t      *snake;     //���h�J�ʔ���萔�|�C���^
	sys_ra_old_t        *old;       //�J�ʊ����W�v�l�|�C���^
} rain_cnst_t;


/********************************************************************
  �\���́F���ʉ��Z�萔
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
#define SYS_RV_ENZAN_QMODE_NM   1       //��M���ʉ��Z
#define SYS_RV_ENZAN_QMODE_HQ   2       //�g�p�萔���Z
#define	SYS_RV_ENZAN_KEIKAKU	1		//�v�捂����
typedef struct tagSysRiverEnzan {
	short   sno;            //�����ǔ�
	char    w_mode;         //���ʉ��Z���� (�ȊO:��M����)
	double  hosei;          //���ʕ␳�l   (-999.99�`999.99, �ȊO:�ݒ�l�Ȃ�)
	double  minus;          //�}�C�i�X���� (1�`99.99, �ȊO:�ݒ�l�Ȃ�)
	char    q_mode;         //���ʉ��Z���� (1:��M����, 2:�g�p�萔, �ȊO:�����)
	char	alm4_jude;		//�v�捂����i1:���肠��,�ȊO:����Ȃ��j
	char	yobi;
} sys_rv_enzan_t;


/********************************************************************
  �\���́F���ʈُ�l����
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
typedef struct tagSysRiverKentei {
	short   sno;            //�����ǔ�
	double  w_upp;          //���ʏ���l          (-9.99�`4000.00, �ȊO:����l�Ȃ�)
	double  w_low;          //���ʉ����l          (-9.99�`4000.00, �ȊO:����l�Ȃ�)
	short   w_fixtime;      //���ʈ��l���莞��  (1�`99, �ȊO:����l�Ȃ�)
	double  w_fix;          //���ʈ��l����l    (-9.99�`4000.00, �ȊO:����l�Ȃ�)
	double  q_upp;          //���ʏ���l          (0�`99999.99, �ȊO:����l�Ȃ�)
	double  q_low;          //���ʉ����l          (0�`99999.99, �ȊO:����l�Ȃ�)
	short   q_fixtime;      //���ʈ��l���莞��  (1�`99, �ȊO:����l�Ȃ�)
	double  q_fix;          //���ʈ��l����l    (0�`99999.99, �ȊO:����l�Ȃ�)
	double  w10_max;        //����10���ő�ω���  (0�`20.00, �ȊO:����l�Ȃ�)
	double  w30_max;        //����30���ő�ω���  (0�`20.00, �ȊO:����l�Ȃ�)
	double  w1h_max;        //���ʎ��ԍő�ω���  (0�`20.00, �ȊO:����l�Ȃ�)
	double  q10_max;        //����10���ő�ω���  (0�`999.99, �ȊO:����l�Ȃ�)
	double  q30_max;        //����30���ő�ω���  (0�`999.99, �ȊO:����l�Ȃ�)
	double  q1h_max;        //���ʎ��ԍő�ω���  (0�`999.99, �ȊO:����l�Ȃ�)
} sys_rv_kentei_t;


/********************************************************************
  �\���́F���ʌx���l
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
typedef struct tagSysRiverAlm {
	short   sno;            //�����ǔ�
	double  w_alm1;         //�ʕ񐅈�   (-9.99�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	double  w_alm2;         //�x������   (-9.99�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	double  w_alm3;         //�댯����   (-9.99�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	double  w_alm4;         //�v�捂���� (-9.99�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	double  w_regr;         //�ʏ퐅��   (-9.99�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	double  q_alm1;         //�ʕ񗬗�   (0�`999.99, �ȊO:�ݒ�l�Ȃ�)
	double  q_alm2;         //�x������   (0�`999.99, �ȊO:�ݒ�l�Ȃ�)
	double  q_alm3;         //�댯����   (0�`999.99, �ȊO:�ݒ�l�Ȃ�)
	double  q_alm4;         //�v�捂���� (0�`999.99, �ȊO:�ݒ�l�Ȃ�)
	double  q_regr;         //�ʏ헬��   (0�`999.99, �ȊO:�ݒ�l�Ȃ�)
} sys_rv_alm_t;


/********************************************************************
  �\���́F�͐�f�ʐ}
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
typedef struct tagSysRiverDanmen {
	short   sno;            //�����ǔ�
} sys_rv_danmen_t;


/********************************************************************
  �\���́F�g�|�p�萔
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
typedef struct tagSysRiverHQ {
	short   sno;            //�����ǔ�
	double  H0;             //�g�O�F�ؑ֐���       (-9.99�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	double  H1;             //�g�P�F�ؑ֐���       (-9.99�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	double  A1;             //�`�P�F�����␳�萔   (+-9999999999, �ȊO:�ݒ�l�Ȃ� �����_��)
	double  B1;             //�a�P�F�f�ʐϕ␳�萔 (+-9999999999, �ȊO:�ݒ�l�Ȃ� �����_��)
	double  H2;             //�g�Q�F�ؑ֐���       (-9.99�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	double  A2;             //�`�Q�F�����␳�萔   (+-9999999999, �ȊO:�ݒ�l�Ȃ� �����_��)
	double  B2;             //�a�Q�F�f�ʐϕ␳�萔 (+-9999999999, �ȊO:�ݒ�l�Ȃ� �����_��)
	double  H3;             //�g�R�F�ؑ֐���       (-9.99�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	double  A3;             //�`�R�F�����␳�萔   (+-9999999999, �ȊO:�ݒ�l�Ȃ� �����_��)
	double  B3;             //�a�R�F�f�ʐϕ␳�萔 (+-9999999999, �ȊO:�ݒ�l�Ȃ� �����_��)
	double  H4;             //�g�S�F�ؑ֐���       (-9.99�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	double  A4;             //�`�S�F�����␳�萔   (+-9999999999, �ȊO:�ݒ�l�Ȃ� �����_��)
	double  B4;             //�a�S�F�f�ʐϕ␳�萔 (+-9999999999, �ȊO:�ݒ�l�Ȃ� �����_��)
	double  H5;             //�g�T�F�ؑ֐���       (-9.99�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	double  A5;             //�`�T�F�����␳�萔   (+-9999999999, �ȊO:�ݒ�l�Ȃ� �����_��)
	double  B5;             //�a�T�F�f�ʐϕ␳�萔 (+-9999999999, �ȊO:�ݒ�l�Ȃ� �����_��)
	double  H6;             //�g�U�F�ؑ֐���       (-9.99�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	double  A6;             //�`�U�F�����␳�萔   (+-9999999999, �ȊO:�ݒ�l�Ȃ� �����_��)
	double  B6;             //�a�U�F�f�ʐϕ␳�萔 (+-9999999999, �ȊO:�ݒ�l�Ȃ� �����_��)
} sys_rv_hq_t;


/********************************************************************
  �\���́F���ʊ����W�v�l
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
typedef struct tagSysRiverOld {
	short   sno;            //�����ǔ�
} sys_rv_old_t;


/********************************************************************
  �\���́F���ʃV�X�e���萔
  ����  �Fenzan         ���ʉ��Z�����|�C���^�i�擪�ǂւ̃|�C���^�j
        �Fkentei        ���ʈُ�l����|�C���^�i�擪�ǂւ̃|�C���^�j
        �Fkeihou        ���ʌx���l�|�C���^�i�擪�ǂւ̃|�C���^�j
        �Fdanmen        �͐�f�ʐ}�|�C���^�i�擪�ǂւ̃|�C���^�j
        �Fhq            �g�|�p�萔�|�C���^�i�擪�ǂւ̃|�C���^�j
        �Fold           ���ʊ����W�v�l�|�C���^�i�擪�ǂւ̃|�C���^�j
********************************************************************/
typedef struct tagRiverCnst {
	sys_rv_enzan_t      *enzan;     //���ʉ��Z�����|�C���^
	sys_rv_kentei_t     *kentei;    //���ʈُ�l����|�C���^
	sys_rv_alm_t        *keihou;    //���ʌx���l�|�C���^
	sys_rv_danmen_t     *danmen;    //�͐�f�ʐ}�|�C���^
	sys_rv_hq_t         *hq;        //�g�|�p�萔�|�C���^
	sys_rv_old_t        *old;       //���ʊ����W�v�l�|�C���^
} river_cnst_t;


/********************************************************************
  �\���́F�_�����ʉ��Z����
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
#define SYS_DM_ENZAN_MODE_INPT  1       //���͒l
#define SYS_DM_ENZAN_MODE_CALC  2       //���Z
typedef struct tagSysDamEnzan {
	short   sno;            //�����ǔ�
	short   chsi_mode;      //�����ʉ��Z����           (1:���͒l, �ȊO:�����)
	short   scsr_mode;      //�������ʉ��Z����         (1:���͒l, �ȊO:�����)
	short   zrnr_mode;      //�S�����ʉ��Z����         (1:���͒l, �ȊO:�����)
	short   zhrr_mode;      //�S�����ʉ��Z����         (1:���͒l, �ȊO:�����)
	short   akyr_mode;      //��e�ʉ��Z����           (1:���͒l, 2:���Z�l, �ȊO:�����)
	short   ycrt_mode;      //�L�����������Z����       (1:���͒l, 2:���Z�l, �ȊO:�����)
	short   csir_mode;      //�������ʉ��Z����         (1:���͒l, 2:���Z�l, �ȊO:�����)
	short   dmhr_mode;      //�_�������ʉ��Z����       (1:���͒l, �ȊO:�����)
	short   gthr_mode;      //�Q�[�g�����ʉ��Z����     (1:���͒l, �ȊO:�����)
	short   vlhr_mode;      //�o���u�����ʉ��Z����     (1:���͒l, �ȊO:�����)
	short   rshr_mode;      //���������ʉ��Z����       (1:���͒l, �ȊO:�����)
	short   hdss_mode;      //���d�g�p���ʉ��Z����     (1:���͒l, �ȊO:�����)
	short   yosr_mode;      //�g���ʉ��Z����           (1:���͒l, �ȊO:�����)
	short   hokr_mode;      //�⋋�ʉ��Z����           (1:���͒l, �ȊO:�����)
	short   akis_mode;      //�󐅈ʉ��Z����           (1:���͒l, 2:���Z�l, �ȊO:�����)
	short   ycsr_mode;      //�L�������ʉ��Z����       (1:���͒l, 2:���Z�l, �ȊO:�����)
	short   totl_mode;      //�_�����ʓ��W�v�l���Z���� (1:���͒l, 2:���Z�l, �ȊO:�����)
} sys_dm_enzan_t;


/********************************************************************
  �\���́F�_�����ʈُ�l����
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
typedef struct tagSysDamKentei {
	short   sno;            //�����ǔ�
} sys_dm_kentei_t;


/********************************************************************
  �\���́F�_����l
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
typedef struct tagSysDamAlm {
	short   sno;            //�����ǔ�
	double  skei_kozi;      //�݌v�^����                 (0�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	double  surcharge;      //�T�[�`���[�W����           (0�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	double  tadasigaki;     //��������������             (0�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	double  joji_mansui;    //�펞������                 (0�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	double  kozi_mansui;    //�^����������               (0�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	double  saitei_w;       //�Œᐅ��                   (0�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	double  shi_w;          //������                     (0�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	double  skei_kozi_q;    //�݌v�^������               (0�`99999.999, �ȊO:�ݒ�l�Ȃ�)
	double  kei_taka_q;     //�v�捂������               (0�`99999.999, �ȊO:�ݒ�l�Ȃ�)
	double  kei_maxho;      //�v��ő������             (0�`99999.999, �ȊO:�ݒ�l�Ȃ�)
	double  kihon_taka_q;   //��{��������               (0�`99999.999, �ȊO:�ݒ�l�Ȃ�)
	double  kozi_chset_q;   //�^�����ߊJ�n����           (0�`99999.999, �ȊO:�ݒ�l�Ȃ�)
	double  kouzuiryo;      //�^����������(�^����)       (0�`99999.999, �ȊO:�ݒ�l�Ȃ�)
	double  horyu_kijun;    //�����ʒʕ��l           (0�`99999.999, �ȊO:�ݒ�l�Ȃ�)
	double  taisha_yoryo;   //�������e��                 (0�`99999.999, �ȊO:�ݒ�l�Ȃ�)
	double  srch_yuukou;    //�T�[�`���[�W�L�������e��   (0�`99999.999, �ȊO:�ݒ�l�Ȃ�)
	double  jouj_yuukou;    //�펞�����ʗL�������e��     (0�`99999.999, �ȊO:�ݒ�l�Ȃ�)
	double  seigen1;        //�������ʂP                 (0�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	double  yuukou1;        //�L�������e�ʂP             (0�`99999.999, �ȊO:�ݒ�l�Ȃ�)
	short   seigen_st_m1;   //�������ʂP���ԊJ�n(��)     (1�`12, �ȊO:�ݒ�l�Ȃ�)
	short   seigen_st_d1;   //�������ʂP���ԊJ�n(��)     (1�`31, �ȊO:�ݒ�l�Ȃ�)
	short   seigen_ed_m1;   //�������ʂP���ԏI��(��)     (1�`12, �ȊO:�ݒ�l�Ȃ�)
	short   seigen_ed_d1;   //�������ʂP���ԏI��(��)     (1�`31, �ȊO:�ݒ�l�Ȃ�)
	double  seigen2;        //�������ʂQ                 (0�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	double  yuukou2;        //�L�������e�ʂQ             (0�`99999.999, �ȊO:�ݒ�l�Ȃ�)
	short   seigen_st_m2;   //�������ʂQ���ԊJ�n(��)     (1�`12, �ȊO:�ݒ�l�Ȃ�)
	short   seigen_st_d2;   //�������ʂQ���ԊJ�n(��)     (1�`31, �ȊO:�ݒ�l�Ȃ�)
	short   seigen_ed_m2;   //�������ʂQ���ԏI��(��)     (1�`12, �ȊO:�ݒ�l�Ȃ�)
	short   seigen_ed_d2;   //�������ʂQ���ԏI��(��)     (1�`31, �ȊO:�ݒ�l�Ȃ�)
	double  seigen3;        //�������ʂR                 (0�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	double  yuukou3;        //�L�������e�ʂR             (0�`99999.999, �ȊO:�ݒ�l�Ȃ�)
	short   seigen_st_m3;   //�������ʂR���ԊJ�n(��)     (1�`12, �ȊO:�ݒ�l�Ȃ�)
	short   seigen_st_d3;   //�������ʂR���ԊJ�n(��)     (1�`31, �ȊO:�ݒ�l�Ȃ�)
	short   seigen_ed_m3;   //�������ʂR���ԏI��(��)     (1�`12, �ȊO:�ݒ�l�Ȃ�)
	short   seigen_ed_d3;   //�������ʂR���ԏI��(��)     (1�`31, �ȊO:�ݒ�l�Ȃ�)
	double  seigen4;        //�������ʂS                 (0�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	double  yuukou4;        //�L�������e�ʂS             (0�`99999.999, �ȊO:�ݒ�l�Ȃ�)
	short   seigen_st_m4;   //�������ʂS���ԊJ�n(��)     (1�`12, �ȊO:�ݒ�l�Ȃ�)
	short   seigen_st_d4;   //�������ʂS���ԊJ�n(��)     (1�`31, �ȊO:�ݒ�l�Ȃ�)
	short   seigen_ed_m4;   //�������ʂS���ԏI��(��)     (1�`12, �ȊO:�ݒ�l�Ȃ�)
	short   seigen_ed_d4;   //�������ʂS���ԏI��(��)     (1�`31, �ȊO:�ݒ�l�Ȃ�)
	double  yobihoryu1;     //�\���������ʂP             (0�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	short   yobi_st_m1;     //�\���������ʂP���ԊJ�n(��) (1�`12, �ȊO:�ݒ�l�Ȃ�)
	short   yobi_st_d1;     //�\���������ʂP���ԊJ�n(��) (1�`31, �ȊO:�ݒ�l�Ȃ�)
	short   yobi_ed_m1;     //�\���������ʂP���ԏI��(��) (1�`12, �ȊO:�ݒ�l�Ȃ�)
	short   yobi_ed_d1;     //�\���������ʂP���ԏI��(��) (1�`31, �ȊO:�ݒ�l�Ȃ�)
	double  yobihoryu2;     //�\���������ʂQ             (0�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	short   yobi_st_m2;     //�\���������ʂQ���ԊJ�n(��) (1�`12, �ȊO:�ݒ�l�Ȃ�)
	short   yobi_st_d2;     //�\���������ʂQ���ԊJ�n(��) (1�`31, �ȊO:�ݒ�l�Ȃ�)
	short   yobi_ed_m2;     //�\���������ʂQ���ԏI��(��) (1�`12, �ȊO:�ݒ�l�Ȃ�)
	short   yobi_ed_d2;     //�\���������ʂQ���ԏI��(��) (1�`31, �ȊO:�ݒ�l�Ȃ�)
	double  yobihoryu3;     //�\���������ʂR             (0�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	short   yobi_st_m3;     //�\���������ʂR���ԊJ�n(��) (1�`12, �ȊO:�ݒ�l�Ȃ�)
	short   yobi_st_d3;     //�\���������ʂR���ԊJ�n(��) (1�`31, �ȊO:�ݒ�l�Ȃ�)
	short   yobi_ed_m3;     //�\���������ʂR���ԏI��(��) (1�`12, �ȊO:�ݒ�l�Ȃ�)
	short   yobi_ed_d3;     //�\���������ʂR���ԏI��(��) (1�`31, �ȊO:�ݒ�l�Ȃ�)
	double  yobihoryu4;     //�\���������ʂS             (0�`4000.00, �ȊO:�ݒ�l�Ȃ�)
	short   yobi_st_m4;     //�\���������ʂS���ԊJ�n(��) (1�`12, �ȊO:�ݒ�l�Ȃ�)
	short   yobi_st_d4;     //�\���������ʂS���ԊJ�n(��) (1�`31, �ȊO:�ݒ�l�Ȃ�)
	short   yobi_ed_m4;     //�\���������ʂS���ԏI��(��) (1�`12, �ȊO:�ݒ�l�Ȃ�)
	short   yobi_ed_d4;     //�\���������ʂS���ԏI��(��) (1�`31, �ȊO:�ݒ�l�Ȃ�)
} sys_dm_alm_t;


/********************************************************************
  �\���́F�_������
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
typedef struct tagSysDamShogen {
	short   sno;            //�����ǔ�
} sys_dm_shogen_t;


/********************************************************************
  �\���́F�_�����ʃV�X�e���萔
  ����  �Fenzan         �_�����ʉ��Z�����|�C���^�i�擪�ǂւ̃|�C���^�j
        �Fkentei        �_�����ʈُ�l����|�C���^�i�擪�ǂւ̃|�C���^�j
        �Falm           �_����l�|�C���^�i�擪�ǂւ̃|�C���^�j
        �Fshogen        �_�������l�|�C���^�i�擪�ǂւ̃|�C���^�j
********************************************************************/
typedef struct tagDamCnst {
	sys_dm_enzan_t      *enzan;     //�_�����ʉ��Z�����|�C���^
	sys_dm_kentei_t     *kentei;    //�_�����ʈُ�l����|�C���^
	sys_dm_alm_t        *alm;       //�_����l�|�C���^
	sys_dm_shogen_t     *shogen;    //�_�������|�C���^
} dam_cnst_t;


/********************************************************************
  �\���́F���ʉ��Z����
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
#define SYS_TD_ENZAN_MODE_INPT  1       //���͒l
#define SYS_TD_ENZAN_MODE_CNST  2       //�萔
#define SYS_TD_ENZAN_MODE_SIOM  3       //�����\
typedef struct tagSysTideEnzan {
	short   sno;            //�����ǔ�
	char    js_mode;        //�������ʉ��Z���� (�ȊO:���͒l)
	char    tm_mode;        //�V�����ʉ��Z���� (1:���͒l, 2:�萔, 3:�����\, �ȊO:�����)
} sys_td_enzan_t;


/********************************************************************
  �\���́F���ʈُ�l����
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
typedef struct tagSysTideKentei {
	short   sno;            //�����ǔ�
} sys_td_kentei_t;


/********************************************************************
  �\���́F���ʌx���l
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
typedef struct tagSysTideAlm {
	short   sno;            //�����ǔ�
	double  td_alm1;        //�ʕ񒪈�     (-9.99�`99.99, �ȊO:�ݒ�l�Ȃ�)
	double  td_alm2;        //�x������     (-9.99�`99.99, �ȊO:�ݒ�l�Ȃ�)
	double  td_alm3;        //�댯����     (-9.99�`99.99, �ȊO:�ݒ�l�Ȃ�)
	double  td_alm1_d;      //�ʕ񒪈ʕ΍� (0�`99.99, �ȊO:�ݒ�l�Ȃ�)
	double  td_alm2_d;      //�x�����ʕ΍� (0�`99.99, �ȊO:�ݒ�l�Ȃ�)
	double  td_alm3_d;      //�댯���ʕ΍� (0�`99.99, �ȊO:�ݒ�l�Ȃ�)
} sys_td_alm_t;


/********************************************************************
  �\���́F�V�����ʉ��Z�萔
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
typedef struct tagSysTideTenmon {
	short   sno;            //�����ǔ�
} sys_td_tenmon_t;


/********************************************************************
  �\���́F�����\
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
typedef struct tagSysTideSiomi {
	short   sno;            //�����ǔ�
} sys_td_siomi_t;


/********************************************************************
  �\���́F���ʃV�X�e���萔
  ����  �Fenzan         ���ʉ��Z�����|�C���^�i�擪�ǂւ̃|�C���^�j
        �Fkentei        ���ʈُ�l����|�C���^�i�擪�ǂւ̃|�C���^�j
        �Falm           ���ʌx���l�|�C���^�i�擪�ǂւ̃|�C���^�j
        �Ftenmon        �V�����ʉ��Z�萔�|�C���^�i�擪�ǂւ̃|�C���^�j
        �Fsiomi         �����\�|�C���^�i�擪�ǂւ̃|�C���^�j
********************************************************************/
typedef struct tagTideCnst {
	sys_td_enzan_t      *enzan;     //���ʉ��Z�����|�C���^
	sys_td_kentei_t     *kentei;    //���ʈُ�l����|�C���^
	sys_td_alm_t        *alm;       //���ʌx���l�|�C���^
	sys_td_tenmon_t     *tenmon;    //�V�����ʉ��Z�萔�|�C���^
	sys_td_siomi_t      *siomi;     //�����\�|�C���^
} tide_cnst_t;


/********************************************************************
  �\���́F�����^�������Z����
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
#define SYS_WD_ENZAN_MODE_INPT  1       //���͒l
#define SYS_WD_ENZAN_MODE_AD    2       //�`�c�ϊ� [2003.07.02]
typedef struct tagSysWindEnzan {
	short   sno;            //�����ǔ�
	char    avwv_mode;      //���ϕ������Z����     (1:���͒l, �ȊO:�����)
	char    mxwv_mode;      //�u�ԍő啗�����Z���� (1:���͒l, �ȊO:�����)
	char    dw16_mode;      //16���ʕ������Z����   (1:���͒l, �ȊO:�����)
	char    mxdw_mode;      //�u�ԍő啗�����Z���� (1:���͒l, �ȊO:�����)
} sys_wd_enzan_t;


/********************************************************************
  �\���́F�����ُ�l����
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
typedef struct tagSysWindKentei {
	short   sno;            //�����ǔ�
} sys_wd_kentei_t;


/********************************************************************
  �\���́F�����x���l
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
typedef struct tagSysWindAlm {
	short   sno;            //�����ǔ�
	double  av_alm1;        //���ϕ������Ӓl     (1�`60.0, �ȊO:����Ȃ�)
	double  av_alm2;        //���ϕ����x���l     (1�`60.0, �ȊO:����Ȃ�)
	double  mx_alm1;        //�u�ԍő啗�����Ӓl (1�`60.0, �ȊO:����Ȃ�)
	double  mx_alm2;        //�u�ԍő啗���x���l (1�`60.0, �ȊO:����Ȃ�)
} sys_wd_alm_t;


/********************************************************************
  �\���́F���������V�X�e���萔
  ����  �Fenzan         �����^�������Z�����|�C���^�i�擪�ǂւ̃|�C���^�j
        �Fkentei        �����ُ�l����|�C���^�i�擪�ǂւ̃|�C���^�j
        �Falm           �����x���l�|�C���^�i�擪�ǂւ̃|�C���^�j
********************************************************************/
typedef struct tagWindCnst {
	sys_wd_enzan_t      *enzan;     //�����^�������Z�����|�C���^
	sys_wd_kentei_t     *kentei;    //�����ُ�l����|�C���^
	sys_wd_alm_t        *alm;       //�����x���l�|�C���^
} wind_cnst_t;


/********************************************************************
  �\���́F���p��ԉ��Z����
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
typedef struct tagSysRelayEnzan {
	short   sno;            //�����ǔ�
	char    mode;           //���p�Ǐ�ԉ��Z����
} sys_rl_enzan_t;


/********************************************************************
  �\���́F���p���V�X�e���萔
  ����  �Fenzan         ���p��ԉ��Z�����|�C���^�i�擪�ǂւ̃|�C���^�j
********************************************************************/
typedef struct tagRelayCnst {
	sys_rl_enzan_t      *enzan;     //���p��ԉ��Z�����|�C���^
} relay_cnst_t;


/********************************************************************
  �\���́F�ϐ቉�Z���� [2003.06.04]
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
#define SYS_SN_ENZAN_MODE_INPT  1        //�����l
#define SYS_SN_ENZAN_MODE_AD1   2        //�`�^�c�ϊ��P
#define SYS_SN_ENZAN_MODE_CALC  1        //���Z����
#define SYS_SN_ENZAN_MODE_NONE  9        //���Z����

#define SYS_SN_ENZAN_S_MON      10       //�݌v�~��ʉ��Z�J�n���t(��)�f�t�H���g�l
#define SYS_SN_ENZAN_S_DAY      31       //�݌v�~��ʉ��Z�J�n���t(��)�f�t�H���g�l
#define SYS_SN_ENZAN_S_HOUR     24       //�݌v�~��ʉ��Z�J�n���t(��)�f�t�H���g�l
#define SYS_SN_ENZAN_E_MON      4        //�݌v�~��ʉ��Z�I�����t(��)�f�t�H���g�l
#define SYS_SN_ENZAN_E_DAY      30       //�݌v�~��ʉ��Z�I�����t(��)�f�t�H���g�l
#define SYS_SN_ENZAN_E_HOUR     24       //�݌v�~��ʉ��Z�I�����t(��)�f�t�H���g�l

typedef struct tagSysSnowEnzan {
	short   sno;            //�����ǔ�
	char    ss_mode;        //�ϐ�[���Z���� (1:�����l, 2:�`�^�c�ϊ��P, �ȊO:�����l)
	short   hosei;          //�ϐ�[�␳�l  (0�`9999, �ȊO:0)
	char    ks_mode;        //�~��ʉ��Z���� (1:���Z���� 9:���Z����, �ȊO:���Z����)
	short   s_mon;          //�݌v�~��ʉ��Z�J�n���t(��) (10,11,12, �ȊO:10)
	short   s_day;          //�݌v�~��ʉ��Z�J�n���t(��) (1�`31,    �ȊO:31)
	short   s_hour;         //�݌v�~��ʉ��Z�J�n���t(��) (1�`24,    �ȊO:24)
	short   e_mon;          //�݌v�~��ʉ��Z�I�����t(��) (3,4,5,    �ȊO: 4)
	short   e_day;          //�݌v�~��ʉ��Z�I�����t(��) (1�`31,    �ȊO:30)
	short   e_hour;         //�݌v�~��ʉ��Z�I�����t(��) (1�`24,    �ȊO:24)
} sys_sn_enzan_t;


/********************************************************************
  �\���́F�ϐ�ُ�l���� [2003.06.04]
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
#define SYS_SN_KENTEI_SS_UPP    500      //�P�ϐ�[����l�f�t�H���g�l
#define SYS_SN_KENTEI_KS1H_UPP  100      //���ԍ~��ʏ���l�f�t�H���g�l
#define SYS_SN_KENTEI_KS_UPP    300      //�~��ʏ���l�f�t�H���g�l
typedef struct tagSysSnowKentei {
	short   sno;            //�����ǔ�
	short   ss_upp;         //�ϐ�[�ʏ���l       (1�`9999, �ȊO:500)
	short   ks1h_upp;       //���ԍ~��ʏ���l     (1�`9999, �ȊO:100)
	short   ks_upp;         //�~��ʏ���l         (1�`9999, �ȊO:200)
	short   ss_max;         //�ϐ�[�ő�ω���     (1�`9999, �ȊO:�ݒ�l����)
	short   ks1h_max;       //���ԍ~��ʍő�ω��� (1�`9999, �ȊO:�ݒ�l����)
} sys_sn_kentei_t;


/********************************************************************
  �\���́F�ϐ�x���l [2003.06.04]
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
typedef struct tagSysSnowAlm {
	short   sno;            //�����ǔ�
	short   ss_alm1;        //�ϐ�[���Ӓl         (1�`9999, �ȊO:����l�Ȃ�)
	short   ss_alm2;        //�ϐ�[�x���l         (1�`9999, �ȊO:����l�Ȃ�)
	short   ks1h_alm1;      //���ԍ~��ʒ��Ӓl     (1�`9999, �ȊO:����l�Ȃ�)
	short   ks1h_alm2;      //���ԍ~��ʌx���l     (1�`9999, �ȊO:����l�Ȃ�)
} sys_sn_alm_t;


/********************************************************************
  �\���́F�ϐᕡ���ǐݒ� [2003.06.04]
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
typedef struct tagSysSnowPair {
	short   sno;            //�����ǔ�
	short   temp_st;        //�C���Ή���           (1�`999, �ȊO:�ݒ�l�Ȃ�)
	short   road_st;        //�H�ʑΉ���           (1�`999, �ȊO:�ݒ�l�Ȃ�)
} sys_sn_pair_t;


/********************************************************************
  �\���́F�C�����Z���� [2003.06.04]
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
#define SYS_TP_ENZAN_MODE_INPT  1       //�����l
#define SYS_TP_ENZAN_MODE_AD1   2       //�`�^�c�ϊ��P
typedef struct tagSysTempEnzan {
	short   sno;            //�����ǔ�
	char    mode;           //�C�����Z���� (1:�����l, 2:�`�^�c�ϊ��P �ȊO:�����l)
} sys_tp_enzan_t;


/********************************************************************
  �\���́F�C���ُ�l���� [2003.06.09]
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
#define SYS_TP_KENTEI_TP_UPP    50       //�C������l�f�t�H���g�l
#define SYS_TP_KENTEI_TP_LOW    -50      //�C�������l�f�t�H���g�l
typedef struct tagSysTempKentei {
	short   sno;            //�����ǔ�
	double  tp_upp;         //�C������l           (-99.9�`99.9, �ȊO:50.0)
	double  tp_low;         //�C�������l           (-99.9�`99.9, �ȊO:-50.0)
	double  tp_max;         //�C���ő�ω���       (  0.1�`99.9, �ȊO:�ݒ�l����)
} sys_tp_kentei_t;


/********************************************************************
  �\���́F�C���x���l [2003.06.09]
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
typedef struct tagSysTempAlm {
	short   sno;            //�����ǔ�
	double  tp_l_alm1;      //�C�����Ӓl(�ቷ�p)   (-99.9�` 0.1, �ȊO:����Ȃ�)
	double  tp_l_alm2;      //�C���x���l(�ቷ�p)   (-99.9�` 0.1, �ȊO:����Ȃ�)
} sys_tp_alm_t;


/********************************************************************
  �\���́F�H�����Z���� [2003.06.04]
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
#define SYS_RD_ENZAN_MODE_INPT  1       //�����l
#define SYS_RD_ENZAN_CHNG_21_1  1       //�Q�P�����̂P
typedef struct tagSysRoadEnzan {
	short   sno;            //�����ǔ�
	char    mode;           //�H�����Z���� (1:�����l, �ȊO:�����l)
	char    chng;           //�H�ʏ�ԕϊ����� (1:�Q�P�����̂P, �ȊO:�����)
} sys_rd_enzan_t;


/********************************************************************
  �\���́F�H���ُ�l���� [2003.06.09]
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
#define SYS_RD_KENTEI_TP_UPP    70       //�H������l�f�t�H���g�l
#define SYS_RD_KENTEI_TP_LOW    -50      //�H�������l�f�t�H���g�l
typedef struct tagSysRoadKentei {
	short   sno;            //�����ǔ�
	double  rd_upp;         //�H������l           (-99.9�`99.9, �ȊO:70.0)
	double  rd_low;         //�H�������l           (-99.9�`99.9, �ȊO:-50.0)
	double  rd_max;         //�H���ő�ω���       (  0.1�`99.9, �ȊO:�ݒ�l����)
} sys_rd_kentei_t;


/********************************************************************
  �\���́F�H���x���l [2003.06.09]
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
typedef struct tagSysRoadAlm {
	short   sno;            //�����ǔ�
	double  rd_l_alm1;      //�H�����Ӓl(�ቷ�p)   (-99.9�` 0.1, �ȊO:����Ȃ�)
	double  rd_l_alm2;      //�H���x���l(�ቷ�p)   (-99.9�` 0.1, �ȊO:����Ȃ�)
} sys_rd_alm_t;


/********************************************************************
  �\���́F���E���J�ʉ��Z���� [2003.06.09]
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
#define SYS_SK_ENZAN_MODE_INPT  1       //���͒l
#define SYS_SK_ENZAN_MODE_CALC  2       //���Z
typedef struct tagSysSekiEnzan {
	short   sno;            //�����ǔ�
	short   chsr_mode;      //�����ʉ��Z����           (1:���͒l, �ȊO:�����)
	short   akyr_mode;      //��e�ʉ��Z����           (1:���͒l, �ȊO:�����)
	short   rynr_mode;      //�����ʉ��Z����           (1:���͒l, �ȊO:�����)
	short   shrr_mode;      //�������ʉ��Z����         (1:���͒l, �ȊO:�����)
	short   zssr_mode;      //�S�搅�ʉ��Z����         (1:���͒l, �ȊO:�����)
	short   ssr1_mode;      //�搅�ʂP���Z����         (1:���͒l, �ȊO:�����)
	short   ssr2_mode;      //�搅�ʂQ���Z����         (1:���͒l, �ȊO:�����)
	short   ssr3_mode;      //�搅�ʂR���Z����         (1:���͒l, �ȊO:�����)
	short   hokr_mode;      //�⋋�ʉ��Z����           (1:���͒l, �ȊO:�����)
	short   rs01_mode;      //�\���P���Z����           (1:���͒l, �ȊO:�����)
	short   rs02_mode;      //�\���Q���Z����           (1:���͒l, �ȊO:�����)
	short   rs03_mode;      //�\���R���Z����           (1:���͒l, �ȊO:�����)
	short   rs04_mode;      //�\���S���Z����           (1:���͒l, �ȊO:�����)
	short   rs05_mode;      //�\���T���Z����           (1:���͒l, �ȊO:�����)
	short   rs06_mode;      //�\���U���Z����           (1:���͒l, �ȊO:�����)
	short   rs07_mode;      //�\���V���Z����           (1:���͒l, �ȊO:�����)
	short   rs08_mode;      //�\���W���Z����           (1:���͒l, �ȊO:�����)
	short   rs09_mode;      //�\���X���Z����           (1:���͒l, �ȊO:�����)
	short   rs10_mode;      //�\��10���Z����           (1:���͒l, �ȊO:�����)
	short   rs11_mode;      //�\��11���Z����           (1:���͒l, �ȊO:�����)
	short   rs12_mode;      //�\��12���Z����           (1:���͒l, �ȊO:�����)
	short   rs13_mode;      //�\��13���Z����           (1:���͒l, �ȊO:�����)
	short   rs14_mode;      //�\��14���Z����           (1:���͒l, �ȊO:�����)
	short   rs15_mode;      //�\��15���Z����           (1:���͒l, �ȊO:�����)
	short   rs16_mode;      //�\��16���Z����           (1:���͒l, �ȊO:�����)
} sys_sk_enzan_t;


/********************************************************************
  �\���́F���E���J�ʈُ�l���� [2003.06.04]
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
typedef struct tagSysSekiKentei {
	short   sno;            //�����ǔ�
	double  chsr10_max;     //������10���ő�ω���     (���g�p)
	double  chsr30_max;     //������30���ő�ω���     (���g�p)
	double  chsr1h_max;     //�����ʎ��ԍő�ω���     (���g�p)
	double  akyr10_max;     //��e��10���ő�ω���     (���g�p)
	double  akyr30_max;     //��e��30���ő�ω���     (���g�p)
	double  akyr1h_max;     //��e�ʎ��ԍő�ω���     (���g�p)
	double  rynr_upp;       //�����ʏ���l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rynr_low;       //�����ʉ����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rynr10_max;     //������10���ő�ω���     (���g�p)
	double  rynr30_max;     //������30���ő�ω���     (���g�p)
	double  rynr1h_max;     //�����ʎ��ԍő�ω���     (���g�p)
	double  shrr_upp;       //�������ʏ���l           (0�`99999.99, �ȊO:����l�Ȃ�)
	double  shrr_low;       //�������ʉ����l           (0�`99999.99, �ȊO:����l�Ȃ�)
	double  shrr10_max;     //��������10���ő�ω���   (���g�p)
	double  shrr30_max;     //��������30���ő�ω���   (���g�p)
	double  shrr1h_max;     //�������ʎ��ԍő�ω���   (���g�p)
	double  zssr_upp;       //�S�搅�ʏ���l           (0�`99999.99, �ȊO:����l�Ȃ�)
	double  zssr_low;       //�S�搅�ʉ����l           (0�`99999.99, �ȊO:����l�Ȃ�)
	double  zssr10_max;     //�S�搅��10���ő�ω���   (���g�p)
	double  zssr30_max;     //�S�搅��30���ő�ω���   (���g�p)
	double  zssr1h_max;     //�S�搅�ʎ��ԍő�ω���   (���g�p)
	double  ssr1_upp;       //�搅�ʂP����l           (0�`99999.99, �ȊO:����l�Ȃ�)
	double  ssr1_low;       //�搅�ʂP�����l           (0�`99999.99, �ȊO:����l�Ȃ�)
	double  ssr110_max;     //�搅�ʂP10���ő�ω���   (���g�p)
	double  ssr130_max;     //�搅�ʂP30���ő�ω���   (���g�p)
	double  ssr11h_max;     //�搅�ʂP���ԍő�ω���   (���g�p)
	double  ssr2_upp;       //�搅�ʂQ����l           (0�`99999.99, �ȊO:����l�Ȃ�)
	double  ssr2_low;       //�搅�ʂQ�����l           (0�`99999.99, �ȊO:����l�Ȃ�)
	double  ssr210_max;     //�搅�ʂQ10���ő�ω���   (���g�p)
	double  ssr230_max;     //�搅�ʂQ30���ő�ω���   (���g�p)
	double  ssr21h_max;     //�搅�ʂQ���ԍő�ω���   (���g�p)
	double  ssr3_upp;       //�搅�ʂR����l           (0�`99999.99, �ȊO:����l�Ȃ�)
	double  ssr3_low;       //�搅�ʂR�����l           (0�`99999.99, �ȊO:����l�Ȃ�)
	double  ssr310_max;     //�搅�ʂR10���ő�ω���   (���g�p)
	double  ssr330_max;     //�搅�ʂR30���ő�ω���   (���g�p)
	double  ssr31h_max;     //�搅�ʂR���ԍő�ω���   (���g�p)
	double  rs01_upp;       //�\���P����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs01_low;       //�\���P�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs02_upp;       //�\���Q����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs02_low;       //�\���Q�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs03_upp;       //�\���R����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs03_low;       //�\���R�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs04_upp;       //�\���S����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs04_low;       //�\���S�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs05_upp;       //�\���T����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs05_low;       //�\���T�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs06_upp;       //�\���U����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs06_low;       //�\���U�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs07_upp;       //�\���V����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs07_low;       //�\���V�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs08_upp;       //�\���W����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs08_low;       //�\���W�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs09_upp;       //�\���X����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs09_low;       //�\���X�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs10_upp;       //�\��10����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs10_low;       //�\��10�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs11_upp;       //�\��11����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs11_low;       //�\��11�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs12_upp;       //�\��12����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs12_low;       //�\��12�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs13_upp;       //�\��13����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs13_low;       //�\��13�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs14_upp;       //�\��14����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs14_low;       //�\��14�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs15_upp;       //�\��15����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs15_low;       //�\��15�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs16_upp;       //�\��16����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs16_low;       //�\��16�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
} sys_sk_kentei_t;


/********************************************************************
  �\���́F���E���x���l [2003.06.04]
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
#define SYS_RA_ALM_NONE      9999        //�x�񔻒�Ȃ�
typedef struct tagSysSekiAlm {
	short   sno;            //�����ǔ�
	double  kouzui;         //�^����                   (0�`99999.999, �ȊO:�ݒ�l�Ȃ�)
	double  skei_kozi_q;    //�݌v�^������             (0�`99999.999, �ȊO:�ݒ�l�Ȃ�)
} sys_sk_alm_t;


/********************************************************************
  �\���́F�r���@�ꉉ�Z���� [2003.06.04]
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
#define SYS_DR_ENZAN_MODE_INPT  1       //���͒l
typedef struct tagSysDrainEnzan {
	short   sno;            //�����ǔ�
	short   shsr_mode;      //���r���ʉ��Z����         (1:���͒l, �ȊO:�����)
	short   chyr_mode;      //�����ʉ��Z����           (1:���͒l, �ȊO:�����)
	short   rs01_mode;      //�\���P���Z����           (1:���͒l, �ȊO:�����)
	short   rs02_mode;      //�\���Q���Z����           (1:���͒l, �ȊO:�����)
	short   rs03_mode;      //�\���R���Z����           (1:���͒l, �ȊO:�����)
	short   rs04_mode;      //�\���S���Z����           (1:���͒l, �ȊO:�����)
	short   rs05_mode;      //�\���T���Z����           (1:���͒l, �ȊO:�����)
	short   rs06_mode;      //�\���U���Z����           (1:���͒l, �ȊO:�����)
	short   rs07_mode;      //�\���V���Z����           (1:���͒l, �ȊO:�����)
	short   rs08_mode;      //�\���W���Z����           (1:���͒l, �ȊO:�����)
	short   rs09_mode;      //�\���X���Z����           (1:���͒l, �ȊO:�����)
	short   rs10_mode;      //�\��10���Z����           (1:���͒l, �ȊO:�����)
	short   rs11_mode;      //�\��11���Z����           (1:���͒l, �ȊO:�����)
	short   rs12_mode;      //�\��12���Z����           (1:���͒l, �ȊO:�����)
	short   rs13_mode;      //�\��13���Z����           (1:���͒l, �ȊO:�����)
	short   rs14_mode;      //�\��14���Z����           (1:���͒l, �ȊO:�����)
	short   rs15_mode;      //�\��15���Z����           (1:���͒l, �ȊO:�����)
	short   rs16_mode;      //�\��16���Z����           (1:���͒l, �ȊO:�����)
} sys_dr_enzan_t;


/********************************************************************
  �\���́F�r���@��ُ�l���� [2003.06.04]
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
typedef struct tagSysDrainKentei {
	short   sno;            //�����ǔ�
	double  shsr10_max;     //���r����10���ő�ω���   (���g�p)
	double  shsr30_max;     //���r����30���ő�ω���   (���g�p)
	double  shsr1h_max;     //���r���ʎ��ԍő�ω���   (���g�p)
	double  chyr10_max;     //������10���ő�ω���     (���g�p)
	double  chyr30_max;     //������30���ő�ω���     (���g�p)
	double  chyr1h_max;     //�����ʎ��ԍő�ω���     (���g�p)
	double  rs01_upp;       //�\���P����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs01_low;       //�\���P�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs02_upp;       //�\���Q����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs02_low;       //�\���Q�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs03_upp;       //�\���R����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs03_low;       //�\���R�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs04_upp;       //�\���S����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs04_low;       //�\���S�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs05_upp;       //�\���T����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs05_low;       //�\���T�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs06_upp;       //�\���U����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs06_low;       //�\���U�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs07_upp;       //�\���V����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs07_low;       //�\���V�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs08_upp;       //�\���W����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs08_low;       //�\���W�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs09_upp;       //�\���X����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs09_low;       //�\���X�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs10_upp;       //�\��10����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs10_low;       //�\��10�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs11_upp;       //�\��11����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs11_low;       //�\��11�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs12_upp;       //�\��12����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs12_low;       //�\��12�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs13_upp;       //�\��13����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs13_low;       //�\��13�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs14_upp;       //�\��14����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs14_low;       //�\��14�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs15_upp;       //�\��15����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs15_low;       //�\��15�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs16_upp;       //�\��16����l             (0�`99999.99, �ȊO:����l�Ȃ�)
	double  rs16_low;       //�\��16�����l             (0�`99999.99, �ȊO:����l�Ȃ�)
} sys_dr_kentei_t;


/********************************************************************
  �\���́F�r���@��x���l [2003.06.04]
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
typedef struct tagSysDrainAlm {
	short   sno;            //�����ǔ�
} sys_dr_alm_t;


/********************************************************************
  �\���́F�����H�����Z���� [2003.07.02]
  ����  �Fsno           �����ǔ�        (1�`999, �ȊO:�ݒ�l�Ȃ�)
          �ȉ����L�Q��
********************************************************************/
#define SYS_SE_ENZAN_MODE_CALC1 1       //���Z�����P
#define SYS_SE_ENZAN_MODE_INPT  2       //�����l
typedef struct tagSysSeeEnzan {
	short   sno;            //�����ǔ�
	char    mode;           //�������Z���� (1:���Z�����P,2:�����l, �ȊO:���Z�����P)
} sys_se_enzan_t;



/********************************************************************
  �v���g�^�C�v�錾
********************************************************************/

/**********************************************************
  �֐�  �Finisplit
  �@�\  �F�����񕪊�
  ����  �Fsp                [IN]  ������
        �Fp_name            [OUT] ����
        �Fp_value           [OUT] �l
  �߂�l�F
  ����  �F�h���� = �l�h�̕�����𖼏̂ƒl�ɕ������܂��B
**********************************************************/
void inisplit(char *sp, char *p_name, char *p_value);

/**********************************************************
  �֐�  �Fcsvtrim
  �@�\  �F�O��X�y�[�X�폜
  ����  �Fsp                [I/O] ������
  �߂�l�F
  ����  �F�����񂩂�O�X�y�[�X�A��X�y�[�X���폜���܂��B
**********************************************************/
void csvtrim(char *sp);

/**********************************************************
  �֐�  �Fcsvsplit
  �@�\  �FCSV�����񕪊�
  ����  �Fsp                [IN]  CSV������(�J���}��؂�)
        �Fn                 [IN]  �擾����ʒu
        �Fp_value           [OUT] �擾����������
  �߂�l�F
  ����  �FCSV�����񂩂�w�肳�ꂽ�ʒu�̕�������擾���܂��B
**********************************************************/
void csvsplit(char *sp, int n, char *p_value);

/********************************************************************
  �֐�  �Fcsvsplit_n
  �@�\  �F�����񕪊�(��������������)
  ����  �Fsp                [IN]  ������
        �Fn                 [IN]  �擾����ʒu
        �Fp_value           [OUT] �擾����������
        �Fv_len             [IN]  �擾�̈�̃T�C�Y
  �߂�l�F�擾������
  ����  �F��؂蕶���ŕ�������Ăł����������
        �F�w�肳�ꂽ�ʒu�̕�������擾���܂��B
********************************************************************/
int csvsplit_n(char *sp, int n, char *p_value, int v_len);

/**********************************************************
  �֐�  �Fcsvread
  �@�\  �F�P�s�ǂݍ���
  ����  �Ffp                [IN]  �t�@�C���\����
        �Fdp                [OUT] �ǂݍ��񂾕�����
  �߂�l�F
  ����  �FCSV�t�@�C������P�s�ǂݍ��݂܂��B
**********************************************************/
void csvread(FILE *fp, char *dp);

/**********************************************************
  �֐�  �Fcsvread_n
  �@�\  �F�P�s�ǂݍ���(��������������)
  ����  �Ffp                [IN]  �t�@�C���\����
        �Fdp                [OUT] �ǂݍ��񂾕�����
        �Fd_len             [IN]  �擾�̈�T�C�Y
  �߂�l�F
  ����  �FCSV�t�@�C������P�s�ǂݍ��݂܂��B
**********************************************************/
void csvread_n(FILE *fp, char *dp, int d_len);

/**********************************************************
  �֐�  �FgetMyNumber
  �@�\  �F�����u�ԍ��̎擾
  ����  �Fpt                [OUT] �����u�ԍ�
  �߂�l�F����
        �F     0 = ����
        �F  �ȊO = ���s
  ����  �F
**********************************************************/
int  getMyNumber(sys_mynumber_t *pt);

/**********************************************************
  �֐�  �FgetServerIniFile
  �@�\  �F�T�[�o�������t�@�C���ǂݍ���
  ����  �FofficeID          [IN]  ���������ԍ�
        �FunitID            [IN]  �����u�ԍ�
		�Fpt                [OUT] �T�[�o���
  �߂�l�F����
        �F     0 = ����
        �F  �ȊO = ���s
  ����  �F
**********************************************************/
int  getServerIniFile(int officeID, int unitID, ini_server_t *pt);

/**********************************************************
  �֐�  �FgetSysConstKindInfo
  �@�\  �F�ǎ�ʖ����̗̈揉����
  ����  �Fpt                [IN]  �ǎ�ʊǗ����
  �߂�l�F����
        �F     0 = ����
        �F  �ȊO = ���s
  ����  �F
**********************************************************/
int  getSysConstKindInfo (kind_manager_t *pt);

/**********************************************************
  �֐�  �FsetSysConstKindInfo
  �@�\  �F�ǎ�ʖ����̎擾
  ����  �Fpt                [OUT] �ǎ�ʊǗ����
  �߂�l�F����
        �F     0 = ����
        �F  �ȊO = ���s
  ����  �F
**********************************************************/
int  setSysConstKindInfo (kind_manager_t *pt);

/**********************************************************
  �֐�  �FfreeSysConstKindInfo
  �@�\  �F�ǎ�ʖ����̗̈�J��
  ����  �Fpt                [IN]  �ǎ�ʊǗ����
  �߂�l�F
  ����  �F
**********************************************************/
void freeSysConstKindInfo(kind_manager_t *pt);

/**********************************************************
  �֐�  �FcheckSysConstKindInfo
  �@�\  �F�V�X�e���萔�����X�V�`�F�b�N
  ����  �Fpt                [IN]  �ǎ�ʊǗ����
        �Fdpath             [IN]  �f�[�^�p�X
        �Fpno               [IN]  �\�t�g�ԍ�
  �߂�l�F�X�V�t���O
        �F  TRUE  = �X�V
        �F  FALSE = ���X�V
  ����  �F
**********************************************************/
BOOL checkSysConstKindInfo(kind_manager_t *pt, char *dpath, int pno);

/**********************************************************
  �֐�  �FgetSysConstExio
  �@�\  �F���u�ԒʐM���ǂݍ���
  ����  �Fapp               [IN]  �A�v���P�[�V�����p�X
        �Fsyskind           [IN]  �������
        �FofficeNo          [IN]  �������ԍ�
        �FunitNo            [IN]  ���u�ԍ�
        �FhouroNo           [IN]  ���H�ԍ�
        �Fp                 [OUT] ���u�ԒʐM���
  �߂�l�F����
        �F     0 = ����
        �F  �ȊO = ���s
  ����  �F
**********************************************************/
int  getSysConstExio(char *app, char *syskind, int officeNo, int unitNo, int houroNo, sys_exioinfo_t *p);


void *getSysConstRainEnzan  (char *path, int max);
void *getSysConstRainKentei (char *path, int max);
void *getSysConstRainAlm    (char *path, int max);
void *getSysConstRainSnake  (char *path, int max);
void *getSysConstRainOld    (char *path, int max);
void *getSysConstRiverEnzan (char *path, int max);
void *getSysConstRiverKentei(char *path, int max);
void *getSysConstRiverAlm   (char *path, int max);
void *getSysConstRiverDanmen(char *path, int max);
void *getSysConstRiverHQ    (char *path, int max);
void *getSysConstRiverOld   (char *path, int max);
void *getSysConstDamEnzan   (char *path, int max);
void *getSysConstDamKentei  (char *path, int max);
void *getSysConstDamAlm     (char *path, int max);
void *getSysConstDamShogen  (char *path, int max);
void *getSysConstTideEnzan  (char *path, int max);
void *getSysConstTideKentei (char *path, int max);
void *getSysConstTideAlm    (char *path, int max);
void *getSysConstTideTenmon (char *path, int max);
void *getSysConstTideSiomi  (char *path, int max);
void *getSysConstWindEnzan  (char *path, int max);
void *getSysConstWindKentei (char *path, int max);
void *getSysConstWindAlm    (char *path, int max);
void *getSysConstRelayEnzan (char *path, int max);
void *getSysConstSnowEnzan  (char *path, int max);	// [2003.06.04]
void *getSysConstSnowKentei (char *path, int max);	// [2003.06.04]
void *getSysConstSnowAlm    (char *path, int max);	// [2003.06.04]
void *getSysConstSnowPair   (char *path, int max);	// [2003.06.04]
void *getSysConstTempEnzan  (char *path, int max);	// [2003.06.04]
void *getSysConstTempKentei (char *path, int max);	// [2003.06.04]
void *getSysConstTempAlm    (char *path, int max);	// [2003.06.04]
void *getSysConstRoadEnzan  (char *path, int max);	// [2003.06.04]
void *getSysConstRoadKentei (char *path, int max);	// [2003.06.04]
void *getSysConstRoadAlm    (char *path, int max);	// [2003.06.04]
void *getSysConstSekiEnzan  (char *path, int max);	// [2003.06.04]
void *getSysConstSekiKentei (char *path, int max);	// [2003.06.04]
void *getSysConstSekiAlm    (char *path, int max);	// [2003.06.04]
void *getSysConstDrainEnzan (char *path, int max);	// [2003.06.04]
void *getSysConstDrainKentei(char *path, int max);	// [2003.06.04]
void *getSysConstDrainAlm   (char *path, int max);	// [2003.06.04]
void *getSysConstSeeEnzan   (char *path, int max);	// [2003.07.02]
void *getSysConstRainCastEnzan  (char *path, int max);	// [2004.01.09]
void *getSysConstRainCastKentei (char *path, int max);	// [2004.01.09]
void *getSysConstRainCastAlm    (char *path, int max);	// [2004.01.09]
void *getSysConstRainCastSnake  (char *path, int max);	// [2004.01.09]
void *getSysConstRiverCastEnzan (char *path, int max);	// [2004.01.14]
void *getSysConstRiverCastKentei(char *path, int max);	// [2004.01.14]
void *getSysConstRiverCastAlm   (char *path, int max);	// [2004.01.14]
void *getSysConstRiverCastHQ    (char *path, int max);	// [2004.01.14]

#endif
