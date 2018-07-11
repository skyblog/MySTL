#ifndef FUNCTIONAL_H_INCLUDED
#define FUNCTIONAL_H_INCLUDED

namespace MySTL
{
    template<class T>
    struct less{
        typedef T first_argument_type;
        typedef T second_argument_type;
        typedef bool result_type;

        result_type operator()(const first_argument_type& x, const second_argument_type& y)
        {
            return x<y;
        }
        result_type operator()(const first_argument_type& x, const second_argument_type& y) const
        {
            return x<y;
        }
    };

    template <class Arg, class Result>
    struct unary_function{
        typedef Arg     argument_type;
        typedef Result  result_type;
    };

    template <class Arg1, class Arg2, class Result>
    struct binary_function{
        typedef Arg1  first_argument_type;
        typedef Arg2  second_argument_type;
        typedef Result  result_type;
    };

    template<class T>
    struct identity :public unary_function<T,T>{
        const T& operator()(const T& x) const {return x;}
    };

    template<class Pair>
    struct select1st: public unary_function<Pair, typename Pair::first_type>
    {
        const typename Pair::first_type& operator()(const Pair& x) const
        {
            return x.first;
        }
    };

    template <class T>
    struct equal_to:public binary_function<T,T,bool>
    {
        bool operator()(const T& x, const T& y) const
        {
            return x==y;
        }
    };


}


#endif // FUNCTIONAL_H_INCLUDED
