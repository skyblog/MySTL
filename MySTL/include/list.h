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

        //���캯��
        __list_iterator(){}
        __list_iterator(link_type x):node(x){}
        __list_iterator(const iterator& x):node(x.node){}


        //==,!=
        bool operator==(const self& x) const {return node == x.node;}
        bool operator!=(const self& x) const {return node != x.node;}

        //������
        reference operator*() const {return (*node).data;}
        //->
        pointer operator->() const {return &(operator*());}//�ȵ�����ȡ��ַ����Ϊ����operator*�����Ƚϸ��ӵ�����µļ򵥻�

        //++,--,
        self& operator++(){//++p�����ؼ�֮������ã�������ֵ
            node = (link_type)((*node).next);
            return *this;
        }

        self operator++(int){//p++�����ؼ�֮ǰ��ֵ��ֻ������ֵ
            self tmp = *this;
            ++*this;//��ǰ׺��������ʵ��
            return tmp;
        }
        self& operator--(){
            node = (link_type)((*node).prev);
            return *this;
        }
        self operator--(int){//int,�������ṩ0��Ϊ����βε�ʵ��
            self tmp = *this;
            --*this;
            return tmp;
        }

        self& operator+(size_type n){//++p�����ؼ�֮������ã�������ֵ
            for(size_type i=0; i<n; ++i)
                node = (link_type)((*node).next);
            return *this;
        }

        self& operator-(size_type n){//++p�����ؼ�֮������ã�������ֵ
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
        link_type node; //���ڱ�ʾ������״�б��ָ��

        //���á��ͷš����졢����һ���ڵ�
        //����һ���ڵ㲢����
        link_type get_node(){return list_node_allocator::allocate();}
        //�ͷ�һ���ڵ�
        void put_node(link_type p){list_node_allocator::deallocate(p);}

        //���ò�����һ���ڵ㣬����
        link_type create_node(const T& x){
            link_type p = get_node();
            MySTL::construct(&p->data, x);
            return p;
        }

        //���ٲ��ͷ�һ���ڵ�
        void destroy_node(link_type p){
            MySTL::destroy(&p->data);
            put_node(p);
        }

        //����һ��������
        void empty_initialize(){
            node = get_node();
            node->next = node;
            node->prev = node;
        }
        //[first,last)�Ƶ�position֮ǰ
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
        //�ɹ����õĺ�����

        //���캯��
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

        //erase,������һ�ڵ�
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
        //x�Ӻ���position֮ǰ
        void splice(iterator position, list& x){
            if(!x.empty())
                transfer(position,x.begin(),x.end());
        }
        //i��ָԪ�ؽӺ���position֮ǰ
        void splice(iterator position, list&, iterator i)
        {
            iterator j=i;
            ++j;
            if(position == i || position == j) //����ͬһlist���
                return;
            transfer(position,i,j);
        }
        //[first,last)֮��
        void splice(iterator position, iterator first, iterator last)
        {
            if(first != last)
                transfer(position, first, last);
        }

        //merge,reverse,sort
        void merge(list& x);//x�ϲ���this������lists���ݱ����Ⱦ�����������
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
    //��ֵΪvalue�����нڵ�ɾ��
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
    //�Ƴ���������ͬ��Ԫ��
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
        //����/����һ��Ԫ��
        if(node->next == node)
            return;
        if(link_type(node->next)->next == node)
            return;

        //ÿ��old�ڵ��Ƶ�begin����֮ǰ��ʵ�ַ���
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
        list<T,Alloc> counter[64];//counter[i]���2^(i+1)��Ԫ�أ���counter[i]=2^(i+1)ʱ���Ͱ���ת�Ƹ�counter[i+1]
        int fill = 0;//��ʾ��һ����������counter[fill]


        while(!empty()){
            carry.splice(carry.begin(), *this, begin());//ÿ�δӶ�����ȡ��һ���ڵ�-->carry
            int i=0;
            while(i<fill && !counter[i].empty()){//���counter[i]��Ϊ�գ���һֱ��ǰ�ݽ�
                counter[i].merge(carry);
                carry.swap(counter[i++]);

            }
            carry.swap(counter[i]);
            if(i == fill)//counter[fill]������Ԫ�غ�fill++
                ++fill;
        }


        for(int i=1;i<fill;++i){
            counter[i].merge(counter[i-1]);
        }


        swap(counter[fill-1]);
    }



}


#endif // LIST_H_INCLUDED
