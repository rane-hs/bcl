#include "stdafx.h"
#include "systemConst.h"

/*-------------------------------------------------------------------
  �\���́F�ǎ�ʋ��L�������Ǘ����
  ����  �Fapp           �f�[�^�p�X
        �FofficeID      �������ԍ�
        �FunitID        ���u�ԍ�
        �Ftim           �ǂݍ��ݓ���
        �Fmax           �ő�ǎ�ʔԍ�
        �Fmsize         �Ǐ�񃁃����T�C�Y
        �Fpt            �Ǘ��p�|�C���^
-------------------------------------------------------------------*/
typedef struct {
	char        app[_MAX_PATH];     //�f�[�^�p�X
	int         officeID;           //�������ԍ�
	int         unitID;             //���u�ԍ�
	int         max;                //�ő�ǎ�ʔԍ�
	long        msize;              //�Ǐ�񃁃����T�C�Y
	time_t      tim;                //�ǂݍ��ݓ���
	time_t      tim_local[99];      //�ǎ������X�V����
	time_t      tim_action[99];     //�ǉғ����X�V����
	time_t      tim_name[99];       //�ǖ��̏��X�V����
	time_t      tim_optname[99];    //�Ǖ⑫���̏��X�V����
	time_t      tim_code[99];       //�ǃR�[�h���X�V����
//	time_t      tim_comp1[99];      //�Ǎ\�����X�V����
//	time_t      tim_comp2[99];      //�Ǎ\�����X�V����
	//�擪����̑��Έʒu
	long        idx_kind;           //�ǎ�ʏ��ʒu
	long        idx_local;          //�ǎ������ʒu
	long        idx_action;         //�ǉғ����ʒu
	long        idx_name;           //�ǖ��̏��ʒu
	long        idx_optname;        //�Ǖ⑫���̏��ʒu
	long        idx_code;           //�ǃR�[�h���ʒu
//	long        idx_comp1;          //�Ǎ\�����ʒu
//	long        idx_comp2;          //�Ǎ\�����ʒu
} kind_sharedmem_t;


/*-------------------------------------------------------------------
  ���[�J���֐��錾
-------------------------------------------------------------------*/
void inisplit  (char *sp, char *p_name, char *p_value);
void csvtrim   (char *sp);
void csvsplit  (char *sp, int n, char *p_value);
int  csvsplit_n(char *sp, int n, char *p_value, int v_len);
void csvread   (FILE *fp, char *dp);
void csvread_n (FILE *fp, char *dp, int d_len);

int  getMyNumber(sys_mynumber_t *pt);
int  getServerIniFile(int officeID, int unitID, ini_server_t *pt);

int  getSysConstKind   (char *app, sys_kind_t *pt);
int  getSysConstLocal  (kind_sharedmem_t *mp, char *app, int officeID, int unitID, sys_kind_t *kp, sys_local_t *pt);
int  getSysConstAction (kind_sharedmem_t *mp, char *app, sys_kind_t *kp, sys_action_t  *pt);
int  getSysConstName   (kind_sharedmem_t *mp, char *app, sys_kind_t *kp, sys_name_t    *pt);
int  getSysConstOptName(kind_sharedmem_t *mp, char *app, sys_kind_t *kp, sys_optname_t *pt);
int  getSysConstCode   (kind_sharedmem_t *mp, char *app, sys_kind_t *kp, sys_code_t    *pt);
//int  getSysConstComp1  (kind_sharedmem_t *mp, char *app, sys_kind_t *kp, sys_comp1_t   *pt);
//int  getSysConstComp2  (kind_sharedmem_t *mp, char *app, sys_kind_t *kp, sys_comp2_t   *pt);

int  createSysConstKindInfo(sharedMem_t *pt);
int  readSysConstKindInfo  (sharedMem_t *pt);

int  getSysConstKindInfo  (kind_manager_t *pt);
int  setSysConstKindInfo  (kind_manager_t *pt);
void freeSysConstKindInfo (kind_manager_t *pt);
BOOL checkSysConstKindInfo(kind_manager_t *pt, char *dpath, int pno);

int  getSysConstExio(char *app, char *syskind, int officeNo, int unitNo, int houroNo, sys_exioinfo_t *p);


/*-------------------------------------------------------------------
  ���W���[�����ϐ�
-------------------------------------------------------------------*/
char mCsvLine[SYS_MAX_LINE];        //CSV�t�@�C���P�s�ǂݍ��ݗp



/*---------------------------------------------------------
*	�֐��Finisplit
*	�@�\�F�����񕪊�
*	�����Fsp                [IN]  ������
        �Fp_name            [OUT] ����
        �Fp_value           [OUT] �l
*	�ߒl�F
*	�����F�h���� = �l�h�̕�����𖼏̂ƒl�ɕ������܂��B
---------------------------------------------------------*/
void inisplit(char *sp, char *p_name, char *p_value)
{
	char spt[] = " =";
	char *wk;
	char *dp;

	if (p_name  == NULL) return;
	if (p_value == NULL) return;
	p_name[0]  = '\0';
	p_value[0] = '\0';
	if (sp == NULL) return;

	wk = new char[strlen(sp) + 1];
	if (wk == NULL) return;

	strcpy(wk, sp);
	dp = strtok(wk, spt);
	strcpy(p_name, dp);
	dp = strtok(NULL, spt);
	strcpy(p_value, dp);

	if (wk != NULL) free(wk);
}

/*---------------------------------------------------------
*	�֐��Fcsvtrim
*	�@�\�F�O��X�y�[�X�폜
*	�����Fsp                [I/O] ������
*	�ߒl�F
*	�����F�����񂩂�O�X�y�[�X�A��X�y�[�X���폜���܂��B
---------------------------------------------------------*/
void csvtrim(char *sp)
{
	int i,j,l;

	if (sp[0] == '\0') return;
	i = 0;
	j = strlen(sp);
	l = strlen(sp);
	while (sp[i]   == ' ' && i < l) i++;
	while (sp[j-1] == ' ' && 0 < j) j--;
	if (i < j && (0 < i || j < l)) memmove(sp, sp+i, j-i);
	sp[j-i] = '\0';
}

/*---------------------------------------------------------
*	�֐��Fcsvsplit
*	�@�\�FCSV�����񕪊�
*	�����Fsp                [IN]  CSV������(�J���}��؂�)
        �Fn                 [IN]  �擾����ʒu
        �Fp_value           [OUT] �擾����������
*	�ߒl�F
*	�����FCSV�����񂩂�w�肳�ꂽ�ʒu�̕�������擾���܂��B
---------------------------------------------------------*/
void csvsplit(char *sp, int n, char *p_value)
{
	char *dp = sp;
	int i = 0;
	int j = 0;

	if (p_value == NULL) return;
	p_value[0] = '\0';
	if (sp == NULL) return;

	while (*dp != '\0' && i <= n) {
		if (*dp == ',') i++;
		else if (i == n) p_value[j++] = *dp;
		dp++;
	}
	p_value[j] = '\0';
	csvtrim(p_value);
}

/*-------------------------------------------------------------------
*	�֐��Fcsvsplit_n
*	�@�\�F�����񕪊�(��������������)
*	�����Fsp                [IN]  ������
        �Fn                 [IN]  �擾����ʒu
        �Fp_value           [OUT] �擾����������
        �Fv_len             [IN]  �擾�̈�̃T�C�Y
*	�ߒl�F�擾������
*	�����F��؂蕶���ŕ�������Ăł����������
        �F�w�肳�ꂽ�ʒu�̕�������擾���܂��B
-------------------------------------------------------------------*/
int csvsplit_n(char *sp, int n, char *p_value, int v_len)
{
	char *dp = sp;
	int i = 0;
	int j = 0;

	p_value[0] = '\0';
	while (*dp != '\0' && i <= n) {
		if (j >= (v_len-1)) break;	//NULL�����邽��(v_len-1)
		if (*dp == ',') i++;
		else if (i == n) p_value[j++] = *dp;
		dp++;
	}
	p_value[j] = '\0';
	csvtrim(p_value);

	return j;
}

/*---------------------------------------------------------
*	�֐��Fcsvread
*	�@�\�F�P�s�ǂݍ���
*	�����Ffp                [IN]  �t�@�C���\����
        �Fdp                [OUT] �ǂݍ��񂾕�����
*	�ߒl�F
*	�����FCSV�t�@�C������P�s�ǂݍ��݂܂��B
---------------------------------------------------------*/
void csvread(FILE *fp, char *dp)
{
	int  i = 0;
	char c = 0;

	if (fp == NULL) return;
	if (dp == NULL) return;

	c = fgetc(fp);
	while ( !feof(fp) && c != '\n' && c != '\0') {
		if (c != '\n') dp[i++] = c;
		c = fgetc(fp);
	}
	dp[i] = '\0';
}

/*---------------------------------------------------------
*	�֐��Fcsvread_n
*	�@�\�F�P�s�ǂݍ���(��������������)
*	�����Ffp                [IN]  �t�@�C���\����
        �Fdp                [OUT] �ǂݍ��񂾕�����
        �Fd_len             [IN]  �擾�̈�T�C�Y
*	�ߒl�F
*	�����FCSV�t�@�C������P�s�ǂݍ��݂܂��B
---------------------------------------------------------*/
void csvread_n(FILE *fp, char *dp, int d_len)
{
	int  i = 0;
	char c = 0;

	if (fp == NULL) return;
	if (dp == NULL) return;

	c = fgetc(fp);
	while ( !feof(fp) && c != '\n' && c != '\0') {
		if (i < (d_len-1))	//NULL�������(d_len-1)
			if (c != '\n') dp[i++] = c;
		c = fgetc(fp);
	}
	dp[i] = '\0';
}

/*---------------------------------------------------------
*	�֐��FgetMyNumber
*	�@�\�F�����u�ԍ��̎擾
*	�����Fpt                [OUT] �����u�ԍ�
*	�ߒl�F����
        �F     0 = ����
        �F  �ȊO = ���s
*	�����F
---------------------------------------------------------*/
int getMyNumber(sys_mynumber_t *pt)
{
	FILE  *fp;
	char  wk[8];
	int   v;
	div_t vi;

	if (pt == NULL)
		return SYS_ERR_POINTER;

	pt->intOfficeID = 0;
	pt->intUnitID   = 0;

	fp = fopen("..\\common_ini\\my_number.txt", "r");
	if (fp == NULL)
		return SYS_ERR_FILE_OPEN;

	fscanf(fp, "%s", wk);
	fclose(fp);

	v = atoi(wk);
	vi = div(v, 100);
	pt->intOfficeID = vi.quot;
	pt->intUnitID   = vi.rem;

	return SYS_ERR_SUCCESS;
}

/*---------------------------------------------------------
*	�֐��FgetServerIniFile
*	�@�\�F�T�[�o�������t�@�C���ǂݍ���
*	�����FofficeID          [IN]  ���������ԍ�
        �FunitID            [IN]  �����u�ԍ�
		�Fpt                [OUT] �T�[�o���
*	�ߒl�F����
        �F     0 = ����
        �F  �ȊO = ���s
*	�����F
---------------------------------------------------------*/
int getServerIniFile(int officeID, int unitID, ini_server_t *pt)
{
	char iniPath[_MAX_PATH];

	if (pt == NULL)
		return SYS_ERR_POINTER;

	sprintf(iniPath, "../common_ini/server%02d%02d.ini", officeID, unitID);
	//�A�v���P�[�V�����p�X�̓ǂݍ���
	GetPrivateProfileString("PATH", "JRC", "", pt->strAppPath, sizeof(pt->strAppPath), iniPath);
	//�f�[�^�p�X�̓ǂݍ���
	GetPrivateProfileString("PATH", "DATA", "", pt->strDataPath, sizeof(pt->strDataPath), iniPath);
	//�l�n�h���C�u�̓ǂݍ���
	GetPrivateProfileString("PATH", "MO",  "", pt->strMO, sizeof(pt->strMO), iniPath);
	//�t�@�C���ۑ�����
	pt->intDays = GetPrivateProfileInt("FILE", "DAYS", 400, iniPath);
	//�t�@�C���ۑ����ԁi�P���j
	pt->intDays1M = GetPrivateProfileInt("FILE", "DAYS1M", 45, iniPath);
	//�����t�@�C���쐬�L��
	pt->intNowFile = GetPrivateProfileInt("FILE", "NOWFILE", 0, iniPath);

//2003.02.27 �ǉ��i��������j----------------------------------------
	//�f�[�^����M������ԁi���j
	pt->intBeFore = GetPrivateProfileInt("FILE", "BEFORE", 60, iniPath);
	//�ߋ��f�[�^������ԁi���j
	pt->intBeHind = GetPrivateProfileInt("FILE", "BEHIND", 60, iniPath);
//2003.02.27 �ǉ��i�����܂Łj----------------------------------------

	if (pt->strAppPath[0] == '\0' || pt->strDataPath[0] == '\0')
		return SYS_ERR_SERVER_INFO;

	return SYS_ERR_SUCCESS;
}

/*---------------------------------------------------------
*	�֐��FgetSysConstKind
*	�@�\�F�ǎ�ʏ��ǂݍ���
*	�����Fapp               [IN]  �A�v���P�[�V�����p�X
        �Fpt                [OUT] �ǎ�ʏ��
*	�ߒl�F
*	�����F�ǎ�ʏ���ǂݍ��݂܂��B
---------------------------------------------------------*/
int getSysConstKind(char *app, sys_kind_t *pt)
{
	FILE *fp;
	char fpath[_MAX_PATH];
	char dt[32];
	int  no;

	if (app == NULL)
		return SYS_ERR_POINTER;
	if (pt == NULL)
		return SYS_ERR_POINTER;

	//�ǎ�ʏ��̓ǂݍ���
	sprintf(fpath, "%sprogram\\common_ini\\kind_info.csv", app);
	fp = fopen(fpath, "r");
	if (fp == NULL)
		return SYS_ERR_FILE_OPEN;

	//�K�v�Ȃ��s��ǂݔ�΂�
	csvread_n(fp, mCsvLine, sizeof(mCsvLine));
	csvread_n(fp, mCsvLine, sizeof(mCsvLine));
	csvread_n(fp, mCsvLine, sizeof(mCsvLine));
	csvread_n(fp, mCsvLine, sizeof(mCsvLine));
	csvread_n(fp, mCsvLine, sizeof(mCsvLine));
	csvread_n(fp, mCsvLine, sizeof(mCsvLine));
	csvread_n(fp, mCsvLine, sizeof(mCsvLine));

	while ( !feof(fp) ) {

		//�P�s�ǂݍ���
		csvread_n(fp, mCsvLine, sizeof(mCsvLine));

		//�ǎ�ʔԍ��̔���
		csvsplit_n(mCsvLine, 0, dt, sizeof(dt));
		no = atoi(dt);
		if (no >= 1 && no <= SYS_MAX_KIND) {
			//�ǎ�ʔԍ�
			pt[no-1].kno = no;

			//�ǎ�ʖ�
			//csvsplit(mCsvLine, 1, dt);
			//strcpy(pt[no-1].name, dt);
			csvsplit_n(mCsvLine, 1, pt[no-1].name, sizeof(pt[no-1].name));

			//�ǎ�ʖ��i�a���j
			//csvsplit(mCsvLine, 2, dt);
			//strcpy(pt[no-1].jname, dt);
			csvsplit_n(mCsvLine, 2, pt[no-1].jname, sizeof(pt[no-1].jname));

			//�ő�ǐ�
			csvsplit_n(mCsvLine, 3, dt, sizeof(dt));
			pt[no-1].max = atoi(dt);

			//�ϑ����ڐ�
			csvsplit_n(mCsvLine, 4, dt, sizeof(dt));
			pt[no-1].titem = atoi(dt);

			//���W�v���ڐ�
			csvsplit_n(mCsvLine, 5, dt, sizeof(dt));
			pt[no-1].ditem = atoi(dt);

			//���Z��b���ڐ�
			csvsplit_n(mCsvLine, 6, dt, sizeof(dt));
			pt[no-1].bitem = atoi(dt);
		}
	}
	fclose(fp);

	return SYS_ERR_SUCCESS;
}

/*---------------------------------------------------------
*	�֐��FgetSysConstLocal
*	�@�\�F�ǎ������ǂݍ���
*	�����Fapp               [IN]  �f�[�^�p�X
        �FofficeID          [IN]  �������ԍ�
        �FunitID            [IN]  ���u�ԍ�
        �Fkp                [IN]  �ǎ�ʏ��
        �Fpt                [OUT] �ǎ������
*	�ߒl�F
*	�����F�ǎ������t�@�C����ǂݍ��݂܂��B
        �F�O������Ƃ��āA
        �F  �ǎ�ʏ�񂪐ݒ肳��Ă��邱��
        �F  �ǎ������̗̈�͊m�ۂ���Ă��邱��
        �F�t�@�C�������݂��Ȃ��ꍇ�A�����l���ݒ肳��܂��B
---------------------------------------------------------*/
int getSysConstLocal(kind_sharedmem_t *mp, char *app, int officeID, int unitID, sys_kind_t *kp, sys_local_t *pt)
{
	sys_local_t *p = pt;
	FILE *fp;
	char fpath[_MAX_PATH];
	char dt[32];
	int  no;
	int  v;
	int  i;
//2002.11.19 �ǉ��i��������j----------------------------------------
//�t�@�C���ǂݍ��ݎ��ɍX�V�������r����悤�ɕύX
	struct _stat st;
	int  ret;
//2002.11.19 �ǉ��i�����܂Łj----------------------------------------

	if (app == NULL)
		return SYS_ERR_POINTER;
	if (kp == NULL)
		return SYS_ERR_POINTER;
	if (pt == NULL)
		return SYS_ERR_POINTER;


	//�ǎ������̓ǂݍ���
	for (i = 0; i < SYS_MAX_KIND; i++) {
		if (kp[i].max <= 0) continue;

		//�t�@�C���p�X�̐ݒ�
		sprintf(fpath, "%ssys_table\\csv\\%s_local_%02d%02d00.csv", app, kp[i].name, officeID, unitID);

//2002.11.19 �C���i��������j----------------------------------------
//�t�@�C���ǂݍ��ݎ��ɍX�V�������r����悤�ɕύX
//		//�t�@�C���I�[�v��
//		fp = fopen(fpath, "r");

		//�t�@�C���|�C���^�̏�����
		fp = NULL;
		//�t�@�C�����̎擾
		ret = _stat(fpath, &st);
		if (ret == 0) {
			//�X�V�����̔�r
			if (mp->tim_local[i] != st.st_mtime) {
				mp->tim_local[i] = st.st_mtime;
				//�̈�̏�����
				memset(p, 0, kp[i].max * sizeof(sys_local_t));
				//�t�@�C���I�[�v��
				fp = fopen(fpath, "r");
			}
		}
//2002.11.19 �C���i�����܂Łj----------------------------------------

		if (fp != NULL) {
			//�K�v�Ȃ��s��ǂݔ�΂�
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));

			while ( !feof(fp) ) {

				//�P�s�ǂݍ���
				csvread_n(fp, mCsvLine, sizeof(mCsvLine));

				//�����ǔԂ̔���
				csvsplit_n(mCsvLine, 0, dt, sizeof(dt));
				no = atoi(dt);
				if (1 <= no && no <= kp[i].max) {
					//�����ǔ�
					p[no-1].sno = no;

					//�V�X�e�����ʎ������
					csvsplit_n(mCsvLine, 1, dt, sizeof(dt));
					v = atoi(dt);
					if (v == SYS_LOCAL_ENTRY_ON)
						p[no-1].entry = SYS_LOCAL_ENTRY_ON;
					else
						p[no-1].entry = 0;

					//���[�J���n���
					csvsplit_n(mCsvLine, 2, dt, sizeof(dt));
					v = atoi(dt);
					if (1 <= v && v <= 99)
						p[no-1].kei = v;
					else
						p[no-1].kei = 0;

					//���[�J���f�[�^�ۑ�����
					csvsplit_n(mCsvLine, 3, dt, sizeof(dt));
					v = atoi(dt);
					if (v == SYS_LOCAL_CYCLE_1M)
						p[no-1].cycle = SYS_LOCAL_CYCLE_1M;
					else
						p[no-1].cycle = 0;

					//���[�J�����Z���{���
					csvsplit_n(mCsvLine, 4, dt, sizeof(dt));
					v = atoi(dt);
					if ((v == SYS_LOCAL_TCALC_IP) ||
                        (v == SYS_LOCAL_TCALC_10) ||
                        (v == SYS_LOCAL_TCALC_01))
						p[no-1].tcalc = v;
					else
						p[no-1].tcalc = 0;

					//���[�J�����W�v���Z���{���
					csvsplit_n(mCsvLine, 5, dt, sizeof(dt));
					v = atoi(dt);
					if (v == SYS_LOCAL_DCALC_DO)
						p[no-1].dcalc = SYS_LOCAL_DCALC_DO;
					else
						p[no-1].dcalc = 0;

					//�\������
					csvsplit_n(mCsvLine, 6, dt, sizeof(dt));
					//�\������
					csvsplit_n(mCsvLine, 7, dt, sizeof(dt));
				}
			}
			fclose(fp);
		}
		//���̃|�C���^�ֈړ�
		p = p + kp[i].max;
	}
	return SYS_ERR_SUCCESS;
}

/*---------------------------------------------------------
*	�֐��FgetSysConstAction
*	�@�\�F�ǉғ����ǂݍ���
*	�����Fapp               [IN]  �f�[�^�p�X
        �Fkp                [IN]  �ǎ�ʏ��
        �Fpt                [OUT] �ǉғ����
*	�ߒl�F
*	�����F�ǉғ����t�@�C����ǂݍ��݂܂��B
        �F�O������Ƃ��āA
        �F  �ǎ�ʏ�񂪐ݒ肳��Ă��邱��
        �F  �ǉғ����̗̈�͊m�ۂ���Ă��邱��
        �F�t�@�C�������݂��Ȃ��ꍇ�A�����l���ݒ肳��܂��B
---------------------------------------------------------*/
int getSysConstAction(kind_sharedmem_t *mp, char *app, sys_kind_t *kp, sys_action_t *pt)
{
	sys_action_t *p = pt;
	FILE *fp;
	char fpath[_MAX_PATH];
	char dt[32];
	int  no;
	int  v;
	int  i;
//2002.11.19 �ǉ��i��������j----------------------------------------
//�t�@�C���ǂݍ��ݎ��ɍX�V�������r����悤�ɕύX
	struct _stat st;
	int  ret;
//2002.11.19 �ǉ��i�����܂Łj----------------------------------------

	if (app == NULL)
		return SYS_ERR_POINTER;
	if (kp == NULL)
		return SYS_ERR_POINTER;
	if (pt == NULL)
		return SYS_ERR_POINTER;

	//�ǉғ����̓ǂݍ���
	for (i = 0; i < SYS_MAX_KIND; i++) {
		if (kp[i].max <= 0) continue;

		//�t�@�C���p�X�̐ݒ�
		sprintf(fpath, "%ssys_table\\csv\\%s_action.csv", app, kp[i].name);

//2002.11.19 �C���i��������j----------------------------------------
//�t�@�C���ǂݍ��ݎ��ɍX�V�������r����悤�ɕύX
//		//�t�@�C���I�[�v��
//		fp = fopen(fpath, "r");

		//�t�@�C���|�C���^�̏�����
		fp = NULL;
		//�t�@�C�����̎擾
		ret = _stat(fpath, &st);
		if (ret == 0) {
			//�X�V�����̔�r
			if (mp->tim_action[i] != st.st_mtime) {
				mp->tim_action[i] = st.st_mtime;
				//�̈�̏�����
				memset(p, 0, kp[i].max * sizeof(sys_action_t));
				//�t�@�C���I�[�v��
				fp = fopen(fpath, "r");
			}
		}
//2002.11.19 �C���i�����܂Łj----------------------------------------

		if (fp != NULL) {
			//�K�v�Ȃ��s��ǂݔ�΂�
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));

			while ( !feof(fp) ) {

				//�P�s�ǂݍ���
				csvread_n(fp, mCsvLine, sizeof(mCsvLine));

				//�����ǔԂ̔���
				csvsplit_n(mCsvLine, 0, dt, sizeof(dt));
				no = atoi(dt);
				if (1 <= no && no <= kp[i].max) {
					//�����ǔ�
					p[no-1].sno = no;

					//�ǉғ���
					csvsplit_n(mCsvLine, 1, dt, sizeof(dt));
					v = atoi(dt);
					if (v == SYS_ACTION_ACTION_MAINTE || v == SYS_ACTION_ACTION_CLOSE)
						p[no-1].action = v;
					else
						p[no-1].action = 0;

				}
			}
			fclose(fp);
		}
		//���̃|�C���^�ֈړ�
		p = p + kp[i].max;
	}
	return SYS_ERR_SUCCESS;
}

/*---------------------------------------------------------
*	�֐��FgetSysConstName
*	�@�\�F�ǖ��̏��ǂݍ���
*	�����Fapp               [IN]  �f�[�^�p�X
        �Fkp                [IN]  �ǎ�ʏ��
        �Fpt                [OUT] �ǖ��̏��
*	�ߒl�F
*	�����F�ǖ��̏��t�@�C����ǂݍ��݂܂��B
        �F�O������Ƃ��āA
        �F  �ǎ�ʏ�񂪐ݒ肳��Ă��邱��
        �F  �ǖ��̏��̗̈�͊m�ۂ���Ă��邱��
        �F�t�@�C�������݂��Ȃ��ꍇ�A�����l���ݒ肳��܂��B
---------------------------------------------------------*/
int getSysConstName(kind_sharedmem_t *mp, char *app, sys_kind_t *kp, sys_name_t *pt)
{
	sys_name_t *p = pt;
	FILE  *fp;
	char   fpath[_MAX_PATH];
	char   dt[32];
	int    no;
	int    v;
	double f;
	int    i;
//2002.11.19 �ǉ��i��������j----------------------------------------
//�t�@�C���ǂݍ��ݎ��ɍX�V�������r����悤�ɕύX
	struct _stat st;
	int ret;
//2002.11.19 �ǉ��i�����܂Łj----------------------------------------


	if (app == NULL)
		return SYS_ERR_POINTER;
	if (kp == NULL)
		return SYS_ERR_POINTER;
	if (pt == NULL)
		return SYS_ERR_POINTER;

	//�ǖ��̏��̓ǂݍ���
	for (i = 0; i < SYS_MAX_KIND; i++) {
		if (kp[i].max <= 0) continue;

		//�t�@�C���p�X�̐ݒ�
		sprintf(fpath, "%ssys_table\\csv\\%s_name.csv", app, kp[i].name);

//2002.11.19 �C���i��������j----------------------------------------
//�t�@�C���ǂݍ��ݎ��ɍX�V�������r����悤�ɕύX
//		//�t�@�C���I�[�v��
//		fp = fopen(fpath, "r");

		//�t�@�C���|�C���^�̏�����
		fp = NULL;
		//�t�@�C�����̎擾
		ret = _stat(fpath, &st);
		if (ret == 0) {
			//�X�V�����̔�r
			if (mp->tim_name[i] != st.st_mtime) {
				mp->tim_name[i] = st.st_mtime;
				//�̈�̏�����
				memset(p, 0, kp[i].max * sizeof(sys_name_t));
				//�t�@�C���I�[�v��
				fp = fopen(fpath, "r");
			}
		}
//2002.11.19 �C���i�����܂Łj----------------------------------------

		if (fp != NULL) {
			//�K�v�Ȃ��s��ǂݔ�΂�
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));

			while ( !feof(fp) ) {

				//�P�s�ǂݍ���
				csvread_n(fp, mCsvLine, sizeof(mCsvLine));

				//�����ǔԂ̔���
				csvsplit_n(mCsvLine, 0, dt, sizeof(dt));
				no = atoi(dt);
				if (1 <= no && no <= kp[i].max) {
					//�����ǔ�
					p[no-1].sno = no;

					//�Ǘ��ԍ��P
					csvsplit_n(mCsvLine, 1, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= SYS_MAX_MNO)
							p[no-1].mno1 = v;
					}

					//�Ǘ��ԍ��Q
					csvsplit_n(mCsvLine, 2, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= SYS_MAX_MNO)
							p[no-1].mno2 = v;
					}

					//�Ǘ��ԍ��R
					csvsplit_n(mCsvLine, 3, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= SYS_MAX_MNO)
							p[no-1].mno3 = v;
					}

					//�Ǘ��ԍ��S
					csvsplit_n(mCsvLine, 4, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= SYS_MAX_MNO)
							p[no-1].mno4 = v;
					}

					//�w��ϑ����R�[�h
					csvsplit_n(mCsvLine, 5, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (v == SYS_NAME_ACODE_APPOINT)
							p[no-1].acode = v;
					}

					//��_�P�R�[�h
					csvsplit_n(mCsvLine, 6, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (v == SYS_NAME_PCODE_STANDARD)
							p[no-1].pcode1 = v;
					}

					//��_�Q�R�[�h
					csvsplit_n(mCsvLine, 7, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (v == SYS_NAME_PCODE_STANDARD)
							p[no-1].pcode2 = v;
					}

					//�Ǘ��Ҏ�ʃR�[�h
					csvsplit_n(mCsvLine, 8, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						switch (v) {
						case SYS_NAME_MCODE_KASEN:
						case SYS_NAME_MCODE_SABOU:
						case SYS_NAME_MCODE_ROAD:
						case SYS_NAME_MCODE_DAM:
						case SYS_NAME_MCODE_POST:
						case SYS_NAME_MCODE_MLIT:
						case SYS_NAME_MCODE_JMA:
						case SYS_NAME_MCODE_CITY:
						case SYS_NAME_MCODE_PREF:
						case SYS_NAME_MCODE_OTHER:
							p[no-1].mcode = v;
						}
					}

					//�������������@jurisdiction office code
					csvsplit_n(mCsvLine, 9, p[no-1].office_name, sizeof(p[no-1].office_name));

					//�ϑ�����
					csvsplit_n(mCsvLine, 10, p[no-1].station_name, sizeof(p[no-1].station_name));

					//���於
					csvsplit_n(mCsvLine, 11, p[no-1].valley_name, sizeof(p[no-1].valley_name));


					//���n�� river system
					csvsplit_n(mCsvLine, 12, p[no-1].rsys_name, sizeof(p[no-1].rsys_name));

					//�͐얼
					csvsplit_n(mCsvLine, 13, p[no-1].river_name, sizeof(p[no-1].river_name));

					//��\���ݖ� represe seat
					csvsplit_n(mCsvLine, 14, p[no-1].represe_name, sizeof(p[no-1].represe_name));

					//�H�����
					csvsplit_n(mCsvLine, 15, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (v == SYS_NAME_ROUTE_NATIONAL ||
							v == SYS_NAME_ROUTE_PREF ||
							v == SYS_NAME_ROUTE_LOCAL)
							p[no-1].route = v;
					}

					//�H����
					csvsplit_n(mCsvLine, 16, p[no-1].route_name, sizeof(p[no-1].route_name));

					//�ϑ������i�T�u�P�j
					csvsplit_n(mCsvLine, 17, p[no-1].sub_name1, sizeof(p[no-1].sub_name1));

					//�ϑ������i�T�u�Q�j
					csvsplit_n(mCsvLine, 18, p[no-1].sub_name2, sizeof(p[no-1].sub_name2));

					//���ǖ�
					csvsplit_n(mCsvLine, 19, p[no-1].shorts_name, sizeof(p[no-1].shorts_name));

					//���h�u���b�N��
					csvsplit_n(mCsvLine, 20, p[no-1].sabou_block, sizeof(p[no-1].sabou_block));

					//�s���{���� prefecture
					csvsplit_n(mCsvLine, 21, p[no-1].pref_name, sizeof(p[no-1].pref_name));

					//�Q�E�s��
					csvsplit_n(mCsvLine, 22, p[no-1].city_name, sizeof(p[no-1].city_name));

					//��E���E����
					csvsplit_n(mCsvLine, 23, p[no-1].area_name, sizeof(p[no-1].area_name));

					//�厚�E�n�ԓ���,
					csvsplit_n(mCsvLine, 24, p[no-1].addr_name, sizeof(p[no-1].addr_name));

					//�ϑ����ܓx�i�x�j
					csvsplit_n(mCsvLine, 25, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (24 <= v && v <= 46)
							p[no-1].lat = v;
					}

					//�ϑ����ܓx�i���j
					csvsplit_n(mCsvLine, 26, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (0 <= v && v <= 59)
							p[no-1].lat_min = v;
					}

					//�ϑ����ܓx�i�b�j
					csvsplit_n(mCsvLine, 27, dt, sizeof(dt));
					if (dt[0] != '\0') {
						f = atof(dt);
						if (0.0 <= f && f <= 59.0)
							p[no-1].lat_sec = f;
					}

					//�ϑ����o�x�i�x�j
					csvsplit_n(mCsvLine, 28, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (123 <= v && v <= 146)
							p[no-1].lon = v;
					}

					//�ϑ����o�x�i���j
					csvsplit_n(mCsvLine, 29, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (0 <= v && v <= 59)
							p[no-1].lon_min = v;
					}

					//�ϑ����o�x�i�b�j
					csvsplit_n(mCsvLine, 30, dt, sizeof(dt));
					if (dt[0] != '\0') {
						f = atof(dt);
						if (0.0 <= f && f <= 59.0)
							p[no-1].lon_sec = f;
					}

					//�\��1
					//�\��2
				}
			}
			fclose(fp);
		}
		//���̃|�C���^�ֈړ�
		p = p + kp[i].max;
	}
	return SYS_ERR_SUCCESS;
}

/*---------------------------------------------------------
*	�֐��FgetSysConstOptName
*	�@�\�F�Ǖ⑫���̏��ǂݍ���
*	�����Fapp               [IN]  �f�[�^�p�X
        �Fkp                [IN]  �ǎ�ʏ��
        �Fpt                [OUT] �Ǖ⑫���̏��
*	�ߒl�F
*	�����F�Ǖ⑫���̏��t�@�C����ǂݍ��݂܂��B
        �F�O������Ƃ��āA
        �F  �ǎ�ʏ�񂪐ݒ肳��Ă��邱��
        �F  �Ǖ⑫���̏��̗̈�͊m�ۂ���Ă��邱��
        �F�t�@�C�������݂��Ȃ��ꍇ�A�����l���ݒ肳��܂��B
---------------------------------------------------------*/
int getSysConstOptName(kind_sharedmem_t *mp, char *app, sys_kind_t *kp, sys_optname_t *pt)
{
	sys_optname_t *p = pt;
	FILE  *fp;
	char   fpath[_MAX_PATH];
	char   dt[32];
	int    no;
	int    v;
	double f;
	int    i;
//2002.11.19 �ǉ��i��������j----------------------------------------
//�t�@�C���ǂݍ��ݎ��ɍX�V�������r����悤�ɕύX
	struct _stat st;
	int ret;
//2002.11.19 �ǉ��i�����܂Łj----------------------------------------


	if (app == NULL)
		return SYS_ERR_POINTER;
	if (kp == NULL)
		return SYS_ERR_POINTER;
	if (pt == NULL)
		return SYS_ERR_POINTER;


	//�Ǖ⑫���̏��̓ǂݍ���
	for (i = 0; i < SYS_MAX_KIND; i++) {
		if (kp[i].max <= 0) continue;

		//�t�@�C���p�X�̐ݒ�
		sprintf(fpath, "%ssys_table\\csv\\%s_opt-name.csv", app, kp[i].name);

//2002.11.19 �C���i��������j----------------------------------------
//�t�@�C���ǂݍ��ݎ��ɍX�V�������r����悤�ɕύX
//		//�t�@�C���I�[�v��
//		fp = fopen(fpath, "r");

		//�t�@�C���|�C���^�̏�����
		fp = NULL;
		//�t�@�C�����̎擾
		ret = _stat(fpath, &st);
		if (ret == 0) {
			//�X�V�����̔�r
			if (mp->tim_optname[i] != st.st_mtime) {
				mp->tim_optname[i] = st.st_mtime;
				//�̈�̏�����
				memset(p, 0, kp[i].max * sizeof(sys_optname_t));
				//�t�@�C���I�[�v��
				fp = fopen(fpath, "r");
			}
		}
//2002.11.19 �C���i�����܂Łj----------------------------------------

		if (fp != NULL) {
			//�K�v�Ȃ��s��ǂݔ�΂�
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));

			while ( !feof(fp) ) {

				//�P�s�ǂݍ���
				csvread_n(fp, mCsvLine, sizeof(mCsvLine));

				//�����ǔԂ̔���
				csvsplit_n(mCsvLine, 0, dt, sizeof(dt));
				no = atoi(dt);
				if (1 <= no && no <= kp[i].max) {
					//�����ǔ�
					p[no-1].sno = no;

					//�W��
					csvsplit_n(mCsvLine, 1, dt, sizeof(dt));
					if (dt[0] != '\0') {
						f = atof(dt);
						if (-9.99 <= f && f <= 4000.00)
							p[no-1].altitude = f;
					}

					//�͌�����̋���
					csvsplit_n(mCsvLine, 2, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (-10 <= v && v <= 999999)
							p[no-1].rever_dis = v;
					}

					//�ϑ�����
					csvsplit_n(mCsvLine, 3, p[no-1].member, sizeof(p[no-1].member));

					//�d�b�ԍ�
					csvsplit_n(mCsvLine, 4, p[no-1].tel_no, sizeof(p[no-1].tel_no));

					//�Ǘ��c�̖�
					csvsplit_n(mCsvLine, 5, p[no-1].ctl_name, sizeof(p[no-1].ctl_name));

					//�Z���T�[��ʃR�[�h
					csvsplit_n(mCsvLine, 6, p[no-1].sensor_code, sizeof(p[no-1].sensor_code));

					//�ϑ���ʃR�[�h
					csvsplit_n(mCsvLine, 7, p[no-1].kind_code, sizeof(p[no-1].kind_code));

					//�ϑ��J�n���t�i�N�j
					csvsplit_n(mCsvLine, 8, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1800 <= v && v <= 2100)
							p[no-1].start_year = v;
					}

					//�ϑ��J�n���t�i���j
					csvsplit_n(mCsvLine, 9, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 12)
							p[no-1].start_mon = v;
					}

					//�ϑ��J�n���t�i���j
					csvsplit_n(mCsvLine, 10, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 31)
							p[no-1].start_day = v;
					}

					//�Ď��Ώۋ�於�́i���h�j
					csvsplit_n(mCsvLine, 11, p[no-1].obs_area_suibou, sizeof(p[no-1].obs_area_suibou));

					//�Ď��Ώۋ�於�́i���h�j
					csvsplit_n(mCsvLine, 12, p[no-1].obs_area_sabo, sizeof(p[no-1].obs_area_sabo));

					//�Ď��Ώۋ�於�́i���H�j
					csvsplit_n(mCsvLine, 13, p[no-1].obs_area_road, sizeof(p[no-1].obs_area_road));

					//�\��1

					//�\��2
				}
			}
			fclose(fp);
		}
		//���̃|�C���^�ֈړ�
		p = p + kp[i].max;
	}
	return SYS_ERR_SUCCESS;
}

/*---------------------------------------------------------
*	�֐��FgetSysConstCode
*	�@�\�F�ǃR�[�h���ǂݍ���
*	�����Fapp               [IN]  �f�[�^�p�X
        �Fkp                [IN]  �ǎ�ʏ��
        �Fpt                [OUT] �ǖ��̏��
*	�ߒl�F
*	�����F�ǃR�[�h���t�@�C����ǂݍ��݂܂��B
        �F�O������Ƃ��āA
        �F  �ǎ�ʏ�񂪐ݒ肳��Ă��邱��
        �F  �ǉғ����̗̈�͊m�ۂ���Ă��邱��
        �F�t�@�C�������݂��Ȃ��ꍇ�A�����l���ݒ肳��܂��B
---------------------------------------------------------*/
int getSysConstCode(kind_sharedmem_t *mp, char *app, sys_kind_t *kp, sys_code_t *pt)
{
	sys_code_t *p = pt;
	FILE *fp;
	char fpath[_MAX_PATH];
	char dt[32];
	int  no;
	int  v;
	int  i;
//2002.11.19 �ǉ��i��������j----------------------------------------
//�t�@�C���ǂݍ��ݎ��ɍX�V�������r����悤�ɕύX
	struct _stat st;
	int  ret;
//2002.11.19 �ǉ��i�����܂Łj----------------------------------------


	if (app == NULL)
		return SYS_ERR_POINTER;
	if (kp == NULL)
		return SYS_ERR_POINTER;
	if (pt == NULL)
		return SYS_ERR_POINTER;

	//�ǃR�[�h���̓ǂݍ���
	for (i = 0; i < SYS_MAX_KIND; i++) {
		if (kp[i].max <= 0) continue;

		//�t�@�C���p�X�̐ݒ�
		sprintf(fpath, "%ssys_table\\csv\\%s_code.csv", app, kp[i].name);

//2002.11.19 �C���i��������j----------------------------------------
//�t�@�C���ǂݍ��ݎ��ɍX�V�������r����悤�ɕύX
//		//�t�@�C���I�[�v��
//		fp = fopen(fpath, "r");

		//�t�@�C���|�C���^�̏�����
		fp = NULL;
		//�t�@�C�����̎擾
		ret = _stat(fpath, &st);
		if (ret == 0) {
			//�X�V�����̔�r
			if (mp->tim_code[i] != st.st_mtime) {
				mp->tim_code[i] = st.st_mtime;
				//�̈�̏�����
				memset(p, 0, kp[i].max * sizeof(sys_code_t));
				//�t�@�C���I�[�v��
				fp = fopen(fpath, "r");
			}
		}
//2002.11.19 �C���i�����܂Łj----------------------------------------

		if (fp != NULL) {
			//�K�v�Ȃ��s��ǂݔ�΂�
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));

			while ( !feof(fp) ) {

				//�P�s�ǂݍ���
				csvread_n(fp, mCsvLine, sizeof(mCsvLine));

				//�����ǔԂ̔���
				csvsplit_n(mCsvLine, 0, dt, sizeof(dt));
				no = atoi(dt);
				if (1 <= no && no <= kp[i].max) {
					//�����ǔ�
					p[no-1].sno = no;

					//�G���A�ԍ��i���j
					csvsplit_n(mCsvLine, 1, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 255)
							p[no-1].area_no = v;
					}

					//�������ԍ��i���j
					csvsplit_n(mCsvLine, 2, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 255)
							p[no-1].office_no = v;
					}

					//���n��R�[�h�i���j
					csvsplit_n(mCsvLine, 3, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 999)
							p[no-1].river_sys_code = v;
					}

					//�͐�R�[�h�i���j
					csvsplit_n(mCsvLine, 4, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 999)
							p[no-1].river_code = v;
					}

					//�ϑ����ԍ��i���j
					csvsplit_n(mCsvLine, 5, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (0 <= v && v <= 65535)
							p[no-1].observatory_no = v;
					}

					//�s���{���R�[�h�i���j
					csvsplit_n(mCsvLine, 6, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 99)
							p[no-1].pref_code = v;
					}

					//�s�����R�[�h�i���j
					csvsplit_n(mCsvLine, 7, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 999)
							p[no-1].city_code = v;
					}

					//�\��1

					//�\��2
				}
			}
			fclose(fp);
		}
		//���̃|�C���^�ֈړ�
		p = p + kp[i].max;
	}
	return SYS_ERR_SUCCESS;
}

#if 0
/*---------------------------------------------------------
*	�֐��FgetSysConstComp1
*	�@�\�F�Ǎ\�����ǂݍ���
*	�����Fapp               [IN]  �f�[�^�p�X
        �Fkp                [IN]  �ǎ�ʏ��
        �Fpt                [OUT] �ǖ��̏��
*	�ߒl�F
*	�����F�Ǎ\�����t�@�C����ǂݍ��݂܂��B
        �F�O������Ƃ��āA
        �F  �ǎ�ʏ�񂪐ݒ肳��Ă��邱��
        �F  �ǉғ����̗̈�͊m�ۂ���Ă��邱��
        �F�t�@�C�������݂��Ȃ��ꍇ�A�����l���ݒ肳��܂��B
---------------------------------------------------------*/
int getSysConstComp1(kind_sharedmem_t *mp, char *app, sys_kind_t *kp, sys_comp1_t *pt)
{
	sys_comp1_t *p = pt;
	FILE  *fp;
	char   fpath[_MAX_PATH];
	char   dt[32];
	int    no;
	int    v;
	double f;
	int    i;
//2002.11.19 �ǉ��i��������j----------------------------------------
//�t�@�C���ǂݍ��ݎ��ɍX�V�������r����悤�ɕύX
	struct _stat st;
	int ret;
//2002.11.19 �C���i�����܂Łj----------------------------------------


	if (app == NULL)
		return SYS_ERR_POINTER;
	if (kp == NULL)
		return SYS_ERR_POINTER;
	if (pt == NULL)
		return SYS_ERR_POINTER;

	//�Ǎ\�����̓ǂݍ���
	for (i = 0; i < SYS_MAX_KIND; i++) {
		if (kp[i].max <= 0) continue;

		//�t�@�C���p�X�̐ݒ�
		sprintf(fpath, "%ssys_table\\csv\\%s_comp1.csv", app, kp[i].name);

//2002.11.19 �C���i��������j----------------------------------------
//�t�@�C���ǂݍ��ݎ��ɍX�V�������r����悤�ɕύX
//		//�t�@�C���I�[�v��
//		fp = fopen(fpath, "r");

		//�t�@�C���|�C���^�̏�����
		fp = NULL;
		//�t�@�C�����̎擾
		ret = _stat(fpath, &st);
		if (ret == 0) {
			//�X�V�����̔�r
			if (mp->tim_comp1[i] != st.st_mtime) {
				mp->tim_comp1[i] = st.st_mtime;
				//�̈�̏�����
				memset(p, 0, kp[i].max * sizeof(sys_comp1_t));
				//�t�@�C���I�[�v��
				fp = fopen(fpath, "r");
			}
		}
//2002.11.19 �C���i�����܂Łj----------------------------------------

		if (fp != NULL) {
			//�K�v�Ȃ��s��ǂݔ�΂�
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));

			while ( !feof(fp) ) {

				//�P�s�ǂݍ���
				csvread_n(fp, mCsvLine, sizeof(mCsvLine));

				//�����ǔԂ̔���
				csvsplit_n(mCsvLine, 0, dt, sizeof(dt));
				no = atoi(dt);
				if (1 <= no && no <= kp[i].max) {
					//�����ǔ�
					p[no-1].sno = no;

					//�������ԍ�
					csvsplit_n(mCsvLine, 1, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 99)
							p[no-1].office_no = v;
					}

					//
					//�Z���T�[�P
					//

					//�Z���T�[
					csvsplit_n(mCsvLine, 2, p[no-1].s1_sensor, sizeof(p[no-1].s1_sensor));
					//�`��
					csvsplit_n(mCsvLine, 3, p[no-1].s1_form, sizeof(p[no-1].s1_form));
					//�^��
					csvsplit_n(mCsvLine, 4, p[no-1].s1_type, sizeof(p[no-1].s1_type));
					//���t�P
					csvsplit_n(mCsvLine, 5, p[no-1].s1_date_1, sizeof(p[no-1].s1_date_1));
					//���t�Q
					csvsplit_n(mCsvLine, 6, p[no-1].s1_date_2, sizeof(p[no-1].s1_date_2));
					//���t�R
					csvsplit_n(mCsvLine, 7, p[no-1].s1_date_3, sizeof(p[no-1].s1_date_3));
					//�����Ǝ�
					csvsplit_n(mCsvLine, 8, p[no-1].s1_introduce, sizeof(p[no-1].s1_introduce));
					//�A����
					csvsplit_n(mCsvLine, 9, p[no-1].s1_intro_tel, sizeof(p[no-1].s1_intro_tel));
					//�_���Ǝ�
					csvsplit_n(mCsvLine, 10, p[no-1].s1_chacker, sizeof(p[no-1].s1_chacker));
					//�A����
					csvsplit_n(mCsvLine, 11, p[no-1].s1_chk_tel, sizeof(p[no-1].s1_chk_tel));
					//���l�P
					csvsplit_n(mCsvLine, 12, p[no-1].s1_note_1, sizeof(p[no-1].s1_note_1));
					//���l�Q
					csvsplit_n(mCsvLine, 13, p[no-1].s1_note_2, sizeof(p[no-1].s1_note_2));
					//���l�R
					csvsplit_n(mCsvLine, 14, p[no-1].s1_note_3, sizeof(p[no-1].s1_note_3));
					//���l�S
					csvsplit_n(mCsvLine, 15, p[no-1].s1_note_4, sizeof(p[no-1].s1_note_4));

					//
					//�Z���T�[�Q
					//

					//�Z���T�[
					csvsplit_n(mCsvLine, 16, p[no-1].s2_sensor, sizeof(p[no-1].s2_sensor));
					//�`��
					csvsplit_n(mCsvLine, 17, p[no-1].s2_form, sizeof(p[no-1].s2_form));
					//�^��
					csvsplit_n(mCsvLine, 18, p[no-1].s2_type, sizeof(p[no-1].s2_type));
					//���t�P
					csvsplit_n(mCsvLine, 19, p[no-1].s2_date_1, sizeof(p[no-1].s2_date_1));
					//���t�Q
					csvsplit_n(mCsvLine, 20, p[no-1].s2_date_2, sizeof(p[no-1].s2_date_2));
					//���t�R
					csvsplit_n(mCsvLine, 21, p[no-1].s2_date_3, sizeof(p[no-1].s2_date_3));
					//�����Ǝ�
					csvsplit_n(mCsvLine, 22, p[no-1].s2_introduce, sizeof(p[no-1].s2_introduce));
					//�A����
					csvsplit_n(mCsvLine, 23, p[no-1].s2_intro_tel, sizeof(p[no-1].s2_intro_tel));
					//�_���Ǝ�
					csvsplit_n(mCsvLine, 24, p[no-1].s2_chacker, sizeof(p[no-1].s2_chacker));
					//�A����
					csvsplit_n(mCsvLine, 25, p[no-1].s2_chk_tel, sizeof(p[no-1].s2_chk_tel));
					//���l�P
					csvsplit_n(mCsvLine, 26, p[no-1].s2_note_1, sizeof(p[no-1].s2_note_1));
					//���l�Q
					csvsplit_n(mCsvLine, 27, p[no-1].s2_note_2, sizeof(p[no-1].s2_note_2));
					//���l�R
					csvsplit_n(mCsvLine, 28, p[no-1].s2_note_3, sizeof(p[no-1].s2_note_3));
					//���l�S
					csvsplit_n(mCsvLine, 29, p[no-1].s2_note_4, sizeof(p[no-1].s2_note_4));

					//
					//�Z���T�[�R
					//

					//�Z���T�[
					csvsplit_n(mCsvLine, 30, p[no-1].s3_sensor, sizeof(p[no-1].s3_sensor));
					//�`��
					csvsplit_n(mCsvLine, 31, p[no-1].s3_form, sizeof(p[no-1].s3_form));
					//�^��
					csvsplit_n(mCsvLine, 32, p[no-1].s3_type, sizeof(p[no-1].s3_type));
					//���t�P
					csvsplit_n(mCsvLine, 33, p[no-1].s3_date_1, sizeof(p[no-1].s3_date_1));
					//���t�Q
					csvsplit_n(mCsvLine, 34, p[no-1].s3_date_2, sizeof(p[no-1].s3_date_2));
					//���t�R
					csvsplit_n(mCsvLine, 35, p[no-1].s3_date_3, sizeof(p[no-1].s3_date_3));
					//�����Ǝ�
					csvsplit_n(mCsvLine, 36, p[no-1].s3_introduce, sizeof(p[no-1].s3_introduce));
					//�A����
					csvsplit_n(mCsvLine, 37, p[no-1].s3_intro_tel, sizeof(p[no-1].s3_intro_tel));
					//�_���Ǝ�
					csvsplit_n(mCsvLine, 38, p[no-1].s3_chacker, sizeof(p[no-1].s3_chacker));
					//�A����
					csvsplit_n(mCsvLine, 39, p[no-1].s3_chk_tel, sizeof(p[no-1].s3_chk_tel));
					//���l�P
					csvsplit_n(mCsvLine, 40, p[no-1].s3_note_1, sizeof(p[no-1].s3_note_1));
					//���l�Q
					csvsplit_n(mCsvLine, 41, p[no-1].s3_note_2, sizeof(p[no-1].s3_note_2));
					//���l�R
					csvsplit_n(mCsvLine, 42, p[no-1].s3_note_3, sizeof(p[no-1].s3_note_3));
					//���l�S
					csvsplit_n(mCsvLine, 43, p[no-1].s3_note_4, sizeof(p[no-1].s3_note_4));

					//
					//�Z���T�[�S
					//

					//�Z���T�[
					csvsplit_n(mCsvLine, 44, p[no-1].s4_sensor, sizeof(p[no-1].s4_sensor));
					//�`��
					csvsplit_n(mCsvLine, 45, p[no-1].s4_form, sizeof(p[no-1].s4_form));
					//�^��
					csvsplit_n(mCsvLine, 46, p[no-1].s4_type, sizeof(p[no-1].s4_type));
					//���t�P
					csvsplit_n(mCsvLine, 47, p[no-1].s4_date_1, sizeof(p[no-1].s4_date_1));
					//���t�Q
					csvsplit_n(mCsvLine, 48, p[no-1].s4_date_2, sizeof(p[no-1].s4_date_2));
					//���t�R
					csvsplit_n(mCsvLine, 49, p[no-1].s4_date_3, sizeof(p[no-1].s4_date_3));
					//�����Ǝ�
					csvsplit_n(mCsvLine, 50, p[no-1].s4_introduce, sizeof(p[no-1].s4_introduce));
					//�A����
					csvsplit_n(mCsvLine, 51, p[no-1].s4_intro_tel, sizeof(p[no-1].s4_intro_tel));
					//�_���Ǝ�
					csvsplit_n(mCsvLine, 52, p[no-1].s4_checker, sizeof(p[no-1].s4_checker));
					//�A����
					csvsplit_n(mCsvLine, 53, p[no-1].s4_chk_tel, sizeof(p[no-1].s4_chk_tel));
					//���l�P
					csvsplit_n(mCsvLine, 54, p[no-1].s4_note_1, sizeof(p[no-1].s4_note_1));
					//���l�Q
					csvsplit_n(mCsvLine, 55, p[no-1].s4_note_2, sizeof(p[no-1].s4_note_2));
					//���l�R
					csvsplit_n(mCsvLine, 56, p[no-1].s4_note_3, sizeof(p[no-1].s4_note_3));
					//���l�S
					csvsplit_n(mCsvLine, 57, p[no-1].s4_note_4, sizeof(p[no-1].s4_note_4));

					//
					//�Z���T�[�T
					//

					//�Z���T�[
					csvsplit_n(mCsvLine, 58, p[no-1].s5_sensor, sizeof(p[no-1].s5_sensor));
					//�`��
					csvsplit_n(mCsvLine, 59, p[no-1].s5_form, sizeof(p[no-1].s5_form));
					//�^��
					csvsplit_n(mCsvLine, 60, p[no-1].s5_type, sizeof(p[no-1].s5_type));
					//���t�P
					csvsplit_n(mCsvLine, 61, p[no-1].s5_date_1, sizeof(p[no-1].s5_date_1));
					//���t�Q
					csvsplit_n(mCsvLine, 62, p[no-1].s5_date_2, sizeof(p[no-1].s5_date_2));
					//���t�R
					csvsplit_n(mCsvLine, 63, p[no-1].s5_date_3, sizeof(p[no-1].s5_date_3));
					//�����Ǝ�
					csvsplit_n(mCsvLine, 64, p[no-1].s5_introduce, sizeof(p[no-1].s5_introduce));
					//�A����
					csvsplit_n(mCsvLine, 65, p[no-1].s5_intro_tel, sizeof(p[no-1].s5_intro_tel));
					//�_���Ǝ�
					csvsplit_n(mCsvLine, 66, p[no-1].s5_checker, sizeof(p[no-1].s5_checker));
					//�A����
					csvsplit_n(mCsvLine, 67, p[no-1].s5_chk_tel, sizeof(p[no-1].s5_chk_tel));
					//���l�P
					csvsplit_n(mCsvLine, 68, p[no-1].s5_note_1, sizeof(p[no-1].s5_note_1));
					//���l�Q
					csvsplit_n(mCsvLine, 69, p[no-1].s5_note_2, sizeof(p[no-1].s5_note_2));
					//���l�R
					csvsplit_n(mCsvLine, 70, p[no-1].s5_note_3, sizeof(p[no-1].s5_note_3));
					//���l�S
					csvsplit_n(mCsvLine, 71, p[no-1].s5_note_4, sizeof(p[no-1].s5_note_4));

					//
					//�L�^�v�P
					//

					//�L�^�v
					csvsplit_n(mCsvLine, 72, p[no-1].r1_recorder, sizeof(p[no-1].r1_recorder));
					//�`��
					csvsplit_n(mCsvLine, 73, p[no-1].r1_form, sizeof(p[no-1].r1_form));
					//�^��
					csvsplit_n(mCsvLine, 74, p[no-1].r1_type, sizeof(p[no-1].r1_type));
					//�^�p�J�n��
					csvsplit_n(mCsvLine, 75, p[no-1].r1_begin, sizeof(p[no-1].r1_begin));
					//�X�V�N����
					csvsplit_n(mCsvLine, 76, p[no-1].r1_update, sizeof(p[no-1].r1_update));
					//�����Ǝ�
					csvsplit_n(mCsvLine, 77, p[no-1].r1_introduce, sizeof(p[no-1].r1_introduce));
					//�A����
					csvsplit_n(mCsvLine, 78, p[no-1].r1_intro_tel, sizeof(p[no-1].r1_intro_tel));
					//�_���Ǝ�
					csvsplit_n(mCsvLine, 79, p[no-1].r1_checker, sizeof(p[no-1].r1_checker));
					//�A����
					csvsplit_n(mCsvLine, 80, p[no-1].r1_chk_tel, sizeof(p[no-1].r1_chk_tel));
					//�L�^�v�̊�����
					csvsplit_n(mCsvLine, 81, p[no-1].r1_reel_day, sizeof(p[no-1].r1_reel_day));
					//�L�^�v�̌̏ᒷ�������L��
					csvsplit_n(mCsvLine, 82, p[no-1].r1_breakdown, sizeof(p[no-1].r1_breakdown));
					//���l�P
					csvsplit_n(mCsvLine, 83, p[no-1].r1_note_1, sizeof(p[no-1].r1_note_1));
					//���l�Q
					csvsplit_n(mCsvLine, 84, p[no-1].r1_note_2, sizeof(p[no-1].r1_note_2));

					//
					//�L�^�v�Q
					//

					//�L�^�v
					csvsplit_n(mCsvLine, 85, p[no-1].r2_recorder, sizeof(p[no-1].r2_recorder));
					//�`��
					csvsplit_n(mCsvLine, 86, p[no-1].r2_form, sizeof(p[no-1].r2_form));
					//�^��
					csvsplit_n(mCsvLine, 87, p[no-1].r2_type, sizeof(p[no-1].r2_type));
					//�^�p�J�n��
					csvsplit_n(mCsvLine, 88, p[no-1].r2_begin, sizeof(p[no-1].r2_begin));
					//�X�V�N����
					csvsplit_n(mCsvLine, 89, p[no-1].r2_update, sizeof(p[no-1].r2_update));
					//�����Ǝ�
					csvsplit_n(mCsvLine, 90, p[no-1].r2_introduce, sizeof(p[no-1].r2_introduce));
					//�A����
					csvsplit_n(mCsvLine, 91, p[no-1].r2_intro_tel, sizeof(p[no-1].r2_intro_tel));
					//�_���Ǝ�
					csvsplit_n(mCsvLine, 92, p[no-1].r2_checker, sizeof(p[no-1].r2_checker));
					//�A����
					csvsplit_n(mCsvLine, 93, p[no-1].r2_chk_tel, sizeof(p[no-1].r2_chk_tel));
					//�L�^�v�̊�����
					csvsplit_n(mCsvLine, 94, p[no-1].r2_reel_day, sizeof(p[no-1].r2_reel_day));
					//�L�^�v�̌̏ᒷ�������L��
					csvsplit_n(mCsvLine, 95, p[no-1].r2_breakdown, sizeof(p[no-1].r2_breakdown));
					//���l�P
					csvsplit_n(mCsvLine, 96, p[no-1].r2_note_1, sizeof(p[no-1].r2_note_1));
					//���l�Q
					csvsplit_n(mCsvLine, 97, p[no-1].r2_note_2, sizeof(p[no-1].r2_note_2));


					//
					//�L�^�v�R
					//

					//�L�^�v
					csvsplit_n(mCsvLine, 98, p[no-1].r3_recorder, sizeof(p[no-1].r3_recorder));
					//�`��
					csvsplit_n(mCsvLine, 99, p[no-1].r3_form, sizeof(p[no-1].r3_form));
					//�^��
					csvsplit_n(mCsvLine, 100, p[no-1].r3_type, sizeof(p[no-1].r3_type));
					//�^�p�J�n��
					csvsplit_n(mCsvLine, 101, p[no-1].r3_begin, sizeof(p[no-1].r3_begin));
					//�X�V�N����
					csvsplit_n(mCsvLine, 102, p[no-1].r3_update, sizeof(p[no-1].r3_update));
					//�����Ǝ�
					csvsplit_n(mCsvLine, 103, p[no-1].r3_introduce, sizeof(p[no-1].r3_introduce));
					//�A����
					csvsplit_n(mCsvLine, 104, p[no-1].r3_intro_tel, sizeof(p[no-1].r3_intro_tel));
					//�_���Ǝ�
					csvsplit_n(mCsvLine, 105, p[no-1].r3_checker, sizeof(p[no-1].r3_checker));
					//�A����
					csvsplit_n(mCsvLine, 106, p[no-1].r3_chk_tel, sizeof(p[no-1].r3_chk_tel));
					//�L�^�v�̊�����
					csvsplit_n(mCsvLine, 107, p[no-1].r3_reel_day, sizeof(p[no-1].r3_reel_day));
					//�L�^�v�̌̏ᒷ�������L��
					csvsplit_n(mCsvLine, 108, p[no-1].r3_breakdown, sizeof(p[no-1].r3_breakdown));
					//���l�P
					csvsplit_n(mCsvLine, 109, p[no-1].r3_note_1, sizeof(p[no-1].r3_note_1));
					//���l�Q
					csvsplit_n(mCsvLine, 110, p[no-1].r3_note_2, sizeof(p[no-1].r3_note_2));

					//
					//�L�^�v�S
					//

					//�L�^�v
					csvsplit_n(mCsvLine, 111, p[no-1].r4_recorder, sizeof(p[no-1].r4_recorder));
					//�`��
					csvsplit_n(mCsvLine, 112, p[no-1].r4_form, sizeof(p[no-1].r4_form));
					//�^��
					csvsplit_n(mCsvLine, 113, p[no-1].r4_type, sizeof(p[no-1].r4_type));
					//�^�p�J�n��
					csvsplit_n(mCsvLine, 114, p[no-1].r4_begin, sizeof(p[no-1].r4_begin));
					//�X�V�N����
					csvsplit_n(mCsvLine, 115, p[no-1].r4_update, sizeof(p[no-1].r4_update));
					//�����Ǝ�
					csvsplit_n(mCsvLine, 116, p[no-1].r4_introduce, sizeof(p[no-1].r4_introduce));
					//�A����
					csvsplit_n(mCsvLine, 117, p[no-1].r4_intro_tel, sizeof(p[no-1].r4_intro_tel));
					//�_���Ǝ�
					csvsplit_n(mCsvLine, 118, p[no-1].r4_checker, sizeof(p[no-1].r4_checker));
					//�A����
					csvsplit_n(mCsvLine, 119, p[no-1].r4_chk_tel, sizeof(p[no-1].r4_chk_tel));
					//�L�^�v�̊�����
					csvsplit_n(mCsvLine, 120, p[no-1].r4_reel_day, sizeof(p[no-1].r4_reel_day));
					//�L�^�v�̌̏ᒷ�������L��
					csvsplit_n(mCsvLine, 121, p[no-1].r4_breakdown, sizeof(p[no-1].r4_breakdown));
					//���l�P
					csvsplit_n(mCsvLine, 122, p[no-1].r4_note_1, sizeof(p[no-1].r4_note_1));
					//���l�Q
					csvsplit_n(mCsvLine, 123, p[no-1].r4_note_2, sizeof(p[no-1].r4_note_2));

					//
					//�L�^�v�T
					//

					//�L�^�v
					csvsplit_n(mCsvLine, 124, p[no-1].r5_recorder, sizeof(p[no-1].r5_recorder));
					//�`��
					csvsplit_n(mCsvLine, 125, p[no-1].r5_form, sizeof(p[no-1].r5_form));
					//�^��
					csvsplit_n(mCsvLine, 126, p[no-1].r5_type, sizeof(p[no-1].r5_type));
					//�^�p�J�n��
					csvsplit_n(mCsvLine, 127, p[no-1].r5_begin, sizeof(p[no-1].r5_begin));
					//�X�V�N����
					csvsplit_n(mCsvLine, 128, p[no-1].r5_update, sizeof(p[no-1].r5_update));
					//�����Ǝ�
					csvsplit_n(mCsvLine, 129, p[no-1].r5_introduce, sizeof(p[no-1].r5_introduce));
					//�A����
					csvsplit_n(mCsvLine, 130, p[no-1].r5_intro_tel, sizeof(p[no-1].r5_intro_tel));
					//�_���Ǝ�
					csvsplit_n(mCsvLine, 131, p[no-1].r5_checker, sizeof(p[no-1].r5_checker));
					//�A����
					csvsplit_n(mCsvLine, 132, p[no-1].r5_chk_tel, sizeof(p[no-1].r5_chk_tel));
					//�L�^�v�̊�����
					csvsplit_n(mCsvLine, 133, p[no-1].r5_reel_day, sizeof(p[no-1].r5_reel_day));
					//�L�^�v�̌̏ᒷ�������L��
					csvsplit_n(mCsvLine, 134, p[no-1].r5_breakdown, sizeof(p[no-1].r5_breakdown));
					//���l�P
					csvsplit_n(mCsvLine, 135, p[no-1].r5_note_1, sizeof(p[no-1].r5_note_1));
					//���l�Q
					csvsplit_n(mCsvLine, 136, p[no-1].r5_note_2, sizeof(p[no-1].r5_note_2));

					//
					//�`�����
					//

					//�`�����
					csvsplit_n(mCsvLine, 137, p[no-1].t_transmitte, sizeof(p[no-1].t_transmitte));
					//�`��
					csvsplit_n(mCsvLine, 138, p[no-1].t_form, sizeof(p[no-1].t_form));
					//�^��
					csvsplit_n(mCsvLine, 139, p[no-1].t_type, sizeof(p[no-1].t_type));
					//�^�p�J�n��
					csvsplit_n(mCsvLine, 140, p[no-1].t_begin, sizeof(p[no-1].t_begin));
					//�X�V�N����
					csvsplit_n(mCsvLine, 141, p[no-1].t_update, sizeof(p[no-1].t_update));
					//�����Ǝ�
					csvsplit_n(mCsvLine, 142, p[no-1].t_introduce, sizeof(p[no-1].t_introduce));
					//�A����
					csvsplit_n(mCsvLine, 143, p[no-1].t_intro_tel, sizeof(p[no-1].t_intro_tel));
					//�_���Ǝ�
					csvsplit_n(mCsvLine, 144, p[no-1].t_checker, sizeof(p[no-1].t_checker));
					//�A����
					csvsplit_n(mCsvLine, 145, p[no-1].t_chk_tel, sizeof(p[no-1].t_chk_tel));
					//���M���g��
					csvsplit_n(mCsvLine, 146, p[no-1].t_snd_hz, sizeof(p[no-1].t_snd_hz));
					//��M���g��
					csvsplit_n(mCsvLine, 147, p[no-1].t_rcv_hz, sizeof(p[no-1].t_rcv_hz));

					//
					//�󒆐�
					//

					//�󒆐�
					csvsplit_n(mCsvLine, 148, p[no-1].a_air_line, sizeof(p[no-1].a_air_line));
					//�ʐM����
					csvsplit_n(mCsvLine, 149, p[no-1].a_form, sizeof(p[no-1].a_form));
					//���l
					csvsplit_n(mCsvLine, 150, p[no-1].a_note_1, sizeof(p[no-1].a_note_1));
					//���l
					csvsplit_n(mCsvLine, 151, p[no-1].a_note_2, sizeof(p[no-1].a_note_2));

					//
					//�d��
					//

					//�d��
					csvsplit_n(mCsvLine, 152, p[no-1].p_power, sizeof(p[no-1].p_power));
					//�`��
					csvsplit_n(mCsvLine, 153, p[no-1].p_form, sizeof(p[no-1].p_form));
					//�^��
					csvsplit_n(mCsvLine, 154, p[no-1].p_type, sizeof(p[no-1].p_type));
					//�^�p�J�n��
					csvsplit_n(mCsvLine, 155, p[no-1].p_begin, sizeof(p[no-1].p_begin));
					//�X�V�N����
					csvsplit_n(mCsvLine, 156, p[no-1].p_update, sizeof(p[no-1].p_update));
					//�����Ǝ�
					csvsplit_n(mCsvLine, 157, p[no-1].p_introduce, sizeof(p[no-1].p_introduce));
					//�A����
					csvsplit_n(mCsvLine, 158, p[no-1].p_intro_tel, sizeof(p[no-1].p_intro_tel));
					//�_���Ǝ�
					csvsplit_n(mCsvLine, 159, p[no-1].p_checker, sizeof(p[no-1].p_checker));
					//�A����
					csvsplit_n(mCsvLine, 160, p[no-1].p_chk_tel, sizeof(p[no-1].p_chk_tel));
					//�g�p�d��
					csvsplit_n(mCsvLine, 161, p[no-1].p_name, sizeof(p[no-1].p_name));
					//�~�d�r�e��
					csvsplit_n(mCsvLine, 162, p[no-1].p_capacity, sizeof(p[no-1].p_capacity));
					//���l
					csvsplit_n(mCsvLine, 163, p[no-1].p_note_1, sizeof(p[no-1].p_note_1));
					//���l
					csvsplit_n(mCsvLine, 164, p[no-1].p_note_2, sizeof(p[no-1].p_note_2));

					//
					//�ǎ�
					//

					//�ǎ�
					csvsplit_n(mCsvLine, 165, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 99)
							p[no-1].o_office = v;
					}
					//�ǎɎ��
					csvsplit_n(mCsvLine, 166, p[no-1].o_kind, sizeof(p[no-1].o_kind));
					//�ǎɕ~�n
					csvsplit_n(mCsvLine, 167, p[no-1].o_site, sizeof(p[no-1].o_site));
					//�ǎɌ���
					csvsplit_n(mCsvLine, 168, p[no-1].o_space, sizeof(p[no-1].o_space));
					//���̑�
					csvsplit_n(mCsvLine, 169, p[no-1].o_others, sizeof(p[no-1].o_others));
					//�ؒn�l��
					csvsplit_n(mCsvLine, 170, p[no-1].o_tenant, sizeof(p[no-1].o_tenant));
					//�ؒn��
					csvsplit_n(mCsvLine, 171, p[no-1].o_rent, sizeof(p[no-1].o_rent));
					//�d������
					csvsplit_n(mCsvLine, 172, p[no-1].o_utility_pole, sizeof(p[no-1].o_utility_pole));
					//�L�^�̗L���ۊǏꏊ
					csvsplit_n(mCsvLine, 173, p[no-1].o_rec_keep, sizeof(p[no-1].o_rec_keep));
					//�\��
					//�\��


					//
					//�e�����[�^�ݒu��
					//
					//�ݒu��
					csvsplit_n(mCsvLine, 176, p[no-1].tm_telemeter, sizeof(p[no-1].tm_telemeter));
					//�e�����[�^�J�n���t�i�N�j
					csvsplit_n(mCsvLine, 177, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1800 <= v && v <= 2100)
							p[no-1].tm_year = v;
					}
					//�e�����[�^�J�n���t�i���j
					csvsplit_n(mCsvLine, 178, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 12)
							p[no-1].tm_mon = v;
					}
					//�e�����[�^�J�n���t�i���j
					csvsplit_n(mCsvLine, 179, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 31)
							p[no-1].tm_day = v;
					}
					//�w��n�n�Ս�
					csvsplit_n(mCsvLine, 180, dt, sizeof(dt));
					if (dt[0] != '\0') {
						f = atof(dt);
						if (0.00 <= f && f <= 999.99)
							p[no-1].tm_back_basehi = f;
					}
					//�ʐ��W
					csvsplit_n(mCsvLine, 181, p[no-1].tm_sign, sizeof(p[no-1].tm_sign));
					//�\��
					//�\��

				}
			}
			fclose(fp);
		}
		//���̃|�C���^�ֈړ�
		p = p + kp[i].max;
	}
	return SYS_ERR_SUCCESS;
}

/*---------------------------------------------------------
*	�֐��FgetSysConstComp2
*	�@�\�F�Ǎ\�����ǂݍ���
*	�����Fapp               [IN]  �f�[�^�p�X
        �Fkp                [IN]  �ǎ�ʏ��
        �Fpt                [OUT] �ǖ��̏��
*	�ߒl�F
*	�����F�Ǎ\�����t�@�C����ǂݍ��݂܂��B
        �F�O������Ƃ��āA
        �F  �ǎ�ʏ�񂪐ݒ肳��Ă��邱��
        �F  �ǉғ����̗̈�͊m�ۂ���Ă��邱��
        �F�t�@�C�������݂��Ȃ��ꍇ�A�����l���ݒ肳��܂��B
---------------------------------------------------------*/
int getSysConstComp2(kind_sharedmem_t *mp, char *app, sys_kind_t *kp, sys_comp2_t *pt)
{
	sys_comp2_t *p = pt;
	FILE *fp;
	char fpath[_MAX_PATH];
	char dt[32];
	int  no;
	int  v;
	int  i;
//2002.11.19 �ǉ��i��������j----------------------------------------
//�t�@�C���ǂݍ��ݎ��ɍX�V�������r����悤�ɕύX
	struct _stat st;
	int  ret;
//2002.11.19 �ǉ��i�����܂Łj----------------------------------------


	if (app == NULL)
		return SYS_ERR_POINTER;
	if (kp == NULL)
		return SYS_ERR_POINTER;
	if (pt == NULL)
		return SYS_ERR_POINTER;

	//�Ǎ\�����̓ǂݍ���
	for (i = 0; i < SYS_MAX_KIND; i++) {
		if (kp[i].max <= 0) continue;

		//�t�@�C���p�X�̐ݒ�
		sprintf(fpath, "%ssys_table\\csv\\%s_comp2.csv", app, kp[i].name);

//2002.11.19 �C���i��������j----------------------------------------
//�t�@�C���ǂݍ��ݎ��ɍX�V�������r����悤�ɕύX
//		//�t�@�C���I�[�v��
//		fp = fopen(fpath, "r");

		//�t�@�C���|�C���^�̏�����
		fp = NULL;
		//�t�@�C�����̎擾
		ret = _stat(fpath, &st);
		if (ret == 0) {
			//�X�V�����̔�r
			if (mp->tim_comp2[i] != st.st_mtime) {
				mp->tim_comp2[i] = st.st_mtime;
				//�̈�̏�����
				memset(p, 0, kp[i].max * sizeof(sys_comp2_t));
				//�t�@�C���I�[�v��
				fp = fopen(fpath, "r");
			}
		}
//2002.11.19 �C���i�����܂Łj----------------------------------------
		if (fp != NULL) {
			//�K�v�Ȃ��s��ǂݔ�΂�
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));

			while ( !feof(fp) ) {

				//�P�s�ǂݍ���
				csvread_n(fp, mCsvLine, sizeof(mCsvLine));

				//�����ǔԂ̔���
				csvsplit_n(mCsvLine, 0, dt, sizeof(dt));
				no = atoi(dt);
				if (1 <= no && no <= kp[i].max) {
					//�����ǔ�
					p[no-1].sno = no;

					//�������ԍ�
					csvsplit_n(mCsvLine, 1, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 99)
							p[no-1].office_no = v;
					}

					//
					//�����������u�P
					//

					//�����������u
					csvsplit_n(mCsvLine, 2, p[no-1].u1_unit, sizeof(p[no-1].u1_unit));
					//�`��
					csvsplit_n(mCsvLine, 3, p[no-1].u1_form, sizeof(p[no-1].u1_form));
					//�^��
					csvsplit_n(mCsvLine, 4, p[no-1].u1_type, sizeof(p[no-1].u1_type));
					//���t�P
					csvsplit_n(mCsvLine, 5, p[no-1].u1_date_1, sizeof(p[no-1].u1_date_1));
					//���t�Q
					csvsplit_n(mCsvLine, 6, p[no-1].u1_date_2, sizeof(p[no-1].u1_date_2));
					//���t�R
					csvsplit_n(mCsvLine, 7, p[no-1].u1_date_3, sizeof(p[no-1].u1_date_3));
					//�����Ǝ�
					csvsplit_n(mCsvLine, 8, p[no-1].u1_introduce, sizeof(p[no-1].u1_introduce));
					//�A����
					csvsplit_n(mCsvLine, 9, p[no-1].u1_intro_tel, sizeof(p[no-1].u1_intro_tel));
					//�_���Ǝ�
					csvsplit_n(mCsvLine, 10, p[no-1].u1_checker, sizeof(p[no-1].u1_checker));
					//�A����
					csvsplit_n(mCsvLine, 11, p[no-1].u1_chk_tel, sizeof(p[no-1].u1_chk_tel));
					//���l�P
					csvsplit_n(mCsvLine, 12, p[no-1].u1_note_1, sizeof(p[no-1].u1_note_1));
					//���l�Q
					csvsplit_n(mCsvLine, 13, p[no-1].u1_note_2, sizeof(p[no-1].u1_note_2));
					//���l�R
					csvsplit_n(mCsvLine, 14, p[no-1].u1_note_3, sizeof(p[no-1].u1_note_3));
					//���l�S
					csvsplit_n(mCsvLine, 15, p[no-1].u1_note_4, sizeof(p[no-1].u1_note_4));

					//
					//�����������u�Q
					//

					//�����������u
					csvsplit_n(mCsvLine, 16, p[no-1].u2_unit, sizeof(p[no-1].u2_unit));
					//�`��
					csvsplit_n(mCsvLine, 17, p[no-1].u2_form, sizeof(p[no-1].u2_form));
					//�^��
					csvsplit_n(mCsvLine, 18, p[no-1].u2_type, sizeof(p[no-1].u2_type));
					//���t�P
					csvsplit_n(mCsvLine, 19, p[no-1].u2_date_1, sizeof(p[no-1].u2_date_1));
					//���t�Q
					csvsplit_n(mCsvLine, 20, p[no-1].u2_date_2, sizeof(p[no-1].u2_date_2));
					//���t�R
					csvsplit_n(mCsvLine, 21, p[no-1].u2_date_3, sizeof(p[no-1].u2_date_3));
					//�����Ǝ�
					csvsplit_n(mCsvLine, 22, p[no-1].u2_introduce, sizeof(p[no-1].u2_introduce));
					//�A����
					csvsplit_n(mCsvLine, 23, p[no-1].u2_intro_tel, sizeof(p[no-1].u2_intro_tel));
					//�_���Ǝ�
					csvsplit_n(mCsvLine, 24, p[no-1].u2_checker, sizeof(p[no-1].u2_checker));
					//�A����
					csvsplit_n(mCsvLine, 25, p[no-1].u2_chk_tel, sizeof(p[no-1].u2_chk_tel));
					//���l�P
					csvsplit_n(mCsvLine, 26, p[no-1].u2_note_1, sizeof(p[no-1].u2_note_1));
					//���l�Q
					csvsplit_n(mCsvLine, 27, p[no-1].u2_note_2, sizeof(p[no-1].u2_note_2));
					//���l�R
					csvsplit_n(mCsvLine, 28, p[no-1].u2_note_3, sizeof(p[no-1].u2_note_3));
					//���l�S
					csvsplit_n(mCsvLine, 29, p[no-1].u2_note_4, sizeof(p[no-1].u2_note_4));

					//
					//�����������u�R
					//

					//�����������u
					csvsplit_n(mCsvLine, 30, p[no-1].u3_unit, sizeof(p[no-1].u3_unit));
					//�`��
					csvsplit_n(mCsvLine, 31, p[no-1].u3_form, sizeof(p[no-1].u3_form));
					//�^��
					csvsplit_n(mCsvLine, 32, p[no-1].u3_type, sizeof(p[no-1].u3_type));
					//���t�P
					csvsplit_n(mCsvLine, 33, p[no-1].u3_date_1, sizeof(p[no-1].u3_date_1));
					//���t�Q
					csvsplit_n(mCsvLine, 34, p[no-1].u3_date_2, sizeof(p[no-1].u3_date_2));
					//���t�R
					csvsplit_n(mCsvLine, 35, p[no-1].u3_date_3, sizeof(p[no-1].u3_date_3));
					//�����Ǝ�
					csvsplit_n(mCsvLine, 36, p[no-1].u3_introduce, sizeof(p[no-1].u3_introduce));
					//�A����
					csvsplit_n(mCsvLine, 37, p[no-1].u3_intro_tel, sizeof(p[no-1].u3_intro_tel));
					//�_���Ǝ�
					csvsplit_n(mCsvLine, 38, p[no-1].u3_checker, sizeof(p[no-1].u3_checker));
					//�A����
					csvsplit_n(mCsvLine, 39, p[no-1].u3_chk_tel, sizeof(p[no-1].u3_chk_tel));
					//���l�P
					csvsplit_n(mCsvLine, 40, p[no-1].u3_note_1, sizeof(p[no-1].u3_note_1));
					//���l�Q
					csvsplit_n(mCsvLine, 41, p[no-1].u3_note_2, sizeof(p[no-1].u3_note_2));
					//���l�R
					csvsplit_n(mCsvLine, 42, p[no-1].u3_note_3, sizeof(p[no-1].u3_note_3));
					//���l�S
					csvsplit_n(mCsvLine, 43, p[no-1].u3_note_4, sizeof(p[no-1].u3_note_4));

					//
					//�����������u�S
					//

					//�����������u
					csvsplit_n(mCsvLine, 44, p[no-1].u4_unit, sizeof(p[no-1].u4_unit));
					//�`��
					csvsplit_n(mCsvLine, 45, p[no-1].u4_form, sizeof(p[no-1].u4_form));
					//�^��
					csvsplit_n(mCsvLine, 46, p[no-1].u4_type, sizeof(p[no-1].u4_type));
					//���t�P
					csvsplit_n(mCsvLine, 47, p[no-1].u4_date_1, sizeof(p[no-1].u4_date_1));
					//���t�Q
					csvsplit_n(mCsvLine, 48, p[no-1].u4_date_2, sizeof(p[no-1].u4_date_2));
					//���t�R
					csvsplit_n(mCsvLine, 49, p[no-1].u4_date_3, sizeof(p[no-1].u4_date_3));
					//�����Ǝ�
					csvsplit_n(mCsvLine, 50, p[no-1].u4_introduce, sizeof(p[no-1].u4_introduce));
					//�A����
					csvsplit_n(mCsvLine, 51, p[no-1].u4_intro_tel, sizeof(p[no-1].u4_intro_tel));
					//�_���Ǝ�
					csvsplit_n(mCsvLine, 52, p[no-1].u4_checker, sizeof(p[no-1].u4_checker));
					//�A����
					csvsplit_n(mCsvLine, 53, p[no-1].u4_chk_tel, sizeof(p[no-1].u4_chk_tel));
					//���l�P
					csvsplit_n(mCsvLine, 54, p[no-1].u4_note_1, sizeof(p[no-1].u4_note_1));
					//���l�Q
					csvsplit_n(mCsvLine, 55, p[no-1].u4_note_2, sizeof(p[no-1].u4_note_2));
					//���l�R
					csvsplit_n(mCsvLine, 56, p[no-1].u4_note_3, sizeof(p[no-1].u4_note_3));
					//���l�S
					csvsplit_n(mCsvLine, 57, p[no-1].u4_note_4, sizeof(p[no-1].u4_note_4));

					//
					//�����������u�T
					//

					//�����������u
					csvsplit_n(mCsvLine, 58, p[no-1].u5_unit, sizeof(p[no-1].u5_unit));
					//�`��
					csvsplit_n(mCsvLine, 59, p[no-1].u5_form, sizeof(p[no-1].u5_form));
					//�^��
					csvsplit_n(mCsvLine, 60, p[no-1].u5_type, sizeof(p[no-1].u5_type));
					//���t�P
					csvsplit_n(mCsvLine, 61, p[no-1].u5_date_1, sizeof(p[no-1].u5_date_1));
					//���t�Q
					csvsplit_n(mCsvLine, 62, p[no-1].u5_date_2, sizeof(p[no-1].u5_date_2));
					//���t�R
					csvsplit_n(mCsvLine, 63, p[no-1].u5_date_3, sizeof(p[no-1].u5_date_3));
					//�����Ǝ�
					csvsplit_n(mCsvLine, 64, p[no-1].u5_introduce, sizeof(p[no-1].u5_introduce));
					//�A����
					csvsplit_n(mCsvLine, 65, p[no-1].u5_intro_tel, sizeof(p[no-1].u5_intro_tel));
					//�_���Ǝ�
					csvsplit_n(mCsvLine, 66, p[no-1].u5_checker, sizeof(p[no-1].u5_checker));
					//�A����
					csvsplit_n(mCsvLine, 67, p[no-1].u5_chk_tel, sizeof(p[no-1].u5_chk_tel));
					//���l�P
					csvsplit_n(mCsvLine, 68, p[no-1].u5_note_1, sizeof(p[no-1].u5_note_1));
					//���l�Q
					csvsplit_n(mCsvLine, 69, p[no-1].u5_note_2, sizeof(p[no-1].u5_note_2));
					//���l�R
					csvsplit_n(mCsvLine, 70, p[no-1].u5_note_3, sizeof(p[no-1].u5_note_3));
					//���l�S
					csvsplit_n(mCsvLine, 71, p[no-1].u5_note_4, sizeof(p[no-1].u5_note_4));

					//
					//�����������u�U
					//

					//�����������u
					csvsplit_n(mCsvLine, 72, p[no-1].u6_unit, sizeof(p[no-1].u6_unit));
					//�`��
					csvsplit_n(mCsvLine, 73, p[no-1].u6_form, sizeof(p[no-1].u6_form));
					//�^��
					csvsplit_n(mCsvLine, 74, p[no-1].u6_type, sizeof(p[no-1].u6_type));
					//���t�P
					csvsplit_n(mCsvLine, 75, p[no-1].u6_date_1, sizeof(p[no-1].u6_date_1));
					//���t�Q
					csvsplit_n(mCsvLine, 76, p[no-1].u6_date_2, sizeof(p[no-1].u6_date_2));
					//���t�R
					csvsplit_n(mCsvLine, 77, p[no-1].u6_date_3, sizeof(p[no-1].u6_date_3));
					//�����Ǝ�
					csvsplit_n(mCsvLine, 78, p[no-1].u6_introduce, sizeof(p[no-1].u6_introduce));
					//�A����
					csvsplit_n(mCsvLine, 79, p[no-1].u6_intro_tel, sizeof(p[no-1].u6_intro_tel));
					//�_���Ǝ�
					csvsplit_n(mCsvLine, 80, p[no-1].u6_checker, sizeof(p[no-1].u6_checker));
					//�A����
					csvsplit_n(mCsvLine, 81, p[no-1].u6_chk_tel, sizeof(p[no-1].u6_chk_tel));
					//���l�P
					csvsplit_n(mCsvLine, 82, p[no-1].u6_note_1, sizeof(p[no-1].u6_note_1));
					//���l�Q
					csvsplit_n(mCsvLine, 83, p[no-1].u6_note_2, sizeof(p[no-1].u6_note_2));
					//���l�R
					csvsplit_n(mCsvLine, 84, p[no-1].u6_note_3, sizeof(p[no-1].u6_note_3));
					//���l�S
					csvsplit_n(mCsvLine, 85, p[no-1].u6_note_4, sizeof(p[no-1].u6_note_4));

					//
					//�����������u�V
					//

					//�����������u
					csvsplit_n(mCsvLine, 86, p[no-1].u7_unit, sizeof(p[no-1].u7_unit));
					//�`��
					csvsplit_n(mCsvLine, 87, p[no-1].u7_form, sizeof(p[no-1].u7_form));
					//�^��
					csvsplit_n(mCsvLine, 88, p[no-1].u7_type, sizeof(p[no-1].u7_type));
					//���t�P
					csvsplit_n(mCsvLine, 89, p[no-1].u7_date_1, sizeof(p[no-1].u7_date_1));
					//���t�Q
					csvsplit_n(mCsvLine, 90, p[no-1].u7_date_2, sizeof(p[no-1].u7_date_2));
					//���t�R
					csvsplit_n(mCsvLine, 91, p[no-1].u7_date_3, sizeof(p[no-1].u7_date_3));
					//�����Ǝ�
					csvsplit_n(mCsvLine, 92, p[no-1].u7_introduce, sizeof(p[no-1].u7_introduce));
					//�A����
					csvsplit_n(mCsvLine, 93, p[no-1].u7_intro_tel, sizeof(p[no-1].u7_intro_tel));
					//�_���Ǝ�
					csvsplit_n(mCsvLine, 94, p[no-1].u7_checker, sizeof(p[no-1].u7_checker));
					//�A����
					csvsplit_n(mCsvLine, 95, p[no-1].u7_chk_tel, sizeof(p[no-1].u7_chk_tel));
					//���l�P
					csvsplit_n(mCsvLine, 96, p[no-1].u7_note_1, sizeof(p[no-1].u7_note_1));
					//���l�Q
					csvsplit_n(mCsvLine, 97, p[no-1].u7_note_2, sizeof(p[no-1].u7_note_2));
					//���l�R
					csvsplit_n(mCsvLine, 98, p[no-1].u7_note_3, sizeof(p[no-1].u7_note_3));
					//���l�S
					csvsplit_n(mCsvLine, 99, p[no-1].u7_note_4, sizeof(p[no-1].u7_note_4));

					//
					//�����������u�W
					//

					//�����������u
					csvsplit_n(mCsvLine, 100, p[no-1].u8_unit, sizeof(p[no-1].u8_unit));
					//�`��
					csvsplit_n(mCsvLine, 101, p[no-1].u8_form, sizeof(p[no-1].u8_form));
					//�^��
					csvsplit_n(mCsvLine, 102, p[no-1].u8_type, sizeof(p[no-1].u8_type));
					//���t�P
					csvsplit_n(mCsvLine, 103, p[no-1].u8_date_1, sizeof(p[no-1].u8_date_1));
					//���t�Q
					csvsplit_n(mCsvLine, 104, p[no-1].u8_date_2, sizeof(p[no-1].u8_date_2));
					//���t�R
					csvsplit_n(mCsvLine, 105, p[no-1].u8_date_3, sizeof(p[no-1].u8_date_3));
					//�����Ǝ�
					csvsplit_n(mCsvLine, 106, p[no-1].u8_introduce, sizeof(p[no-1].u8_introduce));
					//�A����
					csvsplit_n(mCsvLine, 107, p[no-1].u8_intro_tel, sizeof(p[no-1].u8_intro_tel));
					//�_���Ǝ�
					csvsplit_n(mCsvLine, 108, p[no-1].u8_checker, sizeof(p[no-1].u8_checker));
					//�A����
					csvsplit_n(mCsvLine, 109, p[no-1].u8_chk_tel, sizeof(p[no-1].u8_chk_tel));
					//���l�P
					csvsplit_n(mCsvLine, 110, p[no-1].u8_note_1, sizeof(p[no-1].u8_note_1));
					//���l�Q
					csvsplit_n(mCsvLine, 111, p[no-1].u8_note_2, sizeof(p[no-1].u8_note_2));
					//���l�R
					csvsplit_n(mCsvLine, 112, p[no-1].u8_note_3, sizeof(p[no-1].u8_note_3));
					//���l�S
					csvsplit_n(mCsvLine, 113, p[no-1].u8_note_4, sizeof(p[no-1].u8_note_4));

					//
					//�����������u�X
					//

					//�����������u
					csvsplit_n(mCsvLine, 114, p[no-1].u9_unit, sizeof(p[no-1].u9_unit));
					//�`��
					csvsplit_n(mCsvLine, 115, p[no-1].u9_form, sizeof(p[no-1].u9_form));
					//�^��
					csvsplit_n(mCsvLine, 116, p[no-1].u9_type, sizeof(p[no-1].u9_type));
					//���t�P
					csvsplit_n(mCsvLine, 117, p[no-1].u9_date_1, sizeof(p[no-1].u9_date_1));
					//���t�Q
					csvsplit_n(mCsvLine, 118, p[no-1].u9_date_2, sizeof(p[no-1].u9_date_2));
					//���t�R
					csvsplit_n(mCsvLine, 119, p[no-1].u9_date_3, sizeof(p[no-1].u9_date_3));
					//�����Ǝ�
					csvsplit_n(mCsvLine, 120, p[no-1].u9_introduce, sizeof(p[no-1].u9_introduce));
					//�A����
					csvsplit_n(mCsvLine, 121, p[no-1].u9_intro_tel, sizeof(p[no-1].u9_intro_tel));
					//�_���Ǝ�
					csvsplit_n(mCsvLine, 122, p[no-1].u9_checker, sizeof(p[no-1].u9_checker));
					//�A����
					csvsplit_n(mCsvLine, 123, p[no-1].u9_chk_tel, sizeof(p[no-1].u9_chk_tel));
					//���l�P
					csvsplit_n(mCsvLine, 124, p[no-1].u9_note_1, sizeof(p[no-1].u9_note_1));
					//���l�Q
					csvsplit_n(mCsvLine, 125, p[no-1].u9_note_2, sizeof(p[no-1].u9_note_2));
					//���l�R
					csvsplit_n(mCsvLine, 126, p[no-1].u9_note_3, sizeof(p[no-1].u9_note_3));
					//���l�S
					csvsplit_n(mCsvLine, 127, p[no-1].u9_note_4, sizeof(p[no-1].u9_note_4));

					//
					//�����������u�P�O
					//

					//�����������u
					csvsplit_n(mCsvLine, 128, p[no-1].u10_unit, sizeof(p[no-1].u10_unit));
					//�`��
					csvsplit_n(mCsvLine, 129, p[no-1].u10_form, sizeof(p[no-1].u10_form));
					//�^��
					csvsplit_n(mCsvLine, 130, p[no-1].u10_type, sizeof(p[no-1].u10_type));
					//���t�P
					csvsplit_n(mCsvLine, 131, p[no-1].u10_date_1, sizeof(p[no-1].u10_date_1));
					//���t�Q
					csvsplit_n(mCsvLine, 132, p[no-1].u10_date_2, sizeof(p[no-1].u10_date_2));
					//���t�R
					csvsplit_n(mCsvLine, 133, p[no-1].u10_date_3, sizeof(p[no-1].u10_date_3));
					//�����Ǝ�
					csvsplit_n(mCsvLine, 134, p[no-1].u10_introduce, sizeof(p[no-1].u10_introduce));
					//�A����
					csvsplit_n(mCsvLine, 135, p[no-1].u10_intro_tel, sizeof(p[no-1].u10_intro_tel));
					//�_���Ǝ�
					csvsplit_n(mCsvLine, 136, p[no-1].u10_checker, sizeof(p[no-1].u10_checker));
					//�A����
					csvsplit_n(mCsvLine, 137, p[no-1].u10_chk_tel, sizeof(p[no-1].u10_chk_tel));
					//���l�P
					csvsplit_n(mCsvLine, 138, p[no-1].u10_note_1, sizeof(p[no-1].u10_note_1));
					//���l�Q
					csvsplit_n(mCsvLine, 139, p[no-1].u10_note_2, sizeof(p[no-1].u10_note_2));
					//���l�R
					csvsplit_n(mCsvLine, 140, p[no-1].u10_note_3, sizeof(p[no-1].u10_note_3));
					//���l�S
					csvsplit_n(mCsvLine, 141, p[no-1].u10_note_4, sizeof(p[no-1].u10_note_4));

					//
					//�L���P
					//

					//�L���N����
					csvsplit_n(mCsvLine, 142, p[no-1].n1_notice_date, sizeof(p[no-1].n1_notice_date));
					//�L�����e
					csvsplit_n(mCsvLine, 143, p[no-1].n1_contents, sizeof(p[no-1].n1_contents));
					//�L���Ή��Ǝ�
					csvsplit_n(mCsvLine, 144, p[no-1].n1_trader, sizeof(p[no-1].n1_trader));
					//�L�����l
					csvsplit_n(mCsvLine, 145, p[no-1].n1_note, sizeof(p[no-1].n1_note));
					//�\��
					csvsplit_n(mCsvLine, 146, p[no-1].n1_reserve1, sizeof(p[no-1].n1_reserve1));
					//�\��
					csvsplit_n(mCsvLine, 147, p[no-1].n1_reserve2, sizeof(p[no-1].n1_reserve2));

					//
					//�L���Q
					//

					//�L���N����
					csvsplit_n(mCsvLine, 148, p[no-1].n2_notice_date, sizeof(p[no-1].n2_notice_date));
					//�L�����e
					csvsplit_n(mCsvLine, 149, p[no-1].n2_contents, sizeof(p[no-1].n2_contents));
					//�L���Ή��Ǝ�
					csvsplit_n(mCsvLine, 150, p[no-1].n2_trader, sizeof(p[no-1].n2_trader));
					//�L�����l
					csvsplit_n(mCsvLine, 151, p[no-1].n2_note, sizeof(p[no-1].n2_note));
					//�\��
					csvsplit_n(mCsvLine, 152, p[no-1].n2_reserve1, sizeof(p[no-1].n2_reserve1));
					//�\��
					csvsplit_n(mCsvLine, 153, p[no-1].n2_reserve2, sizeof(p[no-1].n2_reserve2));

					//
					//�L���R
					//

					//�L���N����
					csvsplit_n(mCsvLine, 154, p[no-1].n3_notice_date, sizeof(p[no-1].n3_notice_date));
					//�L�����e
					csvsplit_n(mCsvLine, 155, p[no-1].n3_contents, sizeof(p[no-1].n3_contents));
					//�L���Ή��Ǝ�
					csvsplit_n(mCsvLine, 156, p[no-1].n3_trader, sizeof(p[no-1].n3_trader));
					//�L�����l
					csvsplit_n(mCsvLine, 157, p[no-1].n3_note, sizeof(p[no-1].n3_note));
					//�\��
					csvsplit_n(mCsvLine, 158, p[no-1].n3_reserve1, sizeof(p[no-1].n3_reserve1));
					//�\��
					csvsplit_n(mCsvLine, 159, p[no-1].n3_reserve2, sizeof(p[no-1].n3_reserve2));

					//
					//�L���S
					//

					//�L���N����
					csvsplit_n(mCsvLine, 160, p[no-1].n4_notice_date, sizeof(p[no-1].n4_notice_date));
					//�L�����e
					csvsplit_n(mCsvLine, 161, p[no-1].n4_contents, sizeof(p[no-1].n4_contents));
					//�L���Ή��Ǝ�
					csvsplit_n(mCsvLine, 162, p[no-1].n4_trader, sizeof(p[no-1].n4_trader));
					//�L�����l
					csvsplit_n(mCsvLine, 163, p[no-1].n4_note, sizeof(p[no-1].n4_note));
					//�\��
					csvsplit_n(mCsvLine, 164, p[no-1].n4_reserve1, sizeof(p[no-1].n4_reserve1));
					//�\��
					csvsplit_n(mCsvLine, 165, p[no-1].n4_reserve2, sizeof(p[no-1].n4_reserve2));

					//
					//�L���T
					//

					//�L���N����
					csvsplit_n(mCsvLine, 166, p[no-1].n5_notice_date, sizeof(p[no-1].n5_notice_date));
					//�L�����e
					csvsplit_n(mCsvLine, 167, p[no-1].n5_contents, sizeof(p[no-1].n5_contents));
					//�L���Ή��Ǝ�
					csvsplit_n(mCsvLine, 168, p[no-1].n5_trader, sizeof(p[no-1].n5_trader));
					//�L�����l
					csvsplit_n(mCsvLine, 169, p[no-1].n5_note, sizeof(p[no-1].n5_note));
					//�\��
					csvsplit_n(mCsvLine, 170, p[no-1].n5_reserve1, sizeof(p[no-1].n5_reserve1));
					//�\��
					csvsplit_n(mCsvLine, 171, p[no-1].n5_reserve2, sizeof(p[no-1].n5_reserve2));

					//
					//�L���U
					//

					//�L���N����
					csvsplit_n(mCsvLine, 172, p[no-1].n6_notice_date, sizeof(p[no-1].n6_notice_date));
					//�L�����e
					csvsplit_n(mCsvLine, 173, p[no-1].n6_contents, sizeof(p[no-1].n6_contents));
					//�L���Ή��Ǝ�
					csvsplit_n(mCsvLine, 174, p[no-1].n6_trader, sizeof(p[no-1].n6_trader));
					//�L�����l
					csvsplit_n(mCsvLine, 175, p[no-1].n6_note, sizeof(p[no-1].n6_note));
					//�\��
					csvsplit_n(mCsvLine, 176, p[no-1].n6_reserve1, sizeof(p[no-1].n6_reserve1));
					//�\��
					csvsplit_n(mCsvLine, 177, p[no-1].n6_reserve2, sizeof(p[no-1].n6_reserve2));

					//
					//�L���V
					//

					//�L���N����
					csvsplit_n(mCsvLine, 178, p[no-1].n7_notice_date, sizeof(p[no-1].n7_notice_date));
					//�L�����e
					csvsplit_n(mCsvLine, 179, p[no-1].n7_contents, sizeof(p[no-1].n7_contents));
					//�L���Ή��Ǝ�
					csvsplit_n(mCsvLine, 180, p[no-1].n7_trader, sizeof(p[no-1].n7_trader));
					//�L�����l
					csvsplit_n(mCsvLine, 181, p[no-1].n7_note, sizeof(p[no-1].n7_note));
					//�\��
					csvsplit_n(mCsvLine, 182, p[no-1].n7_reserve1, sizeof(p[no-1].n7_reserve1));
					//�\��
					csvsplit_n(mCsvLine, 183, p[no-1].n7_reserve2, sizeof(p[no-1].n7_reserve2));

					//
					//�L���W
					//

					//�L���N����
					csvsplit_n(mCsvLine, 184, p[no-1].n8_notice_date, sizeof(p[no-1].n8_notice_date));
					//�L�����e
					csvsplit_n(mCsvLine, 185, p[no-1].n8_contents, sizeof(p[no-1].n8_contents));
					//�L���Ή��Ǝ�
					csvsplit_n(mCsvLine, 186, p[no-1].n8_trader, sizeof(p[no-1].n8_trader));
					//�L�����l
					csvsplit_n(mCsvLine, 187, p[no-1].n8_note, sizeof(p[no-1].n8_note));
					//�\��
					csvsplit_n(mCsvLine, 188, p[no-1].n8_reserve1, sizeof(p[no-1].n8_reserve1));
					//�\��
					csvsplit_n(mCsvLine, 189, p[no-1].n8_reserve2, sizeof(p[no-1].n8_reserve2));

					//
					//�L���X
					//

					//�L���N����
					csvsplit_n(mCsvLine, 190, p[no-1].n9_notice_date, sizeof(p[no-1].n9_notice_date));
					//�L�����e
					csvsplit_n(mCsvLine, 191, p[no-1].n9_contents, sizeof(p[no-1].n9_contents));
					//�L���Ή��Ǝ�
					csvsplit_n(mCsvLine, 192, p[no-1].n9_trader, sizeof(p[no-1].n9_trader));
					//�L�����l
					csvsplit_n(mCsvLine, 193, p[no-1].n9_note, sizeof(p[no-1].n9_note));
					//�\��
					csvsplit_n(mCsvLine, 194, p[no-1].n9_reserve1, sizeof(p[no-1].n9_reserve1));
					//�\��
					csvsplit_n(mCsvLine, 195, p[no-1].n9_reserve2, sizeof(p[no-1].n9_reserve2));

					//
					//�L���P�O
					//

					//�L���N����
					csvsplit_n(mCsvLine, 196, p[no-1].n10_notice_date, sizeof(p[no-1].n10_notice_date));
					//�L�����e
					csvsplit_n(mCsvLine, 197, p[no-1].n10_contents, sizeof(p[no-1].n10_contents));
					//�L���Ή��Ǝ�
					csvsplit_n(mCsvLine, 198, p[no-1].n10_trader, sizeof(p[no-1].n10_trader));
					//�L�����l
					csvsplit_n(mCsvLine, 199, p[no-1].n10_note, sizeof(p[no-1].n10_note));
					//�\��
					csvsplit_n(mCsvLine, 200, p[no-1].n10_reserve1, sizeof(p[no-1].n10_reserve1));
					//�\��
					csvsplit_n(mCsvLine, 201, p[no-1].n10_reserve2, sizeof(p[no-1].n10_reserve2));
				}
			}
			fclose(fp);
		}
		//���̃|�C���^�ֈړ�
		p = p + kp[i].max;
	}
	return SYS_ERR_SUCCESS;
}
#endif

/*---------------------------------------------------------
*	�֐��FcreateSysConstKindInfo
*	�@�\�F�ǎ�ʊǗ���񋤗L�������쐬
*	�����Fpt                [OUT] �ǎ�ʊǗ���񋤗L������
*	�ߒl�F
*	�����F
---------------------------------------------------------*/
int createSysConstKindInfo(sharedMem_t *pt)
{
	sys_mynumber_t   myNumber;
	ini_server_t     srvInfo;
	sys_kind_t       *sysKind;
	kind_sharedmem_t *mp;
	int  kmax;
	int  i;
	int  ret;

	long msize     = 0;
	long s_head    = 0;
	long s_info    = 0;
	long s_kind    = 0;
	long s_local   = 0;
	long s_action  = 0;
	long s_name    = 0;
	long s_optname = 0;
	long s_code    = 0;
//	long s_comp1   = 0;
//	long s_comp2   = 0;


	//�p�����[�^�̊m�F
	if (pt == NULL)
		return SYS_ERR_POINTER;

	//�ϐ��̏�����
	sysKind = new sys_kind_t[SYS_MAX_KIND];
	if (sysKind == NULL)
		return SYS_ERR_ALLOC_MEMORY;
	memset(sysKind,   0, sizeof(sys_kind_t) * SYS_MAX_KIND);
	memset(&myNumber, 0, sizeof(myNumber));
	memset(&srvInfo,  0, sizeof(srvInfo));

	//�����u�ԍ��̓ǂݍ���
	ret = getMyNumber(&myNumber);
	if (ret != SYS_ERR_SUCCESS) {
		free(sysKind);
		return ret;
	}

	//�T�[�o�������t�@�C���̓ǂݍ���
	ret = getServerIniFile(myNumber.intOfficeID, myNumber.intUnitID, &srvInfo);
	if (ret != SYS_ERR_SUCCESS) {
		free(sysKind);
		return ret;
	}

	//�ǎ�ʏ��̓ǂݍ���
	ret = getSysConstKind(srvInfo.strAppPath, sysKind);
	if (ret != SYS_ERR_SUCCESS) {
		free(sysKind);
		return ret;
	}

	//�ǎ�ʏ�񂩂烁�����T�C�Y�̌v�Z
	s_head = sizeof(kind_sharedmem_t);
	s_kind = sizeof(sys_kind_t) * SYS_MAX_KIND;
	for (i = 0, kmax = 0; i < SYS_MAX_KIND; i++) {
		if (sysKind[i].max <= 0) continue;
		s_local   += sizeof(sys_local_t  ) * sysKind[i].max;
		s_action  += sizeof(sys_action_t ) * sysKind[i].max;
		s_name    += sizeof(sys_name_t   ) * sysKind[i].max;
		s_optname += sizeof(sys_optname_t) * sysKind[i].max;
		s_code    += sizeof(sys_code_t   ) * sysKind[i].max;
//		s_comp1   += sizeof(sys_comp1_t  ) * sysKind[i].max;
//		s_comp2   += sizeof(sys_comp2_t  ) * sysKind[i].max;
		//�ő�ǎ�ʔԍ�
		kmax = i+1;
	}
//	msize = s_kind + s_local + s_action + s_name + s_optname + s_code + s_comp1 + s_comp2;
	msize = s_kind + s_local + s_action + s_name + s_optname + s_code;

	//���L�������̊m��
	ret = dll_createSharedMemory(pt, SYS_NAME_KIND, s_head + msize);
	if (ret < 0) {
		free(sysKind);
		return SYS_ERR_SHARED_MEMORY;
	}

	//�Ǘ����̐ݒ�
	memset(pt->mem, 0, s_head + msize);
	mp = (kind_sharedmem_t *)pt->mem;
	strcpy(mp->app, srvInfo.strDataPath);
	mp->officeID = myNumber.intOfficeID;
	mp->unitID   = myNumber.intUnitID;
	mp->max      = kmax;
	mp->msize    = msize;

	//�擪����̑��Έʒu�̐ݒ�
	mp->idx_kind    = s_head;
	mp->idx_local   = s_head + s_kind;
	mp->idx_action  = s_head + s_kind + s_local;
	mp->idx_name    = s_head + s_kind + s_local + s_action;
	mp->idx_optname = s_head + s_kind + s_local + s_action + s_name;
	mp->idx_code    = s_head + s_kind + s_local + s_action + s_name + s_optname;
//	mp->idx_comp1   = s_head + s_kind + s_local + s_action + s_name + s_optname + s_code;
//	mp->idx_comp2   = s_head + s_kind + s_local + s_action + s_name + s_optname + s_code + s_comp1;

	//�ǎ�ʏ��̃R�s�[
	memcpy(pt->mem + mp->idx_kind, sysKind, s_kind);
	free(sysKind);

	return SYS_ERR_SUCCESS;
}


/*---------------------------------------------------------
*	�֐��FreadSysConstKindInfo
*	�@�\�F�ǎ�ʏ��ǂݍ���
*	�����Fpt                [I/O] �ǎ�ʊǗ���񋤗L������
*	�ߒl�F
*	�����F
---------------------------------------------------------*/
int readSysConstKindInfo(sharedMem_t *pt)
{
	kind_sharedmem_t *mp = (kind_sharedmem_t *)pt->mem;

	if (pt == NULL)
		return SYS_ERR_POINTER;
	if (pt->mem == NULL)
		return SYS_ERR_POINTER;

//2002.11.19 �폜�i��������j----------------------------------------
//�t�@�C���ǂݍ��ݎ��ɍX�V�������r����悤�ɕύX������
//���̃N���A�͊e���ǂݍ��ݎ��ɍs�����ƂƂ���
//	//���̃N���A
//	memset(pt->mem + mp->idx_local, 0, mp->idx_comp - mp->idx_local);
//2002.11.19 �폜�i�����܂Łj----------------------------------------

	//�ǎ������̓ǂݍ���
	getSysConstLocal  (mp, mp->app, mp->officeID, mp->unitID, (sys_kind_t *)(pt->mem + mp->idx_kind), (sys_local_t *)(pt->mem + mp->idx_local));
	//�ǉғ����̓ǂݍ���
	getSysConstAction (mp, mp->app, (sys_kind_t *)(pt->mem + mp->idx_kind), (sys_action_t  *)(pt->mem + mp->idx_action ));
	//�ǖ��̏��̓ǂݍ���
	getSysConstName   (mp, mp->app, (sys_kind_t *)(pt->mem + mp->idx_kind), (sys_name_t    *)(pt->mem + mp->idx_name   ));
	//�Ǖ⑫���̏��̓ǂݍ���
	getSysConstOptName(mp, mp->app, (sys_kind_t *)(pt->mem + mp->idx_kind), (sys_optname_t *)(pt->mem + mp->idx_optname));
	//�ǃR�[�h���̓ǂݍ���
	getSysConstCode   (mp, mp->app, (sys_kind_t *)(pt->mem + mp->idx_kind), (sys_code_t    *)(pt->mem + mp->idx_code   ));
//	//�Ǎ\�����P�̓ǂݍ���
//	getSysConstComp1  (mp, mp->app, (sys_kind_t *)(pt->mem + mp->idx_kind), (sys_comp1_t   *)(pt->mem + mp->idx_comp1  ));
//	//�Ǎ\�����Q�̓ǂݍ���
//	getSysConstComp2  (mp, mp->app, (sys_kind_t *)(pt->mem + mp->idx_kind), (sys_comp2_t   *)(pt->mem + mp->idx_comp2  ));

//2002.11.19 �폜�i��������j----------------------------------------
//�t�@�C���ǂݍ��ݎ��ɍX�V�������r����悤�ɕύX������
//	//�ǂݍ��ݓ����̐ݒ�
//	dll_getNowTime(&mp->tim);
//2002.11.19 �폜�i�����܂Łj----------------------------------------

	return SYS_ERR_SUCCESS;
}

/*---------------------------------------------------------
*	�֐��FgetSysConstKindInfo
*	�@�\�F�ǎ�ʖ����̗̈揉����
*	�����Fpt                [IN]  �ǎ�ʊǗ����
*	�ߒl�F����
        �F     0 = ����
        �F  �ȊO = ���s
*	�����F
---------------------------------------------------------*/
int getSysConstKindInfo(kind_manager_t *pt)
{
	kind_sharedmem_t *mp;
	sys_kind_t  *kp;
	kind_info_t wp;
	HANDLE      hLock;
	sharedMem_t p;
	BOOL bln;
	char *dp;
	long psize;
	int ret;
	int i;


	if (pt == NULL)
		return SYS_ERR_POINTER;

	memset(pt, 0, sizeof(kind_manager_t));

	//���b�N�n���h���̎擾
	ret = dll_createLock(NULL, SYS_NAME_LOCK, &hLock);
	if (ret < 0)
		return SYS_ERR_LOCK_HANDLE;
	else
		pt->hLock = hLock;

	//�Ǘ��̈�̃��b�N
	bln = dll_setLock(pt->hLock, SYS_LOCK_TIMEOUT);
	if (bln == FALSE) {
		freeSysConstKindInfo(pt);
		return SYS_ERR_LOCK_TIMEOUT;
	}

	//���L�������̃I�[�v��
	ret = dll_openSharedMemory(&p, SYS_NAME_KIND);
	if (ret < 0)
		ret = createSysConstKindInfo(&p);

	//���b�N�̉���
	dll_releaseLock(pt->hLock);

	if (p.hd != NULL) {
		pt->p.hd  = p.hd;
		pt->p.mem = p.mem;

		//�������̊m��
		mp = (kind_sharedmem_t *)p.mem;
		psize = sizeof(kind_info_t) * SYS_MAX_KIND;
		dp = new char[psize + mp->msize];
		if (dp == NULL) {
			ret = SYS_ERR_ALLOC_MEMORY;
		} else {
			//�Ǘ����̐ݒ�
			pt->max   = mp->max;
			pt->pKind = (kind_info_t *)dp;
			//�|�C���^���̐ݒ�
			memset(dp, 0, psize + mp->msize);
			wp.kind    = (sys_kind_t    *)(dp + psize + mp->idx_kind    - sizeof(kind_sharedmem_t));
			wp.local   = (sys_local_t   *)(dp + psize + mp->idx_local   - sizeof(kind_sharedmem_t));
			wp.action  = (sys_action_t  *)(dp + psize + mp->idx_action  - sizeof(kind_sharedmem_t));
			wp.name    = (sys_name_t    *)(dp + psize + mp->idx_name    - sizeof(kind_sharedmem_t));
			wp.optname = (sys_optname_t *)(dp + psize + mp->idx_optname - sizeof(kind_sharedmem_t));
			wp.code    = (sys_code_t    *)(dp + psize + mp->idx_code    - sizeof(kind_sharedmem_t));
//			wp.comp1   = (sys_comp1_t   *)(dp + psize + mp->idx_comp1   - sizeof(kind_sharedmem_t));
//			wp.comp2   = (sys_comp2_t   *)(dp + psize + mp->idx_comp2   - sizeof(kind_sharedmem_t));
			kp = (sys_kind_t *)(pt->p.mem + mp->idx_kind);
			for (i = 0; i < SYS_MAX_KIND; i++) {
				if (kp[i].max <= 0) continue;
				pt->pKind[i].kind    = &wp.kind[i];
				pt->pKind[i].local   = wp.local;
				pt->pKind[i].action  = wp.action;
				pt->pKind[i].name    = wp.name;
				pt->pKind[i].optname = wp.optname;
				pt->pKind[i].code    = wp.code;
//				pt->pKind[i].comp1   = wp.comp1;
//				pt->pKind[i].comp2   = wp.comp2;
				wp.local   = wp.local   + kp[i].max;
				wp.action  = wp.action  + kp[i].max;
				wp.name    = wp.name    + kp[i].max;
				wp.optname = wp.optname + kp[i].max;
				wp.code    = wp.code    + kp[i].max;
//				wp.comp1   = wp.comp1   + kp[i].max;
//				wp.comp2   = wp.comp2   + kp[i].max;
			}
		}
	}

	//�ǎ�ʏ��̓ǂݍ���
	if (ret == SYS_ERR_SUCCESS)
		ret = setSysConstKindInfo(pt);

	if (ret != SYS_ERR_SUCCESS)
		freeSysConstKindInfo(pt);

	return ret;
}

/*---------------------------------------------------------
*	�֐��FsetSysConstKindInfo
*	�@�\�F�ǎ�ʖ����̎擾
*	�����Fpt                [OUT] �ǎ�ʊǗ����
*	�ߒl�F����
        �F     0 = ����
        �F  �ȊO = ���s
*	�����F
---------------------------------------------------------*/
int setSysConstKindInfo(kind_manager_t *pt)
{
	kind_sharedmem_t *mp;

//2002.11.19 �폜�i��������j----------------------------------------
//�t�@�C����ǂݍ��ގ��ɍX�V�������r����悤�ɕύX
//	tm24_t ntm24, rtm24;
//	time_t ntim;
//2002.11.19 �폜�i�����܂Łj----------------------------------------

	BOOL bln;
	int ret;


	//�Ǘ��̈�̃��b�N
	bln = dll_setLock(pt->hLock, SYS_LOCK_TIMEOUT);
	if (bln == FALSE) {
		return SYS_ERR_LOCK_TIMEOUT;
	}

//2002.11.19 �C���i��������j----------------------------------------
//�t�@�C����ǂݍ��ގ��ɍX�V�������r����悤�ɕύX
//
//	//���L�������̓ǂݍ��ݓ����m�F
//	mp = (kind_sharedmem_t *)pt->p.mem;
//	dll_getNowTime(&ntim);
//	dll_localtime24(ntim,    &ntm24);
//	dll_localtime24(mp->tim, &rtm24);
//	if (mp->tim == 0 || ntm24.tm_mday != rtm24.tm_mday) {
//		ret = readSysConstKindInfo(&pt->p);
//	} else {
//		ret = SYS_ERR_SUCCESS;
//	}
//
//	//�����̓ǂݍ��ݎ����Ƌ��L�������̓ǂݍ��ݎ����̔�r
//	dll_localtime24(pt->tim, &ntm24);
//	dll_localtime24(mp->tim, &rtm24);
//	if (pt->tim == 0 || ntm24.tm_mday != rtm24.tm_mday) {
//		memcpy((char *)pt->pKind + sizeof(kind_info_t) * SYS_MAX_KIND, pt->p.mem + mp->idx_kind, mp->msize);
//		pt->tim = mp->tim;
//	}

	//�t�@�C���̓ǂݍ���
	ret = readSysConstKindInfo(&pt->p);
	if (ret == SYS_ERR_SUCCESS) {
		//�����̗̈�ɃR�s�[
		mp = (kind_sharedmem_t *)pt->p.mem;
		memcpy((char *)pt->pKind + sizeof(kind_info_t) * SYS_MAX_KIND, pt->p.mem + mp->idx_kind, mp->msize);
	}
//2002.11.19 �C���i�����܂Łj----------------------------------------

	//���b�N�̉���
	dll_releaseLock(pt->hLock);

	return ret;
}

/*---------------------------------------------------------
*	�֐��FfreeSysConstKindInfo
*	�@�\�F�ǎ�ʖ����̗̈�J��
*	�����Fpt                [IN]  �ǎ�ʊǗ����
*	�ߒl�F
*	�����F
---------------------------------------------------------*/
void freeSysConstKindInfo(kind_manager_t *pt)
{
	if (pt->p.hd  != NULL) dll_freeSharedMemory(&pt->p);
	if (pt->pKind != NULL) free(pt->pKind);
	if (pt->hLock != NULL) dll_closeLock(pt->hLock);
	memset(pt, 0, sizeof(kind_manager_t));
}

/*---------------------------------------------------------
*	�֐��FcheckSysConstKindInfo
*	�@�\�F�V�X�e���萔�����X�V�`�F�b�N
*	�����Fpt                [IN]  �ǎ�ʊǗ����
        �Fdpath             [IN]  �f�[�^�p�X
        �Fpno               [IN]  �\�t�g�ԍ�
*	�ߒl�F�X�V�t���O
        �F  TRUE  = �X�V
        �F  FALSE = ���X�V
*	�����F
---------------------------------------------------------*/
BOOL checkSysConstKindInfo(kind_manager_t *pt, char *dpath, int pno)
{
	char fpath[_MAX_PATH];
	struct _stat st;
	BOOL bln;
	int ret;

	//�ʒm�t�@�C�����̐ݒ�
	sprintf(fpath, "%sprogram\\notice_%02d.txt", dpath, pno);
	//�ʒm�t�@�C���̑��݂��m�F
	ret = _stat(fpath, &st);
	if (ret == 0) {
		bln = TRUE;
	} else {
		bln = FALSE;
	}

	//�ʒm�t�@�C������
	if (bln == TRUE) {
		//�ēǍ���
		setSysConstKindInfo(pt);
		//�ʒm�t�@�C���̍폜
		remove(fpath);
	}
	return bln;
}

/*---------------------------------------------------------
*	�֐��FgetSysConstExio
*	�@�\�F���u�ԒʐM���ǂݍ���
*	�����Fapp               [IN]  �A�v���P�[�V�����p�X
        �Fsyskind           [IN]  �������
        �FofficeNo          [IN]  �������ԍ�
        �FunitNo            [IN]  ���u�ԍ�
        �FhouroNo           [IN]  ���H�ԍ�
        �Fp                 [OUT] ���u�ԒʐM���
*	�ߒl�F����
        �F     0 = ����
        �F  �ȊO = ���s
*	�����F
---------------------------------------------------------*/
int getSysConstExio(char *app, char *syskind, int officeNo, int unitNo, int houroNo, sys_exioinfo_t *p)
{
	FILE *fp;
	fpos_t pos;
	char fpath[_MAX_PATH];
	char dt[128];
	char wk[2048];
	int no;
	int cnt;
	int i;


	//�߂�l�̏�����
	p->cnt = 0;
	p->dp = NULL;

 	//�t�@�C���I�[�v��
	sprintf(fpath, "%ssys_table\\csv\\%s_exio_%02d%02d%02d.csv", app, syskind, officeNo, unitNo, houroNo);
	fp = fopen(fpath, "r");
	if (fp == NULL) {
		return -1;
	}
	//�����̎擾
	for (cnt = 0; !feof(fp); cnt++)
		csvread_n(fp, wk, sizeof(wk));
	cnt = cnt - 7;
	//�擪�ɖ߂�
	pos = 0;
	fsetpos(fp, &pos);

	//�̈�̎擾
	p->dp = new sys_exio_t[cnt];
	if (p->dp == NULL) {
		fclose(fp);
		return -1;
	}

	//�P�s��
	csvread_n(fp, wk, sizeof(wk));
	//�������ԍ�
	csvsplit_n(wk, 3, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.officeNo = no;
	//��MPORT
	csvsplit_n(wk, 5, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.r_port = no;
	//�P���l�o��
	csvsplit_n(wk, 10, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.ioflg_1m = no;

	//�Q�s��
	csvread_n(fp, wk, sizeof(wk));
	//���u�ԍ�
	csvsplit_n(wk, 3, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.unitNo = no;
	//���M��PORT
	csvsplit_n(wk, 5, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.s_port = no;
	//���O�^�C���P��START
	csvsplit_n(wk, 7, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.rag_1m.st = no;
	//���O�^�C���P��END
	csvsplit_n(wk, 8, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.rag_1m.et = no;
	//�莞�o��
	csvsplit_n(wk, 10, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.ioflg_hm = no;

	//�R�s��
	csvread_n(fp, wk, sizeof(wk));
	//���H�ԍ�
	csvsplit_n(wk, 3, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.houroNo = no;
	//���M��h�o
	csvsplit_n(wk, 5, dt, sizeof(dt));
	strcpy(p->hd.s_ip, dt);
	//���O�^�C�����莞START
	csvsplit_n(wk, 7, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.rag_hm.st = no;
	//���O�^�C�����莞END
	csvsplit_n(wk, 8, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.rag_hm.et = no;
	//���W�v�o��
	csvsplit_n(wk, 10, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.ioflg_dy = no;

	//�S�s��
	csvread_n(fp, wk, sizeof(wk));
	//��������
	csvsplit_n(wk, 3, dt, sizeof(dt));
	strcpy(p->hd.myOfficeName, dt);
	//���M�掖������
	csvsplit_n(wk, 5, dt, sizeof(dt));
	strcpy(p->hd.toOfficeName, dt);
	//���O�^�C�����W�vSTART
	csvsplit_n(wk, 7, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.rag_dy.st = no;
	//���O�^�C�����W�vEND
	csvsplit_n(wk, 8, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.rag_dy.et = no;

	//�T�s��
	csvread_n(fp, wk, sizeof(wk));
	//���u��
	csvsplit_n(wk, 3, dt, sizeof(dt));
	strcpy(p->hd.myUnitName, dt);
	//���M�摕�u��
	csvsplit_n(wk, 5, dt, sizeof(dt));
	strcpy(p->hd.toUnitName, dt);

	//�U�s��
	csvread_n(fp, wk, sizeof(wk));
	//�V�s��
	csvread_n(fp, wk, sizeof(wk));

	//�W�s�ڈȍ~
	i = 0;
	csvread_n(fp, wk, sizeof(wk));
	while ( !feof(fp) ) {
		//���o�͎�ʃR�[�h
		csvsplit_n(wk, 0, dt, sizeof(dt));
		no = atoi(dt);
		if (no == SYS_EXIO_KIND_IN || no == SYS_EXIO_KIND_OUT) {
			p->dp[i].io_kind = no;
			//���o�͋ǔ�
			csvsplit_n(wk, 1, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].io_sno = no;
			//���o�͍���
			csvsplit_n(wk, 2, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].io_ino = no;
			//���o�͊Ǘ��������ԍ�
			csvsplit_n(wk, 3, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].io_office = no;
			//���o�̓f�[�^��ʔԍ�
			csvsplit_n(wk, 4, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].io_data = no;
			//�{���R�[�h
			csvsplit_n(wk, 5, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].code = no;
			//�t�@�C�����
			csvsplit_n(wk, 6, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].filetype = no;
			//�ǎ�ʃR�[�h
			csvsplit_n(wk, 7, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].kno = no;
			//�����ǔ�
			csvsplit_n(wk, 8, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].sno = no;
			//�f�[�^���ڏ���
			csvsplit_n(wk, 9, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].ino = no;
			//�X�P�[���␳�R�[�h
			csvsplit_n(wk, 10, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].scale = no;
			//�\��
			csvsplit_n(wk, 11, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].reserve1 = no;
			//�\��
			csvsplit_n(wk, 12, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].reserve2 = no;
			i++;
		}
		//���̍s
		csvread_n(fp, wk, sizeof(wk));
	}
	//�t�@�C���N���[�Y
	fclose(fp);

	p->cnt = i;
	return 0;
}

