#include <iostream>
#include "Alloc.h"
#include "Construct.h"
#include "UninitializedFunction.h"
#include "Functional.h"
#include "Allocator.h"
#include "vector.h"
#include "list.h"
#include "deque.h"
#include "stack.h"
#include "queue.h"
#include "Algorithm.h"
#include "priority_queue.h"
#include "slist.h"
#include "rb_tree.h"
#include "set.h"
#include "map.h"
#include "multimap.h"
#include "multiset.h"
#include "hash_table.h"
#include "hash_set.h"
#include "hash_map.h"

using namespace std;

void VectorTest();
void ListTest();
void DequeTest();
void StackTest();
void QueueTest();
void HeapTest();
void PQueue_Test();
void SlistTest();
void RBtreeTest();
void SetTest();
void MapTest();
void HashtableTest();
void HashSetTest();
void HashMapTest();

struct eqstr
{
    bool operator()(const char* s1, const char* s2) const {return strcmp(s1,s2) == 0;}
};
int main()
{
    //VectorTest();
    //ListTest();
    //DequeTest();
    //StackTest();
    //QueueTest();
    //HeapTest();
    //PQueue_Test();
    //SlistTest();
    //RBtreeTest();
    //SetTest();
   // MapTest();
   //HashtableTest();

   //HashSetTest();
   HashMapTest();


    return 0;
}
void HashMapTest()
{
    MySTL::hash_map<const char*, int, hash<const char*>, eqstr> simap;
    simap["jjhou"] = 1;
    simap["jerry"] = 2;
    simap["jason"] = 3;
    simap["jimmy"] = 4;

    MySTL::pair<const char*,int> value("david",5);
    simap.insert(value);

    MySTL::hash_map<const char*, int, hash<const char*>, eqstr>::iterator  iter = simap.begin();
    for(;iter!=simap.end();++iter){
        cout << iter->first << " " << iter->second << endl;
    }


    int number = simap["jjhou"];
    cout << number << endl;

    MySTL::hash_map<const char*, int, hash<const char*>, eqstr>::iterator  iter2;

    iter2 = simap.find("mchen");
    if(iter2 == simap.end())
        cout << "Not Found" << endl;

    iter2 = simap.find("jerry");
    if(iter2 != simap.end())
        cout << "Found" << endl;

    iter2->second = 9;
    int number2 = simap["jerry"];
    cout << number2 << endl;


    simap["aa"] = 3;
    simap["bb"] = 3;
    simap["cc"] = 3;
    iter = simap.begin();
    for(;iter!=simap.end();++iter){
        cout << iter->first << " " << iter->second << endl;
    }
    simap.erase("aa");
    cout << "-----------"<<endl;
    iter = simap.begin();
    for(;iter!=simap.end();++iter){
        cout << iter->first << " " << iter->second << endl;
    }


    cout << "-----------"<<endl;
    iter2 =simap.find("jerry");
    simap.erase(simap.begin(),iter2);
    iter = simap.begin();
    for(;iter!=simap.end();++iter){
        cout << iter->first << " " << iter->second << endl;
    }


}


void HashSetTest()
{
    MySTL::hash_set<int> ihs;
    ihs.insert(59);
    ihs.insert(63);
    ihs.insert(108);
    ihs.insert(2);
    ihs.insert(53);
    ihs.insert(55);
    ihs.insert(2);
    ihs.insert(59);
    ihs.insert(59);

    MySTL::hash_set<int>::iterator ite = ihs.begin();
    for(;ite!=ihs.end();++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;

    for(int i=0; i<=100; ++i)
        ihs.insert(i);
    cout << ihs.size()<<endl;
    cout << ihs.bucket_count()<<endl;
    ite = ihs.begin();
    for(;ite!=ihs.end();++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;

    ihs.erase(2);
    ite = ihs.begin();
    for(;ite!=ihs.end();++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;

    ihs.erase(ihs.begin());
    ite = ihs.begin();
    for(;ite!=ihs.end();++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;

    ite = ihs.find(10);
    if(ite != ihs.end())
        cout << "Found: " << *ite<<endl;
    else
        cout << "Not Found." << endl;

}


void HashtableTest()
{
    MySTL::hashtable<int,int,hash<int>, MySTL::identity<int>,equal_to<int> > iht(50, hash<int>(), equal_to<int>());

    cout << iht.size()<<endl;
    cout << iht.bucket_count()<<endl;
    cout << iht.max_bucket_count()<<endl;

    iht.insert_unique(59);
    iht.insert_unique(63);
    iht.insert_unique(108);
    iht.insert_unique(2);
    iht.insert_unique(53);
    iht.insert_unique(55);
    cout << iht.size()<<endl;

    MySTL::hashtable<int,int,hash<int>, MySTL::identity<int>,equal_to<int> >::iterator ite = iht.begin();

    while(ite != iht.end()){
        cout << *ite << " ";
        ++ite;
    }
    cout << endl;

    for(int i=0; i<iht.bucket_count();++i)
    {
        int n=iht.elems_in_buckets(i);
        if(n!=0)
            cout << "bucket[" << i << "] has " << n << " elems." << endl;
    }
    //表格重建验证
    for(int i=0; i<=47; ++i)
        iht.insert_equal(i);
    cout << iht.size()<<endl;
    cout << iht.bucket_count()<<endl;

    ite = iht.begin();

    while(ite != iht.end()){
        cout << *ite << " ";
        ++ite;
    }
    cout << endl;

    for(int i=0; i<iht.bucket_count();++i)
    {
        int n=iht.elems_in_buckets(i);
        if(n!=0)
            cout << "bucket[" << i << "] has " << n << " elems." << endl;
    }

    ite = iht.find(2);
    if(ite != iht.end())
        cout << "Found: " << *ite<<endl;
    else
        cout << "Not Found." << endl;

    ite = iht.find(1000);
    if(ite != iht.end())
        cout << "Found: " << *ite<<endl;
    else
        cout << "Not Found." << endl;

     cout << iht.count(2) << endl;
     cout << iht.count(100) << endl;

     iht.erase(iht.begin());
     ite = iht.begin();

    while(ite != iht.end()){
        cout << *ite << " ";
        ++ite;
    }
    cout << endl;

    iht.erase(iht.begin(), iht.find(2));
     ite = iht.begin();

    while(ite != iht.end()){
        cout << *ite << " ";
        ++ite;
    }
    cout << endl;

    iht.erase(2);
     ite = iht.begin();

    while(ite != iht.end()){
        cout << *ite << " ";
        ++ite;
    }
    cout << endl;


}
void MapTest()
{
    MySTL::map<string, int> simap;
    simap[string("jjhou")] = 1;
    simap[string("jerry")] = 2;
    simap[string("jason")] = 3;
    simap[string("jimmy")] = 4;

    MySTL::pair<string,int> value(string("david"),5);
    simap.insert(value);

    MySTL::map<string,int>::iterator  iter = simap.begin();
    for(;iter!=simap.end();++iter){
        cout << iter->first << " " << iter->second << endl;
    }


    int number = simap[string("jjhou")];
    cout << number << endl;

    MySTL::map<string,int>::iterator  iter2;

    iter2 = simap.find(string("mchen"));
    if(iter2 == simap.end())
        cout << "Not Found" << endl;

    iter2 = simap.find(string("jerry"));
    if(iter2 != simap.end())
        cout << "Found" << endl;

    iter2->second = 9;
    int number2 = simap[string("jerry")];
    cout << number2 << endl;


    simap[string("aa")] = 3;
    simap[string("bb")] = 3;
    simap[string("cc")] = 3;
    iter = simap.begin();
    for(;iter!=simap.end();++iter){
        cout << iter->first << " " << iter->second << endl;
    }
    simap.erase("aa");
    cout << "-----------"<<endl;
    iter = simap.begin();
    for(;iter!=simap.end();++iter){
        cout << iter->first << " " << iter->second << endl;
    }


    cout << "-----------"<<endl;
    iter2 =simap.find("jerry");
    simap.erase(simap.begin(),iter2);
    iter = simap.begin();
    for(;iter!=simap.end();++iter){
        cout << iter->first << " " << iter->second << endl;
    }

}
void SetTest()
{
    int ia[5] = {0,1,2,3,4};
    MySTL::set<int> iset(ia,ia+5);

    cout << "size=" << iset.size()<<endl;
    cout << "3 count = " << iset.count(3)<<endl;

    iset.insert(3);
    cout << "size=" << iset.size()<<endl;
    cout << "3 count = " << iset.count(3)<<endl;

    iset.insert(5);
    cout << "size=" << iset.size()<<endl;
    cout << "3 count = " << iset.count(3)<<endl;

    iset.erase(1);
    cout << "size=" << iset.size()<<endl;
    cout << "3 count = " << iset.count(3)<<endl;
    cout << "1 count = " << iset.count(1)<<endl;

    MySTL::set<int>::iterator  ite1 = iset.begin();
    MySTL::set<int>::iterator  ite2 = iset.end();

    for(;ite1 != ite2; ++ite1)
        cout << *ite1 << " ";
    cout << endl;

    ite1 = iset.find(3);
    if(ite1 != iset.end())
        cout << "found:" << *ite1<<endl;

     ite1 = iset.find(1);
    if(ite1 == iset.end())
        cout << "Not found" <<endl;


    ite1 = iset.find(3);

    //*ite1 = 9; //会出错是对的，因为set的iterator是const的，不能修改值
}
void RBtreeTest()
{
    MySTL::rb_tree<int,int,MySTL::identity<int>,MySTL::less<int> > itree;
    MySTL::rb_tree<int,int,MySTL::identity<int>,MySTL::less<int> >::iterator ite1,ite2;
    cout << "size=" <<  itree.size()<<endl;

    itree.insert_unique(10);
    itree.insert_unique(7);

    itree.insert_unique(8);//cout << "-----"<<endl;

    itree.insert_unique(15);
    itree.insert_unique(5);
    itree.insert_unique(6);
    itree.insert_unique(11);

    itree.insert_unique(13);

    itree.insert_unique(12);

    cout << "size=" << itree.size()<<endl;

    for(ite1 = itree.begin(); ite1!=itree.end();++ite1 )
    {
        cout << *ite1 << " ";
    }
    cout << endl;

    MySTL::__rb_tree_base_iterator tmp;

    for(ite1 = itree.begin(); ite1!=itree.end();++ite1 )
    {
        tmp = MySTL::__rb_tree_base_iterator(ite1);
        cout << *ite1 << "(" << tmp.node->color << ") ";
    }
    cout << endl;


    if(itree.find(10) != itree.end())
        cout << "Find"<<endl;
    else
        cout << "Not Find" << endl;

    if(itree.find(100)!= itree.end())
        cout << "Find"<<endl;
    else
        cout << "Not Find" << endl;

    ite1 = itree.begin();
    itree.erase(ite1);

    ite1 = itree.begin();
    itree.erase(ite1);
    ite1 = itree.begin();
    itree.erase(ite1);

    for(ite1 = itree.begin(); ite1!=itree.end();++ite1 )
    {
        tmp = MySTL::__rb_tree_base_iterator(ite1);
        cout << *ite1 << "(" << tmp.node->color << ") ";
    }
    cout << endl;

    ite1 = itree.end();
    ite1--;
    cout << "--test: "<<*ite1<<endl;
    itree.erase(ite1);

    for(ite1 = itree.begin(); ite1!=itree.end();++ite1 )
    {
        tmp = MySTL::__rb_tree_base_iterator(ite1);
        cout << *ite1 << "(" << tmp.node->color << ") ";
    }
    cout << endl;

    ite1 = itree.begin();
    ite2 = ite1;
    ++(++(++(ite2)));
    cout << "--test: "<<*ite1 << " : "<< *ite2 <<endl;

    itree.erase(ite1,ite2);

    for(ite1 = itree.begin(); ite1!=itree.end();++ite1 )
    {
        tmp = MySTL::__rb_tree_base_iterator(ite1);
        cout << *ite1 << "(" << tmp.node->color << ") ";
    }
    cout << endl;

     itree.erase(13);

    for(ite1 = itree.begin(); ite1!=itree.end();++ite1 )
    {
        tmp = MySTL::__rb_tree_base_iterator(ite1);
        cout << *ite1 << "(" << tmp.node->color << ") ";
    }
    cout << endl;


    itree.clear();
    cout << "size=" << itree.size()<<endl;
    for(ite1 = itree.begin(); ite1!=itree.end();++ite1 )
    {
        tmp = MySTL::__rb_tree_base_iterator(ite1);
        cout << *ite1 << "(" << tmp.node->color << ") ";
    }
    cout << endl;


    cout <<"-----------"<<endl;
    itree.insert_unique(10);
    itree.insert_unique(7);

    itree.insert_unique(8);//cout << "-----"<<endl;

    itree.insert_unique(15);
    itree.insert_unique(5);
    itree.insert_unique(6);
    itree.insert_unique(11);
    itree.insert_unique(11);
    itree.insert_unique(13);
    itree.insert_unique(13);
    itree.insert_unique(12);
    for(ite1 = itree.begin(); ite1!=itree.end();++ite1 )
    {
        tmp = MySTL::__rb_tree_base_iterator(ite1);
        cout << *ite1 << "(" << tmp.node->color << ") ";
    }
    cout << endl;

    itree.insert_equal(13);
    itree.insert_equal(12);
    for(ite1 = itree.begin(); ite1!=itree.end();++ite1 )
    {
        tmp = MySTL::__rb_tree_base_iterator(ite1);
        cout << *ite1 << "(" << tmp.node->color << ") ";
    }
    cout << endl;




}

void SlistTest()
{
    //int i;
    MySTL::slist<int> islist;
    cout <<"_________"<<endl;
    cout << "size=" <<islist.size()<<endl;

    islist.push_front(9);
    islist.push_front(1);
    islist.push_front(2);
    islist.push_front(3);
    islist.push_front(4);
    cout << "size=" <<islist.size()<<endl;

    MySTL::slist<int>::iterator ite = islist.begin();
    MySTL::slist<int>::iterator ite2 = islist.end();
    for(;ite!=ite2; ++ite){
        cout << *ite<<" ";
    }
    cout << endl;






}

void PQueue_Test()
{
    int ia[9] = {0,1,2,3,4,8,9,3,5};
    MySTL::priority_queue<int> ivec(ia, ia+9);
    cout << "size=" << ivec.size()<<endl;

    for(MySTL::priority_queue<int>::size_type i=0;i<ivec.size();i++)
    {
        cout << ivec.top()<<" ";
    }
    cout << endl;

    while(!ivec.empty())
    {
        cout << ivec.top()<<' ';
        ivec.pop();
    }
    cout << endl;

    ivec.push(0);
    ivec.push(5);
    ivec.push(1);
    ivec.push(7);
    ivec.push(2);
    ivec.push(4);
    ivec.push(3);
    ivec.push(6);
    while(!ivec.empty())
    {
        cout << ivec.top()<<' ';
        ivec.pop();
    }
    cout << endl;




}

void HeapTest()
{
    int ia[9] = {0,1,2,3,4,8,9,3,5};
    MySTL::vector<int> ivec(ia, ia+9);
    for(MySTL::vector<int>::size_type i=0; i<ivec.size(); i++)
    {
        cout << ivec[i]<<" ";
    }
    cout << endl;
    make_heap(ivec.begin(), ivec.end());
    for(MySTL::vector<int>::size_type i=0; i<ivec.size(); i++)
    {
        cout << ivec[i]<<" ";
    }
    cout << endl;

    ivec.push_back(7);
    push_heap(ivec.begin(), ivec.end());
    for(MySTL::vector<int>::size_type i=0; i<ivec.size(); i++)
    {
        cout << ivec[i]<<" ";
    }
    cout << endl;

    pop_heap(ivec.begin(),ivec.end());
    cout << ivec.back()<<endl;
    ivec.pop_back();
    for(MySTL::vector<int>::size_type i=0; i<ivec.size(); i++)
    {
        cout << ivec[i]<<" ";
    }
    cout << endl;

    sort_heap(ivec.begin(), ivec.end());
    for(MySTL::vector<int>::size_type i=0; i<ivec.size(); i++)
    {
        cout << ivec[i]<<" ";
    }
    cout << endl;
}
void QueueTest()
{
    MySTL::queue<int> iqueue;
    cout << "size=" << iqueue.size()<<endl;
    cout << "empty:" << iqueue.empty()<<endl;

    iqueue.push(1);
    iqueue.push(3);
    iqueue.push(5);
    iqueue.push(7);

    cout << "size=" << iqueue.size()<<endl;
    cout << "empty:" << iqueue.empty()<<endl;

    cout << iqueue.front()<<endl;
    cout << iqueue.back()<<endl;

    iqueue.pop(); cout << iqueue.front()<<endl;
    iqueue.pop(); cout << iqueue.front()<<endl;
    iqueue.pop(); cout << iqueue.front()<<endl;
    cout << "size=" << iqueue.size()<<endl;


    MySTL::queue<int, MySTL::list<int> > iqueue2;
    cout << "size=" << iqueue2.size()<<endl;
    cout << "empty:" << iqueue2.empty()<<endl;

    iqueue2.push(1);
    iqueue2.push(3);
    iqueue2.push(5);
    iqueue2.push(7);

    cout << "size=" << iqueue2.size()<<endl;
    cout << "empty:" << iqueue2.empty()<<endl;

    cout << iqueue2.front()<<endl;
    cout << iqueue2.back()<<endl;

    iqueue2.pop(); cout << iqueue2.front()<<endl;
    iqueue2.pop(); cout << iqueue2.front()<<endl;
    iqueue2.pop(); cout << iqueue2.front()<<endl;
    cout << "size=" << iqueue2.size()<<endl;


    MySTL::queue<int, MySTL::vector<int> > iqueue3;
    cout << "size=" << iqueue3.size()<<endl;
    cout << "empty:" << iqueue3.empty()<<endl;

    iqueue3.push(1);
    iqueue3.push(3);
    iqueue3.push(5);
    iqueue3.push(7);

    cout << "size=" << iqueue3.size()<<endl;
    cout << "empty:" << iqueue3.empty()<<endl;

    cout << iqueue3.front()<<endl;
    cout << iqueue3.back()<<endl;

    iqueue3.pop(); cout << iqueue3.front()<<endl;
    iqueue3.pop(); cout << iqueue3.front()<<endl;
    iqueue3.pop(); cout << iqueue3.front()<<endl;
    cout << "size=" << iqueue3.size()<<endl;

    MySTL::queue<int, MySTL::deque<int> > iqueue4;
    MySTL::queue<int, MySTL::list<int> > iqueue5;
    MySTL::queue<int, MySTL::vector<int> > iqueue6;

    cout << (iqueue == iqueue4) <<endl;
    iqueue4.push(7);
    cout << (iqueue == iqueue4) <<endl;
    cout << (iqueue2 == iqueue5) <<endl;
    cout << (iqueue3 == iqueue6) << endl;
}
void StackTest()
{
    MySTL::stack<int> istack;
    cout << "size=" << istack.size()<<endl;
    istack.push(1);
    istack.push(3);
    istack.push(5);
    istack.push(7);
    cout << "size=" << istack.size()<<endl;
    cout << istack.top()<<endl;
    istack.pop();cout << istack.top()<<endl;
    istack.pop();cout << istack.top()<<endl;
    istack.pop();cout << istack.top()<<endl;
    cout << "size=" << istack.size()<<endl;


    MySTL::stack<int,MySTL::list<int>> istack2;
    cout << endl<<"size=" << istack2.size()<<endl;
    istack2.push(1);
    istack2.push(3);
    istack2.push(5);
    istack2.push(7);
    cout << "size=" << istack2.size()<<endl;
    cout << istack2.top()<<endl;
    istack2.pop();cout << istack2.top()<<endl;
    istack2.pop();cout << istack2.top()<<endl;
    istack2.pop();cout << istack2.top()<<endl;
    cout << "size=" << istack2.size()<<endl;

    MySTL::stack<int,MySTL::vector<int>> istack3;
    cout << endl<<"size=" << istack3.size()<<endl;
    istack3.push(1);
    istack3.push(3);
    istack3.push(5);
    istack3.push(7);
    cout << "size=" << istack3.size()<<endl;
    cout << istack3.top()<<endl;
    istack3.pop();cout << istack3.top()<<endl;
    istack3.pop();cout << istack3.top()<<endl;
    istack3.pop();cout << istack3.top()<<endl;
    cout << "size=" << istack3.size()<<endl;

    MySTL::stack<int,MySTL::list<int>> istack4;
    cout << (istack2 == istack4) << endl;

    MySTL::stack<int,MySTL::vector<int>> istack5;
    cout << (istack3 == istack5) << endl;

    MySTL::stack<int> istack6;
    istack6.push(1);
    cout << (istack == istack6) << endl;
}
void DequeTest()
{
    MySTL::deque<int> ideq(20,9);
    cout << "size=" <<ideq.size()<<endl;
    for(MySTL::deque<int>::size_type i=1;i<ideq.size();++i)
        cout << ideq[i]<<" ";
    cout << endl;


    for(MySTL::deque<int>::size_type i=0;i<ideq.size();++i)
        ideq[i] = i;

    MySTL::deque<int>::iterator tmp;
    for(tmp=ideq.begin();tmp!=ideq.end();++tmp)
        cout << *tmp << " ";
    cout << endl;

    cout << "front:" << ideq.front()<<endl;
    cout << "back:" << ideq.back()<<endl;
    //cout << "MaxSize:" << ideq.max_size()<<endl;
    cout << "empty:" << ideq.empty()<<endl;

    MySTL::deque<int> ideq2;
    for(MySTL::deque<int>::size_type i=0;i<ideq2.size();++i)
        cout << ideq2[i]<<" ";
    cout << endl;
    cout << "------------x1(x)---------------"<<endl;
    MySTL::deque<int> ideq3(ideq);
    for(MySTL::deque<int>::size_type i=0;i<ideq3.size();++i)
        cout << ideq3[i]<<" ";
    cout << endl;

    cout << "-----------push--------------"<<endl;

    for(int i=0;i<512*8;i++){
        ideq3.push_back(i);
    }
    for(tmp=ideq3.begin();tmp!=ideq3.end();++tmp)
        cout << *tmp << " ";
    cout << endl;

    tmp = ideq3.insert(ideq3.begin()+500,1936);
    cout << *tmp << endl;
    for(MySTL::deque<int>::size_type i=0;i<ideq3.size();++i)
        cout << ideq3[i]<<" ";
    cout << endl;

    ideq.push_front(1000);
    for(MySTL::deque<int>::size_type i=0;i<ideq.size();++i)
        cout << ideq[i]<<" ";
    cout << endl;
    cout << "-----------pop--------------"<<endl;
    for(int i=0;i<512*8;i++){
        ideq3.pop_back();
    }
    for(MySTL::deque<int>::size_type i=0;i<ideq3.size();++i)
        cout << ideq3[i]<<" ";
    cout << endl;

    ideq.pop_front();
    ideq.pop_front();
    for(MySTL::deque<int>::size_type i=0;i<ideq.size();++i)
        cout << ideq[i]<<" ";
    cout << endl;

    cout << "----------clear,erase,insert----------"<<endl;
    ideq.clear();
    for(MySTL::deque<int>::size_type i=0;i<ideq.size();++i)
        cout << ideq[i]<<" ";
    cout << endl;

    cout << "not erase: ";
    for(MySTL::deque<int>::size_type i=0;i<ideq3.size();++i)
        cout << ideq3[i]<<" ";
    cout << endl;

    tmp = ideq3.begin()+10;
    ideq3.erase(tmp);
    for(MySTL::deque<int>::size_type i=0;i<ideq3.size();++i)
        cout << ideq3[i]<<" ";
    cout << endl;

    tmp = ideq3.begin()+3;
    ideq3.erase(tmp, tmp+4);
    for(MySTL::deque<int>::size_type i=0;i<ideq3.size();++i)
        cout << ideq3[i]<<" ";
    cout << endl;

    tmp = ideq3.insert(ideq3.begin(),1934);
    cout << *tmp << endl;
    for(MySTL::deque<int>::size_type i=0;i<ideq3.size();++i)
        cout << ideq3[i]<<" ";
    cout << endl;

    tmp = ideq3.insert(ideq3.end(),1935);
    cout << *tmp << endl;
    for(MySTL::deque<int>::size_type i=0;i<ideq3.size();++i)
        cout << ideq3[i]<<" ";
    cout << endl;

    tmp = ideq3.insert(ideq3.begin()+3,1936);
    cout << *tmp << endl;
    for(MySTL::deque<int>::size_type i=0;i<ideq3.size();++i)
        cout << ideq3[i]<<" ";
    cout << endl;

    tmp = ideq3.insert(ideq3.begin()+10,1936);
    cout << *tmp << endl;
    for(MySTL::deque<int>::size_type i=0;i<ideq3.size();++i)
        cout << ideq3[i]<<" ";
    cout << endl;


}

void ListTest()
{
    MySTL::list<int> ilist;
    cout << "size=" << ilist.size()<<endl;
    cout << "empty:"<<ilist.empty()<<endl;
    ilist.push_back(0);
    ilist.push_back(1);
    ilist.push_back(2);
    ilist.push_back(3);
    ilist.push_back(4);
    cout << "size=" << ilist.size()<<endl;

    MySTL::list<int>::iterator ite;
    for(ite = ilist.begin(); ite!=ilist.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;

    ite = ilist.begin();
    ++ite;
    ++ite;
    ++ite;
    ilist.insert(ite, 99);
    cout << "size=" << ilist.size() << endl;
    cout << *ite <<endl;
    for(ite = ilist.begin(); ite!=ilist.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;


    ite = ilist.begin();
    ++ite;
    cout << *(ilist.erase(ite))<<endl;;
    cout << "size=" << ilist.size() << endl;
    for(ite = ilist.begin(); ite!=ilist.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;

    cout <<"empty?:"<< ilist.empty()<<endl;
    cout <<"front:"<<ilist.front()<<endl;
    cout << "back:"<<ilist.back()<<endl;

    ite = ilist.begin();
    ++ite;
    ++ite;
    ++ite;
    ilist.insert(ite,5, 33);
    cout << "size=" << ilist.size() << endl;
    cout << *ite <<endl;
    for(ite = ilist.begin(); ite!=ilist.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;

    ilist.push_back(100);
    ilist.push_front(101);
    for(ite = ilist.begin(); ite!=ilist.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;

    ite = ilist.begin();
    ++ite;
    MySTL::list<int>::iterator ite2;
    ite2 = ite;
    ++ite2;
    ++ite2;
    cout << *(ilist.erase(ite,ite2))<<endl;;
    cout << "size=" << ilist.size() << endl;
    for(ite = ilist.begin(); ite!=ilist.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;

    ilist.pop_back();
    for(ite = ilist.begin(); ite!=ilist.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;

    ilist.pop_front();
    for(ite = ilist.begin(); ite!=ilist.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;

    ilist.resize(20,66);
    for(ite = ilist.begin(); ite!=ilist.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;
    ilist.resize(10);
    for(ite = ilist.begin(); ite!=ilist.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;

    ilist.remove(33);
    for(ite = ilist.begin(); ite!=ilist.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;

    ilist.unique();
    for(ite = ilist.begin(); ite!=ilist.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;

    ilist.reverse();
    for(ite = ilist.begin(); ite!=ilist.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;

    cout << "-----------swap-----------"<<endl;
    MySTL::list<int> ilist2;
    ilist2.insert(ilist2.begin(), 10,8);
    for(ite = ilist2.begin(); ite!=ilist2.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;
    ilist.swap(ilist2);
    for(ite = ilist.begin(); ite!=ilist.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;
    for(ite = ilist2.begin(); ite!=ilist2.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;
    cout <<"-------------splice--------------"<<endl;


    ilist.splice(ilist.begin(), ilist2, ilist2.begin());
    for(ite = ilist.begin(); ite!=ilist.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;
    for(ite = ilist2.begin(); ite!=ilist2.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;

    cout << "-------------==/!=---------------"<<endl;
    if(ilist2 == ilist)
        cout << "equel" << endl;
    else
        cout << "unequel"<<endl;
    MySTL::list<int> ilist3;
    ilist3.push_back(4);
    ilist3.push_back(3);
    ilist3.push_back(99);
    if(ilist2 == ilist3)
        cout << "equel" << endl;
    else
        cout << "unequel"<<endl;
    if(ilist2 != ilist3)
        cout << "unequel" << endl;
    else
        cout << "equel"<<endl;

    cout << "---------merge------------"<<endl;

    ilist.merge(ilist2);
    for(ite = ilist.begin(); ite!=ilist.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;

    cout << "-------------sort-----------"<<endl;
    ilist.sort();
    for(ite = ilist.begin(); ite!=ilist.end(); ++ite)
    {
        cout << *ite << " ";
    }
    cout << endl;

}

void VectorTest(){
    MySTL::vector<string> v1(10, "abc");
    for(int i=0;i<10;i++)
        cout << v1[i] << endl;
    long m=10;
    MySTL::vector<string> v2(m);
    for(int i=0;i<10;i++)
        cout << v2[i] << endl;

    MySTL::vector<string> v3(v1);
    for(int i=0;i<=10;i++)
        cout << v3[i] << endl;

    v2=v3;
    for(int i=0;i<=10;i++)
        cout << v2[i] << endl;

    v2[3] = "hello";
    for(int i=0;i<=10;i++)
        cout << v2[i] << endl;

    if(v2==v3)
        cout << "True"<<endl;
    else
        cout <<"False"<<endl;
    if(v1 == v3)
        cout <<"True"<<endl;
    else
        cout <<"False"<<endl;

    if(v2!=v3)
        cout << "True"<<endl;
    else
        cout <<"False"<<endl;
    if(v1!= v3)
        cout <<"True"<<endl;
    else
        cout <<"False"<<endl;

    v1[0] = "beginssss";
    v1[9] = "endkkkkk";
    cout <<"begin:"<< *(v1.begin())<<endl;
    cout <<"end:"<< *(v1.end()-1)<<endl;
    cout <<"front:"<<v1.front()<<endl;
    cout <<"end:"<<v1.back()<<endl;

    cout << v1.size()<<endl;
    cout << v1.empty()<<endl;

    v1.resize(20,"add");
    for(MySTL::vector<string>::size_type i=0;i<v1.size();i++)
        cout << v1[i] << endl;
    v1.resize(12,"add");
    for(MySTL::vector<string>::size_type i=0;i<v1.size();i++)
        cout << v1[i] << endl;
    v1.resize(26);
    for(MySTL::vector<string>::size_type i=0;i<v1.size();i++)
        cout << v1[i] << endl;
    v1.resize(27,"add");
    v1.insert(v1.begin(),3,"insert");
    for(MySTL::vector<string>::size_type i=0;i<v1.size();i++)
        cout << v1[i] << endl;
    cout << v1.capacity()<<endl;
    v1.insert(v1.begin()+7,3,"insert");
    for(MySTL::vector<string>::size_type i=0;i<v1.size();i++)
        cout << v1[i] << endl;
    cout << "----------push_back----------"<<endl;
    cout << v1.size()<<endl;
    v1.push_back("push_back");
    for(MySTL::vector<string>::size_type i=0;i<v1.size();i++)
        cout << v1[i] << endl;
    cout << v1.size()<<endl;
    cout << v1.capacity()<<endl;
    v1.pop_back();
    for(MySTL::vector<string>::size_type i=0;i<v1.size();i++)
        cout << v1[i] << endl;
    cout << v1.size()<<endl;
    cout << v1.capacity()<<endl;
    cout << "----------reserve&push_back----------"<<endl;
    v1.reserve(1000);
    for(MySTL::vector<string>::size_type i=0;i<v1.size();i++)
        cout << v1[i] << endl;
    cout << v1.capacity()<<endl;
    cout << v1.size()<<endl;

    v1.clear();
    for(MySTL::vector<string>::size_type i=0;i<v1.size();i++)
        cout << v1[i] << endl;
    cout << v1.capacity()<<endl;
    cout << v1.size()<<endl;
    cout << "-----------swap--------------"<<endl;
    v1.swap(v3);
    for(MySTL::vector<string>::size_type i=0;i<v1.size();i++)
        cout << v1[i] << endl;
    cout << v1.capacity()<<endl;
    cout << v1.size()<<endl;
    for(MySTL::vector<string>::size_type i=0;i<v3.size();i++)
        cout << v3[i] << endl;
    cout << v3.capacity()<<endl;
    cout << v3.size()<<endl;

    cout << "----------------------"<<endl;
    v3.push_back("0");
    v3.push_back("1");
    v3.push_back("2");
    v3.push_back("3");
    v3.push_back("4");
    v3.push_back("5");
    v3.push_back("6");
    v3.push_back("7");
    v3.push_back("8");
    v3.push_back("9");
    for(MySTL::vector<string>::size_type i=0;i<v3.size();i++)
        cout << v3[i] << "  ";
    cout << endl;
    v3.erase(v3.begin()+7);
    for(MySTL::vector<string>::size_type i=0;i<v3.size();i++)
        cout << v3[i] << "  ";
    cout << endl;
    v3.erase(v3.begin()+2,v3.begin()+5);
    for(MySTL::vector<string>::size_type i=0;i<v3.size();i++)
        cout << v3[i] << "  ";
    cout << endl;

}
