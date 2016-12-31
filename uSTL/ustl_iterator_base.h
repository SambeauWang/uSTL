#ifndef __U_STL_INTERNAL_ITERATOR_BASE_H
#define __U_STL_INTERNAL_ITERATOR_BASE_H

// TODO: 
//#include <concept_checks.h>
#include "ustl_config.h"

__U_STL_BEGIN_NAMESPACE

struct uinput_iterator_tag {};
struct uoutput_iterator_tag {};
struct uforward_iterator_tag : public uinput_iterator_tag {};
struct ubidirectional_iterator_tag : public uforward_iterator_tag{};
struct urandom_access_iterator_tag : public ubidirectional_iterator_tag{};

template<class _Category, class _Tp, class _Distance = ptrdiff_t, class _Pointer = _Tp*, class _Reference = _Tp&>
struct uiterator{
	typedef _Category	iterator_category;
	typedef _Tp			value_type;
	typedef _Distance	difference_type;
	typedef _Tp*		pointer;
	typedef _Tp&		reference;
};

template<class _iterator>
struct uiterator_traits{
	typedef typename _iterator::iterator_category	iterator_category;
	typedef typename _iterator::value_type			value_type;
	typedef typename _iterator::difference_type		difference_type;
	typedef typename _iterator::pointer				pointer;
	typedef typename _iterator::reference			reference;
};

template<class _Tp>
struct uiterator_traits <_Tp*> {
	typedef urandom_access_iterator_tag		iterator_category;
	typedef _Tp								value_type;
	typedef ptrdiff_t						difference_type;
	typedef _Tp*							pointer;
	typedef _Tp&							reference;
};

template<class _Tp>
struct uiterator_traits <const _Tp*> {
	typedef urandom_access_iterator_tag		iterator_category;
	typedef _Tp								value_type;
	typedef ptrdiff_t						difference_type;
	typedef const _Tp*							pointer;
	typedef const _Tp&							reference;
};

// They are included for backward compatibility with the HP STL.
template <class _Iter>
inline typename uiterator_traits<_Iter>::iterator_category
__u_iterator_category(const _Iter&)
{
	typedef typename uiterator_traits<_Iter>::iterator_category _Category;
	return _Category();
}

template <class _Iter>
inline typename uiterator_traits<_Iter>::difference_type*
__u_distance_type(const _Iter&)
{
	return static_cast<typename uiterator_traits<_Iter>::difference_type*>(0);
}

template <class _Iter>
inline typename uiterator_traits<_Iter>::value_type*
__u_value_type(const _Iter&)
{
	return static_cast<typename uiterator_traits<_Iter>::value_type*>(0);
}

template <class _Iter>
inline typename uiterator_traits<_Iter>::iterator_category
uiterator_category(const _Iter& __i) { return __u_iterator_category(__i); }


template <class _Iter>
inline typename uiterator_traits<_Iter>::difference_type*
udistance_type(const _Iter& __i) { return __u_distance_type(__i); }

template <class _Iter>
inline typename uiterator_traits<_Iter>::value_type*
uvalue_type(const _Iter& __i) { return __u_value_type(__i); }

#define __U_ITERATOR_CATEGORY(__i) __u_iterator_category(__i)
#define __U_DISTANCE_TYPE(__i)     __u_distance_type(__i)
#define __U_VALUE_TYPE(__i)        __u_value_type(__i)

// distance(): the distance of two iterators
template<class _InputIterator>
inline typename uiterator_traits<_InputIterator>::difference_type
__u_distance(_InputIterator __first, _InputIterator __last, uinput_iterator_tag){
	typename uiterator_traits<_InputIterator>::difference_type __n = 0;
	while (__first != __last){
		++__first;
		++__n;
	}

	return __n;
}

template<class _RandomAccessIterator>
inline typename uiterator_traits<_RandomAccessIterator>::difference_type
__u_distance(_RandomAccessIterator _first, _RandomAccessIterator _last, urandom_access_iterator_tag){
	return _last - _first;
}

template<class _InputIterator>
inline typename uiterator_traits<_InputIterator>::difference_type
udistance(_InputIterator _first, _InputIterator _last){
	typedef typename uiterator_traits<_InputIterator>::iterator_category _Category;
	return __u_distance(_first, _last, _Category());
}

//advance(): move iterator by __n steps
template<class _InputIter, class _Distance>
inline void __u_advance(_InputIter& __i, _Distance __n, uinput_iterator_tag){
	while (__n-- != 0)
		__i++;
}

template<class _BidirectionalIterator, class _Distance>
inline void __u_advance(_BidirectionalIterator& __i, _Distance __n, ubidirectional_iterator_tag){
	if (__n > 0)
		while (__n-- != 0) __i++;
	eles
		while (__n++ != 0) __i--;
}

template<class _RandomAccessIterator, class _Distance>
inline void __u_advance(_RandomAccessIterator& __i, _Distance __n, urandom_access_iterator_tag){
	__i += __n;
}

template <class _InputIterator, class _Distance>
inline void uadvance(_InputIterator& __i, _Distance __n){
	typedef typename uiterator_traits<__i>::iterator_category _Category;
	__u_advance(__i, __n, _Category());
}

__U_STL_END_NAMESPACE

#endif