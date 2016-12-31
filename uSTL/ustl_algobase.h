#ifndef __U_STL_INTERNAL_ALGOBASE_H
#define __U_STL_INTERNAL_ALGOBASE_H

#include "ustl_config.h"
#include "ustl_pair.h"
#include"utype_traits.h"

#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <stddef.h>
#include <new.h>
#include <iostream>

#include "ustl_iterator_base.h"
//#include "ustl_iterator.h"

__U_STL_BEGIN_NAMESPACE

//--------------------------------------------------
// min and max
template<class _Tp>
inline const _Tp& umin(const _Tp& __a, const _Tp& __b){
	return __b < __a ? __b : __a;
}

template<class _Tp, class _Compare>
inline const _Tp& umin(const _Tp& __a, const _Tp& __b, _Compare __comp){
	return __comp(__a, __b) ? __a : __b;
}

template<class _Tp>
inline const _Tp& umax(const _Tp& __a, const _Tp& __b){
	return __a < __b ? __b : __a;
}

template<class _Tp, class _Compare>
inline const _Tp& umax(const _Tp& __a, const _Tp& __b, _Compare __comp){
	return __comp(__a, __b) ? __b : __a;
}

// swap and iter_swap
template<class _FI1, class _FI2, class _Tp>
inline void __iter_swap(_FI1 __a, _FI2 __b, _Tp*){
	_Tp __tmp = *__a;
	*__a = *__b;
	*__b = __tmp;
}

template<class _FI1, class _FI2>
inline void uiter_swap(_FI1 __a, _FI2 __b){
	__iter_swap(__a, __b, __U_VALUE_TYPE(__a));
}

template<class _Tp>
inline void uswap(_Tp& __a, _Tp& __b){
	_Tp __tmp = __a;
	__a = __b;
	__b = __tmp;
}

//--------------------------------------------------
// copy function

template<class _InputIterator, class _OutputIterator, class _Distance>
inline _OutputIterator __copy(_InputIterator __first, _InputIterator __last, _OutputIterator __result, uinput_iterator_tag, _Distance *){
	for (; __first != __last; ++__first, ++_result)
		*__result = *__first;
	return __result;
}

template<class _RandomAccessIterator, class _OutputIterator, class _Distance>
inline _OutputIterator __copy(_RandomAccessIterator __first, _RandomAccessIterator __last, _OutputIterator __result, urandom_access_iterator_tag, _Distance*){
	_Distance __n = __last - __first;
	for (; n > 0;--__n){
		*__result = *__first;
		++__result;
		++__first;
	}
	return __result;
}

template <class _Tp>
inline _Tp* __copy_trivial(const _Tp* __first, const _Tp* __last, _Tp* __result){
	memmove(__result, __first, sizeof(_Tp)*(__last - __first));
	return __result + (__last - __first);
}

template<class _InputIterator, class _OutputIterator, class _BoolType>
struct __copy_dispatch{
	static _OutputIterator copy(_InputIterator __first, _InputIterator __last, _OutputIterator __result){
		typedef typename uiterator_traits<_InputIterator>::iterator_category _Category;
		typedef typename uiterator_traits<_InputIterator>::difference_type _Distance;
		return __copy(__first, __last, __result, _Category(), (_Distance*)0);
	}
};

template<class _Tp>
struct __copy_dispatch < _Tp*, _Tp*, __true_type> {
	static _Tp* copy(_Tp* __first, _Tp* __last, _Tp* __result){
		return __copy_trivial(__first, __last, __result);
	}
};

template<class _Tp>
struct __copy_dispatch <const _Tp*,_Tp*, __true_type> {
	static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result){
		return __copy_trivial(__first, __last, __result);
	}
};

template<class _InputIterator, class _OutputIterator>
inline _OutputIterator ucopy(_InputIterator __first, _InputIterator __last, _OutputIterator __result){
	typedef typename uiterator_traits<_InputIterator>::value_type _Tp;
	typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _Trivial;
	return __copy_dispatch<_InputIterator, _OutputIterator, _Trivial>::copy(__first, __last, __result);
}

//--------------------------------------------------
// copy_backward
template <class _BidirectionIterator1, class _BidirectionIterator2, class _Distance>
inline _BidirectionIterator2 __copy_backward(_BidirectionIterator1 __first, _BidirectionIterator1 __last, _BidirectionIterator2 __result,
	ubidirectional_iterator_tag, _Distance*){
	while (__first != __last)
		*--__result = *--__last;
	return __result;
}

template <class _RandomAccessIterator, class _BidirectionIterator, class _Distance>
inline _RandomAccessIterator __copy_backward(_BidirectionIterator __first, _BidirectionIterator __last, _RandomAccessIterator __result,
	urandom_access_iterator_tag, _Distance*){
	for (_Distance __n = __last - __first; __n > 0; --__n)
		*--__result = *--__last;
	return __result;
}

template<class _BI1, class _BI2, class _BoolType>
struct __copy_backward_dispatch{
	typedef typename uiterator_traits<_BI1>::iterator_category	_Cat;
	typedef typename uiterator_traits<_BI1>::difference_type	_Distance;
	static _BI2 copy(_BI1 __first, _BI1 __last, _BI2 __result){
		return __copy_backward(__first, __last, __result, _Cat(), (_Distance *)0);
	}
};

template<class _Tp>
struct __copy_backward_dispatch < _Tp*, _Tp*, __true_type > {
	static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result){
		const ptrdiff_t _Num = __last - __first;
		memmove(__result - _Num, __first, _Num*sizeof(_Tp)*_Num);
		return __result - _Num;
	}
};

template <class _Tp>
struct __copy_backward_dispatch<const _Tp*, _Tp*, __true_type>
{
	static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) {
		return  __copy_backward_dispatch<_Tp*, _Tp*, __true_type>
			::copy(__first, __last, __result);
	}
};

template<class _BI1, class _BI2>
inline _BI2 ucopy_backward(_BI1 __first, _BI1 __last, _BI2 __result){
	typedef typename __type_traits<typename iterator_traits<_BI2>::value_type>
		::has_trivial_assignment_operator
		_Trivial;
	return __copy_backward_dispatch<_BI1, _BI2, _Trivial>::copy(__first, __last, __result);
}

//--------------------------------------------------
// copy_n (not part of the C++ standard)

template <class _InputIter, class _Size, class _OutputIter>
upair<_InputIter, _OutputIter> __copy_n(_InputIter __first, _Size __count, _OutputIter __result, uinput_iterator_tag) {
	for (; __count > 0; --__count) {
		*__result = *__first;
		++__first;
		++__result;
	}
	return upair<_InputIter, _OutputIter>(__first, __result);
}

template <class _RAIter, class _Size, class _OutputIter>
inline upair<_RAIter, _OutputIter> __copy_n(_RAIter __first, _Size __count, _OutputIter __result, urandom_access_iterator_tag) {
	_RAIter __last = __first + __count;
	return upair<_RAIter, _OutputIter>(__last, copy(__first, __last, __result));
}

template <class _InputIter, class _Size, class _OutputIter>
inline upair<_InputIter, _OutputIter> __copy_n(_InputIter __first, _Size __count, _OutputIter __result) {
	return __copy_n(__first, __count, __result,
		__U_ITERATOR_CATEGORY(__first));
}

template <class _InputIter, class _Size, class _OutputIter>
inline upair<_InputIter, _OutputIter> ucopy_n(_InputIter __first, _Size __count, _OutputIter __result) {
	return __copy_n(__first, __count, __result);
}

//--------------------------------------------------
// fill and fill_n
template<class _ForwardIterator, class _Tp>
inline void ufill(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __val){
	for (; __first != __last; ++__first)
		*__first = __val;
}

template<class _OutputIterator, class _Size, class _Tp>
inline _OutputIterator ufill_n(_OutputIterator __first, _Size __n, const _Tp& __val){
	for (; __n > 0; --__n,++__first)
		*__first = __val;
	return __first;
}

// Specialization: for one-byte types we can use memset.

inline void ufill(unsigned char* __first, unsigned char* __last,
	const unsigned char& __c) {
	unsigned char __tmp = __c;
	memset(__first, __tmp, __last - __first);
}

inline void ufill(signed char* __first, signed char* __last,
	const signed char& __c) {
	signed char __tmp = __c;
	memset(__first, static_cast<unsigned char>(__tmp), __last - __first);
}

inline void ufill(char* __first, char* __last, const char& __c) {
	char __tmp = __c;
	memset(__first, static_cast<unsigned char>(__tmp), __last - __first);
}

template <class _Size>
inline unsigned char* ufill_n(unsigned char* __first, _Size __n,
	const unsigned char& __c) {
	fill(__first, __first + __n, __c);
	return __first + __n;
}

template <class _Size>
inline signed char* ufill_n(char* __first, _Size __n,
	const signed char& __c) {
	fill(__first, __first + __n, __c);
	return __first + __n;
}

template <class _Size>
inline char* ufill_n(char* __first, _Size __n, const char& __c) {
	fill(__first, __first + __n, __c);
	return __first + __n;
}

//--------------------------------------------------
// equal and mismatch

template<class _II1, class _II2>
inline bool uequal(_II1 __first1, _II1 __last1, _II2 __first2){
	for (; __first1 != __last1; ++__first1, ++__first2)
		if (*__first1 != *__first2)
			return false;
	return true;
}

template<class _II1, class _II2, class _BP>
inline bool uequal(_II1 __first1, _II1 __last1, _II2 __first2, _BP __binary_pred){
	for (; __first1 != __last1; ++__first1, ++__first2)
		if (!__binary_pred(*__first1, *__first2))
			return false;
	return true;
}

//--------------------------------------------------
// lexicographical_compare and lexicographical_compare_3way.
// (the latter is not part of the C++ standard.)
//TODO: lexicographical_compare_3way

template<class _II1, class _II2>
inline bool ulexicographical_compare(_II1 __first1, _II1 __last1, _II2 __first2, _II2 __last2){
	for (; __first1 != __last1 && __first2 != __last2; ++__first1, ++__first2)
		if (*__first1 < *__first2)
			return true;
		else if (*__first1 > *__first2)
			return false;
	return __first1 == __last1 && __first2 != __last2;
}

template<class _II1, class _II2, class _Compare>
inline bool ulexicographical_compare(_II1 __first1, _II1 __last1, _II2 __first2, _II2 __last2, _Compare _comp){
	for (; __first1 != __last1 && __first2 != __last2; ++__first1, ++__first2)
		if (_comp(*__first1, *__first2))
			return true;
		else if (_comp(*_first2, *__first1))
			return false;
	return __first1 == __last1 && __first2 != __last2;
}

inline bool ulexicographical_compare(const unsigned char* __first1, const unsigned char* __last1, const unsigned char* __first2, const unsigned char* __last2){
	const size_t __len1 = __last1 - __first1;
	const size_t __len2 = __last2 - __first2;
	const int __result = memcmp(__first1, __first2, umin(__len1, __len2));
	return __result != 0 ? __result < 0 : __len1 < __len2;
}

__U_STL_END_NAMESPACE
#endif //__U_STL_INTERNAL_ALGOBASE_H