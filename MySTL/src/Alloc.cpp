#include "Alloc.h"

namespace MySTL
{


    //staic成员变量类外初始化
    char *Alloc::start_free = 0;
    char *Alloc::end_free = 0;
    size_t Alloc::heap_size  =0;

    Alloc::obj *volatile Alloc::free_list[Alloc::__NFREELISTS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};

    //空间配置函数
    void *Alloc::allocate(size_t bytes)
    {
        //大于128，一级配置器，直接malloc
        if(bytes>(size_t)__MAX_BYTES){
            return malloc(bytes);
        }
        //<128,二级配置器，内存池
        size_t index = FREELIST_INDEX(bytes);
        obj *volatile result = free_list[index];
        if(result){//有空间
            free_list[index] = result->next;
            return result;
        }else{//没有足够空间
            return refill(ROUND_UP(bytes));
        }
    }
    //回收内存块
    void Alloc::deallocate(void *p, size_t bytes)
    {
        if(bytes > (size_t)__MAX_BYTES){
            free(p);
        }else{
            size_t index = FREELIST_INDEX(bytes);
            obj *tmp = static_cast<obj*>(p);
            tmp->next = free_list[index];
            free_list[index] = tmp;
        }
    }

    //重新分配空间
    void *Alloc::reallocate(void *p, size_t old_sz,size_t new_sz)
    {
        deallocate(p,old_sz);
        p = allocate(new_sz);
        return p;
    }
    //为内存池增加空间，返回大小为bytes对象，假设bytes已上调至8的倍数
    void *Alloc::refill(size_t bytes)
    {
        int nobjs = NOBJS;

        //
        char *chunk = chunk_alloc(bytes, nobjs);
        //obj *volatile *my_free_list = 0;
        obj *result = 0;
        obj *current_obj=0,*next_obj = 0;

        if(nobjs == 1){//若只获得一个区块，此区块分给调用者，free-list无新节点
            return chunk;
        }
        //否则
        size_t index = FREELIST_INDEX(bytes);
        result = (obj*)(chunk);
        //free_list指向新空间，注意chunk[0]分配给调用者，
        free_list[index] = next_obj = (obj*)(chunk+bytes);
        for(int i=1;i<nobjs;++i){
            current_obj = next_obj;
            next_obj = (obj*)((char*)next_obj+bytes);
            if(nobjs-1 == i){
                current_obj->next = 0;
                break;
            }else{
                current_obj->next = next_obj;
            }

        }
        return result;
    }
    //从内存池中取空间给free-list用，nobjs个大小为bytes的连续区块，bytes已经是8的倍数
    char *Alloc::chunk_alloc(size_t bytes, int &nobjs)
    {
        char *result = 0;
        size_t total_bytes = bytes*nobjs;
        size_t bytes_left = end_free - start_free;

        if(bytes_left >= total_bytes){//内存池剩余容量完全够
            result = start_free;
            start_free += total_bytes;
            return result;
        }
        else if (bytes_left > bytes){//剩余容量不完全够，但至少够一个
            nobjs = bytes_left/bytes;
            total_bytes = nobjs * bytes;
            result = start_free;
            start_free += total_bytes;
            return result;
        }
        else{//一个都不够时，增加内存池的大小
            //新水量为需求量的两倍加上一个随配置次数越来越大的附加量
            size_t bytes_to_get = 2*total_bytes + ROUND_UP(heap_size >> 4);

            if(bytes_left >0){//内存池还有些剩余的,配给适当的free-list部分
                size_t index = FREELIST_INDEX(bytes_left);
                ((obj*)start_free)->next = free_list[index];
                free_list[index] = (obj*)start_free;
            }

            //配置heap堆空间，用来补充内存池
            start_free = (char *)malloc(bytes_to_get);

            if(0 == start_free){//分配失败,将已分配在free-list中的未用区块内存释放到内存池中，区块足够大，则从bytes开始
                size_t index = 0;
                obj *p;
                for(int i=bytes; i<__MAX_BYTES; i+=__ALIGN){
                    index = FREELIST_INDEX(i);
                    p = free_list[index];
                    if(p != 0){
                        free_list[index] = p->next;
                        start_free = (char*)p;
                        end_free = start_free + i;
                        return(chunk_alloc(bytes,nobjs));
                    }
                }
                end_free = 0;//山穷水尽，抛出异常
            }


            heap_size += bytes_to_get;
            end_free = start_free + bytes_to_get;
            return chunk_alloc(bytes,nobjs);//递归调用自己，为了修正nlbjs


        }
    }
}

