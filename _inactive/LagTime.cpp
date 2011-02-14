///////////////////////////////////////////////////////////////////////////
//
//  �y�ؑ����h�Џ��V�X�e���|���O�^�C���N���X
//
////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CLagTime.h"


/*-------------------------------------------------------------------
*	�@�\�F�R���X�g���N�^
*	�����F
*	�ߒl�F
*	�����F
-------------------------------------------------------------------*/
CLagTime::CLagTime()
{

	memset(&inf, 0, sizeof(inf));
	lag_minus = 0;
	lag_plus  = 0;
}

/*-------------------------------------------------------------------
*	�@�\�F�f�X���N�^
*	�����F
*	�ߒl�F
*	�����F
-------------------------------------------------------------------*/
CLagTime::~CLagTime()
{

}


/*-------------------------------------------------------------------
*	�@�\�F������
*	�����Fno				[IN]  ���H�ԍ�
        �Fm1				[IN]  ���O�^�C��(�|�b)
        �Fm2				[IN]  ���O�^�C��(�{�b)
*	�ߒl�F����
        �F   0 = ����
*	�����F
-------------------------------------------------------------------*/
int CLagTime::init(int no, int m1, int m2)
{

	// ���H�ԍ����i�[
	houroNo = no;

	// ���O�^�C�������i�[����
	lag_minus = m1;
	lag_plus  = m2;

	return 0;
}

/*-------------------------------------------------------------------
*	�@�\�F���菈��
*	�����F
*	�ߒl�F����
        �F   1 = ���O�^�C����
        �F   0 = ���O�^�C���ȊO
*	�����F
-------------------------------------------------------------------*/
int CLagTime::judgment()
{
	time_t	now_dot;
	time_t	lag_dot;
	time_t	start_dot;
	time_t	end_dot;
	tm24_t	tm24_now;
	tm24_t	tm24_wk;
	time_t	tm;

	// ���ݎ����擾
	dll_getNowTime24(&tm24_now);
	// ���ݎ�����胉�O�^�C���E�`�F�b�N���s���B
	memcpy(&tm24_wk, &tm24_now, sizeof(tm24_wk));
	// ����10, 20, 30, 40, 50, 00�ɂ���
	if ((tm24_wk.tm_min % 10) > (lag_plus / 60)) {
		tm24_wk.tm_min = tm24_wk.tm_min + (10 - (tm24_wk.tm_min % 10));
		if (tm24_wk.tm_min >= 60) {
			dll_mktime24(&tm24_wk, &tm);
			dll_localtime24_0(tm, &tm24_wk);
		}
	} else {
		tm24_wk.tm_min = ((tm24_wk.tm_min / 10) * 10);
	}
	tm24_wk.tm_sec = 0;
	if ((tm24_wk.tm_hour == 0) && (tm24_wk.tm_min == 0)) {
		// 24:00�ɂ���
		dll_timeAddMin(&tm24_wk, -10);
		tm24_wk.tm_hour = 24;
		tm24_wk.tm_min  = 00;
	}
	// ���ݎ�����DOT�֕ϊ�
	dll_mktime24(&tm24_now, &now_dot);
	// ���莞������DOT�֕ϊ�
	dll_mktime24(&tm24_wk, &lag_dot);
	// �J�n�A�I�����O�^�C�����쐬
	start_dot = lag_dot + lag_minus;
	end_dot   = lag_dot + lag_plus;
	if ((start_dot <= now_dot) && (now_dot <= end_dot)) {
		// ���O�^�C����
		inf.flg = 1;
	} else {
		// ���O�^�C���O
		inf.flg = 0;
	}

	// �ϑ�����
	inf.time24 = tm24_wk;
	dll_mktime24(&inf.time24, &inf.time);
	return inf.flg;
}
