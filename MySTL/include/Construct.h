#ifndef CONSTRUCT_H
#define CONSTRUCT_H

#include <new>

#include "TypeTraits.h"
#include "Iterator.h"

namespace MySTL
{
    template<class T1,class T2>
    inline void construct(T1* p, const T2& value);
    template<class T>
    inline void destroy(T *p);
    template<class ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator second);
    template<class ForwardIterator, class T>
    inline void __destroy(ForwardIterator first, ForwardIterator second, T*);
    template <class ForwardIterator>
    inline void __destroy_aux(ForwardIterator first,ForwardIterator second, _false_type);
    template <class ForwardIterator>
    inline void __destroy_aux(ForwardIterator ,ForwardIterator , _true_type);


    template<class T1,class T2>
    inline void construct(T1* p, const T2& value){
        new(p) T1(value);//定位new
    }

    template<class T>
    inline void destroy(T *p){
        p->~T();
    }
    //[first,second)范围内的析构掉，首先判断其析构函数是否在释放空间之前什么也不做（比如或某个类只有基本类型基本类型，），若其什么也不做，则调用它是一种效率的浪费
    //若类中有动态分配内存的指针，则在析构函数中必须显示的释放这块空间
    template<class ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator second){
        __destroy(first, second, value_type(first));//value_type(first)获得迭代器所指对象类别
    }


    //判断元素的数值型别是否有trivial destructor
    template<class ForwardIterator, class T>
    inline void __destroy(ForwardIterator first, ForwardIterator second, T*){
        typedef typename _type_traits<T>::has_trivial_destructor trivial_destructor;
        __destroy_aux(first, second, trivial_destructor());//trivial_destructor()判断该函数是否无关痛痒

    }

    //如果元素的数值型别有non-trivial destructor,有定义的析构函数
    template <class ForwardIterator>
    inline void __destroy_aux(ForwardIterator first,ForwardIterator second, _false_type){
        for(; first != second; ++first){
            destroy(&*first);
        }
    }
    //如果元素的数值型别有trivial destructor，无析构函数，只有系统自带的
    template <class ForwardIterator>
    inline void __destroy_aux(ForwardIterator ,ForwardIterator , _true_type){}

}




#endif // CONSTRUCT_H
