#ifndef __U_STL_INTERNAL_TREE_H
#define __U_STL_INTERNAL_TREE_H

#include "ustl_alloc.h"
#include "ustl_construct.h"
#include "ustl_algobase.h"
#include "ustl_iterator_base.h"


typedef bool _Rb_tree_Color_type;
const _Rb_tree_Color_type _S_rb_tree_red = false;
const _Rb_tree_Color_type _S_rb_tree_black = true;

struct _Rb_tree_node_base{
	typedef _Rb_tree_Color_type _Color_type;
	typedef _Rb_tree_node_base* _Base_ptr;

	_Color_type _M_color;
	_Base_ptr _M_parent;
	_Base_ptr _M_left;
	_Base_ptr _M_right;

	static _Base_ptr _S_minimum(_Base_ptr __x){
		while (__x->_M_left != 0) __x = __x->_M_left;
		return __x;
	}
	static _Base_ptr _S_maximum(_Base_ptr __x){
		while (__x->_M_right != 0) __x = __x->_M_right;
		return __x;
	}
};

template<class _Val>
struct _Rb_tree_node : public _Rb_tree_node_base{
	typedef _Rb_tree_node<_Val>* _Link_type;
	_Val _M_value_field;
};

struct _Rb_tree_base_iterator{
	typedef _Rb_tree_node_base::_Base_ptr _Base_ptr;
	typedef ubidirectional_iterator_tag	iterator_category;
	typedef ptrdiff_t	difference_type;
	_Base_ptr	_M_node;

	void _M_increment(){
		if (_M_node->_M_right != 0){
			_M_node = _M_node->_M_right;
			while (_M_node->_M_left != 0)
				_M_node = _M_node->_M_left;
		}
		else{
			_Base_ptr __y = _M_node->_M_parent;
			while (__y->_M_right == _M_node){
				_M_node = __y;
				__y = _M_node->_M_parent;
			}
			if (_M_node->_M_right != __y)
				_M_node = __y;
		}
	}

	void _M_decrement(){
		if (_M_node->_M_color == _S_rb_tree_red && _M_node->_M_parent->_M_parent == _M_node)
			_M_node = _M_node->_M_right;
		else if (_M_node->_M_left != 0){
			_M_node = _M_node->_M_left;
			while (_M_node->_M_right != 0)
				_M_node = _M_node->_M_right;
		}
		else{
			_Base_ptr __y = _M_node->_M_parent;
			while (__y->_M_left == _M_node){
				_M_node = __y;
				__y = _M_node->_M_parent;
			}
			_M_node = __y;
		}
	}
};

template<class _Val, class _Ref, class _Ptr>
struct _Rb_tree_iterator : public _Rb_tree_base_iterator{
	typedef _Val												value_tyep;
	typedef _Ref												reference;
	typedef _Ptr												pointer;
	typedef _Rb_tree_iterator<_Val, _Val&, _Val*>				iterator;
	typedef _Rb_tree_iterator<_Val, const _Val&, const _Val&>	const_iterator;
	typedef _Rb_tree_iterator<_Val, _Ref, _Ptr>					_Self;
	typedef _Rb_tree_node<_Val>*								_Link_type;

	_Rb_tree_iterator(){}
	_Rb_tree_iterator(_Link_type __x){ _M_node = __x; }
	_Rb_tree_iterator(const iterator& __x){ _M_node = __x._M_node; }

	reference operator*(){ return (_Link_type)_M_node->_M_value_field; }
	pointer operator->(){ return &(operator*()); }

	_Self& operator++(){ _M_increment(); return *this; }
	_Self operator++(int){
		_Self __tmp = *this;
		_M_increment();
		return __tmp;
	}
	_Self& operator--(){ _M_decrement(); return *this; }
	_Self operator--(int){
		_Self __tmp = *this;
		_M_decrement();
		return __tmp;
	}
};

inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y){
	return __x._M_node == __y._M_node;
}
inline bool operator!=(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y){
	return __x._M_node != __y._M_node;
}

inline ubidirectional_iterator_tag uiterator_category(const _Rb_tree_base_iterator&){
	return ubidirectional_iterator_tag();
}

inline _Rb_tree_base_iterator::difference_type* udistance_type(const _Rb_tree_base_iterator&){
	return (_Rb_tree_base_iterator::difference_type*)0;
}

template<class _Val, class _Ref, class _Ptr>
inline _Val* uvalue_type(const _Rb_tree_iterator<_Val, _Ref, _Ptr>&){
	return (_Val*) 0;
}

inline void _Rb_tree_rotate_left(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root){
	_Rb_tree_node_base* __y = __x->_M_right;

	__x->_M_right = __y->_M_left;
	if (__y->_M_left != 0)
		__y->_M_left->_M_parent = __x;

	__y->_M_parent = __x->_M_parent;
	if (__root == __x)
		__root = __y;
	else if (__x == __x->_M_parent->_M_left)
		__x->_M_parent->_M_left = __y;
	else
		__x->_M_parent->_M_right = __y;

	__y->_M_left = __x;
	__x->_M_parent = __y;
}

inline void _Rb_tree_rotate_right(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root){
	_Rb_tree_node_base* __y = __x->_M_left;

	__x->_M_left = __y->_M_right;
	if (__y->_M_right != 0)
		__y->_M_right->_M_parent = __x;

	__y->_M_parent = __x->_M_parent;
	if (__root == __x)
		__root = __y;
	else if (__x == __x->_M_parent->_M_left)
		__x->_M_parent->_M_left = __y;
	else
		__x->_M_parent->_M_right = __y;

	__y->_M_right = __x;
	__x->_M_parent = __y;
}

inline void _Rb_tree_rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root){
	__x->_M_color = _S_rb_tree_red;
	while (__x != __root && __x->_M_parent->_M_color == _S_rb_tree_red){
		if (__x->_M_parent == __x->_M_parent->_M_parent->_M_left){
			_Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_right;
			if (__y && __y->_M_color == _S_rb_tree_red){
				__x->_M_parent->_M_color = _S_rb_tree_black;
				__y->_M_color = _S_rb_tree_black;
				__x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
				__x = __x->_M_parent->_M_parent;
			}
			else{
				if (__x == __x->_M_parent->_M_right){
					__x = __x->_M_parent;
					_Rb_tree_rotate_left(__x, __root);
				}

				__x->_M_parent->_M_color = _S_rb_tree_black;
				__x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
				_Rb_tree_rotate_right(__x->_M_parent->_M_parent, __root);
			}
		}
		else{
			_Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_left;
			if (__y && __y->_M_color == _S_rb_tree_red){
				__x->_M_parent->_M_color = _S_rb_tree_black;
				__y->_M_color = _S_rb_tree_black;
				__x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
				__x = __x->_M_parent->_M_parent;
			}
			else{
				if (__x == __x->_M_parent->_M_left){
					__x = __x->_M_parent;
					_Rb_tree_rotate_right(__x, __root);
				}

				__x->_M_parent->_M_color = _S_rb_tree_black;
				__x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
				_Rb_tree_rotate_left(__x->_M_parent->_M_parent, __root);
			}
		}
	}
	__root->_M_color = _S_rb_tree_black;
}

inline _Rb_tree_node_base* _Rb_tree_rebalance_for_erase(_Rb_tree_node_base* __z, _Rb_tree_node_base*& __root, 
	_Rb_tree_node_base*& __leftmost, _Rb_tree_node_base*& __rightmost){
	_Rb_tree_node_base* __y = __z;
	_Rb_tree_node_base* __x = 0;
	_Rb_tree_node_base* __x_parent = 0;
	if (__y->_M_left == 0)
		__x = __y->_M_right;
	else if (__y->_M_right == 0)
		__x = __y->_M_left;
	else{
		__y = __y->_M_right;
		while (__y->_M_left != 0)
			__y = __y->_M_left;
		__x = __y->_M_right;
	}
	if (__y != __z){
		__y->_M_left = __z->_M_left;
		__z->_M_left->_M_parent = __y;
		if (__z->_M_right != __y){
			__x_parent = __x->_M_parent;
			__y->_M_parent->_M_left = __x;
			if (__x) __x->_M_parent = __y->_M_parent;
			__y->_M_right = __z->_M_right;
			__z->_M_right->_M_parent = __y;
		}
		else
			__x_parent = __y;
		__y->_M_parent = __z->_M_parent;
		if (__z == __root)
			__root = __y;
		else
			__y->_M_parent = __z->_M_parent;

		//TODO: implement swap function
		_Rb_tree_Color_type __tmp = __y->_M_color;
		__y->_M_color = __z->_M_color;
		__z->_M_color = __tmp;

		__y = __z;
	}
	else{
		__x_parent = __y->_M_parent;
		if(__x) __x->_M_parent = __y->_M_parent;
		if (__root == __z)
			__root = __x;
		else if (__y == __y->_M_parent->_M_left)
			__y->_M_parent->_M_left = __x;
		else
			__y->_M_parent->_M_right = __x;

		if (__leftmost == __z)
			if (__z->_M_right == 0) 
				__leftmost = __z->_M_parent;
			else
				__leftmost = _Rb_tree_node_base::_S_minimum(__x);
		if (__rightmost == __z)
			if (__z->_M_left == 0)
				__rightmost = __z->_M_parent;
			else
				__rightmost = _Rb_tree_node_base::_S_maximum(__x);
	}

	if (__y->_M_color != _S_rb_tree_red){
		while (__x != __root && (__x == 0 || __x->_M_color == _S_rb_tree_black))
			if (__x == __x_parent->_M_left){
				_Rb_tree_node_base *__w = __x_parent->_M_right;
				if (__w->_M_color == _S_rb_tree_red){
					__x_parent->_M_color = _S_rb_tree_red;
					__w->_M_color = _S_rb_tree_black;
					_Rb_tree_rotate_left(__x_parent, __root);
					__w = __x_parent->_M_right;
				}

				if ((__w->_M_left == 0 || __w->_M_left->_M_color == _S_rb_tree_black) &&
					(__w->_M_right == 0 || __w->_M_right->_M_color == _S_rb_tree_black)){
					__w = _S_rb_tree_red;
					__x = __x_parent;
					__x_parent = __x_parent->_M_parent;
				}
				else{
					if (__w->_M_right == 0 || __w->_M_right->_M_color == _S_rb_tree_black){
						__w->_M_color = _S_rb_tree_red;
						if (__w->_M_left)__w->_M_left->_M_color = _S_rb_tree_black;
						_Rb_tree_rotate_right(__w, __root);
						__w = __x_parent->_M_right;
					}

					__w->_M_color = __x_parent->_M_color;
					if(__w->_M_right) __w->_M_right->_M_color = _S_rb_tree_black;
					__x_parent->_M_color = _S_rb_tree_black;
					_Rb_tree_rotate_left(__x_parent, __root);
					break;
				}
			}
			else{
				_Rb_tree_node_base *__w = __x_parent->_M_left;
				if (__w->_M_color == _S_rb_tree_red){
					__x_parent->_M_color = _S_rb_tree_red;
					__w->_M_color = _S_rb_tree_black;
					_Rb_tree_rotate_right(__x_parent, __root);
					__w = __x_parent->_M_left;
				}

				if ((__w->_M_left == 0 || __w->_M_left->_M_color == _S_rb_tree_black) &&
					(__w->_M_right == 0 || __w->_M_right->_M_color == _S_rb_tree_black)){
					__w = _S_rb_tree_red;
					__x = __x_parent;
					__x_parent = __x_parent->_M_parent;
				}
				else{
					if (__w->_M_left == 0 || __w->_M_left->_M_color == _S_rb_tree_black){
						__w->_M_color = _S_rb_tree_red;
						if (__w->_M_right)__w->_M_right->_M_color = _S_rb_tree_black;
						_Rb_tree_rotate_left(__w, __root);
						__w = __x_parent->_M_left;
					}

					__w->_M_color = __x_parent->_M_color;
					if (__w->_M_left) __w->_M_left->_M_color = _S_rb_tree_black;
					__x_parent->_M_color = _S_rb_tree_black;
					_Rb_tree_rotate_right(__x_parent, __root);
					break;
				}
			}
			if (__x)__x->_M_color = _S_rb_tree_black;
	}

	return __y;
}

template<class _Tp, class _Alloc>
struct _Rb_tree_base{
	typedef _Alloc allocator_type;
	allocator_type get_allocator() const{ return allocator_type(); }

	_Rb_tree_base( const allocator_type& ) : _M_header(0){ _M_header = _M_get_node(); }
	~_Rb_tree_base(){ _M_put_node(_M_header); }

protected:
	typedef usimple_alloc<_Rb_tree_node<_Tp>, _Alloc> _Alloc_type;
	_Rb_tree_node<_Tp>* _M_header;

	_Rb_tree_node<_Tp>* _M_get_node(){
		return _Alloc_type::allocator(1);
	}
	void _M_put_node(_Rb_tree_node<_Tp>* __p){
		_Alloc_type::deallcator(__p, 1);
	}
};

template<class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = __U_STL_DEFAULT_ALLOCATOR(_Value)>
class _Rb_tree : protected _Rb_tree_base < _Value, _Alloc > {
	typedef _Rb_tree_base<_Value, _Alloc> _Base;
protected:
	typedef _Rb_tree_node_base* _Base_ptr;
	typedef _Rb_tree_node<_Value> _Rb_tree_node;
	typedef _Rb_tree_Color_type _Color_type;
public:
	typedef _Key key_type;
	typedef _Value value_type;
	typedef value_type*  pointer;
	typedef const value_type* const_point;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef _Rb_tree_node* _Link_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef typename _Base::allocator_type allocator_type;
	allocator_type get_allocator() const { return _Base::get_allocator(); }
	
protected:
	_Link_type _M_create_node(const value_type& __x){
		_Link_type __tmp = _M_get_node();
		__U_STL_TRY{
			uconstruct(&__tmp->_M_value_field, __x);
		}__U_STL_UNWIND(_M_put_node(__tmp));

		return __tmp;
	}

	_Link_type _M_clone_node(_Link_type __x){
		_Link_type __tmp = _M_create_node(__x->_M_value_field);
		__tmp->_M_color = __x->_M_color;
		__tmp->_M_left = 0;
		__tmp->_M_right = 0;
		return __tmp;
	}

	void destroy_node(_Link_type __p){
		udestroy(&__p->_M_value_field);
		_M_put_node(__p);
	}

protected:
	size_type _M_node_count;
	_Compare _M_key_compare;

	_Link_type& _M_root()const { return (_Link_type&)_M_header->_M_parent; }
	_Link_type& _M_leftmost()const { return (_Link_type&)_M_header->_M_left; }
	_Link_type& _M_rightmost()const { return (_Link_type&)_M_header->_M_right; }

	static _Link_type& _S_left(_Link_type __x)
	{
		return (_Link_type&)(__x->_M_left);
	}
	static _Link_type& _S_right(_Link_type __x)
	{
		return (_Link_type&)(__x->_M_right);
	}
	static _Link_type& _S_parent(_Link_type __x)
	{
		return (_Link_type&)(__x->_M_parent);
	}
	static reference _S_value(_Link_type __x)
	{
		return __x->_M_value_field;
	}
	static const _Key& _S_key(_Link_type __x)
	{
		return _KeyOfValue()(_S_value(__x));
	}
	static _Color_type& _S_color(_Link_type __x)
	{
		return (_Color_type&)(__x->_M_color);
	}

	static _Link_type& _S_left(_Base_ptr __x)
	{
		return (_Link_type&)(__x->_M_left);
	}
	static _Link_type& _S_right(_Base_ptr __x)
	{
		return (_Link_type&)(__x->_M_right);
	}
	static _Link_type& _S_parent(_Base_ptr __x)
	{
		return (_Link_type&)(__x->_M_parent);
	}
	static reference _S_value(_Base_ptr __x)
	{
		return ((_Link_type)__x)->_M_value_field;
	}
	static const _Key& _S_key(_Base_ptr __x)
	{
		return _KeyOfValue()(_S_value(_Link_type(__x)));
	}
	static _Color_type& _S_color(_Base_ptr __x)
	{
		return (_Color_type&)(_Link_type(__x)->_M_color);
	}

	static _Link_type _S_minimum(_Link_type __x)
	{
		return (_Link_type)_Rb_tree_node_base::_S_minimum(__x);
	}

	static _Link_type _S_maximum(_Link_type __x)
	{
		return (_Link_type)_Rb_tree_node_base::_S_maximum(__x);
	}

public:
	typedef _Rb_tree_iterator<value_type, pointer, reference> iterator;
	typedef _Rb_tree_iterator<value_type, const_point, const_reference> const_iterator;

private:
	iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v);
	_Link_type _M_copy(_Link_type __x, _Link_type __p);
	void _M_erase(_Link_type __x);

	void _M_empty_initialize(){
		_S_color(_M_header) = _S_rb_tree_red;

		_M_root() = 0;
		_M_leftmost() = _M_header;
		_M_rightmost() = _M_header;
	}

public:
	_Rb_tree() : _Base(allocator_type()), _M_node_count(0), _M_key_compare(){
		_M_empty_initialize();
	}

	_Rb_tree(const _Compare& _comp) : _Base(allocator_type()), _M_node_count(0), _M_key_compare(_comp){
		_M_empty_initialize();
	}

	_Rb_tree(const _Compare& _comp, const allocator_type& __a) : _Base(__a), _M_node_count(0), _M_key_compare(_comp){
		_M_empty_initialize();
	}

	_Rb_tree(const _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>& __x) 
		: _Base(__x.get_allocator()), _M_node_count(0), _M_key_compare(__x._M_key_compare){
		if (__x._M_root == 0)
			_M_empty_initialize();
		else{
			_S_color(_M_header) = _S_rb_tree_red;
			_M_copy(__x._M_root(), _M_header);
			_M_leftmost() = _S_minimum(_M_root());
			_M_rightmost() = _S_maximum(_M_root());
		}
		_M_node_count = __x->_M_node_count;
	}
	~_Rb_tree(){ clear(); }

	_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>&
		operator=(const _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>& __x);

	void erase(iterator __position);
	size_type erase(const key_type& __x);
	void erase(iterator __first, iterator __last);
	void erase(const key_type* __first, const key_type* __last);
	void clear(){
		if (_M_node_count != 0){
			_M_erase(_M_root());
			_M_leftmost() = _M_header;
			_M_root() = 0;
			_M_rightmost() = _M_header;
			_M_node_count = 0;
		}
	}

	_Compare key_comp(){ return _M_key_compare; }
	iterator begin(){ return _M_leftmost(); }
	const_iterator begin(){ return _M_leftmost(); }
	iterator end(){ return _M_header; }
	const_iterator end(){ return _M_header; }
	//TODO: rbegin and rend() function

	bool empty(){ return _M_node_count == 0; }
	size_type size(){ return _M_node_count; }
	size_type max_size(){ return size_type(-1); }

	void swap(_Rb_tree< _Key, _Value, _KeyOfValue, _Compare, _Alloc>& __t){
		uswap(_M_header, __t._M_header);
		uswap(_M_node_count, __t._M_node_count);
		uswap(_M_key_compare, __t._M_key_compare);
	}
	
	upair<iterator, bool> insert_unique(const value_type& __x);
	iterator insert_equal(const value_type& __x);
	iterator insert_unique(iterator __position, const value_type& __x);
	iterator insert_equal(iterator __position, const value_type& __x);

	void insert_unique(const_iterator __first, const_iterator __last);
	void insert_unique(const value_type* __first, const value_type* __last);
	void insert_equal(const_iterator __first, const_iterator __last);
	void insert_equal(const value_type* __first, const value_type* __last);

	pair<iterator, iterator> equal_range(const key_type& __x);
	pair<const_iterator, const_iterator> equal_range(const key_type& __x) const;
	
};

template<class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_equal(iterator __position, const value_type& __v){
	if (__position._M_node == _M_header->_M_left){
		if (size()>0 && !_M_key_compare(_S_key(__position._M_node, _KeyOfValue()(__v))))
			return _M_insert(__position._M_node, __position._M_node, __v);
		else
			return insert_equal(__v);
	}
	else if (__position._M_node == _M_header){
		if (!_M_key_compare(_KeyOfValue()(__v), _S_key(_M_rightmost())))
			return _M_insert(0, _M_rightmost(), __v);
		else
			return insert_equal(__v);
	}
	else{
		iterator __before = __position;
		--__before;
		if (!_M_key_compare(_KeyOfValue()(__v), _S_key(__before._M_node)) && !_M_key_compare(_S_key(__position._M_node), _KeyOfValue()(__v))){
			if (_S_right(__before._M_node) == 0)
				return _M_insert(0, __before._M_node);
			else
				return _M_insert(__position._M_node, __position._M_node, __v);
		}
		else
			return insert_unique(__v).first;
	}
}

template<class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(iterator __position, const value_type& __v){
	if (__position._M_node == _M_header->_M_left){
		if (size()>0 && _M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node)))
			return _M_insert(__position._M_node, __position._M_node, __v);
		else
			return insert_unique(__v).first;
	}
	else if (__position._M_node == _M_header){
		if (_M_key_compare(_S_key(_M_rightmost()), _KeyOfValue()(__v)))
			return _M_insert(0, _M_rightmost(), __v);
		else
			return insert_unique(__v).first;
	}
	else{
		iterator __before = __position;
		--__before;
		if (_M_key_compare(_S_key(__before), _KeyOfValue()(__v)) && _M_key_compare(_KeyOfValue()(__v), _S_key(__position))){
			if (_S_right(__before._M_node) == 0)
				return _M_insert(0, __before._M_node);
			else
				return _M_insert(__position._M_node, __position._M_node, __v);
		}
		else
			return insert_unique(__v).first;
	}
}

template<class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_equal(const value_type& __v){
	_Link_type __y = _M_header;
	_Link_type __x = _M_root();

	while (__x != 0){
		__y = __x;
		__x = _M_key_compare(_KeyOfValue()(__v), _S_key(__x)) ? _S_left(__x) : _S_right(__x);
	}

	return _M_insert(__x, __y, __v);
}

template<class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
upair<typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator, bool> 
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(const value_type& __v){
	_Link_type __y = _M_root;
	_Link_type __x = _M_root();
	bool __comp = true;

	while (__x != 0){
		__y = __x;
		__comp = _M_key_compare()(_KeyOfValue(__v), _S_key(__x));
		__x = __comp ? _S_left(__x) : _S_right(__x);
	}

	iterator __j = iterator(__y);
	if (__comp)
		if (__j == begin())
			return upair<iterator, bool>(_M_insert(__x, __y, __v), true);
		else
			--j;
	if (_M_key_compare(_S_key(__j._M_node), _KeyOfValue(__v)))
		return upair<iterator, bool>(_M_insert(__x, __y, __v), true);
	return upair<iterator, bool>(__j, false);
}


template <class _Key, class _Val, class _KoV, class _Cmp, class _Alloc>
void
_Rb_tree<_Key, _Val, _KoV, _Cmp, _Alloc>
::insert_equal(const _Val* __first, const _Val* __last)
{
	for (; __first != __last; ++__first)
		insert_equal(*__first);
}

template <class _Key, class _Val, class _KoV, class _Cmp, class _Alloc>
void
_Rb_tree<_Key, _Val, _KoV, _Cmp, _Alloc>
::insert_equal(const_iterator __first, const_iterator __last)
{
	for (; __first != __last; ++__first)
		insert_equal(*__first);
}

template <class _Key, class _Val, class _KoV, class _Cmp, class _Alloc>
void
_Rb_tree<_Key, _Val, _KoV, _Cmp, _Alloc>
::insert_unique(const _Val* __first, const _Val* __last)
{
	for (; __first != __last; ++__first)
		insert_unique(*__first);
}

template <class _Key, class _Val, class _KoV, class _Cmp, class _Alloc>
void _Rb_tree<_Key, _Val, _KoV, _Cmp, _Alloc>
::insert_unique(const_iterator __first, const_iterator __last)
{
	for (; __first != __last; ++__first)
		insert_unique(*__first);
}

template<class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_insert(_Base_ptr __x_, _Base_ptr __y_, const _Value& __v){
	_Link_type __x = (_Link_type)__x_;
	_Link_type __y = (_Link_type)__y_;
	_Link_type __z;

	if (__y == _M_header || __x != 0 || _M_key_compare(_KeyOfValue()(__v), _S_key(__y))){
		__z = _M_create_node(__v);
		_S_left(__y) = __z;

		if (__y == _M_header){
			_M_root() = __z;
			_M_leftmost() = __z;
			_M_rightmost() = __z;
		}
		else if (__y == _M_leftmost())
			_M_leftmost() = __z;
	}
	else{
		__z = _M_create_node(__v);
		_S_right(__y) = __z;
		if (__y == _M_rightmost()) _M_rightmost() = __z;
	}

	_S_parent(__z) = __y;
	_S_right(__z) = 0;
	_S_left(__z) = 0;
	_Rb_tree_rebalance(__z, _M_header->_M_parent);
	++_M_node_count;
	return iterator(__z);
}

template<class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Link_type 
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_copy(_Link_type __x, _Link_type __p){
	_Link_type __top = _M_clone_node(__x);
	__top->_M_parent = __p;

	__U_STL_TRY{
		if (__x->_M_right) __top->_M_right = _M_copy(_S_right(__x), __top);
		__p = __top;
		__x = _S_left(__x);

		while (__x != 0){
			_Link_type __y = _M_clone_node(__x);
			__y->_M_parent = __p;
			__p->_M_left = __y;

			if (__x->_M_right)
				__y->_M_right = _M_copy(_S_right(__x), __y);
			__p = __y;
			__x = _S_left(__x);
		}
	}__U_STL_UNWIND(_M_erase(__top));
	
	return __top;
}

template<class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_erase(_Link_type __x){
	while (__x != 0){
		_M_erase(_S_right(__x));
		_Link_type __y = _S_left(__x);
		destroy_node(__x);
		__x = __y;
	}
}

template<class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
inline void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::erase(iterator __position){
	_Link_type __y = (_Link_type)_Rb_tree_rebalance_for_erase(__position._M_node, _M_header->_M_parent, _M_header->_M_left, _M_header->_M_right);
	destroy_node(__y);
	--_M_node_count;
}

template<class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
inline void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::erase(iterator __first, iterator __last){
	if (__first == begin() && __last == end())
		clear();
	else
		while (__first != __last) erase(__first++);
}

#endif