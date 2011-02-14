#include "stdafx.h"
#include "indexfile.h"
#include <fstream>
M__USING_BCL

/*!-------------------------------------------------------
*	機能：Incremental index
*	引数：
*	戻り：
*	備考：
*	履歴：[新規]2009-06-23 H.Seki
*		：[変更]xxxx-xx-xx xxx
--------------------------------------------------------*/
index_T::operator++(int){		//(int) が後置「n++」()にすると前置「++n」
	idx++;
	if(idx > 9999) idx = 1;
	return idx;
};
#if 0
/*!-------------------------------------------------------
*	機能：インデックス代入処理
*	引数：new_idx	[IN]
*	戻り：
*	備考：
*	履歴：[新規]2009-06-23 H.Seki
*		：[変更]xxxx-xx-xx xxx
--------------------------------------------------------*/
index_T::operator=(const short new_idx){
	idx = new_idx;
};
#endif
//チェック処理
//true：変更なし
//false：制定処理を行った。
bool index_T::Check0(){
	bool	bRet = true;
	if(idx > 9999){
		idx %= 10000;	// 0 - 9998
		idx++;			// 1 - 9999
		bRet = false;
	}else if(idx < 0){
		idx = 1;
		bRet = false;
	}
	return bRet;
}
bool index_T::Check1(){
	bool	bRet = true;
	if(idx > 9999){
		idx %= 10000;	// 0 - 9998
		idx++;			// 1 - 9999
		bRet = false;
	}else if(idx < 1){
		idx = 1;
		bRet = false;
	}
	return bRet;
}
/*-------------------------------------------------------------------
*	機能：idxデータを読み込む
*	引数：fpath		[IN]
*	戻り：インデックス
*	履歴：[新規]2007-11-16 H.Seki
*		：[変更]xxxx-xx-xx xxx
-------------------------------------------------------------------*/
int index_T::ReadIdx(){
	std::ifstream fs;
	int		out_idx=0;
	idx = 0;

	//インデックスの読み込み（バイナリ）
	fs.open(idxPath.c_str(), std::ios::binary);
	if (fs.is_open() == false){
		return -1;
	}
	if (fs.fail()){
		return -3;
	}

	try {
		fs.read((char *)&out_idx, sizeof(int));
	} catch (...) {
		//ファイルクローズ
		fs.close();
		return -2;
	}
	//ファイルクローズ
	fs.close();

	idx = out_idx;

	return 0;
}
/*-------------------------------------------------------------------
*	機能：idxデータを書き込む
*	引数：fpath		[IN]
*		：out_idx	[IN]
*	戻り：0=成功、負=失敗
*	履歴：[新規]2007-11-16 H.Seki
*		：[変更]xxxx-xx-xx xxx
-------------------------------------------------------------------*/
int index_T::WriteIdx(){
	std::ofstream fs;

	//インデックスの読み込み（バイナリ）
	fs.open(idxPath.c_str(), std::ios::binary);
	if (fs.is_open() == false){
		return -1;
	}

	try {
		int out_idx = idx;
		fs.write((char *)&out_idx, sizeof(int));
	} catch (...) {
		//ファイルクローズ
		fs.close();
		return -2;
	}
	//ファイルクローズ
	fs.close();

	return 0;
}
