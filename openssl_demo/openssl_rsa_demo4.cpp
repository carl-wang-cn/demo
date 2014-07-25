#include <stdio.h>
#include<string.h>
#include <openssl/bio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#define MSG_LEN (128+1)

void print_hex(char* buff)
{
    for (int i=0;buff[i];i++)
        printf("%02x",(unsigned char)buff[i]);
    printf("\n");
}

int rsa_verify(char *in, char *key_path, char* in2, int len)
{
    RSA *p_rsa;
    FILE *file;
    if((file=fopen(key_path,"r"))==NULL)
    {
        perror("open key file error");
        return 0;
    }
    //if((p_rsa=PEM_read_RSA_PUBKEY(file,NULL,&ccbb,NULL))==NULL){
    if((p_rsa=PEM_read_RSAPublicKey(file,NULL,NULL,NULL))==NULL)
    {
        ERR_print_errors_fp(stdout);
        return 0;
    }
    if(!RSA_verify(NID_md5,(unsigned char*)in,strlen(in),(unsigned char*)in2,len,p_rsa))
    {
        return 0;
    }
    RSA_free(p_rsa);
    fclose(file);
    return 1;
}

int rsa_sign(char *in, char *key_path, char* out, int* plen)
{
    RSA *p_rsa;
    FILE *file;
    if((file=fopen(key_path,"r"))==NULL)
    {
        perror("open key file error");
        return 0;
    }
    if((p_rsa=PEM_read_RSAPrivateKey(file,NULL,NULL,NULL))==NULL)
    {
        ERR_print_errors_fp(stdout);
        return 0;
    }
    if(!RSA_sign(NID_md5,(unsigned char*)in,strlen(in),(unsigned char*)out,(unsigned int*)plen,p_rsa))
    {
        return 0;
    }
    RSA_free(p_rsa);
    fclose(file);
    return 1;
}

int main(int argc,char**argv)
{
    char text[MSG_LEN] = {};
    char sign[MSG_LEN] = {};
    int len=0;

    strcpy((char*)text, "123456789 123456789 123456789 12a");
    char pubkey[]="public.rsa";
    char prikey[]="private.rsa";

    if(!rsa_sign(text,prikey,sign,&len))
    {
        printf("sign error\n");
        return -1;
    }
    printf("sign %d:",strlen((char*)sign));
    print_hex(sign);

    if(!rsa_verify(text,pubkey,sign,len))
    {
        printf("verify error\n");
        return -1;
    }
    printf("verify ok\n");

    return 0;
}
