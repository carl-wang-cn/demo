/*
 * 该示例实现两个功能
 * 1. 测试系统多多少位
 * 2. 测试系统默认字节序时大端还是小端
 */

#include <iostream>

using namespace std;

int main()
{

    // 判断当前系统是多少位
    char *a = NULL; 

    if (4 == sizeof(void *))
    {
        cout << "os is 32 bit" <<endl;
    }
    else if (8 == sizeof(void *))
    {
        cout << "os is 64 bit" <<endl;
    }


    // 判断当前系统是大端还是小端
    union ut
    {
        short s;
        char c[2];
    } u;

    if (sizeof(short) == 2)
    {
        u.s = 0x0102;

        if (u.c[0] == 0x01 && u.c[1] == 0x02)
        {
            cout << "big endian" <<endl;
        }
        else if (u.c[0] == 0x02 && u.c[1] == 0x01)
        {
            cout << "small endian" <<endl;
        }
    }

    return 0;
}
