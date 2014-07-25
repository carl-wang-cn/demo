#include <stdio.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
//16进制打印函数
void print_hex(char* buff, int len=0)
{
    if(len>0)
    {
        for (int i=0;i<len;i++)
        {
            printf("%02x",(unsigned char)buff[i]);
        }
    }
    else
    {
        for (int i=0;buff[i];i++)
        {
            printf("%02x",(unsigned char)buff[i]);
        }
    }
    printf("\n");
}

int createRSA()
{
    int ret=1;
    RSA *pRSA;//下面生成首先生成私钥，这里私钥是包含公钥的
    pRSA = RSA_generate_key(1024, RSA_F4, NULL, NULL);//RSA_3
    if(!RSA_check_key(pRSA))
    {
        ret=0;
    }

    int size = i2d_RSAPublicKey(pRSA, NULL);//提取公钥大小
    if(!size)
    {
        ret = 0;
    }
    unsigned char* pubbuf=(unsigned char*)malloc(size+1);//生成公钥存储空间
    unsigned char *p = pubbuf;
    if(pubbuf==NULL)
    {
        ret=0;
    }
    size = i2d_RSAPublicKey(pRSA, &p);//必须用p，不能直接&pubbuf，因为参数会改变
    if(!size)
    {
        ret = 0;
    }

    printf("PriKeyBuff, Len=%d\n", size);
    //print_hex((char*)pubbuf1,size);

    RSA* pubkey;//从buf中，生成对应的公钥
    p = pubbuf;
    //这里同样需要用p替代pubbuf，因为如果直接用pubbuf，那么pubbuf的值将会被改变
    //而我们将无法得知 之前动态分配的内存地址，从而无法释放这片内存，导致内存泄露，所以要注意。

    pubkey=d2i_RSAPublicKey(NULL, (const unsigned char**)&p, size);

    //下面是公钥加密测试
    char in[10]="love";
    char out[128]={0};
    char in2[128]={0};
    RSA_public_encrypt(strlen(in),(unsigned char*)in,(unsigned char*)out,pubkey,RSA_PKCS1_PADDING);
    print_hex(out,128);
    //私钥解密
    RSA_private_decrypt(128,(unsigned char*)out,(unsigned char*)in2,pRSA,RSA_PKCS1_PADDING);
    printf("%s\n",in2);
    print_hex(in2);


    //下面打印公钥对应的信息，公钥包含N和e，而私钥拥N和d
    //实际上，私钥含有p、q等，所以可以推导出公钥
    printf("N(%d bytes):%s\n",BN_num_bytes(pubkey->n), BN_bn2hex(pubkey->n));
    printf("e(%d bytes):%s\n",BN_num_bytes(pubkey->e), BN_bn2hex(pubkey->e));

    // 打印下面这些信息，会出现段错误，因为公钥不存在那些信息
    //printf("p(%d bytes):%s\n",BN_num_bytes(pubkey->p), BN_bn2hex(pubkey->p));
    //printf("q(%d bytes):%s\n",BN_num_bytes(pubkey->q), BN_bn2hex(pubkey->q));
    //printf("d(%d bytes):%s\n",BN_num_bytes(pubkey->d), BN_bn2hex(pubkey->d));

    if(pRSA) RSA_free(pRSA);
    if(pubkey) RSA_free(pubkey);
    if(pubbuf) free(pubbuf);
    return ret;
}

int main(int argc,char**argv)
{
    if(!createRSA())
    {
        printf("error\n");
        return -1;
    }
    return 0;
}
