#include "TinyAes.h"
#include "Foundation.h"
#include <cstring>

namespace TinyTls
{

FAes::FAes(const char* KeyData)
{
    constexpr static int32 Rounds = 5;
    unsigned char Key[32], Iv[32];
    unsigned int Salt[] = { 598451, 744830 }; // 300 k / ns
    /*
     * Gen key & IV for AES 256 CBC mode. A SHA1 digest is used to hash the supplied key material.
     * nrounds is the number of times the we hash the material. More rounds are more secure but
     * slower.
     */
    const auto Length = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), (const unsigned char*)Salt,
            (const unsigned char*)KeyData, (int32)strlen(KeyData), Rounds, Key, Iv);

    check(Length == 32);
    if (Length == 32)
    {
        EncoderContext = EVP_CIPHER_CTX_new();
        DecoderContext = EVP_CIPHER_CTX_new();

        EVP_CIPHER_CTX_init(EncoderContext);
        EVP_EncryptInit_ex(EncoderContext, EVP_aes_256_cbc(), nullptr, Key, Iv);

        EVP_CIPHER_CTX_init(DecoderContext);
        EVP_DecryptInit_ex(DecoderContext, EVP_aes_256_cbc(), nullptr, Key, Iv);
    }
}

FAes::~FAes()
{
    EVP_CIPHER_CTX_free(EncoderContext);
    EVP_CIPHER_CTX_free(DecoderContext);
}

unsigned char* FAes::Encrypt(const void* Data, int32& Size) const
{
    // max Ciphertext len for a n bytes of plaintext is n + AES_BLOCK_SIZE -1 bytes
    int32 CLen = Size + AES_BLOCK_SIZE;
    int32 FLen = 0;
    auto* Ciphertext = new unsigned char[CLen];

    EVP_EncryptInit_ex(EncoderContext, nullptr, nullptr, nullptr, nullptr);
    EVP_EncryptUpdate(EncoderContext, Ciphertext, &CLen, (const unsigned char *)Data, Size);
    EVP_EncryptFinal_ex(EncoderContext, Ciphertext + CLen, &FLen);

    Size = CLen + FLen;
    return Ciphertext;
}

unsigned char* FAes::Decrypt(const void *Data, int32 &Size) const
{
    /* plaintext will always be equal to or lesser than length of ciphertext*/
    int32 PLen = Size;
    int32 FLen = 0;
    auto* Result = new unsigned char[PLen];

    EVP_DecryptInit_ex(DecoderContext, nullptr, nullptr, nullptr, nullptr);
    EVP_DecryptUpdate(DecoderContext, Result, &PLen, (const unsigned char*)Data, Size);
    EVP_DecryptFinal_ex(DecoderContext, Result + PLen, &FLen);

    Size = PLen + FLen;
    return Result;
}

}
