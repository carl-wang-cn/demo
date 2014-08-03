```
├── aes_demo.cpp
├── openssl_rsa_demo1.cpp
├── openssl_rsa_demo2.cpp
├── openssl_rsa_demo3.cpp
└── openssl_sign_verify.cpp
```

- aes\_demo.cpp， 一个简单的aes加解密示例。
- openssl\_rsa\_demo1.cpp，在code中生成密钥，保存到两个全局变量中，然后使用pubkey加密，prikey解密
- openssl\_rsa\_demo2.cpp，运行此程序前，需要先用命令行openssl命令来生成pem格式公私钥文件，具体命令在该cpp中文件中有给出
- openssl\_rsa\_demo3.cpp，与上面demo2对应，此示例在code中生成pem格式公私钥文件，然后再使用该密钥对进行加解密操作。
- openssl\_sign\_verify.cpp，私钥签名，公钥验证 openssl\_sign\_verify.cpp，该示例需要用到demo3生成的pem格式公私钥文件

**Note**： 以上示例通过g++编译测试通过，编译需加-lcrypto来连接openssl的加密库
