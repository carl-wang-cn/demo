#include <stdio.h>
#include<string.h>
#include <openssl/bio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

const int  MSG_LEN = 128+1;
const char *PUBKEY_FILE = "public.rsa";
const char *PRIKEY_FILE = "private.rsa";

void print_hex(char* buff)
{
    for (int i=0; buff[i]; i++)
        printf("%02x", (unsigned char)buff[i]); 
    printf("\n"); 
}

int rsa_verify(char *in, const char *key_path, char* in2, int len)
{
    RSA *p_rsa; 
    FILE *file; 
    if ((file=fopen(key_path, "r"))==NULL)
    {
        perror("open key file error"); 
        return 0; 
    }
    //if ((p_rsa=PEM_read_RSA_PUBKEY(file, NULL, &ccbb, NULL))==NULL){
    if ((p_rsa=PEM_read_RSAPublicKey(file, NULL, NULL, NULL))==NULL)
    {
        ERR_print_errors_fp(stdout); 
        return 0; 
    }
    if (!RSA_verify(NID_md5, (unsigned char*)in, strlen(in), (unsigned char*)in2, len, p_rsa))
    {
        return 0; 
    }
    RSA_free(p_rsa); 
    fclose(file); 
    return 1; 
}

int rsa_sign(char *in, const char *key_path, char* out, int* plen)
{
    RSA *p_rsa; 
    FILE *file; 
    if ((file=fopen(key_path, "r"))==NULL)
    {
        perror("open key file error"); 
        return 0; 
    }
    if ((p_rsa=PEM_read_RSAPrivateKey(file, NULL, NULL, NULL))==NULL)
    {
        ERR_print_errors_fp(stdout); 
        return 0; 
    }
    if (!RSA_sign(NID_md5, (unsigned char*)in, strlen(in), (unsigned char*)out, (unsigned int*)plen, p_rsa))
    {
        return 0; 
    }
    RSA_free(p_rsa); 
    fclose(file); 
    return 1; 
}

int main(int argc, char**argv)
{
    char text[MSG_LEN] = "this is the data to be signed"; 
    char sign[MSG_LEN] = {}; 
    int len=0; 


    if (!rsa_sign(text, PRIKEY_FILE, sign, &len))
    {
        printf("sign error\n"); 
        return -1; 
    }
    printf("sign %lu:", strlen((char*)sign)); 
    print_hex(sign); 

    if (!rsa_verify(text, PUBKEY_FILE, sign, len))
    {
        printf("verify error\n"); 
        return -1; 
    }
    printf("verify ok\n"); 

    return 0; 
}
