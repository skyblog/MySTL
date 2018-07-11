#ifndef HASH_TABLE_H_INCLUDED
#define HASH_TABLE_H_INCLUDED


#include "Allocator.h"
#include "Construct.h"
#include "UninitializedFunction.h"
#include "TypeTraits.h"
#include "Algorithm.h"
#include "Functional.h"
#include "vector.h"
#include "hash_function.h"

namespace MySTL
{
    template <class Value>
    struct __hashtable_node
    {
        __hashtable_node* next;
        Value val;
    };

    template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    struct __hashtable_iterator;
    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    class hashtable;
    template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    struct __hashtable_const_iterator;


    template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    struct __hashtable_iterator
    {
        typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>         _hashtable;
        typedef __hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>   iterator;
        typedef __hashtable_const_iterator<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>   const_iterator;
        typedef __hashtable_node<Value>                                             node;

        typedef forward_iterator_tag    iterator_category;
        typedef Value                   value_type;
        typedef ptrdiff_t               difference_type;
        typedef size_t                  size_type;
        typedef Value&                  reference;
        typedef Value*                  pointer;


        node*           cur;  //迭代器目前所指的节点
        _hashtable*      ht;   //保持对容器的连接关系 bucket跳到bucket

        __hashtable_iterator(node* n, _hashtable* tab): cur(n), ht(tab) {}
        __hashtable_iterator(){}
        reference operator*() const {return cur->val;}
        pointer operator->() const {return &(operator*());}
        iterator& operator++();
        iterator operator++(int)
        {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        bool operator==(const iterator& it) const {return cur == it.cur;}
        bool operator!=(const iterator& it) const {return cur != it.cur;}

    };

    template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    __hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>&
    __hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::operator++()
    {
        const node* old = cur;
        cur = cur->next;
        if(!cur){//下一个非空bucket的头
            size_type bucket = ht->bkt_num(old->val);
            while(!cur && ++bucket < ht->buckets.size())
                cur = ht->buckets[bucket];
        }
        return *this;
    }

     template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    struct __hashtable_const_iterator
    {
        typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>         _hashtable;
        typedef __hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>   iterator;
        typedef __hashtable_const_iterator<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>   const_iterator;
        typedef __hashtable_node<Value>                                             node;

        typedef forward_iterator_tag    iterator_category;
        typedef Value                   value_type;
        typedef ptrdiff_t               difference_type;
        typedef size_t                  size_type;
        typedef const Value&                  reference;
        typedef const Value*                  pointer;


        const node*           cur;  //迭代器目前所指的节点
        const _hashtable*      ht;   //保持对容器的连接关系 bucket跳到bucket

        __hashtable_const_iterator(const node* n, const _hashtable* tab): cur(n), ht(tab) {}
        __hashtable_const_iterator(){}
        __hashtable_const_iterator(const iterator& it):cur(it.cur), ht(it.ht){}

        reference operator*() const {return cur->val;}
        pointer operator->() const {return &(operator*());}

        const_iterator& operator++();
        const_iterator operator++(int)
        {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        bool operator==(const const_iterator& it) const {return cur == it.cur;}
        bool operator!=(const const_iterator& it) const {return cur != it.cur;}

    };

    template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    __hashtable_const_iterator<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>&
    __hashtable_const_iterator<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::operator++()
    {
        const node* old = cur;
        cur = cur->next;
        if(!cur){//下一个非空bucket的头
            size_type bucket = ht->bkt_num(old->val);
            while(!cur && ++bucket < ht->buckets.size())
                cur = ht->buckets[bucket];
        }
        return *this;
    }

    //28个质数
    static const int __stl_num_primes = 28;
    //这个范围：unsigned long 一共[0,2^32-1](0~4294967295)
    static const unsigned long __stl_prime_list[__stl_num_primes] =
    {
        53ul,         97ul,         193ul,       389ul,       769ul,
        1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
        49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
        1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
        50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
        1610612741ul, 3221225473ul, 4294967291ul
    };


    //找出上面28个质数中，最接近并且大于n的质数
    inline unsigned long __stl_next_prime(unsigned long n)
    {
        if (n < __stl_prime_list[0])
            return __stl_prime_list[0];

        int first = 0;
        int last = __stl_num_primes-1;
        int mid = last >> 1;
        while(first <= last)
        {
            if(__stl_prime_list[first] >= n){
                return __stl_prime_list[first];
            }


            if (__stl_prime_list[mid]< n)
                first = mid + 1;
            else if(__stl_prime_list[mid] == n)
                return __stl_prime_list[mid];
            else
                last = mid;

            mid = (first+last)/2;
        }
        return __stl_prime_list[__stl_num_primes-1];
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    bool operator==(const hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>& x, const hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>& y)
    {

        typedef typename hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::node node;
        if(x.buckets.size() != y.buckets.size())
            return false;
        for(size_t n=0; n<x.buckets.size(); ++n)
        {
            node* cur1 = x.buckets[n];
            node* cur2 = y.buckets[n];
            for(; cur1&& cur2 && cur1->val == cur2->val;cur1 = cur1->next, cur2=cur2->next){}
            if(cur1 || cur2)
                return false;
        }
        return true;
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    bool operator!=(const hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>& x, const hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>& y)
    {
        return !(x == y);
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc = Allocator<__hashtable_node<Value> > >
    class hashtable
    {
        friend bool operator== <>(const hashtable& ,const hashtable& );
		friend bool operator!= <>(const hashtable& ,const hashtable& );

        //定义友元，使得buckets等私有成员和函数在iterator中可用
        friend struct __hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>;
        friend struct __hashtable_const_iterator<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>;
    public:
        typedef HashFcn     hasher;
        typedef EqualKey    key_equal;
        typedef size_t      size_type;
        typedef Key         key_type;
        typedef Value       value_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef ptrdiff_t   difference_type;

        typedef __hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>   iterator;
        typedef __hashtable_const_iterator<value_type,Key,HashFcn,ExtractKey,EqualKey,Alloc>   const_iterator;


    private:
        hasher  hash;
        key_equal   equals;//判断键值是否相等的函数
        ExtractKey  get_key;

        typedef __hashtable_node<Value> node;
        typedef Alloc                   node_allocator;


        vector<node*> buckets;
        size_type   num_elements;

    private:
        size_type next_size(size_type n) const {return __stl_next_prime(n);}

        void initialize_buckets(size_type n)
        {
            const size_type n_buckets = next_size(n);
            buckets.reserve(n_buckets);
            buckets.insert(buckets.end(), n_buckets, (node*)0);
            num_elements = 0;
        }
        void copy_from(const hashtable& ht);
        node* new_node(const value_type& obj)
        {
            node* n = get_node();
            n->next = 0;
            try{
                construct(&n->val, obj);
                return n;
            }
            catch(...){
                put_node(n);
            }
        }
        void delete_node(node* p)
        {
            destroy(&p->val);
            put_node(p);
        }
        node* get_node(){return node_allocator::allocate();}
        void put_node(node* p){node_allocator::deallocate(p);}

        void erase_bucket(const size_type n, node* first, node* last);
        void erase_bucket(const size_type n, node* last);

        size_type bkt_num(const value_type& obj, size_type n) const
        {
            return bkt_num_key(get_key(obj), n);
        }
        size_type bkt_num(const value_type& obj) const
        {
            return bkt_num_key(get_key(obj));
        }
        size_type bkt_num_key(const key_type& key) const
        {
            return bkt_num_key(key, buckets.size());
        }
        size_type bkt_num_key(const key_type& key, size_type n) const
        {
            return hash(key) % n;
        }

        iterator insert_equal_noresize(const value_type& obj);



    public:
        //构造函数
        hashtable(size_type n, const HashFcn& hf, const EqualKey& eql):hash(hf),equals(eql), get_key(ExtractKey()),num_elements(0){initialize_buckets(n);}

        hashtable(size_type n, const HashFcn& hf, const EqualKey& eql, const ExtractKey& ext):hash(hf), equals(eql), get_key(ext),num_elements(0){initialize_buckets(n);}

        hashtable(const hashtable& ht):hash(ht.hash), equals(ht.equals), get_key(ht.get_key),num_elements(0){copy_from(ht);}

        hashtable& operator=(const hashtable& ht)
        {
            if(&ht != this)
            {
                clear();
                hash = ht.hash;
                equals = ht.equals;
                get_key = ht.get_key;
                copy_from(ht);
            }
            return *this;
        }

        ~hashtable() {clear();}

    public:
        size_type bucket_count() const {return buckets.size();}
        size_type max_bucket_count() const {return __stl_prime_list[__stl_num_primes-1];}
        bool empty() const {return size() == 0;}
        size_type size() const {return num_elements;}
        size_type max_size() const {return size_type(-1);}
        hasher hash_funct() const {return hash;}
        key_equal key_eq() const {return equals;}

        size_type elems_in_buckets(size_type bucket) const
        {
            size_type re = 0;
            node* tmp = buckets[bucket];
            while(tmp != 0)
            {
                ++re;
                tmp = tmp->next;
            }
            return re;
        }

        void swap(hashtable& ht)
        {
            hasher tmp;
            tmp = hash;
            hash = ht.hash;
            ht.hash = tmp;

            key_equal tmp1;
            tmp1 = equals;
            equals = ht.equals;
            ht.equals = tmp1;

            ExtractKey tmp2;
            tmp2 = get_key;
            get_key = ht.get_key;
            ht.get_key = tmp2;

            buckets.swap(ht.buckets);

            size_type tmp3;
            tmp3 = num_elements;
            num_elements = ht.num_elements;
            ht.num_elements = tmp3;

        }

        iterator begin()
        {
            for(size_type n = 0; n < buckets.size(); ++n){
                if(buckets[n])
                    return iterator(buckets[n], this);
            }
            return end();
        }
        iterator end() {return iterator(0,this);}


         const_iterator begin() const
        {
            for(size_type n = 0; n < buckets.size(); ++n){
                if(buckets[n])
                    return const_iterator(buckets[n], this);
            }
            return end();
        }
        const_iterator end() const {return const_iterator(0,this);}


        pair<iterator, bool> insert_unique(const value_type& obj)
        {
            resize(num_elements+1);
            return insert_unique_noresize(obj);
        }

        pair<iterator, bool> insert_unique_noresize(const value_type& obj);
        void resize(size_type num_elements_hint);

        iterator insert_equal(const value_type& obj)
        {
            resize(num_elements+1);
            return insert_equal_noresize(obj);
        }
        //iterator insert_equal_noresize(const value_type& obj);

        pair<iterator,iterator> equal_range(const key_type& key)
        {
            typedef pair<iterator,iterator> re;
            const size_type n = bkt_num_key(key);

            for(node* first = buckets[n]; first; first = first->next)
            {
                if(equals(get_key(first->val),key))
                {
                    for(node* cur = first->next; cur; cur=cur->next)
                    {
                        if(!equals(get_key(cur->val),key))
                            return re(iterator(first,this),iterator(cur,this));
                    }

                    for(size_type m = n+1; m<buckets.size();++m)
                    {
                        if(buckets[m])
                            return re(iterator(first,this),iterator(buckets[m],this));
                    }
                    return re(iterator(first,this),end());
                }
            }
            return re(end(),end());
        }
        pair<const_iterator, const_iterator> equal_range(const key_type& key) const
        {
            typedef pair<const_iterator,const_iterator> re;
            const size_type n = bkt_num_key(key);

            for(node* first = buckets[n]; first; first = first->next)
            {
                if(equals(get_key(first->val),key))
                {
                    for(node* cur = first->next; cur; cur=cur->next)
                    {
                        if(!equals(get_key(cur->val),key))
                            return re(const_iterator(first,this),const_iterator(cur,this));
                    }

                    for(size_type m = n+1; m<buckets.size();++m)
                    {
                        if(buckets[m])
                            return re(const_iterator(first,this),const_iterator(buckets[m],this));
                    }
                    return re(const_iterator(first,this),end());
                }
            }
            return re(end(),end());
        }

        void clear();
        size_type erase(const key_type& key);
        void erase(const iterator& it);
        void erase(iterator first, iterator last);
        void erase(const const_iterator& it);
        void erase(const_iterator first, const_iterator last);

        iterator find(const key_type& key)
        {
            size_type n = bkt_num_key(key);
            node* first = buckets[n];
            while(first && !equals(get_key(first->val),key))
                first = first->next;
            return iterator(first,this);
        }
        reference find_or_insert(const value_type& obj)
        {
            resize(num_elements+1);

            size_type n = bkt_num(obj);
            node* first = buckets[n];

            for(node* cur=first;cur;cur=cur->next)
            {
                if(equals(get_key(cur->val),get_key(obj)))
                    return cur->val;
            }

            node* tmp = new_node(obj);
            tmp->next = first;
            buckets[n] = tmp;
            ++num_elements;
            return tmp->val;

        }

        const_iterator find(const key_type& key) const
        {
            size_type n = bkt_num_key(key);
            node* first = buckets[n];
            while(first && !equals(get_key(first->val),key))
                first = first->next;
            return const_iterator(first,this);
        }


        size_type count(const key_type& key) const
        {
            const size_type n = bkt_num_key(key);
            size_type re = 0;
            for(const node* cur = buckets[n]; cur; cur = cur->next)
            {
                if(equals(get_key(cur->val), key))
                    ++re;
            }
            return re;
        }
    };

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::erase_bucket(const size_type n, node* first, node* last)
    {
        node* cur = buckets[n];
        if(cur == first)
            erase_bucket(n,last);
        else{
            node* next;
            for(next = cur->next; next != first; cur = next, next = cur->next);

            while(next!=last)
            {
                cur->next = next->next;
                delete_node(next);
                next = cur->next;
                --num_elements;
            }

        }
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::erase_bucket(const size_type n, node* last)
    {
        node* cur = buckets[n];


        while(cur!=last)
        {
            node* next = cur->next;
            delete_node(cur);
            cur = next;
            buckets[n] = cur;
            --num_elements;
        }


    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::size_type hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::erase(const key_type& key)
    {
        const size_type n = bkt_num_key(key);
        node* first = buckets[n];
        size_type erased = 0;

        if(first){
            node* cur = first;
            node* next = cur->next;
            while(next)
            {
                if(equals(get_key(next->val),key))
                {
                    cur->next = next->next;
                    delete_node(next);
                    next = cur->next;
                    ++erased;
                    --num_elements;
                }
                else{
                    cur=next;
                    next = cur->next;
                }
            }

            if(equals(get_key(first->val),key))
            {
                buckets[n] = first->next;
                delete_node(first);
                ++erased;
                --num_elements;
            }
        }
        return erased;
    }


    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::erase(iterator first, iterator last)
    {
        size_type f_bucket = first.cur? bkt_num(first.cur->val):buckets.size();
        size_type l_bucket = last.cur? bkt_num(last.cur->val):buckets.size();

        if(first.cur == last.cur)
            return;
        else if(f_bucket == l_bucket)
            erase_bucket(f_bucket, first.cur, last.cur );
        else{
            erase_bucket(f_bucket, first.cur, 0 );
            for(size_type n = f_bucket+1; n<l_bucket; ++n)
                erase_bucket(n,0);
            if(l_bucket != buckets.size())
                erase_bucket(l_bucket, last.cur);
        }
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::erase(const_iterator first, const_iterator last)
    {
        erase(iterator(const_cast<node*>(first.cur),const_cast<hashtable*>(first.ht)),iterator(const_cast<node*>(last.cur),const_cast<hashtable*>(last.ht)));
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::erase(const iterator& it)
    {
        node* p = it.cur;
        if(p)
        {
            //找到p的前置节点
            const size_type n = bkt_num(p->val);
            node* cur = buckets[n];

            if(cur == p)
            {
                buckets[n] = cur->next;
                delete_node(cur);
                --num_elements;
            }
            else{
                while(cur->next!=0 && cur->next != p)
                {
                    cur =cur->next;
                }
                if(cur->next == p)
                {
                    cur->next = p->next;
                    delete_node(p);
                    --num_elements;
                }
            }

        }
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    inline void hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::erase(const const_iterator& it)
    {
        erase(iterator(const_cast<node*>(it.cur),const_cast<hashtable*>(it.ht)));
    }


    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::clear()
    {
        for(size_type i=0; i<buckets.size(); ++i)
        {
            node* cur = buckets[i];

            while(cur!=0)
            {
                node* next = cur->next;
                delete_node(cur);
                cur = next;
            }
            buckets[i] = 0;
        }
        num_elements = 0;
    }


    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    typename hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::iterator
    hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::insert_equal_noresize(const value_type& obj)
    {
        const size_type n = bkt_num(obj);
        node* first = buckets[n];

        for(node* cur = first; cur; cur = cur->next)
        {
            //若有相同key值，不插入
            if(equals(get_key(cur->val), get_key(obj))){
                node* tmp = new_node(obj);
                tmp->next = cur->next;
                cur->next = tmp;
                ++num_elements;
                return iterator(tmp,this);
            }
        }

        node* tmp = new_node(obj);
        tmp->next = first;
        buckets[n] = tmp;
        ++num_elements;
        return iterator(tmp,this);
    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    pair<typename hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::iterator, bool>
    hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::insert_unique_noresize(const value_type& obj)
    {
        const size_type n = bkt_num(obj);
        node* first = buckets[n];

        for(node* cur = first; cur; cur = cur->next)
        {
            //若有相同key值，不插入
            if(equals(get_key(cur->val), get_key(obj)))
                return pair<iterator,bool>(iterator(cur,this), false);
        }

        node* tmp = new_node(obj);
        tmp->next = first;
        buckets[n] = tmp;
        ++num_elements;
        return pair<iterator, bool>(iterator(tmp,this), true);
    }



    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::resize(size_type num_elements_hint)
    {
        const size_type old_n = buckets.size();

        if(old_n < num_elements_hint)//须重建表格
        {
            const size_type n = next_size(num_elements_hint);
            if( n > old_n)
            {
                vector<node*> tmp(n, (node*)0);

                try{
                    for(size_type bucket = 0; bucket < old_n; ++bucket)
                    {
                        node* first = buckets[bucket];
                        while(first)
                        {
                            //节点落在哪个新的bucket中
                            size_type new_bucket = bkt_num(first->val, n);
                            buckets[bucket] = first->next;
                            first->next = tmp[new_bucket];
                            tmp[new_bucket] = first;
                            first = buckets[bucket];
                        }
                    }
                    buckets.swap(tmp);
                }catch(...){
                    for(size_type bu = 0; bu<tmp.size(); ++bu)
                    {
                        while(tmp[bu])
                        {
                            node* next = tmp[bu]->next;
                            delete_node(tmp[bu]);
                            tmp[bu] = next;
                        }
                    }
                    throw;
                }
            }
        }

    }

    template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
    void hashtable<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::copy_from(const hashtable& ht)
    {
        buckets.clear();
        buckets.reserve(ht.buckets.size());
        buckets.insert(buckets.end(),ht.buckets.size(),(node*)0);

        try
        {
            for(size_type i=0; i< ht.buckets.size(); ++i){
                const node* cur = ht.buckets[i];
                if(cur){
                    node* _copy = new_node(cur->val);
                    buckets[i] = _copy;

                    for(node* _next = cur->next; _next; cur = _next, _next = cur->next){
                        _copy->next = new_node(_next->val);
                        _copy = _copy->next;
                    }
                }
            }
            num_elements = ht.num_elements;
        }catch(...){
            clear();
        }
    }

}


#endif // HASH_TABLE_H_INCLUDED
