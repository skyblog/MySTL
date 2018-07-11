#ifndef VECTOR_H
#define VECTOR_H

#include "Allocator.h"
#include "Iterator.h"
#include "Construct.h"
#include "UninitializedFunction.h"
#include "TypeTraits.h"

namespace MySTL{

    template <class T, class Alloc>
    class vector;

    template <class T, class Alloc>
    bool operator==(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs);
    template <class T, class Alloc>
    bool operator!=(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs);


    template <class T, class Alloc = Allocator<T> >
    class vector
    {
        friend bool operator== <>(const vector& ,const vector& );
		friend bool operator!= <>(const vector& ,const vector& );

    public:
        typedef T                       value_type;
        typedef value_type*             pointer;
        typedef value_type*             iterator;
        typedef value_type&             reference;
        typedef size_t                  size_type;
        typedef ptrdiff_t               difference_type;
        typedef const T*                const_iterator;
        typedef const T&                const_reference;
        //typedef reverse_iterator_t<T*>  reverse_iterator;
        //typedef reverse_iterator_t<const T*> const_reverse_iterator;

    protected:
        typedef Alloc data_allocator;
        iterator start; //��ʾĿǰʹ�ÿռ��ͷ
        iterator finish; //Ŀǰʹ�ÿռ��β
        iterator end_of_storage;//Ŀǰ���ÿռ��β


        //void insert_aux(iterator position, const T& x);
        //���ÿռ䲢������n��x
        iterator allocate_and_fill(size_type n, const T& x){
            iterator result = data_allocator::allocate(n);
            MySTL::uninitialized_fill_n(result, n, x);
            return result;
        }
        //��ʼ��n��value
        void fill_initialize(size_type n, const T& value){
            start = allocate_and_fill(n,value);
            finish = start + n;
            end_of_storage  = finish;
        }
        //deallocate
        void deallocate(){
            if(start){
                data_allocator::deallocate(start, end_of_storage - start);
            }
        }
        //insert_aux,���ÿռ䣬��vector�ռ䲻����ʱ
        void insert_aux(iterator position, const T& x);

    public:
        //���캯��
        vector():start(0),finish(0),end_of_storage(0){}
        vector(size_type n, const T& value){fill_initialize(n,value);}
        vector(int n, const T& value){fill_initialize(n,value);}
        vector(long n, const T& value){fill_initialize(n,value);}
        explicit vector(size_type n){fill_initialize(n,T());}
        template<class InputIterator>
        vector(InputIterator first, InputIterator last)
        {
            start = data_allocator::allocate(last - first);
            finish = MySTL::uninitialized_copy(first, last, start);
            end_of_storage = finish;
        }
        /**
        vector(vector &&v){
            start = v.start;
            finish = v.finish;
            end_of_storage = v.end_of_storage;
            v.start = v.finish = v.end_of_storage = 0;
        }*/
        vector(const vector& v){
            start = data_allocator::allocate(v.end_of_storage - v.start);
            finish = MySTL::uninitialized_copy(v.start, v.finish, start);
            end_of_storage = start + (v.end_of_storage - v.start);
        }

        //=
        vector& operator = (const vector& v){
            if (this != &v){
                start = data_allocator::allocate(v.end_of_storage - v.start);
                finish = MySTL::uninitialized_copy(v.start, v.finish, start);
                end_of_storage = start + (v.end_of_storage - v.start);

            }
            return *this;
        }

		vector& operator = (vector&& v){
		    if(this != &v){
                destroy(start,finish);
                deallocate();
                start = v.start;
                finish = v.finish;
                end_of_storage = v.end_of_storage;
                v.start = v.finish = v.end_of_storage = 0;
		    }
		    return *this;
		}


        //��������
        ~vector(){
            destroy(start, finish);//Construct��ȫ�ֺ���
            deallocate();
        }

        //�ж�
        bool operator == (const vector& v)const{
            if(size() != v.size()){
                return false;
            }
            else{
                auto ptr1 = start;
                auto ptr2 = v.start;
                for(;ptr1 != finish && ptr2!=v.finish; ++ptr1,++ptr2){
                    if(*ptr1 != *ptr2)
                        return false;
                }
                return true;
            }
        }
		bool operator != (const vector& v)const{return !(*this == v);}



    public:
        //���������
        iterator begin(){return start;}
        iterator end(){return finish;}
        iterator begin() const {return start;}
        iterator end() const {return finish;}


        //��ʹ�ÿռ����
        //�жϺ������µĿռ䶼����insert��
        size_type size() const {return size_type(finish - start);}
        bool empty() const { return start == finish;}
        void resize(size_type new_size, const T& x){
            if(new_size < size()){
                erase(begin()+new_size, end());
            }else{
                insert(end(),new_size-size(),x);
            }
        }
        void resize(size_type new_size){return resize(new_size, T());}


        //�������
        size_type capacity() const {return size_type(end_of_storage-start);}
        void reserve(size_type new_capacity);

        //����Ԫ�����
        reference operator[](size_type i){return *(begin()+i);}
        const_reference operator[](size_type i ) const {return *(begin()+i);}
        reference front() {return *(begin());}
        reference back() {return *(end()-1);}
        const_reference front() const {return *(begin());}
        const_reference back() const {return *(end()-1);}


        //��ɾ�޸�����
        void push_back(const T& x){
            if(finish != end_of_storage){
                MySTL::construct(finish, x);
                ++finish;
            }
            else{
                insert_aux(end(),x);
            }
        }

        void pop_back(){
            --finish;
            destroy(finish);
        }

        void pop_front(){
            erase(begin());
        }
        //���ر�ɾԪ�غ�һ��������
        iterator erase(iterator position){
            if(position+1 != end()){
                std::copy(position+1, end(), position);
            }
            --finish;
            destroy(finish);
            return position;

        }
        iterator erase(iterator first, iterator last){
            if(last != end()){
                std::copy(last, end(), first);
            }
            iterator num = finish -(last - first);
            destroy(num,finish);
            finish = num;

            return first;
        }
        void clear(){
            destroy(begin(),end());
            finish = start;
        }

        void swap(vector& v){
            if(this != &v){
                iterator tmp;
                tmp = start;
                start = v.start;
                v.start = tmp;
                tmp = finish;
                finish = v.finish;
                v.finish = tmp;
                tmp = end_of_storage;
                end_of_storage = v.end_of_storage;
                v.end_of_storage = tmp;
                //std::swap(start, v.start);
                //std::swap(finish, v.finish);
                //std::swap(end_of_storage, v.end_of_storage);
            }
        }

        void insert(iterator position, size_type n, const T& x);
        iterator insert(iterator position, const T& x){
            const size_type index = position - begin();
            insert_aux(position, x);
            return begin()+index;
        }

    };


    template <class T, class Alloc>
    bool operator==(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
    {
        if(lhs.size() != rhs.size()){
                return false;
        }
        else{
            auto ptr1 = lhs.begin();
            auto ptr2 = rhs.begin();
            for(;ptr1 != lhs.end() && ptr2!=rhs.end(); ++ptr1,++ptr2){
                if(*ptr1 != *ptr2)
                    return false;
            }
            return true;
        }
    }
    template <class T, class Alloc>
    bool operator!=(const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs){return !(lhs == rhs);}

    //position֮ǰ����x
    template <class T, class Alloc >
    void vector<T, Alloc>::insert_aux(iterator position, const T& x){
        if(finish != end_of_storage){//���б��ÿռ�
            //���ÿռ���ʼ������һ��Ԫ�أ�vector���ֵΪ���ʼֵ
            construct(finish, *(finish-1));
            ++finish;
            //�����
            T x_copy = x;
            std::copy_backward(position, finish-2, finish-1);
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
                new_finish =  MySTL::uninitialized_copy(start, position, new_start);
                construct(new_finish, x);
                ++new_finish;
                new_finish =  MySTL::uninitialized_copy(position, finish, new_finish);

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
        T* new_finish = MySTL::uninitialized_copy(begin(),end(),new_start);
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
                    MySTL::uninitialized_copy(finish-n, finish, finish);
                    finish+=n;
                    std::copy_backward(position, old_finish-n, old_finish);
                    std::fill(position, position+n, copy_x);

                }
                else{//�ѷ���ռ䲻��n����n��һ����ҲҪ���Ƶ����ÿռ���
                    //���踴�Ƶ����ÿռ��k��x���ƹ�ȥ
                    //���ѷ���ռ�Ԫ�غ���
                    //����n-k��x
                    MySTL::uninitialized_fill_n(finish, n-elems_after, copy_x);
                    finish+=(n-elems_after);
                    MySTL::uninitialized_copy(position, old_finish, finish);
                    finish += elems_after;
                    std::fill(position, old_finish, copy_x);
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

                new_finish = MySTL::uninitialized_copy(start,position,new_finish);
                new_finish = MySTL::uninitialized_fill_n(new_finish, n, x);
                new_finish = MySTL::uninitialized_copy(position, finish, new_finish);

                destroy(start, finish);
                deallocate();
                start = new_start;
                finish = new_finish;
                end_of_storage = new_start + new_size;
            }
        }
    }

}


#endif // VECTOR_H
