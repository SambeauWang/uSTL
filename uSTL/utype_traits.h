#ifndef __U_TYPE_TRAITS_H
#define __U_TYPE_TRAITS_H

struct __true_type{};
struct __false_type{};

#include "ustl_config.h"

template<class _Tp>
struct __type_traits{
	typedef __true_type     this_dummy_member_must_be_first;
	typedef __false_type	has_trivial_default_constructor;
	typedef __false_type	has_trivial_copy_constructor;
	typedef __false_type	has_trivial_assignment_operator;
	typedef __false_type    has_trivial_destructor;
	typedef __false_type	is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<bool> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<char> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<signed char> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<unsigned char> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<short> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<unsigned short> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<int> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<unsigned int> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<long> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<unsigned long> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<long long> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<unsigned long long> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<float> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<double> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<long double> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

template <class _Tp>
struct __type_traits<_Tp*> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<char*> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<signed char*> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<unsigned char*> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<const char*> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<const signed char*> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<const unsigned char*> {
	typedef __true_type    has_trivial_default_constructor;
	typedef __true_type    has_trivial_copy_constructor;
	typedef __true_type    has_trivial_assignment_operator;
	typedef __true_type    has_trivial_destructor;
	typedef __true_type    is_POD_type;
};

// The following could be written in terms of numeric_limits.  
// We're doing it separately to reduce the number of dependencies.

template <class _Tp> struct _uIs_integer {
	typedef __false_type _Integral;
};

__STL_TEMPLATE_NULL struct _uIs_integer<bool> {
	typedef __true_type _Integral;
};

__STL_TEMPLATE_NULL struct _uIs_integer<char> {
	typedef __true_type _Integral;
};

__STL_TEMPLATE_NULL struct _uIs_integer<signed char> {
	typedef __true_type _Integral;
};

__STL_TEMPLATE_NULL struct _uIs_integer<unsigned char> {
	typedef __true_type _Integral;
};

__STL_TEMPLATE_NULL struct _uIs_integer<short> {
	typedef __true_type _Integral;
};

__STL_TEMPLATE_NULL struct _uIs_integer<unsigned short> {
	typedef __true_type _Integral;
};

__STL_TEMPLATE_NULL struct _uIs_integer<int> {
	typedef __true_type _Integral;
};

__STL_TEMPLATE_NULL struct _uIs_integer<unsigned int> {
	typedef __true_type _Integral;
};

__STL_TEMPLATE_NULL struct _uIs_integer<long> {
	typedef __true_type _Integral;
};

__STL_TEMPLATE_NULL struct _uIs_integer<unsigned long> {
	typedef __true_type _Integral;
};

# endif