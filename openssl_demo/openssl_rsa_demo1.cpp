// 在代码中生成公私密钥对

#include <openssl/rsa.h>
#include <iostream>
#include <memory.h>

const int MAX_RSA_KEY_LENGTH = 512; //密钥的最大长度是512字节

unsigned char pub_key[MAX_RSA_KEY_LENGTH] = {0};
unsigned char pri_key[MAX_RSA_KEY_LENGTH] = {0};
int pub_key_len = 0;
int pri_key_len = 0;

using namespace std;

int gen_rsakeypair(void)
{
    RSA *rsa = NULL;
    unsigned char *pt = NULL;
    unsigned char *pt2 = NULL;

    //生成RSA密钥对：
    rsa = RSA_new();
    rsa = RSA_generate_key(1024, 0x10001, NULL, NULL);

    pt = pub_key;
    pub_key_len = i2d_RSAPublicKey(rsa, &pt);

    pt2 = pri_key; // 这里如果复用上面的pt指针，在下一行就会改变pub_key_len的值，不知道为什么
    pri_key_len = i2d_RSAPrivateKey(rsa, &pt2);

    if(rsa != NULL)
    {
        RSA_free(rsa);
        rsa = NULL;
    }
    return 0;
}

//取得RSA格式的私钥：
int get_prikey(const unsigned char *pri_key, unsigned long pri_key_len, RSA **pri_rsa)
{
    const unsigned char *pt = pri_key;
    *pri_rsa = d2i_RSAPrivateKey(NULL, &pt, pri_key_len);
    if (NULL == pri_rsa)
    {
        cout << "pri_rsa == NULL!" << endl;
        return 0x22;
    }
    return 0;
}

//取得RSA格式的公钥：
int get_pubkey(const unsigned char *pub_key,unsigned long pub_keylen, RSA **pub_rsa)
{
    const unsigned char *pt = pub_key;
    *pub_rsa = d2i_RSAPublicKey(NULL, &pt, pub_keylen);
    if (NULL == pub_rsa)
    {
        cout << "pub_rsa == NULL!" << endl;
        return 0x31;
    }
    return 0;
}

//公钥加密
int enc_by_rsapubkey(RSA *rsa, unsigned char *plain_text, unsigned long plain_text_len,
        unsigned char *encrypted_text, unsigned long *encrypted_text_len)
{
    *encrypted_text_len = RSA_public_encrypt(plain_text_len, plain_text, encrypted_text, rsa, RSA_PKCS1_PADDING);
    return *encrypted_text_len;
}

//私钥解密：
int dec_by_rsaprikey(RSA *rsa, unsigned char *encrypted_text, unsigned long encrypted_text_len,
        unsigned char *plain_text, unsigned long *plain_text_len)
{
    *plain_text_len = RSA_private_decrypt(encrypted_text_len, encrypted_text, plain_text, rsa, RSA_PKCS1_PADDING);
    return *plain_text_len;
}


int main(int argc, char **argv)
{
    unsigned char plain_text[8] = {0x01, 0x03, 0x99, 0x4, 0x80, 0x65, 0x34, 0x08};
    unsigned char encrypted_text[512] = {0};
    unsigned char decrypted_text[512] = {0};
    unsigned long encrypted_len = 0;
    unsigned long decrypted_len = 0;

    gen_rsakeypair();

    RSA *p_rsa_pubkey = NULL;
    p_rsa_pubkey = RSA_new();

    get_pubkey(pub_key, pub_key_len, &p_rsa_pubkey);
    //公钥加密：
    enc_by_rsapubkey(p_rsa_pubkey, plain_text, sizeof(plain_text), encrypted_text, &encrypted_len);

    if(p_rsa_pubkey != NULL)
    {
        RSA_free(p_rsa_pubkey); 
        p_rsa_pubkey = NULL;
    }

    RSA *p_rsa_prikey = NULL;
    p_rsa_prikey = RSA_new();

    get_prikey(pri_key, pri_key_len, &p_rsa_prikey);
    //私钥解密：
    dec_by_rsaprikey(p_rsa_prikey, encrypted_text, encrypted_len, decrypted_text, &decrypted_len);
    if(p_rsa_prikey != NULL)
    {
        RSA_free(p_rsa_prikey); 
        p_rsa_prikey = NULL;
    }

    //数据对比：
    if(0 == memcmp(plain_text, decrypted_text, decrypted_len))
    {
        cout << "OK!" << endl;
    }
    else
    {
        cout << "FAILED!" << endl;
    }

    return 0;
}
