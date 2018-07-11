#ifndef HASH_FUNCTION_H_INCLUDED
#define HASH_FUNCTION_H_INCLUDED

namespace MySTL
{
    template <class Key>
    struct hash{};


        //对const char* 提供字符串转换函数
    inline size_t __stl_hash_string(const char* __s)
    {
      unsigned long __h = 0;
      for ( ; *__s; ++__s)
        __h = 5*__h + *__s;

      return size_t(__h);
    }

    template<> struct hash<char*>
    {
      size_t operator()(const char* __s) const { return __stl_hash_string(__s); }
    };

    template<> struct hash<const char*>
    {
      size_t operator()(const char* __s) const { return __stl_hash_string(__s); }
    };

    //下面的hash函数都是直接返回原值
    //对于char,unsigned char,signed char,int,unsigned int,
    //short, unsigned short, long,unsigned long都只是返回数值本身
    template<> struct hash<char> {
      size_t operator()(char __x) const { return __x; }
    };
    template<> struct hash<unsigned char> {
      size_t operator()(unsigned char __x) const { return __x; }
    };
    template<> struct hash<signed char> {
      size_t operator()(unsigned char __x) const { return __x; }
    };
    template<> struct hash<short> {
      size_t operator()(short __x) const { return __x; }
    };
    template<> struct hash<unsigned short> {
      size_t operator()(unsigned short __x) const { return __x; }
    };
    template<> struct hash<int> {
      size_t operator()(int __x) const { return __x; }
    };
    template<> struct hash<unsigned int> {
      size_t operator()(unsigned int __x) const { return __x; }
    };
    template<> struct hash<long> {
      size_t operator()(long __x) const { return __x; }
    };
    template<> struct hash<unsigned long> {
      size_t operator()(unsigned long __x) const { return __x; }
    };


}

#endif // HASH_FUNCTION_H_INCLUDED
