#ifndef __U_STL_INTERNAL_UNINITIALIZED_H
#define __U_STL_INTERNAL_UNINITIALIZED_H

#include "ustl_config.h"
#include "ustl_pair.h"
#include "utype_traits.h"
#include "ustl_iterator_base.h"
#include "ustl_construct.h"
#include "ustl_algobase.h"
#include <string.h>

__U_STL_BEGIN_NAMESPACE

template<class __InputIterator, class __ForwardIterator>
inline __ForwardIterator __u_uninitialized_copy_aux(__InputIterator __first, __InputIterator __last, __ForwardIterator __result, __true_type){
	return ucopy(__first, __last, __result);
}


template<class __InputIterator, class __ForwardIterator>
inline __ForwardIterator __u_uninitialized_copy_aux(__InputIterator __first, __InputIterator __last, __ForwardIterator __result, __false_type){
	__ForwardIterator __cur = __result;
	__U_STL_TRY{
		for (; __first != __last; ++__first, ++__cur)
			__uConstruct(&*__cur, *__first);
		return __cur;
	}__U_STL_UNWIND(_uDestroy(__result, __cur));
}

template<class __InputIterator, class __ForwardIterator, class _Tp>
inline __ForwardIterator __u_uninitialized_copy_aux(__InputIterator __first, __InputIterator __last, __ForwardIterator __result, _Tp*){
	typedef typename __type_traits<_Tp>::is_POD_type _Is_POD;
	return __u_uninitialized_copy_aux(__first, __last, __result, _Is_POD());
}

template<class __InputIterator, class __ForwardIterator>
inline __ForwardIterator uuninitialized_copy(__InputIterator __first, __InputIterator __last, __ForwardIterator __result){
	return __u_uninitialized_copy_aux(__first, __last, __result, __U_VALUE_TYPE(__result));
}

inline char* uuninitialized_copy(const char* __first, const char* __last,
	char* __result) {
	memmove(__result, __first, __last - __first);
	return __result + (__last - __first);
}

template<class _InputIterator, class _Size, class _ForwardIterator>
inline upair<_InputIterator, _ForwardIterator>
__u_uninitialized_copy_n(_InputIterator __first, _Size __Count, _ForwardIterator __result, uinput_iterator_tag){
	_ForwardIterator __cur = __result;
	__U_STL_TRY{
		for (; __Count > 0; --__Count, ++__first, ++__cur)
			__uConstruct(&*__cur, *__first);
		return upair<_InputIterator, _ForwardIterator>(__first, __cur);
	} __U_STL_UNWIND(_uDestroy(__result, __cur));
}

template<class _RandomAccessIterator, class _Size, class _ForwardIterator>
inline upair<_RandomAccessIterator, _ForwardIterator>
__u_uninitialized_copy_n(_RandomAccessIterator __first, _Size __Count, _ForwardIterator __result, urandom_access_iterator_tag){
	_RandomAccessIterator __last = __first + __Count;
	return upair<_RandomAccessIterator, _ForwardIterator>(__last, uuninitialized_copy(__first, __last, __result));
}

template<class _InputIterator, class _Size, class _ForwardIterator>
inline upair<_InputIterator, _ForwardIterator>
__u_uninitialized_copy_n(_InputIterator __first, _Size __Count, _ForwardIterator __result){
	return __u_uninitialized_copy_n(__first, __Count, __result, __U_ITERATOR_CATEGORY(__first));
}

template <class _InputIter, class _Size, class _ForwardIter>
inline upair<_InputIter, _ForwardIter>
uuninitialized_copy_n(_InputIter __first, _Size __count,
_ForwardIter __result) {
	return __uninitialized_copy_n(__first, __count, __result,
		__ITERATOR_CATEGORY(__first));
}

template<class _ForwardIterator, class _Tp>
inline void __u_initialized_fill_aux(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __x, __true_type){
	ufill(__first, __last, __x);
}

template<class _ForwardIterator, class _Tp>
inline void __u_initialized_fill_aux(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __x, __false_type){
	__U_STL_TRY{
		_ForwardIterator __cur = __first;
		for (; __cur != __last; ++__cur)
			__uConstruct(&*__cur, __x);
	}__U_STL_UNWIND(_uDestroy(__first, __cur));
}

template<class _ForwardIterator, class _Tp, class _Tp1>
inline void __u_initialized_fill_aux(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __x, _Tp1*){
	typedef typename __type_traits<_Tp1>::is_POD_type _Is_POD;
	__u_initialized_fill_aux(__first, __last, __X, _Is_POD());
}

template<class _ForwardIterator, class _Tp>
inline void uuinitialized_fill(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __x){
	__u_initialized_fill_aux(__first, __last, __x, __U_VALUE_TYPE(__first));
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter
__u_uninitialized_fill_n_aux(_ForwardIter __first, _Size __n,
const _Tp& __x, __true_type)
{
	return fill_n(__first, __n, __x);
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter
__u_uninitialized_fill_n_aux(_ForwardIter __first, _Size __n,
const _Tp& __x, __false_type)
{
	_ForwardIter __cur = __first;
	__U_STL_TRY{
		for (; __n > 0; --__n, ++__cur)
		_uConstruct(&*__cur, __x);
		return __cur;
	}
	__U_STL_UNWIND(_uDestroy(__first, __cur));
}

template <class _ForwardIter, class _Size, class _Tp, class _Tp1>
inline _ForwardIter
__u_uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, _Tp1*)
{
	typedef typename __type_traits<_Tp1>::is_POD_type _Is_POD;
	return __u_uninitialized_fill_n_aux(__first, __n, __x, _Is_POD());
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter
uuninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
	return __u_uninitialized_fill_n(__first, __n, __x, __VALUE_TYPE(__first));
}

__U_STL_END_NAMESPACE

#endif