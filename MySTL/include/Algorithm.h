#ifndef ALGORITHM_H_INCLUDED
#define ALGORITHM_H_INCLUDED


#include "Allocator.h"
#include "Construct.h"
#include "UninitializedFunction.h"
#include "TypeTraits.h"
#include "Iterator.h"

namespace MySTL
{

    //***************heap相关*******************
    //push_heap

    template <class RandomAccessIterator, class Distance, class T, class Compare>
    void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value, Compare comp)//参数：第一个所在位置，最后一个index，第一个index，插入值
    {
        Distance parent = (holeIndex-1)/2;//父节点

        while(holeIndex > topIndex && comp(*(first+parent),value))//*(first+parent) < value)//maxheap
        {
            *(first + holeIndex) = *(first+parent);
            holeIndex = parent;
            parent = (holeIndex-1)/2;
        }
        *(first + holeIndex) = value;
    }

    template<class RandomAccessIterator, class Distance, class T, class Compare>
    inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare comp)
    {
        __push_heap(first, Distance((last - first)-1), Distance(0), T(*(last-1)), comp);
    }

    template<class RandomAccessIterator, class Compare>
    inline void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
    {
        __push_heap_aux(first,last,distance_type(first),value_type(first) ,comp);
    }

    //pop_heap:头节点放到最后一个节点，从根节点开始下溯,注意：pop出的根节点值并未删除，而是放在尾端
    template<class RandomAccessIterator, class Compare>
    inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
    {
        __pop_heap_aux(first, last, value_type(first), comp);
    }

    template<class RandomAccessIterator, class T, class Compare>
    inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*, Compare comp)
    {
        __pop_heap(first, last-1, last-1, T(*(last-1)), distance_type(first), comp);
    }

    template<class RandomAccessIterator, class T, class Distance, class Compare>
    inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*, Compare comp)
    {
        *result = *first;

        __adjust_heap(first, Distance(0), Distance(last-first), value, comp);//易知，这里输入的len不包括放在尾端的根节点
    }

    template <class RandomAccessIterator, class Distance, class T, class Compare>
    void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value, Compare comp)//参数：首，第一个index，长度，需调整位置的值
    {
        Distance topIndex = holeIndex;
        Distance secondChild = 2*holeIndex+2;//右子节点
        while(secondChild < len)
        {
            if(comp(*(first+secondChild),*(first+secondChild -1 ))){//(*(first+secondChild) < *(first+secondChild -1 )){
                --secondChild;
            }
            *(first+holeIndex) = *(first+secondChild);
            holeIndex = secondChild;
            secondChild = 2*holeIndex+2;

        }
        if(secondChild == len){//无右子节点，只有左子节点
            *(first + holeIndex) = *(first + secondChild - 1);
            holeIndex = secondChild-1;
        }
        __push_heap(first, holeIndex, topIndex, value, comp);

    }

    //sort_heap:对heap区域进行排序，不再按完全二叉树的形式存在
    template <class RandomAccessIterator, class Compare>
    void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
    {
        while (last - first >1)
            pop_heap(first, last--,comp);//每次pop都把根节点放在了最后，所以是从小到大排序的
    }

    //make_heap
    template <class RandomAccessIterator, class Compare>
    inline void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
    {
        __make_heap(first, last, value_type(first), distance_type(first),comp);
    }

    template <class RandomAccessIterator, class T, class Distance, class Compare>
    void __make_heap(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*, Compare comp)
    {
        if(last - first < 2)
            return;

        Distance len = last - first;

        Distance parent = (len -2)/2;
        while(true){
            __adjust_heap(first, parent, len, T(*(first + parent)),comp);
            if(parent == 0)
                return;
            --parent;
        }

    }





}

#endif // ALGORITHM_H_INCLUDED
