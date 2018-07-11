#ifndef HASH_MAP_H_INCLUDED
#define HASH_MAP_H_INCLUDED

#include "Allocator.h"
#include "Construct.h"
#include "UninitializedFunction.h"
#include "TypeTraits.h"
#include "Algorithm.h"
#include "Functional.h"
#include "hash_table.h"
#include "hash_function.h"

namespace MySTL
{

    template <class Key, class T,class HashFcn = hash<Key>,class EqualKey = equal_to<Key>,class Alloc = Allocator<__hashtable_node<pair<const Key, T> > > >
    class hash_map;

    template <class Key, class T, class HashFcn, class EqualKey, class Alloc>
    inline bool operator==(const hash_map<Key, T, HashFcn, EqualKey, Alloc>& hs1,const hash_map<Key, T, HashFcn, EqualKey, Alloc>& hs2)
    {
        return hs1.rep == hs2.rep;
    }

    template <class Key, class T, class HashFcn, class EqualKey, class Alloc>
    inline bool operator!=(const hash_map<Key, T, HashFcn, EqualKey, Alloc>& hs1,const hash_map<Key,T, HashFcn, EqualKey, Alloc>& hs2)
    {
        return !(hs1.rep == hs2.rep);
    }

    template <class Key, class T, class HashFcn, class EqualKey, class Alloc>
    class hash_map
    {
        friend bool operator== <> (const hash_map&, const hash_map&);
        friend bool operator!= <> (const hash_map&, const hash_map&);

    private:
        typedef hashtable<pair<const Key,T>, Key, HashFcn, select1st<pair<const Key,T>>, EqualKey, Alloc> ht;
        ht  rep;

    public:
        typedef typename ht::key_type      key_type;
        typedef T                           data_type;
        typedef T                           mapped_type;
        typedef typename ht::value_type     value_type;
        typedef typename ht::hasher         hasher;
        typedef typename ht::key_equal      key_equal;

        typedef typename ht::size_type      size_type;
        typedef typename ht::difference_type    difference_type;
        typedef typename ht::const_pointer  const_pointer;
        typedef typename ht::pointer        pointer;
        typedef typename ht::reference    reference;
        typedef typename ht::const_reference    const_reference;

        typedef typename ht::const_iterator     const_iterator;
        typedef typename ht::iterator     iterator;

        hasher hash_funct() const {return rep.hash_funct();}
        key_equal key_eq() const {return rep.key_eq();}

    public:
        hash_map():rep(100, hasher(), key_equal()){}
        explicit hash_map(size_type n):rep(n, hasher(), key_equal()){}
        hash_map(size_type n, const hasher& hf): rep(n, hf, key_equal()){}
        hash_map(size_type n, const hasher& hf, const key_equal& eql):rep(n,hf,eql){}

        template <class InputIterator>
        hash_map(InputIterator f, InputIterator l):rep(100, hasher(), key_equal())
        {
            rep.insert_unique(f,l);
        }

        template <class InputIterator>
        hash_map(InputIterator f, InputIterator l, size_type n):rep(n, hasher(), key_equal())
        {
            rep.insert_unique(f,l);
        }

        template <class InputIterator>
        hash_map(InputIterator f, InputIterator l, size_type n, const hasher& hf):rep(n, hf, key_equal())
        {
            rep.insert_unique(f,l);
        }
        template <class InputIterator>
        hash_map(InputIterator f, InputIterator l,size_type n, const hasher& hf, const key_equal& eql):rep(n, hf, eql)
        {
            rep.insert_unique(f,l);
        }

    public:
        size_type size() const {return rep.size();}
        size_type max_size() const {return rep.maxsize();}
        bool empty() const {return rep.empty();}
        void swap(hash_map& hs) {rep.swap(hs.rep);}


        iterator begin()  {return rep.begin();}
        iterator end()  {return rep.end();}
        const_iterator begin() const {return rep.begin();}
        const_iterator end() const {return rep.end();}

    public:
        pair<iterator, bool> insert(const value_type& obj)
        {
            return rep.insert_unique(obj);
        }

        template<class InputIterator>
        void insert(InputIterator f, InputIterator l)
        {
            rep.insert_unique(f,l);
        }
        iterator find(const key_type& key) {return rep.find(key);}
        const_iterator find(const key_type& key) const {return rep.find(key);}

        T& operator[](const key_type& key)
        {
            return rep.find_or_insert(value_type(key,T())).second;
        }

        size_type count(const key_type& key) const {return rep.count(key);}

        pair<iterator,iterator>equal_range(const key_type& key)
        {
            return rep.equal_range(key);
        }
        pair<const_iterator,const_iterator>equal_range(const key_type& key) const
        {
            return rep.equal_range(key);
        }

        size_type erase(const key_type& key){return rep.erase(key);}
        void erase(iterator it){return rep.erase(it);}
        void erase(iterator f, iterator l){return rep.erase(f,l);}
        void clear(){rep.clear();}

    public:
        void resize(size_type hint) {rep.resize(hint);}
        size_type bucket_count() const {return rep.bucket_count();}
        size_type max_bucket_count() const {return rep.max_bucket_count();}
        size_type elems_in_buckets(size_type n) const {return rep.elems_in_buckets(n);}



    };
}

#endif // HASH_MAP_H_INCLUDED
