#include <stdio.h>
#include<string.h>
#include <openssl/bio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#define MSG_LEN (128+1)

//打印函数，以16进制显示字符内容
void print_hex(char* buff)
{
    for (int i=0;buff[i];i++)
    {
        printf("%02x",(unsigned char)buff[i]);
    }
    printf("\n");
}

//生成RSA公钥和私钥文件
int createRSAPEM()
{
    int ret=0;
    BIO *bpub, *bpri;
    bpub = BIO_new_file("public.rsa", "w");
    if (!bpub)
    {
        printf("failed to create public bio file\n");
    }

    bpri = BIO_new_file("private.rsa", "w");
    if (!bpri)
    {
        printf("failed to create private bio file\n");
    }

    if (!bpub || !bpri) 
    {
        goto EXIT;
    }

    RSA *pRSA;
    pRSA = RSA_generate_key( 1024, RSA_F4, NULL, NULL);
    if (pRSA != NULL)
    {
        if (!PEM_write_bio_RSAPublicKey(bpub, pRSA))//PEM_write_bio_RSA_PUBKEY
        {
            printf("PEM_write_bio_RSAPublicKey: failed\n");
            goto EXIT;
        }

        //if (!PEM_write_bio_RSAPrivateKey(bpri, pRSA, EVP_aes_256_cbc(), NULL, 0, NULL, NULL))//暂时设置密码
        if (!PEM_write_bio_RSAPrivateKey(bpri, pRSA, NULL, NULL, 0, NULL, NULL))
        {
            printf("PEM_write_bio_PrivateKey: failed\n");
            goto EXIT;
        }
        ret =1;
    }
EXIT:
    if (bpub)
    {
        BIO_free(bpub);
    }
    if (bpri)
    {
        BIO_free(bpri);
    }
    if (pRSA)
    { 
        RSA_free(pRSA);
    }

    return ret;
}

//使用公钥加密
int rsa_encrypt(char *in, char *key_path, char* out)
{
    RSA *p_rsa;
    FILE *file;
    int rsa_len;
    if((file=fopen(key_path,"r"))==NULL)
    {
        perror("open key file error");
        return 0;
    }

    //if((p_rsa=PEM_read_RSA_PUBKEY(file,NULL,&ccbb,NULL))==NULL){
    if((p_rsa=PEM_read_RSAPublicKey(file,NULL,NULL,NULL))==NULL){
        ERR_print_errors_fp(stdout);
        return 0;
    }
    rsa_len=RSA_size(p_rsa);
    if(RSA_public_encrypt(rsa_len,(unsigned char*)in,(unsigned char*)out,p_rsa,RSA_NO_PADDING)<0)
    {
        return 0;
    }
    RSA_free(p_rsa);
    fclose(file);
    return 1;
}

//使用私钥解密
int rsa_decrypt(char *in, char *key_path, char* out)
{
    RSA *p_rsa;
    FILE *file;
    int rsa_len;
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

    rsa_len=RSA_size(p_rsa);
    if(RSA_private_decrypt(rsa_len,(unsigned char*)in,(unsigned char*)out,p_rsa,RSA_NO_PADDING)<0)
    {
        return 0;
    }
    RSA_free(p_rsa);
    fclose(file);
    return 1;
}

int main(int argc,char**argv)
{
    char clearString[MSG_LEN] = {};
    char encryptString[MSG_LEN] = {};;
    char decryptString[MSG_LEN] = {}; 

    createRSAPEM();
    strcpy((char*)clearString, "123456789 123456789 123456789 12a");

    char pubkey[]="public.rsa";
    char prikey[]="private.rsa";

    if(!rsa_encrypt(clearString, pubkey, encryptString))
    {
        printf("encrypt error\n");
        return -1;
    }

    if(!rsa_decrypt(encryptString, prikey, decryptString))
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
