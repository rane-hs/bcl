#include "stdafx.h"
#include "memAccess.h"


/*-------------------------------------------------------------------
  ���[�J���֐��錾
-------------------------------------------------------------------*/
int  timeCmp(time_t t1, mem_obstime_t t2);
void obsToTime(mem_obstime_t *p, time_t *tm);
void timeToObs(time_t tm, mem_obstime_t *p);

mem_datamem_t *selectDataMemory(mem_manager_t *mp, int typ, int kno, time_t tim);
int  openDataMemory (mem_manager_t *mp, kind_manager_t *kp);
void closeDataMemory(mem_manager_t *mp);

void initDataMemoryHM  (mem_manager_t *mp, int kno, char *p);
void initDataMemoryDay (mem_manager_t *mp, int kno, char *p);
void initDataMemory    (mem_manager_t *mp, mem_datamem_t *dp, time_t tim);
int  writeLatestMemory (mem_manager_t *mp, int typ, int kno, time_t tim, int sno, char *dp);
int  writeDataMemoryNow(mem_manager_t *mp, int typ, int kno, time_t tim, mem_datamem_t *dp);
int  writeDataMemory   (mem_manager_t *mp, int typ, int kno, time_t tim, char *dp);
int  writeDataMemoryS  (mem_manager_t *mp, int typ, int kno, time_t tim, int sno, char *dp);
int  writeDataMemoryI  (mem_manager_t *mp, int typ, time_t tim, int cnt, mem_reqdata_t *dp);
int  readDataMemoryI   (mem_manager_t *mp, int typ, time_t tim, int cnt, mem_reqdata_t *dp);

mem_datamem_t *checkDataMemory (mem_manager_t *mp, int typ, int kno, time_t tim);
int getUpToDateTime(mem_manager_t *mp, int typ, int kno, time_t *);

//2003.06.23 �ǉ��i��������j----------------------------------------
//���ɍX�V����Ă���ǂ͏㏑�����Ȃ��o�[�W����
int  writeDataMemoryS_nOW(mem_manager_t *mp, int typ, int kno, time_t tim, int sno, char *dp);
int  writeDataMemoryI_nOW(mem_manager_t *mp, int typ, time_t tim, int cnt, mem_reqdata_t *dp);
//2003.06.23 �ǉ��i�����܂Łj----------------------------------------


/*-------------------------------------------------------------------
  ���W���[�����ϐ�
-------------------------------------------------------------------*/


/*---------------------------------------------------------
*	�@�\�F
*	�����Ft1            [IN]  �w�莞��
        �Ft2            [IN]  �X�V����
*	�ߒl�F��r����
        �F  -1 = �w�莞�� < �X�V����
        �F   0 = �w�莞�� = �X�V����
        �F   1 = �w�莞�� > �X�V����
        �F   2 = �w�莞�� > ���� > �X�V����
*	�����F�������ރf�[�^�̎w�莞����
        �F�X�V��񃁃������̍X�V�����Ɣ�r���܂��B
---------------------------------------------------------*/
int timeCmp(time_t t1, mem_obstime_t t2)
{
	tm24_t tm24;
	time_t tim;
	time_t ntim;
	int ret;

	//�X�V�������O�Ȃ�
	if (t2.mm == 0 || t2.dd == 0) {
//2005.03.04 �C���i��������j----------------------------------------
//�X�V�������O�Ȃ琳�����ׂ��ł��鎖�Ƃ���
//		ntim = 0;
		return 2;
//2005.03.04 �C���i�����܂Łj----------------------------------------
	//�X�V�������ݒ肳��Ă���
	} else {
		tm24.tm_year = t2.yy;
		tm24.tm_mon  = t2.mm;
		tm24.tm_mday = t2.dd;
		tm24.tm_hour = t2.hh;
		tm24.tm_min  = t2.nn;
		tm24.tm_sec  = 0;
		ntim = 0;
		dll_mktime24(&tm24, &ntim);
	}
	//��r
	ret = t1 - ntim;
	//�V��������
	if (ret > 0) {
		//�X�V�����𐳎��ɐ؂�グ
		tm24.tm_hour++;
		tm24.tm_min = 0;
		ntim = 0;
		dll_mktime24(&tm24, &ntim);
		//�w�莞���𐳎��ɐ؂�̂�
		dll_localtime24(t1, &tm24);
		tm24.tm_min = 0;
		tim = 0;
		dll_mktime24(&tm24, &tim);

		//�������ׂ��ł���
		if (tim >= ntim)
			ret = 2;
		else
			ret = 1;
	//��������
	} else if (ret == 0) {
		dll_localtime24(t1, &tm24);
		if (tm24.tm_min == 0)
			ret = 2;
		else
			ret = 1;
	//�Â�����
	} else {
		ret = -1;
	}
	return ret;
}

/*---------------------------------------------------------
*	�@�\�F�����ϊ�(�X�V�����^��time_t�^)
*	�����Fp                 [IN]  �ϊ����̎���
        �Ftm                [OUT] �ϊ���̎���
*	�ߒl�F
*	�����F
---------------------------------------------------------*/
void obsToTime(mem_obstime_t *p, time_t *tm)
{
	tm24_t tm24;

	*tm = 0;
	tm24.tm_year = p->yy;
	tm24.tm_mon  = p->mm;
	tm24.tm_mday = p->dd;
	tm24.tm_hour = p->hh;
	tm24.tm_min  = p->nn;
	tm24.tm_sec  = 0;
	dll_mktime24(&tm24, tm);
}

/*---------------------------------------------------------
*	�@�\�F�����ϊ�(time_t�^���X�V�����^)
*	�����Ftm                [IN]  �ϊ����̎���
        �Fp                 [OUT] �ϊ���̎���
*	�ߒl�F
*	�����F
---------------------------------------------------------*/
void timeToObs(time_t tm, mem_obstime_t *p)
{
	tm24_t tm24;

	dll_localtime24(tm, &tm24);
	p->yy = tm24.tm_year;
	p->mm = tm24.tm_mon;
	p->dd = tm24.tm_mday;
	p->hh = tm24.tm_hour;
	p->nn = tm24.tm_min;
}

/*---------------------------------------------------------
*	�@�\�F���L�f�[�^�������I��
*	�����Fmp                [I/O] �f�[�^�������Ǘ����
        �Ftyp               [IN]  �f�[�^�������^�C�v
        �Fkno               [IN]  �ǎ�ʔԍ�
        �Ftim               [IN]  �f�[�^����
*	�ߒl�F���L�f�[�^���������
*	�����F
---------------------------------------------------------*/
mem_datamem_t *selectDataMemory(mem_manager_t *mp, int typ, int kno, time_t tim)
{
	mem_datamem_t *pp;
	tm24_t tm24;
	div_t  dv;

	//�������ُ�Ȃ�G���[
	if (tim <= 0)
		return NULL;

	//�����擾
	dll_localtime24_0(tim, &tm24);
	dv = div(tm24.tm_min, 10);

	//�^�C�v�ɂ�胁������I��
	switch (typ) {
	case MEM_TYPE_INPUT_1M:
		pp = &mp->tmem[kno-1].min[dv.rem];
		break;
	case MEM_TYPE_INPUT_HM:
		pp = &mp->tmem[kno-1].tim[dv.quot];
		break;
	case MEM_TYPE_DATA_1M:
		pp = &mp->dmem[kno-1].min[dv.rem];
		break;;
	case MEM_TYPE_DATA_HM:
		pp = &mp->dmem[kno-1].tim[dv.quot];
		break;
	case MEM_TYPE_DAILY:
		pp = &mp->dmem[kno-1].day;
		break;
	default:
		pp = NULL;
		break;
	}
	return pp;
}

/*---------------------------------------------------------
*	�@�\�F���L�f�[�^�������I�[�v��
*	�����Fmp                [OUT] ���L�f�[�^���������
*	�ߒl�F
*	�����F
  ���l  �F2003.06.10  Ver 1.3 �ɑΉ��̂��ߕύX
---------------------------------------------------------*/
int openDataMemory(mem_manager_t *mp, kind_manager_t *kp)
{
	mem_manager_t mMem;
	char mname[32];
	int  ret;
	int  i,j;
	char *p;
	int  r_hm, r_day, r_new, r_now;
	int  d_hm, d_day, d_new, d_now;


	//�p�����[�^�̊m�F
	if (mp == NULL)
		return ERR_MEM_MP;
	if (kp == NULL)
		return ERR_MEM_KP;
	if (kp->pKind == NULL)
		return ERR_MEM_KP;


	//�߂�l�̏�����
	memset(mp, 0, sizeof(mem_manager_t));

	//�ҏW�p�̈�̏�����
	memset(&mMem, 0, sizeof(mem_manager_t));
	//�ǎ�ʏ��̐ݒ�
	mMem.kp = kp;

	//���̓f�[�^������
	for (i = 0; i < mMem.kp->max; i++) {
		if (mMem.kp->pKind[i].kind == NULL) continue;

		//���L�������̃I�[�v��
		sprintf(mname, "INPMEM_%d", i+1);
		ret = dll_openSharedMemory(&mMem.tmem[i].mp, mname);

		//�|�C���^�ƃT�C�Y�̐ݒ�
		p = mMem.tmem[i].mp.mem;
		r_hm  = sizeof(mem_obstime_t) + sizeof(char) * mMem.kp->pKind[i].kind->max;
		d_hm  = sizeof(bf_data_t) * mMem.kp->pKind[i].kind->max * mMem.kp->pKind[i].kind->titem;
		r_day = 0;
		d_day = 0;
		r_new = (sizeof(mem_obstime_t) + sizeof(char)) * mMem.kp->pKind[i].kind->max;
		d_new = sizeof(bf_data_t) * mMem.kp->pKind[i].kind->max * mMem.kp->pKind[i].kind->titem;
		r_now = 0;
		d_now = 0;

		//�P���f�[�^
		for (j = 0; j < 10; j++) {
			mMem.tmem[i].min[j].typ  = MEM_TYPE_INPUT_1M;
			mMem.tmem[i].min[j].kno  = i + 1;
			mMem.tmem[i].min[j].max  = mMem.kp->pKind[i].kind->max;
			mMem.tmem[i].min[j].item = mMem.kp->pKind[i].kind->titem;
			if (mMem.tmem[i].mp.mem != NULL && d_hm > 0) {
				mMem.tmem[i].min[j].r.mem = p;
				mMem.tmem[i].min[j].d.mem = p + r_hm;
				p += r_hm + d_hm;
			}
		}
		//���莞�f�[�^
		for (j = 0; j < 6; j++) {
			mMem.tmem[i].tim[j].typ  = MEM_TYPE_INPUT_HM;
			mMem.tmem[i].tim[j].kno  = i + 1;
			mMem.tmem[i].tim[j].max  = mMem.kp->pKind[i].kind->max;
			mMem.tmem[i].tim[j].item = mMem.kp->pKind[i].kind->titem;
			if (mMem.tmem[i].mp.mem != NULL && d_hm > 0) {
				mMem.tmem[i].tim[j].r.mem = p;
				mMem.tmem[i].tim[j].d.mem = p + r_hm;
				p += r_hm + d_hm;
			}
		}
		//���W�v�f�[�^�͖��g�p

		//�ŐV�f�[�^
		mMem.tmem[i].now.typ  = MEM_TYPE_NOW;
		mMem.tmem[i].now.kno  = i + 1;
		mMem.tmem[i].now.max  = mMem.kp->pKind[i].kind->max;
		mMem.tmem[i].now.item = mMem.kp->pKind[i].kind->titem;
		if (mMem.tmem[i].mp.mem != NULL && d_new > 0 ) {
			mMem.tmem[i].now.r.mem = p;
			mMem.tmem[i].now.d.mem = p + r_new;
			p += r_new + d_new;
		}

		//�P�������f�[�^�͖��g�p
		//���莞�����f�[�^�͖��g�p

	}

	//���Z�f�[�^������
	for (i = 0; i < mMem.kp->max; i++) {
		if (mMem.kp->pKind[i].kind == NULL) continue;

		//���L�������̃I�[�v��
		sprintf(mname, "OPEMEM_%d", i+1);
		ret = dll_openSharedMemory(&mMem.dmem[i].mp, mname);

		//�|�C���^�ƃT�C�Y�̐ݒ�
		p = mMem.dmem[i].mp.mem;
		r_hm  = sizeof(mem_obstime_t) + sizeof(char) * mMem.kp->pKind[i].kind->max;
		d_hm  = sizeof(bf_data_t) * mMem.kp->pKind[i].kind->max * mMem.kp->pKind[i].kind->titem;
		r_day = sizeof(mem_obstime_t) + sizeof(char) * mMem.kp->pKind[i].kind->max;
		d_day = sizeof(bf_data_t) * mMem.kp->pKind[i].kind->max * mMem.kp->pKind[i].kind->ditem;
		r_new = (sizeof(mem_obstime_t) + sizeof(char)) * mMem.kp->pKind[i].kind->max;
		d_new = sizeof(bf_data_t) * mMem.kp->pKind[i].kind->max * mMem.kp->pKind[i].kind->titem;
//2005.03.04 �C���i��������j----------------------------------------
//�X�V��񃁃����̃T�C�Y�v�Z���Ԉ���Ă���
//		r_now = sizeof(mem_obstime_t) + sizeof(char) * mMem.kp->pKind[i].kind->max;
		r_now = sizeof(mem_obstime_t) * mMem.kp->pKind[i].kind->max;
//2005.03.04 �C���i�����܂Łj----------------------------------------
		d_now = sizeof(bf_data_t) * mMem.kp->pKind[i].kind->max * mMem.kp->pKind[i].kind->titem;

		//�P���f�[�^
		for (j = 0; j < 10; j++) {
			mMem.dmem[i].min[j].typ  = MEM_TYPE_DATA_1M;
			mMem.dmem[i].min[j].kno  = i + 1;
			mMem.dmem[i].min[j].max  = mMem.kp->pKind[i].kind->max;
			mMem.dmem[i].min[j].item = mMem.kp->pKind[i].kind->titem;
			if (mMem.dmem[i].mp.mem != NULL && d_hm > 0) {
				mMem.dmem[i].min[j].r.mem = p;
				mMem.dmem[i].min[j].d.mem = p + r_hm;
				p += r_hm + d_hm;
			}
		}
		//���莞�f�[�^
		for (j = 0; j < 6; j++) {
			mMem.dmem[i].tim[j].typ  = MEM_TYPE_DATA_HM;
			mMem.dmem[i].tim[j].kno  = i + 1;
			mMem.dmem[i].tim[j].max  = mMem.kp->pKind[i].kind->max;
			mMem.dmem[i].tim[j].item = mMem.kp->pKind[i].kind->titem;
			if (mMem.dmem[i].mp.mem != NULL && d_hm) {
				mMem.dmem[i].tim[j].r.mem = p;
				mMem.dmem[i].tim[j].d.mem = p + r_hm;
				p += r_hm + d_hm;
			}
		}

		//���W�v�f�[�^
		mMem.dmem[i].day.typ  = MEM_TYPE_DAILY;
		mMem.dmem[i].day.kno  = i + 1;
		mMem.dmem[i].day.max  = mMem.kp->pKind[i].kind->max;
		mMem.dmem[i].day.item = mMem.kp->pKind[i].kind->ditem;
		if (mMem.dmem[i].mp.mem != NULL && d_day) {
			mMem.dmem[i].day.r.mem = p;
			mMem.dmem[i].day.d.mem = p + r_day;
			p += r_day + d_day;
		}

		//�ŐV�f�[�^
		mMem.dmem[i].now.typ  = MEM_TYPE_NOW;
		mMem.dmem[i].now.kno  = i + 1;
		mMem.dmem[i].now.max  = mMem.kp->pKind[i].kind->max;
		mMem.dmem[i].now.item = mMem.kp->pKind[i].kind->titem;
		if (mMem.dmem[i].mp.mem != NULL && d_new) {
			mMem.dmem[i].now.r.mem = p;
			mMem.dmem[i].now.d.mem = p + r_new;
			p += r_new + d_new;
		}

		//�P�������f�[�^
		mMem.dmem[i].now_1m.typ  = MEM_TYPE_NOW_1M;
		mMem.dmem[i].now_1m.kno  = i + 1;
		mMem.dmem[i].now_1m.max  = mMem.kp->pKind[i].kind->max;
		mMem.dmem[i].now_1m.item = mMem.kp->pKind[i].kind->titem;
		if (mMem.dmem[i].mp.mem != NULL && d_now) {
			mMem.dmem[i].now_1m.r.mem = p;
			mMem.dmem[i].now_1m.d.mem = p + r_now;
			p += r_now + d_now;
		}

		//���莞�����f�[�^
		mMem.dmem[i].now_hm.typ  = MEM_TYPE_NOW_HM;
		mMem.dmem[i].now_hm.kno  = i + 1;
		mMem.dmem[i].now_hm.max  = mMem.kp->pKind[i].kind->max;
		mMem.dmem[i].now_hm.item = mMem.kp->pKind[i].kind->titem;
		if (mMem.dmem[i].mp.mem != NULL && d_now > 0) {
			mMem.dmem[i].now_hm.r.mem = p;
			mMem.dmem[i].now_hm.d.mem = p + r_now;
			p += r_now + d_now;
		}

	}

	//���b�N�p�n���h���̎擾
	dll_createLock(NULL, LOCK_NAME_INPUT, &mMem.hLockInput);
	dll_createLock(NULL, LOCK_NAME_DATA,  &mMem.hLockData);
	dll_createLock(NULL, LOCK_NAME_DAILY, &mMem.hLockDaily);

	//�߂�l�̐ݒ�
	memcpy(mp, &mMem, sizeof(mMem));

	return ERR_MEM_SUCCESS;
}

/*---------------------------------------------------------
*	�@�\�F���L�f�[�^�������N���[�Y
*	�����Fmp                [IN]  ���L�f�[�^���������
*	�ߒl�F
*	�����F
  ���l  �F2003.06.10  Ver 1.3 �ɑΉ��̂��ߕύX
---------------------------------------------------------*/
void closeDataMemory(mem_manager_t *mp)
{
	int i;

	//�p�����[�^�̊m�F
	if (mp == NULL)
		return;
	if (mp->kp == NULL)
		return;
	if (mp->kp->pKind == NULL)
		return;

	//���̓f�[�^������
	for (i = 0; i < mp->kp->max; i++) {
		if (mp->kp->pKind[i].kind == NULL) continue;
		//�������̉��
		dll_freeSharedMemory(&mp->tmem[i].mp);
	}

	//���Z�f�[�^������
	for (i = 0; i < mp->kp->max; i++) {
		if (mp->kp->pKind[i].kind == NULL) continue;
		//�������̉��
		dll_freeSharedMemory(&mp->dmem[i].mp);
	}

	//���b�N�p�n���h���̊J��
	dll_closeLock(mp->hLockInput);
	dll_closeLock(mp->hLockData);
	dll_closeLock(mp->hLockDaily);

}

/*---------------------------------------------------------
*	�֐��FinitDataMemoryHM
*	�@�\�F���L�f�[�^������������(�P���E���莞�p)
*	�����Fkno               [IN]  �ǎ�ʔԍ�
        �Fp                 [I/O] �f�[�^�������|�C���^
*	�ߒl�F
*	�����F�f�[�^�����������������܂��B
        �F�ǎ�ʔԍ��ɂ��ǉғ����E�ǎ��������Q�Ƃ�
		�F�f�[�^�̃t���O�����������܂��B
---------------------------------------------------------*/
void initDataMemoryHM(mem_manager_t *mp, int kno, char *p)
{
	bf_data_t *dp = (bf_data_t *)p;
	int max_item;
	int i,j;
	unsigned char f1;

	//�p�����[�^�̊m�F
	if (mp == NULL)
		return;
	if (mp->kp == NULL)
		return;
	if (mp->kp->pKind == NULL)
		return;
	if (kno < 1 || kno > mp->kp->max)
		return;
	if (mp->kp->pKind[kno-1].kind == NULL)
		return;
	if (p == NULL)
		return;

	//�ő區�ڐ��̌���
	max_item = mp->kp->pKind[kno-1].kind->titem;

	//�������̃N���A
	memset(dp,  0, sizeof(bf_data_t) * mp->kp->pKind[kno-1].kind->max * max_item);

	//�f�[�^���̃t���O������
	for (i = 0; i < mp->kp->pKind[kno-1].kind->max; i++) {
		//����
		if (mp->kp->pKind[kno-1].local[i].entry == SYS_LOCAL_ENTRY_ON) {
			//�ێ璆
			if (mp->kp->pKind[kno-1].action[i].action == SYS_ACTION_ACTION_MAINTE) {
				f1 = BF_MAINTE;
			//��
			} else if (mp->kp->pKind[kno-1].action[i].action == SYS_ACTION_ACTION_CLOSE) {
				f1 = BF_CLOSE;
			//���O�^�C���A�E�g(������)
			} else {
				f1 = BF_TOUT;
			}
		//�����
		} else {
			f1= BF_DISABLE;
		}
		//�t���O�̏�����
		for (j = 0; j < max_item; j++) {
			dp[i*max_item+j].f[0] = f1;
		}
	}
}

/*---------------------------------------------------------
*	�֐��FinitDataMemoryDay
*	�@�\�F���L�f�[�^������������(���W�v�p)
*	�����Fkno               [IN]  �ǎ�ʔԍ�
        �Fp                 [I/O] �f�[�^�������|�C���^
*	�ߒl�F
*	�����F�f�[�^�����������������܂��B
        �F�ǎ�ʔԍ��ɂ��ǉғ����E�ǎ��������Q�Ƃ�
		�F�f�[�^�̃t���O�����������܂��B
---------------------------------------------------------*/
void initDataMemoryDay(mem_manager_t *mp, int kno, char *p)
{
	bf_data_t *dp = (bf_data_t *)p;
	int max_item;
	int i,j;
	unsigned char f1;

	//�p�����[�^�̊m�F
	if (mp == NULL)
		return;
	if (mp->kp == NULL)
		return;
	if (mp->kp->pKind == NULL)
		return;
	if (kno < 1 || kno > mp->kp->max)
		return;
	if (mp->kp->pKind[kno-1].kind == NULL)
		return;
	if (p == NULL)
		return;

	//�ő區�ڐ��̌���
	max_item = mp->kp->pKind[kno-1].kind->ditem;

	//�������̃N���A
	memset(dp,  0, sizeof(bf_data_t) * mp->kp->pKind[kno-1].kind->max * max_item);

	//�f�[�^���̃t���O������
	for (i = 0; i < mp->kp->pKind[kno-1].kind->max; i++) {
		//����
		if (mp->kp->pKind[kno-1].local[i].entry == SYS_LOCAL_ENTRY_ON) {
			//�ێ璆
			if (mp->kp->pKind[kno-1].action[i].action == SYS_ACTION_ACTION_MAINTE) {
				f1 = BF_MAINTE;
			//��
			} else if (mp->kp->pKind[kno-1].action[i].action == SYS_ACTION_ACTION_CLOSE) {
				f1 = BF_CLOSE;
			//���O�^�C���A�E�g(������)
			} else {
				f1 = BF_TOUT;
			}
		//�����
		} else {
			f1= BF_DISABLE;
		}
		//�t���O�̏�����
		for (j = 0; j < max_item; j++) {
			dp[i*max_item+j].f[0] = f1;
		}
	}
}

/*---------------------------------------------------------
*	�֐��FinitDataMemory
*	�@�\�F���L�f�[�^������������(�P���E���莞�p)
*	�����Fkno               [IN]  �ǎ�ʔԍ�
        �Ftim               [IN]  �f�[�^����
        �Fp                 [I/O] �f�[�^�������|�C���^
*	�ߒl�F
*	�����F�X�V��񃁃����ƃf�[�^�����������������܂��B
---------------------------------------------------------*/
void initDataMemory(mem_manager_t *mp, mem_datamem_t *dp, time_t tim)
{
	//�X�V��񃁃����̃N���A
	memset(dp->r.mem, 0, sizeof(mem_obstime_t) + sizeof(char) * dp->max);
	//�����̐ݒ�
	timeToObs(tim, (mem_obstime_t *)dp->r.mem);

	//�f�[�^�������̏�����
	if (dp->typ == MEM_TYPE_DAILY)
		initDataMemoryDay(mp, dp->kno, dp->d.mem);
	else
		initDataMemoryHM(mp, dp->kno, dp->d.mem);
}

/*---------------------------------------------------------
*	�֐��FwriteLatestMemory
*	�@�\�F�ŐV�f�[�^��������������
*	�����Fmp                [I/O] �f�[�^�������Ǘ����
        �Ftyp               [IN]  �f�[�^�������^�C�v
        �Fkno               [IN]  �ǎ�ʔԍ�
        �Ftim               [IN]  �f�[�^����
        �Fsno               [IN]  �ǔԍ�
        �Fdp                [IN]  �������݃f�[�^
*	�ߒl�F
*	�����F
---------------------------------------------------------*/
int writeLatestMemory(mem_manager_t *mp, int typ, int kno, time_t tim, int sno, char *dp)
{
	mem_datamem_t *pp;
	mem_obstime_t *tp;
	char      *rp;
	bf_data_t *bp;
	tm24_t    tm24;

	//�p�����[�^�̊m�F
	if (mp == NULL)
		return ERR_MEM_MP;
	if (mp->kp == NULL)
		return ERR_MEM_MP;
	if (mp->kp->pKind == NULL)
		return ERR_MEM_MP;
	//�f�[�^�������^�C�v
	if (typ < MEM_TYPE_INPUT_1M || typ > MEM_TYPE_DAILY)
		return ERR_MEM_TYPE;
	//�ǎ�ʔԍ�
	if (kno < 1 || kno > mp->kp->max)
		return ERR_MEM_KNO;
	if (mp->kp->pKind[kno-1].kind == NULL)
		return ERR_MEM_KNO;
	//�f�[�^����
	if (tim == 0)
		return ERR_MEM_TIME;
	//�������݃f�[�^
	if (dp == NULL)
		return ERR_MEM_DATA;

	//���̓f�[�^������
	if (typ == MEM_TYPE_INPUT_1M || typ == MEM_TYPE_INPUT_HM)
		pp = (mem_datamem_t *)&mp->tmem[kno-1].now;
	//���Z�f�[�^������
	else if (typ == MEM_TYPE_DATA_1M || typ == MEM_TYPE_DATA_HM)
		pp = (mem_datamem_t *)&mp->dmem[kno-1].now;
	else
		return 0;

	tp = (mem_obstime_t *)pp->r.mem;
	rp = pp->r.mem + sizeof(mem_obstime_t) * pp->max;
	bp = (bf_data_t *)pp->d.mem;

	//�������͍쐬����Ă��Ȃ�
	if (pp->r.mem == NULL)
		return ERR_MEM_MEMORY;
	if (pp->d.mem == NULL)
		return ERR_MEM_MEMORY;
	//�ǔԍ�
	if (sno < 1 || sno > pp->max)
		return ERR_MEM_SNO;

	//�b���O�ɂ���
	dll_localtime24_0(tim, &tm24);

	//�������ɏ�������
	memcpy(&bp[(sno-1) * pp->item], dp, sizeof(bf_data_t) * pp->item);
	tp[sno-1].yy = tm24.tm_year;
	tp[sno-1].mm = tm24.tm_mon;
	tp[sno-1].dd = tm24.tm_mday;
	tp[sno-1].hh = tm24.tm_hour;
	tp[sno-1].nn = tm24.tm_min;
	if (rp[sno-1] >= 255)
		rp[sno-1] = 1;
	else
		rp[sno-1]++;

	return 0;
}

/*---------------------------------------------------------
*	�֐��FwriteDataMemoryNow
*	�@�\�F�����f�[�^��������������
*	�����Fmp                [I/O] �f�[�^�������Ǘ����
        �Ftyp               [IN]  �f�[�^�������^�C�v
        �Fkno               [IN]  �ǎ�ʔԍ�
        �Ftim               [IN]  �f�[�^����
        �Fdp                [IN]  �������݃f�[�^
*	�ߒl�F
*	�����F
---------------------------------------------------------*/
int writeDataMemoryNow(mem_manager_t *mp, int typ, int kno, time_t tim, mem_datamem_t *dp)
{
	mem_datamem_t *pp;
	mem_obstime_t *s_ip;
	bf_data_t     *s_dp;
	char          *d_ip;
	bf_data_t     *d_dp;
	tm24_t tm24;
	time_t wtim;
	int i, j;
	int flg;
	int ret;


	//�p�����[�^�̊m�F
	if (mp == NULL)
		return ERR_MEM_MP;
	if (mp->kp == NULL)
		return ERR_MEM_MP;
	if (mp->kp->pKind == NULL)
		return ERR_MEM_MP;
	//�f�[�^�������^�C�v
	if (typ < MEM_TYPE_INPUT_1M || typ > MEM_TYPE_DAILY)
		return ERR_MEM_TYPE;
	//�ǎ�ʔԍ�
	if (kno < 1 || kno > mp->kp->max)
		return ERR_MEM_KNO;
	if (mp->kp->pKind[kno-1].kind == NULL)
		return ERR_MEM_KNO;
	//�f�[�^����
	if (tim == 0)
		return ERR_MEM_TIME;
	//�������݃f�[�^
	if (dp == NULL)
		return ERR_MEM_DATA;

	//�����f�[�^������
	switch (typ) {
	case MEM_TYPE_INPUT_1M:
		return 0;
		break;
	case MEM_TYPE_INPUT_HM:
		return 0;
		break;
	case MEM_TYPE_DATA_1M:
		pp = (mem_datamem_t *)&mp->dmem[kno-1].now_1m;
		break;
	case MEM_TYPE_DATA_HM:
		pp = (mem_datamem_t *)&mp->dmem[kno-1].now_hm;
		break;
	default:
		return 0;
		break;
	}
	s_ip = (mem_obstime_t *)pp->r.mem;
	s_dp = (bf_data_t *)pp->d.mem;
	d_ip = (char *)dp->r.mem + sizeof(mem_obstime_t);
	d_dp = (bf_data_t *)dp->d.mem;

	//�������͍쐬����Ă��Ȃ�
	if (pp->r.mem == NULL)
		return ERR_MEM_MEMORY;
	if (pp->d.mem == NULL)
		return ERR_MEM_MEMORY;


	//�b���O�ɂ���
	wtim = 0;
	dll_localtime24_0(tim, &tm24);
	dll_mktime24(&tm24, &wtim);

	//������ǂ̔���
	for (i = 0; i < pp->max; i++) {
		if (mp->kp->pKind[kno-1].local[i].entry != SYS_LOCAL_ENTRY_ON) {
			for (j = 0; j < pp->item; j++) {
				s_dp[i * pp->item + j].dt = 0;
				s_dp[i * pp->item + j].f[0] = BF_DISABLE;
				s_dp[i * pp->item + j].f[1] = 0;
				s_dp[i * pp->item + j].f[2] = 0;
				s_dp[i * pp->item + j].f[3] = 0;
			}
			memset(&s_ip[i], 0, sizeof(mem_obstime_t));
		}
	}
	for (i = 0; i < pp->max; i++) {
		//�����̔���
		ret = timeCmp(wtim, s_ip[i]);
		//�����������͐������ׂ��ł���
		if (ret == 2) {
			//�X�V
			if (d_ip[i] == 0) continue;
			//����
			if (mp->kp->pKind[kno-1].local[i].entry == SYS_LOCAL_ENTRY_ON) {
				memcpy(&s_dp[i * pp->item], &d_dp[i * pp->item], sizeof(bf_data_t) * pp->item);
				s_ip[i].yy = tm24.tm_year;
				s_ip[i].mm = tm24.tm_mon;
				s_ip[i].dd = tm24.tm_mday;
				s_ip[i].hh = tm24.tm_hour;
				s_ip[i].nn = tm24.tm_min;
			}
		//�X�V���t�@�C���̎������V����
		} else if (ret >= 0) {
			//�X�V
			if (d_ip[i] == 0) continue;
			//����
			if (mp->kp->pKind[kno-1].local[i].entry == SYS_LOCAL_ENTRY_ON) {
				//�t���O�P�̊m�F
				flg = FALSE;
				for (j = 0; j < pp->item; j++) {
					if (d_dp[i * pp->item + j].f[0] < 128) {
						flg = TRUE;
						break;
					}
				}
				//����ȃf�[�^������Ώ�������
				if (flg == TRUE) {
//2003.05.29�i�C���j��������-----------------------------------------
//����f�[�^�ł��莞�ԂŃt���O���ُ�ȍ��ڂ͏������܂Ȃ����ɂȂ���
//					memcpy(&s_dp[i * pp->item], &d_dp[i * pp->item], sizeof(bf_data_t) * pp->item);
					for (j = 0; j < pp->item; j++) {
						if (d_dp[i * pp->item + j].f[0] < 128) {
							memcpy(&s_dp[i * pp->item + j], &d_dp[i * pp->item + j], sizeof(bf_data_t));
						}
					}
//2003.05.29�i�C���j�����܂�-----------------------------------------
					s_ip[i].yy = tm24.tm_year;
					s_ip[i].mm = tm24.tm_mon;
					s_ip[i].dd = tm24.tm_mday;
					s_ip[i].hh = tm24.tm_hour;
					s_ip[i].nn = tm24.tm_min;
				}
			}
		}
	}
	return 0;
}

/*---------------------------------------------------------
*	�֐��FwriteDataMemory
*	�@�\�F���L�f�[�^��������������(�S�ǑS����)
*	�����Fmp                [I/O] �f�[�^���������
        �Ftyp               [IN]  �f�[�^�������^�C�v
        �Fkno               [IN]  �ǎ�ʔԍ�
        �Ftim               [IN]  �f�[�^����
        �Fdp                [IN]  �������݃f�[�^
*	�ߒl�F
*	�����F
---------------------------------------------------------*/
int writeDataMemory(mem_manager_t *mp, int typ, int kno, time_t tim, char *dp)
{
	mem_datamem_t *pp;
	mem_obstime_t *tp;
	char      *rp;
	bf_data_t *bp;
	tm24_t    tm24;
	time_t    rtim;
	time_t    wtim;
	int       i;

	//�p�����[�^�̊m�F
	if (mp == NULL)
		return ERR_MEM_MP;
	if (mp->kp == NULL)
		return ERR_MEM_MP;
	if (mp->kp->pKind == NULL)
		return ERR_MEM_MP;
	//�f�[�^�������^�C�v
	if (typ < MEM_TYPE_INPUT_1M || typ > MEM_TYPE_DAILY)
		return ERR_MEM_TYPE;
	//�ǎ�ʔԍ�
	if (kno < 1 || kno > mp->kp->max)
		return ERR_MEM_KNO;
	if (mp->kp->pKind[kno-1].kind == NULL)
		return ERR_MEM_KNO;
	//�f�[�^����
	if (tim == 0)
		return ERR_MEM_TIME;
	//�������݃f�[�^
	if (dp == NULL)
		return ERR_MEM_DATA;

	//�e��|�C���^�̐ݒ�
	pp = selectDataMemory(mp, typ, kno, tim);
	tp = (mem_obstime_t *)pp->r.mem;
	rp = pp->r.mem + sizeof(mem_obstime_t);
	bp = (bf_data_t *)pp->d.mem;

	//�������͍쐬����Ă��Ȃ�
	if (pp->r.mem == NULL)
		return ERR_MEM_MEMORY;
	if (pp->d.mem == NULL)
		return ERR_MEM_MEMORY;

	//�b���O�ɂ���
	wtim = 0;
	dll_localtime24_0(tim, &tm24);
	dll_mktime24(&tm24, &wtim);

	//�X�V�������ω������Ȃ烁�����̏�����
	obsToTime(tp, &rtim);
	if (rtim != wtim)
		initDataMemory(mp, pp, wtim);

	//�������ɏ�������
	memcpy(bp, dp, sizeof(bf_data_t) * pp->max * pp->item);
	for (i = 0; i < pp->max; i++) {
		rp[i] = 1;
	}

	//�ŐV�f�[�^�ɏ�������
	for (i = 0; i < pp->max; i++) {
		writeLatestMemory(mp, typ, kno, wtim, i+1, dp + sizeof(bf_data_t) * i);
	}
	//�����f�[�^�ɏ�������
	writeDataMemoryNow(mp, typ, kno, wtim, pp);

	return 0;
}

/*---------------------------------------------------------
*	�֐��FwriteDataMemoryS
*	�@�\�F���L�f�[�^��������������(�P�ǑS����)
*	�����Fmp                [I/O] �f�[�^���������
        �Ftyp               [IN]  �f�[�^�������^�C�v
        �Fkno               [IN]  �ǎ�ʔԍ�
        �Ftim               [IN]  �f�[�^����
        �Fsno               [IN]  �ǔԍ�
        �Fdp                [IN]  �������݃f�[�^
*	�ߒl�F
*	�����F
---------------------------------------------------------*/
int writeDataMemoryS(mem_manager_t *mp, int typ, int kno, time_t tim, int sno, char *dp)
{
	mem_datamem_t *pp;
	mem_obstime_t *tp;
	char      *rp;
	bf_data_t *bp;
	tm24_t    tm24;
	time_t    rtim;
	time_t    wtim;

	//�p�����[�^�̊m�F
	if (mp == NULL)
		return ERR_MEM_MP;
	if (mp->kp == NULL)
		return ERR_MEM_MP;
	if (mp->kp->pKind == NULL)
		return ERR_MEM_MP;
	//�f�[�^�������^�C�v
	if (typ < MEM_TYPE_INPUT_1M || typ > MEM_TYPE_DAILY)
		return ERR_MEM_TYPE;
	//�ǎ�ʔԍ�
	if (kno < 1 || kno > mp->kp->max)
		return ERR_MEM_KNO;
	if (mp->kp->pKind[kno-1].kind == NULL)
		return ERR_MEM_KNO;
	//�f�[�^����
	if (tim == 0)
		return ERR_MEM_TIME;
	//�������݃f�[�^
	if (dp == NULL)
		return ERR_MEM_DATA;

	//�e��|�C���^�̐ݒ�
	pp = selectDataMemory(mp, typ, kno, tim);
	tp = (mem_obstime_t *)pp->r.mem;
	rp = pp->r.mem + sizeof(mem_obstime_t);
	bp = (bf_data_t *)pp->d.mem;

	//�������͍쐬����Ă��Ȃ�
	if (pp->r.mem == NULL)
		return ERR_MEM_MEMORY;
	if (pp->d.mem == NULL)
		return ERR_MEM_MEMORY;
	//�ǔԍ�
	if (sno < 1 || sno > pp->max)
		return ERR_MEM_SNO;

	//�b���O�ɂ���
	wtim = 0;
	dll_localtime24_0(tim, &tm24);
	dll_mktime24(&tm24, &wtim);

	//�X�V�������ω������Ȃ烁�����̏�����
	obsToTime(tp, &rtim);
	if (rtim != wtim)
		initDataMemory(mp, pp, wtim);

	//�������ɏ�������
	memcpy(&bp[(sno-1) * pp->item], dp, sizeof(bf_data_t) * pp->item);
	rp[sno-1] = 1;

	//�ŐV�f�[�^�ɏ�������
	writeLatestMemory(mp, typ, kno, wtim, sno, dp);
	//�����f�[�^�ɏ�������
	writeDataMemoryNow(mp, typ, kno, wtim, pp);

	return 0;
}

/*---------------------------------------------------------
*	�֐��FwriteDataMemoryI
*	�@�\�F���L�f�[�^��������������(���ǂ�����)
*	�����Fmp                [I/O] �f�[�^�������Ǘ����
        �Ftyp               [IN]  �f�[�^�������^�C�v
        �Ftim               [IN]  �f�[�^����
        �Fcnt               [IN]  �������ݍ��ڌ���
        �Fdp                [IN]  �������݃f�[�^
*	�ߒl�F
*	�����F
---------------------------------------------------------*/
int writeDataMemoryI(mem_manager_t *mp, int typ, time_t tim, int cnt, mem_reqdata_t *dp)
{
	mem_datamem_t *pp;
	mem_obstime_t *tp;
	char          *rp;
	bf_data_t     *bp;
	bf_data_t     *wp;
	tm24_t tm24;
	time_t rtim;
	time_t wtim;
	char kidx[99] = {0};
	int i, j, k;
	int ret;
	BOOL flg;


	//�p�����[�^�̊m�F
	if (mp == NULL)
		return ERR_MEM_MP;
	if (mp->kp == NULL)
		return ERR_MEM_MP;
	if (mp->kp->pKind == NULL)
		return ERR_MEM_MP;
	//�f�[�^�������^�C�v
	if (typ < MEM_TYPE_INPUT_1M || typ > MEM_TYPE_DAILY)
		return ERR_MEM_TYPE;
	//�f�[�^����
	if (tim == 0)
		return ERR_MEM_TIME;
	//�������݃f�[�^
	if (dp == NULL)
		return ERR_MEM_DATA;

	//�b���O�ɂ���
	wtim = 0;
	dll_localtime24_0(tim, &tm24);
	dll_mktime24(&tm24, &wtim);

	//�ǎ�ʁE�ǔԍ��E���ڔԍ��E������
	for (k = 0; k < cnt; k++) {
		//�ǎ��
		if (dp[k].kno < 1 || dp[k].kno > mp->kp->max)
			return ERR_MEM_KNO;
		if (mp->kp->pKind[dp[k].kno-1].kind == NULL)
			return ERR_MEM_KNO;
		//�������I��
		pp = selectDataMemory(mp, typ, dp[k].kno, wtim);
		//�������͍쐬����Ă��Ȃ�
		if (pp->r.mem == NULL)
			return ERR_MEM_MEMORY;
		if (pp->d.mem == NULL)
			return ERR_MEM_MEMORY;
		//�ǔԍ�
		if (dp[k].sno < 1 || dp[k].sno > pp->max)
			return ERR_MEM_SNO;
		//���ڔԍ�
		if (dp[k].ino < 1 || dp[k].ino > pp->item)
			return ERR_MEM_ITEM;

		//�ǎ�ʂɏ������ݗv������
		kidx[dp[k].kno-1] = 1;
	}

	//�S�ǎ��
	for (i = 0; i < mp->kp->max; i++) {
		if (kidx[i] == 0) continue;

		//�e��|�C���^�̐ݒ�
		pp = selectDataMemory(mp, typ, i+1, wtim);
		tp = (mem_obstime_t *)pp->r.mem;
		rp = pp->r.mem + sizeof(mem_obstime_t);
		bp = (bf_data_t *)pp->d.mem;

		//�X�V�������ω������Ȃ烁�����̏�����
		obsToTime(tp, &rtim);
		if (rtim != wtim)
			initDataMemory(mp, pp, wtim);

		//�ҏW�̈�̊m��
		wp = new bf_data_t[pp->item];
		if (wp == NULL)
			return ERR_MEM_ALLOC;

		//�ő�ǐ��܂�
		for (j = 0; j < pp->max; j++) {
			flg = FALSE;
			memcpy(wp, &bp[j * pp->item], sizeof(bf_data_t) * pp->item);

			for (k = 0; k < cnt; k++) {
				if (dp[k].kno == i+1 && dp[k].sno == j+1) {
					//�ҏW�̈�ɏ�������
					memcpy(&wp[dp[k].ino-1], &dp[k].dt, sizeof(bf_data_t));
					flg = TRUE;
				}
			}

			//�X�V������΋��L�������ɏ�������
			if ( flg ) {
				ret = writeDataMemoryS(mp, typ, i+1, wtim, j+1, (char *)wp);
				if (ret != 0) {
					if (wp != NULL) free(wp);
					return ret;
				}
			}
		}
		if (wp != NULL) free(wp);
	}
	return 0;
}

/*---------------------------------------------------------
*	�֐��FreadDataMemoryI
*	�@�\�F���L�f�[�^�������ǂݍ���(���ǂ�����)
*	�����Fmp                [I/O] �f�[�^�������Ǘ����
        �Ftyp               [IN]  �f�[�^�������^�C�v
        �Ftim               [IN]  �f�[�^����
        �Fcnt               [IN]  �ǂݍ��ݍ��ڌ���
        �Fdp                [IN]  �ǂݍ��݃f�[�^
*	�ߒl�F
*	�����F
---------------------------------------------------------*/
int readDataMemoryI(mem_manager_t *mp, int typ, time_t tim, int cnt, mem_reqdata_t *dp)
{
	mem_datamem_t *pp;
	mem_obstime_t *tp;
	char          *rp;
	bf_data_t     *bp;
	tm24_t tm24;
	time_t mtim;
	time_t wtim;
	char kidx[99] = {0};
	int i, j, k;


	//�p�����[�^�̊m�F
	if (mp == NULL)
		return ERR_MEM_MP;
	if (mp->kp == NULL)
		return ERR_MEM_MP;
	if (mp->kp->pKind == NULL)
		return ERR_MEM_MP;
	//�f�[�^�������^�C�v
	if (typ < MEM_TYPE_INPUT_1M || typ > MEM_TYPE_DAILY)
		return ERR_MEM_TYPE;
	//�f�[�^����
	if (tim == 0)
		return ERR_MEM_TIME;
	//�������݃f�[�^
	if (dp == NULL)
		return ERR_MEM_DATA;

	//�b���O�ɂ���
	wtim = 0;
	dll_localtime24_0(tim, &tm24);
	dll_mktime24(&tm24, &wtim);

	//�ǎ�ʁE�ǔԍ��E���ڔԍ��E������
	for (k = 0; k < cnt; k++) {
		//�ǎ��
		if (dp[k].kno < 1 || dp[k].kno > mp->kp->max)
			return ERR_MEM_KNO;
		if (mp->kp->pKind[dp[k].kno-1].kind == NULL)
			return ERR_MEM_KNO;
		//�������I��
		pp = selectDataMemory(mp, typ, dp[k].kno, wtim);
		//�������͍쐬����Ă��Ȃ�
		if (pp->r.mem == NULL)
			return ERR_MEM_MEMORY;
		if (pp->d.mem == NULL)
			return ERR_MEM_MEMORY;
		//�ǔԍ�
		if (dp[k].sno < 1 || dp[k].sno > pp->max)
			return ERR_MEM_SNO;
		//���ڔԍ�
		if (dp[k].ino < 1 || dp[k].ino > pp->item)
			return ERR_MEM_ITEM;
		//����
		obsToTime((mem_obstime_t *)pp->r.mem, &mtim);
		if (wtim != mtim)
			return ERR_MEM_TIME;

		//�ǎ�ʂɓǂݍ��ݗv������
		kidx[dp[k].kno-1] = 1;
	}

	//�S�ǎ��
	for (i = 0; i < mp->kp->max; i++) {
		if (kidx[i] == 0) continue;

		//�e��|�C���^�̐ݒ�
		pp = selectDataMemory(mp, typ, i+1, wtim);
		tp = (mem_obstime_t *)pp->r.mem;
		rp = pp->r.mem + sizeof(mem_obstime_t);
		bp = (bf_data_t *)pp->d.mem;

		//�ő�ǐ��܂�
		for (j = 0; j < pp->max; j++) {
			for (k = 0; k < cnt; k++) {
				if (dp[k].kno == i+1 && dp[k].sno == j+1) {
					//�ҏW�̈�ɓǂݍ���
					memcpy(&dp[k].dt, &bp[(dp[k].sno-1)*pp->item+dp[k].ino-1], sizeof(bf_data_t));
				}
			}
		}
	}
	return 0;
}

/*---------------------------------------------------------
*	�֐��FcheckDataMemory
*	�@�\�F���L�f�[�^�������I��
*	�����Fmp                [I/O] �f�[�^�������Ǘ����
        �Ftyp               [IN]  �f�[�^�������^�C�v
        �Fkno               [IN]  �ǎ�ʔԍ�
        �Ftim               [IN]  �f�[�^����
*	�ߒl�F���L�f�[�^���������
*	�����F
---------------------------------------------------------*/
mem_datamem_t *checkDataMemory(mem_manager_t *mp, int typ, int kno, time_t tim)
{
	mem_datamem_t *pp;
	tm24_t tm24;
	time_t mtim;
	div_t  dv;

	//�����擾
	dll_localtime24_0(tim, &tm24);
	dv = div(tm24.tm_min, 10);

	//�^�C�v�ɂ�胁������I��
	switch (typ) {
	case MEM_TYPE_INPUT_1M:
		pp = &mp->tmem[kno-1].min[dv.rem];
		break;
	case MEM_TYPE_INPUT_HM:
		pp = &mp->tmem[kno-1].tim[dv.quot];
		break;
	case MEM_TYPE_DATA_1M:
		pp = &mp->dmem[kno-1].min[dv.rem];
		break;;
	case MEM_TYPE_DATA_HM:
		pp = &mp->dmem[kno-1].tim[dv.quot];
		break;
	case MEM_TYPE_DAILY:
		pp = &mp->dmem[kno-1].day;
		break;
	default:
		pp = NULL;
		break;
	}
	//�����̔�r
	obsToTime((mem_obstime_t *)pp->r.mem, &mtim);
	if (tim != mtim)
		pp = NULL;

	return pp;
}

/*---------------------------------------------------------
*	�֐��FgetUpToDateTime
*	�@�\�F�ŐV�����擾
*	�����Fmp                [I/O] �f�[�^�������Ǘ����
        �Ftyp               [IN]  �f�[�^�������^�C�v
        �Fkno               [IN]  �ǎ�ʔԍ�
*	�ߒl�F�ŐV����
*	�����F
---------------------------------------------------------*/
int getUpToDateTime(mem_manager_t *mp, int typ, int kno, time_t *tt)
{
	mem_obstime_t *tp;
	time_t tim, wtim;
	int i;

	//�p�����[�^�̊m�F
	if (mp == NULL)
		return ERR_MEM_MP;
	if (mp->kp == NULL)
		return ERR_MEM_MP;
	if (mp->kp->pKind == NULL)
		return ERR_MEM_MP;
	//�f�[�^�������^�C�v
	if (typ < MEM_TYPE_INPUT_1M || typ > MEM_TYPE_DAILY)
		return ERR_MEM_TYPE;
	//�ǎ�ʔԍ�
	if (kno < 1 || kno > mp->kp->max)
		return ERR_MEM_KNO;
	if (mp->kp->pKind[kno-1].kind == NULL)
		return ERR_MEM_KNO;

	//�ϐ��̏�����
	tim = 0;

	//���̓�����
	if (typ == MEM_TYPE_INPUT_1M || typ == MEM_TYPE_INPUT_HM) {
		//�P��
		for (i = 0; i < 10; i++) {
			tp = (mem_obstime_t *)mp->dmem[kno-1].min[i].r.mem;
			if (tp == NULL) continue;
			obsToTime(tp, &wtim);
			if (tim < wtim) tim = wtim;
		}
		//���莞
		for (i = 0; i < 6; i++) {
			tp = (mem_obstime_t *)mp->dmem[kno-1].tim[i].r.mem;
			if (tp == NULL) continue;
			obsToTime(tp, &wtim);
			if (tim < wtim) tim = wtim;
		}
	//���Z������
	} else if (typ == MEM_TYPE_DATA_1M || MEM_TYPE_DATA_HM) {
		//�P��
		for (i = 0; i < 10; i++) {
			tp = (mem_obstime_t *)mp->dmem[kno-1].min[i].r.mem;
			if (tp == NULL) continue;
			obsToTime(tp, &wtim);
			if (tim < wtim) tim = wtim;
		}
		//���莞
		for (i = 0; i < 6; i++) {
			tp = (mem_obstime_t *)mp->dmem[kno-1].tim[i].r.mem;
			if (tp == NULL) continue;
			obsToTime(tp, &wtim);
			if (tim < wtim) tim = wtim;
		}
	}

	*tt = tim;

	return ERR_MEM_SUCCESS;
}


//2003.06.23 �ǉ��i��������j----------------------------------------
//���ɍX�V����Ă���ǂ͏㏑�����Ȃ��o�[�W����

/*---------------------------------------------------------
*	�֐��FwriteDataMemoryS_nOW
*	�@�\�F���L�f�[�^��������������(�P�ǑS����)
*	�����Fmp                [I/O] �f�[�^���������
        �Ftyp               [IN]  �f�[�^�������^�C�v
        �Fkno               [IN]  �ǎ�ʔԍ�
        �Ftim               [IN]  �f�[�^����
        �Fsno               [IN]  �ǔԍ�
        �Fdp                [IN]  �������݃f�[�^
*	�ߒl�F
*	�����F���ɍX�V����Ă���ǂ͏㏑�����Ȃ�
---------------------------------------------------------*/
int writeDataMemoryS_nOW(mem_manager_t *mp, int typ, int kno, time_t tim, int sno, char *dp)
{
	mem_datamem_t *pp;
	mem_obstime_t *tp;
	char      *rp;
	bf_data_t *bp;
	tm24_t    tm24;
	time_t    rtim;
	time_t    wtim;

	//�p�����[�^�̊m�F
	if (mp == NULL)
		return ERR_MEM_MP;
	if (mp->kp == NULL)
		return ERR_MEM_MP;
	if (mp->kp->pKind == NULL)
		return ERR_MEM_MP;
	//�f�[�^�������^�C�v
	if (typ < MEM_TYPE_INPUT_1M || typ > MEM_TYPE_DAILY)
		return ERR_MEM_TYPE;
	//�ǎ�ʔԍ�
	if (kno < 1 || kno > mp->kp->max)
		return ERR_MEM_KNO;
	if (mp->kp->pKind[kno-1].kind == NULL)
		return ERR_MEM_KNO;
	//�f�[�^����
	if (tim == 0)
		return ERR_MEM_TIME;
	//�������݃f�[�^
	if (dp == NULL)
		return ERR_MEM_DATA;

	//�e��|�C���^�̐ݒ�
	pp = selectDataMemory(mp, typ, kno, tim);
	tp = (mem_obstime_t *)pp->r.mem;
	rp = pp->r.mem + sizeof(mem_obstime_t);
	bp = (bf_data_t *)pp->d.mem;

	//�������͍쐬����Ă��Ȃ�
	if (pp->r.mem == NULL)
		return ERR_MEM_MEMORY;
	if (pp->d.mem == NULL)
		return ERR_MEM_MEMORY;
	//�ǔԍ�
	if (sno < 1 || sno > pp->max)
		return ERR_MEM_SNO;

	//�b���O�ɂ���
	wtim = 0;
	dll_localtime24_0(tim, &tm24);
	dll_mktime24(&tm24, &wtim);

	//�X�V�������ω������Ȃ烁�����̏�����
	obsToTime(tp, &rtim);
	if (rtim != wtim)
		initDataMemory(mp, pp, wtim);

	if (rp[sno-1] == 0) {
		//�������ɏ�������
		memcpy(&bp[(sno-1) * pp->item], dp, sizeof(bf_data_t) * pp->item);
		rp[sno-1] = 1;

		//�ŐV�f�[�^�ɏ�������
		writeLatestMemory(mp, typ, kno, wtim, sno, dp);
		//�����f�[�^�ɏ�������
		writeDataMemoryNow(mp, typ, kno, wtim, pp);
	}

	return 0;
}

/*---------------------------------------------------------
*	�֐��FwriteDataMemoryI_nOW
*	�@�\�F���L�f�[�^��������������(���ǂ�����)
*	�����Fmp                [I/O] �f�[�^�������Ǘ����
        �Ftyp               [IN]  �f�[�^�������^�C�v
        �Ftim               [IN]  �f�[�^����
        �Fcnt               [IN]  �������ݍ��ڌ���
        �Fdp                [IN]  �������݃f�[�^
*	�ߒl�F
*	�����F���ɍX�V����Ă���ǂ͏㏑�����Ȃ�
---------------------------------------------------------*/
int writeDataMemoryI_nOW(mem_manager_t *mp, int typ, time_t tim, int cnt, mem_reqdata_t *dp)
{
	mem_datamem_t *pp;
	mem_obstime_t *tp;
	char          *rp;
	bf_data_t     *bp;
	bf_data_t     *wp;
	tm24_t tm24;
	time_t rtim;
	time_t wtim;
	char kidx[99] = {0};
	int i, j, k;
	int ret;
	BOOL flg;


	//�p�����[�^�̊m�F
	if (mp == NULL)
		return ERR_MEM_MP;
	if (mp->kp == NULL)
		return ERR_MEM_MP;
	if (mp->kp->pKind == NULL)
		return ERR_MEM_MP;
	//�f�[�^�������^�C�v
	if (typ < MEM_TYPE_INPUT_1M || typ > MEM_TYPE_DAILY)
		return ERR_MEM_TYPE;
	//�f�[�^����
	if (tim == 0)
		return ERR_MEM_TIME;
	//�������݃f�[�^
	if (dp == NULL)
		return ERR_MEM_DATA;

	//�b���O�ɂ���
	wtim = 0;
	dll_localtime24_0(tim, &tm24);
	dll_mktime24(&tm24, &wtim);

	//�ǎ�ʁE�ǔԍ��E���ڔԍ��E������
	for (k = 0; k < cnt; k++) {
		//�ǎ��
		if (dp[k].kno < 1 || dp[k].kno > mp->kp->max)
			return ERR_MEM_KNO;
		if (mp->kp->pKind[dp[k].kno-1].kind == NULL)
			return ERR_MEM_KNO;
		//�������I��
		pp = selectDataMemory(mp, typ, dp[k].kno, wtim);
		//�������͍쐬����Ă��Ȃ�
		if (pp->r.mem == NULL)
			return ERR_MEM_MEMORY;
		if (pp->d.mem == NULL)
			return ERR_MEM_MEMORY;
		//�ǔԍ�
		if (dp[k].sno < 1 || dp[k].sno > pp->max)
			return ERR_MEM_SNO;
		//���ڔԍ�
		if (dp[k].ino < 1 || dp[k].ino > pp->item)
			return ERR_MEM_ITEM;

		//�ǎ�ʂɏ������ݗv������
		kidx[dp[k].kno-1] = 1;
	}

	//�S�ǎ��
	for (i = 0; i < mp->kp->max; i++) {
		if (kidx[i] == 0) continue;

		//�e��|�C���^�̐ݒ�
		pp = selectDataMemory(mp, typ, i+1, wtim);
		tp = (mem_obstime_t *)pp->r.mem;
		rp = pp->r.mem + sizeof(mem_obstime_t);
		bp = (bf_data_t *)pp->d.mem;

		//�X�V�������ω������Ȃ烁�����̏�����
		obsToTime(tp, &rtim);
		if (rtim != wtim)
			initDataMemory(mp, pp, wtim);

		//�ҏW�̈�̊m��
		wp = new bf_data_t[pp->item];
		if (wp == NULL)
			return ERR_MEM_ALLOC;

		//�ő�ǐ��܂�
		for (j = 0; j < pp->max; j++) {
			flg = FALSE;
			memcpy(wp, &bp[j * pp->item], sizeof(bf_data_t) * pp->item);

			for (k = 0; k < cnt; k++) {
				if (dp[k].kno == i+1 && dp[k].sno == j+1) {
					//�ҏW�̈�ɏ�������
					memcpy(&wp[dp[k].ino-1], &dp[k].dt, sizeof(bf_data_t));
					flg = TRUE;
				}
			}

			//�X�V������΋��L�������ɏ�������
			if ( flg ) {
				ret = writeDataMemoryS_nOW(mp, typ, i+1, wtim, j+1, (char *)wp);
				if (ret != 0) {
					if (wp != NULL) free(wp);
					return ret;
				}
			}
		}
		if (wp != NULL) free(wp);
	}
	return 0;
}

//2003.06.23 �ǉ��i�����܂Łj----------------------------------------
