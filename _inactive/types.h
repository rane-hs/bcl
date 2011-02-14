#include "bcl.h"
#include <string>

M__BEGIN_NAMESPACE_BCL

typedef enum read_ret{
	read_success = 1,		//読み込み成功
	open_failure,	//オープン失敗
	read_failure,	//読み込み失敗
	data_err,		//内部データ不正
	read_out_term
}read_code_t;
typedef enum write_ret{
	write_success = 1,		//書き込み成功
	file_aleady_exists,	//ファイルが存在するので終了
	write_open_error,	//書き込み用として開くのに失敗
	file_write_failure,	//ファイル書き込み失敗
	write_code_term
}write_code_t;

inline std::string WriteCodeStr(int s){
	switch(s){
		case write_success:			return "書き込み成功";
		case file_aleady_exists:	return "ファイルが存在するので終了";
		case write_open_error:		return "書き込み用として開くのに失敗";
		case file_write_failure:	return "ファイル書き込み失敗";
		default:	return "不明なコード";
	}
};
inline std::string ReadCodeStr(int s){
	switch(s){
		case read_success:	return "読み込み成功";
		case open_failure:	return "オープン失敗";
		case read_failure:	return "読み込み失敗";
		case data_err:	return "内部データ不正";
		default:	return "不明なコード";
	}
};

M__END_NAMESPACE_BCL
