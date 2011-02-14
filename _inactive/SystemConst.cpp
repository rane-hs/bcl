#include "stdafx.h"
#include "systemConst.h"

/*-------------------------------------------------------------------
  構造体：局種別共有メモリ管理情報
  項目  ：app           データパス
        ：officeID      事務所番号
        ：unitID        装置番号
        ：tim           読み込み日時
        ：max           最大局種別番号
        ：msize         局情報メモリサイズ
        ：pt            管理用ポインタ
-------------------------------------------------------------------*/
typedef struct {
	char        app[_MAX_PATH];     //データパス
	int         officeID;           //事務所番号
	int         unitID;             //装置番号
	int         max;                //最大局種別番号
	long        msize;              //局情報メモリサイズ
	time_t      tim;                //読み込み日時
	time_t      tim_local[99];      //局実装情報更新日時
	time_t      tim_action[99];     //局稼動情報更新日時
	time_t      tim_name[99];       //局名称情報更新日時
	time_t      tim_optname[99];    //局補足名称情報更新日時
	time_t      tim_code[99];       //局コード情報更新日時
//	time_t      tim_comp1[99];      //局構成情報更新日時
//	time_t      tim_comp2[99];      //局構成情報更新日時
	//先頭からの相対位置
	long        idx_kind;           //局種別情報位置
	long        idx_local;          //局実装情報位置
	long        idx_action;         //局稼動情報位置
	long        idx_name;           //局名称情報位置
	long        idx_optname;        //局補足名称情報位置
	long        idx_code;           //局コード情報位置
//	long        idx_comp1;          //局構成情報位置
//	long        idx_comp2;          //局構成情報位置
} kind_sharedmem_t;


/*-------------------------------------------------------------------
  ローカル関数宣言
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
  モジュール内変数
-------------------------------------------------------------------*/
char mCsvLine[SYS_MAX_LINE];        //CSVファイル１行読み込み用



/*---------------------------------------------------------
*	関数：inisplit
*	機能：文字列分割
*	引数：sp                [IN]  文字列
        ：p_name            [OUT] 名称
        ：p_value           [OUT] 値
*	戻値：
*	説明：”名称 = 値”の文字列を名称と値に分割します。
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
*	関数：csvtrim
*	機能：前後スペース削除
*	引数：sp                [I/O] 文字列
*	戻値：
*	説明：文字列から前スペース、後スペースを削除します。
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
*	関数：csvsplit
*	機能：CSV文字列分割
*	引数：sp                [IN]  CSV文字列(カンマ区切り)
        ：n                 [IN]  取得する位置
        ：p_value           [OUT] 取得した文字列
*	戻値：
*	説明：CSV文字列から指定された位置の文字列を取得します。
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
*	関数：csvsplit_n
*	機能：文字列分割(文字数制限あり)
*	引数：sp                [IN]  文字列
        ：n                 [IN]  取得する位置
        ：p_value           [OUT] 取得した文字列
        ：v_len             [IN]  取得領域のサイズ
*	戻値：取得文字数
*	説明：区切り文字で分割されてできた文字列で
        ：指定された位置の文字列を取得します。
-------------------------------------------------------------------*/
int csvsplit_n(char *sp, int n, char *p_value, int v_len)
{
	char *dp = sp;
	int i = 0;
	int j = 0;

	p_value[0] = '\0';
	while (*dp != '\0' && i <= n) {
		if (j >= (v_len-1)) break;	//NULLを入れるため(v_len-1)
		if (*dp == ',') i++;
		else if (i == n) p_value[j++] = *dp;
		dp++;
	}
	p_value[j] = '\0';
	csvtrim(p_value);

	return j;
}

/*---------------------------------------------------------
*	関数：csvread
*	機能：１行読み込み
*	引数：fp                [IN]  ファイル構造体
        ：dp                [OUT] 読み込んだ文字列
*	戻値：
*	説明：CSVファイルから１行読み込みます。
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
*	関数：csvread_n
*	機能：１行読み込み(文字数制限あり)
*	引数：fp                [IN]  ファイル構造体
        ：dp                [OUT] 読み込んだ文字列
        ：d_len             [IN]  取得領域サイズ
*	戻値：
*	説明：CSVファイルから１行読み込みます。
---------------------------------------------------------*/
void csvread_n(FILE *fp, char *dp, int d_len)
{
	int  i = 0;
	char c = 0;

	if (fp == NULL) return;
	if (dp == NULL) return;

	c = fgetc(fp);
	while ( !feof(fp) && c != '\n' && c != '\0') {
		if (i < (d_len-1))	//NULLを入れる為(d_len-1)
			if (c != '\n') dp[i++] = c;
		c = fgetc(fp);
	}
	dp[i] = '\0';
}

/*---------------------------------------------------------
*	関数：getMyNumber
*	機能：自装置番号の取得
*	引数：pt                [OUT] 自装置番号
*	戻値：成否
        ：     0 = 成功
        ：  以外 = 失敗
*	説明：
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
*	関数：getServerIniFile
*	機能：サーバ初期化ファイル読み込み
*	引数：officeID          [IN]  自事務所番号
        ：unitID            [IN]  自装置番号
		：pt                [OUT] サーバ情報
*	戻値：成否
        ：     0 = 成功
        ：  以外 = 失敗
*	説明：
---------------------------------------------------------*/
int getServerIniFile(int officeID, int unitID, ini_server_t *pt)
{
	char iniPath[_MAX_PATH];

	if (pt == NULL)
		return SYS_ERR_POINTER;

	sprintf(iniPath, "../common_ini/server%02d%02d.ini", officeID, unitID);
	//アプリケーションパスの読み込み
	GetPrivateProfileString("PATH", "JRC", "", pt->strAppPath, sizeof(pt->strAppPath), iniPath);
	//データパスの読み込み
	GetPrivateProfileString("PATH", "DATA", "", pt->strDataPath, sizeof(pt->strDataPath), iniPath);
	//ＭＯドライブの読み込み
	GetPrivateProfileString("PATH", "MO",  "", pt->strMO, sizeof(pt->strMO), iniPath);
	//ファイル保存期間
	pt->intDays = GetPrivateProfileInt("FILE", "DAYS", 400, iniPath);
	//ファイル保存期間（１分）
	pt->intDays1M = GetPrivateProfileInt("FILE", "DAYS1M", 45, iniPath);
	//現況ファイル作成有無
	pt->intNowFile = GetPrivateProfileInt("FILE", "NOWFILE", 0, iniPath);

//2003.02.27 追加（ここから）----------------------------------------
	//データ未受信判定期間（分）
	pt->intBeFore = GetPrivateProfileInt("FILE", "BEFORE", 60, iniPath);
	//過去データ判定期間（分）
	pt->intBeHind = GetPrivateProfileInt("FILE", "BEHIND", 60, iniPath);
//2003.02.27 追加（ここまで）----------------------------------------

	if (pt->strAppPath[0] == '\0' || pt->strDataPath[0] == '\0')
		return SYS_ERR_SERVER_INFO;

	return SYS_ERR_SUCCESS;
}

/*---------------------------------------------------------
*	関数：getSysConstKind
*	機能：局種別情報読み込み
*	引数：app               [IN]  アプリケーションパス
        ：pt                [OUT] 局種別情報
*	戻値：
*	説明：局種別情報を読み込みます。
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

	//局種別情報の読み込み
	sprintf(fpath, "%sprogram\\common_ini\\kind_info.csv", app);
	fp = fopen(fpath, "r");
	if (fp == NULL)
		return SYS_ERR_FILE_OPEN;

	//必要ない行を読み飛ばす
	csvread_n(fp, mCsvLine, sizeof(mCsvLine));
	csvread_n(fp, mCsvLine, sizeof(mCsvLine));
	csvread_n(fp, mCsvLine, sizeof(mCsvLine));
	csvread_n(fp, mCsvLine, sizeof(mCsvLine));
	csvread_n(fp, mCsvLine, sizeof(mCsvLine));
	csvread_n(fp, mCsvLine, sizeof(mCsvLine));
	csvread_n(fp, mCsvLine, sizeof(mCsvLine));

	while ( !feof(fp) ) {

		//１行読み込み
		csvread_n(fp, mCsvLine, sizeof(mCsvLine));

		//局種別番号の判定
		csvsplit_n(mCsvLine, 0, dt, sizeof(dt));
		no = atoi(dt);
		if (no >= 1 && no <= SYS_MAX_KIND) {
			//局種別番号
			pt[no-1].kno = no;

			//局種別名
			//csvsplit(mCsvLine, 1, dt);
			//strcpy(pt[no-1].name, dt);
			csvsplit_n(mCsvLine, 1, pt[no-1].name, sizeof(pt[no-1].name));

			//局種別名（和名）
			//csvsplit(mCsvLine, 2, dt);
			//strcpy(pt[no-1].jname, dt);
			csvsplit_n(mCsvLine, 2, pt[no-1].jname, sizeof(pt[no-1].jname));

			//最大局数
			csvsplit_n(mCsvLine, 3, dt, sizeof(dt));
			pt[no-1].max = atoi(dt);

			//観測項目数
			csvsplit_n(mCsvLine, 4, dt, sizeof(dt));
			pt[no-1].titem = atoi(dt);

			//日集計項目数
			csvsplit_n(mCsvLine, 5, dt, sizeof(dt));
			pt[no-1].ditem = atoi(dt);

			//演算基礎項目数
			csvsplit_n(mCsvLine, 6, dt, sizeof(dt));
			pt[no-1].bitem = atoi(dt);
		}
	}
	fclose(fp);

	return SYS_ERR_SUCCESS;
}

/*---------------------------------------------------------
*	関数：getSysConstLocal
*	機能：局実装情報読み込み
*	引数：app               [IN]  データパス
        ：officeID          [IN]  事務所番号
        ：unitID            [IN]  装置番号
        ：kp                [IN]  局種別情報
        ：pt                [OUT] 局実装情報
*	戻値：
*	説明：局実装情報ファイルを読み込みます。
        ：前提条件として、
        ：  局種別情報が設定されていること
        ：  局実装情報の領域は確保されていること
        ：ファイルが存在しない場合、初期値が設定されます。
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
//2002.11.19 追加（ここから）----------------------------------------
//ファイル読み込み時に更新時刻を比較するように変更
	struct _stat st;
	int  ret;
//2002.11.19 追加（ここまで）----------------------------------------

	if (app == NULL)
		return SYS_ERR_POINTER;
	if (kp == NULL)
		return SYS_ERR_POINTER;
	if (pt == NULL)
		return SYS_ERR_POINTER;


	//局実装情報の読み込み
	for (i = 0; i < SYS_MAX_KIND; i++) {
		if (kp[i].max <= 0) continue;

		//ファイルパスの設定
		sprintf(fpath, "%ssys_table\\csv\\%s_local_%02d%02d00.csv", app, kp[i].name, officeID, unitID);

//2002.11.19 修正（ここから）----------------------------------------
//ファイル読み込み時に更新時刻を比較するように変更
//		//ファイルオープン
//		fp = fopen(fpath, "r");

		//ファイルポインタの初期化
		fp = NULL;
		//ファイル情報の取得
		ret = _stat(fpath, &st);
		if (ret == 0) {
			//更新時刻の比較
			if (mp->tim_local[i] != st.st_mtime) {
				mp->tim_local[i] = st.st_mtime;
				//領域の初期化
				memset(p, 0, kp[i].max * sizeof(sys_local_t));
				//ファイルオープン
				fp = fopen(fpath, "r");
			}
		}
//2002.11.19 修正（ここまで）----------------------------------------

		if (fp != NULL) {
			//必要ない行を読み飛ばす
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));

			while ( !feof(fp) ) {

				//１行読み込み
				csvread_n(fp, mCsvLine, sizeof(mCsvLine));

				//内部局番の判定
				csvsplit_n(mCsvLine, 0, dt, sizeof(dt));
				no = atoi(dt);
				if (1 <= no && no <= kp[i].max) {
					//内部局番
					p[no-1].sno = no;

					//システム共通実装情報
					csvsplit_n(mCsvLine, 1, dt, sizeof(dt));
					v = atoi(dt);
					if (v == SYS_LOCAL_ENTRY_ON)
						p[no-1].entry = SYS_LOCAL_ENTRY_ON;
					else
						p[no-1].entry = 0;

					//ローカル系情報
					csvsplit_n(mCsvLine, 2, dt, sizeof(dt));
					v = atoi(dt);
					if (1 <= v && v <= 99)
						p[no-1].kei = v;
					else
						p[no-1].kei = 0;

					//ローカルデータ保存期間
					csvsplit_n(mCsvLine, 3, dt, sizeof(dt));
					v = atoi(dt);
					if (v == SYS_LOCAL_CYCLE_1M)
						p[no-1].cycle = SYS_LOCAL_CYCLE_1M;
					else
						p[no-1].cycle = 0;

					//ローカル演算実施情報
					csvsplit_n(mCsvLine, 4, dt, sizeof(dt));
					v = atoi(dt);
					if ((v == SYS_LOCAL_TCALC_IP) ||
                        (v == SYS_LOCAL_TCALC_10) ||
                        (v == SYS_LOCAL_TCALC_01))
						p[no-1].tcalc = v;
					else
						p[no-1].tcalc = 0;

					//ローカル日集計演算実施情報
					csvsplit_n(mCsvLine, 5, dt, sizeof(dt));
					v = atoi(dt);
					if (v == SYS_LOCAL_DCALC_DO)
						p[no-1].dcalc = SYS_LOCAL_DCALC_DO;
					else
						p[no-1].dcalc = 0;

					//予備項目
					csvsplit_n(mCsvLine, 6, dt, sizeof(dt));
					//予備項目
					csvsplit_n(mCsvLine, 7, dt, sizeof(dt));
				}
			}
			fclose(fp);
		}
		//次のポインタへ移動
		p = p + kp[i].max;
	}
	return SYS_ERR_SUCCESS;
}

/*---------------------------------------------------------
*	関数：getSysConstAction
*	機能：局稼動情報読み込み
*	引数：app               [IN]  データパス
        ：kp                [IN]  局種別情報
        ：pt                [OUT] 局稼動情報
*	戻値：
*	説明：局稼動情報ファイルを読み込みます。
        ：前提条件として、
        ：  局種別情報が設定されていること
        ：  局稼動情報の領域は確保されていること
        ：ファイルが存在しない場合、初期値が設定されます。
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
//2002.11.19 追加（ここから）----------------------------------------
//ファイル読み込み時に更新時刻を比較するように変更
	struct _stat st;
	int  ret;
//2002.11.19 追加（ここまで）----------------------------------------

	if (app == NULL)
		return SYS_ERR_POINTER;
	if (kp == NULL)
		return SYS_ERR_POINTER;
	if (pt == NULL)
		return SYS_ERR_POINTER;

	//局稼動情報の読み込み
	for (i = 0; i < SYS_MAX_KIND; i++) {
		if (kp[i].max <= 0) continue;

		//ファイルパスの設定
		sprintf(fpath, "%ssys_table\\csv\\%s_action.csv", app, kp[i].name);

//2002.11.19 修正（ここから）----------------------------------------
//ファイル読み込み時に更新時刻を比較するように変更
//		//ファイルオープン
//		fp = fopen(fpath, "r");

		//ファイルポインタの初期化
		fp = NULL;
		//ファイル情報の取得
		ret = _stat(fpath, &st);
		if (ret == 0) {
			//更新時刻の比較
			if (mp->tim_action[i] != st.st_mtime) {
				mp->tim_action[i] = st.st_mtime;
				//領域の初期化
				memset(p, 0, kp[i].max * sizeof(sys_action_t));
				//ファイルオープン
				fp = fopen(fpath, "r");
			}
		}
//2002.11.19 修正（ここまで）----------------------------------------

		if (fp != NULL) {
			//必要ない行を読み飛ばす
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));

			while ( !feof(fp) ) {

				//１行読み込み
				csvread_n(fp, mCsvLine, sizeof(mCsvLine));

				//内部局番の判定
				csvsplit_n(mCsvLine, 0, dt, sizeof(dt));
				no = atoi(dt);
				if (1 <= no && no <= kp[i].max) {
					//内部局番
					p[no-1].sno = no;

					//局稼動状況
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
		//次のポインタへ移動
		p = p + kp[i].max;
	}
	return SYS_ERR_SUCCESS;
}

/*---------------------------------------------------------
*	関数：getSysConstName
*	機能：局名称情報読み込み
*	引数：app               [IN]  データパス
        ：kp                [IN]  局種別情報
        ：pt                [OUT] 局名称情報
*	戻値：
*	説明：局名称情報ファイルを読み込みます。
        ：前提条件として、
        ：  局種別情報が設定されていること
        ：  局名称情報の領域は確保されていること
        ：ファイルが存在しない場合、初期値が設定されます。
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
//2002.11.19 追加（ここから）----------------------------------------
//ファイル読み込み時に更新時刻を比較するように変更
	struct _stat st;
	int ret;
//2002.11.19 追加（ここまで）----------------------------------------


	if (app == NULL)
		return SYS_ERR_POINTER;
	if (kp == NULL)
		return SYS_ERR_POINTER;
	if (pt == NULL)
		return SYS_ERR_POINTER;

	//局名称情報の読み込み
	for (i = 0; i < SYS_MAX_KIND; i++) {
		if (kp[i].max <= 0) continue;

		//ファイルパスの設定
		sprintf(fpath, "%ssys_table\\csv\\%s_name.csv", app, kp[i].name);

//2002.11.19 修正（ここから）----------------------------------------
//ファイル読み込み時に更新時刻を比較するように変更
//		//ファイルオープン
//		fp = fopen(fpath, "r");

		//ファイルポインタの初期化
		fp = NULL;
		//ファイル情報の取得
		ret = _stat(fpath, &st);
		if (ret == 0) {
			//更新時刻の比較
			if (mp->tim_name[i] != st.st_mtime) {
				mp->tim_name[i] = st.st_mtime;
				//領域の初期化
				memset(p, 0, kp[i].max * sizeof(sys_name_t));
				//ファイルオープン
				fp = fopen(fpath, "r");
			}
		}
//2002.11.19 修正（ここまで）----------------------------------------

		if (fp != NULL) {
			//必要ない行を読み飛ばす
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));

			while ( !feof(fp) ) {

				//１行読み込み
				csvread_n(fp, mCsvLine, sizeof(mCsvLine));

				//内部局番の判定
				csvsplit_n(mCsvLine, 0, dt, sizeof(dt));
				no = atoi(dt);
				if (1 <= no && no <= kp[i].max) {
					//内部局番
					p[no-1].sno = no;

					//管理番号１
					csvsplit_n(mCsvLine, 1, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= SYS_MAX_MNO)
							p[no-1].mno1 = v;
					}

					//管理番号２
					csvsplit_n(mCsvLine, 2, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= SYS_MAX_MNO)
							p[no-1].mno2 = v;
					}

					//管理番号３
					csvsplit_n(mCsvLine, 3, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= SYS_MAX_MNO)
							p[no-1].mno3 = v;
					}

					//管理番号４
					csvsplit_n(mCsvLine, 4, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= SYS_MAX_MNO)
							p[no-1].mno4 = v;
					}

					//指定観測所コード
					csvsplit_n(mCsvLine, 5, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (v == SYS_NAME_ACODE_APPOINT)
							p[no-1].acode = v;
					}

					//基準点１コード
					csvsplit_n(mCsvLine, 6, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (v == SYS_NAME_PCODE_STANDARD)
							p[no-1].pcode1 = v;
					}

					//基準点２コード
					csvsplit_n(mCsvLine, 7, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (v == SYS_NAME_PCODE_STANDARD)
							p[no-1].pcode2 = v;
					}

					//管理者種別コード
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

					//所轄事務所名　jurisdiction office code
					csvsplit_n(mCsvLine, 9, p[no-1].office_name, sizeof(p[no-1].office_name));

					//観測所名
					csvsplit_n(mCsvLine, 10, p[no-1].station_name, sizeof(p[no-1].station_name));

					//流域名
					csvsplit_n(mCsvLine, 11, p[no-1].valley_name, sizeof(p[no-1].valley_name));


					//水系名 river system
					csvsplit_n(mCsvLine, 12, p[no-1].rsys_name, sizeof(p[no-1].rsys_name));

					//河川名
					csvsplit_n(mCsvLine, 13, p[no-1].river_name, sizeof(p[no-1].river_name));

					//代表所在名 represe seat
					csvsplit_n(mCsvLine, 14, p[no-1].represe_name, sizeof(p[no-1].represe_name));

					//路線種別
					csvsplit_n(mCsvLine, 15, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (v == SYS_NAME_ROUTE_NATIONAL ||
							v == SYS_NAME_ROUTE_PREF ||
							v == SYS_NAME_ROUTE_LOCAL)
							p[no-1].route = v;
					}

					//路線名
					csvsplit_n(mCsvLine, 16, p[no-1].route_name, sizeof(p[no-1].route_name));

					//観測所名（サブ１）
					csvsplit_n(mCsvLine, 17, p[no-1].sub_name1, sizeof(p[no-1].sub_name1));

					//観測所名（サブ２）
					csvsplit_n(mCsvLine, 18, p[no-1].sub_name2, sizeof(p[no-1].sub_name2));

					//略局名
					csvsplit_n(mCsvLine, 19, p[no-1].shorts_name, sizeof(p[no-1].shorts_name));

					//砂防ブロック名
					csvsplit_n(mCsvLine, 20, p[no-1].sabou_block, sizeof(p[no-1].sabou_block));

					//都道府県名 prefecture
					csvsplit_n(mCsvLine, 21, p[no-1].pref_name, sizeof(p[no-1].pref_name));

					//群・市名
					csvsplit_n(mCsvLine, 22, p[no-1].city_name, sizeof(p[no-1].city_name));

					//区・町・村名
					csvsplit_n(mCsvLine, 23, p[no-1].area_name, sizeof(p[no-1].area_name));

					//大字・地番等名,
					csvsplit_n(mCsvLine, 24, p[no-1].addr_name, sizeof(p[no-1].addr_name));

					//観測所緯度（度）
					csvsplit_n(mCsvLine, 25, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (24 <= v && v <= 46)
							p[no-1].lat = v;
					}

					//観測所緯度（分）
					csvsplit_n(mCsvLine, 26, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (0 <= v && v <= 59)
							p[no-1].lat_min = v;
					}

					//観測所緯度（秒）
					csvsplit_n(mCsvLine, 27, dt, sizeof(dt));
					if (dt[0] != '\0') {
						f = atof(dt);
						if (0.0 <= f && f <= 59.0)
							p[no-1].lat_sec = f;
					}

					//観測所経度（度）
					csvsplit_n(mCsvLine, 28, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (123 <= v && v <= 146)
							p[no-1].lon = v;
					}

					//観測所経度（分）
					csvsplit_n(mCsvLine, 29, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (0 <= v && v <= 59)
							p[no-1].lon_min = v;
					}

					//観測所経度（秒）
					csvsplit_n(mCsvLine, 30, dt, sizeof(dt));
					if (dt[0] != '\0') {
						f = atof(dt);
						if (0.0 <= f && f <= 59.0)
							p[no-1].lon_sec = f;
					}

					//予備1
					//予備2
				}
			}
			fclose(fp);
		}
		//次のポインタへ移動
		p = p + kp[i].max;
	}
	return SYS_ERR_SUCCESS;
}

/*---------------------------------------------------------
*	関数：getSysConstOptName
*	機能：局補足名称情報読み込み
*	引数：app               [IN]  データパス
        ：kp                [IN]  局種別情報
        ：pt                [OUT] 局補足名称情報
*	戻値：
*	説明：局補足名称情報ファイルを読み込みます。
        ：前提条件として、
        ：  局種別情報が設定されていること
        ：  局補足名称情報の領域は確保されていること
        ：ファイルが存在しない場合、初期値が設定されます。
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
//2002.11.19 追加（ここから）----------------------------------------
//ファイル読み込み時に更新時刻を比較するように変更
	struct _stat st;
	int ret;
//2002.11.19 追加（ここまで）----------------------------------------


	if (app == NULL)
		return SYS_ERR_POINTER;
	if (kp == NULL)
		return SYS_ERR_POINTER;
	if (pt == NULL)
		return SYS_ERR_POINTER;


	//局補足名称情報の読み込み
	for (i = 0; i < SYS_MAX_KIND; i++) {
		if (kp[i].max <= 0) continue;

		//ファイルパスの設定
		sprintf(fpath, "%ssys_table\\csv\\%s_opt-name.csv", app, kp[i].name);

//2002.11.19 修正（ここから）----------------------------------------
//ファイル読み込み時に更新時刻を比較するように変更
//		//ファイルオープン
//		fp = fopen(fpath, "r");

		//ファイルポインタの初期化
		fp = NULL;
		//ファイル情報の取得
		ret = _stat(fpath, &st);
		if (ret == 0) {
			//更新時刻の比較
			if (mp->tim_optname[i] != st.st_mtime) {
				mp->tim_optname[i] = st.st_mtime;
				//領域の初期化
				memset(p, 0, kp[i].max * sizeof(sys_optname_t));
				//ファイルオープン
				fp = fopen(fpath, "r");
			}
		}
//2002.11.19 修正（ここまで）----------------------------------------

		if (fp != NULL) {
			//必要ない行を読み飛ばす
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));

			while ( !feof(fp) ) {

				//１行読み込み
				csvread_n(fp, mCsvLine, sizeof(mCsvLine));

				//内部局番の判定
				csvsplit_n(mCsvLine, 0, dt, sizeof(dt));
				no = atoi(dt);
				if (1 <= no && no <= kp[i].max) {
					//内部局番
					p[no-1].sno = no;

					//標高
					csvsplit_n(mCsvLine, 1, dt, sizeof(dt));
					if (dt[0] != '\0') {
						f = atof(dt);
						if (-9.99 <= f && f <= 4000.00)
							p[no-1].altitude = f;
					}

					//河口からの距離
					csvsplit_n(mCsvLine, 2, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (-10 <= v && v <= 999999)
							p[no-1].rever_dis = v;
					}

					//観測員名
					csvsplit_n(mCsvLine, 3, p[no-1].member, sizeof(p[no-1].member));

					//電話番号
					csvsplit_n(mCsvLine, 4, p[no-1].tel_no, sizeof(p[no-1].tel_no));

					//管理団体名
					csvsplit_n(mCsvLine, 5, p[no-1].ctl_name, sizeof(p[no-1].ctl_name));

					//センサー種別コード
					csvsplit_n(mCsvLine, 6, p[no-1].sensor_code, sizeof(p[no-1].sensor_code));

					//観測種別コード
					csvsplit_n(mCsvLine, 7, p[no-1].kind_code, sizeof(p[no-1].kind_code));

					//観測開始日付（年）
					csvsplit_n(mCsvLine, 8, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1800 <= v && v <= 2100)
							p[no-1].start_year = v;
					}

					//観測開始日付（月）
					csvsplit_n(mCsvLine, 9, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 12)
							p[no-1].start_mon = v;
					}

					//観測開始日付（日）
					csvsplit_n(mCsvLine, 10, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 31)
							p[no-1].start_day = v;
					}

					//監視対象区域名称（水防）
					csvsplit_n(mCsvLine, 11, p[no-1].obs_area_suibou, sizeof(p[no-1].obs_area_suibou));

					//監視対象区域名称（砂防）
					csvsplit_n(mCsvLine, 12, p[no-1].obs_area_sabo, sizeof(p[no-1].obs_area_sabo));

					//監視対象区域名称（道路）
					csvsplit_n(mCsvLine, 13, p[no-1].obs_area_road, sizeof(p[no-1].obs_area_road));

					//予備1

					//予備2
				}
			}
			fclose(fp);
		}
		//次のポインタへ移動
		p = p + kp[i].max;
	}
	return SYS_ERR_SUCCESS;
}

/*---------------------------------------------------------
*	関数：getSysConstCode
*	機能：局コード情報読み込み
*	引数：app               [IN]  データパス
        ：kp                [IN]  局種別情報
        ：pt                [OUT] 局名称情報
*	戻値：
*	説明：局コード情報ファイルを読み込みます。
        ：前提条件として、
        ：  局種別情報が設定されていること
        ：  局稼動情報の領域は確保されていること
        ：ファイルが存在しない場合、初期値が設定されます。
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
//2002.11.19 追加（ここから）----------------------------------------
//ファイル読み込み時に更新時刻を比較するように変更
	struct _stat st;
	int  ret;
//2002.11.19 追加（ここまで）----------------------------------------


	if (app == NULL)
		return SYS_ERR_POINTER;
	if (kp == NULL)
		return SYS_ERR_POINTER;
	if (pt == NULL)
		return SYS_ERR_POINTER;

	//局コード情報の読み込み
	for (i = 0; i < SYS_MAX_KIND; i++) {
		if (kp[i].max <= 0) continue;

		//ファイルパスの設定
		sprintf(fpath, "%ssys_table\\csv\\%s_code.csv", app, kp[i].name);

//2002.11.19 修正（ここから）----------------------------------------
//ファイル読み込み時に更新時刻を比較するように変更
//		//ファイルオープン
//		fp = fopen(fpath, "r");

		//ファイルポインタの初期化
		fp = NULL;
		//ファイル情報の取得
		ret = _stat(fpath, &st);
		if (ret == 0) {
			//更新時刻の比較
			if (mp->tim_code[i] != st.st_mtime) {
				mp->tim_code[i] = st.st_mtime;
				//領域の初期化
				memset(p, 0, kp[i].max * sizeof(sys_code_t));
				//ファイルオープン
				fp = fopen(fpath, "r");
			}
		}
//2002.11.19 修正（ここまで）----------------------------------------

		if (fp != NULL) {
			//必要ない行を読み飛ばす
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));

			while ( !feof(fp) ) {

				//１行読み込み
				csvread_n(fp, mCsvLine, sizeof(mCsvLine));

				//内部局番の判定
				csvsplit_n(mCsvLine, 0, dt, sizeof(dt));
				no = atoi(dt);
				if (1 <= no && no <= kp[i].max) {
					//内部局番
					p[no-1].sno = no;

					//エリア番号（国）
					csvsplit_n(mCsvLine, 1, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 255)
							p[no-1].area_no = v;
					}

					//事務所番号（国）
					csvsplit_n(mCsvLine, 2, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 255)
							p[no-1].office_no = v;
					}

					//水系域コード（国）
					csvsplit_n(mCsvLine, 3, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 999)
							p[no-1].river_sys_code = v;
					}

					//河川コード（国）
					csvsplit_n(mCsvLine, 4, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 999)
							p[no-1].river_code = v;
					}

					//観測所番号（国）
					csvsplit_n(mCsvLine, 5, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (0 <= v && v <= 65535)
							p[no-1].observatory_no = v;
					}

					//都道府県コード（国）
					csvsplit_n(mCsvLine, 6, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 99)
							p[no-1].pref_code = v;
					}

					//市町村コード（国）
					csvsplit_n(mCsvLine, 7, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 999)
							p[no-1].city_code = v;
					}

					//予備1

					//予備2
				}
			}
			fclose(fp);
		}
		//次のポインタへ移動
		p = p + kp[i].max;
	}
	return SYS_ERR_SUCCESS;
}

#if 0
/*---------------------------------------------------------
*	関数：getSysConstComp1
*	機能：局構成情報読み込み
*	引数：app               [IN]  データパス
        ：kp                [IN]  局種別情報
        ：pt                [OUT] 局名称情報
*	戻値：
*	説明：局構成情報ファイルを読み込みます。
        ：前提条件として、
        ：  局種別情報が設定されていること
        ：  局稼動情報の領域は確保されていること
        ：ファイルが存在しない場合、初期値が設定されます。
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
//2002.11.19 追加（ここから）----------------------------------------
//ファイル読み込み時に更新時刻を比較するように変更
	struct _stat st;
	int ret;
//2002.11.19 修正（ここまで）----------------------------------------


	if (app == NULL)
		return SYS_ERR_POINTER;
	if (kp == NULL)
		return SYS_ERR_POINTER;
	if (pt == NULL)
		return SYS_ERR_POINTER;

	//局構成情報の読み込み
	for (i = 0; i < SYS_MAX_KIND; i++) {
		if (kp[i].max <= 0) continue;

		//ファイルパスの設定
		sprintf(fpath, "%ssys_table\\csv\\%s_comp1.csv", app, kp[i].name);

//2002.11.19 修正（ここから）----------------------------------------
//ファイル読み込み時に更新時刻を比較するように変更
//		//ファイルオープン
//		fp = fopen(fpath, "r");

		//ファイルポインタの初期化
		fp = NULL;
		//ファイル情報の取得
		ret = _stat(fpath, &st);
		if (ret == 0) {
			//更新時刻の比較
			if (mp->tim_comp1[i] != st.st_mtime) {
				mp->tim_comp1[i] = st.st_mtime;
				//領域の初期化
				memset(p, 0, kp[i].max * sizeof(sys_comp1_t));
				//ファイルオープン
				fp = fopen(fpath, "r");
			}
		}
//2002.11.19 修正（ここまで）----------------------------------------

		if (fp != NULL) {
			//必要ない行を読み飛ばす
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));

			while ( !feof(fp) ) {

				//１行読み込み
				csvread_n(fp, mCsvLine, sizeof(mCsvLine));

				//内部局番の判定
				csvsplit_n(mCsvLine, 0, dt, sizeof(dt));
				no = atoi(dt);
				if (1 <= no && no <= kp[i].max) {
					//内部局番
					p[no-1].sno = no;

					//事務所番号
					csvsplit_n(mCsvLine, 1, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 99)
							p[no-1].office_no = v;
					}

					//
					//センサー１
					//

					//センサー
					csvsplit_n(mCsvLine, 2, p[no-1].s1_sensor, sizeof(p[no-1].s1_sensor));
					//形式
					csvsplit_n(mCsvLine, 3, p[no-1].s1_form, sizeof(p[no-1].s1_form));
					//型名
					csvsplit_n(mCsvLine, 4, p[no-1].s1_type, sizeof(p[no-1].s1_type));
					//日付１
					csvsplit_n(mCsvLine, 5, p[no-1].s1_date_1, sizeof(p[no-1].s1_date_1));
					//日付２
					csvsplit_n(mCsvLine, 6, p[no-1].s1_date_2, sizeof(p[no-1].s1_date_2));
					//日付３
					csvsplit_n(mCsvLine, 7, p[no-1].s1_date_3, sizeof(p[no-1].s1_date_3));
					//導入業者
					csvsplit_n(mCsvLine, 8, p[no-1].s1_introduce, sizeof(p[no-1].s1_introduce));
					//連絡先
					csvsplit_n(mCsvLine, 9, p[no-1].s1_intro_tel, sizeof(p[no-1].s1_intro_tel));
					//点検業者
					csvsplit_n(mCsvLine, 10, p[no-1].s1_chacker, sizeof(p[no-1].s1_chacker));
					//連絡先
					csvsplit_n(mCsvLine, 11, p[no-1].s1_chk_tel, sizeof(p[no-1].s1_chk_tel));
					//備考１
					csvsplit_n(mCsvLine, 12, p[no-1].s1_note_1, sizeof(p[no-1].s1_note_1));
					//備考２
					csvsplit_n(mCsvLine, 13, p[no-1].s1_note_2, sizeof(p[no-1].s1_note_2));
					//備考３
					csvsplit_n(mCsvLine, 14, p[no-1].s1_note_3, sizeof(p[no-1].s1_note_3));
					//備考４
					csvsplit_n(mCsvLine, 15, p[no-1].s1_note_4, sizeof(p[no-1].s1_note_4));

					//
					//センサー２
					//

					//センサー
					csvsplit_n(mCsvLine, 16, p[no-1].s2_sensor, sizeof(p[no-1].s2_sensor));
					//形式
					csvsplit_n(mCsvLine, 17, p[no-1].s2_form, sizeof(p[no-1].s2_form));
					//型名
					csvsplit_n(mCsvLine, 18, p[no-1].s2_type, sizeof(p[no-1].s2_type));
					//日付１
					csvsplit_n(mCsvLine, 19, p[no-1].s2_date_1, sizeof(p[no-1].s2_date_1));
					//日付２
					csvsplit_n(mCsvLine, 20, p[no-1].s2_date_2, sizeof(p[no-1].s2_date_2));
					//日付３
					csvsplit_n(mCsvLine, 21, p[no-1].s2_date_3, sizeof(p[no-1].s2_date_3));
					//導入業者
					csvsplit_n(mCsvLine, 22, p[no-1].s2_introduce, sizeof(p[no-1].s2_introduce));
					//連絡先
					csvsplit_n(mCsvLine, 23, p[no-1].s2_intro_tel, sizeof(p[no-1].s2_intro_tel));
					//点検業者
					csvsplit_n(mCsvLine, 24, p[no-1].s2_chacker, sizeof(p[no-1].s2_chacker));
					//連絡先
					csvsplit_n(mCsvLine, 25, p[no-1].s2_chk_tel, sizeof(p[no-1].s2_chk_tel));
					//備考１
					csvsplit_n(mCsvLine, 26, p[no-1].s2_note_1, sizeof(p[no-1].s2_note_1));
					//備考２
					csvsplit_n(mCsvLine, 27, p[no-1].s2_note_2, sizeof(p[no-1].s2_note_2));
					//備考３
					csvsplit_n(mCsvLine, 28, p[no-1].s2_note_3, sizeof(p[no-1].s2_note_3));
					//備考４
					csvsplit_n(mCsvLine, 29, p[no-1].s2_note_4, sizeof(p[no-1].s2_note_4));

					//
					//センサー３
					//

					//センサー
					csvsplit_n(mCsvLine, 30, p[no-1].s3_sensor, sizeof(p[no-1].s3_sensor));
					//形式
					csvsplit_n(mCsvLine, 31, p[no-1].s3_form, sizeof(p[no-1].s3_form));
					//型名
					csvsplit_n(mCsvLine, 32, p[no-1].s3_type, sizeof(p[no-1].s3_type));
					//日付１
					csvsplit_n(mCsvLine, 33, p[no-1].s3_date_1, sizeof(p[no-1].s3_date_1));
					//日付２
					csvsplit_n(mCsvLine, 34, p[no-1].s3_date_2, sizeof(p[no-1].s3_date_2));
					//日付３
					csvsplit_n(mCsvLine, 35, p[no-1].s3_date_3, sizeof(p[no-1].s3_date_3));
					//導入業者
					csvsplit_n(mCsvLine, 36, p[no-1].s3_introduce, sizeof(p[no-1].s3_introduce));
					//連絡先
					csvsplit_n(mCsvLine, 37, p[no-1].s3_intro_tel, sizeof(p[no-1].s3_intro_tel));
					//点検業者
					csvsplit_n(mCsvLine, 38, p[no-1].s3_chacker, sizeof(p[no-1].s3_chacker));
					//連絡先
					csvsplit_n(mCsvLine, 39, p[no-1].s3_chk_tel, sizeof(p[no-1].s3_chk_tel));
					//備考１
					csvsplit_n(mCsvLine, 40, p[no-1].s3_note_1, sizeof(p[no-1].s3_note_1));
					//備考２
					csvsplit_n(mCsvLine, 41, p[no-1].s3_note_2, sizeof(p[no-1].s3_note_2));
					//備考３
					csvsplit_n(mCsvLine, 42, p[no-1].s3_note_3, sizeof(p[no-1].s3_note_3));
					//備考４
					csvsplit_n(mCsvLine, 43, p[no-1].s3_note_4, sizeof(p[no-1].s3_note_4));

					//
					//センサー４
					//

					//センサー
					csvsplit_n(mCsvLine, 44, p[no-1].s4_sensor, sizeof(p[no-1].s4_sensor));
					//形式
					csvsplit_n(mCsvLine, 45, p[no-1].s4_form, sizeof(p[no-1].s4_form));
					//型名
					csvsplit_n(mCsvLine, 46, p[no-1].s4_type, sizeof(p[no-1].s4_type));
					//日付１
					csvsplit_n(mCsvLine, 47, p[no-1].s4_date_1, sizeof(p[no-1].s4_date_1));
					//日付２
					csvsplit_n(mCsvLine, 48, p[no-1].s4_date_2, sizeof(p[no-1].s4_date_2));
					//日付３
					csvsplit_n(mCsvLine, 49, p[no-1].s4_date_3, sizeof(p[no-1].s4_date_3));
					//導入業者
					csvsplit_n(mCsvLine, 50, p[no-1].s4_introduce, sizeof(p[no-1].s4_introduce));
					//連絡先
					csvsplit_n(mCsvLine, 51, p[no-1].s4_intro_tel, sizeof(p[no-1].s4_intro_tel));
					//点検業者
					csvsplit_n(mCsvLine, 52, p[no-1].s4_checker, sizeof(p[no-1].s4_checker));
					//連絡先
					csvsplit_n(mCsvLine, 53, p[no-1].s4_chk_tel, sizeof(p[no-1].s4_chk_tel));
					//備考１
					csvsplit_n(mCsvLine, 54, p[no-1].s4_note_1, sizeof(p[no-1].s4_note_1));
					//備考２
					csvsplit_n(mCsvLine, 55, p[no-1].s4_note_2, sizeof(p[no-1].s4_note_2));
					//備考３
					csvsplit_n(mCsvLine, 56, p[no-1].s4_note_3, sizeof(p[no-1].s4_note_3));
					//備考４
					csvsplit_n(mCsvLine, 57, p[no-1].s4_note_4, sizeof(p[no-1].s4_note_4));

					//
					//センサー５
					//

					//センサー
					csvsplit_n(mCsvLine, 58, p[no-1].s5_sensor, sizeof(p[no-1].s5_sensor));
					//形式
					csvsplit_n(mCsvLine, 59, p[no-1].s5_form, sizeof(p[no-1].s5_form));
					//型名
					csvsplit_n(mCsvLine, 60, p[no-1].s5_type, sizeof(p[no-1].s5_type));
					//日付１
					csvsplit_n(mCsvLine, 61, p[no-1].s5_date_1, sizeof(p[no-1].s5_date_1));
					//日付２
					csvsplit_n(mCsvLine, 62, p[no-1].s5_date_2, sizeof(p[no-1].s5_date_2));
					//日付３
					csvsplit_n(mCsvLine, 63, p[no-1].s5_date_3, sizeof(p[no-1].s5_date_3));
					//導入業者
					csvsplit_n(mCsvLine, 64, p[no-1].s5_introduce, sizeof(p[no-1].s5_introduce));
					//連絡先
					csvsplit_n(mCsvLine, 65, p[no-1].s5_intro_tel, sizeof(p[no-1].s5_intro_tel));
					//点検業者
					csvsplit_n(mCsvLine, 66, p[no-1].s5_checker, sizeof(p[no-1].s5_checker));
					//連絡先
					csvsplit_n(mCsvLine, 67, p[no-1].s5_chk_tel, sizeof(p[no-1].s5_chk_tel));
					//備考１
					csvsplit_n(mCsvLine, 68, p[no-1].s5_note_1, sizeof(p[no-1].s5_note_1));
					//備考２
					csvsplit_n(mCsvLine, 69, p[no-1].s5_note_2, sizeof(p[no-1].s5_note_2));
					//備考３
					csvsplit_n(mCsvLine, 70, p[no-1].s5_note_3, sizeof(p[no-1].s5_note_3));
					//備考４
					csvsplit_n(mCsvLine, 71, p[no-1].s5_note_4, sizeof(p[no-1].s5_note_4));

					//
					//記録計１
					//

					//記録計
					csvsplit_n(mCsvLine, 72, p[no-1].r1_recorder, sizeof(p[no-1].r1_recorder));
					//形式
					csvsplit_n(mCsvLine, 73, p[no-1].r1_form, sizeof(p[no-1].r1_form));
					//型名
					csvsplit_n(mCsvLine, 74, p[no-1].r1_type, sizeof(p[no-1].r1_type));
					//運用開始日
					csvsplit_n(mCsvLine, 75, p[no-1].r1_begin, sizeof(p[no-1].r1_begin));
					//更新年月日
					csvsplit_n(mCsvLine, 76, p[no-1].r1_update, sizeof(p[no-1].r1_update));
					//導入業者
					csvsplit_n(mCsvLine, 77, p[no-1].r1_introduce, sizeof(p[no-1].r1_introduce));
					//連絡先
					csvsplit_n(mCsvLine, 78, p[no-1].r1_intro_tel, sizeof(p[no-1].r1_intro_tel));
					//点検業者
					csvsplit_n(mCsvLine, 79, p[no-1].r1_checker, sizeof(p[no-1].r1_checker));
					//連絡先
					csvsplit_n(mCsvLine, 80, p[no-1].r1_chk_tel, sizeof(p[no-1].r1_chk_tel));
					//記録計の巻日数
					csvsplit_n(mCsvLine, 81, p[no-1].r1_reel_day, sizeof(p[no-1].r1_reel_day));
					//記録計の故障長期欠測有無
					csvsplit_n(mCsvLine, 82, p[no-1].r1_breakdown, sizeof(p[no-1].r1_breakdown));
					//備考１
					csvsplit_n(mCsvLine, 83, p[no-1].r1_note_1, sizeof(p[no-1].r1_note_1));
					//備考２
					csvsplit_n(mCsvLine, 84, p[no-1].r1_note_2, sizeof(p[no-1].r1_note_2));

					//
					//記録計２
					//

					//記録計
					csvsplit_n(mCsvLine, 85, p[no-1].r2_recorder, sizeof(p[no-1].r2_recorder));
					//形式
					csvsplit_n(mCsvLine, 86, p[no-1].r2_form, sizeof(p[no-1].r2_form));
					//型名
					csvsplit_n(mCsvLine, 87, p[no-1].r2_type, sizeof(p[no-1].r2_type));
					//運用開始日
					csvsplit_n(mCsvLine, 88, p[no-1].r2_begin, sizeof(p[no-1].r2_begin));
					//更新年月日
					csvsplit_n(mCsvLine, 89, p[no-1].r2_update, sizeof(p[no-1].r2_update));
					//導入業者
					csvsplit_n(mCsvLine, 90, p[no-1].r2_introduce, sizeof(p[no-1].r2_introduce));
					//連絡先
					csvsplit_n(mCsvLine, 91, p[no-1].r2_intro_tel, sizeof(p[no-1].r2_intro_tel));
					//点検業者
					csvsplit_n(mCsvLine, 92, p[no-1].r2_checker, sizeof(p[no-1].r2_checker));
					//連絡先
					csvsplit_n(mCsvLine, 93, p[no-1].r2_chk_tel, sizeof(p[no-1].r2_chk_tel));
					//記録計の巻日数
					csvsplit_n(mCsvLine, 94, p[no-1].r2_reel_day, sizeof(p[no-1].r2_reel_day));
					//記録計の故障長期欠測有無
					csvsplit_n(mCsvLine, 95, p[no-1].r2_breakdown, sizeof(p[no-1].r2_breakdown));
					//備考１
					csvsplit_n(mCsvLine, 96, p[no-1].r2_note_1, sizeof(p[no-1].r2_note_1));
					//備考２
					csvsplit_n(mCsvLine, 97, p[no-1].r2_note_2, sizeof(p[no-1].r2_note_2));


					//
					//記録計３
					//

					//記録計
					csvsplit_n(mCsvLine, 98, p[no-1].r3_recorder, sizeof(p[no-1].r3_recorder));
					//形式
					csvsplit_n(mCsvLine, 99, p[no-1].r3_form, sizeof(p[no-1].r3_form));
					//型名
					csvsplit_n(mCsvLine, 100, p[no-1].r3_type, sizeof(p[no-1].r3_type));
					//運用開始日
					csvsplit_n(mCsvLine, 101, p[no-1].r3_begin, sizeof(p[no-1].r3_begin));
					//更新年月日
					csvsplit_n(mCsvLine, 102, p[no-1].r3_update, sizeof(p[no-1].r3_update));
					//導入業者
					csvsplit_n(mCsvLine, 103, p[no-1].r3_introduce, sizeof(p[no-1].r3_introduce));
					//連絡先
					csvsplit_n(mCsvLine, 104, p[no-1].r3_intro_tel, sizeof(p[no-1].r3_intro_tel));
					//点検業者
					csvsplit_n(mCsvLine, 105, p[no-1].r3_checker, sizeof(p[no-1].r3_checker));
					//連絡先
					csvsplit_n(mCsvLine, 106, p[no-1].r3_chk_tel, sizeof(p[no-1].r3_chk_tel));
					//記録計の巻日数
					csvsplit_n(mCsvLine, 107, p[no-1].r3_reel_day, sizeof(p[no-1].r3_reel_day));
					//記録計の故障長期欠測有無
					csvsplit_n(mCsvLine, 108, p[no-1].r3_breakdown, sizeof(p[no-1].r3_breakdown));
					//備考１
					csvsplit_n(mCsvLine, 109, p[no-1].r3_note_1, sizeof(p[no-1].r3_note_1));
					//備考２
					csvsplit_n(mCsvLine, 110, p[no-1].r3_note_2, sizeof(p[no-1].r3_note_2));

					//
					//記録計４
					//

					//記録計
					csvsplit_n(mCsvLine, 111, p[no-1].r4_recorder, sizeof(p[no-1].r4_recorder));
					//形式
					csvsplit_n(mCsvLine, 112, p[no-1].r4_form, sizeof(p[no-1].r4_form));
					//型名
					csvsplit_n(mCsvLine, 113, p[no-1].r4_type, sizeof(p[no-1].r4_type));
					//運用開始日
					csvsplit_n(mCsvLine, 114, p[no-1].r4_begin, sizeof(p[no-1].r4_begin));
					//更新年月日
					csvsplit_n(mCsvLine, 115, p[no-1].r4_update, sizeof(p[no-1].r4_update));
					//導入業者
					csvsplit_n(mCsvLine, 116, p[no-1].r4_introduce, sizeof(p[no-1].r4_introduce));
					//連絡先
					csvsplit_n(mCsvLine, 117, p[no-1].r4_intro_tel, sizeof(p[no-1].r4_intro_tel));
					//点検業者
					csvsplit_n(mCsvLine, 118, p[no-1].r4_checker, sizeof(p[no-1].r4_checker));
					//連絡先
					csvsplit_n(mCsvLine, 119, p[no-1].r4_chk_tel, sizeof(p[no-1].r4_chk_tel));
					//記録計の巻日数
					csvsplit_n(mCsvLine, 120, p[no-1].r4_reel_day, sizeof(p[no-1].r4_reel_day));
					//記録計の故障長期欠測有無
					csvsplit_n(mCsvLine, 121, p[no-1].r4_breakdown, sizeof(p[no-1].r4_breakdown));
					//備考１
					csvsplit_n(mCsvLine, 122, p[no-1].r4_note_1, sizeof(p[no-1].r4_note_1));
					//備考２
					csvsplit_n(mCsvLine, 123, p[no-1].r4_note_2, sizeof(p[no-1].r4_note_2));

					//
					//記録計５
					//

					//記録計
					csvsplit_n(mCsvLine, 124, p[no-1].r5_recorder, sizeof(p[no-1].r5_recorder));
					//形式
					csvsplit_n(mCsvLine, 125, p[no-1].r5_form, sizeof(p[no-1].r5_form));
					//型名
					csvsplit_n(mCsvLine, 126, p[no-1].r5_type, sizeof(p[no-1].r5_type));
					//運用開始日
					csvsplit_n(mCsvLine, 127, p[no-1].r5_begin, sizeof(p[no-1].r5_begin));
					//更新年月日
					csvsplit_n(mCsvLine, 128, p[no-1].r5_update, sizeof(p[no-1].r5_update));
					//導入業者
					csvsplit_n(mCsvLine, 129, p[no-1].r5_introduce, sizeof(p[no-1].r5_introduce));
					//連絡先
					csvsplit_n(mCsvLine, 130, p[no-1].r5_intro_tel, sizeof(p[no-1].r5_intro_tel));
					//点検業者
					csvsplit_n(mCsvLine, 131, p[no-1].r5_checker, sizeof(p[no-1].r5_checker));
					//連絡先
					csvsplit_n(mCsvLine, 132, p[no-1].r5_chk_tel, sizeof(p[no-1].r5_chk_tel));
					//記録計の巻日数
					csvsplit_n(mCsvLine, 133, p[no-1].r5_reel_day, sizeof(p[no-1].r5_reel_day));
					//記録計の故障長期欠測有無
					csvsplit_n(mCsvLine, 134, p[no-1].r5_breakdown, sizeof(p[no-1].r5_breakdown));
					//備考１
					csvsplit_n(mCsvLine, 135, p[no-1].r5_note_1, sizeof(p[no-1].r5_note_1));
					//備考２
					csvsplit_n(mCsvLine, 136, p[no-1].r5_note_2, sizeof(p[no-1].r5_note_2));

					//
					//伝送回線
					//

					//伝送回線
					csvsplit_n(mCsvLine, 137, p[no-1].t_transmitte, sizeof(p[no-1].t_transmitte));
					//形式
					csvsplit_n(mCsvLine, 138, p[no-1].t_form, sizeof(p[no-1].t_form));
					//型名
					csvsplit_n(mCsvLine, 139, p[no-1].t_type, sizeof(p[no-1].t_type));
					//運用開始日
					csvsplit_n(mCsvLine, 140, p[no-1].t_begin, sizeof(p[no-1].t_begin));
					//更新年月日
					csvsplit_n(mCsvLine, 141, p[no-1].t_update, sizeof(p[no-1].t_update));
					//導入業者
					csvsplit_n(mCsvLine, 142, p[no-1].t_introduce, sizeof(p[no-1].t_introduce));
					//連絡先
					csvsplit_n(mCsvLine, 143, p[no-1].t_intro_tel, sizeof(p[no-1].t_intro_tel));
					//点検業者
					csvsplit_n(mCsvLine, 144, p[no-1].t_checker, sizeof(p[no-1].t_checker));
					//連絡先
					csvsplit_n(mCsvLine, 145, p[no-1].t_chk_tel, sizeof(p[no-1].t_chk_tel));
					//送信周波数
					csvsplit_n(mCsvLine, 146, p[no-1].t_snd_hz, sizeof(p[no-1].t_snd_hz));
					//受信周波数
					csvsplit_n(mCsvLine, 147, p[no-1].t_rcv_hz, sizeof(p[no-1].t_rcv_hz));

					//
					//空中線
					//

					//空中線
					csvsplit_n(mCsvLine, 148, p[no-1].a_air_line, sizeof(p[no-1].a_air_line));
					//通信方式
					csvsplit_n(mCsvLine, 149, p[no-1].a_form, sizeof(p[no-1].a_form));
					//備考
					csvsplit_n(mCsvLine, 150, p[no-1].a_note_1, sizeof(p[no-1].a_note_1));
					//備考
					csvsplit_n(mCsvLine, 151, p[no-1].a_note_2, sizeof(p[no-1].a_note_2));

					//
					//電源
					//

					//電源
					csvsplit_n(mCsvLine, 152, p[no-1].p_power, sizeof(p[no-1].p_power));
					//形式
					csvsplit_n(mCsvLine, 153, p[no-1].p_form, sizeof(p[no-1].p_form));
					//型名
					csvsplit_n(mCsvLine, 154, p[no-1].p_type, sizeof(p[no-1].p_type));
					//運用開始日
					csvsplit_n(mCsvLine, 155, p[no-1].p_begin, sizeof(p[no-1].p_begin));
					//更新年月日
					csvsplit_n(mCsvLine, 156, p[no-1].p_update, sizeof(p[no-1].p_update));
					//導入業者
					csvsplit_n(mCsvLine, 157, p[no-1].p_introduce, sizeof(p[no-1].p_introduce));
					//連絡先
					csvsplit_n(mCsvLine, 158, p[no-1].p_intro_tel, sizeof(p[no-1].p_intro_tel));
					//点検業者
					csvsplit_n(mCsvLine, 159, p[no-1].p_checker, sizeof(p[no-1].p_checker));
					//連絡先
					csvsplit_n(mCsvLine, 160, p[no-1].p_chk_tel, sizeof(p[no-1].p_chk_tel));
					//使用電源
					csvsplit_n(mCsvLine, 161, p[no-1].p_name, sizeof(p[no-1].p_name));
					//蓄電池容量
					csvsplit_n(mCsvLine, 162, p[no-1].p_capacity, sizeof(p[no-1].p_capacity));
					//備考
					csvsplit_n(mCsvLine, 163, p[no-1].p_note_1, sizeof(p[no-1].p_note_1));
					//備考
					csvsplit_n(mCsvLine, 164, p[no-1].p_note_2, sizeof(p[no-1].p_note_2));

					//
					//局舎
					//

					//局舎
					csvsplit_n(mCsvLine, 165, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 99)
							p[no-1].o_office = v;
					}
					//局舎種類
					csvsplit_n(mCsvLine, 166, p[no-1].o_kind, sizeof(p[no-1].o_kind));
					//局舎敷地
					csvsplit_n(mCsvLine, 167, p[no-1].o_site, sizeof(p[no-1].o_site));
					//局舎建坪
					csvsplit_n(mCsvLine, 168, p[no-1].o_space, sizeof(p[no-1].o_space));
					//その他
					csvsplit_n(mCsvLine, 169, p[no-1].o_others, sizeof(p[no-1].o_others));
					//借地人名
					csvsplit_n(mCsvLine, 170, p[no-1].o_tenant, sizeof(p[no-1].o_tenant));
					//借地代
					csvsplit_n(mCsvLine, 171, p[no-1].o_rent, sizeof(p[no-1].o_rent));
					//電柱共架
					csvsplit_n(mCsvLine, 172, p[no-1].o_utility_pole, sizeof(p[no-1].o_utility_pole));
					//記録の有無保管場所
					csvsplit_n(mCsvLine, 173, p[no-1].o_rec_keep, sizeof(p[no-1].o_rec_keep));
					//予備
					//予備


					//
					//テレメータ設置状況
					//
					//設置状況
					csvsplit_n(mCsvLine, 176, p[no-1].tm_telemeter, sizeof(p[no-1].tm_telemeter));
					//テレメータ開始日付（年）
					csvsplit_n(mCsvLine, 177, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1800 <= v && v <= 2100)
							p[no-1].tm_year = v;
					}
					//テレメータ開始日付（月）
					csvsplit_n(mCsvLine, 178, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 12)
							p[no-1].tm_mon = v;
					}
					//テレメータ開始日付（日）
					csvsplit_n(mCsvLine, 179, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 31)
							p[no-1].tm_day = v;
					}
					//背後地地盤高
					csvsplit_n(mCsvLine, 180, dt, sizeof(dt));
					if (dt[0] != '\0') {
						f = atof(dt);
						if (0.00 <= f && f <= 999.99)
							p[no-1].tm_back_basehi = f;
					}
					//量水標
					csvsplit_n(mCsvLine, 181, p[no-1].tm_sign, sizeof(p[no-1].tm_sign));
					//予備
					//予備

				}
			}
			fclose(fp);
		}
		//次のポインタへ移動
		p = p + kp[i].max;
	}
	return SYS_ERR_SUCCESS;
}

/*---------------------------------------------------------
*	関数：getSysConstComp2
*	機能：局構成情報読み込み
*	引数：app               [IN]  データパス
        ：kp                [IN]  局種別情報
        ：pt                [OUT] 局名称情報
*	戻値：
*	説明：局構成情報ファイルを読み込みます。
        ：前提条件として、
        ：  局種別情報が設定されていること
        ：  局稼動情報の領域は確保されていること
        ：ファイルが存在しない場合、初期値が設定されます。
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
//2002.11.19 追加（ここから）----------------------------------------
//ファイル読み込み時に更新時刻を比較するように変更
	struct _stat st;
	int  ret;
//2002.11.19 追加（ここまで）----------------------------------------


	if (app == NULL)
		return SYS_ERR_POINTER;
	if (kp == NULL)
		return SYS_ERR_POINTER;
	if (pt == NULL)
		return SYS_ERR_POINTER;

	//局構成情報の読み込み
	for (i = 0; i < SYS_MAX_KIND; i++) {
		if (kp[i].max <= 0) continue;

		//ファイルパスの設定
		sprintf(fpath, "%ssys_table\\csv\\%s_comp2.csv", app, kp[i].name);

//2002.11.19 修正（ここから）----------------------------------------
//ファイル読み込み時に更新時刻を比較するように変更
//		//ファイルオープン
//		fp = fopen(fpath, "r");

		//ファイルポインタの初期化
		fp = NULL;
		//ファイル情報の取得
		ret = _stat(fpath, &st);
		if (ret == 0) {
			//更新時刻の比較
			if (mp->tim_comp2[i] != st.st_mtime) {
				mp->tim_comp2[i] = st.st_mtime;
				//領域の初期化
				memset(p, 0, kp[i].max * sizeof(sys_comp2_t));
				//ファイルオープン
				fp = fopen(fpath, "r");
			}
		}
//2002.11.19 修正（ここまで）----------------------------------------
		if (fp != NULL) {
			//必要ない行を読み飛ばす
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));
			csvread_n(fp, mCsvLine, sizeof(mCsvLine));

			while ( !feof(fp) ) {

				//１行読み込み
				csvread_n(fp, mCsvLine, sizeof(mCsvLine));

				//内部局番の判定
				csvsplit_n(mCsvLine, 0, dt, sizeof(dt));
				no = atoi(dt);
				if (1 <= no && no <= kp[i].max) {
					//内部局番
					p[no-1].sno = no;

					//事務所番号
					csvsplit_n(mCsvLine, 1, dt, sizeof(dt));
					if (dt[0] != '\0') {
						v = atoi(dt);
						if (1 <= v && v <= 99)
							p[no-1].office_no = v;
					}

					//
					//事務所内装置１
					//

					//事務所内装置
					csvsplit_n(mCsvLine, 2, p[no-1].u1_unit, sizeof(p[no-1].u1_unit));
					//形式
					csvsplit_n(mCsvLine, 3, p[no-1].u1_form, sizeof(p[no-1].u1_form));
					//型名
					csvsplit_n(mCsvLine, 4, p[no-1].u1_type, sizeof(p[no-1].u1_type));
					//日付１
					csvsplit_n(mCsvLine, 5, p[no-1].u1_date_1, sizeof(p[no-1].u1_date_1));
					//日付２
					csvsplit_n(mCsvLine, 6, p[no-1].u1_date_2, sizeof(p[no-1].u1_date_2));
					//日付３
					csvsplit_n(mCsvLine, 7, p[no-1].u1_date_3, sizeof(p[no-1].u1_date_3));
					//導入業者
					csvsplit_n(mCsvLine, 8, p[no-1].u1_introduce, sizeof(p[no-1].u1_introduce));
					//連絡先
					csvsplit_n(mCsvLine, 9, p[no-1].u1_intro_tel, sizeof(p[no-1].u1_intro_tel));
					//点検業者
					csvsplit_n(mCsvLine, 10, p[no-1].u1_checker, sizeof(p[no-1].u1_checker));
					//連絡先
					csvsplit_n(mCsvLine, 11, p[no-1].u1_chk_tel, sizeof(p[no-1].u1_chk_tel));
					//備考１
					csvsplit_n(mCsvLine, 12, p[no-1].u1_note_1, sizeof(p[no-1].u1_note_1));
					//備考２
					csvsplit_n(mCsvLine, 13, p[no-1].u1_note_2, sizeof(p[no-1].u1_note_2));
					//備考３
					csvsplit_n(mCsvLine, 14, p[no-1].u1_note_3, sizeof(p[no-1].u1_note_3));
					//備考４
					csvsplit_n(mCsvLine, 15, p[no-1].u1_note_4, sizeof(p[no-1].u1_note_4));

					//
					//事務所内装置２
					//

					//事務所内装置
					csvsplit_n(mCsvLine, 16, p[no-1].u2_unit, sizeof(p[no-1].u2_unit));
					//形式
					csvsplit_n(mCsvLine, 17, p[no-1].u2_form, sizeof(p[no-1].u2_form));
					//型名
					csvsplit_n(mCsvLine, 18, p[no-1].u2_type, sizeof(p[no-1].u2_type));
					//日付１
					csvsplit_n(mCsvLine, 19, p[no-1].u2_date_1, sizeof(p[no-1].u2_date_1));
					//日付２
					csvsplit_n(mCsvLine, 20, p[no-1].u2_date_2, sizeof(p[no-1].u2_date_2));
					//日付３
					csvsplit_n(mCsvLine, 21, p[no-1].u2_date_3, sizeof(p[no-1].u2_date_3));
					//導入業者
					csvsplit_n(mCsvLine, 22, p[no-1].u2_introduce, sizeof(p[no-1].u2_introduce));
					//連絡先
					csvsplit_n(mCsvLine, 23, p[no-1].u2_intro_tel, sizeof(p[no-1].u2_intro_tel));
					//点検業者
					csvsplit_n(mCsvLine, 24, p[no-1].u2_checker, sizeof(p[no-1].u2_checker));
					//連絡先
					csvsplit_n(mCsvLine, 25, p[no-1].u2_chk_tel, sizeof(p[no-1].u2_chk_tel));
					//備考１
					csvsplit_n(mCsvLine, 26, p[no-1].u2_note_1, sizeof(p[no-1].u2_note_1));
					//備考２
					csvsplit_n(mCsvLine, 27, p[no-1].u2_note_2, sizeof(p[no-1].u2_note_2));
					//備考３
					csvsplit_n(mCsvLine, 28, p[no-1].u2_note_3, sizeof(p[no-1].u2_note_3));
					//備考４
					csvsplit_n(mCsvLine, 29, p[no-1].u2_note_4, sizeof(p[no-1].u2_note_4));

					//
					//事務所内装置３
					//

					//事務所内装置
					csvsplit_n(mCsvLine, 30, p[no-1].u3_unit, sizeof(p[no-1].u3_unit));
					//形式
					csvsplit_n(mCsvLine, 31, p[no-1].u3_form, sizeof(p[no-1].u3_form));
					//型名
					csvsplit_n(mCsvLine, 32, p[no-1].u3_type, sizeof(p[no-1].u3_type));
					//日付１
					csvsplit_n(mCsvLine, 33, p[no-1].u3_date_1, sizeof(p[no-1].u3_date_1));
					//日付２
					csvsplit_n(mCsvLine, 34, p[no-1].u3_date_2, sizeof(p[no-1].u3_date_2));
					//日付３
					csvsplit_n(mCsvLine, 35, p[no-1].u3_date_3, sizeof(p[no-1].u3_date_3));
					//導入業者
					csvsplit_n(mCsvLine, 36, p[no-1].u3_introduce, sizeof(p[no-1].u3_introduce));
					//連絡先
					csvsplit_n(mCsvLine, 37, p[no-1].u3_intro_tel, sizeof(p[no-1].u3_intro_tel));
					//点検業者
					csvsplit_n(mCsvLine, 38, p[no-1].u3_checker, sizeof(p[no-1].u3_checker));
					//連絡先
					csvsplit_n(mCsvLine, 39, p[no-1].u3_chk_tel, sizeof(p[no-1].u3_chk_tel));
					//備考１
					csvsplit_n(mCsvLine, 40, p[no-1].u3_note_1, sizeof(p[no-1].u3_note_1));
					//備考２
					csvsplit_n(mCsvLine, 41, p[no-1].u3_note_2, sizeof(p[no-1].u3_note_2));
					//備考３
					csvsplit_n(mCsvLine, 42, p[no-1].u3_note_3, sizeof(p[no-1].u3_note_3));
					//備考４
					csvsplit_n(mCsvLine, 43, p[no-1].u3_note_4, sizeof(p[no-1].u3_note_4));

					//
					//事務所内装置４
					//

					//事務所内装置
					csvsplit_n(mCsvLine, 44, p[no-1].u4_unit, sizeof(p[no-1].u4_unit));
					//形式
					csvsplit_n(mCsvLine, 45, p[no-1].u4_form, sizeof(p[no-1].u4_form));
					//型名
					csvsplit_n(mCsvLine, 46, p[no-1].u4_type, sizeof(p[no-1].u4_type));
					//日付１
					csvsplit_n(mCsvLine, 47, p[no-1].u4_date_1, sizeof(p[no-1].u4_date_1));
					//日付２
					csvsplit_n(mCsvLine, 48, p[no-1].u4_date_2, sizeof(p[no-1].u4_date_2));
					//日付３
					csvsplit_n(mCsvLine, 49, p[no-1].u4_date_3, sizeof(p[no-1].u4_date_3));
					//導入業者
					csvsplit_n(mCsvLine, 50, p[no-1].u4_introduce, sizeof(p[no-1].u4_introduce));
					//連絡先
					csvsplit_n(mCsvLine, 51, p[no-1].u4_intro_tel, sizeof(p[no-1].u4_intro_tel));
					//点検業者
					csvsplit_n(mCsvLine, 52, p[no-1].u4_checker, sizeof(p[no-1].u4_checker));
					//連絡先
					csvsplit_n(mCsvLine, 53, p[no-1].u4_chk_tel, sizeof(p[no-1].u4_chk_tel));
					//備考１
					csvsplit_n(mCsvLine, 54, p[no-1].u4_note_1, sizeof(p[no-1].u4_note_1));
					//備考２
					csvsplit_n(mCsvLine, 55, p[no-1].u4_note_2, sizeof(p[no-1].u4_note_2));
					//備考３
					csvsplit_n(mCsvLine, 56, p[no-1].u4_note_3, sizeof(p[no-1].u4_note_3));
					//備考４
					csvsplit_n(mCsvLine, 57, p[no-1].u4_note_4, sizeof(p[no-1].u4_note_4));

					//
					//事務所内装置５
					//

					//事務所内装置
					csvsplit_n(mCsvLine, 58, p[no-1].u5_unit, sizeof(p[no-1].u5_unit));
					//形式
					csvsplit_n(mCsvLine, 59, p[no-1].u5_form, sizeof(p[no-1].u5_form));
					//型名
					csvsplit_n(mCsvLine, 60, p[no-1].u5_type, sizeof(p[no-1].u5_type));
					//日付１
					csvsplit_n(mCsvLine, 61, p[no-1].u5_date_1, sizeof(p[no-1].u5_date_1));
					//日付２
					csvsplit_n(mCsvLine, 62, p[no-1].u5_date_2, sizeof(p[no-1].u5_date_2));
					//日付３
					csvsplit_n(mCsvLine, 63, p[no-1].u5_date_3, sizeof(p[no-1].u5_date_3));
					//導入業者
					csvsplit_n(mCsvLine, 64, p[no-1].u5_introduce, sizeof(p[no-1].u5_introduce));
					//連絡先
					csvsplit_n(mCsvLine, 65, p[no-1].u5_intro_tel, sizeof(p[no-1].u5_intro_tel));
					//点検業者
					csvsplit_n(mCsvLine, 66, p[no-1].u5_checker, sizeof(p[no-1].u5_checker));
					//連絡先
					csvsplit_n(mCsvLine, 67, p[no-1].u5_chk_tel, sizeof(p[no-1].u5_chk_tel));
					//備考１
					csvsplit_n(mCsvLine, 68, p[no-1].u5_note_1, sizeof(p[no-1].u5_note_1));
					//備考２
					csvsplit_n(mCsvLine, 69, p[no-1].u5_note_2, sizeof(p[no-1].u5_note_2));
					//備考３
					csvsplit_n(mCsvLine, 70, p[no-1].u5_note_3, sizeof(p[no-1].u5_note_3));
					//備考４
					csvsplit_n(mCsvLine, 71, p[no-1].u5_note_4, sizeof(p[no-1].u5_note_4));

					//
					//事務所内装置６
					//

					//事務所内装置
					csvsplit_n(mCsvLine, 72, p[no-1].u6_unit, sizeof(p[no-1].u6_unit));
					//形式
					csvsplit_n(mCsvLine, 73, p[no-1].u6_form, sizeof(p[no-1].u6_form));
					//型名
					csvsplit_n(mCsvLine, 74, p[no-1].u6_type, sizeof(p[no-1].u6_type));
					//日付１
					csvsplit_n(mCsvLine, 75, p[no-1].u6_date_1, sizeof(p[no-1].u6_date_1));
					//日付２
					csvsplit_n(mCsvLine, 76, p[no-1].u6_date_2, sizeof(p[no-1].u6_date_2));
					//日付３
					csvsplit_n(mCsvLine, 77, p[no-1].u6_date_3, sizeof(p[no-1].u6_date_3));
					//導入業者
					csvsplit_n(mCsvLine, 78, p[no-1].u6_introduce, sizeof(p[no-1].u6_introduce));
					//連絡先
					csvsplit_n(mCsvLine, 79, p[no-1].u6_intro_tel, sizeof(p[no-1].u6_intro_tel));
					//点検業者
					csvsplit_n(mCsvLine, 80, p[no-1].u6_checker, sizeof(p[no-1].u6_checker));
					//連絡先
					csvsplit_n(mCsvLine, 81, p[no-1].u6_chk_tel, sizeof(p[no-1].u6_chk_tel));
					//備考１
					csvsplit_n(mCsvLine, 82, p[no-1].u6_note_1, sizeof(p[no-1].u6_note_1));
					//備考２
					csvsplit_n(mCsvLine, 83, p[no-1].u6_note_2, sizeof(p[no-1].u6_note_2));
					//備考３
					csvsplit_n(mCsvLine, 84, p[no-1].u6_note_3, sizeof(p[no-1].u6_note_3));
					//備考４
					csvsplit_n(mCsvLine, 85, p[no-1].u6_note_4, sizeof(p[no-1].u6_note_4));

					//
					//事務所内装置７
					//

					//事務所内装置
					csvsplit_n(mCsvLine, 86, p[no-1].u7_unit, sizeof(p[no-1].u7_unit));
					//形式
					csvsplit_n(mCsvLine, 87, p[no-1].u7_form, sizeof(p[no-1].u7_form));
					//型名
					csvsplit_n(mCsvLine, 88, p[no-1].u7_type, sizeof(p[no-1].u7_type));
					//日付１
					csvsplit_n(mCsvLine, 89, p[no-1].u7_date_1, sizeof(p[no-1].u7_date_1));
					//日付２
					csvsplit_n(mCsvLine, 90, p[no-1].u7_date_2, sizeof(p[no-1].u7_date_2));
					//日付３
					csvsplit_n(mCsvLine, 91, p[no-1].u7_date_3, sizeof(p[no-1].u7_date_3));
					//導入業者
					csvsplit_n(mCsvLine, 92, p[no-1].u7_introduce, sizeof(p[no-1].u7_introduce));
					//連絡先
					csvsplit_n(mCsvLine, 93, p[no-1].u7_intro_tel, sizeof(p[no-1].u7_intro_tel));
					//点検業者
					csvsplit_n(mCsvLine, 94, p[no-1].u7_checker, sizeof(p[no-1].u7_checker));
					//連絡先
					csvsplit_n(mCsvLine, 95, p[no-1].u7_chk_tel, sizeof(p[no-1].u7_chk_tel));
					//備考１
					csvsplit_n(mCsvLine, 96, p[no-1].u7_note_1, sizeof(p[no-1].u7_note_1));
					//備考２
					csvsplit_n(mCsvLine, 97, p[no-1].u7_note_2, sizeof(p[no-1].u7_note_2));
					//備考３
					csvsplit_n(mCsvLine, 98, p[no-1].u7_note_3, sizeof(p[no-1].u7_note_3));
					//備考４
					csvsplit_n(mCsvLine, 99, p[no-1].u7_note_4, sizeof(p[no-1].u7_note_4));

					//
					//事務所内装置８
					//

					//事務所内装置
					csvsplit_n(mCsvLine, 100, p[no-1].u8_unit, sizeof(p[no-1].u8_unit));
					//形式
					csvsplit_n(mCsvLine, 101, p[no-1].u8_form, sizeof(p[no-1].u8_form));
					//型名
					csvsplit_n(mCsvLine, 102, p[no-1].u8_type, sizeof(p[no-1].u8_type));
					//日付１
					csvsplit_n(mCsvLine, 103, p[no-1].u8_date_1, sizeof(p[no-1].u8_date_1));
					//日付２
					csvsplit_n(mCsvLine, 104, p[no-1].u8_date_2, sizeof(p[no-1].u8_date_2));
					//日付３
					csvsplit_n(mCsvLine, 105, p[no-1].u8_date_3, sizeof(p[no-1].u8_date_3));
					//導入業者
					csvsplit_n(mCsvLine, 106, p[no-1].u8_introduce, sizeof(p[no-1].u8_introduce));
					//連絡先
					csvsplit_n(mCsvLine, 107, p[no-1].u8_intro_tel, sizeof(p[no-1].u8_intro_tel));
					//点検業者
					csvsplit_n(mCsvLine, 108, p[no-1].u8_checker, sizeof(p[no-1].u8_checker));
					//連絡先
					csvsplit_n(mCsvLine, 109, p[no-1].u8_chk_tel, sizeof(p[no-1].u8_chk_tel));
					//備考１
					csvsplit_n(mCsvLine, 110, p[no-1].u8_note_1, sizeof(p[no-1].u8_note_1));
					//備考２
					csvsplit_n(mCsvLine, 111, p[no-1].u8_note_2, sizeof(p[no-1].u8_note_2));
					//備考３
					csvsplit_n(mCsvLine, 112, p[no-1].u8_note_3, sizeof(p[no-1].u8_note_3));
					//備考４
					csvsplit_n(mCsvLine, 113, p[no-1].u8_note_4, sizeof(p[no-1].u8_note_4));

					//
					//事務所内装置９
					//

					//事務所内装置
					csvsplit_n(mCsvLine, 114, p[no-1].u9_unit, sizeof(p[no-1].u9_unit));
					//形式
					csvsplit_n(mCsvLine, 115, p[no-1].u9_form, sizeof(p[no-1].u9_form));
					//型名
					csvsplit_n(mCsvLine, 116, p[no-1].u9_type, sizeof(p[no-1].u9_type));
					//日付１
					csvsplit_n(mCsvLine, 117, p[no-1].u9_date_1, sizeof(p[no-1].u9_date_1));
					//日付２
					csvsplit_n(mCsvLine, 118, p[no-1].u9_date_2, sizeof(p[no-1].u9_date_2));
					//日付３
					csvsplit_n(mCsvLine, 119, p[no-1].u9_date_3, sizeof(p[no-1].u9_date_3));
					//導入業者
					csvsplit_n(mCsvLine, 120, p[no-1].u9_introduce, sizeof(p[no-1].u9_introduce));
					//連絡先
					csvsplit_n(mCsvLine, 121, p[no-1].u9_intro_tel, sizeof(p[no-1].u9_intro_tel));
					//点検業者
					csvsplit_n(mCsvLine, 122, p[no-1].u9_checker, sizeof(p[no-1].u9_checker));
					//連絡先
					csvsplit_n(mCsvLine, 123, p[no-1].u9_chk_tel, sizeof(p[no-1].u9_chk_tel));
					//備考１
					csvsplit_n(mCsvLine, 124, p[no-1].u9_note_1, sizeof(p[no-1].u9_note_1));
					//備考２
					csvsplit_n(mCsvLine, 125, p[no-1].u9_note_2, sizeof(p[no-1].u9_note_2));
					//備考３
					csvsplit_n(mCsvLine, 126, p[no-1].u9_note_3, sizeof(p[no-1].u9_note_3));
					//備考４
					csvsplit_n(mCsvLine, 127, p[no-1].u9_note_4, sizeof(p[no-1].u9_note_4));

					//
					//事務所内装置１０
					//

					//事務所内装置
					csvsplit_n(mCsvLine, 128, p[no-1].u10_unit, sizeof(p[no-1].u10_unit));
					//形式
					csvsplit_n(mCsvLine, 129, p[no-1].u10_form, sizeof(p[no-1].u10_form));
					//型名
					csvsplit_n(mCsvLine, 130, p[no-1].u10_type, sizeof(p[no-1].u10_type));
					//日付１
					csvsplit_n(mCsvLine, 131, p[no-1].u10_date_1, sizeof(p[no-1].u10_date_1));
					//日付２
					csvsplit_n(mCsvLine, 132, p[no-1].u10_date_2, sizeof(p[no-1].u10_date_2));
					//日付３
					csvsplit_n(mCsvLine, 133, p[no-1].u10_date_3, sizeof(p[no-1].u10_date_3));
					//導入業者
					csvsplit_n(mCsvLine, 134, p[no-1].u10_introduce, sizeof(p[no-1].u10_introduce));
					//連絡先
					csvsplit_n(mCsvLine, 135, p[no-1].u10_intro_tel, sizeof(p[no-1].u10_intro_tel));
					//点検業者
					csvsplit_n(mCsvLine, 136, p[no-1].u10_checker, sizeof(p[no-1].u10_checker));
					//連絡先
					csvsplit_n(mCsvLine, 137, p[no-1].u10_chk_tel, sizeof(p[no-1].u10_chk_tel));
					//備考１
					csvsplit_n(mCsvLine, 138, p[no-1].u10_note_1, sizeof(p[no-1].u10_note_1));
					//備考２
					csvsplit_n(mCsvLine, 139, p[no-1].u10_note_2, sizeof(p[no-1].u10_note_2));
					//備考３
					csvsplit_n(mCsvLine, 140, p[no-1].u10_note_3, sizeof(p[no-1].u10_note_3));
					//備考４
					csvsplit_n(mCsvLine, 141, p[no-1].u10_note_4, sizeof(p[no-1].u10_note_4));

					//
					//記事１
					//

					//記事年月日
					csvsplit_n(mCsvLine, 142, p[no-1].n1_notice_date, sizeof(p[no-1].n1_notice_date));
					//記事内容
					csvsplit_n(mCsvLine, 143, p[no-1].n1_contents, sizeof(p[no-1].n1_contents));
					//記事対応業者
					csvsplit_n(mCsvLine, 144, p[no-1].n1_trader, sizeof(p[no-1].n1_trader));
					//記事備考
					csvsplit_n(mCsvLine, 145, p[no-1].n1_note, sizeof(p[no-1].n1_note));
					//予備
					csvsplit_n(mCsvLine, 146, p[no-1].n1_reserve1, sizeof(p[no-1].n1_reserve1));
					//予備
					csvsplit_n(mCsvLine, 147, p[no-1].n1_reserve2, sizeof(p[no-1].n1_reserve2));

					//
					//記事２
					//

					//記事年月日
					csvsplit_n(mCsvLine, 148, p[no-1].n2_notice_date, sizeof(p[no-1].n2_notice_date));
					//記事内容
					csvsplit_n(mCsvLine, 149, p[no-1].n2_contents, sizeof(p[no-1].n2_contents));
					//記事対応業者
					csvsplit_n(mCsvLine, 150, p[no-1].n2_trader, sizeof(p[no-1].n2_trader));
					//記事備考
					csvsplit_n(mCsvLine, 151, p[no-1].n2_note, sizeof(p[no-1].n2_note));
					//予備
					csvsplit_n(mCsvLine, 152, p[no-1].n2_reserve1, sizeof(p[no-1].n2_reserve1));
					//予備
					csvsplit_n(mCsvLine, 153, p[no-1].n2_reserve2, sizeof(p[no-1].n2_reserve2));

					//
					//記事３
					//

					//記事年月日
					csvsplit_n(mCsvLine, 154, p[no-1].n3_notice_date, sizeof(p[no-1].n3_notice_date));
					//記事内容
					csvsplit_n(mCsvLine, 155, p[no-1].n3_contents, sizeof(p[no-1].n3_contents));
					//記事対応業者
					csvsplit_n(mCsvLine, 156, p[no-1].n3_trader, sizeof(p[no-1].n3_trader));
					//記事備考
					csvsplit_n(mCsvLine, 157, p[no-1].n3_note, sizeof(p[no-1].n3_note));
					//予備
					csvsplit_n(mCsvLine, 158, p[no-1].n3_reserve1, sizeof(p[no-1].n3_reserve1));
					//予備
					csvsplit_n(mCsvLine, 159, p[no-1].n3_reserve2, sizeof(p[no-1].n3_reserve2));

					//
					//記事４
					//

					//記事年月日
					csvsplit_n(mCsvLine, 160, p[no-1].n4_notice_date, sizeof(p[no-1].n4_notice_date));
					//記事内容
					csvsplit_n(mCsvLine, 161, p[no-1].n4_contents, sizeof(p[no-1].n4_contents));
					//記事対応業者
					csvsplit_n(mCsvLine, 162, p[no-1].n4_trader, sizeof(p[no-1].n4_trader));
					//記事備考
					csvsplit_n(mCsvLine, 163, p[no-1].n4_note, sizeof(p[no-1].n4_note));
					//予備
					csvsplit_n(mCsvLine, 164, p[no-1].n4_reserve1, sizeof(p[no-1].n4_reserve1));
					//予備
					csvsplit_n(mCsvLine, 165, p[no-1].n4_reserve2, sizeof(p[no-1].n4_reserve2));

					//
					//記事５
					//

					//記事年月日
					csvsplit_n(mCsvLine, 166, p[no-1].n5_notice_date, sizeof(p[no-1].n5_notice_date));
					//記事内容
					csvsplit_n(mCsvLine, 167, p[no-1].n5_contents, sizeof(p[no-1].n5_contents));
					//記事対応業者
					csvsplit_n(mCsvLine, 168, p[no-1].n5_trader, sizeof(p[no-1].n5_trader));
					//記事備考
					csvsplit_n(mCsvLine, 169, p[no-1].n5_note, sizeof(p[no-1].n5_note));
					//予備
					csvsplit_n(mCsvLine, 170, p[no-1].n5_reserve1, sizeof(p[no-1].n5_reserve1));
					//予備
					csvsplit_n(mCsvLine, 171, p[no-1].n5_reserve2, sizeof(p[no-1].n5_reserve2));

					//
					//記事６
					//

					//記事年月日
					csvsplit_n(mCsvLine, 172, p[no-1].n6_notice_date, sizeof(p[no-1].n6_notice_date));
					//記事内容
					csvsplit_n(mCsvLine, 173, p[no-1].n6_contents, sizeof(p[no-1].n6_contents));
					//記事対応業者
					csvsplit_n(mCsvLine, 174, p[no-1].n6_trader, sizeof(p[no-1].n6_trader));
					//記事備考
					csvsplit_n(mCsvLine, 175, p[no-1].n6_note, sizeof(p[no-1].n6_note));
					//予備
					csvsplit_n(mCsvLine, 176, p[no-1].n6_reserve1, sizeof(p[no-1].n6_reserve1));
					//予備
					csvsplit_n(mCsvLine, 177, p[no-1].n6_reserve2, sizeof(p[no-1].n6_reserve2));

					//
					//記事７
					//

					//記事年月日
					csvsplit_n(mCsvLine, 178, p[no-1].n7_notice_date, sizeof(p[no-1].n7_notice_date));
					//記事内容
					csvsplit_n(mCsvLine, 179, p[no-1].n7_contents, sizeof(p[no-1].n7_contents));
					//記事対応業者
					csvsplit_n(mCsvLine, 180, p[no-1].n7_trader, sizeof(p[no-1].n7_trader));
					//記事備考
					csvsplit_n(mCsvLine, 181, p[no-1].n7_note, sizeof(p[no-1].n7_note));
					//予備
					csvsplit_n(mCsvLine, 182, p[no-1].n7_reserve1, sizeof(p[no-1].n7_reserve1));
					//予備
					csvsplit_n(mCsvLine, 183, p[no-1].n7_reserve2, sizeof(p[no-1].n7_reserve2));

					//
					//記事８
					//

					//記事年月日
					csvsplit_n(mCsvLine, 184, p[no-1].n8_notice_date, sizeof(p[no-1].n8_notice_date));
					//記事内容
					csvsplit_n(mCsvLine, 185, p[no-1].n8_contents, sizeof(p[no-1].n8_contents));
					//記事対応業者
					csvsplit_n(mCsvLine, 186, p[no-1].n8_trader, sizeof(p[no-1].n8_trader));
					//記事備考
					csvsplit_n(mCsvLine, 187, p[no-1].n8_note, sizeof(p[no-1].n8_note));
					//予備
					csvsplit_n(mCsvLine, 188, p[no-1].n8_reserve1, sizeof(p[no-1].n8_reserve1));
					//予備
					csvsplit_n(mCsvLine, 189, p[no-1].n8_reserve2, sizeof(p[no-1].n8_reserve2));

					//
					//記事９
					//

					//記事年月日
					csvsplit_n(mCsvLine, 190, p[no-1].n9_notice_date, sizeof(p[no-1].n9_notice_date));
					//記事内容
					csvsplit_n(mCsvLine, 191, p[no-1].n9_contents, sizeof(p[no-1].n9_contents));
					//記事対応業者
					csvsplit_n(mCsvLine, 192, p[no-1].n9_trader, sizeof(p[no-1].n9_trader));
					//記事備考
					csvsplit_n(mCsvLine, 193, p[no-1].n9_note, sizeof(p[no-1].n9_note));
					//予備
					csvsplit_n(mCsvLine, 194, p[no-1].n9_reserve1, sizeof(p[no-1].n9_reserve1));
					//予備
					csvsplit_n(mCsvLine, 195, p[no-1].n9_reserve2, sizeof(p[no-1].n9_reserve2));

					//
					//記事１０
					//

					//記事年月日
					csvsplit_n(mCsvLine, 196, p[no-1].n10_notice_date, sizeof(p[no-1].n10_notice_date));
					//記事内容
					csvsplit_n(mCsvLine, 197, p[no-1].n10_contents, sizeof(p[no-1].n10_contents));
					//記事対応業者
					csvsplit_n(mCsvLine, 198, p[no-1].n10_trader, sizeof(p[no-1].n10_trader));
					//記事備考
					csvsplit_n(mCsvLine, 199, p[no-1].n10_note, sizeof(p[no-1].n10_note));
					//予備
					csvsplit_n(mCsvLine, 200, p[no-1].n10_reserve1, sizeof(p[no-1].n10_reserve1));
					//予備
					csvsplit_n(mCsvLine, 201, p[no-1].n10_reserve2, sizeof(p[no-1].n10_reserve2));
				}
			}
			fclose(fp);
		}
		//次のポインタへ移動
		p = p + kp[i].max;
	}
	return SYS_ERR_SUCCESS;
}
#endif

/*---------------------------------------------------------
*	関数：createSysConstKindInfo
*	機能：局種別管理情報共有メモリ作成
*	引数：pt                [OUT] 局種別管理情報共有メモリ
*	戻値：
*	説明：
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


	//パラメータの確認
	if (pt == NULL)
		return SYS_ERR_POINTER;

	//変数の初期化
	sysKind = new sys_kind_t[SYS_MAX_KIND];
	if (sysKind == NULL)
		return SYS_ERR_ALLOC_MEMORY;
	memset(sysKind,   0, sizeof(sys_kind_t) * SYS_MAX_KIND);
	memset(&myNumber, 0, sizeof(myNumber));
	memset(&srvInfo,  0, sizeof(srvInfo));

	//自装置番号の読み込み
	ret = getMyNumber(&myNumber);
	if (ret != SYS_ERR_SUCCESS) {
		free(sysKind);
		return ret;
	}

	//サーバ初期化ファイルの読み込み
	ret = getServerIniFile(myNumber.intOfficeID, myNumber.intUnitID, &srvInfo);
	if (ret != SYS_ERR_SUCCESS) {
		free(sysKind);
		return ret;
	}

	//局種別情報の読み込み
	ret = getSysConstKind(srvInfo.strAppPath, sysKind);
	if (ret != SYS_ERR_SUCCESS) {
		free(sysKind);
		return ret;
	}

	//局種別情報からメモリサイズの計算
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
		//最大局種別番号
		kmax = i+1;
	}
//	msize = s_kind + s_local + s_action + s_name + s_optname + s_code + s_comp1 + s_comp2;
	msize = s_kind + s_local + s_action + s_name + s_optname + s_code;

	//共有メモリの確保
	ret = dll_createSharedMemory(pt, SYS_NAME_KIND, s_head + msize);
	if (ret < 0) {
		free(sysKind);
		return SYS_ERR_SHARED_MEMORY;
	}

	//管理情報の設定
	memset(pt->mem, 0, s_head + msize);
	mp = (kind_sharedmem_t *)pt->mem;
	strcpy(mp->app, srvInfo.strDataPath);
	mp->officeID = myNumber.intOfficeID;
	mp->unitID   = myNumber.intUnitID;
	mp->max      = kmax;
	mp->msize    = msize;

	//先頭からの相対位置の設定
	mp->idx_kind    = s_head;
	mp->idx_local   = s_head + s_kind;
	mp->idx_action  = s_head + s_kind + s_local;
	mp->idx_name    = s_head + s_kind + s_local + s_action;
	mp->idx_optname = s_head + s_kind + s_local + s_action + s_name;
	mp->idx_code    = s_head + s_kind + s_local + s_action + s_name + s_optname;
//	mp->idx_comp1   = s_head + s_kind + s_local + s_action + s_name + s_optname + s_code;
//	mp->idx_comp2   = s_head + s_kind + s_local + s_action + s_name + s_optname + s_code + s_comp1;

	//局種別情報のコピー
	memcpy(pt->mem + mp->idx_kind, sysKind, s_kind);
	free(sysKind);

	return SYS_ERR_SUCCESS;
}


/*---------------------------------------------------------
*	関数：readSysConstKindInfo
*	機能：局種別情報読み込み
*	引数：pt                [I/O] 局種別管理情報共有メモリ
*	戻値：
*	説明：
---------------------------------------------------------*/
int readSysConstKindInfo(sharedMem_t *pt)
{
	kind_sharedmem_t *mp = (kind_sharedmem_t *)pt->mem;

	if (pt == NULL)
		return SYS_ERR_POINTER;
	if (pt->mem == NULL)
		return SYS_ERR_POINTER;

//2002.11.19 削除（ここから）----------------------------------------
//ファイル読み込み時に更新時刻を比較するように変更した為
//情報のクリアは各自読み込み時に行うこととする
//	//情報のクリア
//	memset(pt->mem + mp->idx_local, 0, mp->idx_comp - mp->idx_local);
//2002.11.19 削除（ここまで）----------------------------------------

	//局実装情報の読み込み
	getSysConstLocal  (mp, mp->app, mp->officeID, mp->unitID, (sys_kind_t *)(pt->mem + mp->idx_kind), (sys_local_t *)(pt->mem + mp->idx_local));
	//局稼動情報の読み込み
	getSysConstAction (mp, mp->app, (sys_kind_t *)(pt->mem + mp->idx_kind), (sys_action_t  *)(pt->mem + mp->idx_action ));
	//局名称情報の読み込み
	getSysConstName   (mp, mp->app, (sys_kind_t *)(pt->mem + mp->idx_kind), (sys_name_t    *)(pt->mem + mp->idx_name   ));
	//局補足名称情報の読み込み
	getSysConstOptName(mp, mp->app, (sys_kind_t *)(pt->mem + mp->idx_kind), (sys_optname_t *)(pt->mem + mp->idx_optname));
	//局コード情報の読み込み
	getSysConstCode   (mp, mp->app, (sys_kind_t *)(pt->mem + mp->idx_kind), (sys_code_t    *)(pt->mem + mp->idx_code   ));
//	//局構成情報１の読み込み
//	getSysConstComp1  (mp, mp->app, (sys_kind_t *)(pt->mem + mp->idx_kind), (sys_comp1_t   *)(pt->mem + mp->idx_comp1  ));
//	//局構成情報２の読み込み
//	getSysConstComp2  (mp, mp->app, (sys_kind_t *)(pt->mem + mp->idx_kind), (sys_comp2_t   *)(pt->mem + mp->idx_comp2  ));

//2002.11.19 削除（ここから）----------------------------------------
//ファイル読み込み時に更新時刻を比較するように変更した為
//	//読み込み日時の設定
//	dll_getNowTime(&mp->tim);
//2002.11.19 削除（ここまで）----------------------------------------

	return SYS_ERR_SUCCESS;
}

/*---------------------------------------------------------
*	関数：getSysConstKindInfo
*	機能：局種別毎情報の領域初期化
*	引数：pt                [IN]  局種別管理情報
*	戻値：成否
        ：     0 = 成功
        ：  以外 = 失敗
*	説明：
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

	//ロックハンドルの取得
	ret = dll_createLock(NULL, SYS_NAME_LOCK, &hLock);
	if (ret < 0)
		return SYS_ERR_LOCK_HANDLE;
	else
		pt->hLock = hLock;

	//管理領域のロック
	bln = dll_setLock(pt->hLock, SYS_LOCK_TIMEOUT);
	if (bln == FALSE) {
		freeSysConstKindInfo(pt);
		return SYS_ERR_LOCK_TIMEOUT;
	}

	//共有メモリのオープン
	ret = dll_openSharedMemory(&p, SYS_NAME_KIND);
	if (ret < 0)
		ret = createSysConstKindInfo(&p);

	//ロックの解除
	dll_releaseLock(pt->hLock);

	if (p.hd != NULL) {
		pt->p.hd  = p.hd;
		pt->p.mem = p.mem;

		//メモリの確保
		mp = (kind_sharedmem_t *)p.mem;
		psize = sizeof(kind_info_t) * SYS_MAX_KIND;
		dp = new char[psize + mp->msize];
		if (dp == NULL) {
			ret = SYS_ERR_ALLOC_MEMORY;
		} else {
			//管理情報の設定
			pt->max   = mp->max;
			pt->pKind = (kind_info_t *)dp;
			//ポインタ情報の設定
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

	//局種別情報の読み込み
	if (ret == SYS_ERR_SUCCESS)
		ret = setSysConstKindInfo(pt);

	if (ret != SYS_ERR_SUCCESS)
		freeSysConstKindInfo(pt);

	return ret;
}

/*---------------------------------------------------------
*	関数：setSysConstKindInfo
*	機能：局種別毎情報の取得
*	引数：pt                [OUT] 局種別管理情報
*	戻値：成否
        ：     0 = 成功
        ：  以外 = 失敗
*	説明：
---------------------------------------------------------*/
int setSysConstKindInfo(kind_manager_t *pt)
{
	kind_sharedmem_t *mp;

//2002.11.19 削除（ここから）----------------------------------------
//ファイルを読み込む時に更新日時を比較するように変更
//	tm24_t ntm24, rtm24;
//	time_t ntim;
//2002.11.19 削除（ここまで）----------------------------------------

	BOOL bln;
	int ret;


	//管理領域のロック
	bln = dll_setLock(pt->hLock, SYS_LOCK_TIMEOUT);
	if (bln == FALSE) {
		return SYS_ERR_LOCK_TIMEOUT;
	}

//2002.11.19 修正（ここから）----------------------------------------
//ファイルを読み込む時に更新日時を比較するように変更
//
//	//共有メモリの読み込み日時確認
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
//	//自分の読み込み時刻と共有メモリの読み込み時刻の比較
//	dll_localtime24(pt->tim, &ntm24);
//	dll_localtime24(mp->tim, &rtm24);
//	if (pt->tim == 0 || ntm24.tm_mday != rtm24.tm_mday) {
//		memcpy((char *)pt->pKind + sizeof(kind_info_t) * SYS_MAX_KIND, pt->p.mem + mp->idx_kind, mp->msize);
//		pt->tim = mp->tim;
//	}

	//ファイルの読み込み
	ret = readSysConstKindInfo(&pt->p);
	if (ret == SYS_ERR_SUCCESS) {
		//自分の領域にコピー
		mp = (kind_sharedmem_t *)pt->p.mem;
		memcpy((char *)pt->pKind + sizeof(kind_info_t) * SYS_MAX_KIND, pt->p.mem + mp->idx_kind, mp->msize);
	}
//2002.11.19 修正（ここまで）----------------------------------------

	//ロックの解除
	dll_releaseLock(pt->hLock);

	return ret;
}

/*---------------------------------------------------------
*	関数：freeSysConstKindInfo
*	機能：局種別毎情報の領域開放
*	引数：pt                [IN]  局種別管理情報
*	戻値：
*	説明：
---------------------------------------------------------*/
void freeSysConstKindInfo(kind_manager_t *pt)
{
	if (pt->p.hd  != NULL) dll_freeSharedMemory(&pt->p);
	if (pt->pKind != NULL) free(pt->pKind);
	if (pt->hLock != NULL) dll_closeLock(pt->hLock);
	memset(pt, 0, sizeof(kind_manager_t));
}

/*---------------------------------------------------------
*	関数：checkSysConstKindInfo
*	機能：システム定数即時更新チェック
*	引数：pt                [IN]  局種別管理情報
        ：dpath             [IN]  データパス
        ：pno               [IN]  ソフト番号
*	戻値：更新フラグ
        ：  TRUE  = 更新
        ：  FALSE = 未更新
*	説明：
---------------------------------------------------------*/
BOOL checkSysConstKindInfo(kind_manager_t *pt, char *dpath, int pno)
{
	char fpath[_MAX_PATH];
	struct _stat st;
	BOOL bln;
	int ret;

	//通知ファイル名の設定
	sprintf(fpath, "%sprogram\\notice_%02d.txt", dpath, pno);
	//通知ファイルの存在を確認
	ret = _stat(fpath, &st);
	if (ret == 0) {
		bln = TRUE;
	} else {
		bln = FALSE;
	}

	//通知ファイルあり
	if (bln == TRUE) {
		//再読込み
		setSysConstKindInfo(pt);
		//通知ファイルの削除
		remove(fpath);
	}
	return bln;
}

/*---------------------------------------------------------
*	関数：getSysConstExio
*	機能：装置間通信情報読み込み
*	引数：app               [IN]  アプリケーションパス
        ：syskind           [IN]  処理種別
        ：officeNo          [IN]  事務所番号
        ：unitNo            [IN]  装置番号
        ：houroNo           [IN]  方路番号
        ：p                 [OUT] 装置間通信情報
*	戻値：成否
        ：     0 = 成功
        ：  以外 = 失敗
*	説明：
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


	//戻り値の初期化
	p->cnt = 0;
	p->dp = NULL;

 	//ファイルオープン
	sprintf(fpath, "%ssys_table\\csv\\%s_exio_%02d%02d%02d.csv", app, syskind, officeNo, unitNo, houroNo);
	fp = fopen(fpath, "r");
	if (fp == NULL) {
		return -1;
	}
	//件数の取得
	for (cnt = 0; !feof(fp); cnt++)
		csvread_n(fp, wk, sizeof(wk));
	cnt = cnt - 7;
	//先頭に戻す
	pos = 0;
	fsetpos(fp, &pos);

	//領域の取得
	p->dp = new sys_exio_t[cnt];
	if (p->dp == NULL) {
		fclose(fp);
		return -1;
	}

	//１行目
	csvread_n(fp, wk, sizeof(wk));
	//事務所番号
	csvsplit_n(wk, 3, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.officeNo = no;
	//受信PORT
	csvsplit_n(wk, 5, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.r_port = no;
	//１分値出力
	csvsplit_n(wk, 10, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.ioflg_1m = no;

	//２行目
	csvread_n(fp, wk, sizeof(wk));
	//装置番号
	csvsplit_n(wk, 3, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.unitNo = no;
	//送信先PORT
	csvsplit_n(wk, 5, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.s_port = no;
	//ラグタイム１分START
	csvsplit_n(wk, 7, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.rag_1m.st = no;
	//ラグタイム１分END
	csvsplit_n(wk, 8, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.rag_1m.et = no;
	//定時出力
	csvsplit_n(wk, 10, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.ioflg_hm = no;

	//３行目
	csvread_n(fp, wk, sizeof(wk));
	//方路番号
	csvsplit_n(wk, 3, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.houroNo = no;
	//送信先ＩＰ
	csvsplit_n(wk, 5, dt, sizeof(dt));
	strcpy(p->hd.s_ip, dt);
	//ラグタイム正定時START
	csvsplit_n(wk, 7, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.rag_hm.st = no;
	//ラグタイム正定時END
	csvsplit_n(wk, 8, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.rag_hm.et = no;
	//日集計出力
	csvsplit_n(wk, 10, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.ioflg_dy = no;

	//４行目
	csvread_n(fp, wk, sizeof(wk));
	//事務所名
	csvsplit_n(wk, 3, dt, sizeof(dt));
	strcpy(p->hd.myOfficeName, dt);
	//送信先事務所名
	csvsplit_n(wk, 5, dt, sizeof(dt));
	strcpy(p->hd.toOfficeName, dt);
	//ラグタイム日集計START
	csvsplit_n(wk, 7, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.rag_dy.st = no;
	//ラグタイム日集計END
	csvsplit_n(wk, 8, dt, sizeof(dt));
	no = atoi(dt);
	p->hd.rag_dy.et = no;

	//５行目
	csvread_n(fp, wk, sizeof(wk));
	//装置名
	csvsplit_n(wk, 3, dt, sizeof(dt));
	strcpy(p->hd.myUnitName, dt);
	//送信先装置名
	csvsplit_n(wk, 5, dt, sizeof(dt));
	strcpy(p->hd.toUnitName, dt);

	//６行目
	csvread_n(fp, wk, sizeof(wk));
	//７行目
	csvread_n(fp, wk, sizeof(wk));

	//８行目以降
	i = 0;
	csvread_n(fp, wk, sizeof(wk));
	while ( !feof(fp) ) {
		//入出力種別コード
		csvsplit_n(wk, 0, dt, sizeof(dt));
		no = atoi(dt);
		if (no == SYS_EXIO_KIND_IN || no == SYS_EXIO_KIND_OUT) {
			p->dp[i].io_kind = no;
			//入出力局番
			csvsplit_n(wk, 1, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].io_sno = no;
			//入出力項番
			csvsplit_n(wk, 2, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].io_ino = no;
			//入出力管理事務所番号
			csvsplit_n(wk, 3, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].io_office = no;
			//入出力データ種別番号
			csvsplit_n(wk, 4, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].io_data = no;
			//倍長コード
			csvsplit_n(wk, 5, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].code = no;
			//ファイル種別
			csvsplit_n(wk, 6, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].filetype = no;
			//局種別コード
			csvsplit_n(wk, 7, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].kno = no;
			//内部局番
			csvsplit_n(wk, 8, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].sno = no;
			//データ項目順番
			csvsplit_n(wk, 9, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].ino = no;
			//スケール補正コード
			csvsplit_n(wk, 10, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].scale = no;
			//予備
			csvsplit_n(wk, 11, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].reserve1 = no;
			//予備
			csvsplit_n(wk, 12, dt, sizeof(dt));
			no = atoi(dt);
			p->dp[i].reserve2 = no;
			i++;
		}
		//次の行
		csvread_n(fp, wk, sizeof(wk));
	}
	//ファイルクローズ
	fclose(fp);

	p->cnt = i;
	return 0;
}

