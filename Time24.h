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

#define TIME24_YMDHN_HYPHEN "%24Y-%24m-%24d %24H:%M:%S"

#define TIME24_YMDHNS_NO_ZERO "%24Y/%24m/%24d %24H:%M:%S"
#define TIME24_YMDHN_NO_ZERO "%24Y/%24m/%24d %24H:%M"
#define TIME24_YMD_NO_ZERO "%24Y/%24m/%24d"

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

	//�w�肵��duration��ǉ���������Ԃ�
	//�l�I�ɂ킩��₷���Ȃ�悤�ȃ��b�p�ł���
	time24 const ymd_duration(value_type const &yy=0, value_type const &mm=0, value_type const &dd=0) const
	{
		return time24(*this + clx::time_duration(yy, mm, dd));
	}
	time24 const time_duration(value_type const &hh=0, value_type const &nn=0, value_type const &ss=0) const
	{
		return time24(*this + clx::hours(hh) + clx::minutes(nn) + clx::seconds(ss));
	}
	time24 const set_ymd_duration(value_type const &yy=0, value_type const &mm=0, value_type const &dd=0)
	{
		return *this = ymd_duration(yy,mm,dd);
	}
	time24 const set_time_duration(value_type const &hh=0, value_type const &nn=0, value_type const &ss=0)
	{
		return *this = time_duration(hh,nn,ss);
	}

	const std::string to_string24(const std::string &desc) const
	{
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

	time24 const yesterday() const
	{
		//�ق�Ƃ͂�����������
		return ymd_duration(0, 0, -1);
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
		if(c_time() == 0) return false;
		if(year() == 1900) return false;
		return true;
	}
};

inline const time24 NowTime(){ time24 s; return s.set_now(); }
inline const std::string ttos24_ymd(const time24 &tim){ return tim.to_string24(TIME24_YMD); }
inline const std::string ttos24(const time24 &tim){ return tim.to_string24(TIME24_YMDHNS); }
inline const std::string ttos(const time24 &tim){ return tim.to_string(TIME_YMDHNS); }
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

}//bcl::

#endif //__BCL_TIME24_H__