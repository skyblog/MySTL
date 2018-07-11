#ifndef UNINITIALIZEDFUNCTION_H_INCLUDED
#define UNINITIALIZEDFUNCTION_H_INCLUDED

//#include "Algorithm.h"
#include <Algorithm>
#include <cstring>
#include "Construct.h"
#include "Iterator.h"
#include "TypeTraits.h"

namespace MySTL{
    //-------------------------uninitialized_copy---------------------------------

    //[result, result+(last-first))
    template<class InputIterator, class ForwardIterator, class T>
    inline ForwardIterator _uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*){
        typedef typename _type_traits<T>::is_POD_type isPODType;//POD:标量型别或传统的C struct型别
		return _uninitialized_copy_aux(first, last, result, isPODType());
    }
	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _true_type){//是POD：最有效率的初值填写手法
		memcpy(result, first, (last - first) * sizeof(*first));
		return result + (last - first);
	}
	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _false_type){//Not POD，最保险安全的construct
		int i = 0;
		for (; first != last; ++first, ++i){
			construct((result + i), *first);
		}
		return (result + i);
	}

    template<class InputIterator, class ForwardIterator>
	inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result){
		return _uninitialized_copy(first, last, result, value_type(result));
	}

	//------------------------uninitialized_fill--------------------------
	template<class ForwardIterator, class T, class T1>
	inline void _uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value, T1*){
	    typedef typename _type_traits<T1>::is_POD_type isPODType;
		_uninitialized_fill_aux(first, last, value, isPODType());
	}

	template<class ForwardIterator, class T>
	inline void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, _true_type){
		std::fill(first, last, value);
	}
	template<class ForwardIterator, class T>
	inline void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, _false_type){
		for (; first != last; ++first){
			construct(first, value);
		}
	}
	template<class ForwardIterator, class T>
	inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value){
		_uninitialized_fill(first, last, value, value_type(first));
	}

	//------------------------uninitialized_fill_n-------------------------


	template<class ForwardIterator, class Size, class T, class T1>
	inline ForwardIterator _uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*){
	    typedef typename _type_traits<T1>::is_POD_type isPODType;
		return _uninitialized_fill_n_aux(first, n, x, isPODType());
	}

	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, _true_type){
		return std::fill_n(first, n, x);
	}
	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, _false_type){
		int i = 0;
		for (; i != n; ++i){
			construct((T*)(first + i), x);
		}
		return (first + i);
	}

	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x){
		return _uninitialized_fill_n(first, n, x, value_type(first));
	}
}


#endif // UNINITIALIZEDFUNCTION_H_INCLUDED
