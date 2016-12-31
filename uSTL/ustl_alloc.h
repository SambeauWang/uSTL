#ifndef __U_STL_INTERNAL_ALLOC_H
#define __U_STL_INTERNAL_ALLOC_H

// TODO: undefined
//#ifdef __STL_STATIC_TEMPLATE_MEMBER_BUG
//#  define __USE_MALLOC
//#endif

#include "ustl_config.h"

#ifndef __U_THROW_BAD_ALLOC
#	if defined(__U_STL_NO_BAD_ALLOC) || !defined(__U_STL_USE_EXCEPTIONS)
#		include <stdio.h>
#		include <stdlib.h>
#		define __U_THROW_BAD_ALLOC fprintf(stderr, "out of memory\n"); exit(1);
#	else
#		include <new>
#		define __U_THROW_BAD_ALLOC throw std::bad_alloc()
#	endif
#endif

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifndef __U_RESTRICT
#  define __U_RESTRICT
#endif

// TODO: add stl namespace in ustl_config.h
__U_STL_BEGIN_NAMESPACE

// Malloc-based allocator.  Typically slower than default alloc below.
// Typically thread-safe and more storage efficient.
//#ifdef __U_STL_STATIC_TEMPLATE_MEMBER_BUG
//# ifdef __U_DECLARE_GLOBALS_HERE
//	void(*__malloc_alloc_oom_handler)() = 0;
//// g++ 2.7.2 does not handle static template data members.
//# else
//	extern void(*__malloc_alloc_oom_handler)();
//# endif
//#endif

template<int __inst>
class __u_malloc_alloc_template{
private:
	static void* _S_oom_malloc(size_t);
	static void* _S_oom_realloc(void*, size_t);
	static void(*__molloc_alloc_oom_handler)();

public:
	static void* allocate(size_t __n){
		void* __result = malloc(__n);
		if (0 == __result) __result = _S_oom_malloc(__n);
		return __result;
	}

	static void deallocate(void* __p, size_t __n){
		free(__p);
	}

	static void* reallocate(void* __p, size_t __old_sz, size_t __new_sz){
		void* __result = reallocate(__p, __new_sz);
		if (0 == __result) __result = _S_oom_realloc(__p, __new_sz);
		return __result;
	}

	static void (*__set_malloc_handler(void (*__f)()))(){
		void(*__old)() = __molloc_alloc_oom_handler;
		__molloc_alloc_oom_handler = __f;
		return (__old);
	}
};

template<int __inst>
void(*__u_malloc_alloc_template<__inst>::__molloc_alloc_oom_handler)() = 0;

template<int __inst>
void* __u_malloc_alloc_template<__inst>::_S_oom_malloc(size_t __n){
	void(*__my_alloc_handler)();
	void* __result;
	for (;;){
		__my_alloc_handler = __molloc_alloc_oom_handler;
		if (0 == __my_alloc_handler) __U_THROW_BAD_ALLOC;
		(*__my_alloc_handler)();
		__result = malloc(__n);
		if (__result) return (__result);
	}
}

template<int __inst>
void* __u_malloc_alloc_template<__inst>::_S_oom_realloc(void *__p, size_t __n){
	void(*__my_alloc_handler)();
	void* __result;
	for (;;){
		__my_alloc_handler = __molloc_alloc_oom_handler;
		if (0 == __my_alloc_handler) __U_THROW_BAD_ALLOC;
		(*__my_alloc_handler)();
		__result = reallocate(__p, __n);
		if (__result) return (__result);
	}
}

typedef __u_malloc_alloc_template<0> malloc_alloc;

template<class _Tp, class _Alloc>
class usimple_alloc {
public:
	static _Tp* allocate(size_t __n)
	{
		return (0 == __n) ? 0 : (_Tp *)_Alloc::allocate(__n * sizeof(_Tp));
	}
	static _Tp* allocate(void){
		return (_Tp *) _Alloc::allocate(sizeof(_Tp));
	}

	static void deallocate(_Tp *__p, size_t __n){
		if (0 != __n)_Alloc::deallocate(__p, __n*sizeof(_Tp));
	}
	static void deallocate(_Tp *__p){
		_Alloca::deallocate(__p, sizeof(_Tp));
	}
};

// TODO: a adaptor of alloc is used for debugging.
//template <class _Alloc>
//class debug_alloc {}


template<bool threads, int inst>
class __u_default_alloc_template{
private:
	enum {_U_ALIGN = 8};
	enum {_U_MAX_BYTES = 128 };
	enum {_U_NFREELISTS = 16};

	static size_t _S_round_up(size_t __bytes){
		return (((__bytes)+(size_t)_U_ALIGN - 1) & ~((size_t)_U_ALIGN - 1));
	}

	union _Obj {
		union _Obj* _M_free_list_link;
		char _M_client_data[1];
	};

	static _Obj* volatile _S_free_list[_U_NFREELISTS];

	static size_t _S_freelist_index(size_t __bytes){
		return (((__bytes)+(size_t)_U_ALIGN - 1) / ((size_t)_U_ALIGN - 1));
	}

	static void* _S_refill(size_t __n);
	static char* _S_chunk_alloc(size_t __size, int& __nobjs);

	static char* _S_start_free;
	static char* _S_end_free;
	static size_t _S_heap_size;

public:
	static void* allocate(size_t __n){
		void* __ret = 0;

		if (__n > (size_t)_U_MAX_BYTES){
			__ret = malloc_alloc::allocate(__n);
		}
		else{
			_Obj* volatile* __my_free_list = _S_free_list + _S_freelist_index(__n);
			_Obj* __result = *__my_free_list;
			if (0 == __result){
				__ret = _S_refill(_S_round_up(__n));
			}
			else{
				*__my_free_list = __result->_M_free_list_link;
				__ret = __result;
			}
		}

		return (__ret);
	}

	static void deallocate(void* __p, size_t __n){
		if (__n > (size_t)_U_MAX_BYTES){
			malloc_alloc::deallocate(__p, __n);
		}
		else{
			_Obj* __q = (_Obj*)__p;
			_Obj* volatile* __my_free_list = _S_free_list + _S_freelist_index(__n);
			__q->_M_free_list_link = *__my_free_list;
			*__my_free_list = __q;
		}
	}

	static void* reallocate(void* __p, size_t __old_sz, size_t __new_sz);
};

typedef __u_default_alloc_template<false, 0> ualloc;
typedef __u_default_alloc_template<false, 0> single_client_alloc;

template <bool __threads, int __inst>
inline bool operator==(const __u_default_alloc_template<__threads, __inst>&,
	const __u_default_alloc_template<__threads, __inst>&)
{
	return true;
}

template <bool __threads, int __inst>
inline bool operator!=(const __u_default_alloc_template<__threads, __inst>&,
	const __u_default_alloc_template<__threads, __inst>&)
{
	return false;
}

template<bool __threads, int __inst>
char* __u_default_alloc_template<__threads, __inst>::_S_chunk_alloc(size_t __size, int& __nobjs){
	char* __result;
	size_t __total_bytes = __size * __nobjs;
	size_t __bytes_left = _S_end_free - _S_start_free;

	if (__bytes_left > __total_bytes){
		__result = (char*)_S_start_free;
		_S_start_free += __total_bytes;
		return (__result);
	}
	else if (__bytes_left > __size){
		__nobjs = (int)(__bytes_left / __size);
		__total_bytes = __nobjs * __size;
		__result = (char*)_S_start_free;
		_S_start_free += __total_bytes;
		return (__result);
	}
	else{
		size_t __bytes_to_get = 2 * __total_bytes + _S_freelist_index(_S_heap_size >> 4);
		if (__bytes_left > 0){
			_Obj* volatile* __my_free_list = _S_free_list + _S_freelist_index(__bytes_left);
			((_Obj*)_S_start_free)->_M_free_list_link = *__my_free_list;
			*__my_free_list = (_Obj*)_S_start_free;
		}

		_S_start_free = (char*)malloc(__bytes_to_get);
		if (0 == _S_start_free){
			size_t __i;
			_Obj* volatile* __my_free_list;
			_Obj* __p;
			for (__i = __size; __i < (size_t)_U_MAX_BYTES; __i += (size_t)_U_ALIGN){
				__my_free_list = _S_free_list + _S_freelist_index(__i);
				__p = *__my_free_list;
				if (0 != __p){
					*__my_free_list = __p->_M_free_list_link;
					_S_start_free = (char*)__p;
					_S_end_free = _S_start_free + __i;
					return (_S_chunk_alloc(__size, __nobjs));
				}
			}
			_S_end_free = 0;
			_S_start_free = (char*)malloc_alloc::allocate(__bytes_to_get);
		}
		_S_heap_size += __bytes_to_get;
		_S_end_free = _S_start_free + __bytes_to_get;
		return (_S_chunk_alloc(__size, __nobjs));
	}
}

template<bool __threads, int __inst>
void* __u_default_alloc_template<__threads, __inst>::_S_refill(size_t __n){
	int __nobjs = 20;
	char* __chunk = _S_chunk_alloc(__n, __nobjs);
	_Obj* __current_obj;
	_Obj* __next_obj;
	_Obj* volatile* __my_free_list;
	int __i;

	if (1 == __nobjs)return (__chunk);
	__my_free_list = _S_free_list + _S_freelist_index(__n);
	*__my_free_list = __next_obj = (_Obj *)(__chunk + __n);
	for (__i=1;;__i++){
		__current_obj = __next_obj;
		__next_obj = (_Obj*)((char*)__next_obj + __n);
		if (__nobjs - 1 == __i){
			__current_obj->_M_free_list_link = 0;
			break;
		}
		else{
			__current_obj->_M_free_list_link = __next_obj;
		}
	}
	return (__chunk);
}

template<bool __threads, int __inst>
void* __u_default_alloc_template<__threads, __inst>::reallocate(void* __p, size_t __old_size, size_t __new_size){
	void* __result;
	size_t __copy_size;

	if (__old_size > (size_t)_U_MAX_BYTES && __new_size > (size_t)_U_MAX_BYTES){
		__result = malloc_alloc::reallocate(__p, __old_size, __new_size);
		return __result;
	}

	if (_S_round_up(__old_size) == _S_round_up(__new_size)) return (__p);
	__result = allocate(__new_size);
	__copy_size = __old_size > __new_size ? __new_size : __old_size;
	memcpy(__result, __p, __copy_size);
	deallocate(__p, __old_size);

	return (__result);
}

template<bool __threads, int __inst>
char* __u_default_alloc_template<__threads, __inst>::_S_start_free = 0;

template<bool __threads, int __inst>
char* __u_default_alloc_template<__threads, __inst>::_S_end_free = 0;

template<bool __threads, int __inst>
size_t __u_default_alloc_template<__threads, __inst>::_S_heap_size = 0;

template<bool __threads, int __inst>
typename __u_default_alloc_template<__threads, __inst>::_Obj* volatile
__u_default_alloc_template<__threads, __inst>::_S_free_list[__u_default_alloc_template<__threads, __inst>::_U_NFREELISTS]
= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };

#ifdef __STL_USE_STD_ALLOCATORS

template<class _Tp>
class uallocator{
	typedef ualloc _Alloc;

	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef _Tp* pointer;
	typedef const _Tp* const_pointer;
	typedef _Tp& reference;
	typedef const _Tp& const_reference;
	typedef _Tp value_type;

	template<_Tp1>struct rebind{
		typedef uallocator<_Tp1> other;
	};

	uallocator() __U_STL_NOTHROW{}
	uallocator(const uallocator&) __U_STL_NOTHROW{}
	template<class _Tp1> uallocator(const uallocator<_Tp1>&) __U_STL_NOTHROW {}
	~uallocator(){}

	pointer address(reference& __x)const { return &__x; }
	const_pointer address(const_reference& __x)const { return &__x; }

	_Tp* allocate(size_type __n, const void* = 0){
		return __n != 0 ? static_cast<_Tp*>(_Alloc::allocate(__n*sizeof(_Tp))) : 0;
	}
	void deallocate(_Tp* __p, size_type __n){
		_Alloc::deallocate(__p, __n*sizeof(_Tp);
	}

	size_type max_size()const __U_STL_NOTHROW{ return size_t(-1) / sizeof(_Tp); }

	void construct(pointer __p, const _Tp& __val){ new(__p)_Tp(__val); }
	void destory(pointer __p){ __p->~_Tp(); }
};

template<>
class uallocator<void> {
public:
	typedef size_t      size_type;
	typedef ptrdiff_t   difference_type;
	typedef void*       pointer;
	typedef const void* const_pointer;
	typedef void        value_type;

	template <class _Tp1> struct rebind {
		typedef uallocator<_Tp1> other;
	};
};


template <class _T1, class _T2>
inline bool operator==(const uallocator<_T1>&, const uallocator<_T2>&)
{
	return true;
}

template <class _T1, class _T2>
inline bool operator!=(const uallocator<_T1>&, const uallocator<_T2>&)
{
	return false;
}

// TODO: Allocator adaptor to turn an SGI-style allocator into a standard-conforming allocator.
//template <class _Tp, class _Alloc>
//struct __allocator {}

template <int inst>
inline bool operator==(const __u_malloc_alloc_template<inst>&,
	const __u_malloc_alloc_template<inst>&)
{
	return true;
}

inline bool operator!=(const __u_malloc_alloc_template<__inst>&,
	const __u_malloc_alloc_template<__inst>&)
{
	return false;
}

// TODO:_Alloc_traits
// Another allocator adaptor: _Alloc_traits.  This serves two
// purposes.  First, make it possible to write containers that can use
// either SGI-style allocators or standard-conforming allocator.
// Second, provide a mechanism so that containers can query whether or
// not the allocator has distinct instances.  If not, the container
// can avoid wasting a word of memory to store an empty object.

#endif  /* __STL_USE_STD_ALLOCATORS */

__U_STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_ALLOC_H */