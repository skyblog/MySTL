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
        new(p) T1(value);//��λnew
    }

    template<class T>
    inline void destroy(T *p){
        p->~T();
    }
    //[first,second)��Χ�ڵ��������������ж������������Ƿ����ͷſռ�֮ǰʲôҲ�����������ĳ����ֻ�л������ͻ������ͣ���������ʲôҲ���������������һ��Ч�ʵ��˷�
    //�������ж�̬�����ڴ��ָ�룬�������������б�����ʾ���ͷ����ռ�
    template<class ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator second){
        __destroy(first, second, value_type(first));//value_type(first)��õ�������ָ�������
    }


    //�ж�Ԫ�ص���ֵ�ͱ��Ƿ���trivial destructor
    template<class ForwardIterator, class T>
    inline void __destroy(ForwardIterator first, ForwardIterator second, T*){
        typedef typename _type_traits<T>::has_trivial_destructor trivial_destructor;
        __destroy_aux(first, second, trivial_destructor());//trivial_destructor()�жϸú����Ƿ��޹�ʹ��

    }

    //���Ԫ�ص���ֵ�ͱ���non-trivial destructor,�ж������������
    template <class ForwardIterator>
    inline void __destroy_aux(ForwardIterator first,ForwardIterator second, _false_type){
        for(; first != second; ++first){
            destroy(&*first);
        }
    }
    //���Ԫ�ص���ֵ�ͱ���trivial destructor��������������ֻ��ϵͳ�Դ���
    template <class ForwardIterator>
    inline void __destroy_aux(ForwardIterator ,ForwardIterator , _true_type){}

}




#endif // CONSTRUCT_H
