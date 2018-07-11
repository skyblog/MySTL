#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include "deque.h"


namespace MySTL{
    //ǰ������
    template <class T, class Sequence>
    class stack;
    template <class T, class Sequence>
    bool operator==(const stack<T,Sequence>& x, const stack<T,Sequence>& y);

    template <class T, class Sequence>
    bool operator!=(const stack<T,Sequence>& x, const stack<T,Sequence>& y);



    template <class T, class Sequence = deque<T> >//Ĭ��dequeʵ��
    class stack{

        friend bool operator== <>(const stack&, const stack&);//�ǳ�Ա������Ԫ
        friend bool operator!= <>(const stack&, const stack&);

    public:
        typedef typename Sequence::value_type       value_type;
        typedef typename Sequence::size_type        size_type;
        typedef typename Sequence::reference        reference;
        typedef typename Sequence::const_reference  const_reference;
    protected:
        Sequence    c;
    public:
        bool empty() const {return c.empty();}
        size_type size() const {return c.size();}
        reference top() {return c.back();}
        const_reference top()const {return c.back();}

        void push(const value_type& x){c.push_back(x);}
        void pop(){c.pop_back();}

    };
    template <class T, class Sequence>
    bool operator==(const stack<T,Sequence>& x, const stack<T,Sequence>& y)
    {
        return x.c == y.c;
    }
    template <class T, class Sequence>
    bool operator!=(const stack<T,Sequence>& x, const stack<T,Sequence>& y)
    {
        return x.c != y.c;
    }
}



#endif // STACK_H_INCLUDED
