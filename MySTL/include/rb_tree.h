#ifndef RB_TREE_H_INCLUDED
#define RB_TREE_H_INCLUDED


#include "Allocator.h"
#include "Iterator.h"
#include "Construct.h"
#include "UninitializedFunction.h"
#include "TypeTraits.h"
#include "pair.h"
#include "Functional.h"

namespace MySTL
{
    typedef bool __rb_tree_color_type;
    const __rb_tree_color_type    __rb_tree_red = false;
    const __rb_tree_color_type    __rb_tree_black = true;

    //�ڵ�
    struct __rb_tree_node_base
    {
        typedef __rb_tree_color_type    color_type;
        typedef __rb_tree_node_base*    base_ptr;

        color_type  color;
        base_ptr    parent;
        base_ptr    left;
        base_ptr    right;

        static base_ptr minimum(base_ptr x)//��Сֵ
        {
            while(x->left!=0)
                x = x->left;
            return x;
        }

        static base_ptr maximum(base_ptr x)//���ֵ
        {
            while(x->right != 0)
                x = x->right;
            return x;
        }

    };

    template <class Value>
    struct __rb_tree_node: public __rb_tree_node_base
    {
        typedef __rb_tree_node<Value>*  link_type;
        Value   value_field;
    };

    //������
    struct __rb_tree_base_iterator
    {
        typedef __rb_tree_node_base::base_ptr   base_ptr;
        typedef bidirectional_iterator_tag  iterator_category;
        typedef ptrdiff_t       difference_type;

        base_ptr    node;

        void increment() //node����һ���������
        {
            if(node->right != 0){ //���ӽڵ㲻Ϊ�գ����ӽڵ�������ӽڵ�
                //std::cout << "XXXXXXXXXXXXXXXXXXXxxx"<<std::endl;
                node = node->right;
                while(node->left!=0)
                {
                    node = node->left;
                    //std::cout <<"aaaaaaaaaaaaaaaaaaaa"<<std::endl;
                }

            }
            else{//���ӽڵ�Ϊ�գ���һ����Ϊ�丸�ڵ��ҽڵ�ĸ��ڵ�
                base_ptr y = node->parent;
                //std::cout << "yyyyyyyyyyyyyyyyyyyyyyyy"<<std::endl;
                while(node == y->right){
                    node = y;
                    y = node->parent;
                    //std::cout << "bbbbbbbbbbbbbbbbbbbbbbbbbbb"<<std::endl;
                }
                if(node->right != y) //ԭ����node�����ֵ��������жϳ�����node= header;�ټ��ϴ�ʱnodeΪ���ڵ�ʱ���жϲ�������node=header
                    node = y;
            }
        }

        void decrement()
        {
            if(node->color == __rb_tree_red && node->parent->parent == node ){//nodeΪheader�����Ϊ��
                node = node->parent;
            }
            else if(node->left!=0){
                base_ptr y = node->left;
                while(y->right != 0){
                    y = y->right;
                }
                node = y;
            }
            else{
                base_ptr y = node->parent;
                while (y->left == node){
                    node = y;
                    y = node->parent;
                }
                node = y;   //ֻ�и�ʱ�����Ϊ��������nodeΪ��Сֵʱ�����Ϊheader
            }
        }
    };

    template <class Value, class Ref, class Ptr>
    struct __rb_tree_iterator : public  __rb_tree_base_iterator
    {
        typedef Value   value_type;
        typedef Ref     reference;
        typedef Ptr     pointer;
        typedef __rb_tree_iterator<Value, Value&, Value*>   iterator;
        typedef __rb_tree_iterator<Value, const Value&, const Value*>   const_iterator;
        typedef __rb_tree_iterator<Value, Ref, Ptr>     self;
        typedef __rb_tree_node<Value>*  link_type;
        typedef size_t      size_type;


        //���캯��
        __rb_tree_iterator(){}
        __rb_tree_iterator(link_type x){node = x;}
        __rb_tree_iterator(const iterator& x){node = x.node;}


        //*,->
        reference operator*() const {return link_type(node)->value_field;}
        pointer operator->() const {return &(operator*());}

        //++,--
        self& operator++()
        {
            increment();
            return *this;
        }
        self operator++(int)
        {
            self tmp = *this;
            increment();
            return tmp;
        }

        self& operator--()
        {
            decrement();
            return *this;
        }
        self operator--(int)
        {
            self tmp = *this;
            decrement();
            return tmp;
        }


    };

    inline bool operator== (const __rb_tree_base_iterator& x, const __rb_tree_base_iterator& y){return x.node==y.node;}
    inline bool operator!= (const __rb_tree_base_iterator& x, const __rb_tree_base_iterator& y){return !(x==y);}



    //������xΪ��ת�㣬rootΪ����xΪ��;ֻ��ת�����ı���ɫ
    inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root)
    {
        __rb_tree_node_base* y = x->right;
        x->right = y->left;
        if(y->left != 0)
            y->left->parent = x;
        y->parent = x->parent;
        if(x == root)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent=y;
    }
    inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root)
    {
        __rb_tree_node_base* y = x->left;
        x->left = y->right;
        if(y->right!=0)
            y->right->parent = x;
        y->parent = x->parent;
        if(x==root)
            root=y;
        else if(x->parent->right == x)
            x->parent->right = y;
        else
            x->parent->left = y;
        y->right=x;
        x->parent = y;

    }

    //ȫ�ֺ���������RB_tree:��ת����ɫ��x������ڵ㣬root:���ڵ�
    inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root)
    {

        x->color = __rb_tree_red;//����ڵ�һ��Ϊ��ɫ
        while(x != root && x->parent->color == __rb_tree_red)
        {
            if(x->parent == x->parent->parent->left)//���ڵ����游�ڵ����ڵ�
            {
                __rb_tree_node_base* y = x->parent->parent->right; //yΪ�����ڵ�
                if( y && y->color == __rb_tree_red)//�����ڵ�Ϊ��,3,4-->�游�ڵ㣺�죻���ڵ㣺��
                {
                    x->parent->color = __rb_tree_black;
                    y->color=__rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    x=x->parent->parent;
                }
                else{//�����ڵ�Ϊ�ڻ��޲����ڵ�

                    if(x == x->parent->right){//�ڲ����,������ת
                        x = x->parent;//std::cout << "XXXXXXXXXXXXXXXXXXXxxx"<<std::endl;
                        __rb_tree_rotate_left(x,root);//std::cout << "XXXXXXXXXXXXXXXXXXXxxx"<<std::endl;
                    }
                    x->parent->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    __rb_tree_rotate_right(x->parent->parent, root);
                }

            }else{//���ڵ�����ڵ���ҽڵ�
                 __rb_tree_node_base* y = x->parent->parent->left;
                 if(y && y->color == __rb_tree_red)
                 {
                     x->parent->color = __rb_tree_black;
                     y->color = __rb_tree_black;
                     x->parent->parent->color = __rb_tree_red;
                     x = x->parent->parent;
                 }
                 else{
                    if(x->parent->left == x)
                    {
                        x = x->parent;
                        __rb_tree_rotate_right(x,root);
                    }
                    x->parent->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    __rb_tree_rotate_left(x->parent->parent, root);
                 }
            }
        }
        root->color = __rb_tree_black;
    }

    inline __rb_tree_node_base* __rb_tree_rebalance_for_erase(__rb_tree_node_base* z, __rb_tree_node_base*& root, __rb_tree_node_base*& leftmost, __rb_tree_node_base*& right_most)
    {
        __rb_tree_node_base* y = z;
        __rb_tree_node_base* x = 0;
        __rb_tree_node_base* x_parent = 0;

        //Ѱ��ʵ�ʵ�ɾ���㣬yΪʵ��ɾ���㣬������ɾ����z�ĵ㣬xΪ���ӽڵ�(��֪y����ֻ��һ���ӽڵ㣩
        if(y->left == 0)
            x = y->right;
        else{
            if(y->right == 0)
                x = y->left;
            else{
                y = y->right;
                while(y->left != 0)
                    y = y->left;
                x = y->right;
            }

        }

        //ɾ���ڵ�
        if(y!=z){//z���Ҿ���
        //����λ��
            //y  &  z->left
            z->left->parent = y;
            y->left = z->left;
            //
            if(y!=z->right){
                //x & y->parent
                x_parent = y->parent;
                if(x!=0)
                    x->parent = y->parent;
                y->parent->left = x;
                //y & z->right
                y->right = z->right;
                z->right->parent = y;
            }
            else{
                x_parent = y;
            }
            //z->parent & y
            if(root == z)
                root = y;
            else if (z->parent->left == z)
                z->parent->left = y;
            else
                z->parent->right = y;
            y->parent = z->parent;
        //������ɫ
            auto tp = y->color;
            y->color = z->color;
            z->color = tp;
        //yָ����ɾ���ڵ㣨����ֵ��
            y = z;
        }
        else{//y=z,z������һ���ӽڵ�Ϊ��,x����z
            //x & y->parent
            x_parent = y->parent;
            if(x!=0)
                x->parent = y->parent;
            if(y == root)
                root = x;
            else if(y->parent->left == y)
                y->parent->left = x;
            else
                y->parent->right = x;

            //leftmost && rightmost
            if(leftmost == y){
                if(z->right == 0)
                    leftmost = z->parent;
                else
                    leftmost = __rb_tree_node_base::minimum(x);
            }
            if(right_most == y){
                if(z->left == 0)
                    right_most = z->parent;
                else
                    right_most = __rb_tree_node_base::maximum(x);
            }
        }

        //����
        if(y->color == __rb_tree_black)//ɾ���ڵ�Ϊ��ɫʱ����Ҫ����
        {
            while(x != root && (x==0 || x->color == __rb_tree_black))//
            {
                if(x == x_parent->left)//xΪ���ӽڵ�
                {
                    __rb_tree_node_base* w = x_parent->right;
                    if(w->color == __rb_tree_red){
                        //����
                        w->color = __rb_tree_black;
                        x_parent->color = __rb_tree_red;
                        __rb_tree_rotate_left(x_parent,root);
                        w = x_parent->right;
                    }

                        //����
                    if((w->left == 0 || w->left->color == __rb_tree_black) && (w->right == 0 || w->right->color == __rb_tree_black)){
                        w->color = __rb_tree_red;
                        x = x_parent;
                        x_parent = x_parent->parent;
                    }
                    else{
                        if(w->right == 0 || w->right->color == __rb_tree_black){
                            if(w->left)
                                w->left->color = __rb_tree_black;
                            w->color = __rb_tree_red;
                            __rb_tree_rotate_right(w,root);
                            w = x_parent->right;
                        }
                        w->color = x_parent->color;
                        x_parent->color = __rb_tree_black;
                        if(w->right)
                            w->right->color = __rb_tree_black;
                        __rb_tree_rotate_left(x_parent,root);
                        break;
                    }
                }
                else
                {
                    __rb_tree_node_base* w = x_parent->right;
                    if(w->color == __rb_tree_red){
                        //����
                        w->color = __rb_tree_black;
                        x_parent->color = __rb_tree_red;
                        __rb_tree_rotate_right(x_parent,root);
                        w = x_parent->left;
                    }
                    if((w->right == 0 || w->right->color == __rb_tree_black) && (w->left == 0 || w->left->color == __rb_tree_black)){
                        w->color = __rb_tree_red;
                        x = x_parent;
                        x_parent = x_parent->parent;
                    }
                    else{
                        if(w->left == 0 || w->left->color == __rb_tree_black){
                            if(w->right)
                                w->right->color = __rb_tree_black;
                            w->color = __rb_tree_red;
                            __rb_tree_rotate_left(w,root);
                            w = x_parent->left;
                        }
                        w->color = x_parent->color;
                        x_parent->color = __rb_tree_black;
                        if(w->left)
                            w->left->color = __rb_tree_black;
                        __rb_tree_rotate_right(x_parent,root);
                        break;
                    }
                }
            }
            if(x)
                x->color = __rb_tree_black;//ΪʹrootΪ��
        }

        return y;
    }


    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc = Allocator<__rb_tree_node<Value> > >
    class rb_tree{
    protected:
        typedef void*                   void_pointer;
        typedef __rb_tree_node_base*    base_ptr;
        typedef __rb_tree_node<Value>   rb_tree_node;
        typedef Alloc                   rb_tree_node_allocator;
        typedef __rb_tree_color_type    color_type;
    public:
        typedef Key                 key_type;
        typedef Value               value_type;
        typedef value_type*         pointer;
        typedef const value_type*   const_pointer;
        typedef value_type&         reference;
        typedef const value_type&   const_reference;
        typedef rb_tree_node*       link_type;
        typedef size_t              size_type;
        typedef ptrdiff_t           difference_type;
        typedef __rb_tree_iterator<value_type, reference, pointer> iterator;
        typedef __rb_tree_iterator<value_type, const_reference, const_pointer>      const_iterator;//������������ﰡ��������__rb_tree_iterator<value_type, const reference,const pointer>
        typedef rb_tree             self;

    protected:
        //allocate & deallocate node
        link_type   get_node(){return rb_tree_node_allocator::allocate();}
        void put_node(link_type p){rb_tree_node_allocator::deallocate(p);}

        link_type create_node(const value_type& x) //allocate��constructһ���ڵ�
        {
            link_type   tmp = get_node();
            try{
                MySTL::construct(&tmp->value_field, x);
            }catch(...){
                put_node(tmp);
            }
            return tmp;
        }

        link_type   clone_node(link_type x){//���ƽڵ�
            link_type   tmp = create_node(x->value_field);
            tmp->color = x->color;
            tmp->left = 0;
            tmp->right = 0;
            return tmp;
        }

        void destroy_node(link_type x)
        {
            MySTL::destroy(&x->value_field);
            put_node(x);
        }

    protected:
        size_type node_count;
        link_type header;
        Compare   key_compare;

        //ȡ��rb header��Ա
        link_type&  root() const {return (link_type&)header->parent;}
        link_type&  left_most() const {return (link_type&)header->left;}
        link_type&  right_most() const {return (link_type&)header->right;}

        //ȡ��x�ڵ�ĳ�Ա(link_type)
        static link_type& left(link_type x)  {return (link_type&)x->left;}
        static link_type& right(link_type x)  {return (link_type&)x->right;}
        static link_type& parent(link_type x)  {return (link_type&)x->parent;}
        static reference value(link_type x)  {return x->value_field;}
        static const key_type& key(link_type x)  {return KeyOfValue()(value(x));}
        static color_type& color(link_type x) {return (color_type&)(x->color);}

        //ȡ��x�ڵ�ĳ�Ա(base_ptr)
        static link_type& left(base_ptr x)  {return (link_type&)x->left;}
        static link_type& right(base_ptr x)  {return (link_type&)x->right;}
        static link_type& parent(base_ptr x)  {return (link_type&)x->parent;}
        static reference value(base_ptr x)  {return ((link_type)x)->value_field;}
        static const key_type& key(base_ptr x)  {return KeyOfValue()(value(link_type(x)));}
        static color_type& color(base_ptr x) {return (color_type&)(link_type(x)->color);}

        //min.max
        static link_type minimum(link_type x) {return (link_type) __rb_tree_node_base::minimum(x);}
        static link_type maxmum(link_type x) {return (link_type) __rb_tree_node_base::maximum(x);}


    private:
        iterator __insert(base_ptr x_, base_ptr y_, const value_type& v);//����ڵ㣬���ڵ㣬����ֵ
        void __erase(link_type x);//����ƽ��ɾ��ȫ��������clear��
        void init()
        {
            //header
            header = get_node();
            color(header) = __rb_tree_red;
            root() = 0;
            left_most() = header;
            right_most() = header;
        }
        link_type __copy(link_type x, link_type p);

    public:
        rb_tree(const Compare& comp = Compare()):node_count(0),key_compare(comp){init();}

        rb_tree(const rb_tree<Key, Value, KeyOfValue,Compare, Alloc>& x):node_count(0),key_compare(x.key_compare)
        {
            if (x.root() == 0)
                init();
            else{
                color(header) = __rb_tree_red;
                root() = __copy(x.root(),header);
                left_most() = minimum(root());
                right_most() = maxmum(root());
            }
            node_count = x.node_count;
        }
        ~rb_tree()
        {
            clear();
            put_node(header);
        }

        self& operator=(const self& x)
        {
            if(this != x)
            {
                clear();
                node_count = 0;
                key_compare = x.key_compare;
                if (x.root() == 0) {
                    root() = 0;
                    left_most() = header;
                    right_most() = header;
                }
                else{
                    root() = __copy(x.root(),header);
                    left_most() = minimum(root());
                    right_most() = maxmum(root());
                    node_count = x.node_count;
                }
            }
            return *this;
        }


    public:
        Compare key_comp() const {return key_compare;}
        iterator begin() {return left_most();}
        const_iterator  begin() const {return left_most();}
        iterator end() {return header;}
        const_iterator  end() const {return header;}
        bool empty() const {return node_count == 0;}
        size_type size() const {return node_count;}
        size_type max_size() const {return size_type(-1);}

        void swap(self& x){
            size_type a;
            a = x.node_count;
            x.node_count = node_count;
            node_count = a;
            link_type b;
            b = x.header;
            x.header = header;
            header = b;
            Compare c;
            c = x.key_compare;
            x.key_compare = key_compare;
            key_compare = c;
        }

    public:
        //����ڵ㣬�ڵ�Ψһ
        pair<iterator,bool> insert_unique(const value_type& x);
        iterator insert_equal(const value_type& v);
        template <class InputIterator>
        void insert_unique(InputIterator first, InputIterator last);
        template <class InputIterator>
        void insert_equal(InputIterator first, InputIterator last);

        //���ҽ��
        iterator find(const key_type& k);
        const_iterator find(const key_type& k) const;
        size_type count(const key_type& x) const
        {
            pair<const_iterator, const_iterator> p = equal_range(x);
            size_type n = 0;
            const_iterator tmp = p.first;
            while(tmp!=p.second){
                ++n;
                ++tmp;
            }
            return n;
        }

        pair<iterator, iterator> equal_range(const key_type& x)
        {
            return pair<iterator, iterator>(lower_bound(x),upper_bound(x));
        }
        pair<const_iterator, const_iterator> equal_range(const key_type& x) const
        {
            return pair<const_iterator, const_iterator>(lower_bound(x),upper_bound(x));
        }

        iterator lower_bound(const key_type& k)
        {
            link_type y = header;
            link_type x = root();
            while(x!=0){
                if(!key_compare(key(x),k))
                    y = x, x =left(x);
                else
                    x = right(x);
            }
            return iterator(y);
        }
        const_iterator lower_bound(const key_type& k) const
        {
            link_type y = header;
            link_type x = root();
            while(x!=0){
                if(!key_compare(key(x),k))
                    y = x, x =left(x);
                else
                    x = right(x);
            }
            return const_iterator(y);
        }

        iterator upper_bound(const key_type& k)
        {
            link_type y = header;
            link_type x = root();
            while(x!=0){
                if(key_compare(k,key(x)))
                    y = x, x =left(x);
                else
                    x = right(x);
            }
            return iterator(y);
        }
        const_iterator upper_bound(const key_type& k) const
        {
            link_type y = header;
            link_type x = root();
            while(x!=0){
                if(key_compare(k,key(x)))
                    y = x, x =left(x);
                else
                    x = right(x);
            }
            return const_iterator(y);
        }




        //ɾ���ڵ�

        void erase(iterator position);
        size_type erase(const key_type& x);
        void erase(iterator first, iterator last);



        void clear()
        {
            if(node_count!=0)
            {
                __erase(root());
                left_most() = header;
                root() = 0;
                right_most() = header;
                node_count=0;
            }
        }

    };


    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc >
    typename rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::link_type
    rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::__copy(link_type x, link_type p)
    {
        link_type top = clone_node(x);
        top->parent = p;

        try{
            if(x->right)
                top->right = __copy(right(x),top);
            p = top;
            x = left(x);

            while(x!=0){
                link_type y = clone_node(x);
                p->left = y;
                y->parent = p;
                if(x->right)
                    y->right = __copy(right(x),y);
                p = y;
                x = left(x);

            }
        }catch(...){
            __erase(top);
        }
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc >
    void rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::__erase(link_type x)
    {
        while(x!=0)
        {
            __erase(right(x));
            link_type y = left(x);
            destroy_node(x);
            x = y;
        }
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc >
    void rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::erase(iterator position)
    {
        if(position == end())
            return;

        link_type y = (link_type) __rb_tree_rebalance_for_erase(position.node, header->parent, header->left, header->right);
        destroy_node(y);
        --node_count;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc >
    void rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::erase(iterator first, iterator last)
    {
        while(first!=last)
        {
            erase(first++);
        }
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc >
    typename rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::size_type rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::erase(const key_type& x)
    {
        pair<iterator,iterator> p = equal_range(x);
        size_type n = 0;
        iterator tmp = p.first;
        while(tmp!=p.second){
            ++n;
           erase(tmp++);
        }
        return n;

    }



    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc >
    typename rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::find(const Key& k)
    {
        link_type y = header;
        link_type x = root();
        while(x!=0)
        {
            if(!key_compare(key(x),k))//k<=x[!(x<k)]
            {
                y = x;
                x = left(x);
            }else{                     //k>x
                x = right(x);
            }
        }

        iterator j = iterator(y);
        return (j==end() || key_compare(k, key(j.node)))? end():j;
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc >
    typename rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::const_iterator
    rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::find(const Key& k) const
    {
        link_type y = header;
        link_type x = root();
        while(x!=0)
        {
            if(!key_compare(key(x),k))//k<=x[!(x<k)]
            {
                y = x;
                x = left(x);
            }else{                     //k>x
                x = right(x);
            }
        }

        const_iterator j = const_iterator(y);
        return (j==end() || key_compare(k, key(j.node)))? end():j;
    }
    //insert_equal�������ظ�������ָ�������ڵ�ĵ�����
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc >
    typename rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::iterator rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::insert_equal(const value_type& v)
    {
        //�ҳ������
        link_type y = header;
        link_type x = root();
        while( x!=0)
        {
            y = x;
            x = key_compare(KeyOfValue()(v), key(x))? left(x):right(x);
        }
        //����
        return __insert(x,y,v);
    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc >
    template <class InputIterator>
    void rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::insert_equal(InputIterator first, InputIterator last)
    {
        for(; first != last; ++ first)
            insert_equal(*first);
    }

    //insert_unique
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc >
    pair<typename rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::iterator,bool>
    rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::insert_unique(const value_type& v)
    {
        link_type y = header;
        link_type x = root();
        bool comp = true;

        //Ѱ�Ҳ����:x����㣬y�����ĸ��ڵ�
        while(x!=0)
        {
            y = x;
            comp = key_compare(KeyOfValue()(v), key(x));
            x = comp? left(x):right(x);
        }

        //���룬Ҫ�����Ƿ��ظ����ж�
        iterator j = iterator(y);
        if(comp){//v<x,�������
            if(j==begin())
                return pair<iterator, bool>(__insert(x,y,v), true);
            else
                --j;
        }
        if(key_compare(key(j.node), KeyOfValue()(v)))
            return pair<iterator, bool>(__insert(x,y,v), true);
        //��ʱ����ȵ�
        return pair<iterator, bool>(j,false);

    }

    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc >
    template <class InputIterator>
    void rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::insert_unique(InputIterator first, InputIterator last)
    {
        for(; first != last; ++ first)
            insert_unique(*first);
    }

    //�����Ĳ���ִ��
    template <class Key, class Value, class KeyOfValue, class Compare, class Alloc >
    typename rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue,Compare, Alloc>::__insert(base_ptr x_, base_ptr y_, const value_type& v)
    {
        link_type x = (link_type) x_;
        link_type y = (link_type) y_;
        link_type z;

        if(y == header || x!=0 || key_compare(KeyOfValue()(v), key(y))){//��߲���
            z = create_node(v);
            left(y) = z;   //yΪheaderʱ����Ϊleft_most = z
            if(y == header){//ά��root��leftmost��rightmost
                root() = z;
                right_most() = z;
            }else if (y == left_most()){
                left_most() = z;
            }

        }
        else{//�ұ߲���
            z = create_node(v);
            right(y) = z;
            if(y == right_most()){
                right_most() = z;
             }
        }

        parent(z) = y;
        left(z) = 0;
        right(z) = 0;

        __rb_tree_rebalance(z, header->parent);
        ++node_count;
        return iterator(z);

    }




}


#endif // RB_TREE_H_INCLUDED
