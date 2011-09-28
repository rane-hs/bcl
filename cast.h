#ifndef BCL_CAST_H
#define BCL_CAST_H

//inc
#include "bclstr.h"

namespace bcl{

/*
 ������<- -> ���l�@���ݕϊ�����
 boost::lexical_cast�ł̓L���X�g�Ɏ��s����Ɨ�O����Ԃ̂�
 ��O�𓊂��Ȃ�lexical_cast_s<T>�Ȃǂ��`
	bcl::lexical_cast_s<int,-1>("5");	//-1���A���Ă�����G���[�I
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
inline const double to_float(const std::string &s){
	return (s.length() < 1)?
		0.0:
		atof(s.c_str());
}

/*!------------------------------------------------------------------
	to string conversion
-------------------------------------------------------------------*/
inline const std::string to_string(const int &a){	return bcl::format("%d", a); }
inline const std::string to_string(const unsigned int &a){	return bcl::format("%d", a); }
inline const std::string to_string(const long &a){	return bcl::format("%ld", a); }
inline const std::string to_string(const unsigned long &a){	return bcl::format("%ld", a); }
inline const std::string to_string(const float &a){	return bcl::format("%f", a); }
inline const std::string to_string(const double &a){	return bcl::format("%lf", a); }
inline const std::string to_string(const long double &a){	return bcl::format("%lf", a); }

inline const unsigned char to_bcd(const int src)
{
	int src1 = src%100;
	char dest1 = src1/10;
	char dest2 = src1%10;
	unsigned char dest = dest2 | (dest1 << 4);
	return dest;
}


}; // bcl::

#endif //BCL_CAST_H