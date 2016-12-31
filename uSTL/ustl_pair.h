#ifndef __U_STL_INTERNAL_PAIR_H
#define __U_STL_INTERNAL_PAIR_H

#include "ustl_config.h"

__U_STL_BEGIN_NAMESPACE

template<class _T1, class _T2>
struct upair{
	typedef _T1 first_type;
	typedef _T2 second_type;

	_T1 first;
	_T2 second;
	upair() :first(_T1()), second(_T2()){}
	upair(const _T1& __x, const _T2& __y) :first(__x), second(__y){}

#ifdef __U_MEMBER_TEMPLATES
	template<class _U1, class _U2>
	upair(const upair<_U1, _U2>& __p) : first(__p.first), second(__p.second){}
#endif
};

template<class _T1, class _T2>
inline bool operator==(const upair<_T1, _T2>& __x, const upair<_T1, _T2>& __y){
	return __x.first == __y.first && __x.second == __y.second;
}

template<class _T1, class _T2>
inline bool operator<(const upair<_T1, _T2>& __x, const upair<_T1, _T2>& __y){
	return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}

template<class _T1, class _T2>
inline bool operator!=(const upair<_T1, _T2>& __x, const upair<_T1, _T2>& __y){
	return !(__x == __y);
}

template<class _T1, class _T2>
inline bool operator>(const upair<_T1, _T2>& __x, const upair<_T1, _T2>& __y){
	return __y < __x;
}

template<class _T1, class _T2>
inline bool operator<=(const upair<_T1, _T2>& __x, const upair<_T1, _T2>& __y){
	return !(__x > __y);
}

template<class _T1, class _T2>
inline bool operator>=(const upair<_T1, _T2>& __x, const upair<_T1, _T2>& __y){
	return !(__x < __y);
}

template<class _T1, class _T2>
inline upair<_T1, _T2> umake_pair(const _T1& __a, const _T2& __b){
	return upair<_T1, _T2>(__a, __b);
}

__U_STL_END_NAMESPACE
#endif