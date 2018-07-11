#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED


#include "Allocator.h"
#include "Construct.h"
#include "UninitializedFunction.h"
#include "TypeTraits.h"

namespace MySTL{

    template<class T>
    struct __list_node{
        T data;
        __list_node *prev;
        __list_node *next;


    };
    template<class T, class Ref, class Ptr>
    struct __list_iterator{
        typedef __list_iterator<T, T&, T*>      iterator;
        typedef __list_iterator<T, Ref, Ptr>    self;

        typedef bidirectional_iterator_tag      iterator_category;
        typedef T                               value_type;
        typedef Ptr                             pointer;
        typedef Ref                             reference;
        typedef __list_node<T>*                 link_type;
        typedef size_t                          size_type;
        typedef ptrdiff_t                       difference_type;


        link_type node;

        //构造函数
        __list_iterator(){}
        __list_iterator(link_type x):node(x){}
        __list_iterator(const iterator& x):node(x.node){}


        //==,!=
        bool operator==(const self& x) const {return node == x.node;}
        bool operator!=(const self& x) const {return node != x.node;}

        //解引用
        reference operator*() const {return (*node).data;}
        //->
        pointer operator->() const {return &(operator*());}//先调用再取地址，是为了在operator*（）比较复杂的情况下的简单化

        //++,--,
        self& operator++(){//++p，返回加之后的引用，可作左值
            node = (link_type)((*node).next);
            return *this;
        }

        self operator++(int){//p++，返回加之前的值，只可做右值
            self tmp = *this;
            ++*this;//用前缀操作符来实现
            return tmp;
        }
        self& operator--(){
            node = (link_type)((*node).prev);
            return *this;
        }
        self operator--(int){//int,编译器提供0作为这个形参的实参
            self tmp = *this;
            --*this;
            return tmp;
        }

        self& operator+(size_type n){//++p，返回加之后的引用，可作左值
            for(size_type i=0; i<n; ++i)
                node = (link_type)((*node).next);
            return *this;
        }

        self& operator-(size_type n){//++p，返回加之后的引用，可作左值
            for(size_type i=0; i<n; ++i)
                node = (link_type)((*node).prev);
            return *this;
        }

    };



    template <class T, class Alloc>
    class list;

    template <class T, class Alloc>
    bool operator==(const list<T,Alloc>& lhs, const list<T,Alloc>& rhs);
    template <class T, class Alloc>
    bool operator!=(const list<T,Alloc>& lhs, const list<T,Alloc>& rhs);


    template <class T, class Alloc = Allocator<__list_node<T>> >
    class list
    {
        friend bool operator== <>(const list& ,const list& );
		friend bool operator!= <>(const list& ,const list& );


    protected:
        typedef __list_node<T>      list_node;
        typedef Alloc               list_node_allocator;

    public:
        typedef T                       value_type;
        typedef value_type*             pointer;
        typedef value_type&             reference;
        typedef ptrdiff_t               difference_type;
        typedef const T&                const_reference;
        typedef size_t                  size_type;
        typedef list_node*              link_type;

        typedef __list_iterator<T, T&, T*>  iterator;
        typedef const __list_iterator<T, T&, T*>  const_iterator;

    protected:
        link_type node; //用于表示整个环状列表的指针

        //配置、释放、构造、销毁一个节点
        //配置一个节点并传回
        link_type get_node(){return list_node_allocator::allocate();}
        //释放一个节点
        void put_node(link_type p){list_node_allocator::deallocate(p);}

        //配置并构造一个节点，返回
        link_type create_node(const T& x){
            link_type p = get_node();
            MySTL::construct(&p->data, x);
            return p;
        }

        //销毁并释放一个节点
        void destroy_node(link_type p){
            MySTL::destroy(&p->data);
            put_node(p);
        }

        //产生一个空链表
        void empty_initialize(){
            node = get_node();
            node->next = node;
            node->prev = node;
        }
        //[first,last)移到position之前
        void transfer(iterator position, iterator first, iterator last)
        {
            if(position != last){
                last.node->prev->next = position.node;
                first.node->prev->next = last.node;
                position.node->prev->next = first.node;
                link_type tmp = position.node->prev;
                position.node->prev = last.node->prev;
                last.node->prev = first.node->prev;
                first.node->prev = tmp;
            }
        }

        size_type distance (iterator const first, iterator const last) const
        {
            size_type re = 0;
            iterator a=first;

            while(a != last)
            {
                ++a;
                ++re;
            }
            return re;

        }

    public:
        //可供调用的函数们

        //构造函数
        list(){empty_initialize();}
        ~list(){
            link_type tmp=node->next;
            while(tmp!=node){
                link_type delete_node = tmp;
                tmp = tmp->next;
                destroy(delete_node);
            }
            put_node(node);
        }

        //begin,end,
        iterator begin() {return (link_type)((*node).next);}
        iterator end() {return node;}
        iterator begin() const{return (link_type)((*node).next);}
        iterator end()const{return node;}

        //empty, size
        bool empty()const {return node->next == node;}
        size_type size() const{

            return distance(begin(),end());
        }


        //front,back
        reference front(){return *begin();}
        reference back(){return *(--end());}

        const_reference front()const {return *begin();}
        const_reference back()const {return *(--end());}


        //insert
        iterator insert(iterator position, const T& x)
        {
            link_type tmp = create_node(x);
            tmp->next = position.node;
            tmp->prev = position.node->prev;

            (link_type(position.node->prev))->next = tmp;
            position.node->prev = tmp;
            return iterator(tmp);

        }
        void insert(iterator position, size_type n, const T& x){
            for(auto i=0; i!=n; ++i){
                position = insert(position,x);
            }
        }
        void insert(iterator position, int n, const T& x){insert(position,(size_type)n, x);}
        void insert(iterator position, long n, const T& x){insert(position, (size_type) n, x);}

        //push_front,push_back
        void push_back(const T& x){insert(end(),x);}
        void push_front(const T& x){insert(begin(),x);}

        //erase,返回下一节点
        iterator erase(iterator position)
        {
            link_type next_node = link_type(position.node->next);
            link_type prev_node = link_type(position.node->prev);
            prev_node->next = next_node;
            next_node->prev = prev_node;
            destroy_node(position.node);
            return iterator(next_node);
        }
        iterator erase(iterator first, iterator last)
        {

            while(first!=last){
                first = erase(first);

            }
            return first;
        }
        //pop_front,pop_back
        void pop_front(){erase(begin());}
        void pop_back(){erase(--end());}


        //clear
        void clear();
        //resize
        void resize(size_type new_size, const T& x);
        void resize(size_type new_size){resize(new_size,T());}
        //remove
        void remove(const T& value);
        void unique();

        //splice
        //x接合于position之前
        void splice(iterator position, list& x){
            if(!x.empty())
                transfer(position,x.begin(),x.end());
        }
        //i所指元素接合于position之前
        void splice(iterator position, list&, iterator i)
        {
            iterator j=i;
            ++j;
            if(position == i || position == j) //考虑同一list情况
                return;
            transfer(position,i,j);
        }
        //[first,last)之间
        void splice(iterator position, iterator first, iterator last)
        {
            if(first != last)
                transfer(position, first, last);
        }

        //merge,reverse,sort
        void merge(list& x);//x合并到this，两个lists内容必须先经过递增排序
        void reverse();
        void sort();


        //swap
        void swap(list& x)
        {
            link_type tmp = node;
            node = x.node;
            x.node = tmp;
        }

        //operator== , operator!=
        bool operator == ( const list& rhs)
        {
            if(size() != rhs.size())
                return false;

            auto node1 = begin();
            auto node2 = rhs.begin();
            while(node1!=end() && node2!=rhs.end()){
                if(*node1 != *node2)
                    return false;
                ++node1;
                ++node2;
            }
            return true;
        }
		bool operator!= (const list& rhs){return !(*this==rhs);}


    };

    template <class T, class Alloc>
    bool operator==(const list<T,Alloc>& lhs, const list<T,Alloc>& rhs)
    {
        if(lhs.size() != rhs.size())
                return false;

        auto node1 = lhs.begin();
        auto node2 = rhs.begin();
        while(node1!=lhs.end() && node2!=rhs.end()){
            if(*node1 != *node2)
                return false;
            ++node1;
            ++node2;
        }
        return true;
    }
    template <class T, class Alloc>
    bool operator!=(const list<T,Alloc>& lhs, const list<T,Alloc>& rhs){return !(lhs == rhs);}


    template <class T, class Alloc>
    void list<T,Alloc>::clear()
    {
        link_type cur = (link_type)node->next;
        while(cur!=node){
            link_type tmp = cur;
            cur = (link_type)cur->next;
            destroy_node(tmp);
        }
        node->next = node;
        node->prev = node;
    }
    //数值为value的所有节点删除
    template <class T, class Alloc>
    void list<T,Alloc>::remove(const T& value)
    {
        iterator first = begin();
        iterator last = end();

        while(first!=last){
            iterator next = first;
            ++next;
            if(*first == value){
                erase(first);
            }
            first = next;
        }
    }
    //移除连续而相同的元素
    template <class T, class Alloc>
    void list<T,Alloc>::unique()
    {
        iterator first = begin();
        iterator last = end();
        if(first == last) return;

        iterator next = first;
        while(++next != last){
            if(*next == *first)
                erase(next);
            else
                first = next;
            next = first;
        }
    }

    template <class T, class Alloc>
    void list<T,Alloc>::resize(size_type new_size, const T& x)
    {
        if(new_size < size()){
            erase(begin()+new_size,end());
        }
        else{
            insert(end(),new_size-size(),x);
        }
    }

    template <class T, class Alloc>
    void list<T,Alloc>::merge(list<T,Alloc>& x)
    {
        iterator first1 = begin();
        iterator first2 = x.begin();

        while(first1 != end() && first2 != x.end()){
            if(*first1 > *first2){
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;
            }
            else
                ++first1;
        }
        if(first2 != x.end())
            transfer(first1, first2, x.end());
    }


    template <class T, class Alloc>
    void list<T,Alloc>::reverse()
    {
        //空链/仅有一个元素
        if(node->next == node)
            return;
        if(link_type(node->next)->next == node)
            return;

        //每个old节点移到begin（）之前，实现反序
        iterator tmp = begin();
        ++tmp;
        while(tmp!=end()){
            iterator old = tmp;
            ++tmp;
            transfer(begin(),old,tmp);
        }

    }
    template <class T, class Alloc>
    void list<T,Alloc>::sort()//quick sort
    {
        if(node->next == node)
            return;
        if(link_type(node->next)->next == node)
            return;

        list<T,Alloc> carry;
        list<T,Alloc> counter[64];//counter[i]存放2^(i+1)个元素，当counter[i]=2^(i+1)时，就把它转移给counter[i+1]
        int fill = 0;//表示下一个待填满的counter[fill]


        while(!empty()){
            carry.splice(carry.begin(), *this, begin());//每次从对象中取出一个节点-->carry
            int i=0;
            while(i<fill && !counter[i].empty()){//如果counter[i]不为空，则一直向前递进
                counter[i].merge(carry);
                carry.swap(counter[i++]);

            }
            carry.swap(counter[i]);
            if(i == fill)//counter[fill]被填入元素后，fill++
                ++fill;
        }


        for(int i=1;i<fill;++i){
            counter[i].merge(counter[i-1]);
        }


        swap(counter[fill-1]);
    }



}


#endif // LIST_H_INCLUDED
