#ifndef ALLOC_H
#define ALLOC_H

#include <cstdlib>

namespace MySTL{
    class Alloc{
    private:
        enum {__ALIGN = 8};//С��������ϵ��߽�,С��������ڴ�����8�ı���
        enum {__MAX_BYTES = 128};//С��������Ͻ磬������malloc���䣬һ����������С���ɶ������������ڴ����������free-list��ʵ��
        enum {__NFREELISTS = __MAX_BYTES/__ALIGN};//free-list�ĸ���
        enum {NOBJS = 20};//refill��Сȱʡֵ��ÿ�����ӽڵ���/������
    private:
        //free-list�Ľڵ㹹�죬һ����ã���ʡ�ռ�
        union obj{
            union obj *next;//ָ����һ�ڵ�
            char client_data[1];//ָ�룬ָ��ʵ�����飬charռһ���ֽ�
        };
    private:
        //�ڴ��free-list��free-list[i]ָ��8*��i+1��bytes��С���ڴ�
        static obj * volatile free_list[__NFREELISTS];
        //chunk allocation state
        static char *start_free;//�ڴ����ʼλ��
        static char *end_free;//�ڴ�ؽ���λ��
        static size_t heap_size;//�ڴ�����Ӹ������������ô�������

    private:
        //��bytes�ϵ���8�ı���
        static size_t ROUND_UP(size_t bytes){
            return ((bytes + __ALIGN -1) & ~(__ALIGN-1));
        }
        //���������С������ʹ��n��free-list����
        static size_t FREELIST_INDEX(size_t bytes){
            return ((bytes + __ALIGN -1)/__ALIGN-1);
        }
        //����һ����СΪn�Ķ��󣬲����ܼ����СΪn���������鵽free-list
        static void* refill(size_t bytes);
        //����һ���ռ䣬������nobjs����СΪsize������
		//�������nobjs�������������㣬nobjs���ܻή��
		static char* chunk_alloc(size_t bytes, int &nobjs);
    public:
        static void *allocate(size_t bytes);//�ռ����ú���
        static void deallocate(void *p, size_t bytes);//�����ڴ��
        static void *reallocate(void *p, size_t old_sz,size_t new_sz);//���·���ռ�
    };
}



#endif // ALLOC_H
