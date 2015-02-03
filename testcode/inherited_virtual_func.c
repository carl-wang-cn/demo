/*
 * 1. 测试两个具有继承关系的Class在通过基类的指针和引用来调用虚函数的表现
 * 2. 测试默认参数传递的表现
 *
 * 注意观察使用默认参数和传递实参的时候程序分别如何表现
 *
 * */

#include <iostream>

using namespace std;

class Base
{
public:
    virtual void Fun(int number = 10)
    {
        std::cout << "Base::Fun with number " << number <<endl;
    }
};

class Derive: public Base 
{
public: 
    virtual void Fun(int number = 20)
    {
        std::cout << "Derive::Fun with number " << number
            <<endl;
    }
}; 

int main() 
{
    Derive b; 
    Base &a = b; 
    Base *c = &b;

    a.Fun();
    a.Fun(30);

    b.Fun();

    c->Fun();
    c->Fun(40);

    return 0;
}
