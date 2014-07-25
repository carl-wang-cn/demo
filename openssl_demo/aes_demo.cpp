#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/aes.h>

#define AES_BITS 128
#define MSG_LEN 128

int aes_encrypt(char* in, char* key, char* out)//, int olen)可能会设置buf长度
{
    if(!in || !key || !out)
    {
        return 0;
    }

    unsigned char iv[AES_BLOCK_SIZE] = {};//加密的初始化向量
    for(int i=0; i<AES_BLOCK_SIZE; ++i)//iv一般设置为全0,可以设置其他，但是加密解密要一样就行
    {
        iv[i]=0;
    }

    AES_KEY aes;
    if(AES_set_encrypt_key((unsigned char*)key, 128, &aes) < 0)
    {
        return 0;
    }

    //这里的长度是char*in的长度，但是如果in中间包含'\0'字符的话
    //那么就只会加密前面'\0'前面的一段，所以，这个len可以作为参数传进来，记录in的长度
    int len=strlen(in);

    //至于解密也是一个道理，光以'\0'来判断字符串长度，确有不妥，后面都是一个道理。
    AES_cbc_encrypt((unsigned char*)in, (unsigned char*)out, len, &aes, iv, AES_ENCRYPT);
    return 1;
}

int aes_decrypt(char* in, char* key, char* out)
{
    if(!in || !key || !out)
    {
        return 0;
    }

    unsigned char iv[AES_BLOCK_SIZE] = {};//加密的初始化向量
    for(int i=0; i<AES_BLOCK_SIZE; ++i)//iv一般设置为全0,可以设置其他，但是加密解密要一样就行
    {
        iv[i]=0;
    }
    
    AES_KEY aes;
    if(AES_set_decrypt_key((unsigned char*)key, 128, &aes) < 0)
    {
        return 0;
    }

    int len=strlen(in);
    AES_cbc_encrypt((unsigned char*)in, (unsigned char*)out, len, &aes, iv, AES_DECRYPT);

    return 1;
}

int main(int argc,char *argv[])
{
    char clearString[MSG_LEN] = {};
    char encryptString[MSG_LEN] = {};
    char decryptString[MSG_LEN] = {};
    char key[AES_BLOCK_SIZE] = {};
    int i = 0;

    strcpy((char*)clearString, "123456789 123456789 123456789 12a");

    for(i = 0; i < 16; i++)//可自由设置密钥
    {
        key[i] = 32 + i;
    }

    if(!aes_encrypt(clearString,key,encryptString))
    {
        printf("encrypt error\n");
        return -1;
    }

    if(!aes_decrypt(encryptString,key,decryptString))
    {
        printf("decrypt error\n");
        return -1;
    }

    if (memcmp(clearString, decryptString, MSG_LEN) == 0)
    {
        printf("ok\n");
    }
    else
    {
        printf("error\n");
    }

    return 0;
}
