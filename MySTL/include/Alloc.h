#ifndef ALLOC_H
#define ALLOC_H

#include <cstdlib>

namespace MySTL{
    class Alloc{
    private:
        enum {__ALIGN = 8};//小型区块的上调边界,小额区块的内存需求：8的倍数
        enum {__MAX_BYTES = 128};//小型区块的上界，超过由malloc分配，一级配置器；小于由二级配置器，内存池来管理，以free-list来实现
        enum {__NFREELISTS = __MAX_BYTES/__ALIGN};//free-list的个数
        enum {NOBJS = 20};//refill大小缺省值，每次增加节点数/区块数
    private:
        //free-list的节点构造，一物二用，节省空间
        union obj{
            union obj *next;//指向下一节点
            char client_data[1];//指针，指向实际区块，char占一个字节
        };
    private:
        //内存池free-list，free-list[i]指向8*（i+1）bytes大小的内存
        static obj * volatile free_list[__NFREELISTS];
        //chunk allocation state
        static char *start_free;//内存池起始位置
        static char *end_free;//内存池结束位置
        static size_t heap_size;//内存池增加附加量，随配置次数增加

    private:
        //将bytes上调到8的倍数
        static size_t ROUND_UP(size_t bytes){
            return ((bytes + __ALIGN -1) & ~(__ALIGN-1));
        }
        //根据区块大小，决定使用n个free-list区块
        static size_t FREELIST_INDEX(size_t bytes){
            return ((bytes + __ALIGN -1)/__ALIGN-1);
        }
        //返回一个大小为n的对象，并可能加入大小为n的其他区块到free-list
        static void* refill(size_t bytes);
        //配置一大块空间，可容纳nobjs个大小为size的区块
		//如果配置nobjs个区块有所不便，nobjs可能会降低
		static char* chunk_alloc(size_t bytes, int &nobjs);
    public:
        static void *allocate(size_t bytes);//空间配置函数
        static void deallocate(void *p, size_t bytes);//回收内存块
        static void *reallocate(void *p, size_t old_sz,size_t new_sz);//重新分配空间
    };
}



#endif // ALLOC_H
