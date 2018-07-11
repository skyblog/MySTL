#ifndef DEQUE_H_INCLUDED
#define DEQUE_H_INCLUDED


#include "Allocator.h"
#include "Construct.h"
#include "UninitializedFunction.h"
#include "TypeTraits.h"


namespace MySTL{

    //���ػ�������С������n��ʾ��������С����������sz��ʾsizeof(value_type)
    //n!=0ʱ���û��Զ��壬ֱ�ӷ���
    //n==0ʱ��Ĭ�ϴ�С512���򴫻�512/sz,sz>512ʱ������1
    inline size_t __deque_buf_size(size_t n, size_t sz)
    {
        return n!=0 ? n:(sz<512? size_t(512/sz):size_t(1));
    }

    //deque������Ӧ�߱��Ľṹ��������������Ƿ������Ե�����ǣ���ǰ��/����ʱҪ����һ�����������Ա�����ʱ����map
    template <class T, class Ref, class Ptr, size_t BufSiz>
    struct __deque_iterator{
        typedef __deque_iterator<T,T&,T*,BufSiz>                    iterator;
        typedef __deque_iterator<T, const T&, const T*, BufSiz>     const_iterator;

        typedef random_access_iterator_tag                          iterator_category;
        typedef T                                                   value_type;
        typedef Ptr                         pointer;
        typedef Ref                         reference;
        typedef size_t                      size_type;
        typedef ptrdiff_t                   difference_type;
        typedef T**                         map_pointer;

        typedef __deque_iterator            self;

        //������ָ���Ԫ�����ڵĻ�����
        T*  cur;     //�˵�������ָ�Ļ�����������Ԫ��
        T*  first;   //�˵�������ָ�Ļ�������ͷ
        T*  last;    //�˵�������ָ�Ļ�������β�������ÿռ䣩
        map_pointer node;//ָ��map��ָ��˻������Ľڵ�

        static size_t buffer_size(){return __deque_buf_size(BufSiz, sizeof(T));}

        //ָ���»�����
        void set_node(map_pointer new_node)
        {
            node = new_node;
            first = *new_node;
            last = first + difference_type(buffer_size());
        }

        //����������
        //*��->
        reference operator*(){return *cur;}
        pointer operator->(){return &(operator*());}
        //distance = iterator - iterator
        difference_type operator-(const self& x) const
        {
            return difference_type(buffer_size())*(node-x.node-1) + (cur - first) + (x.last - x.cur);
        }

        //++,--
        self& operator++()//++a
        {
            ++cur;
            if(cur==last){
                set_node(node+1);
                cur = first;
            }
            return *this;
        }
        self& operator++(int)//a++
        {
            self tmp = *this;
            ++*this;
            return tmp;
        }
        self& operator--()//--a
        {
            if(cur == first){
                set_node(node-1);
                cur = last;
            }
            --cur;
            return *this;
        }
        self& operator--(int)//a--
        {
            self tmp = *this;
            --*this;
            return tmp;
        }

        //+=,+
        self& operator+=(difference_type n)
        {
            difference_type offset = cur + n - first;

            if(offset >= 0 && offset <difference_type(buffer_size())){//ͬһ������
                cur +=n;
            }
            else{//��ͬ������
                difference_type node_offset = offset>0 ? offset/difference_type(buffer_size()):-difference_type((-offset-1)/buffer_size())-1;
                set_node(node+node_offset);
                cur = first + (offset - node_offset*difference_type(buffer_size()));
            }
            return *this;
        }
        self operator+(difference_type n) const
        {
            self tmp = *this;
            return tmp+=n;
        }

        //-=,-
        self& operator-=(difference_type n){return *this+= -n;}

        self operator-(difference_type n) const
        {
            self tmp = *this;
            return tmp-=n;
        }

        //[]�����ȡ,this:ָ����󣨵���������ָ�룬*this�����󣬴˵��������ٵ���operator+
        reference operator[](difference_type n) const {return *((*this)+n);}

        //==,!=,<
        bool operator==(const self& x) const {return cur==x.cur;}
        bool operator!=(const self& x) const {return !(*this == x);}
        bool operator<(const self& x) const {return (node == x.node)? (cur<x.cur) : (node<x.node);}
    };


    template <class T, class Alloc, class Alloc_map , size_t BufSize>
    class deque;

    template <class T, class Alloc, class Alloc_map , size_t BufSize>
    bool operator==(const deque<T,Alloc,Alloc_map,BufSize>& lhs, const deque<T,Alloc,Alloc_map,BufSize>& rhs);
    template <class T, class Alloc, class Alloc_map , size_t BufSize>
    bool operator!=(const deque<T,Alloc,Alloc_map,BufSize>& lhs, const deque<T,Alloc,Alloc_map,BufSize>& rhs);


    template <class T, class Alloc = Allocator<T>, class Alloc_map = Allocator<T*>, size_t BufSize = 0>
    class deque{

        friend bool operator== <>(const deque& ,const deque& );
		friend bool operator!= <>(const deque& ,const deque& );

    public:
        typedef T                       value_type;
        typedef value_type*             pointer;
        typedef const value_type*       const_pointer;
        typedef value_type&             reference;
        typedef const value_type&       const_reference;
        typedef size_t                  size_type;
        typedef ptrdiff_t               difference_type;
        typedef __deque_iterator<T,T&,T*,BufSize>                iterator;
        typedef const __deque_iterator<T, T&, T*,BufSize>        const_iterator;
    protected:
        typedef pointer*            map_pointer;
        typedef Alloc               data_allocator;
        typedef Alloc_map           map_allocator;

    protected:
        iterator start;   //��һ���ڵ�
        iterator finish;  //���һ���ڵ�
        map_pointer map;  //ָ��map��map�ǿ������ڴ�ռ䣬��ÿ��Ԫ�ض���ָ�룬pointer����
        size_type map_size;//map���ж���ָ��

    public:
        //begin,end
        iterator begin(){return start;}
        iterator end(){return finish;}
        iterator begin() const {return start;}
        iterator end() const {return finish;}
        //front,back
        reference front(){return *start;}
        reference back(){return *(finish-1);}
        const_reference front()const {return *start;}
        const_reference back()const {return *(finish-1);}
        //[]
        reference operator[](size_type n){return start[difference_type(n)];}


        //size,max_size(),empty
        size_type size() const {return finish - start;}
        size_type max_size() const {return size_type(-1);}
        bool empty() const {return start == finish;}
    protected:
        static size_t buffer_size(){return __deque_buf_size(BufSize, sizeof(T));}
        static size_type initial_map_size() {return 8;}//map���ٹ���Ľڵ�����8��

        pointer allocate_node(){return data_allocator::allocate(buffer_size());}
        void deallocate_node(pointer node){data_allocator::deallocate(node,buffer_size());}

        //allocate&construct n��value��deque
        void fill_initialize(size_type n, const value_type& value);
        //���룺Ԫ�ظ��������������ź�deque�ṹ:map,����ָ�򻺴�����allocate��start��finish��ʼ��
        void create_map_and_nodes(size_type num_elements);
        //deallcate map������ָ����
        void destroy_map_and_nodes(){
            for(map_pointer tmp = start.node; tmp <= finish.node; ++tmp){
                deallocate_node(*tmp);
            }
            map_allocator::deallocate(map,map_size);
        }
        void reserve_map_at_back(size_type nodes_to_add = 1)//�жϲ�����map
        {
            if(finish.node-map +1 + nodes_to_add > map_size ){
                reallocate_map(nodes_to_add, false);
            }
        }
        void reallocate_map(size_type nodes_to_add, bool add_at_front);//����map

        void reserve_map_at_front(size_type nodes_to_add = 1)
        {
            if(nodes_to_add > start.node - map){
                reallocate_map(nodes_to_add, true);
            }
        }


    public:
        //���캯��
        deque():start(),finish(),map(0),map_size(0){
            create_map_and_nodes(0);
        }

        deque(size_type n, const value_type& value):start(),finish(),map(0),map_size(0){
            fill_initialize(n,value);
        }

        deque(int n, const value_type& value):start(),finish(),map(0),map_size(0){
            fill_initialize(n,value);
        }
        deque(long n, const value_type& value):start(),finish(),map(0),map_size(0){
            fill_initialize(n,value);
        }
        deque(const deque& x);

        explicit deque(size_type n):start(),finish(),map(0),map_size(0)
        {
            fill_initialize(n,value_type());
        }

        //��������
        ~deque()
        {
            MySTL::destroy(start,finish);//destroy
            destroy_map_and_nodes();//deallocate
        }

    public:
        void push_back(const value_type& x);
        void push_front(const value_type& x);
        void pop_back();
        void pop_front();

        void clear();
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);
        iterator insert(iterator position, const value_type& x);





    };

    template <class T, class Alloc, class Alloc_map , size_t BufSize>
    bool operator==(const deque<T,Alloc,Alloc_map,BufSize>& lhs, const deque<T,Alloc,Alloc_map,BufSize>& rhs)
    {
        if(lhs.size() != rhs.size()){
            return false;
        }else{
            auto ptr1 = lhs.begin();
            auto ptr2 = rhs.begin();
            for(;ptr1!=lhs.end() && ptr2!=rhs.end();++ptr1,++ptr2){
                if(*ptr1 != *ptr2)
                    return false;
            }
            return true;
        }

    }
    template <class T, class Alloc, class Alloc_map , size_t BufSize>
    bool operator!=(const deque<T,Alloc,Alloc_map,BufSize>& lhs, const deque<T,Alloc,Alloc_map,BufSize>& rhs){return !(lhs == rhs);}


    template <class T, class Alloc, class Alloc_map , size_t BufSize>
    void deque<T,Alloc,Alloc_map,BufSize>::fill_initialize(size_type n, const value_type& value)
    {
        create_map_and_nodes(n);

        map_pointer cur;
        try{
            //���ֵ
            for(cur = start.node; cur < finish.node; ++cur){
                MySTL::uninitialized_fill(*cur, *cur + buffer_size(), value);
            }
            //���һ��
            MySTL::uninitialized_fill(finish.first, finish.cur, value);

        }
        catch(...){
            for(map_pointer tmp = start.node; tmp<cur; ++tmp){
                MySTL::destroy(*tmp, *tmp+buffer_size());
            }
            destroy_map_and_nodes();
            throw;
        }
    }

    template <class T, class Alloc, class Alloc_map , size_t BufSize>
    void deque<T,Alloc,Alloc_map,BufSize>::create_map_and_nodes(size_type num_elements)
    {
        size_type num_nodes = num_elements/buffer_size() + 1;

        map_size = std::max(initial_map_size(), num_nodes+2);//���ٸ�����or num_nodes+2��ǰ�����һ��������ʱ����
        map = map_allocator::allocate(map_size);//allocate,���ó���map_size���ڵ��map

        //nstart:��ʼλ�ã�nfinish������λ��
        //����һ��������map���м�λ�ã�num_node+2ʱǰ�����һ����8ʱ���м�
        //Ϊ��ͷβ����������һ����
        map_pointer nstart = map + (map_size - num_nodes)/2;
        map_pointer nfinish = nstart + num_nodes -1;

        map_pointer cur;
        //Ϊÿ��ָ��ָ������allocateĬ�ϴ�С�Ļ�����
        try{
            for(cur = nstart; cur <= nfinish; ++cur){
                *cur = allocate_node();
            }
        }catch(...){
            for(map_pointer tmp = nstart; tmp<cur; ++tmp)
                deallocate_node(*tmp);
            map_allocator::deallocate(map, map_size);
            throw;
        }

        start.set_node(nstart);
        finish.set_node(nfinish);
        start.cur = start.first;
        finish.cur = finish.first + num_elements % buffer_size();
    }
    template <class T, class Alloc, class Alloc_map , size_t BufSize>
    deque<T,Alloc,Alloc_map,BufSize>::deque(const deque& x):start(),finish(),map(0),map_size(0)
    {
        create_map_and_nodes(x.size());

        map_pointer cur;
        map_pointer xcur;

        try{
            //���ֵ
            for(cur = start.node, xcur = x.start.node; cur < finish.node && xcur < x.finish.node; ++cur, ++xcur){
                MySTL::uninitialized_copy(*xcur, *xcur + buffer_size(), *cur);
            }
            //���һ��
            MySTL::uninitialized_copy(x.finish.first, x.finish.cur, *cur);

        }
        catch(...){
            for(map_pointer tmp = start.node; tmp<cur; ++tmp){
                MySTL::destroy(*tmp, *tmp+buffer_size());
            }
            destroy_map_and_nodes();
            throw;
        }
    }
    template <class T, class Alloc, class Alloc_map , size_t BufSize>
    void deque<T,Alloc,Alloc_map,BufSize>::reallocate_map(size_type nodes_to_add, bool add_at_front)
    {
        size_type old_num_nodes = finish.node - start.node + 1;
        size_type new_num_nodes = old_num_nodes + nodes_to_add;

        map_pointer new_nstart;

        if(map_size > 2*new_num_nodes){//���������¿ռ�,�����λ�ã�����Ҫ��ǰ�����˺���û����Ҫ����������ǰ��û��
            new_nstart = map + (map_size - new_num_nodes)/2 + (add_at_front ? nodes_to_add:0);

            if( new_nstart < start.node)//Ϊ�˸��Ƶ�ʱ���û�δ�����Ƶ����򱻸��ǣ�����Ҫ���ж�
                std::copy(start.node, finish.node+1, new_nstart);
            else
                std::copy_backward(start.node, finish.node+1, new_nstart + old_num_nodes);

        }
        else{//��������map

            size_type new_map_size = map_size + std::max(map_size, nodes_to_add) + 2;

            //�����¿ռ�
            map_pointer new_map= map_allocator::allocate(new_map_size);
            new_nstart = new_map + (new_map_size - new_num_nodes)/2 + (add_at_front? nodes_to_add:0);

            //���ݿ���
            std::copy(start.node, finish.node+1, new_nstart);
            //�ͷ�ԭmap
            map_allocator::deallocate(map, map_size);

            map = new_map;
            map_size = new_map_size;
        }

        start.set_node(new_nstart);

        finish.set_node(new_nstart + old_num_nodes -1);//finishָ���map�е�node�����һ�������������һ�飬����ָ��Ļ�������cur��������һ��
    }

    template <class T, class Alloc, class Alloc_map , size_t BufSize>
    void deque<T,Alloc,Alloc_map,BufSize>::push_back(const value_type& x)
    {
        //std::cout << "here-----ccc-------------"<<map_size<<std::endl;
        if(finish.cur != finish.last-1){//���ڻ�������������
            construct(finish.cur,x);//finishָ�����һ������һ��
            ++finish.cur;
        }
        else{
            value_type x_copy = x;
            reserve_map_at_back();//����ʱ��һ��map������
            //����һ���»�������map�����չ
            *(finish.node+1) = allocate_node();

            try{
                construct(finish.cur, x_copy);
                finish.set_node(finish.node+1);
                finish.cur = finish.first;
            }
            catch(...){
                deallocate_node(*(finish.node+1));
            }

        }
    }
    template <class T, class Alloc, class Alloc_map , size_t BufSize>
    void deque<T,Alloc,Alloc_map,BufSize>::push_front(const value_type& x)
    {
        if(start.first != start.cur){//��һ���������пռ�
            construct(start.cur-1, x);
            --start.cur;
        }
        else{//��һ�������޿ռ���
            value_type x_copy = x;
            reserve_map_at_front();
            //�����»�����
            *(start.node-1) = allocate_node();
            try{
                start.set_node(start.node - 1);
                start.cur = start.last -1;
                construct(start.cur, x_copy);
            }
            catch(...){
                start.set_node(start.node -1);
                start.cur = start.first;
                deallocate_node(*(start.node -1));
                throw;
            }

        }
    }
    template <class T, class Alloc, class Alloc_map , size_t BufSize>
    void deque<T,Alloc,Alloc_map,BufSize>::pop_back()
    {
        if(finish.first != finish.cur){//ע�⣺finish.curָ�����һ������һ��
            --finish.cur;
            destroy(finish.cur);
        }
        else{
            deallocate_node(finish.first);
            finish.set_node(finish.node-1);
            finish.cur = finish.last -1;
            destroy(finish.cur);
        }
    }

    template <class T, class Alloc, class Alloc_map , size_t BufSize>
    void deque<T,Alloc,Alloc_map,BufSize>::pop_front()
    {
        if(start.last -1 != start.cur)
        {
            destroy(start.cur);
            ++start.cur;
        }
        else{
            destroy(start.cur);
            deallocate_node(start.first);
            start.set_node(start.node+1);
            start.cur = start.first;
        }
    }

    template <class T, class Alloc, class Alloc_map , size_t BufSize>
    void deque<T,Alloc,Alloc_map,BufSize>::clear()
    {
        //ͷβ���⻺����destroy��deallocate��������
        for(map_pointer tmp = start.node+1; tmp<finish.node; ++tmp)
        {
            destroy(*tmp, *tmp+buffer_size());
            deallocate_node(*tmp);
        }

        //ͷβ�����������ִ��˶���
        if(start.node != finish.node){
            destroy(start.cur, start.last);
            destroy(finish.first, finish.cur);
            deallocate_node(finish.first);//����ͷ������
        }
        else{
            destroy(start.cur, finish.cur);
        }

        finish = start;

    }
    template <class T, class Alloc, class Alloc_map , size_t BufSize>
    typename deque<T,Alloc,Alloc_map,BufSize>::iterator
    deque<T,Alloc,Alloc_map,BufSize>::erase(iterator position)
    {
        iterator next = position;
        ++next;

        difference_type index = position - start;
        if(index < (size() >> 1)){//ǰ���Ԫ���٣�ǰ����ƣ��������ǰ��
            std::copy_backward(start, position, next);
            pop_front();
        }
        else{
            std::copy(next, finish, position);
            pop_back();
        }
        return start+index;
    }
    template <class T, class Alloc, class Alloc_map , size_t BufSize>
    typename deque<T,Alloc,Alloc_map,BufSize>::iterator
    deque<T,Alloc,Alloc_map,BufSize>::erase(iterator first, iterator last)
    {
        if(first == start && last == finish){//һ����deque
            clear();
            return finish;
        }
        else{
            difference_type n = last - first;
            difference_type elems_before = first - start;

            if(elems_before < ((size()-n) >> 1)){//ǰ���
                std::copy_backward(start, first, last);
                iterator new_start = start + n;
                destroy(start, new_start);
                for(map_pointer tmp = start.node; tmp<new_start.node; ++tmp){
                    deallocate_node(*tmp);
                }
                start = new_start;
            }
            else{
                std::copy(last,finish,first);

                iterator new_finish = finish-n;
                destroy(new_finish,finish);
                for(map_pointer tmp = new_finish.node; tmp<finish.node; ++tmp){
                    deallocate_node(*tmp);
                }
                finish = new_finish;

            }
            return start + elems_before;

        }

    }

    template <class T, class Alloc, class Alloc_map , size_t BufSize>
    typename deque<T,Alloc,Alloc_map,BufSize>::iterator
    deque<T,Alloc,Alloc_map,BufSize>::insert(iterator position, const value_type& x)//����positionǰ
    {
        if(position.cur == start.cur){//��������ǰ��
            push_front(x);
            return start;
        }
        else if(position.cur == finish.cur){//�����
            push_back(x);
            return finish-1;
        }
        else{//����
             difference_type index = position - start;

             if(index < (size()>>1)){
                std::cout <<"1."<< index << " " << (size()>>1) << std::endl;
                push_front(front());


                std::copy(start+1,position, start);
                --position;

             }
             else{
                std::cout <<"2."<< index << " " << (size()>>1) << std::endl;
                push_back(back());
                std::copy_backward(position,finish-2,finish-1);
             }

             *position = x;
             return position;
        }
    }


}



#endif // DEQUE_H_INCLUDED
