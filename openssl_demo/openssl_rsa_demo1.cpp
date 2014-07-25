#include <openssl/rsa.h>
#include <iostream>
#include <memory.h>
#define _RSA_KEY_PAIR_GENERATE_//密钥是否要生成 只需要在第一次运行时打开此宏
#define _RSA_KEY_PAIR_TOFILE_//密钥对是否要写入文件
#define  MAX_RSA_KEY_LENGTH 512 //密钥的最大长度是512字节
#define PUBKEY_ENCRYPT
#define PRIKEY_DECRYPT

static const char * PUBLIC_KEY_FILE = "pubkey.key";
static const char * PRIVATE_KEY_FILE = "prikey.key";

using namespace std;

int RsaKeyPairGen(void)
{
    RSA *rsa = NULL;

#ifdef _RSA_KEY_PAIR_GENERATE_
    //生成RSA密钥对：
    rsa = RSA_new();
    rsa = RSA_generate_key(1024, 0x10001, NULL, NULL);
#endif

    //把密钥对写入文件，以后从文件里读取
#ifdef _RSA_KEY_PAIR_TOFILE_
    unsigned char ucPubKey[MAX_RSA_KEY_LENGTH] = {0}, ucPriKey[MAX_RSA_KEY_LENGTH] = {0};
    unsigned char* pt = ucPubKey;
    int len = i2d_RSAPublicKey(rsa, &pt);

    FILE *fpubkey = NULL;
    fpubkey = fopen(PUBLIC_KEY_FILE, "wb");
    if(fpubkey == NULL)
    {
        cout << "fopen pubkey.key failed!" << endl;
        return 0x01;
    }
    fwrite(ucPubKey, 1, len, fpubkey);
    fclose(fpubkey);

    unsigned char* pt2 = ucPriKey;
    len = i2d_RSAPrivateKey(rsa,&pt2);
    FILE *fprikey = NULL;
    fprikey = fopen(PRIVATE_KEY_FILE, "wb");
    if(fprikey == NULL)
    {
        cout << "fopen prikey.key failed!" << endl;
        return 0x02;
    }
    fwrite(ucPriKey, 1, len, fprikey);
    fclose(fprikey);
#endif

    if(rsa != NULL)
    {
        RSA_free(rsa);
        rsa = NULL;
    }
    return 0;
}

//从文件里读取私钥的数据，取得RSA格式的私钥：
int GetPriKey(const unsigned char *pucPriKeyData, unsigned long KeyDataLen, RSA* *priRsa)
{
    const unsigned char *Pt = pucPriKeyData;
    *priRsa = d2i_RSAPrivateKey(NULL, &Pt, KeyDataLen);
    if(priRsa == NULL)
    {
        cout << "priRsa == NULL!" << endl;
        return 0x22;
    }
    return 0;
}

//取得RSA格式的公钥：
int GetPubKey(const unsigned char *pucPubKeyData,unsigned long KeyDataLen, RSA* *pubRsa)
{
    const unsigned char *Pt = pucPubKeyData;
    *pubRsa = d2i_RSAPublicKey(NULL, &Pt, KeyDataLen);
    if(pubRsa == NULL)
    {
        cout << "pubRsa == NULL!" << endl;
        return 0x31;
    }
    return 0;
}

//公钥加密会话密钥：
int encSessionKeybyRsaPubKey(RSA *rsa, unsigned char *ucKey, unsigned long ulKeyLen,
        unsigned char *outData, unsigned long *pulOutLen)
{
    return (*pulOutLen = RSA_public_encrypt(ulKeyLen, ucKey, outData, rsa, 1));
}

//私钥解密会话密钥：
int decSessionKeybyRsaPriKey(RSA *rsa, unsigned char *InData, unsigned long ulDataLen,
        unsigned char *ucKey, unsigned long *pulKeyLen)
{
    return (*pulKeyLen = RSA_private_decrypt(ulDataLen, InData, ucKey, rsa, 1));
}


int main(int argc, char* argv[])
{
    unsigned char ucKey[8] = {0x01, 0x03, 0x99, 0x4, 0x80, 0x65, 0x34, 0x08};
    unsigned char ucEncryptedKey[512] = {0}, ucDecryptedKey[512] = {0};
    unsigned long encrypted_len = 0, decrypted_len = 0;


#ifdef  _RSA_KEY_PAIR_GENERATE_
    RsaKeyPairGen();
#endif

    //取得公钥：
    unsigned char ucPubKey[MAX_RSA_KEY_LENGTH] = {0};

    FILE *fpubkey = NULL;
    fpubkey = fopen(PUBLIC_KEY_FILE, "rb");
    if(fpubkey == NULL)
    {
        cout << "fopen pubkey.key failed!" << endl;
        return 0x03;
    }
    fseek(fpubkey, 0, SEEK_END);
    int len_PK = ftell(fpubkey);
    fseek(fpubkey, 0, SEEK_SET);
    fread(ucPubKey, 1, len_PK, fpubkey);
    fclose(fpubkey);

#ifdef  PUBKEY_ENCRYPT
    RSA *pRsaPubKey = NULL;
    pRsaPubKey = RSA_new();

    GetPubKey(ucPubKey, len_PK, &pRsaPubKey);
    //公钥加密：
    encSessionKeybyRsaPubKey(pRsaPubKey, ucKey, sizeof(ucKey), ucEncryptedKey, &encrypted_len);

    if(pRsaPubKey != NULL)
    {
        RSA_free(pRsaPubKey); pRsaPubKey = NULL;
    }
#endif

    //取得私钥：
    unsigned char ucPriKey[MAX_RSA_KEY_LENGTH] = {0};

    FILE *fprikey = NULL;
    fprikey = fopen(PRIVATE_KEY_FILE, "rb");
    if(fprikey == NULL)
    {
        cout << "fopen prikey.key failed!" << endl;
        return 0x02;
    }
    fseek(fprikey, 0, SEEK_END);
    int len_SK = ftell(fprikey);
    fseek(fprikey, 0, SEEK_SET);
    fread(ucPriKey, 1, len_SK, fprikey);
    fclose(fprikey);

#ifdef PRIKEY_DECRYPT
    RSA *pRsaPriKey = NULL;
    pRsaPriKey = RSA_new();

    GetPriKey(ucPriKey, len_SK, &pRsaPriKey);
    //私钥解密：
    decSessionKeybyRsaPriKey(pRsaPriKey, ucEncryptedKey, encrypted_len, ucDecryptedKey, &decrypted_len);
    if(pRsaPriKey != NULL)
    {
        RSA_free(pRsaPriKey); pRsaPriKey = NULL;
    }

    //数据对比：
    if(0 == memcmp(ucKey, ucDecryptedKey, decrypted_len))
    {
        cout << "OK!" << endl;
    }
    else
    {
        cout << "FAILED!" << endl;
    }
#endif

    return 0;
}
