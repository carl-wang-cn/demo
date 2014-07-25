// 通过命令行openssl命令来生成公私密钥对

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<openssl/rsa.h>
#include<openssl/pem.h>
#include<openssl/err.h>

//before start run this app, run the cmd below
//openssl genrsa -out test.key 1024
//openssl rsa -in test.key -pubout -out test_pub.key
//openssl rsa -in test.key -RSAPublicKey_out -out test_pub.key  这个可以使用注释中说死活不成功那个运行成功

#define OPENSSLKEY "test.key"
#define PUBLICKEY "test_pub.key"

char* my_encrypt(char *str,char *path_key);//加密
char* my_decrypt(char *str,char *path_key);//解密

int main(void)
{
    char *source="i like dancing !";
    char *ptr_en = NULL;
    char *ptr_de = NULL;

    printf("source is    :%s\n",source);
    ptr_en = my_encrypt(source, PUBLICKEY);
    printf("after encrypt:%s\n", ptr_en);

    ptr_de = my_decrypt(ptr_en, OPENSSLKEY);
    printf("after decrypt:%s\n",ptr_de);
    if (ptr_en != NULL)
    {
        free(ptr_en);
        ptr_en = NULL;
    }   
    if (ptr_de != NULL)
    {
        free(ptr_de);
        ptr_de = NULL;
    }   
    return 0;
}
char *my_encrypt(char *str,char *path_key)
{
    char *p_en = NULL;
    RSA *p_rsa = NULL;
    FILE *file;
    int flen = 0;
    int rsa_len = 0;

    file = fopen(path_key, "r");
    if (NULL == file)
    {
        perror("open key file error");
        return NULL;    
    }   

    p_rsa = PEM_read_RSA_PUBKEY(file, NULL, NULL, NULL);
//  p_rsa=PEM_read_RSAPublicKey(file,NULL,NULL,NULL))==NULL)  // 换成这句死活通不过，无论是否将公钥分离源文件
    if (NULL == p_rsa)
    {
        ERR_print_errors_fp(stdout);
        return NULL;
    }   

    flen = strlen(str); // must < rsa_len-11
    rsa_len = RSA_size(p_rsa);
    p_en = ( char *)malloc(rsa_len+1);
    memset(p_en, 0, rsa_len+1);
    if (RSA_public_encrypt(flen, (unsigned char *)str, (unsigned char*)p_en, p_rsa,RSA_PKCS1_PADDING) < 0)
    {
        perror("pub encrypt error");
        return NULL;
    }
    RSA_free(p_rsa);
    fclose(file);

    return p_en;
}

char *my_decrypt(char *str, char *path_key)
{
    char *p_de = NULL;
    RSA *p_rsa = NULL;
    FILE *file;
    int rsa_len = 0;
    int flen = 0;

    file = fopen(path_key, "r");
    if(NULL == file)
    {
        perror("open key file error");
        return NULL;
    }

    p_rsa = PEM_read_RSAPrivateKey(file, NULL, NULL, NULL);
    if (NULL == p_rsa)
    {
        ERR_print_errors_fp(stdout);
        return NULL;
    }

    flen = strlen(str); // must < rsa_len-11
    rsa_len = RSA_size(p_rsa);
    p_de = (char *)malloc(rsa_len+1);
    memset(p_de,0,rsa_len+1);
    if(RSA_private_decrypt(flen, (unsigned char *)str, (unsigned char*)p_de, p_rsa, RSA_PKCS1_PADDING) < 0)
    {
        perror("pri decrypt error");
        return NULL;
    }
    RSA_free(p_rsa);
    fclose(file);
    return p_de;
}
