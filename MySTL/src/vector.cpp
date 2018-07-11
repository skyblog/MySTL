#include "vector.h"

namespace MySTL{
    //position之前插入x
    template <class T, class Alloc >
    void vector<T, Alloc>::insert_aux(iterator position, const T& x){
        if(finish != end_of_storage){//还有备用空间
            //备用空间起始处构造一个元素，vector最后值为其初始值
            construct(finish, *(finish-1));
            ++finish;
            //向后复制
            T x_copy = x;
            copy_backward(position, finish-2, finish-1);
            //position处填入x
            *position = x_copy;

        }
        else{//无备用空间
            const size_type old_size = size();
            const size_type len = old_size==0? 1:old_size*2;//两倍原则

            //申请空间
            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;
            //复制
            try
            {
                new_finish = uninitialized_copy(start, position, new_start);
                construct(new_finish, x);
                ++new_finish;
                new_finish = uninitialized_copy(position, finish, new_finish);

            }
            catch(...)
            {
                destroy(new_start, new_finish);
                data_allocator::deallocate(new_start,len);
                throw;
            }

            destroy(start,finish);
            deallocate();

            start = new_start;
            finish = new_finish;
            end_of_storage = start+len;
        }
    }


    template <class T, class Alloc >
    void vector<T, Alloc>::reserve(size_type new_capacity){
        if(new_capacity <= capacity()){
            return;
        }
        T* new_start = data_allocator::allocate(new_capacity);
        T* new_finish = uninitialized_copy(begin(),end(),new_start);
        destroy(start,finish);
        deallocate();

        start = new_start;
        finish = new_finish;
        end_of_storage = start+new_capacity;
    }

    template <class T, class Alloc >
    void vector<T, Alloc>::insert(iterator position, size_type n, const T& x){
        if(n!= 0){
            if((end_of_storage - finish) >= n){//备用空间大于n
                T copy_x = x;

                const size_type elems_after = finish - position;
                iterator old_finish = finish;

                if(elems_after > n){
                    //先把n个后移到未分配空间
                    //再把剩余的后移，
                    //再填入n个x
                    uninitialized_copy(finish-n, finish, finish);
                    finish+=n;
                    copy_backward(position, old_finish-n, old_finish);
                    fill(position, position+n, copy_x);

                }
                else{//已分配空间不够n，则n的一部分也要复制到备用空间中
                    //把需复制到备用空间的k个x复制过去
                    //把已分配空间元素后移
                    //填入n-k个x
                    uninitialized_copy_n(finish, n-elems_after, copy_x);
                    finish+=(n-elems_after);
                    uninitialized_copy(position, old_finish, finish);
                    finish += elems_after;
                    fill(position, old_finish, copy_x);
                }
            }
            else{//备用空间不够

                //计算新空间长度
                const size_type old_size = size();
                const size_type new_size = old_size + std::max(old_size, n);
                //配置新空间
                iterator new_start = data_allocator::allocate(new_size);
                iterator new_finish = new_start;
                //复制start-position
                //复制n个x
                //复制position-finish

                new_finish = uninitialized_copy(start,position,new_finish);
                new_finish = uninitialized_fill_n(new_finish, n, x);
                new_finish = uninitialized_copy(position, finish, new_finish);

                destroy(start, finish);
                deallocate();
                start = new_start;
                finish = new_finish;
                end_of_storage = new_start + new_size;
            }
        }
    }


}



