/*
 * 测试class中不同成员对sizeof的影响，尤其需要注意虚函数，指针，字节对齐几个方面
 * 可以通过对代码中相应部分进行注释，重新编译，来观察不同成员所产生的影响
 * 
 */

#include <iostream>

using namespace std;


class Base
{
public:
    Base();                
    virtual ~Base();         //每个实例都有虚函数表
    void set_num(int num)    //普通成员函数，为各实例公有，不归入sizeof统计
    {
        a=num;
    }
private:
    int  a;                  //占4字节
    char *p;                 //4字节指针(32位系统)
};

class Derive:public Base
{
public:
    Derive():Base(){};     
    ~Derive(){};
private:
    static int st;         //非实例独占
    int  d;                //占4字节
    char *p;               //4字节指针(32位系统)
    char c;

};

int main() 
{ 
    cout <<"sizeof(Base) is " <<sizeof(Base) <<" bytes" <<endl;
    cout <<"sizeof(Derive) is " <<sizeof(Derive) <<" bytes" <<endl;

    return 0;
}

