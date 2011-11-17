#ifndef _BCL_STR_H_
#define _BCL_STR_H_


#include <string>
#include <vector>
#include <cstdio>
#include <cstdarg>
#include <cassert>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <locale>

namespace bcl{

#ifdef _UNICODE	// > VC6
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
public:
#if defined(_MSC_VER) && (_MSC_VER > 1200)	// > VC6
	using array_type::cbegin;
	using array_type::cend;
	using array_type::const_iterator;
#endif
	//using
	using array_type::begin;
	using array_type::end;
	using array_type::empty;
	using array_type::at;
	using array_type::size;
	using array_type::iterator;
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
	const size_t tokenize(const T &s, const T &d)
	{
		//文字列が空
		if (s.length() < 1){	push_back(s);	return 0; 		}
		size_t idx_e = s.find(d);

		if (idx_e == T::npos){	push_back(s);	return 0; 		}

		size_t idx_s;

		T wk;

		//クリア
		clear();

		//分割
		idx_s = 0;
		idx_e = std::string::npos;
		do {
			idx_s = idx_e + d.length();
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
	T const ListString(const T &separator) {
		if(empty() )return "";
		const_iterator it = cbegin();
		T dest = (*it);
		++it;
		for(;
			it != cend() ; ++it){
			dest += (separator + (*it));
		}
		return dest;
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
	wcstombs_s(&conved_length, mbs, src.length() * MB_CUR_MAX + 1, src.c_str(), src.length() * MB_CUR_MAX + 1);
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
# ifdef _CRT_SECURE_NO_DEPRECATE
	mbstowcs(wcs, src.c_str(), src.length() + 1);
# else
	size_t	conved_length;
	mbstowcs(wcs, src.c_str(), src.length() + 1);
# endif
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

struct equal_char_ignorecase {
  bool operator()(char x, char y) const {
    std::locale loc;
    return std::tolower(x,loc) == std::tolower(y,loc);
  }
  bool operator()(wchar_t x, wchar_t y) const {
    std::locale loc;
    return std::tolower(x,loc) == std::tolower(y,loc);
  }
};

inline bool icomp(const std::string& x, const std::string& y) 
{
  return (x.size() == y.size()) && std::equal(x.begin(), x.end(), y.begin(), equal_char_ignorecase());
}
inline bool icomp(const std::wstring& x, const std::wstring& y) 
{
  return (x.size() == y.size()) && std::equal(x.begin(), x.end(), y.begin(), equal_char_ignorecase());
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
inline const Str trim(const Str &s, const Str &t1, const Str &t2)
{
	int idx_s;
	int idx_e;
	Str wk;

	if (s.empty() == true)
		return wk;

	idx_s = s.find_first_not_of(t1);
	idx_e = s.find_last_not_of(t2);
	if (idx_s != Str::npos)
		wk = s.substr(idx_s, idx_e - idx_s + 1);
	return wk;
};

template <class Str>
inline const Str trim(const Str &s, const Str &t)
{
	return trim<Str>(s, t, t);
};

inline const std::string trim(const std::string &s, const char *t){ return trim<std::string>(s, std::string(t));}
inline const std::string trim(const std::string &s, const char *t1, const char *t2){
	return trim<std::string>(s, std::string(t1), std::string(t2));
}
inline const std::wstring trim(const std::wstring &s, const wchar_t *t){ return trim<std::wstring>(s, std::wstring(t));}
inline const std::wstring trim(const std::wstring &s, const wchar_t *t1, const wchar_t *t2){ 
	return trim<std::wstring>(s, std::wstring(t1), std::wstring(t2));
}
/*
	判定(数字)
*/
template <class Str>
const bool is_numeric(const Str &s)
{
	//数字検索
#ifdef _UNICODE
	size_t pos = s.find_first_of(L"0123456789");
#else
	size_t pos = s.find_first_of("0123456789");
#endif
	//数字がない
	if ( pos == Str::npos ) {
		return false;
	}

	//文字列検索
#ifdef _UNICODE
	pos = s.find_first_not_of(L"+-.0123456789");
#else
	pos = s.find_first_not_of("+-.0123456789");
#endif
	//数字以外がある
	if ( pos != Str::npos ) {
		return false;
	}

	return true;
}
/*
	文字列の矩形化
	assert(rectangled("123456789", 3) == "123\n456\n789")
*/
template <class Str_>
const Str_ squarize_line(const Str_ &s, const size_t &width)
{
	return (s.length() <= width)?
		s:
		s.substr(0, width) + TEXT("\n") + squarize_line(s.substr(width), width);
}

template <class Str_>
const Str_ rectangled(const Str_ &s, const size_t &width)
{
	if(s.length() < 1) return s;

	bcl::basic_split<Str_> sp(s, TEXT("\n"));
	Str_ out = squarize_line(sp.at(0), width);
	for(size_t i=1;i<sp.size(); i++)
	{
		out += squarize_line(sp.at(0), width);
		out += TEXT("\n");
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

template <class Str>
Str const ListString(const std::vector<Str> &src, const Str &separator) {
	if(src.empty() )return "";
	std::vector<Str>::const_iterator it = src.cbegin();
	Str dest = (*it);
	++it;
	for(;
		it != src.cend() ; ++it){
		dest += (separator + (*it));
	}
	return dest;
}
template <class Str, class Itr>
Str const ListString(const Itr &first, const Itr &end, const Str &separator) {
	if(first == end)return "";
	Itr it = first;
	Str dest = (*it);
	++it;
	for(;
		it != end ; ++it){
		dest += (separator + (*it));
	}
	return dest;
}

} //bcl::

#endif // _BCL_STR_H_
