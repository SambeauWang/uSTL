#ifndef __U_STL_INTERNAL_CONSTRUCT_H
#define __U_STL_INTERNAL_CONSTRUCT_H

#include <new.h>
#include "utype_traits.h"
#include "ustl_config.h"

// TODO: add stl namespace in ustl_config.h
__U_STL_BEGIN_NAMESPACE

template<class _T1, class _T2>
inline void __uConstruct(_T1* __p, const _T2& uvalue){
	new ((void*)__p) _T1(uvalue);
}

template<class _T1>
inline void __uConstruct(_T1* __p){
	new ((void*)__p) _T1();
}

template<class _T1>
inline void _uDestroy(_T1*__p){
	__p->~_T1();
}

// TODO: add iterator version of destroy function
template<class _ForwardIterator>
inline void __u_destroy_aux(_ForwardIterator __first, _ForwardIterator __last, __false_type){
	_ForwardIterator __cur = __first;
	for (; __cur != __last; ++__cur)
		udestroy(&*__cur);
}

template<class _ForwardIterator>
inline void __u_destroy_aux(_ForwardIterator __first, _ForwardIterator __last, __true_type){}

template<class _ForwardIterator, class _Tp>
inline void __u_destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*){
	typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
	__u_destroy_aux(__first, __last, _Trivial_destructor());
}

template<class _ForwardIterator>
inline void _uDestroy(_ForwardIterator __first, _ForwardIterator __last){
	__u_destroy(__first, __last, __U_VALUE_TYPE(__first));
}

inline void _uDestroy(char*, char*){}
inline void _uDestroy(int*, int*){}
inline void _uDestroy(float*, float*){}
inline void _uDestroy(long*, long*){}
inline void _uDestroy(double*, double*){}

// --------------------------------------------------
// Old names from the HP STL.

template<class _T1, class _T2>
inline void uconstruct(_T1* __p, const _T2& __u_value){
	__uConstruct(__p, __u_value);
}

template<class _T1>
inline void uconstruct(_T1* __p){
	__uConstruct(__p);
}

template<class _T1>
inline void udestroy(_T1* __p){
	_uDestroy(__p);
}

// TODO: add iterator version of destroy function
template <class _ForwardIterator>
inline void udestroy(_ForwardIterator __first, _ForwardIterator __last) {
	_uDestroy(__first, __last);
}

__U_STL_END_NAMESPACE

# endif