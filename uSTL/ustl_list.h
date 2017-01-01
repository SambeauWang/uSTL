#ifndef __U_STL_INTERNAL_LIST_H
#define __U_STL_INTERNAL_LIST_H

#include "ustl_config.h"
#include "ustl_iterator_base.h"
#include "ustl_alloc.h"
#include "ustl_construct.h"
#include "ustl_uninitialized.h"

// List node
struct _List_node_base{
	_List_node_base* _M_prev;
	_List_node_base* _M_next;
};

template<class _Tp>
struct _List_node : public _List_node_base{
	_Tp _M_data;
};

// List iterator
struct _List_iterator_base{
	typedef size_t						size_type;
	typedef ptrdiff_t					difference_type;
	typedef ubidirectional_iterator_tag	iterator_category;

	_List_node_base* _M_node;
	_List_iterator_base() : _M_node(){}
	_List_iterator_base(_List_node_base* __x) : _M_node(__x){}

	void _M_incr(){ _M_node = _M_node->_M_next; }
	void _M_decr(){ _M_node = _M_node->_M_prev; }

	bool operator==(const _List_iterator_base& __x) const{
		return _M_node == __x._M_node;
	}
	bool operator!=(const _List_iterator_base& __x) const{
		return _M_node != __x._M_node;
	}
};

template<class _Tp, class _Ref, class _Ptr>
struct _List_iterator : public _List_iterator_base{
	typedef _List_iterator<_Tp, _Ref, _Ptr>			_Self;
	typedef _List_iterator<_Tp, _Tp&, _Tp*>			iterator;
	typedef const _List_iterator<_Tp, _Tp&, _Tp*>	const_iterator;

	typedef	_Tp					value_type;
	typedef _Ptr				pointer;
	typedef	_Ref				reference;
	typedef _List_node<_Tp>		_Node;

	_List_iterator() :_List_iterator_base(){}
	_List_iterator(_Node* __x) : _List_iterator_base(__x){}
	_List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node){}

	reference operator*() const { return ((_List_node*)_M_node)->_M_data; }
	pointer	operator->() const { return &(operator*()); }

	_Self& operator++(){
		this->_M_incr();
		return *this;
	}
	_Self operator++(int){
		_Self __tmp = *this;
		this->_M_incr();
		return __tmp;
	}
	_Self& operator--(){
		this->_M_decr();
		return *this;
	}
	_Self operator--(int){
		_Self __tmp = *this;
		this->_M_decr();
		return __tmp;
	}
};

template<class _Tp, class _Alloc>
class _List_base{
public:
	typedef _Alloc	allocator_type;
	allocator_type get_allocator(){ return allocator_type(); }

	_List_base(const allocator_type&){
		_M_node = _M_get_node();
		_M_node->_M_next = _M_node;
		_M_node->_M_prev = _M_node;
	}
	~_List_base(){
		clear();
		_M_put_node(_M_node);
	}

	void clear();
protected:
	typedef usimple_alloc<_List_node<_Tp>, _Alloc>	_Alloc_type;
	_List_node<_Tp>* _M_get_node(){ return _Alloc_type::allocator(1); }
	void _M_put_node(_List_node<_Tp>* __p){ _Alloc_type::deallocator(__p, 1); }

private:
	_List_node<_Tp>* _M_node;
};

template<class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear(){
	_List_node<_Tp>* __cur = (_List_node<_Tp>*)_M_node->_M_next;
	while (__cur != _M_node){
		_List_node<_Tp>* __tmp = __cur;
		__cur = (_List_node<_Tp>*)__cur->_M_next;
		_uDestroy(&__tmp->_M_data);
		_M_put_node(__tmp);
	}
	_M_node->_M_next = _M_node;
	_M_node->_M_prev = _M_node;
}

template<class _Tp, class _Alloc = __U_STL_DEFAULT_ALLOCATOR(_Tp)>
class ulist : public _List_base < _Tp, _Alloc > {
protected:
	typedef _List_base<_Tp, _Alloc>	_Base;
	typedef void* Void_pointer;
public:
	typedef	_Tp											value_type;
	typedef value_type*									pointer;
	typedef const value_type*							const_pointer;
	typedef value_type&									reference;
	typedef	const value_type&							const_reference;
	typedef _List_iterator<_Tp, _Tp&, _Tp*>				iterator;
	typedef _List_iterator<_Tp, const _Tp&, const _Tp*>	const_iterator;
	typedef _List_node<_Tp>								_Node;
	typedef size_t										size_type;
	typedef ptrdiff_t									difference_type;

	typedef typename _Base::allocator_type	allocator_type;
	allocator_type get_allocator() const{ return _Base::allocator_type(); }

	//TODO: reverse iterator

protected:
	_Node* _M_create_node(const _Tp& __x){
		_Node* __p = _M_get_node();
		__U_STL_TRY{
			__uConstruct(&__p->_M_data, __x);
		}__U_STL_UNWIND(_M_put_node(__p));
		return __p;
	}

	_Node* _M_create_node(){
		_Node* __p = _M_get_node();
		__U_STL_TRY{
			__uConstruct(&__p->_M_data);
		}__U_STL_UNWIND(_M_put_node(__p));
		return __p;
	}
	
public:
	explicit ulist(const allocator_type& __a = allocator_type()):_Base(__a){}

	// begin() end() size() empty() front() back() function
	iterator begin(){ return (_Node*)(_M_node->_M_next); }
	const_iterator begin() const { return (_Node*)(_M_node->_M_next); }

	iterator end(){ return _M_node; }
	const_iterator end() const { return _M_node; }
	//TODO: rbegin and rend function

	bool empty() const { return _M_node == _M_node->_M_next; }
	size_type size(){ return udistance(begin(), end()); }
	size_type max_size(){ return size_type(-1); }

	reference front(){ return *begin(); }
	const_reference front() const { return *begin(); }
	reference back(){ return *(--end()); }
	const_reference back() const { return *(--end()); }

	// swap function
	void swap(ulist<_Tp, _Alloc>& __x){ uswap(_M_node, __x._M_node); }

	//insert function
	iterator insert(iterator __position, const _Tp& __x){
		_Node* __tmp = _M_create_node(__x);
		__tmp->_M_next = __position._M_node;
		__tmp->_M_prev = __position._M_node->_M_prev;
		__position._M_node->_M_prev->_M_next = __tmp;
		__position._M_node->_M_prev = __tmp;
		return __tmp;
	}
	iterator insert(iterator __position){ return insert(__position, _Tp()); }

	void insert(iterator __position, const _Tp* __first, const _Tp* __last);
	void insert(iterator __position, const_iterator __first, const_iterator __last);
	void _M_fill_insert(iterator __position, size_type __n, const _Tp& __x);
	void insert(iterator __position, size_type __n, const _Tp& __x){
		_M_fill_insert(__position, __n, __x);
	}

	// push_front and push_back function
	void push_front(const _Tp& __x){ insert(begin(), __x); }
	void push_front(){ insert(begin()); }
	void push_back(const _Tp& __x){ insert(end(), __x); }
	void push_back(){ insert(end()); }

	iterator erase(iterator __position){
		_List_node_base* __prev = __position._M_node->_M_prev;
		_List_node_base* __next = __position._M_node->_M_next;
		_Node* __n = (_Node*)__position._M_node;
		__prev->_M_next = __next;
		__next->_M_prev = __prev;
		_uDestroy(&__n->_M_data);
		_M_put_node(__n);
		return iterator((_Node*)__next);
	}
	iterator erase(iterator __first, iterator __last);
	void clear(){ _Base::clear(); }

	// resize function
	void resize(size_type __new_size, const _Tp& __x);
	void resize(size_type __new_size){ this->resize(__new_size, _Tp()); }

	// pop_front and pop_back
	void pop_front(){ erase(begin()); }
	void pop_back(){
		iterator __tmp = end();
		erase(--__tmp);
	}

	// construct function
	ulist(size_type __n, const _Tp& __x, const allocator_type& __a = allocator_type()) :_Base(__a){
		insert(begin(), __n, __x);
	}
	explicit ulist(size_type __n) :_Base(__a){
		insert(begin(), __n, _Tp());
	}
	ulist(const_iterator __first, const_iterator __last, const allocator_type& __a = allocator_type()):_Base(__a){
		this->insert(begin(), __first, __last);
	}
	ulist(const _Tp* __first, const _Tp* __last, const allocator_type& __a = allocator_type()) :_Base(__a){
		this->insert(begin(), __first, __last);
	}
	ulist(const ulist<_Tp, _Alloc>& __x) :_Base(__x.get_allocator()){
		insert(begin(), __x.begin(), __x.end());
	}

	~ulist(){}
	ulist<_Tp, _Alloc>& operator=(const ulist<_Tp, _Alloc>& __x);

	// assign function
	void _M_fill_assign(size_type __n, const _Tp& __val);
	void assign(size_type __n, const _Tp& __val){
		_M_fill_assign(__n, __val);
	}
	
	// transfer function
protected:
	void transfer(iterator __position, iterator __first, iterator __last){
		if (__first != __last){
			__last._M_node->_M_prev->_M_next = __position._M_node;
			__first._M_node->_M_prev->_M_next = __last._M_node;
			__position._M_node->_M_prev->_M_prev = __first._M_node;

			_List_node_base* __tmp = __position._M_node->_M_prev;
			__position._M_node->_M_prev = __last._M_node->_M_prev;
			__last._M_node->_M_prev = __first._M_node->_M_prev;
			__first._M_node->_M_prev = __tmp;
		}
	}

public:
	void splice(iterator __position, ulist& __x){
		if (!__x.empty()){
			this->transfer(__position, __x.begin(), __x.end());
		}
	}
	void splice(iterator __position, ulist& __x, iterator __i){
		iterator __j = __i;
		++__j;
		if (__i == __position && __j == __position) return;
		this->transfer(__position, __i, __j);
	}
	void splice(iterator __position, iterator __first, iterator __last){
		if (__first != __last)
			this->transfer(__position, __first, __last);
	}

	void remove(const _Tp& __val);
	void unique();
	void merge(ulist& __x);
	//void reverse();
	void sort();

	template <class _Predicate> void remove_if(_Predicate);
	template <class _BinaryPredicate> void unique(_BinaryPredicate);
	template <class _StrictWeakOrdering> void merge(ulist&, _StrictWeakOrdering);
	template <class _StrictWeakOrdering> void sort(_StrictWeakOrdering);
};

template<class _Tp, class _Alloc>
inline bool operator==(const ulist<_Tp, _Alloc>& __x, const ulist<_Tp, _Alloc>& __y){
	typedef typename ulist<_Tp, _Alloc>::const_iterator const_iterator;
	const_iterator __last1 = __x.end();
	const_iterator __last2 = __y.end();
	const_iterator __i1 = __x.begin();
	const_iterator __i2 = __y.begin();
	while (__i1 != __last1 && __i2 != __last2 && *__i1 == *__i2){
		++__i1;
		++__i2;
	}

	return __i1 == __last1 && __i2 == __last2;
}

template<class _Tp, class _Alloc>
inline bool operator<(const ulist<_Tp, _Alloc>& __x, const ulist<_Tp, _Alloc>& __y){
	return ulexicographical_compare(__x.begin(), __x.end(), __y.begin(), __y.end());
}

template<class _Tp, class _Alloc>
inline bool operator!=(const ulist<_Tp, _Alloc>& __x, const ulist<_Tp, _Alloc>& __y){
	return !(__x == __y);
}

template<class _Tp, class _Alloc>
inline bool operator>(const ulist<_Tp, _Alloc>& __x, const ulist<_Tp, _Alloc>& __y){
	return __y < __x;
}

template<class _Tp, class _Alloc>
inline bool operator<=(const ulist<_Tp, _Alloc>& __x, const ulist<_Tp, _Alloc>& __y){
	return !(__x > __y);
}

template<class _Tp, class _Alloc>
inline bool operator>=(const ulist<_Tp, _Alloc>& __x, const ulist<_Tp, _Alloc>& __y){
	return !(__x < __y);
}

template<class _Tp, class _Alloc>
inline void swap(ulist<_Tp, _Alloc>& __x, ulist<_Tp, _Alloc>& __y){
	__x.swap(__y);
}

// insert function
template<class _Tp, class _Alloc>
void ulist<_Tp, _Alloc>::insert(iterator __postion, const _Tp* __first, const _Tp* __last){
	for (; __first != __last; ++__first)
		insert(__position, *__first);
}

template<class _Tp, class _Alloc>
void ulist<_Tp, _Alloc>::insert(iterator __postion, const_iterator __first, const_iterator __last){
	for (; __first != __last; ++__first)
		insert(__position, *__first);
}

template<class _Tp, class _Alloc>
void ulist<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n, const _Tp& __x){
	for (; __n != 0; --__n)
		insert(__position, __x);
}

// erase function
template<class _Tp, class _Alloc>
typename ulist<_Tp, _Alloc>::iterator ulist<_Tp, _Alloc>::erase(iterator __first, iterator __last){
	while (__first != __last)
		erase(__first++);
	return __last;
}

// resize function
template<class _Tp, class _Alloc>
void ulist<_Tp, _Alloc>::resize(size_type __new_size, const _Tp& __x){
	iterator __i = begin();
	size_type __len = 0;
	for (; __i != end() && __len < __new_size; ++__len, ++__i);
	if (__len == __new_size)
		erase(__i, end());
	else
		insert(end(), __new_size - __len, __x);
}

template<class _Tp, class _Alloc>
ulist<_Tp, _Alloc>& ulist<_Tp, _Alloc>::operator=(const ulist<_Tp, _Alloc>& __x){
	if (&__x != this){
		iterator __first1 = begin();
		iterator __last1 = end();
		const iterator __first2 = __x.begin();
		const iterator __last2 = __x.end();
		while (__first1 != __last1 && __first2 != __last2)
			*first1++ = *first2++;
		if (__first2 == __last2)
			erase(__first1, __last1);
		else
			insert(__first1, __first2, __last2);
	}
	return *this;
}

template<class _Tp, class _Alloc>
void ulist<_Tp, _Alloc>::_M_fill_assign(size_type __n, const _Tp& __val){
	iterator __i = begin();
	for (; __i != end() && __n > 0; ++__i, --__n)
		*__i = __val;
	if (__n == 0)
		erase(__i, end());
	else
		insert(__i, __n, __val);
}

template<class _Tp, class _Alloc>
void ulist<_Tp, _Alloc>::remove(const _Tp& __val){
	iterator __first = begin();
	iterator __last = end();
	while (__first != __last){
		iterator __next = __first;
		++__next;
		if (*__first == __val) erase(__first);
		__first = __next;
	}
}

template<class _Tp, class _Alloc>
void ulist<_Tp, _Alloc>::unique(){
	iterator __first = begin();
	iterator __last = end();
	if (__first == __last) return;
	iterator __next = __first;
	while (++__next != __last){
		if (*__next == *__first)
			erase(__next);
		else
			__first = __next;
		__next = __first;
	}
}

template<class _Tp, class _Alloc>
void ulist<_Tp, _Alloc>::merge(ulist<_Tp, _Alloc>& __x){
	iterator __first1 = begin();
	iterator __last1 = end();
	iterator __first2 = __x.begin();
	iterator __last2 = __x.end();
	while (__first1 != __last1 && __first2 != __last2){
		if (*__first1 > *__first2){
			iterator __next = __first2;
			transfer(__first1, __first2, ++__next);
			__first2 = __next;
		}
		else
			++__first1;
	}
	if (__first2 != __last2) transfer(__last1, __first2, __last2);
}

//TODO: reverse function

template<class _Tp, class _Alloc>
void ulist<_Tp, _Alloc>::sort(){
	if (_M_node->_M_next != _M_node && _M_node->_M_next->_M_next != _M_node){
		ulist<_Tp, _Alloc> __carry;
		ulist<_Tp, _Alloc> __counter[64];
		int __fill = 0;
		while (!empty()){
			__carry.splice(__carry.begin(), *this, begin());
			int __i = 0;
			while (__i < __fill && !__counter[__i].empty()){
				__counter[__i].merge(__carry);
				__carry.swap(__counter[__i++]);
			}
			__counter[__i].swap(__carry);
			if (__i == __fill) ++__fill;
		}
		for (int __i = 1; __i < __fill; ++__i)
			__counter[__i].merge(__counter[__i - 1]);
		swap(__counter[__fill - 1]);
	}
}

template<class _Tp, class _Alloc>
template<class _Predicate>
void ulist<_Tp, _Alloc>::remove_if(_Predicate __pred){
	iterator __first = begin();
	iterator __last = end();
	while (__first != __last){
		iterator __next = __first;
		++__next;
		if (__pred(*__first)) erase(__first);
		__first = __next;
	}
}

template<class _Tp, class _Alloc>
template<class _BinaryPredicate>
void ulist<_Tp, _Alloc>::unique(_BinaryPredicate __binary_pred){
	iterator __first = begin();
	iterator __last = end();
	if (__first == __last) return;
	iterator __next = __first;
	while (++__next != __last){
		if (__binary_pred(*__first, *__next))
			erase(__next);
		else
			__first = __next;
		__next = __first;
	}
}

template<class _Tp, class _Alloc>
template<class _StrictWeakOrdering>
void ulist<_Tp, _Alloc>::merge(ulist<_Tp, _Alloc>& __x, _StrictWeakOrdering __comp){
	iterator __first1 = begin();
	iterator __last1 = end();
	iterator __first2 = __x.begin();
	iterator __last2 = __x.end();
	while (__first1 != __last1 && __first2 != __last2){
		if (__comp(*__first2, *__first1)){
			iterator __next = __first2;
			transfer(__first1, __first2, ++__next);
			__first2 = __next;
		}
		else
			++__first1;
	}
	if (__first2 != __last2) transfer(__last1, __first2, __last2);
}

template<class _Tp, class _Alloc>
template <class _StrictWeakOrdering>
void ulist<_Tp, _Alloc>::sort(_StrictWeakOrdering __comp){
	if (_M_node->_M_next != _M_node && _M_node->_M_next->_M_next != _M_node){
		ulist<_Tp, _Alloc> __carry;
		ulist<_Tp, _Alloc> __counter[64];
		int __fill = 0;
		while (!empty()){
			__carry.splice(__carry.begin(), *this, begin());
			int __i = 0;
			while (__i < __fill && !__counter[__i].empty()){
				__counter[__i].merge(__carry, __comp);
				__carry.swap(__counter[__i++]);
			}
			__counter[__i].swap(__carry);
			if (__i == __fill) ++__fill;
		}
		for (int __i = 1; __i < __fill; ++__i)
			__counter[__i].merge(__counter[__i - 1], __comp);
		swap(__counter[__fill - 1]);
	}
}

#endif