#include "Alloc.h"

namespace MySTL
{


    //staic��Ա���������ʼ��
    char *Alloc::start_free = 0;
    char *Alloc::end_free = 0;
    size_t Alloc::heap_size  =0;

    Alloc::obj *volatile Alloc::free_list[Alloc::__NFREELISTS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};

    //�ռ����ú���
    void *Alloc::allocate(size_t bytes)
    {
        //����128��һ����������ֱ��malloc
        if(bytes>(size_t)__MAX_BYTES){
            return malloc(bytes);
        }
        //<128,�������������ڴ��
        size_t index = FREELIST_INDEX(bytes);
        obj *volatile result = free_list[index];
        if(result){//�пռ�
            free_list[index] = result->next;
            return result;
        }else{//û���㹻�ռ�
            return refill(ROUND_UP(bytes));
        }
    }
    //�����ڴ��
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

    //���·���ռ�
    void *Alloc::reallocate(void *p, size_t old_sz,size_t new_sz)
    {
        deallocate(p,old_sz);
        p = allocate(new_sz);
        return p;
    }
    //Ϊ�ڴ�����ӿռ䣬���ش�СΪbytes���󣬼���bytes���ϵ���8�ı���
    void *Alloc::refill(size_t bytes)
    {
        int nobjs = NOBJS;

        //
        char *chunk = chunk_alloc(bytes, nobjs);
        //obj *volatile *my_free_list = 0;
        obj *result = 0;
        obj *current_obj=0,*next_obj = 0;

        if(nobjs == 1){//��ֻ���һ�����飬������ָ������ߣ�free-list���½ڵ�
            return chunk;
        }
        //����
        size_t index = FREELIST_INDEX(bytes);
        result = (obj*)(chunk);
        //free_listָ���¿ռ䣬ע��chunk[0]����������ߣ�
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
    //���ڴ����ȡ�ռ��free-list�ã�nobjs����СΪbytes���������飬bytes�Ѿ���8�ı���
    char *Alloc::chunk_alloc(size_t bytes, int &nobjs)
    {
        char *result = 0;
        size_t total_bytes = bytes*nobjs;
        size_t bytes_left = end_free - start_free;

        if(bytes_left >= total_bytes){//�ڴ��ʣ��������ȫ��
            result = start_free;
            start_free += total_bytes;
            return result;
        }
        else if (bytes_left > bytes){//ʣ����������ȫ���������ٹ�һ��
            nobjs = bytes_left/bytes;
            total_bytes = nobjs * bytes;
            result = start_free;
            start_free += total_bytes;
            return result;
        }
        else{//һ��������ʱ�������ڴ�صĴ�С
            //��ˮ��Ϊ����������������һ�������ô���Խ��Խ��ĸ�����
            size_t bytes_to_get = 2*total_bytes + ROUND_UP(heap_size >> 4);

            if(bytes_left >0){//�ڴ�ػ���Щʣ���,����ʵ���free-list����
                size_t index = FREELIST_INDEX(bytes_left);
                ((obj*)start_free)->next = free_list[index];
                free_list[index] = (obj*)start_free;
            }

            //����heap�ѿռ䣬���������ڴ��
            start_free = (char *)malloc(bytes_to_get);

            if(0 == start_free){//����ʧ��,���ѷ�����free-list�е�δ�������ڴ��ͷŵ��ڴ���У������㹻�����bytes��ʼ
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
                end_free = 0;//ɽ��ˮ�����׳��쳣
            }


            heap_size += bytes_to_get;
            end_free = start_free + bytes_to_get;
            return chunk_alloc(bytes,nobjs);//�ݹ�����Լ���Ϊ������nlbjs


        }
    }
}

