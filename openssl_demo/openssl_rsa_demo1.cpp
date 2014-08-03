// 在代码中生成公私密钥对

#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <iostream>
#include <memory.h>

const int MAX_RSA_KEY_LENGTH = 512; //密钥的最大长度是512字节

unsigned char *g_pub_key = NULL;
unsigned char *g_pri_key = NULL;
int g_pub_key_len = 0;
int g_pri_key_len = 0;

using namespace std;

int gen_rsakeypair(void)
{
    RSA *rsa = NULL;
    unsigned char *pt = NULL;
    unsigned char *pt2 = NULL;

    //生成RSA密钥对：
    rsa = RSA_new();
    rsa = RSA_generate_key(1024, RSA_F4, NULL, NULL);

    g_pub_key_len = i2d_RSAPublicKey(rsa, NULL); // get pubkey size
    g_pub_key = (unsigned char *)malloc(g_pub_key_len+1); // alloc memory for g_pub_key
    memset(g_pub_key, 0, g_pub_key_len+1);
    pt = g_pub_key; // must use p, as the para will change 
    g_pub_key_len = i2d_RSAPublicKey(rsa, &pt); // 提取公钥大小, 并将公钥放入buffer中

    g_pri_key_len = i2d_RSAPrivateKey(rsa, NULL); // get prikey size
    g_pri_key = (unsigned char *)malloc(g_pri_key_len+1);
    memset(g_pri_key, 0, g_pri_key_len+1);
    pt2 = g_pri_key; // 这里如果复用上面的pt指针，在下一行就会改变g_pub_key_len的值，不知道为什么
    g_pri_key_len = i2d_RSAPrivateKey(rsa, &pt2);

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
    
    // 打印私钥信息,实际上，私钥含有p、q等，所以可以推导出公钥
    printf("\n");
    printf("pri_key info:\n");
    printf("p(%d bytes):%s\n",BN_num_bytes((*pri_rsa)->p), BN_bn2hex((*pri_rsa)->p));
    printf("q(%d bytes):%s\n",BN_num_bytes((*pri_rsa)->q), BN_bn2hex((*pri_rsa)->q));
    printf("d(%d bytes):%s\n",BN_num_bytes((*pri_rsa)->d), BN_bn2hex((*pri_rsa)->d));
    printf("\n");
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

    //打印公钥对应的信息，公钥包含N和e，而私钥拥N和d
    //实际上，私钥含有p、q等，所以可以推导出公钥
    printf("\n");
    printf("pub_key info:\n");
    printf("N(%d bytes):%s\n",BN_num_bytes((*pub_rsa)->n), BN_bn2hex((*pub_rsa)->n));
    printf("e(%d bytes):%s\n",BN_num_bytes((*pub_rsa)->e), BN_bn2hex((*pub_rsa)->e));
    printf("\n");
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
    unsigned char plain_text[] = "this is the data to be encrypted";
    unsigned char encrypted_text[512] = {0};
    unsigned char decrypted_text[512] = {0};
    unsigned long encrypted_len = 0;
    unsigned long decrypted_len = 0;

    gen_rsakeypair();

    RSA *p_rsa_pubkey = NULL;
    p_rsa_pubkey = RSA_new();

    get_pubkey(g_pub_key, g_pub_key_len, &p_rsa_pubkey);
    //公钥加密：
    enc_by_rsapubkey(p_rsa_pubkey, plain_text, sizeof(plain_text), encrypted_text, &encrypted_len);
    if(p_rsa_pubkey != NULL)
    {
        RSA_free(p_rsa_pubkey); 
        p_rsa_pubkey = NULL;
    }

    RSA *p_rsa_prikey = NULL;
    p_rsa_prikey = RSA_new();

    get_prikey(g_pri_key, g_pri_key_len, &p_rsa_prikey);
    //私钥解密：
    dec_by_rsaprikey(p_rsa_prikey, encrypted_text, encrypted_len, decrypted_text, &decrypted_len);
    if(p_rsa_prikey != NULL)
    {
        RSA_free(p_rsa_prikey); 
        p_rsa_prikey = NULL;
    }

    //数据对比：
    printf("\n");
    printf("before encrypt, the data is:\nplain_text[%lu]:    %s\n", sizeof(plain_text), plain_text);
    printf("\n");
    printf("after encrypt, the data len is:%lu\n", encrypted_len);
    printf("after decrypt, the data is:\ndecrypted_text[%lu]:%s\n", decrypted_len, decrypted_text);
    printf("\n");
    if(0 == memcmp(plain_text, decrypted_text, decrypted_len))
    {
        cout << "Yes, it worked!" << endl;
    }
    else
    {
        cout << "Oh, no!" << endl;
    }

    if (g_pub_key != NULL)
    {
        free(g_pub_key);
        g_pub_key = NULL;
    }
    if (g_pri_key != NULL)
    {
        free(g_pri_key);
        g_pri_key = NULL;
    }

    return 0;
}
