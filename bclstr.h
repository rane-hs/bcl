#ifndef _MY_STR_LIB_H_
#define _MY_STR_LIB_H_


#include <string>
#include <vector>
#include <cstdio>
#include <cstdarg>
#include <cassert>
#include <fstream>
#include <stdexcept>
#if defined(_MSC_VER) && (_MSC_VER > 1200)	// > VC6
#include <boost/lexical_cast.hpp>
#endif

namespace bcl{

#if defined(_MSC_VER) && (_MSC_VER > 1200)	// > VC6
	typedef wchar_t	char_t;
	typedef std::wstring	str_t;
#else
	typedef char	char_t;
	typedef std::string	str_t;
#endif

//置換ラッパ
//s内のd1をd2で置換して返す
template<class E, class T, class A>
const bool replace(
	std::basic_string<E,T,A> &s,
	const std::basic_string<E,T,A> &d1, 
	const std::basic_string<E,T,A> &d2)
{
	bool ret = false;
	size_t pos;
	while(std::string::npos != (pos = s.find(d1)) ){
	s.replace(pos, d1.length(), d2);
		ret = true;
	}
	return ret;
}
inline const bool replace(std::string &s, const char *d1, const char *d2){
	return replace(s, std::string(d1), std::string(d2));
}
inline const bool replace(std::wstring &s, const wchar_t *d1, const wchar_t *d2){
	return replace(s, std::wstring(d1), std::wstring(d2));
}

/********************************************************************
  クラス：CSplit
  機能  ：文字列分割
  説明  ：
********************************************************************/

template <class T>
class basic_split :protected std::vector<T>{
	typedef T type;
	typedef std::vector<T> array_type;
#ifdef _MSC_VER
	typedef array_type::const_iterator c_it;
#else
	typedef typename array_type::const_iterator c_it;
#endif
	
	using array_type::clear;
	using array_type::empty;
	using array_type::begin;
	using array_type::end;
public:
	//using
	using array_type::at;
	using array_type::size;
	using array_type::operator[];

	/********************************************************************
	  機能  ：コンストラクタ
	********************************************************************/
	basic_split(){};
	~basic_split(){clear();};

	/********************************************************************
	  機能  ：コンストラクタ
	  引数  ：s                  [IN]  文字列
			：d                  [IN]  分割文字列
	********************************************************************/
	basic_split(const T &s, const T &d){	tokenize(s, d);};

	const T c_at(const size_t &n) const{
		return at(n);
	}
	const T c_at_s(const size_t &n) const{
		if(n >= size() ) return T("");
		else	return at(n);
	}
	/********************************************************************
	  関数  ：split
	  機能  ：文字列分割
	  引数  ：s                  [IN]  文字列
	        ：d                  [IN]  分割文字列
	        ：
	  戻り値：分割数
	  説明  ：
	********************************************************************/
	const int tokenize(const T &s, const T &d)
	{
		int idx_s;
		int idx_e;
		T wk;

		//クリア
		clear();

		//文字列が空
		if (s.length() < 1)
			return 0;

		//分割
		idx_s = 0;
		idx_e = -1;
		do {
			idx_s = idx_e + 1;
			idx_e = s.find_first_of(d, idx_s);
			wk = s.substr(idx_s, idx_e - idx_s);
			push_back(wk);
		} while (idx_e != std::string::npos);

		return size();
	}
	const T search(const T &s)
	{
		c_it	searchIt;
		if(empty() ) return "";
		for(searchIt = begin(); searchIt != end(); ++searchIt){
			if(searchIt->find(s) != T::npos)
				return *searchIt;
		}
		return "";
	}
};

typedef basic_split<std::string> split;
typedef basic_split<std::wstring> wsplit;

//string <-> wstring conversion
//from : http://d.hatena.ne.jp/kryozahiro/20080809/1218295912
//正しく変換されるには、mainの初期化処理内などで
// setlocale(LC_CTYPE, "");
//と打ち込みましょう

//ワイド文字列からマルチバイト文字列
//ロケール依存
inline const std::string narrow(const std::wstring &src) {
	std::string dest;
	char *mbs = new char[src.length() * MB_CUR_MAX + 1];
#if (_MSC_VER > 1300)		//VC2003-
	size_t	conved_length;
	wcstombs_s(&conved_length, mbs, src.length(), src.c_str(), src.length() * MB_CUR_MAX + 1);
#else
	wcstombs(mbs, src.c_str(), src.length() * MB_CUR_MAX + 1);
#endif
	dest = mbs;
	delete [] mbs;
	return dest;
}

//マルチバイト文字列からワイド文字列
//ロケール依存
inline const std::wstring widen(const std::string &src) {
	std::wstring dest;
	wchar_t *wcs = new wchar_t[src.length() + 1];
#if (_MSC_VER > 1300)		//VC2003-
	size_t	conved_length;
	mbstowcs_s(&conved_length, wcs, src.length(), src.c_str(), src.length() + 1);
#else
	mbstowcs(wcs, src.c_str(), src.length() + 1);
#endif
	dest = wcs;
	delete [] wcs;
	return dest;
}

inline const std::string format(const char* const format, ...)
{
	va_list     arguments;
	std::string buffer;

	assert(format);
	va_start(arguments, format);
#if defined _MSC_VER
	
#if(_MSC_VER <= 1300)		//VC1-VC6
	char	wk[8192];
	try{
		int length = vsprintf(wk, format, arguments) ;
		if(length < 0)   throw std::runtime_error(format);
		buffer.assign(length+1,'\0');
		int result = vsprintf(&buffer[0], format, arguments);
		if(result < 0)   throw std::runtime_error(format);
		buffer.erase(length,1);
	} catch(...) {
		va_end(arguments);
		throw;
	}
	va_end(arguments);
	return buffer;
}

#elif(_MSC_VER > 1300)		//VC2003-
	try{
		const int length = _vscprintf(format, arguments) ;
		if(length < 0)   throw std::runtime_error(format);
		buffer.assign(length+1,'\0');
		int result = vsprintf_s(&buffer[0], length+1, format, arguments);
		if(result < 0)   throw std::runtime_error(format);
		buffer.erase(length,1);
	} catch(...) {
		va_end(arguments);
		throw;
	}
	va_end(arguments);
	return buffer;
}
#endif
#else 		//_MSC_VER
	char	wk[8192];
	try{
		int length = vsprintf(wk, format, arguments) ;
		if(length < 0)   throw std::runtime_error(format);
		buffer.assign(length+1,'\0');
		int result = vsprintf(&buffer[0], format, arguments);
		if(result < 0)   throw std::runtime_error(format);
		buffer.erase(length,1);
	} catch(...) {
		va_end(arguments);
		throw;
	}
	va_end(arguments);
	return buffer;
}
#endif		//_MSC_VER

inline const std::wstring format(const wchar_t* const format, ...)
{
	va_list     arguments;
	std::wstring buffer;

	assert(format);
	va_start(arguments, format);
	
	try{
		const int length = _vscwprintf(format, arguments) ;
		if(length < 0)   throw std::runtime_error(bcl::narrow(format));
		buffer.assign(length+1,'\0');
		int result = vswprintf_s(&buffer[0], length+1, format, arguments);
		if(result < 0)   throw std::runtime_error(bcl::narrow(format));
		buffer.erase(length,1);
	} catch(...) {
		va_end(arguments);
		throw;
	}
	va_end(arguments);
	return buffer;
}

template <class Str>
inline const Str substr(const Str &s, size_t p, size_t m)
{
	std::string wk;
	try {
		wk = s.substr(p, m);
	}
	catch (...) {
		wk = "";
	}
	return wk;
};

template <class Str>
inline const Str rm_crlf(const Str &s)
{
	Str wk = L"";

	if (s.empty() == true)
		return wk;

	wk = s;
	const size_t idx_crlf = s.find_last_not_of("\r\n");
	if (idx_crlf != Str::npos)
		wk = s.substr(0, idx_crlf+1);
	return wk;
};

/*
	トリム
*/
template <class Str>
inline const Str trim(const Str &s, const Str &t)
{
	int idx_s;
	int idx_e;
	Str wk = "";

	if (s.empty() == true)
		return wk;

	idx_s = s.find_first_not_of(t);
	idx_e = s.find_last_not_of(t);
	if (idx_s != Str::npos)
		wk = s.substr(idx_s, idx_e - idx_s + 1);
	return wk;
};

/*
	文字列の矩形化
	assert(rectangled("123456789", 3) == "123\n456\n789")
*/
template <class Str_>
const Str_ squarize_line(const Str_ &s, const size_t &width)
{
	return (s.length() <= width)?
		s:
		s.substr(0, width) + L"\n" + squarize_line(s.substr(width), width);
}

template <class Str_>
const Str_ rectangled(const Str_ &s, const size_t &width)
{
	if(s.length() < 1) return s;

	bcl::basic_split<Str_> sp(s, L"\n");
	Str_ out = squarize_line(sp.at(0), width);
	for(size_t i=1;i<sp.size(); i++)
	{
		out += squarize_line(sp.at(0), width);
		out += L"\n";
	}
	return out;
}

inline const std::string ReadFileString(const std::string &fpath){
	std::ifstream fs(fpath.c_str());
	std::string	dest = "";

	if (!fs.is_open()){
		return "";
	}
	if (fs.fail()){
		return "";
	}

	try {
		std::getline(fs, dest);
	} catch (...) {
		//ファイルクローズ
		fs.close();
		return "";
	}
	//ファイルクローズ
	fs.close();

	return dest;
}

/*
 文字列<- -> 数値　相互変換処理（撤廃予定）
 VC2005以降では、boost::lexical_castを用いること。

 boost::lexical_castではキャストに失敗すると例外が飛ぶので
 例外を投げないlexical_cast_s<T>などの定義をしてもいいかも知れない
	案：bcl::lexical_cast_s<T,N>
	bcl::lexical_cast_s<int,-1>("5");	//-1が帰ってきたらエラー！
*/
/*!------------------------------------------------------------------
	to value conversion
-------------------------------------------------------------------*/
template <typename T>
inline const T lexical_cast(const std::string &s){
	return (s.length() < 1)?
		0:
		static_cast<T>(atol(s.c_str()));
}
template <typename T>
inline const T lexical_cast(const std::wstring &s){
	return (s.length() < 1)?
		0:
		static_cast<T>(_wtol(s.c_str()));
}
template <typename T>
inline const T to_float(const std::string &s){
	return (s.length() < 1)?
		0.0:
		static_cast<T>(atof(s.c_str()));
}

template <typename T, typename S>
const T lexical_cast_s(const S &src, T err) {
	try{
		const T dest = boost::lexical_cast<T>(S);
	}catch(const boost::bad_lexical_cast &e){
		return err;
	}catch(...){
		return err;
	}
	return dest;
}
/*!------------------------------------------------------------------
	to string conversion
-------------------------------------------------------------------*/
inline const std::string to_string(const int &a){	return bcl::format("%d", a); }
inline const std::string to_string(const long &a){	return bcl::format("%ld", a); }
inline const std::string to_string(const float &a){	return bcl::format("%f", a); }
inline const std::string to_string(const double &a){	return bcl::format("%lf", a); }
inline const std::string to_string(const long double &a){	return bcl::format("%lf", a); }


#ifdef __AFXWIN_H__	//mfc conversion

const std::string Olettos(const COleDateTime s);
const COleDateTime stoOlet(const std::string &s);

#endif //__AFXWIN_H__

} //bcl::

#endif // _MY_STR_LIB_H_
