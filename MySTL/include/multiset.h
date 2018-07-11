#ifndef MULTISET_H_INCLUDED
#define MULTISET_H_INCLUDED

#include "Allocator.h"
#include "Construct.h"
#include "UninitializedFunction.h"
#include "TypeTraits.h"
#include "Algorithm.h"
#include "Functional.h"
#include "rb_tree.h"
#include "pair.h"

namespace MySTL
{
    template <class Key, class Compare, class Alloc>
    class multiset;
    template <class Key, class Compare, class Alloc>
    inline bool operator==(const multiset<Key,Compare,Alloc>& x, const multiset<Key,Compare,Alloc>& y )
    {
        return x.t == y.t;
    }
    template <class Key, class Compare, class Alloc>
    inline bool operator!=(const multiset<Key,Compare,Alloc>& x, const multiset<Key,Compare,Alloc>& y )
    {
        return x.t != y.t;
    }


    template <class Key, class Compare = less<Key>, class Alloc =Allocator<__rb_tree_node<Key> > >
    class multiset
    {
    public:
        typedef Key     key_type;
        typedef Key     value_type;
        typedef Compare key_compare;
        typedef Compare value_compare;
    private:
        typedef rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;
        rep_type    t;


    public:
        typedef typename rep_type::const_pointer    pointer;       //
        typedef typename rep_type::const_pointer    const_pointer;
        typedef typename rep_type::const_reference        reference;
        typedef typename rep_type::const_reference  const_reference;
        typedef typename rep_type::const_iterator         iterator; //const——iterator表示set不能执行写入操作，因为其按一定次序排列
        typedef typename rep_type::const_iterator   const_iterator;
        typedef typename rep_type::size_type        size_type;
        typedef typename rep_type::difference_type   difference_type;

    public:
        //构造函数
        multiset():t(Compare()){}
        explicit multiset(const Compare& comp):t(comp){}

        template <class InputIterator>
        multiset(InputIterator first, InputIterator last):t(Compare()){t.insert_equal(first,last);}

        template <class InputIterator>
        multiset(InputIterator first, InputIterator last, const Compare& comp):t(comp){t.insert_equal(first,last);}

        multiset(const multiset<Key, Compare, Alloc>& x):t(x.t){}

        //拷贝构造函数
        multiset<Key, Compare, Alloc>& operator=(const multiset<Key, Compare, Alloc>& x)
        {
            t = x.t;
            return *this;
        }

    public:
        key_compare key_comp() const {return t.key_comp();}
        value_compare value_comp() const {return t.key_comp();}
        iterator begin() const {return t.begin();}
        iterator end() const {return t.end();}
        bool empty() const {return t.empty();}
        size_type size() const {return t.size();}
        size_type max_size() const {return t.max_size();}
        void swap(multiset<Key, Compare, Alloc>& x){return t.swap(x.t);}

        //insert
        typedef pair<iterator, bool> pair_iterator_bool;
        pair<iterator, bool> insert(const value_type& x)
        {
            pair_iterator_bool p = t.insert_equal(x);
            return pair<iterator,bool>(p.first, p.second);
        }
        template<class InputIterator>
        void insert(InputIterator first, InputIterator last)
        {
            t.insert_equal(first,last);
        }
       //erase
        void erase(iterator position)
        {
            typedef typename rep_type::iterator rep_iterator;
            t.erase((rep_iterator&)position);
        }
        size_type erase(const key_type& x)
        {
            return t.erase(x);
        }
        void erase(iterator first, iterator last)
        {
            typedef typename rep_type::iterator rep_iterator;
            t.erase((rep_iterator&)first, (rep_iterator&)last);
        }

        void clear(){t.clear();}

        iterator find(const key_type& x) const {return t.find(x);}
        size_type count(const key_type& x) const {return t.count(x);}
        iterator lower_bound(const key_type& x) const {return t.lower_bound(x);}
        iterator upper_bound(const key_type& x) const {return t.upper_bound(x);}
        pair<iterator,iterator> equal_range(const key_type& x) const {return t.equal_range(x);}

        friend bool operator == <> (const multiset&, const multiset&);
        friend bool operator != <> (const multiset&, const multiset&);

    };
}

#endif // MULTISET_H_INCLUDED
