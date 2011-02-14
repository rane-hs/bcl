#include "stdafx.h"
#include "indexfile.h"
#include <fstream>
M__USING_BCL

/*!-------------------------------------------------------
*	�@�\�FIncremental index
*	�����F
*	�߂�F
*	���l�F
*	�����F[�V�K]2009-06-23 H.Seki
*		�F[�ύX]xxxx-xx-xx xxx
--------------------------------------------------------*/
index_T::operator++(int){		//(int) ����u�un++�v()�ɂ���ƑO�u�u++n�v
	idx++;
	if(idx > 9999) idx = 1;
	return idx;
};
#if 0
/*!-------------------------------------------------------
*	�@�\�F�C���f�b�N�X�������
*	�����Fnew_idx	[IN]
*	�߂�F
*	���l�F
*	�����F[�V�K]2009-06-23 H.Seki
*		�F[�ύX]xxxx-xx-xx xxx
--------------------------------------------------------*/
index_T::operator=(const short new_idx){
	idx = new_idx;
};
#endif
//�`�F�b�N����
//true�F�ύX�Ȃ�
//false�F���菈�����s�����B
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
*	�@�\�Fidx�f�[�^��ǂݍ���
*	�����Ffpath		[IN]
*	�߂�F�C���f�b�N�X
*	�����F[�V�K]2007-11-16 H.Seki
*		�F[�ύX]xxxx-xx-xx xxx
-------------------------------------------------------------------*/
int index_T::ReadIdx(){
	std::ifstream fs;
	int		out_idx=0;
	idx = 0;

	//�C���f�b�N�X�̓ǂݍ��݁i�o�C�i���j
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
		//�t�@�C���N���[�Y
		fs.close();
		return -2;
	}
	//�t�@�C���N���[�Y
	fs.close();

	idx = out_idx;

	return 0;
}
/*-------------------------------------------------------------------
*	�@�\�Fidx�f�[�^����������
*	�����Ffpath		[IN]
*		�Fout_idx	[IN]
*	�߂�F0=�����A��=���s
*	�����F[�V�K]2007-11-16 H.Seki
*		�F[�ύX]xxxx-xx-xx xxx
-------------------------------------------------------------------*/
int index_T::WriteIdx(){
	std::ofstream fs;

	//�C���f�b�N�X�̓ǂݍ��݁i�o�C�i���j
	fs.open(idxPath.c_str(), std::ios::binary);
	if (fs.is_open() == false){
		return -1;
	}

	try {
		int out_idx = idx;
		fs.write((char *)&out_idx, sizeof(int));
	} catch (...) {
		//�t�@�C���N���[�Y
		fs.close();
		return -2;
	}
	//�t�@�C���N���[�Y
	fs.close();

	return 0;
}
