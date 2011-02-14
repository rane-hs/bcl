#include "bcl.h"
#include <string>

M__BEGIN_NAMESPACE_BCL

typedef enum read_ret{
	read_success = 1,		//�ǂݍ��ݐ���
	open_failure,	//�I�[�v�����s
	read_failure,	//�ǂݍ��ݎ��s
	data_err,		//�����f�[�^�s��
	read_out_term
}read_code_t;
typedef enum write_ret{
	write_success = 1,		//�������ݐ���
	file_aleady_exists,	//�t�@�C�������݂���̂ŏI��
	write_open_error,	//�������ݗp�Ƃ��ĊJ���̂Ɏ��s
	file_write_failure,	//�t�@�C���������ݎ��s
	write_code_term
}write_code_t;

inline std::string WriteCodeStr(int s){
	switch(s){
		case write_success:			return "�������ݐ���";
		case file_aleady_exists:	return "�t�@�C�������݂���̂ŏI��";
		case write_open_error:		return "�������ݗp�Ƃ��ĊJ���̂Ɏ��s";
		case file_write_failure:	return "�t�@�C���������ݎ��s";
		default:	return "�s���ȃR�[�h";
	}
};
inline std::string ReadCodeStr(int s){
	switch(s){
		case read_success:	return "�ǂݍ��ݐ���";
		case open_failure:	return "�I�[�v�����s";
		case read_failure:	return "�ǂݍ��ݎ��s";
		case data_err:	return "�����f�[�^�s��";
		default:	return "�s���ȃR�[�h";
	}
};

M__END_NAMESPACE_BCL
