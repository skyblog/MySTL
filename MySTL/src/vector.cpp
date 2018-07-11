#include "vector.h"

namespace MySTL{
    //position֮ǰ����x
    template <class T, class Alloc >
    void vector<T, Alloc>::insert_aux(iterator position, const T& x){
        if(finish != end_of_storage){//���б��ÿռ�
            //���ÿռ���ʼ������һ��Ԫ�أ�vector���ֵΪ���ʼֵ
            construct(finish, *(finish-1));
            ++finish;
            //�����
            T x_copy = x;
            copy_backward(position, finish-2, finish-1);
            //position������x
            *position = x_copy;

        }
        else{//�ޱ��ÿռ�
            const size_type old_size = size();
            const size_type len = old_size==0? 1:old_size*2;//����ԭ��

            //����ռ�
            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;
            //����
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
            if((end_of_storage - finish) >= n){//���ÿռ����n
                T copy_x = x;

                const size_type elems_after = finish - position;
                iterator old_finish = finish;

                if(elems_after > n){
                    //�Ȱ�n�����Ƶ�δ����ռ�
                    //�ٰ�ʣ��ĺ��ƣ�
                    //������n��x
                    uninitialized_copy(finish-n, finish, finish);
                    finish+=n;
                    copy_backward(position, old_finish-n, old_finish);
                    fill(position, position+n, copy_x);

                }
                else{//�ѷ���ռ䲻��n����n��һ����ҲҪ���Ƶ����ÿռ���
                    //���踴�Ƶ����ÿռ��k��x���ƹ�ȥ
                    //���ѷ���ռ�Ԫ�غ���
                    //����n-k��x
                    uninitialized_copy_n(finish, n-elems_after, copy_x);
                    finish+=(n-elems_after);
                    uninitialized_copy(position, old_finish, finish);
                    finish += elems_after;
                    fill(position, old_finish, copy_x);
                }
            }
            else{//���ÿռ䲻��

                //�����¿ռ䳤��
                const size_type old_size = size();
                const size_type new_size = old_size + std::max(old_size, n);
                //�����¿ռ�
                iterator new_start = data_allocator::allocate(new_size);
                iterator new_finish = new_start;
                //����start-position
                //����n��x
                //����position-finish

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



