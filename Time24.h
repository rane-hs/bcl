#ifndef __BCL_TIME24_H__
#define __BCL_TIME24_H__

#include "bclstr.h"
#include <clx/time.h>

namespace bcl{	//bcl::

#define TIME_YMDHNS "%Y/%m/%d %H:%M:%S"
#define TIME_YMDHN "%Y/%m/%d %H:%M"
#define TIME_YMD "%Y/%m/%d"

#define TIME24_YMDHNS "%24Y/%24M/%24D %24H:%M:%S"
#define TIME24_YMDHN "%24Y/%24M/%24D %24H:%M"
#define TIME24_YMD "%24Y/%24M/%24D"

#define TIME24_YMDHNS_U "%24Y%24M%24D%24H%M%S"
#define TIME24_YMDHN_U  "%24Y%24M%24D%24H%M"
#define TIME24_YMD_U    "%24Y%24M%24D"

#define TIME24_YMDHN_HYPHEN "%24Y-%24m-%24d %24H:%M:%S"

#define TIME24_YMDHNS_NO_ZERO "%24Y/%24m/%24d %24H:%M:%S"
#define TIME24_YMDHN_NO_ZERO "%24Y/%24m/%24d %24H:%M"
#define TIME24_YMD_NO_ZERO "%24Y/%24m/%24d"

#ifdef _UNICODE
#define TIME_YMDHNS_W L"%Y/%m/%d %H:%M:%S"
#define TIME_YMDHN_W L"%Y/%m/%d %H:%M"
#define TIME_YMD_W L"%Y/%m/%d"

#define TIME24_YMDHNS_W L"%24Y/%24M/%24D %24H:%M:%S"
#define TIME24_YMDHN_W L"%24Y/%24M/%24D %24H:%M"
#define TIME24_YMD_W L"%24Y/%24M/%24D"

#define TIME24_YMDHNS_U_W L"%24Y%24M%24D%24H%M%S"
#define TIME24_YMDHN_U_W  L"%24Y%24M%24D%24H%M"
#define TIME24_YMD_U_W    L"%24Y%24M%24D"

#define TIME24_YMDHN_HYPHEN_W L"%24Y-%24m-%24d %24H:%M:%S"

#define TIME24_YMDHNS_NO_ZERO_W L"%24Y/%24m/%24d %24H:%M:%S"
#define TIME24_YMDHN_NO_ZERO_W L"%24Y/%24m/%24d %24H:%M"
#define TIME24_YMD_NO_ZERO_W L"%24Y/%24m/%24d"
#endif

typedef clx::date_time clx_time;
/*!-------------------------------------------------------------------
 * class time24
 * �����������s���N���X
-------------------------------------------------------------------*/
/*!-------------------------------------------------------------------
 * [constructor]
 *   time24(time_t) : time_t �^�ŏ�����
 *   time24() : ���ݎ����ŏ�����
 *   time24(time24) : time24 �ŏ�����
 *   time24(y,m,d,h,n,s) : �N���������ŏ�����
-------------------------------------------------------------------*/
/*!-------------------------------------------------------------------
 * int year, month, day, hour, minute, second : �N���������b��Ԃ�
 * int year24,month24,day24,hour24 : 24�����̔N��������Ԃ�
-------------------------------------------------------------------*/
/*!-------------------------------------------------------------------
 * time_t c_time : time_t�^�̒l��Ԃ�
-------------------------------------------------------------------*/
/*!-------------------------------------------------------------------
 * time24 ymd_duration, time_duration : (y,m,d) or (h,n,s) �ŉ�����time24��Ԃ�
 * time24 set_*_duration() : (y,m,d) or (h,n,s) �������ĕԂ�
-------------------------------------------------------------------*/
/*!-------------------------------------------------------------------
 * bool is_valid() : ���펞���Ȃ�΁Atrue��Ԃ�
-------------------------------------------------------------------*/
/*!-------------------------------------------------------------------
 * bool operator< �E�Ӓl�̕����V���������Ȃ�΁Atrue
 * bool operator> ���Ӓl�̕����V���������Ȃ�΁Atrue
 *      operator<= , operator>= �����l
 * time24 operator= ���
-------------------------------------------------------------------*/

//TODO: lexical_cast�ɓ������悤�ɏC��������i�R�s�[ctor�ȂǂȂǁj

//time24 : 
class time24 : public clx_time
{
	typedef int value_type;
	//functions
	bool const is24() const{
		return ( (hour() == 0) && (minute() == 0) && (second() == 0) );
	}
public:
	//public�p�����Ă�Ȃ�Ӗ��ˁ[������Ă���using
	using clx_time::year;
	using clx_time::month;
	using clx_time::day;
	using clx_time::hour;
	using clx_time::minute;
	using clx_time::second;
	using clx_time::totaldays;
	using clx_time::to_string;
	using clx_time::from_string;
	using clx_time::after;
	using clx_time::before;

	//ctors
	//�f�t�H���g�Ō��ݎ����ɂ͂��Ȃ��I
	explicit time24(const time_t &s=0):clx_time(s){}
	explicit time24(const clx_time &s):clx_time(s){}
	explicit time24(const char *s):clx_time(from_string(s,"%Y/%m/%d %H:%M:%S")){}
	explicit time24(const char *s, const char *f):clx_time(from_string(s, f)){}
	//const�Q�Ƃ���Ȃ��������ǂȂ�łȂ񂾂낤
/*	time24(const value_type &yy, const value_type &mm, const value_type &dd,
		 const value_type &hh, const value_type &nn, const value_type &ss)
*/	time24(value_type yy, value_type mm, value_type dd, value_type hh, value_type nn, value_type ss)
		:clx_time(yy,mm,dd,hh,nn,ss)
	{
	}

	const time24 set_now(){
		reset(); return *this;
	}

#ifdef _UNICODE
	const std::string to_string24(const std::string &desc=TIME24_YMDHNS) const
#else
	const std::string to_string24(const std::string &desc=TIME24_YMDHNS) const
#endif
	{
		if(!is_valid() ) return "";
		std::string o = desc;
		bcl::replace(o, "%24Y", bcl::format("%04d", year24()).c_str());
		bcl::replace(o, "%24y", bcl::format("%02d", (year24()%100)).c_str());
		bcl::replace(o, "%24M", bcl::format("%02d", month24()).c_str());
		bcl::replace(o, "%24m", bcl::format("%2d", month24()).c_str());
		bcl::replace(o, "%24D", bcl::format("%02d", day24()).c_str());
		bcl::replace(o, "%24d", bcl::format("%2d", day24()).c_str());
		bcl::replace(o, "%24H", bcl::format("%02d", hour24()).c_str());
		bcl::replace(o, "%24h", bcl::format("%2d", hour24()).c_str());
		return to_string(o.c_str());
	}
	const time24 from_string(const std::string &src)
	{
		from_string(src.c_str(), TIME_YMDHNS);
		return *this;
	}

	//operators
	const time24 operator+=(const clx::time_duration &rd)
	{
		return *this = time24(c_time() + rd.c_time());
	}
	const time24 operator-=(const clx::time_duration &rd)
	{
		return *this = time24(c_time() - rd.c_time());
	}

	//accessors
	time24 const yesterday() const
	{
		//c_time���o�R���Ȃ��ł�����悤�ɂȂ�ׂ�
		return time24(c_time() - clx::days(1).c_time());
	}
	value_type const hour24() const
	{
		return (is24())?
			24 :
			hour();
	}

	value_type const day24() const
	{
		return (is24())?
			yesterday().day() :
			day();
	}

	value_type const month24() const
	{
		return (is24())?
			yesterday().month() :
			month();
	}

	value_type const year24() const
	{
		return (is24())?
			yesterday().year() :
			year();
	}
	const bool is_valid() const{
		if(c_time() < 1) return false;
		if(year() == 1900) return false;
		if(year() < 1) return false;
		if(month() < 1) return false;
		if(day() < 1) return false;
		if(hour() < 0) return false;
		if(minute() < 0) return false;
		if(second() < 0) return false;
		return true;
	}

	//weekday()==6 �Ƃ��������������Ȃ̂Ōʏ�����t�����Ă���
	const bool is_sunday() const { return weekday() == 0;}
	const bool is_monday() const { return weekday() == 1;}
	const bool is_tuesday() const { return weekday() == 2;}
	const bool is_wednesday() const { return weekday() == 3;}
	const bool is_thursday() const { return weekday() == 4;}
	const bool is_friday() const { return weekday() == 5;}
	const bool is_saturday() const { return weekday() == 6;}
};
inline const time24 time_from_string(const std::string &src, const std::string &fmt="%Y/%m/%d %H:%M:%S")
{
	time24 dest;
	dest.from_string(src, fmt);
	return dest;
}
inline const time24 time_from_string(const char *src, const char *fmt)
{
	return time_from_string(std::string(src), std::string(fmt));
}
#if 0
inline const clx::time_duration operator-(const time24 &ld, const time24 &rd)
{
	const size_t	src = static_cast<size_t>(ld.c_time() - rd.c_time());
	const size_t days_buf = src/(60*60*24);
	const size_t hours_buf = days_buf/24;
	const size_t min_buf =  hours_buf/60;
	const size_t sec_buf =  min_buf/60;
	return clx::time_duration(days_buf, hours_buf%24, min_buf%(60), sec_buf%(60));
}
#endif
inline const time24 operator+(const time24 &ld, const clx::time_duration &rd)
{
	return time24(ld.c_time() + rd.c_time());
}
inline const time24 operator-(const time24 &ld, const clx::time_duration &rd)
{
	return time24(ld.c_time() - rd.c_time());
}
inline const clx::time_duration operator-(const time24 &ld, const time24 &rd)
{
	return clx::time_duration(ld.c_time() - rd.c_time());
}

inline const time24 NowTime(){ time24 s; return s.set_now(); }
inline const std::string ttos24_ymd(const time24 &tim){ return tim.to_string24(TIME24_YMD); }
inline const std::string ttos24(const time24 &tim, const std::string &s=TIME24_YMDHNS){ return tim.to_string24(s.c_str()); }
inline const std::string ttos(const time24 &tim, const std::string &s = TIME_YMDHNS){ return tim.to_string(s.c_str()); }
inline const std::string ttos_ymd(const time24 &tim){ return tim.to_string(TIME_YMD); }
inline const std::string ttos24_ymd(const time_t &tim)
{
	time24 s(tim);
	return s.to_string24(TIME24_YMD);
}
inline const std::string ttos24(const time_t &tim)
{ 
	time24 s(tim);
	return s.to_string24(TIME24_YMDHNS); 
}
inline const std::string ttos(const time_t &tim)
{ 
	time24 s(tim);
	return s.to_string(TIME_YMDHNS); 
}
inline const std::string ttos_ymd(const time_t &tim)
{ 
	time24 s(tim);
	return s.to_string(TIME_YMD); 
}
inline const time_t string_to_time(const std::string &s)
{ 
	time24	src;
	src.from_string(s.c_str(), TIME_YMDHNS);
	return src.c_time();
}



}//bcl::

#endif //__BCL_TIME24_H__