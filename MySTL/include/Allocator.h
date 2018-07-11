#ifndef ALLOCATOR_H_INCLUDED
#define ALLOCATOR_H_INCLUDED

#include "Alloc.h"
#include "Construct.h"

#include <cassert>
#include <new>
#include <stddef.h>

namespace MySTL{

    template<class T>
    class Allocator{
    public:
        typedef T     value_type;
        typedef T*    pointer;
        typedef const T*	const_pointer;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;
    public:
        static pointer allocate();
		static pointer allocate(size_type n);
		static void deallocate(pointer ptr);
		static void deallocate(pointer ptr, size_type n);

		static void construct(pointer ptr);
		static void construct(pointer ptr, const_reference value);
		static void destroy(pointer ptr);
		static void destroy(pointer first, pointer last);
    };


    template<class T>
    T* Allocator<T>::allocate(){
        return static_cast<pointer>(Alloc::allocate(sizeof(T)));
    }
    template<class T>
    T* Allocator<T>::allocate(size_type n){
        if (0==n) return 0;
        return static_cast<pointer>(Alloc::allocate(n*sizeof(T)));
    }

    template<class T>
    void Allocator<T>::deallocate(pointer ptr){
        Alloc::deallocate(static_cast<void*>(ptr),sizeof(T));
    }

    template<class T>
    void Allocator<T>::deallocate(pointer ptr, size_type n){
        if(0==n) return;
        Alloc::deallocate(static_cast<void*>(ptr),n*sizeof(T));
    }

    template<class T>
    void Allocator<T>::construct(pointer ptr){
        new(ptr)T();
    }

    template<class T>
    void Allocator<T>::construct(pointer ptr, const_reference value){
        new(ptr)T(value);
    }
    template<class T>
	void Allocator<T>::destroy(pointer ptr){
		ptr->~T();
	}
	template<class T>
	void Allocator<T>::destroy(pointer first, pointer last){
		for (; first != last; ++first){
			first->~T();
		}
	}

}


#endif // ALLOCATOR_H_INCLUDED
