#ifndef __U_STL_INTERNAL_VECTOR_H
#define __U_STL_INTERNAL_VECTOR_H

#include "ustl_algobase.h"
#include "ustl_alloc.h"
#include "ustl_construct.h"
#include "ustl_uninitialized.h"
#include "ustl_config.h"

__U_STL_BEGIN_NAMESPACE

template<class _Tp, class _Alloc>
class _uVector_base{
public:
	typedef _Alloc allocator_type;
	allocator_type get_allocator_type() const{ return allocator_type(); }

	_uVector_base(const _Alloc&) :__M_start(0), __M_finish(0), __M_end_of_storage(0){}
	_uVector_base(size_t __n, const _Alloc&) :__M_start(0), __M_finish(0), __M_end_of_storage(0){
		__M_finish = __M_start = _M_allocate(__n);
		__M_end_of_storage = __M_start + __n;
	}
	~_uVector_base(){
		_M_deallocate(__M_start, __M_end_of_storage - __M_start);
	}

protected:
	_Tp* __M_start;
	_Tp* __M_finish;
	_Tp* __M_end_of_storage;

	typedef usimple_alloc<_Tp, _Alloc> _M_data_allocator;
	_Tp* _M_allocate(size_t __n){
		return _M_data_allocator::allocate(__n);
	}
	void _M_deallocate(_Tp* __p, size_t __n){
		_M_data_allocator::deallocate(__p, __n);
	}
};

template<class _Tp, class _Alloc = __U_STL_DEFAULT_ALLOCATOR(_Tp)>
class uvector : protected _uVector_base < _Tp, _Alloc > {
private:
	typedef _uVector_base<_Tp, _Alloc> _Base;
public:
	typedef	_Tp					value_type;
	typedef value_type*			pointer;
	typedef const value_type*	const_pointer;
	typedef value_type&			reference;
	typedef	const value_type&	const_reference;
	typedef value_type*			iterator;
	typedef const value_type*	const_iterator;
	typedef size_t				size_type;
	typedef ptrdiff_t			difference_type;

	typedef typename _Base::allocator_type	allocator_type;
	allocator_type get_allocator(){
		return _Base::allocator_type();
	}

	//TODO: reverse_iterator


protected:
	void _M_insert_aux(iterator __postion, const _Tp& __x);
	void _M_insert_aux(iterator __position);

public:
	iterator begin(){ return __M_start; }
	const_iterator begin() const { return __M_start; }
	iterator end(){ return __M_finish; }
	const_iterator end() const { return __M_finish; }

	//TODO: rbegin and rend implement

	size_type size() const { return size_type(end() - begin()); }
	size_type max_size() const { return size_type(-1) / sizeof(_Tp); }
	size_type capacity() const { return size_type(__M_end_of_storage - begin()); }
	bool empty() const { return begin() == end(); }

	reference operator[](size_t __n){ return *(begin() + __n); }
	const_reference operator[](size_t __n) const{ return *(begin() + __n); }
	reference at(size_t __n){ return (*this)[__n]; }
	const_reference at(size_t __n) const{ return (*this)[__n]; }

	// create construct and destroy function
	explicit uvector(const allocator_type& __a = allocator_type()) :_Base(__a){}
	uvector(size_type __n, const value_type& __val, const allocator_type& __a = allocator_type()) :_Base(__n, __a){
		__M_finish = uuninitialized_fill_n(__M_start, __n, __val);
	}
	uvector(size_type __n, const allocator_type& __a = allocator_type()) :_Base(__n, __a){
		__M_finish = uuninitialized_fill_n(__M_start, __n, _Tp());
	}
	uvector(uvector<_Tp, _Alloc>& __x) :_Base(__x.size(), __x.get_allocator()){
		__M_finish = uuninitialized_copy(__x.begin(), __x.end(), __M_start);
	}
	uvector(const _Tp* __first, const _Tp* __end, const allocator_type& __a = allocator_type()) :_Base(__end - __first, __a){
		__M_finish = uuninitialized_copy(__first, __end, __M_start);
	}

	~uvector(){ udestroy(__M_start, __M_finish); }

	uvector<_Tp, _Alloc>& operator=(const uvector<_Tp, _Alloc>& __x);
	//TODO: reserve function

	//TODO: assign function
	
	reference front(){ return *begin(); }
	const_reference front() const{ return *begin(); }
	reference back(){ return *(end() - 1); }
	const_reference back() const { return *(end() - 1); }

	//push_back function
	void push_back(const _Tp& __x = _Tp()){
		if (__M_finish != __M_end_of_storage){
			uconstruct(__M_finish, __x);
			++__M_finish;
		}
		else
			_M_insert_aux(end(), __x);
	}

	void swap(uvector<_Tp, _Alloc>& __x){
		uswap(__M_start, __x.__M_start);
		uswap(__M_finish, __x.__M_finish);
		uswap(__M_end_of_storage, __x.__M_end_of_storage);
	}

	iterator insert(iterator __position, _Tp* __x = _Tp()){
		size_type __n = __position - begin();
		if (__M_finish != __M_end_of_storage && __position == __M_finish) {
			uconstruct(__M_finish, __x);
			++__M_finish;
		}
		else
			_M_insert_aux(__position, __x);
		return begin() + __n;
	}

	void insert(iterator __position, const_iterator __start, const_iterator __end);
	void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
	void insert(iterator __position, size_type __n, const _Tp& __x){
		_M_fill_insert(__position, __n, __x);
	}

	// pop_back function
	void pop_back(){
		--__M_finish;
		udestroy(__M_finish);
	}

	// erase function
	iterator erase(iterator __position){
		if (__position + 1 != end())
			ucopy(__position + 1, __M_finish, __position);
		--__M_finish;
		udestroy(__M_finish);
		return __postion;
	}

	iterator erase(iterator __first, iterator __last){
		iterator __i = ucopy(__last, __M_finish, __first);
		udestroy(__i, __M_finish);
		__M_finish = __M_finish - (__last - __first);
		return __first;
	}

	// resize function and clear function
	void resize(size_type __new_size, const _Tp& __x){
		if (__new_size < size())
			erase(begin() + __new_size, end());
		else
			insert(end(), __new_size - size(), __x);
	}
	void resize(size_type __new_size){
		resize(__new_size, _Tp());
	}
	void clear(){ erase(begin(), end()); }

	iterator _M_allocate_and_copy(size_type __n, const_iterator __first, const_iterator __last){
		iterator __result = _M_allocate(__n);
		__U_STL_TRY{
			uuninitialized_copy(__first, __last, __result);
			return __result;
		}__U_STL_UNWIND(_M_deallocate(__result, __n));
	}

	//TODO: _M_range_initialize and _M_range_insert
};

template<class _Tp, class _Alloc>
inline bool operator==(const uvector<_Tp, _Alloc>& __x, const uvector<_Tp, _Alloc>& __y){
	return __x.size() == __y.size() && uequal(__x.begin(), __x.end(), __y.begin(), __y.end());
}

template<class _Tp, class _Alloc>
inline bool operator<(const uvector<_Tp, _Alloc>& __x, const uvector<_Tp, _Alloc>& __y){
	return ulexicographical_compare(__x.begin(), __x.end(), __y.begin(), __y.end());
}

template<class _Tp, class _Alloc>
inline void uswap(const uvector<_Tp, _Alloc>& __x, const uvector<_Tp, _Alloc>& __y){
	__x.swap(__y);
}

template<class _Tp, class _Alloc>
inline bool operator!=(const uvector<_Tp, _Alloc>& __x, const uvector<_Tp, _Alloc>& __y){
	retunr !(__x == __y);
}

template<class _Tp, class _Alloc>
inline bool operator>(const uvector<_Tp, _Alloc>& __x, const uvector<_Tp, _Alloc>& __y){
	return __y < __x;
}

template<class _Tp, class _Alloc>
inline bool operator<=(const uvector<_Tp, _Alloc>& __x, const uvector<_Tp, _Alloc>& __y){
	return !(__x > __y);
}

template<class _Tp, class _Alloc>
inline bool operator>=(const uvector<_Tp, _Alloc>& __x, const uvector<_Tp, _Alloc>& __y){
	return !(__x < __y);
}

template<class _Tp, class _Alloc>
inline uvector<_Tp, _Alloc>& uvector<_Tp, _Alloc>::operator=(const uvector<_Tp, _Alloc>& __x){
	if (&__x != this){
		const size_type __xlen = __x.size();
		if (__xlen > capacity()){
			iterator __tmp = _M_allocate_and_copy(__xlen, __x.begin(), __x.end());
			udestroy(begin(), end());
			__M_deallocate(__M_start, __M_end_of_storage - _M_start);
			__M_start = __temp;
			__M_end_of_storage = __M_start + __xlen;
		}
		else if (size() >= __xlen){
			iterator __i = ucopy(__x.begin(), __x.end(), begin());
			udestroy(__i, end());
		}
		else{
			ucopy(__x.begin(), __x.begin() + size(), begin());
			uuninitialized_copy(__x.begin() + size(), __x.end(), end());
		}
		__M_finish = __M_start + __xlen;
	}
	return *this;
}

template <class _Tp, class _Alloc>
void uvector<_Tp, _Alloc>::_M_insert_aux(iterator __position, const _Tp& __x = _Tp()){
	if (_M_finish != _M_end_of_storage){
		uconstruct(__M_finish, *(__M_finish - 1));
		++__M_finish;
		ucopy_backward(__position, __M_finish - 2, __M_finish - 1);
		*__position = __x;
	}
	else{
		const size_type __old_size = size();
		const size_type __new_size = 0 != __old_size ? 2 * __old_size : 1;
		iterator __new_start = _M_allocate(__new_size);
		iterator __new_finish = __new_start;
		__U_STL_TRY{
			__new_finish = uninitialized_copy(__M_start, __position, __new_start);
			uconstruct(__new_finish, __x);
			++__new_finish;
			__new_finish = uninitialized_copy(__position, __M_finish, __new_finish);
		}__U_STL_UNWIND(udestroy(__new_start, __new_finish), __M_deallocate(__new_start, __new_size));

		udestroy(begin(), end());
		__M_deallocate(begin(), size());
		__M_start = __new_start;
		__M_finish = __new_finish;
		_M_end_of_storage = __new_start + __new_size;
	}
}

template<class _Tp, class _Alloc>
void uvector<_Tp, _Alloc>::insert(iterator __position, const_iterator __start, const_iterator __last){
	if (__last != __start){
		size_type __n = udistance(__start, __last);
		if (size_type(__M_end_of_storage - __M_finish) >= __n){
			size_type __elems_after = __M_finish - __position;
			iterator __old_finish = __M_finish;
			if (__elems_after >= __n){
				uuninitialized_copy(__M_finish - __n, __M_finish, __M_finish);
				ucopy_backward(__position, __M_finish - __n, __M_finish);
				copy(__start, __last, __position);
				__M_finish += __n;
			}
			else{
				__M_finish = uuninitialized_copy(__start + __elems_after, __last, __M_finish);
				__M_finish = uuninitialized_copy(__position, __old_finish, __M_finish);
				copy(__start, __start + __elems_after, __position);
			}
		}
		else{
			size_type __old_size = size();
			size_type __len = __old_size + umax(__n, __old_size);
			iterator __new_start = _M_allocate(__len);
			iterator __new_finish = __new_start;
			__U_STL_TRY{
				__new_finish = uuninitialized_copy(__M_start, __postion, __new_start);
				__new_finish = uuninitialized_copy(__start, __last, __new_finish);
				__new_finish = uuninitialized_copy(__position, __M_finish, __new_finish);
			}__U_STL_UNWIND(udestroy(__new_start, __new_finish), _M_deallocate(__new_start, __len));
			udestroy(__M_start, __M_finish);
			_M_deallocate(__M_start, __M_end_of_storage - __M_start);
			__M_start = __new_start;
			__M_finish = __new_finish;
			__M_end_of_storage = __new_start + __len;
		}
	}
}

template<class _Tp, class _Alloc>
void uvector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n, const _Tp& __x){
	if (__n != 0){
		if (size_type(__M_end_of_storage - __M_finish) >= __n){
			size_type __elems_after = __M_finish - __position;
			iterator __old_finish = __M_finish;
			if (__elems_after >= __n){
				uuninitialized_copy(__M_finish - __n, __M_finish, __M_finish);
				ucopy_backward(__position, __M_finish - __n, __M_finish);
				fill(__position, __position + __n, __x);
				__M_finish += __n;
			}
			else{
				__M_finish = uuninitialized_fill_n(__M_finish, __n - __elems_after, __x);
				__M_finish = uuninitialized_copy(__position, __old_finish, __M_finish);
				fill(__position, __old_finish, __x);
			}
		}
		else{
			size_type __old_size = size();
			size_type __len = __old_size + umax(__n, __old_size);
			iterator __new_start = _M_allocate(__len);
			iterator __new_finish = __new_start;
			__U_STL_TRY{
				__new_finish = uuninitialized_copy(__M_start, __postion, __new_start);
				__new_finish = uuninitialized_fill_n(__new_finish, __n, __x);
				__new_finish = uuninitialized_copy(__position, __M_finish, __new_finish);
			}__U_STL_UNWIND(udestroy(__new_start, __new_finish), _M_deallocate(__new_start, __len));
			udestroy(__M_start, __M_finish);
			_M_deallocate(__M_start, __M_end_of_storage - __M_start);
			__M_start = __new_start;
			__M_finish = __new_finish;
			__M_end_of_storage = __new_start + __len;
		}
	}
}

__U_STL_END_NAMESPACE

#endif